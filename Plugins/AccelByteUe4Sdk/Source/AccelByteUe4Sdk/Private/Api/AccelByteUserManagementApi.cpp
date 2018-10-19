// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserManagementApi.h"
#include "AccelByteCredentials.h"
#include "AccelByteHttpRetrySystem.h"
#include "Base64.h"
#include "AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

void UserManagement::CreateUserAccount(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Username, FString Password, FString DisplayName, FCreateUserAccountSuccess OnSuccess, FErrorDelegate OnError)
{
	FAccelByteModelsUserCreateRequest NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.LoginId = Username;
	NewUserRequest.AuthType = TEXT("EMAILPASSWD");

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users"), *ServerBaseUrl, *Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(NewUserRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(CreateUserAccountResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}


void UserManagement::CreateUserAccountEasy(FString Username, FString Password, FString DisplayName, FCreateUserAccountSuccess OnSuccess, FErrorDelegate OnError)
{
	CreateUserAccount(Settings::ServerBaseUrl, Credentials::Get().GetClientAccessToken(), Credentials::Get().GetClientNamespace(), Username, Password, DisplayName, OnSuccess, OnError);
}

void UserManagement::AddUsernameAndPassword(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString Username, FString Password, FAddUsernameAndPasswordSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/upgradeHeadlessAccount"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{ \"LoginId\": \"%s\", \"Password\": \"%s\"}"), *Username, *Password);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(AddUsernameAndPasswordResponse, OnSuccess, OnError);
}

void UserManagement::AddUsernameAndPasswordEasy(FString Username, FString Password, FAddUsernameAndPasswordSuccess OnSuccess, FErrorDelegate OnError)
{
	AddUsernameAndPassword(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), Username, Password, OnSuccess, OnError);
}

void UserManagement::SendUserAccountVerificationCode(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString Username, FSendUserAccountVerificationCodeSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verificationcode"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{ \"LoginId\": \"%s\"}"), *Username);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(SendUserAccountVerificationCodeResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void UserManagement::SendUserAccountVerificationCodeEasy(FString Username, FSendUserAccountVerificationCodeSuccess OnSuccess, FErrorDelegate OnError)
{
	SendUserAccountVerificationCode(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), Username, OnSuccess, OnError);
}

void UserManagement::VerifyUserAccount(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString VerificationCode, FVerifyUserAccountSuccess OnSuccess, FErrorDelegate OnError)
{
	FString ContactType = TEXT("email");
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/verification"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{ \"Code\": \"%s\",\"ContactType\":\"%s\"}"), *VerificationCode, *ContactType);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(VerifyUserAccountResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void UserManagement::VerifyUserAccountEasy(FString VerificationCode, FVerifyUserAccountSuccess OnSuccess, FErrorDelegate OnError)
{
	VerifyUserAccount(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), VerificationCode, OnSuccess, OnError);
}

void UserManagement::SendPasswordResetCode(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString ClientNamespace, FString Username, FSendPasswordResetCodeSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/forgotPassword"), *ServerBaseUrl, *ClientNamespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{\"LoginId\": \"%s\"}"), *Username);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(SendPasswordResetCodeResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void UserManagement::SendPasswordResetCodeEasy(FString Username, FSendPasswordResetCodeSuccess OnSuccess, FErrorDelegate OnError)
{
	SendPasswordResetCode(Settings::ServerBaseUrl, Settings::ClientId, Settings::ClientSecret, Settings::GameId, Username, OnSuccess, OnError);
}

void UserManagement::ResetPassword(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString Username, FString VerificationCode, FString NewPassword, FResetPasswordSuccess OnSuccess, FErrorDelegate OnError)
{
	FAccelByteModelsPasswordResetRequest ResetPasswordRequest;
	ResetPasswordRequest.Code = VerificationCode;
	ResetPasswordRequest.LoginId = Username;
	ResetPasswordRequest.NewPassword = NewPassword;
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/resetPassword"), *ServerBaseUrl, *Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(ResetPasswordRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(ResetPasswordResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void UserManagement::ResetPasswordEasy(FString Username, FString VerificationCode, FString NewPassword, FResetPasswordSuccess OnSuccess, FErrorDelegate OnError)
{
	ResetPassword(Settings::ServerBaseUrl, Settings::ClientId, Settings::ClientSecret, Settings::GameId, Username, VerificationCode, NewPassword, OnSuccess, OnError);
}

void UserManagement::GetLinkedUserAccounts(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FGetLinkedUserAccountsSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(GetLinkedUserAccountsResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void UserManagement::GetLinkedUserAccountsEasy(FGetLinkedUserAccountsSuccess OnSuccess, FErrorDelegate OnError)
{
	GetLinkedUserAccounts(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OnSuccess, OnError);
}

void UserManagement::LinkUserAccounts(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString PlatformId, FString Ticket, FLinkUserAccountsSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/%s/link"), *ServerBaseUrl, *Namespace, *UserId, *PlatformId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("ticket=%s"), *Ticket);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(LinkUserAccountsResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void UserManagement::LinkUserAccountsEasy(FString PlatformId, FString Ticket, FLinkUserAccountsSuccess OnSuccess, FErrorDelegate OnError)
{
	LinkUserAccounts(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), PlatformId, Ticket, OnSuccess, OnError);
}

void UserManagement::UnlinkUserAccounts(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString PlatformId, FUnlinkUserAccountsSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/%s/unlink"), *ServerBaseUrl, *Namespace, *UserId, *PlatformId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(UnlinkUserAccountsResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void UserManagement::UnlinkUserAccountsEasy(FString PlatformId, FUnlinkUserAccountsSuccess OnSuccess, FErrorDelegate OnError)
{
	UnlinkUserAccounts(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), PlatformId, OnSuccess, OnError);
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

void UserManagement::CreateUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateUserAccountSuccess OnSuccess, FErrorDelegate OnError)
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
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void UserManagement::AddUsernameAndPasswordResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FAddUsernameAndPasswordSuccess OnSuccess, FErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void UserManagement::SendUserAccountVerificationCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FSendUserAccountVerificationCodeSuccess OnSuccess, FErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void UserManagement::VerifyUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FVerifyUserAccountSuccess OnSuccess, FErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void UserManagement::SendPasswordResetCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FSendPasswordResetCodeSuccess OnSuccess, FErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void UserManagement::ResetPasswordResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FResetPasswordSuccess OnSuccess, FErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void UserManagement::GetLinkedUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetLinkedUserAccountsSuccess OnSuccess, FErrorDelegate OnError)
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
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void UserManagement::LinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FLinkUserAccountsSuccess OnSuccess, FErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void UserManagement::UnlinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUnlinkUserAccountsSuccess OnSuccess, FErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

} // Namespace Api
} // Namespace AccelByte
