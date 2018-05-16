// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeIdentity.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeLog.h"
#include "Async.h"
#include "AsyncTaskManagerJustice.h"
#include "Models/FUserCreateResponse.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
FCriticalSection Mutex;

// Refresh Task
class FUserRefreshTokenAsyncTask : public FJusticeAsyncTask
{
public:
	FUserRefreshTokenAsyncTask(FDateTime nextUpdate) :FJusticeAsyncTask(nextUpdate)	{}
	virtual void Tick()
	{
		check(!IsInGameThread() || !FPlatformProcess::SupportsMultithreading());
		JusticeIdentity::RefreshToken(nullptr);
		SetAsDone();
	}
};

class FClientRefreshTokenAsyncTask : public FJusticeAsyncTask
{
public:
	FClientRefreshTokenAsyncTask(FDateTime nextUpdate) :FJusticeAsyncTask(nextUpdate) {}
	virtual void Tick()
	{
		check(!IsInGameThread() || !FPlatformProcess::SupportsMultithreading());
		JusticeIdentity::ClientRefreshToken();
		SetAsDone();
	}
};


// Client Login Retry Task
class FClientLoginRetryTask : public FJusticeRetryTask
{
public:
	FClientLoginRetryTask(FUserLoginCompleteDelegate onComplete,
		int nextRetry = 1,
		int totalElapsedWait = 0) : FJusticeRetryTask(nextRetry, totalElapsedWait),
		OnComplete(onComplete)
	{}
	virtual void Tick()
	{
		check(!IsInGameThread() || !FPlatformProcess::SupportsMultithreading());
		UE_LOG(LogJustice, Log, TEXT("Retry Call JusticeIdentity::RegisterNewPlayer after wait for %d second"), GetLastWait());
		JusticeIdentity::ClientLogin(FUserLoginCompleteDelegate::CreateLambda([&](bool IsSucessfull, FString ErrorStr, UOAuthTokenJustice* Token) {
			if (IsSucessfull)
			{
				OnComplete.ExecuteIfBound(IsSucessfull, ErrorStr, Token);
				SetAsDone();
			}
			else
			{
				if (GetTotalElapsedWait() > 60)
				{
					// take more than 1 minutes after many retries, return failure
					OnComplete.ExecuteIfBound(false, TEXT("Request Timeout"), nullptr);
				}
				else
				{
					FClientLoginRetryTask* RetryTask = new FClientLoginRetryTask(OnComplete,
						GetLastWait() * 2, // wait more longer for next retry
						GetTotalElapsedWait() + GetLastWait());
					FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
				}

			}
		}));
	}
private:
	FUserLoginCompleteDelegate OnComplete;
};

// User Login Retry Task
class FUserLoginRetryTask : public FJusticeRetryTask
{
public:
	FUserLoginRetryTask(FString userId,
						FString password,
						FUserLoginCompleteDelegate onComplete,
						int nextRetry = 1,
						int totalElapsedWait = 0) : FJusticeRetryTask(nextRetry, totalElapsedWait),
													UserId(userId),
													Password(password),		
													OnComplete(onComplete)
	{}
	virtual void Tick()
	{
		check(!IsInGameThread() || !FPlatformProcess::SupportsMultithreading());
		UE_LOG(LogJustice, Log, TEXT("Retry Call JusticeIdentity::RegisterNewPlayer after wait for %d second"), GetLastWait());
		JusticeIdentity::UserLogin(UserId,
			Password,
			FUserLoginCompleteDelegate::CreateLambda([&](bool IsSucessfull, FString ErrorStr, UOAuthTokenJustice* Token) {
			if (IsSucessfull)
			{
				OnComplete.ExecuteIfBound(IsSucessfull, ErrorStr, Token);
				SetAsDone();
			}
			else
			{
				if (GetTotalElapsedWait() > 60)
				{
					// take more than 1 minutes after many retries, return failure
					OnComplete.ExecuteIfBound(false, TEXT("Request Timeout"), nullptr);
				}
				else
				{
					FUserLoginRetryTask* RetryTask = new FUserLoginRetryTask(UserId,
						Password,						
						OnComplete,
						GetLastWait() * 2, // wait more longer for next retry
						GetTotalElapsedWait() + GetLastWait());
					FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
				}

			}
		}));
	}
private:
	FString UserId;
	FString Password;
	FUserLoginCompleteDelegate OnComplete;
};



// Register New Player Retry Task
class FRegisterNewPlayerRetryTask : public FJusticeRetryTask
{
public:
	FRegisterNewPlayerRetryTask(FString userId, 
		FString password, 
		FString displayName, 
		FUserAuthTypeJustice authType, 
		FRegisterPlayerCompleteDelegate onComplete, 
		int nextRetry = 1,
		int totalElapsedWait = 0) 
			:FJusticeRetryTask(nextRetry, totalElapsedWait), 
			 UserId(userId), 
			 Password(password), 
			 DisplayName(displayName), 
			 AuthType(authType), 
			 OnComplete(onComplete)
	{}
	virtual void Tick()
	{
		check(!IsInGameThread() || !FPlatformProcess::SupportsMultithreading());		
		UE_LOG(LogJustice, Log, TEXT("Retry Call JusticeIdentity::RegisterNewPlayer after wait for %d second"), GetLastWait());
		JusticeIdentity::RegisterNewPlayer(UserId, 
										   Password, 
										   DisplayName, 
										   AuthType, 
										   FRegisterPlayerCompleteDelegate::CreateLambda([&](bool IsSuccess, 
																							 FString ErrorStr, 
																							 UUserCreateResponse* Response) {
			if (IsSuccess)
			{
				OnComplete.ExecuteIfBound(IsSuccess, ErrorStr, Response);
				SetAsDone();
			}
			else
			{
				if (GetTotalElapsedWait() > 60)
				{
					// take more than 1 minutes after many retries, return failure
					OnComplete.ExecuteIfBound(false, TEXT("Request Timeout"), nullptr);
				}
				else
				{					
					FRegisterNewPlayerRetryTask* RetryTask = new FRegisterNewPlayerRetryTask(UserId, 
																							 Password, 
																							 DisplayName, 
																							 AuthType, 
																						     OnComplete, 
																						     GetLastWait() * 2, // wait more longer for next retry
																							 GetTotalElapsedWait() + GetLastWait());
					FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
				}

			}
		}));	
	}
private:
	FString UserId;
	FString Password;
	FString DisplayName;
	FUserAuthTypeJustice AuthType;
	FRegisterPlayerCompleteDelegate OnComplete;
};


