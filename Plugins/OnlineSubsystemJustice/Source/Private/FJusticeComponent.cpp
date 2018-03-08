// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "FJusticeComponent.h"
#include "OnlineIdentityJustice.h"
#include "AWSXRayJustice.h"
#include "HTTPJustice.h"

#include "Misc/ConfigCacheIni.h"

UFJusticeComponent* UFJusticeComponent::Instance = nullptr;

void UFJusticeComponent::ClientCredentialLogin()
{
	FString ErrorStr;
	FString ClientID;
	FString ClientSecret;
	FString BaseURL;
	TSharedPtr<FUserOnlineAccountJustice> UserAccountPtr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientId"), ClientID, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientId= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}

	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientSecret"), ClientSecret, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientSecret= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}

	Request->SetURL(BaseURL + TEXT("/iam/oauth/token"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BasicAuth(ClientID, ClientSecret));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	FString Grant = FString::Printf(TEXT("grant_type=client_credentials"));
	Request->SetContentAsString(Grant);
	Request->OnProcessRequestComplete().BindUObject(this, &UFJusticeComponent::TokenGameClientCredentialComplete, RequestTrace);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	UE_LOG_ONLINE(VeryVerbose, TEXT("UFJusticeComponent::ClientCredentialLogin Sucessful, XRayID= %s"),  *RequestTrace->ToString());
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("UFJusticeComponent::ClientCredentialLogin failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());	
	}
}



void UFJusticeComponent::TokenGameClientCredentialComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace)
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
				if (GameClientToken->FromJson(JsonObject))
				{
					TArray<TSharedPtr<FJsonValue>> PermissionArray = JsonObject->GetArrayField(TEXT("permissions"));
					for (TSharedPtr<FJsonValue> Permission : PermissionArray)
					{
						TSharedPtr<FJsonObject> JsonPermissionObject = Permission->AsObject();
						FString Resource = JsonPermissionObject->GetStringField(TEXT("Resource"));
						int32 Action = JsonPermissionObject->GetIntegerField(TEXT("Action"));
						FPermissionJustice PermissionObject = FPermissionJustice(Resource, Action);
						GameClientToken->Permissions.Add(PermissionObject);
						
					}
					GameClientToken->SetLastRefreshTimeToNow();
					GameClientToken->ScheduleNormalRefresh();
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
			break;
		}
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("Game Client Credential failed. Error=%s XRay=%s ReqTime=%.3f"),
			*ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		return;
	}

	UE_LOG_ONLINE(Log, TEXT("Game Client Credential successful. ReqTime=%.3f"),Request->GetElapsedTime());
	
}




