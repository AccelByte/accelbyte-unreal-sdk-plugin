#include "JusticePlatform.h"

void JusticePlatform::RequestCurrentPlayerProfile(FReqestCurrentPlayerProfileCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	Request->SetURL(BaseURL + TEXT("/platform/public/profiles/namespaces/") + FJusticeSDKModule::Get().Namespace + TEXT("/users/") + FJusticeSDKModule::Get().UserToken->UserId);
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	UE_LOG(LogJustice, Log, TEXT("Attemp to call Player Profile: %s"), *Request->GetURL());

	Request->OnProcessRequestComplete().BindStatic(&JusticePlatform::OnRequestCurrentPlayerProfileComplete, RequestTrace, OnComplete);
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

void JusticePlatform::UpdatePlayerProfile(UserProfileInfo newUserProfile, FUpdatePlayerProfileCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	Request->SetURL(BaseURL + TEXT("/platform/public/profiles/namespaces/") + FJusticeSDKModule::Get().Namespace + TEXT("/users/") + FJusticeSDKModule::Get().UserToken->UserId);
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("PUT"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Payload = newUserProfile.ToJson();

	Request->SetContentAsString(Payload);
	UE_LOG(LogJustice, Log, TEXT("Attemp to call UpdatePlayerProfile: %s"), *Request->GetURL());

	Request->OnProcessRequestComplete().BindStatic(&JusticePlatform::OnUpdatePlayerProfileComplete, RequestTrace, OnComplete);
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

UserProfileInfo * JusticePlatform::GetUserProfileInfo()
{
	return FJusticeSDKModule::Get().UserProfile;
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
		}
		break;

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