// Forgot Password Retry Task
class FForgotPasswordRetryTask : public FJusticeRetryTask
{
public:
	FForgotPasswordRetryTask(FString loginId,
		FForgotPasswordCompleteDelegate onComplete,
		int nextRetry = 1,
		int totalElapsedWait = 0)
		:FJusticeRetryTask(nextRetry, totalElapsedWait),		
		LoginId(loginId),
		OnComplete(onComplete)
	{}
	virtual void Tick()
	{
		check(!IsInGameThread() || !FPlatformProcess::SupportsMultithreading());
		UE_LOG(LogJustice, Log, TEXT("Retry Call JusticeIdentity::RegisterNewPlayer after wait for %d second"), GetLastWait());
		JusticeIdentity::ForgotPassword(LoginId,
			FForgotPasswordCompleteDelegate::CreateLambda([&](bool IsSuccess, FString ErrorStr) {
			if (IsSuccess)
			{
				OnComplete.ExecuteIfBound(IsSuccess, ErrorStr);
				SetAsDone();
			}
			else
			{
				if (GetTotalElapsedWait() > 60)
				{
					// take more than 1 minutes after many retries, return failure
					OnComplete.ExecuteIfBound(false, TEXT("Request Timeout"));
				}
				else
				{
					FForgotPasswordRetryTask* RetryTask = new FForgotPasswordRetryTask(LoginId,
						OnComplete,
						GetLastWait() * 2, // wait more longer for next retry
						GetTotalElapsedWait() + GetLastWait());
					FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
				}

			}
		}));
	}
private:
	FString LoginId;
	FForgotPasswordCompleteDelegate OnComplete;
};



// VerifyNewPlayer retry task

class FVerifyNewPlayerRetryTask : public FJusticeRetryTask
{
public:
	FVerifyNewPlayerRetryTask(FString userId,
		FString verificationCode,
		FUserAuthTypeJustice authType,
		FVerifyNewPlayerCompleteDelegate onComplete,
		int nextRetry = 1,
		int totalElapsedWait = 0)
		:FJusticeRetryTask(nextRetry, totalElapsedWait),
		UserId(userId),
		VerificationCode(verificationCode),
		AuthType(authType),
		OnComplete(onComplete)
	{}
	virtual void Tick()
	{
		check(!IsInGameThread() || !FPlatformProcess::SupportsMultithreading());
		UE_LOG(LogJustice, Log, TEXT("Retry Call JusticeIdentity::RegisterNewPlayer after wait for %d second"), GetLastWait());
		JusticeIdentity::VerifyNewPlayer(UserId, VerificationCode, AuthType,
			FVerifyNewPlayerCompleteDelegate::CreateLambda([&](bool IsSuccess, FString ErrorStr) {
			if (IsSuccess)
			{
				OnComplete.ExecuteIfBound(IsSuccess, ErrorStr);
				SetAsDone();
			}
			else
			{
				if (GetTotalElapsedWait() > 60)
				{
					// take more than 1 minutes after many retries, return failure
					OnComplete.ExecuteIfBound(false, TEXT("Request Timeout"));
				}
				else
				{
					FVerifyNewPlayerRetryTask* RetryTask = new FVerifyNewPlayerRetryTask(UserId, VerificationCode, AuthType,
						OnComplete,
						GetLastWait() * 2, // wait more longer for next retry
						GetTotalElapsedWait() + GetLastWait());
					FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
				}

			}
		}));
	}
private:
	FString UserId;
	FString VerificationCode;
	FUserAuthTypeJustice AuthType;
	FVerifyNewPlayerCompleteDelegate OnComplete;
};


