// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticePlatform.h"

void JusticePlatform::RequestCurrentPlayerProfile(FReqestCurrentPlayerProfileCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	Request->SetURL(BaseURL + TEXT("/platform/public/namespaces/") + FJusticeSDKModule::Get().Namespace + TEXT("/users/") + FJusticeSDKModule::Get().UserToken->UserId + TEXT("/profiles"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
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
		OnComplete.Execute(false, ErrorStr, UserProfileInfo());
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
					OnComplete.Execute(true, TEXT(""), *FJusticeSDKModule::Get().UserProfile);
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
					// call another GetCurrentProfileRequest
					UE_LOG(LogJustice, Log, TEXT("Call another GetCurrentProfileRequest"));
					JusticePlatform::RequestCurrentPlayerProfile(OnComplete);
				}
				else
				{
					UE_LOG(LogJustice, Error, TEXT("Create Default User Profile Error:  %s"), *ErrorStr);
					OnComplete.Execute(false, TEXT("Cannot create default user profile"), UserProfileInfo());
				}
			}));
			break;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Player Profile Error : %s"), *ErrorStr);
		OnComplete.Execute(false, ErrorStr, UserProfileInfo());
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

	Request->SetURL(BaseURL + TEXT("/platform/public/namespaces/") + FJusticeSDKModule::Get().Namespace + TEXT("/users/") + FJusticeSDKModule::Get().UserToken->UserId + TEXT("/profiles"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("PUT"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	Request->SetContentAsString(newUserProfile.ToJson());
	Request->OnProcessRequestComplete().BindStatic(&JusticePlatform::OnUpdatePlayerProfileComplete, RequestTrace, OnComplete);
	UE_LOG(LogJustice, Log, TEXT("Attemp to call UpdatePlayerProfile: %s"), *Request->GetURL());
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticePlatform::UpdatePlayerProfile failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.Execute(false, ErrorStr);
	}
}

void JusticePlatform::OnUpdatePlayerProfileComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUpdatePlayerProfileCompleteDelegate OnComplete)
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
			OnComplete.Execute(true, TEXT(""));
		}
		break;

		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnUpdatePlayerProfileComplete Error : %s"), *ErrorStr);
		OnComplete.Execute(false, ErrorStr);
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

	//{{justice_url}}/platform/public/profiles/namespaces/{{namespace}}/users/{{user_id}}
	Request->SetURL(BaseURL + TEXT("/platform/public/namespaces/") + FJusticeSDKModule::Get().Namespace + TEXT("/users/") + FJusticeSDKModule::Get().UserToken->UserId + TEXT("/profiles"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	FString Payload = FString::Printf(TEXT("{\"displayName\": \"%s\", \"email\" : \"%s\",\"country\" : \"CN\"}"), *DisplayName, *Email);
	Request->SetContentAsString(Payload);
	Request->OnProcessRequestComplete().BindStatic(&JusticePlatform::OnCreateDefaultPlayerProfileComplete, RequestTrace, OnComplete);
	UE_LOG(LogJustice, Log, TEXT("Attemp to call CreateDefaultPlayerProfile: %s"), *Request->GetURL());
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticePlatform::UpdatePlayerProfile failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.Execute(false, ErrorStr);
	}
}

void JusticePlatform::OnCreateDefaultPlayerProfileComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUpdatePlayerProfileCompleteDelegate OnComplete)
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
			OnComplete.Execute(true, TEXT(""));
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
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
			break;
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnCreateDefaultPlayerProfileComplete Error=%s ReqTime=%.3f"), *ErrorStr, Request->GetElapsedTime());
		OnComplete.Execute(false, ErrorStr);
	}
}

UserProfileInfo * JusticePlatform::GetUserProfileInfo()
{
	return FJusticeSDKModule::Get().UserProfile;
}