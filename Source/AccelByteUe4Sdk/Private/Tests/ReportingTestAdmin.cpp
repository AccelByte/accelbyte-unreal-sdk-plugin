// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "ReportingTestAdmin.h"
#include "TestUtilities.h"

void ReportingAddReason(const FReportingAddReasonRequest& BodyRequest, const THandler<FReportingAddReasonResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/reporting/v1/admin/namespaces/%s/reasons"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(BodyRequest, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ReportingDeleteReason(const FString& ReasonID, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/reporting/v1/admin/namespaces/%s/reasons/%s"), *BaseUrl, *FRegistry::ServerSettings.Namespace, *ReasonID);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ReportingAdminGetReasons(const FString& ReasonGroup, const THandler<FAccelByteModelsAdminReasonsResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/reporting/v1/admin/namespaces/%s/reasons"), *BaseUrl, *FRegistry::ServerSettings.Namespace);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ReportingAddReasonGroup(const FReportingAddReasonGroupRequest& BodyRequest, const THandler<FReportingAddReasonGroupResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/reporting/v1/admin/namespaces/%s/reasonGroups"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(BodyRequest, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ReportingAdminGetReasonGroups(const THandler<FAccelByteModelsReasonGroupsResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/reporting/v1/admin/namespaces/%s/reasonGroups"), *BaseUrl, *FRegistry::ServerSettings.Namespace);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ReportingDeleteReasonGroup(const FString& ReasonGroupID, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/reporting/v1/admin/namespaces/%s/reasonGroups/%s"), *BaseUrl, *FRegistry::ServerSettings.Namespace, *ReasonGroupID);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