void JusticeIdentity::Login(FString LoginId, FString Password, FGrantTypeJustice GrantType, FUserLoginCompleteDelegate OnComplete)
{
	Mutex.Lock();
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString ClientID = FJusticeSDKModule::Get().ClientID;
	FString ClientSecret = FJusticeSDKModule::Get().ClientSecret;
	FJusticeSDKModule::Get().LoginId = LoginId;
	FJusticeSDKModule::Get().Password = Password;

	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = "";
	if (GrantType == FGrantTypeJustice::PasswordGrant)
	{		
		Request->SetURL(FString::Printf(TEXT("%s/iam/oauth/token"), *BaseURL));
 		Grant = FString::Printf(TEXT("grant_type=password&username=%s&password=%s"), *FGenericPlatformHttp::UrlEncode(LoginId), *FGenericPlatformHttp::UrlEncode(Password));
		Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnLoginComplete, RequestTrace, OnComplete, LoginId, Password);
	}
	else if (GrantType == FGrantTypeJustice::RefreshGrant)
	{		
		Request->SetURL(FString::Printf(TEXT("%s/iam/oauth/token"), *BaseURL));
		FString RefreshToken = FJusticeSDKModule::Get().UserToken->RefreshToken;
		Grant = FString::Printf(TEXT("grant_type=refresh_token&refresh_token=%s"),*FGenericPlatformHttp::UrlEncode(RefreshToken));
		Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnUserRefreshComplete, RequestTrace, OnComplete);
	}
	else if (GrantType == FGrantTypeJustice::ClientCredentialGrant)
	{
		Request->SetURL(FString::Printf(TEXT("%s/iam/oauth/token"), *BaseURL));		
		Grant = FString::Printf(TEXT("grant_type=client_credentials"));
		Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnClientCredentialComplete, RequestTrace, OnComplete);			
	}
	else if (GrantType == FGrantTypeJustice::Device)
	{		
		Request->SetURL(FString::Printf(TEXT("%s/iam/oauth/namespaces/%s/platforms/device/token"), *BaseURL, *Namespace));
		FString deviceID = FGenericPlatformMisc::GetDeviceId();
		check(!deviceID.IsEmpty() && "Cannot get Device ID");
		Grant = FString::Printf(TEXT("device_id=%s"), *deviceID);
		Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnLoginComplete, RequestTrace, OnComplete, LoginId, Password);
	}

	Request->SetContentAsString(Grant);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("Request Failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeIdentity::UserLogin failed. ErrorString=%s, XrayID=%s, ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
	}
	Mutex.Unlock();
}

void JusticeIdentity::OnLoginComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUserLoginCompleteDelegate OnComplete, FString UserId, FString Password)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Ok:
		{
			FString ResponseStr = Response->GetContentAsString();

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if (FJusticeSDKModule::Get().UserParseJson(JsonObject))
				{
						FUserRefreshTokenAsyncTask* NewTask = new FUserRefreshTokenAsyncTask(FJusticeSDKModule::Get().UserToken->NextTokenRefreshUtc);
						FJusticeSDKModule::Get().AsyncTaskManager->AddToRefreshQueue(NewTask);
						UOAuthTokenJustice* newToken = NewObject<UOAuthTokenJustice>();
						newToken->FromParent(FJusticeSDKModule::Get().UserToken);
						OnComplete.ExecuteIfBound(true, TEXT(""), newToken);
				}
				else
				{
					ErrorStr = TEXT("unable to deserlize response from json object");
				}
			}
			else
			{
				ErrorStr = TEXT("unable to deserlize response from server");
			}
			break;
		}
		
		case EHttpResponseCodes::Denied:
			ErrorStr = FString::Printf(TEXT("Client authentication failed, Response=%s"), *Response->GetContentAsString());
			break;
		case EHttpResponseCodes::RequestTimeout:		
		case EHttpResponseCodes::ServerError:
		case EHttpResponseCodes::ServiceUnavail:
		case EHttpResponseCodes::GatewayTimeout:
		{
			FUserLoginRetryTask* RetryTask = new FUserLoginRetryTask(UserId,Password,OnComplete);
			FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
			return;
		}

		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d  Response=%s"), Response->GetResponseCode(), *Response->GetContentAsString());
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Log, TEXT("Token password grant failed. User=%s Error=%s %s %s ReqTime=%.3f"),
			*FJusticeSDKModule::Get().UserToken->UserId, *ErrorStr, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		return;
	}

	UE_LOG(LogJustice, Log, TEXT("Token password grant successful. UserId=%s %s %s ReqTime=%.3f"),
		*FJusticeSDKModule::Get().UserToken->UserId, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
}

void JusticeIdentity::OnUserRefreshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUserLoginCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Ok:
		{
			FString ResponseStr = Response->GetContentAsString();
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if (FJusticeSDKModule::Get().UserParseJson(JsonObject))
				{
					FUserRefreshTokenAsyncTask* NewTask = new FUserRefreshTokenAsyncTask(FJusticeSDKModule::Get().UserToken->NextTokenRefreshUtc);
					FJusticeSDKModule::Get().AsyncTaskManager->AddToRefreshQueue(NewTask);

					UOAuthTokenJustice* newToken = NewObject<UOAuthTokenJustice>();
					newToken->FromParent(FJusticeSDKModule::Get().UserToken);
					OnComplete.ExecuteIfBound(true, TEXT(""), newToken);
				}
				else
				{
					ErrorStr = TEXT("unable to deserlize response from json object");
				}
			}
			else
			{
				ErrorStr = TEXT("unable to deserlize response from server");
			}
			break;
		}		
		default:
		{
			FJusticeSDKModule::Get().UserToken->ScheduleBackoffRefresh();
			FUserRefreshTokenAsyncTask* NewTask = new FUserRefreshTokenAsyncTask(FJusticeSDKModule::Get().UserToken->NextTokenRefreshUtc);
			FJusticeSDKModule::Get().AsyncTaskManager->AddToRefreshQueue(NewTask);
			ErrorStr = FString::Printf(TEXT("Unexpected Response Code=%d,  Response=%s, Start Backoff refresh !"), Response->GetResponseCode(), *Response->GetContentAsString());
		}
			
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Log, TEXT("Refresh grant failed. User=%s Error=%s %s %s ReqTime=%.3f"),
			*FJusticeSDKModule::Get().UserToken->UserId, *ErrorStr, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
		return;
	}

	UE_LOG(LogJustice, Log, TEXT("Refresh grant successful. UserId=%s %s %s ReqTime=%.3f"),
		*FJusticeSDKModule::Get().UserToken->UserId, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
}


