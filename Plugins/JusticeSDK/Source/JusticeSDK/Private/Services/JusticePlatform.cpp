// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticePlatform.h"
#include "JusticeSDK.h"
#include "Services/JusticeIdentity.h"


// Get Item By Query Retry Task
class FRequestCurrentPlayerProfileRetryTask : public FJusticeRetryTask
{
public:
	FRequestCurrentPlayerProfileRetryTask(
		FReqestCurrentPlayerProfileCompleteDelegate onComplete,
		int nextRetry = 1,
		int totalElapsedWait = 0) : FJusticeRetryTask(nextRetry, totalElapsedWait),
		OnComplete(onComplete)
	{}
	virtual void Tick()
	{
		check(!IsInGameThread() || !FPlatformProcess::SupportsMultithreading());
		UE_LOG(LogJustice, Log, TEXT("Retry Call JusticeIdentity::RegisterNewPlayer after wait for %d second"), GetLastWait());
		JusticePlatform::RequestCurrentPlayerProfile(
			FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda([&](bool IsSucessful, FString ErrorString, UserProfileInfo UserInfo) {
			if (IsSucessful)
			{
				OnComplete.ExecuteIfBound(IsSucessful, ErrorString, UserInfo);
				SetAsDone();
			}
			else
			{
				if (GetTotalElapsedWait() > 60)
				{
					// take more than 1 minutes after many retries, return failure
					OnComplete.ExecuteIfBound(false, TEXT("Request Timeout"), UserProfileInfo());
				}
				else
				{
					FRequestCurrentPlayerProfileRetryTask* RetryTask = new FRequestCurrentPlayerProfileRetryTask(OnComplete,
						GetLastWait() * 2, // wait more longer for next retry
						GetTotalElapsedWait() + GetLastWait());
					FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
				}

			}
		}));
	}
private:
	FString Language;
	FString Region;
	FString CategoryPath;
	FString ItemType;
	FString Status;
	int Page;
	int Size;
	FReqestCurrentPlayerProfileCompleteDelegate OnComplete;
};



void JusticePlatform::RequestCurrentPlayerProfile(FReqestCurrentPlayerProfileCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;
	
	Request->SetURL(FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/profiles"), *BaseURL, *Namespace, *UserID));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->OnProcessRequestComplete().BindStatic(&JusticePlatform::OnRequestCurrentPlayerProfileComplete, RequestTrace, OnComplete);

	UE_LOG(LogJustice, Log, TEXT("Attemp to call Player Profile: %s"), *Request->GetURL());
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UFJusticeComponent::RequestCurrentPlayerProfile failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr, UserProfileInfo());
	}
}

void JusticePlatform::OnRequestCurrentPlayerProfileComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FReqestCurrentPlayerProfileCompleteDelegate OnComplete)
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
				if (FJusticeSDKModule::Get().UserProfile->FromJson(JsonObject))
				{
					OnComplete.ExecuteIfBound(true, TEXT(""), *FJusticeSDKModule::Get().UserProfile);
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
		case EHttpResponseCodes::NotFound:
		{			
			FString DisplayName = FJusticeSDKModule::Get().UserToken->DisplayName;
			FString Email = FJusticeSDKModule::Get().LoginId;
			UE_LOG(LogJustice, Log, TEXT("Userprofile not found, Attempt to create Default User Profile"));
			JusticePlatform::CreateDefaultPlayerProfile(Email, DisplayName, FUpdatePlayerProfileCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorStr) {
				UE_LOG(LogJustice, Log, TEXT("Create Default User Profile return with result:  %s"), IsSuccess ? TEXT("Success") : TEXT("Failed"));
				if (IsSuccess)
				{
					UE_LOG(LogJustice, Log, TEXT("Call another GetCurrentProfileRequest"));
					JusticePlatform::RequestCurrentPlayerProfile(OnComplete);
				}
				else
				{
					UE_LOG(LogJustice, Error, TEXT("Create Default User Profile Error:  %s"), *ErrorStr);
					OnComplete.ExecuteIfBound(false, TEXT("Cannot create default user profile"), UserProfileInfo());
				}
			}));
			break;
		}
		case EHttpResponseCodes::Denied:
			JusticeIdentity::RefreshToken(FUserLoginCompleteDelegate::CreateLambda([OnComplete, &ErrorStr](bool IsSuccess,
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
						FRequestCurrentPlayerProfileRetryTask* RetryTask = new FRequestCurrentPlayerProfileRetryTask(OnComplete);
						FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
						return;
					}
				}
				else
				{
					//add to queue: refresh token, if success then retry (register new player)
					FRequestCurrentPlayerProfileRetryTask* RetryTask = new FRequestCurrentPlayerProfileRetryTask(OnComplete);
					FRefreshTokenRetryTask* RefreshTokenTask = new FRefreshTokenRetryTask(RetryTask);
					FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RefreshTokenTask);
					return;
				}
			}));
			break;

			break;
		case EHttpResponseCodes::RequestTimeout:
		case EHttpResponseCodes::ServerError:
		case EHttpResponseCodes::ServiceUnavail:
		case EHttpResponseCodes::GatewayTimeout:
		{
			FRequestCurrentPlayerProfileRetryTask* RetryTask = new FRequestCurrentPlayerProfileRetryTask(OnComplete);
			FJusticeSDKModule::Get().RetryTaskManager->AddToRetryQueue(RetryTask);
			return;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Player Profile Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, UserProfileInfo());
		return;
	}
}

