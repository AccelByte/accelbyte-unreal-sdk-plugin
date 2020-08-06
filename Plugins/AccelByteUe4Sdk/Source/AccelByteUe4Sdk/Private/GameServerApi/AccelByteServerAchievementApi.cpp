// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerAchievementApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerAchievement::ServerAchievement(const AccelByte::ServerCredentials& Credentials, const AccelByte::ServerSettings& Settings) : Credentials(Credentials), Settings(Settings)
{}

ServerAchievement::~ServerAchievement()
{}

void ServerAchievement::UnlockAchievement(const FString& UserId, const FString& AchievementCode, const FVoidHandler OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));
	
	if (UserId.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. User Id is empty."));
		return;
	}
	if (AchievementCode.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. Achievement Code is empty."));
		return;
	}

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/achievements/%s/unlock"), *Settings.AchievementServerUrl, *Credentials.GetClientNamespace(), *UserId, *AchievementCode);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace GameServerApi
} // Namespace AccelByte
