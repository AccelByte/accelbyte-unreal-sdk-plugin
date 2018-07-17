// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticePlatform.h"
#include "JusticeSDK.h"
#include "Services/JusticeIdentity.h"

void JusticePlatform::RequestCurrentPlayerProfile(FReqestCurrentPlayerProfileCompleteDelegate OnComplete)
{
	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/profiles"), *FJusticeBaseURL, *FJusticeNamespace, *FJusticeUserID);
	FString Verb			= GET;
	FString ContentType		= TYPE_JSON;
	FString Accept			= TYPE_JSON;
	FString Payload			= TEXT("");

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticePlatform::OnRequestCurrentPlayerProfileComplete, OnComplete));
}

void JusticePlatform::OnRequestCurrentPlayerProfileComplete(FJusticeHttpResponsePtr Response, FReqestCurrentPlayerProfileCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Player Profile Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString, UserProfileInfo() );
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		FString ResponseStr = Response->Content;
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
		FString Email = FJusticeSDKModule::Get().LoginID;
		UE_LOG(LogJustice, Log, TEXT("Userprofile not found, Attempt to create Default User Profile"));
		JusticePlatform::CreateDefaultPlayerProfile(Email, DisplayName, FUpdatePlayerProfileCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorStr) {
			UE_LOG(LogJustice, Log, TEXT("Create Default User Profile return with result:  %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
			if (bSuccessful)
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
		JusticeIdentity::UserRefreshToken(FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, OAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
					OnComplete.ExecuteIfBound(false, ErrorStr, UserProfileInfo());
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticePlatform::OnRequestCurrentPlayerProfileComplete, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr, UserProfileInfo());
				return;
			}
		}));
		break;

		break;
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr, UserProfileInfo());
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticePlatform::OnRequestCurrentPlayerProfileComplete, OnComplete));
		return;	
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
	}	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Player Profile Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr, UserProfileInfo());
		return;
	}
}

void JusticePlatform::UpdatePlayerProfile(UserProfileInfoUpdate newUserProfile, FUpdatePlayerProfileCompleteDelegate OnComplete)
{
	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/profiles"), *FJusticeBaseURL, *FJusticeNamespace, *FJusticeUserID);
	FString Verb			= PUT;
	FString ContentType		= TYPE_JSON;
	FString Accept			= TYPE_JSON;
	FString Payload			= newUserProfile.ToJson();

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticePlatform::OnUpdatePlayerProfileComplete, OnComplete));
}

void JusticePlatform::OnUpdatePlayerProfileComplete(FJusticeHttpResponsePtr Response, FUpdatePlayerProfileCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Update Player Profile. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString);
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		OnComplete.ExecuteIfBound(true, TEXT(""));
		break;

	}
	case EHttpResponseCodes::Denied:
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, OAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					OnComplete.ExecuteIfBound(false, ErrorStr);
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticePlatform::OnUpdatePlayerProfileComplete, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr);
				return;
			}
		}));
		return;
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticePlatform::OnUpdatePlayerProfileComplete, OnComplete));
		return;		
	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpected response Code=%d, Content: %s"), Response->Code, *Response->Content);
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

	FString Authorization = FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/profiles"), *FJusticeBaseURL, *FJusticeNamespace, *FJusticeUserID);
	FString Verb = POST;
	FString ContentType = TYPE_JSON;
	FString Accept = TYPE_JSON;
	FString Payload = FString::Printf(TEXT("{\"displayName\": \"%s\", \"country\": \"%s\", \"email\": \"%s\"}"), *DisplayName, *RegionID[1], *Email);

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticePlatform::OnCreateDefaultPlayerProfileComplete, OnComplete));
}

void JusticePlatform::OnCreateDefaultPlayerProfileComplete(FJusticeHttpResponsePtr Response, FUpdatePlayerProfileCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Create Default Player Profile Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString);
		return;
	}

	switch (Response->Code)
	{
	case EHttpResponseCodes::Created:
	{
		UE_LOG(LogJustice, Log, TEXT("OnCreateDefaultPlayerProfileComplete success"));
		OnComplete.ExecuteIfBound(true, TEXT(""));
		break;
	}
	case EHttpResponseCodes::BadRequest:
	{
		ErrorStr = FString::Printf(TEXT("Expected Error: Invalid Request. Code: %d"), Response->Code);
		break;
	}
	case EHttpResponseCodes::Forbidden:
	{
		ErrorStr = FString::Printf(TEXT("Expected Error: Forbidden. Code=%d"), Response->Code);
		break;
	}
	case EHttpResponseCodes::NotFound:
	{
		ErrorStr = FString::Printf(TEXT("Expected Error: Data not found. Code=%d"), Response->Code);
		break;
	}
	case EHttpResponseCodes::Denied:
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, OAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Response->TooManyRetries() || Response->TakesTooLong())
				{
					OnComplete.ExecuteIfBound(false, ErrorStr);
					return;
				}
				Response->UpdateRequestForNextRetry();
				FJusticeRetryManager->AddQueue(Response->JusticeRequest,
					Response->NextWait,
					FWebRequestResponseDelegate::CreateStatic(JusticePlatform::OnCreateDefaultPlayerProfileComplete, OnComplete));
				return;
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, ErrorStr);
				return;
			}
		}));
		return;
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticePlatform::OnCreateDefaultPlayerProfileComplete, OnComplete));
		return;	}
	default:
		ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->Code);
		break;
	}	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("OnCreateDefaultPlayerProfileComplete Error=%s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
	}
}

UserProfileInfo * JusticePlatform::GetUserProfileInfo()
{
	return FJusticeSDKModule::Get().UserProfile;
}