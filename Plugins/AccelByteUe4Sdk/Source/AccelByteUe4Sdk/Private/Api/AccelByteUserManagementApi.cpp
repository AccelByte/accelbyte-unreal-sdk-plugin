// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserManagementApi.h"

#include "AccelByteRegistry.h"
#include "Base64.h"

namespace AccelByte
{
namespace Api
{

void UserManagement::CreateUserAccount(const FString& Username, const FString& Password, const FString& DisplayName, const FCreateUserAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FAccelByteModelsUserCreateRequest NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.LoginId = Username;
	NewUserRequest.AuthType = TEXT("EMAILPASSWD");

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetClientNamespace());
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(CreateUserAccountResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserManagement::UpdateUserAccount(const FAccelByteModelsUserUpdateRequest& UpdateRequest, const FUpdateUserAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId());
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(UpdateUserAccountResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserManagement::SendUserUpgradeVerificationCode(const FString & LoginId, const FSendUserUpgradeVerificationCodeSuccess & OnSuccess, const FErrorHandler & OnError, const FString& LanguageTag)
{
	FAccelByteModelsSendVerificationCodeRequest Request
	{
		EAccelByteVerificationCodeContext::UpgradeHeadlessAccount,
		LanguageTag,
		LoginId
	};
	SendUserAccountVerificationCode(Request, OnSuccess, OnError);
}

void UserManagement::UpgradeHeadlessAccountWithVerificationCode(const FAccelByteModelsUpgradeHeadlessAccountWithVerificationCodeRequest& RequestObject, const FUpgradeHeadlessAccountWithVerificationCodeSuccess & OnSuccess, const FErrorHandler & OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/upgradeHeadlessAccountWithVerificationCode"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString<FAccelByteModelsUpgradeHeadlessAccountWithVerificationCodeRequest>(RequestObject, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(UpgradeHeadlessAccountWithVerificationCodeResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserManagement::UpgradeHeadlessAccount(const FString& Username, const FString& Password, const FUpgradeHeadlessAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/upgradeHeadlessAccount"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId());
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(AddUsernameAndPasswordResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserManagement::SendUserAccountVerificationCode(const FAccelByteModelsSendVerificationCodeRequest& SendVerificationCodeRequest, const FSendUserAccountVerificationCodeSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = TEXT("");
	FString Namespace = TEXT("");
	if (SendVerificationCodeRequest.Context == EAccelByteVerificationCodeContext::UserAccountRegistration) 
	{
		Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
		Namespace = FRegistry::Credentials.GetClientNamespace();
	} 
	else
	{
		Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
		Namespace = FRegistry::Credentials.GetUserNamespace();
	}
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/verificationcode"), *FRegistry::Settings.IamServerUrl, *Namespace, *FRegistry::Credentials.GetUserId());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString<FAccelByteModelsSendVerificationCodeRequest>(SendVerificationCodeRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(SendUserAccountVerificationCodeResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserManagement::VerifyUserAccount(const FString& VerificationCode, const FVerifyUserAccountSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString ContactType = TEXT("email");
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/verification"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetClientNamespace(), *FRegistry::Credentials.GetUserId());
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(VerifyUserAccountResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserManagement::SendPasswordResetCode(const FString& Username, const FSendPasswordResetCodeSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = TEXT("Basic " + FBase64::Encode(FRegistry::Settings.ClientId + ":" + FRegistry::Settings.ClientSecret));
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/forgotPassword"), *FRegistry::Settings.IamServerUrl, *FRegistry::Settings.Namespace);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(SendPasswordResetCodeResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserManagement::ResetPassword(const FString& Username, const FString& VerificationCode, const FString& NewPassword, const FResetPasswordSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FAccelByteModelsPasswordResetRequest ResetPasswordRequest;
	ResetPasswordRequest.Code = VerificationCode;
	ResetPasswordRequest.LoginId = Username;
	ResetPasswordRequest.NewPassword = NewPassword;
	FString Authorization = TEXT("Basic " + FBase64::Encode(FRegistry::Settings.ClientId + ":" + FRegistry::Settings.ClientSecret));
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/resetPassword"), *FRegistry::Settings.IamServerUrl, *FRegistry::Settings.Namespace);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(ResetPasswordResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserManagement::GetLinkedUserAccounts(const FGetLinkedUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/platforms"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetClientNamespace(), *FRegistry::Credentials.GetUserId());
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(GetLinkedUserAccountsResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserManagement::LinkUserAccounts(const FString& PlatformId, const FString& Ticket, const FLinkUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/platforms/%s/link"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetClientNamespace(), *FRegistry::Credentials.GetUserId(), *PlatformId);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(LinkUserAccountsResponse, OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserManagement::UnlinkUserAccounts(const FString& PlatformId, const FUnlinkUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/platforms/%s/unlink"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetClientNamespace(), *FRegistry::Credentials.GetUserId(), *PlatformId);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateStatic(UnlinkUserAccountsResponse, OnSuccess, OnError), FPlatformTime::Seconds());
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
		FAccelByteModelsUserResponse Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void UserManagement::AddUsernameAndPasswordResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUpgradeHeadlessAccountSuccess OnSuccess, FErrorHandler OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FRegistry::Credentials.ForceRefreshToken();
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

void UserManagement::UpgradeHeadlessAccountWithVerificationCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUpgradeHeadlessAccountWithVerificationCodeSuccess OnSuccess, FErrorHandler OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FRegistry::Credentials.ForceRefreshToken();
		FAccelByteModelsUserResponse Result;
		FJsonObjectConverter::JsonObjectStringToUStruct<FAccelByteModelsUserResponse>(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

} // Namespace Api
} // Namespace AccelByte
