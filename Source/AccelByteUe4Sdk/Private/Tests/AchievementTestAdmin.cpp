// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AchievementTestAdmin.h"
#include "Core/AccelByteHttpRetryScheduler.h"

void AdminCreateAchievement(const FAchievementRequest& AchievementRequest, const THandler<FAchievementResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/achievement/v1/admin/namespaces/%s/achievements"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(AchievementRequest, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminDeleteAchievement(const FString& AchievementCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/achievement/v1/admin/namespaces/%s/achievements/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *AchievementCode);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
