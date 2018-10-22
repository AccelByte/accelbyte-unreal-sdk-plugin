// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteWalletApi.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"
#include "JsonUtilities.h"

namespace AccelByte
{
namespace Api
{

void Wallet::GetWalletInfoByCurrencyCode(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& CurrencyCode, const FGetWalletByCurrencyCodeSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url				= FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/wallets/%s"), *Settings::PlatformServerUrl, *Namespace, *UserId, *CurrencyCode);
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");
	FString Content			= TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindStatic(GetWalletInfoByCurrencyCodeResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void Wallet::GetWalletInfoByCurrencyCodeEasy(const FString& CurrencyCode, const FGetWalletByCurrencyCodeSuccess& OnSuccess, const FErrorHandler& OnError)
{
	GetWalletInfoByCurrencyCode(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), CurrencyCode, OnSuccess, OnError);
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

void Wallet::GetWalletInfoByCurrencyCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetWalletByCurrencyCodeSuccess OnSuccess, FErrorHandler OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsWalletInfo Result;
		if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0))
		{
			OnSuccess.ExecuteIfBound(Result);
			return;
		}
		Code = static_cast<int32>(ErrorCodes::JsonDeserializationFailed);
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

} // Namespace Api
} // Namespace AccelByte
