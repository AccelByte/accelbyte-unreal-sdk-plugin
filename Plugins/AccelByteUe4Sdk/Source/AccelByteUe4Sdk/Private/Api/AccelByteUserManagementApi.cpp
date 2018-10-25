// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserManagementApi.h"
#include "AccelByteCredentials.h"
#include "Base64.h"
#include "AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

void UserManagement::CreateUserAccount(const FString& AccessToken, const FString& Namespace, const FString& Username, const FString& Password, const FString& DisplayName, const FCreateUserAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FAccelByteModelsUserCreateRequest NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.LoginId = Username;
	NewUserRequest.AuthType = TEXT("EMAILPASSWD");

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users"), *Settings::IamServerUrl, *Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
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


void UserManagement::CreateUserAccountEasy(const FString& Username, const FString& Password, const FString& DisplayName, const FCreateUserAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	CreateUserAccount(Credentials::Get().GetClientAccessToken(), Credentials::Get().GetClientNamespace(), Username, Password, DisplayName, OnSuccess, OnError);
}

void UserManagement::UpdateUserAccount(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FAccelByteModelsUserUpdateRequest& UpdateRequest, const FUpdateUserAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s"), *Settings::IamServerUrl, *Namespace, *UserId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(UpdateRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(UpdateUserAccountResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void UserManagement::UpdateUserAccountEasy(const FAccelByteModelsUserUpdateRequest& UpdateRequest, const FUpdateUserAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	UpdateUserAccount(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), UpdateRequest, OnSuccess, OnError);
}

void UserManagement::AddUsernameAndPassword(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& Username, const FString& Password, const FAddUsernameAndPasswordSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/upgradeHeadlessAccount"), *Settings::IamServerUrl, *Namespace, *UserId);
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
	Request->ProcessRequest();
}

void UserManagement::AddUsernameAndPasswordEasy(const FString& Username, const FString& Password, const FAddUsernameAndPasswordSuccess& OnSuccess, const FErrorHandler& OnError)
{
	AddUsernameAndPassword(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), Username, Password, OnSuccess, OnError);
}

void UserManagement::SendUserAccountVerificationCode(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& Username, const FSendUserAccountVerificationCodeSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/verificationcode"), *Settings::IamServerUrl, *Namespace, *UserId);
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

void UserManagement::SendUserAccountVerificationCodeEasy(const FString& Username, const FSendUserAccountVerificationCodeSuccess& OnSuccess, const FErrorHandler& OnError)
{
	SendUserAccountVerificationCode(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), Username, OnSuccess, OnError);
}

void UserManagement::VerifyUserAccount(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& VerificationCode, const FVerifyUserAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString ContactType = TEXT("email");
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/verification"), *Settings::IamServerUrl, *Namespace, *UserId);
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

void UserManagement::VerifyUserAccountEasy(const FString& VerificationCode, const FVerifyUserAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	VerifyUserAccount(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), VerificationCode, OnSuccess, OnError);
}

void UserManagement::SendPasswordResetCode(const FString& ClientId, const FString& ClientSecret, const FString& ClientNamespace, const FString& Username, const FSendPasswordResetCodeSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/forgotPassword"), *Settings::IamServerUrl, *ClientNamespace);
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

void UserManagement::SendPasswordResetCodeEasy(const FString& Username, const FSendPasswordResetCodeSuccess& OnSuccess, const FErrorHandler& OnError)
{
	SendPasswordResetCode(Settings::ClientId, Settings::ClientSecret, Settings::GameId, Username, OnSuccess, OnError);
}

void UserManagement::ResetPassword(const FString& ClientId, const FString& ClientSecret, const FString& Namespace, const FString& Username, const FString& VerificationCode, const FString& NewPassword, const FResetPasswordSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FAccelByteModelsPasswordResetRequest ResetPasswordRequest;
	ResetPasswordRequest.Code = VerificationCode;
	ResetPasswordRequest.LoginId = Username;
	ResetPasswordRequest.NewPassword = NewPassword;
	FString Authorization = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/resetPassword"), *Settings::IamServerUrl, *Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
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

void UserManagement::ResetPasswordEasy(const FString& Username, const FString& VerificationCode, const FString& NewPassword, const FResetPasswordSuccess& OnSuccess, const FErrorHandler& OnError)
{
	ResetPassword(Settings::ClientId, Settings::ClientSecret, Settings::GameId, Username, VerificationCode, NewPassword, OnSuccess, OnError);
}

void UserManagement::GetLinkedUserAccounts(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FGetLinkedUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/platforms"), *Settings::IamServerUrl, *Namespace, *UserId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

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

void UserManagement::GetLinkedUserAccountsEasy(const FGetLinkedUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError)
{
	GetLinkedUserAccounts(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OnSuccess, OnError);
}

void UserManagement::LinkUserAccounts(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& PlatformId, const FString& Ticket, const FLinkUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/platforms/%s/link"), *Settings::IamServerUrl, *Namespace, *UserId, *PlatformId);
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

void UserManagement::LinkUserAccountsEasy(const FString& PlatformId, const FString& Ticket, const FLinkUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError)
{
	LinkUserAccounts(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), PlatformId, Ticket, OnSuccess, OnError);
}

void UserManagement::UnlinkUserAccounts(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& PlatformId, const FUnlinkUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/platforms/%s/unlink"), *Settings::IamServerUrl, *Namespace, *UserId, *PlatformId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

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

void UserManagement::UnlinkUserAccountsEasy(const FString& PlatformId, const FUnlinkUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError)
{
	UnlinkUserAccounts(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), PlatformId, OnSuccess, OnError);
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

void UserManagement::CreateUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateUserAccountSuccess OnSuccess, FErrorHandler OnError)
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

void UserManagement::UpdateUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUpdateUserAccountSuccess OnSuccess, FErrorHandler OnError)
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

void UserManagement::AddUsernameAndPasswordResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FAddUsernameAndPasswordSuccess OnSuccess, FErrorHandler OnError)
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

void UserManagement::SendUserAccountVerificationCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FSendUserAccountVerificationCodeSuccess OnSuccess, FErrorHandler OnError)
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

void UserManagement::VerifyUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FVerifyUserAccountSuccess OnSuccess, FErrorHandler OnError)
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

void UserManagement::SendPasswordResetCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FSendPasswordResetCodeSuccess OnSuccess, FErrorHandler OnError)
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

void UserManagement::ResetPasswordResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FResetPasswordSuccess OnSuccess, FErrorHandler OnError)
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

void UserManagement::GetLinkedUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetLinkedUserAccountsSuccess OnSuccess, FErrorHandler OnError)
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

void UserManagement::LinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FLinkUserAccountsSuccess OnSuccess, FErrorHandler OnError)
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

void UserManagement::UnlinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUnlinkUserAccountsSuccess OnSuccess, FErrorHandler OnError)
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