void JusticeIdentity::OnClientRefreshComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Ok:
		{
			FString ResponseStr = Response->GetContentAsString();

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if (FJusticeSDKModule::Get().GameClientParseJson(JsonObject))
				{
					FClientRefreshTokenAsyncTask* NewTask = new FClientRefreshTokenAsyncTask(FJusticeSDKModule::Get().GameClientToken->NextTokenRefreshUtc);
					FJusticeSDKModule::Get().AsyncTaskManager->AddToRefreshQueue(NewTask);
					UE_LOG(LogJustice, Log, TEXT("OnClientRefreshComplete Succees, New token received !"));
				}
				else
				{
					ErrorStr = TEXT("unable to deserlize response from json object");
				}
			}
			else
			{
				ErrorStr = TEXT("unable to deserlize response from server");
			}
		}
		break;
		default:
		{
			FJusticeSDKModule::Get().GameClientToken->ScheduleBackoffRefresh();
			FUserRefreshTokenAsyncTask* NewTask = new FUserRefreshTokenAsyncTask(FJusticeSDKModule::Get().GameClientToken->NextTokenRefreshUtc);
			FJusticeSDKModule::Get().AsyncTaskManager->AddToRefreshQueue(NewTask);
			ErrorStr = FString::Printf(TEXT("Unexpcted Response Code=%d,  Response=%s, Start Backoff refresh !"), Response->GetResponseCode(), *Response->GetContentAsString());
		}
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Log, TEXT("OnClientRefreshComplete failed. User=%s Error=%s %s %s ReqTime=%.3f"),
			*FJusticeSDKModule::Get().UserToken->UserId, *ErrorStr, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());		
		return;
	}

	UE_LOG(LogJustice, Log, TEXT("OnClientRefreshComplete successful. UserId=%s %s %s ReqTime=%.3f"),
		*FJusticeSDKModule::Get().UserToken->UserId, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
}



void JusticeIdentity::OnClientCredentialComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUserLoginCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Ok:
		{
			FString ResponseStr = Response->GetContentAsString();

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if(FJusticeSDKModule::Get().GameClientParseJson(JsonObject))
				{
					UOAuthTokenJustice* newToken = NewObject<UOAuthTokenJustice>();
					newToken->FromParent(FJusticeSDKModule::Get().GameClientToken);
					OnComplete.ExecuteIfBound(true, TEXT(""), newToken);
				}
				else
				{
					ErrorStr = TEXT("unable to deserlize response from json object");
				}
			}
			else
			{
				ErrorStr = TEXT("unable to deserlize response from server");
			}
			break;
		}
		case EHttpResponseCodes::Denied:
			ErrorStr = FString::Printf(TEXT("Client authentication failed, Response=%s"), *Response->GetContentAsString());
			break;

		case EHttpResponseCodes::RequestTimeout:
		case EHttpResponseCodes::ServerError:
		case EHttpResponseCodes::ServiceUnavail:
		case EHttpResponseCodes::GatewayTimeout:
		{
			FClientLoginRetryTask* RetryTask = new FClientLoginRetryTask(OnComplete);
			FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
			return;

			return;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
			break;
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("Game Client Credential failed. Error=%s XRay=%s ReqTime=%.3f"),
			*ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());

		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);		
	}	
}

void JusticeIdentity::ForgotPassword(FString LoginId, FForgotPasswordCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/forgotPassword"), *BaseURL, *Namespace));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(FString::Printf(TEXT("{	\"LoginID\": \"%s\"}"), *LoginId));
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnForgotPasswordComplete, RequestTrace, OnComplete, LoginId);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	UE_LOG(LogJustice, VeryVerbose, TEXT("UFJusticeComponent::ForgotPassword Sucessful, XRayID= %s"), *RequestTrace->ToString());
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UFJusticeComponent::ForgotPassword failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
	}
}

void JusticeIdentity::OnForgotPasswordComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FForgotPasswordCompleteDelegate OnComplete, FString LoginId)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::NoContent:
		{
			UE_LOG(LogJustice, Log, TEXT("OnForgotPasswordComplete receive success response "));
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;
		}
		case EHttpResponseCodes::BadRequest:
		{
			ErrorStr = FString::Printf(TEXT("Expected Error: Invalid Request. Code=%d"), Response->GetResponseCode());
			break;
		}
		case EHttpResponseCodes::Forbidden:
		{
			ErrorStr = FString::Printf(TEXT("Expected Error: Forbidden. Code=%d"), Response->GetResponseCode());
			break;
		}
		case EHttpResponseCodes::NotFound:
		{
			ErrorStr = FString::Printf(TEXT("Expected Error: Data not found. Code=%d"), Response->GetResponseCode());
			break;
		}
		case EHttpResponseCodes::Denied:
			JusticeIdentity::RefreshToken(FUserLoginCompleteDelegate::CreateLambda([OnComplete, LoginId, &ErrorStr](bool IsSuccess,
				FString ErrorStr,
				UOAuthTokenJustice* Token) {
				if (IsSuccess)
				{
					if (Token->Bans.Num() > 0)
					{
						FString bansList = FString::Join(Token->Bans, TEXT(","));
						ErrorStr = FString::Printf(TEXT("You got banned, Ban List=%s"), *bansList);
					}
					else
					{
						FForgotPasswordRetryTask* RetryTask = new FForgotPasswordRetryTask(LoginId, OnComplete);
						FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
						return;
					}
				}
				else
				{
					//add to queue: refresh token, if success then retry (register new player)
					FForgotPasswordRetryTask* RetryTask = new FForgotPasswordRetryTask(LoginId, OnComplete);
					FRefreshTokenRetryTask* RefreshTokenTask = new FRefreshTokenRetryTask(RetryTask);
					FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RefreshTokenTask);
					return;
				}
			}));
			break;
		case EHttpResponseCodes::RequestTimeout:
		case EHttpResponseCodes::ServerError:
		case EHttpResponseCodes::ServiceUnavail:
		case EHttpResponseCodes::GatewayTimeout:
		{
			//retry
			JusticeIdentity::ForgotPassword(LoginId, OnComplete);
			return;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
			break;
		}				
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnForgotPasswordComplete Error=%s ReqTime=%.3f"),*ErrorStr, Request->GetElapsedTime());		
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::ResetPassword(FString UserId, FString VerificationCode, FString NewPassword, FResetPasswordCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	ResetPasswordRequest ResetPasswordRequest;
	ResetPasswordRequest.Code = VerificationCode;
	ResetPasswordRequest.LoginID = UserId;
	ResetPasswordRequest.NewPassword = NewPassword;

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/resetPassword"), *BaseURL, *Namespace));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(ResetPasswordRequest.ToJson());
	UE_LOG(LogJustice, Log, TEXT("Attemp to call JusticeIdentity::ResetPassword: %s"), *Request->GetURL());

	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnResetPasswordComplete, RequestTrace, OnComplete, UserId, VerificationCode, NewPassword);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	UE_LOG(LogJustice, VeryVerbose, TEXT("UFJusticeComponent::ResetPassword Sucessful, XRayID= %s"), *RequestTrace->ToString());

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UFJusticeComponent::ResetPassword failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
	}
}

