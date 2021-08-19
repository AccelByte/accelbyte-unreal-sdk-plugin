// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "SubscriptionTestAdmin.h"
#include "Core/AccelByteHttpRetryScheduler.h"

void AdminGrantSubscriptionFree(const FString& UserId, const FFreeSubscriptionRequest& BodyRequest, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/users/%s/subscriptions/platformSubscribe"), *GetAdminBaseUrl(), *FRegistry::Settings.PublisherNamespace, *UserId);
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(BodyRequest, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
