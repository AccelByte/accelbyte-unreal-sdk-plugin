// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteServicesIdentity.h"
#include "JsonUtilities.h"
#include "Base64.h"
#include "AccelByteHttpRetrySystem.h"

namespace AccelByte
{
namespace Services
{

#pragma region OAuth

void Identity::GetAccessTokenWithAuthorizationCodeGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString AuthorizationCode, FString RedirectUri, FGetUserAccessTokenWithAuthorizationCodeGrantSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/oauth/token"), *ServerBaseUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("grant_type=authorization_code&code=%s&redirect_uri=%s"), *AuthorizationCode, *RedirectUri);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetUserAccessTokenWithAuthorizationCodeGrantResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Identity::GetUserAccessTokenWithPasswordGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString LoginId, FString Password, FGetUserAccessTokenWithPasswordGrantSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/oauth/token"), *ServerBaseUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("grant_type=password&username=%s&password=%s&namespace=%s"), *FGenericPlatformHttp::UrlEncode(LoginId), *FGenericPlatformHttp::UrlEncode(Password), *Namespace);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetUserAccessTokenWithPasswordGrantResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Identity::GetAccessTokenWithClientCredentialsGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FGetAccessTokenWithClientCredentialsGrantSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/oauth/token"), *ServerBaseUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("grant_type=client_credentials"));

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetAccessTokenWithClientCredentialsGrantResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Identity::GetAccessTokenWithRefreshTokenGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString RefreshToken, FGetAccessTokenWithRefreshTokenGrantSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/oauth/token"), *ServerBaseUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("grant_type=refresh_token&refresh_token=%s"), *FGenericPlatformHttp::UrlEncode(RefreshToken));

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetAccessTokenWithRefreshTokenGrantResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

//
// Custom grant types 
//

void Identity::GetAccessTokenWithDeviceGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FGetAccessTokenWithDeviceGrantSuccess OnSuccess, ErrorDelegate OnError)
{
	FString DeviceId = FGenericPlatformMisc::GetDeviceId();
	check(!DeviceId.IsEmpty() && "Cannot get Device Id");

	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/oauth/namespaces/%s/platforms/device/token"), *ServerBaseUrl, *Namespace);;
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("device_id=%s"), *DeviceId);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetAccessTokenWithDeviceGrantResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Identity::GetAccessTokenWithPlatformGrant(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString PlatformId, FString Token, FGetAccessTokenWithPlatformGrantSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
    FString Url = FString::Printf(TEXT("%s/iam/oauth/platforms/%s/token"), *ServerBaseUrl, *PlatformId);
    FString Verb = TEXT("POST");
    FString ContentType = TEXT("application/x-www-form-urlencoded");
    FString Accept = TEXT("application/json");
    FString Content = FString::Printf(TEXT("platform_token=%s&namespace=%s"), *Token, *Namespace);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetAccessTokenWithPlatformGrantResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

#pragma endregion OAuth

void Identity::CreateUserAccount(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString Password, FString DisplayName, EUserAuthTypeJustice AuthType, FCreateUserAccountSuccess OnSuccess, ErrorDelegate OnError)
{
	FAccelByteModelsUserCreateRequest NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.LoginId = UserId;
	NewUserRequest.AuthType = (AuthType == Email) ? TEXT("EMAILPASSWD"): TEXT("PHONEPASSWD");

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users"), *ServerBaseUrl, *Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(NewUserRequest, Content);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		CreateUserAccountResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Identity::UpgradeHeadlessUserAccount(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString Email, FString Password, FUpgradeHeadlessAccountSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/upgradeHeadlessAccount"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{ \"LoginId\": \"%s\", \"Password\": \"%s\"}"), *Email, *Password);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		UpgradeHeadlessUserAccountResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Identity::SendVerificationCodeForUserAccountCreation(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString LoginId, FSendVerificationCodeForUserAccountCreationSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verificationcode"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{ \"LoginId\": \"%s\"}"), *LoginId);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		SendVerificationCodeForUserAccountCreationResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Identity::VerifyUserAccountCreation(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString VerificationCode, EUserAuthTypeJustice AuthType, FVerifyUserAccountCreation OnSuccess, ErrorDelegate OnError)
{
	FString ContactType = (AuthType == Email) ? TEXT("email") : TEXT("phone");
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verification"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{ \"Code\": \"%s\",\"ContactType\":\"%s\"}"), *VerificationCode, *ContactType);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		VerifyUserAccountCreationResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Identity::SendVerificationCodeForPasswordReset(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString LoginId, FSendVerificationCodeForPasswordResetSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/forgotPassword"), *ServerBaseUrl, *Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{\"LoginId\": \"%s\"}"), *LoginId);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		SendVerificationCodeForPasswordResetResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Identity::VerifyPasswordReset(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString UserId, FString VerificationCode, FString NewPassword, FVerifyResetPasswordSuccess OnSuccess, ErrorDelegate OnError)
{
	FAccelByteModelsResetPasswordRequest ResetPasswordRequest;
	ResetPasswordRequest.Code = VerificationCode;
	ResetPasswordRequest.LoginId = UserId;
	ResetPasswordRequest.NewPassword = NewPassword;
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/resetPassword"), *ServerBaseUrl, *Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(ResetPasswordRequest, Content);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		VerifyPasswordResetResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Identity::GetLinkedUserAccounts(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FGetLinkedUserAccountsSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetLinkedUserAccountsResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Identity::LinkUserAccounts(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString PlatformId, FString Ticket, FLinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/%s/link"), *ServerBaseUrl, *Namespace, *UserId, *PlatformId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("ticket=%s"), *Ticket);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		LinkUserAccountsResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void Identity::UnlinkUserAccounts(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString PlatformId, FUnlinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/%s/unlink"), *ServerBaseUrl, *Namespace, *UserId, *PlatformId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		UnlinkUserAccountsResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

void Identity::GetUserAccessTokenWithAuthorizationCodeGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetUserAccessTokenWithAuthorizationCodeGrantSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOAuthToken Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::GetUserAccessTokenWithPasswordGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetUserAccessTokenWithPasswordGrantSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOAuthToken Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::GetAccessTokenWithDeviceGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithDeviceGrantSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOAuthToken Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::GetAccessTokenWithPlatformGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithPlatformGrantSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOAuthToken Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::GetAccessTokenWithRefreshTokenGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithRefreshTokenGrantSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOAuthToken Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::CreateUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FCreateUserAccountSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsUserCreateResponse Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::UpgradeHeadlessUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FUpgradeHeadlessAccountSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::SendVerificationCodeForUserAccountCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FSendVerificationCodeForUserAccountCreationSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::VerifyUserAccountCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FVerifyUserAccountCreation OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::SendVerificationCodeForPasswordResetResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FSendVerificationCodeForPasswordResetSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::VerifyPasswordResetResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FVerifyResetPasswordSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::GetLinkedUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetLinkedUserAccountsSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		TArray<FAccelByteModelsLinkedPlatform> Result;
		FJsonObjectConverter::JsonArrayStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::LinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FLinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::UnlinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FUnlinkUserAccountsSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void Identity::GetAccessTokenWithClientCredentialsGrantResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetAccessTokenWithClientCredentialsGrantSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsOAuthToken Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

} // Namespace Services
} // Namespace AccelByte