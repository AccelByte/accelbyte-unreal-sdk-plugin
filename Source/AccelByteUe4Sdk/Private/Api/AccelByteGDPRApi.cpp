// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "Api/AccelByteGDPRApi.h"
#include "Core/AccelByteError.h"
#include "JsonUtilities.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace Api
{
	
GDPR::GDPR(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{
}
	
GDPR::~GDPR(){}

void GDPR::SubmitAccountDeletion(FString const& Password
	, const THandler<FAccelByteModelsGDPRSubmitAccountDeletionResponse>& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/deletions")
	, *SettingsRef.GDPRServerUrl
	, *CredentialsRef->GetNamespace()
	, *CredentialsRef->GetUserId());

	TMap<FString, FString> FormFields;
	FormFields.Add(TEXT("password"), Password);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FormFields, Headers, OnSuccess, OnError);
}

void GDPR::SubmitAccountDeletionOtherPlatform(EAccelBytePlatformType PlatformType
	, const FString& PlatformToken
	, const THandler<FAccelByteModelsGDPRSubmitAccountDeletionResponse>& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FString Url = FString::Printf(TEXT("%s/public/users/me/deletions")
	, *SettingsRef.GDPRServerUrl);

	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);
	TMap<FString, FString> FormFields;
	FormFields.Add(TEXT("platformId"), PlatformId);
	FormFields.Add(TEXT("platformToken"), PlatformToken);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded")},
		{TEXT("Accept"), TEXT("application/json")}
	};
	
	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FormFields, Headers, OnSuccess, OnError);
}

void GDPR::SubmitAccountDeletionOtherPlatformId(const FString& PlatformId
	, const FString& PlatformToken
	, const THandler<FAccelByteModelsGDPRSubmitAccountDeletionResponse>& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FString Url = FString::Printf(TEXT("%s/public/users/me/deletions")
	, *SettingsRef.GDPRServerUrl);
	
	TMap<FString, FString> FormFields;
	FormFields.Add(TEXT("platformId"), PlatformId);
	FormFields.Add(TEXT("platformToken"), PlatformToken);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded")},
		{TEXT("Accept"), TEXT("application/json")}
	};
	
	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FormFields, Headers, OnSuccess, OnError);
}
	
void GDPR::GetStatusAccountDeletion(const THandler<FAccelByteModelsGDPRAccountDeletionStatusResponse>& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FString Url = FString::Printf(TEXT("%s/public/users/me/deletions/status")
	, *SettingsRef.GDPRServerUrl);
	
	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void GDPR::CancelAccountDeletion(const FVoidHandler& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FString Url = FString::Printf(TEXT("%s/public/users/me/deletions")
	, *SettingsRef.GDPRServerUrl);
	
	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
