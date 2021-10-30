// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GroupTestAdmin.h"
#include "Core/AccelByteHttpRetryScheduler.h"

void AdminInitGroupConfig(const THandler<FGroupConfigResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FInitGroupConfigRequest InitGroupConfigRequest;
	InitGroupConfigRequest.Namespace = FRegistry::Settings.Namespace;

	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	const FString Url = FString::Printf(TEXT("%s/group/v1/admin/namespaces/%s/configuration/initiate"),
		*GetAdminBaseUrl(), *InitGroupConfigRequest.Namespace);
	
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(InitGroupConfigRequest, Content);
	
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request,CreateHttpResultHandler(OnSuccess, OnError),
		FPlatformTime::Seconds());
}

void AdminGetGroupConfigList(
	FGetGroupConfigListRequest& GetGroupConfigListRequest,
	const THandler<FGetGroupConfigListResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	GetGroupConfigListRequest.Namespace = FRegistry::Settings.Namespace;
	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());

	const FString Query = FString::Printf(TEXT("limit=%d&offset=%d"),
		GetGroupConfigListRequest.Limit,
		GetGroupConfigListRequest.Offset);
	
	const FString Url = FString::Printf(TEXT("%s/group/v1/admin/namespaces/%s/configuration?%s"),
		*GetAdminBaseUrl(),
		*GetGroupConfigListRequest.Namespace,
		*Query);
	
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError),
		FPlatformTime::Seconds());
}

void AdminDeleteGroupConfig(
	const FString& GroupConfigurationCode,
	const FVoidHandler& OnSuccess,
	const FErrorHandler& OnError)
{
	FDeleteGroupConfigRequest DeleteGroupConfigRequest;
	DeleteGroupConfigRequest.Namespace = FRegistry::Settings.Namespace;
	DeleteGroupConfigRequest.ConfigurationCode = GroupConfigurationCode;

	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	const FString Url = FString::Printf(TEXT("%s/group/v1/admin/namespaces/%s/configuration/%s"),
		*GetAdminBaseUrl(), *DeleteGroupConfigRequest.Namespace, *GroupConfigurationCode);
	
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request,CreateHttpResultHandler(OnSuccess, OnError),
		FPlatformTime::Seconds());
}

void AdminCreateMemberRole(
	const FCreateMemberRoleRequest& CreateMemberRoleRequest,
	const THandler<FCreateMemberRoleResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
		const FString Url = FString::Printf(TEXT("%s/group/v1/admin/namespaces/%s/roles"),
		*GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(CreateMemberRoleRequest, Content);
	
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request,CreateHttpResultHandler(OnSuccess, OnError),
		FPlatformTime::Seconds());
}