void JusticeIdentity::OnResetPasswordComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FResetPasswordCompleteDelegate OnComplete, FString UserId, FString VerificationCode, FString NewPassword)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::NoContent:
		{
			UE_LOG(LogJustice, Log, TEXT("OnResetPasswordComplete receive success response "));
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;
		}
		case EHttpResponseCodes::BadRequest:
		{
			ErrorStr = FString::Printf(TEXT("Expected Error: Invalid Request. Code=%d"), Response->GetResponseCode());
			break;
		}
		case EHttpResponseCodes::Forbidden:
		{
			ErrorStr = FString::Printf(TEXT("Expected Error: Forbidden. Code=%d"), Response->GetResponseCode());
			break;
		}
		case EHttpResponseCodes::NotFound:
		{
			ErrorStr = FString::Printf(TEXT("Expected Error: Data not found. Code=%d"), Response->GetResponseCode());
			break;
		}
		case EHttpResponseCodes::Denied:



		case EHttpResponseCodes::RequestTimeout:
		case EHttpResponseCodes::ServerError:
		case EHttpResponseCodes::ServiceUnavail:
		case EHttpResponseCodes::GatewayTimeout:
		{
			ErrorStr = FString::Printf(TEXT("You have to wait 15 minutes before request another Reset Password. Response Code=%d"), Response->GetResponseCode());
			break;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
			break;
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnResetPasswordComplete Error=%s ReqTime=%.3f"), *ErrorStr, Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::UserLogin(FString LoginId, FString Password, FUserLoginCompleteDelegate OnComplete)
{
	JusticeIdentity::Login(LoginId, Password, FGrantTypeJustice::PasswordGrant, OnComplete);
}

void JusticeIdentity::UserLogout(FUserLogoutCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());	
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString ClientID = FJusticeSDKModule::Get().ClientID;
	FString ClientSecret = FJusticeSDKModule::Get().ClientSecret;
	FString Grant = FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(FJusticeSDKModule::Get().UserToken->AccessToken));	

	Request->SetURL(FString::Printf(TEXT("%s/iam/oauth/revoke/token"), *BaseURL));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(Grant);
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnLogoutComplete, RequestTrace, OnComplete);

	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeIdentity::UserLogout failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}


	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FString ThePath = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir());

	
	FString LocalStorageDir = FString::Printf(TEXT("%swebcache/Local Storage/"), *ThePath);
	FString CookiesFilePath = FString::Printf(TEXT("%swebcache/Cookies"), *ThePath);
	FString CookieJournalFilePath = FString::Printf(TEXT("%swebcache/Cookies-journal"), *ThePath);

	bool result = PlatformFile.DeleteFile(*CookiesFilePath);
	UE_LOG(LogJustice, Log, TEXT("delete cookies result : %d"), result);

	result = PlatformFile.DeleteFile(*CookieJournalFilePath);
	UE_LOG(LogJustice, Log, TEXT("delete cookies journal result : %d"), result);


	FJsonSerializableArray LocalStorageFiles;
	PlatformFile.FindFiles(LocalStorageFiles, *LocalStorageDir, TEXT(""));
	for (int i = 0; i < LocalStorageFiles.Num(); i++)
	{
		UE_LOG(LogJustice, Log, TEXT("Attemp to delete File: %s"), *LocalStorageFiles[i]);
		result = PlatformFile.DeleteFile(*LocalStorageFiles[i]);
		UE_LOG(LogJustice, Log, TEXT("delete result : %d"), result);
	}

	UE_LOG(LogJustice, Log, TEXT("Attemp to delete Folder and entire content: %s"), *ThePath);
	result = PlatformFile.DeleteDirectory(*ThePath);
	UE_LOG(LogJustice, Log, TEXT("delete Folder result : %d"), result);
}

void JusticeIdentity::DeviceLogin(FUserLoginCompleteDelegate OnComplete)
{
	JusticeIdentity::Login(TEXT(""), TEXT(""), FGrantTypeJustice::Device, OnComplete);
}