void JusticePlatform::UpdatePlayerProfile(UserProfileInfo newUserProfile, FUpdatePlayerProfileCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;
	Request->SetURL(FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/profiles"), *BaseURL, *Namespace, *UserID));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("PUT"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(newUserProfile.ToJson());
	Request->OnProcessRequestComplete().BindStatic(&JusticePlatform::OnUpdatePlayerProfileComplete, RequestTrace, OnComplete, newUserProfile);
	UE_LOG(LogJustice, Log, TEXT("Attemp to call UpdatePlayerProfile: %s"), *Request->GetURL());
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticePlatform::UpdatePlayerProfile failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticePlatform::OnUpdatePlayerProfileComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUpdatePlayerProfileCompleteDelegate OnComplete, UserProfileInfo newUserProfile)
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
			OnComplete.ExecuteIfBound(true, TEXT(""));
			break;

		}
		case EHttpResponseCodes::Denied:
		case EHttpResponseCodes::RequestTimeout:
		case EHttpResponseCodes::ServerError:
		case EHttpResponseCodes::ServiceUnavail:
		case EHttpResponseCodes::GatewayTimeout:
		{
			JusticePlatform::UpdatePlayerProfile(newUserProfile, OnComplete);
			return;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnUpdatePlayerProfileComplete Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
		return;
	}
}

void JusticePlatform::CreateDefaultPlayerProfile(FString Email, FString DisplayName, FUpdatePlayerProfileCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;
	FString DefaultLocale;
	#if PLATFORM_IOS
	DefaultLocale =  FIOSPlatformMisc::GetDefaultLocale();
	#elif PLATFORM_ANDROID
	DefaultLocale =  FAndroidMisc::GetDefaultLocale();
	#else
	DefaultLocale = FGenericPlatformMisc::GetDefaultLocale();
	#endif
	TArray<FString> RegionID;
	DefaultLocale.ParseIntoArray(RegionID, TEXT("_"), true);
	FString Payload = FString::Printf(TEXT("{\"displayName\": \"%s\", \"country\": \"%s\", \"email\": \"%s\"}"), *DisplayName, *RegionID[1], *Email);

	Request->SetURL(FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/profiles"), *BaseURL, *Namespace, *UserID));	
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());	
	Request->SetContentAsString(Payload);
	Request->OnProcessRequestComplete().BindStatic(&JusticePlatform::OnCreateDefaultPlayerProfileComplete, RequestTrace, OnComplete, Email, DisplayName);
	UE_LOG(LogJustice, Log, TEXT("Attemp to call CreateDefaultPlayerProfile: %s"), *Request->GetURL());
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticePlatform::UpdatePlayerProfile failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

void JusticePlatform::OnCreateDefaultPlayerProfileComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUpdatePlayerProfileCompleteDelegate OnComplete, FString Email, FString DisplayName)
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
			UE_LOG(LogJustice, Log, TEXT("OnCreateDefaultPlayerProfileComplete receive success response "));
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
			JusticePlatform::CreateDefaultPlayerProfile(Email, DisplayName, OnComplete);
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
			break;
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnCreateDefaultPlayerProfileComplete Error=%s ReqTime=%.3f"), *ErrorStr, Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

UserProfileInfo * JusticePlatform::GetUserProfileInfo()
{
	return FJusticeSDKModule::Get().UserProfile;
}