void JusticeIdentity::RefreshToken(FUserLoginCompleteDelegate OnComplete)
{
	JusticeIdentity::Login(TEXT(""), TEXT(""), FGrantTypeJustice::RefreshGrant, OnComplete);
}

void JusticeIdentity::Relogin(FUserLoginCompleteDelegate OnComplete)
{
	JusticeIdentity::UserLogin(	FJusticeSDKModule::Get().LoginId,
								FJusticeSDKModule::Get().Password,							
								OnComplete);
}

void JusticeIdentity::OnLogoutComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUserLogoutCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
			case EHttpResponseCodes::Ok:
			{
				UE_LOG(LogJustice, Log, TEXT("Token logout receive success response "));
				break;
			}
			default:
			{
				// we don't retry logout
				ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
			}
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Token logout failed. User=%s Error=%s %s %s ReqTime=%.3f"),
			*FJusticeSDKModule::Get().UserToken->UserId, *ErrorStr, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
		return;
	}
	OnComplete.ExecuteIfBound(true, TEXT(""));
}

void JusticeIdentity::ClientLogout()
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString ClientID = FJusticeSDKModule::Get().ClientID;
	FString ClientSecret = FJusticeSDKModule::Get().ClientSecret;
	FString Grant = FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(FJusticeSDKModule::Get().GameClientToken->AccessToken));	

	Request->SetURL(FString::Printf(TEXT("%s/iam/oauth/revoke/token"), *BaseURL));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(Grant);
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnClientLogoutComplete, RequestTrace);

	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeIdentity::ClientLogout failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
	}
}

void JusticeIdentity::ClientRefreshToken()
{
	Mutex.Lock();
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString ClientID = FJusticeSDKModule::Get().ClientID;
	FString ClientSecret = FJusticeSDKModule::Get().ClientSecret;
	
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	Request->SetURL(FString::Printf(TEXT("%s/iam/oauth/token"), *BaseURL));
	FString RefreshToken = FJusticeSDKModule::Get().GameClientToken->RefreshToken;
	FString Grant = FString::Printf(TEXT("grant_type=refresh_token&refresh_token=%s"), *FGenericPlatformHttp::UrlEncode(RefreshToken));
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnClientRefreshComplete, RequestTrace);
	Request->SetContentAsString(Grant);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeIdentity::ClientRefreshToken failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());		
	}
	Mutex.Unlock();
}


void JusticeIdentity::SetRefreshToken(FString RefreshToken)
{
	FJusticeSDKModule::Get().UserToken->RefreshToken = RefreshToken;
}

void JusticeIdentity::OnClientLogoutComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Ok: 
			UE_LOG(LogJustice, Log, TEXT("Client logout receive success response "));
			FJusticeSDKModule::Get().GameClientToken = new OAuthTokenJustice;
			FJusticeSDKModule::Get().AsyncTaskManager->ClearRefreshQueue();		
			break;
		default:		
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Client  logout failed. User=%s Error=%s %s %s ReqTime=%.3f"),
			*FJusticeSDKModule::Get().UserToken->UserId, *ErrorStr, *FJusticeSDKModule::Get().UserToken->GetRefreshStr(), *RequestTrace->ToString(), Request->GetElapsedTime());
		return;
	}
}

void JusticeIdentity::RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, FUserAuthTypeJustice AuthType, FRegisterPlayerCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;	

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users"), *BaseURL, *Namespace));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FUserCreateRequest NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.LoginId = UserId;
	if (AuthType == Email)
	{
		NewUserRequest.AuthType = TEXT("EMAILPASSWD");
	}
	else
	{
		NewUserRequest.AuthType = TEXT("PHONEPASSWD");
	}
	
	Request->SetContentAsString(NewUserRequest.ToJson());
	Request->OnProcessRequestComplete().BindStatic(JusticeIdentity::OnRegisterNewPlayerComplete, RequestTrace, OnComplete, UserId, Password, DisplayName, AuthType);

	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UFJusticeComponent::RegisterNewPlayer failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
	}
}

void JusticeIdentity::OnRegisterNewPlayerComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FRegisterPlayerCompleteDelegate OnComplete, FString UserId, FString Password, FString DisplayName, FUserAuthTypeJustice AuthType)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Created:
		{
			UE_LOG(LogJustice, Log, TEXT("OnRegisterNewPlayerComplete : Entity Created"));
			//parse json
			FString ResponseStr = Response->GetContentAsString();
			FUserCreateResponse UserCreateResponse;
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{
				if (UserCreateResponse.FromJson(JsonObject))
				{
					UUserCreateResponse* pUserCreateResponse = NewObject<UUserCreateResponse>();
					pUserCreateResponse->LoadFromStruct(UserCreateResponse);
					ReissueVerificationCode(pUserCreateResponse->UserId, pUserCreateResponse->LoginId, FVerifyNewPlayerCompleteDelegate::CreateLambda([OnComplete, pUserCreateResponse](bool IsSuccess, FString ErrorStr) {
						if (IsSuccess)
						{
							OnComplete.ExecuteIfBound(true, TEXT(""), pUserCreateResponse);
						}
						else
						{
							OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
						}					
					}));
				}
				else
				{
					OnComplete.ExecuteIfBound(false, TEXT("Cannot serialize to UserCreateResponse"), nullptr);
				}
			}
			else
			{
				OnComplete.ExecuteIfBound(false, TEXT("Invalid json response"), nullptr);
			}
			break;
		}
		case EHttpResponseCodes::BadRequest:
			ErrorStr = TEXT("Invalid Request");
			break;
		case EHttpResponseCodes::Forbidden:
			ErrorStr = TEXT("Forbidden");
			break;
		case EHttpResponseCodes::Conflict:
			ErrorStr = TEXT("Conflict");
			break;
		case EHttpResponseCodes::Denied:
			JusticeIdentity::RefreshToken(FUserLoginCompleteDelegate::CreateLambda([OnComplete, UserId, Password, DisplayName, AuthType, &ErrorStr](bool IsSuccess,
																						  FString ErrorStr, 
																						  UOAuthTokenJustice* Token) {
				if (IsSuccess)
				{
					if (Token->Bans.Num() > 0)
					{
						FString bansList = FString::Join(Token->Bans, TEXT(","));
						ErrorStr = FString::Printf(TEXT("You got banned, Ban List=%s"), *bansList);
					}
					else
					{
						FRegisterNewPlayerRetryTask* RetryTask = new FRegisterNewPlayerRetryTask(UserId, Password, DisplayName, AuthType, OnComplete);
						FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
						return;
					}
				}
				else
				{
					//add to queue: refresh token, if success then retry (register new player)
					FRegisterNewPlayerRetryTask* RetryTask = new FRegisterNewPlayerRetryTask(UserId, Password, DisplayName, AuthType, OnComplete);
					FRefreshTokenRetryTask* RefreshTokenTask = new FRefreshTokenRetryTask(RetryTask);
					FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RefreshTokenTask);
					return;					
				}
			}));
			break;
		case EHttpResponseCodes::RequestTimeout:
		case EHttpResponseCodes::ServerError:
		case EHttpResponseCodes::ServiceUnavail:
		case EHttpResponseCodes::GatewayTimeout:
		{
			FRegisterNewPlayerRetryTask* RetryTask = new FRegisterNewPlayerRetryTask(UserId, Password, DisplayName, AuthType, OnComplete);
			FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
			return;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnRegisterNewPlayerComplete. Error Message: %s XRay: %s ReqTime: %.3f"),
			*ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());

		OnComplete.ExecuteIfBound(false, ErrorStr, nullptr);
	}
}

void JusticeIdentity::VerifyNewPlayer(FString UserId, FString VerificationCode, FUserAuthTypeJustice AuthType, FVerifyNewPlayerCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString ContactType;
	if (AuthType == Email)
	{
		ContactType = TEXT("email");
	}
	else if (AuthType == Phone)
	{
		ContactType = TEXT("phone");
	}	
	FString Payload = FString::Printf(TEXT("{ \"Code\": \"%s\",\"ContactType\":\"%s\"}"), *VerificationCode, *ContactType);
	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verification"), *BaseURL, *Namespace, *UserId));	
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());	
	Request->SetContentAsString(Payload);
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnVerifyNewPlayerComplete, RequestTrace, OnComplete, UserId, VerificationCode, AuthType);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("VerifyNewPlayer failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}


void JusticeIdentity::OnVerifyNewPlayerComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FVerifyNewPlayerCompleteDelegate OnComplete, FString UserId, FString VerificationCode, FUserAuthTypeJustice AuthType)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::NoContent:
			UE_LOG(LogJustice, VeryVerbose, TEXT("TokenVerifyNewPlayerComplete : Player Activated"));
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;
		case EHttpResponseCodes::BadRequest:
			ErrorStr = TEXT("Invalid Request");
			break;
		case EHttpResponseCodes::Forbidden:
			ErrorStr = TEXT("Forbidden");
			break;
		case EHttpResponseCodes::Denied:
			JusticeIdentity::RefreshToken(FUserLoginCompleteDelegate::CreateLambda([OnComplete, UserId, VerificationCode, AuthType, &ErrorStr](bool IsSuccess,
				FString ErrorStr,
				UOAuthTokenJustice* Token) {
				if (IsSuccess)
				{
					if (Token->Bans.Num() > 0)
					{
						FString bansList = FString::Join(Token->Bans, TEXT(","));
						ErrorStr = FString::Printf(TEXT("You got banned, Ban List=%s"), *bansList);
					}
					else
					{
						FVerifyNewPlayerRetryTask* RetryTask = new FVerifyNewPlayerRetryTask(UserId, VerificationCode, AuthType, OnComplete);
						FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
						return;
					}
				}
				else
				{
					//add to queue: refresh token, if success then retry (register new player)
					FVerifyNewPlayerRetryTask* RetryTask = new FVerifyNewPlayerRetryTask(UserId, VerificationCode, AuthType, OnComplete);
					FRefreshTokenRetryTask* RefreshTokenTask = new FRefreshTokenRetryTask(RetryTask);
					FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RefreshTokenTask);
					return;
				}
			}));

			break;
		case EHttpResponseCodes::RequestTimeout:
		case EHttpResponseCodes::ServerError:
		case EHttpResponseCodes::ServiceUnavail:
		case EHttpResponseCodes::GatewayTimeout:
		{
			JusticeIdentity::VerifyNewPlayer(UserId, VerificationCode, AuthType, OnComplete);
			return;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("VerifyNewPlayerCompleteDelegate. Error=%s XRay=%s ReqTime=%.3f"),
			*ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::ReissueVerificationCode(FString UserId, FString LoginId, FVerifyNewPlayerCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString Payload = FString::Printf(TEXT("{ \"LoginID\": \"%s\"}"), *LoginId);	

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verificationcode"), *BaseURL, *Namespace, *UserId));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(Payload);
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnReissueVerificationCodeComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("ReissueVerificationCode failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}


void JusticeIdentity::OnReissueVerificationCodeComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FVerifyNewPlayerCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::NoContent:
			UE_LOG(LogJustice, VeryVerbose, TEXT("OnReissueVerificationCodeComplete : Operation succeeded"));
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;
		case EHttpResponseCodes::BadRequest:
			ErrorStr = TEXT("Invalid Request");
			break;
		case EHttpResponseCodes::Forbidden:
			ErrorStr = TEXT("Forbidden");
			break;
		case EHttpResponseCodes::Denied:


		case EHttpResponseCodes::RequestTimeout:
		case EHttpResponseCodes::ServerError:
		case EHttpResponseCodes::ServiceUnavail:
		case EHttpResponseCodes::GatewayTimeout:
		{
			ErrorStr = FString::Printf(TEXT("You have to wait 15 minutes before request another ReissueVerificationCode. Response Code=%d"), Response->GetResponseCode());
			break;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnReissueVerificationCodeComplete. Error:%s XRay: %s ReqTime: %.3f"),
			*ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::GetLinkedPlatform(FGetLinkedPlatformCompleteDelegate OnComplete) 
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString UserId = GetUserId();

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms"), *BaseURL, *Namespace, *UserId));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnGetLinkedPlatformComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("GetLinkedPlatform failed. Error=%s XrayID=%s ReqTime=%.3f"));
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<LinkedPlatform>());
	}
}

void JusticeIdentity::OnGetLinkedPlatformComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetLinkedPlatformCompleteDelegate OnComplete)
{
	check(&OnComplete != nullptr);
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid()) 
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Ok:
		{
			FString ResponseStr = Response->GetContentAsString();
			UE_LOG(LogJustice, Log, TEXT("OnGetLinkedPlatformComplete : %s"), *ResponseStr);

			TSharedPtr<FJsonValue> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			TArray<LinkedPlatform> Result;

			if (ResponseStr.Contains("null")) {
				OnComplete.ExecuteIfBound(true, ErrorStr, TArray<LinkedPlatform>());
			}
			else
			if (FJsonSerializer::Deserialize(Reader, JsonObject)) 
			{
				if (JsonObject.IsValid()) 
				{
					TArray< TSharedPtr<FJsonValue> >JsonArray = JsonObject->AsArray();
					for (int platform = 0; platform != JsonArray.Num(); platform++)
					{
						LinkedPlatform linkedPlatform;
						if (linkedPlatform.FromJson(JsonArray[platform]->AsObject()))
						{
							Result.Add(linkedPlatform);
						}
					}
					OnComplete.ExecuteIfBound(true, ErrorStr, Result);
				}
			}
			else
			{
				ErrorStr = TEXT("unable to deserialize response from server");
			}
		}
		break;

		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("On Get User Linked Platforms Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, TArray<LinkedPlatform>());
		return;
	}
}

void JusticeIdentity::LinkPlatform(FString PlatformId, FString Ticket, FLinkPlatformCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString UserId = GetUserId();
	FString TicketForm = FString::Printf(TEXT("ticket=%s"), *Ticket);
	
	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/%s/link"), *BaseURL, *Namespace, *UserId, *PlatformId));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(TicketForm);
	
	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnLinkPlatformComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("LinkPlatform failed. Error=%s XrayID=%s ReqTime%.3f"));
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::OnLinkPlatformComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FLinkPlatformCompleteDelegate OnComplete)
{
	check(&OnComplete != nullptr);
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else 
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::NoContent:
			UE_LOG(LogJustice, VeryVerbose, TEXT("OnLinkPlatformComplete : Operation succeeded"));
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;
		case EHttpResponseCodes::BadRequest:
			ErrorStr = TEXT("Invalid Request");
			break;
		case EHttpResponseCodes::Forbidden:
			ErrorStr = TEXT("Forbidden");
			break;
		default:
			ErrorStr = FString::Printf(TEXT("unexpected response code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("On Link Platform Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
		return;
	}
}

void JusticeIdentity::UnlinkPlatform(FString PlatformId, FUnlinkPlatformCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString UserId = GetUserId();

	Request->SetURL(FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/%s/unlink"), *BaseURL, *Namespace, *UserId, *PlatformId));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().GameClientToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("text/plain; charset=UTF-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	Request->OnProcessRequestComplete().BindStatic(&JusticeIdentity::OnUnlinkPlatformComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UnlinkPlatform failed. Error=%s XrayID=%s ReqTime%.3f"));
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticeIdentity::OnUnlinkPlatformComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUnlinkPlatformCompleteDelegate OnComplete)
{
	check(&OnComplete != nullptr);
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::NoContent:
			UE_LOG(LogJustice, VeryVerbose, TEXT("OnUnlinkPlatformComplete : Operation succeeded"));
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;
		case EHttpResponseCodes::BadRequest:
			ErrorStr = TEXT("Invalid Request");
			break;
		case EHttpResponseCodes::Forbidden:
			ErrorStr = TEXT("Forbidden");
			break;
		default:
			ErrorStr = FString::Printf(TEXT("unexpected response code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("On Unlink Platform Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
		return;
	}
}

void JusticeIdentity::ClientLogin()
{
	Login(TEXT(""), TEXT(""), FGrantTypeJustice::ClientCredentialGrant, OnComplete);
}

OAuthTokenJustice * JusticeIdentity::GetUserToken()
{
	return FJusticeSDKModule::Get().UserToken;
}

OAuthTokenJustice * JusticeIdentity::GetClientToken()
{
	return FJusticeSDKModule::Get().GameClientToken;
}

FString JusticeIdentity::GetUserId()
{
	return FJusticeSDKModule::Get().UserToken->UserId;
}