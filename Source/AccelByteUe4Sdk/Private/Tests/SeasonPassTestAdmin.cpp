// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "SeasonPassTestAdmin.h"


void SeasonCreateSeason(const FSeasonPassCreateRequest& Req, const THandler<FSeasonPassCreateSeasonResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/seasonpass/admin/namespaces/%s/seasons"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Req, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void SeasonQuerySeason(const TArray<EAccelByteSeasonPassStatus>& Status, const THandler<FSeasonPassCreateGetSeasonsPagingResponse>& OnSuccess, const FErrorHandler& OnError, int32 offset, int32 limit)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Params;
	if (Status.Contains(EAccelByteSeasonPassStatus::DRAFT))
	{
		Params += TEXT("status=DRAFT");
	}
	if (Status.Contains(EAccelByteSeasonPassStatus::PUBLISHED))
	{
		Params += Params.IsEmpty() ? TEXT("status=PUBLISHED") : TEXT("&status=PUBLISHED");
	}
	if (Status.Contains(EAccelByteSeasonPassStatus::RETIRED))
	{
		Params += Params.IsEmpty() ? TEXT("status=RETIRED") : TEXT("&status=RETIRED");
	}
	Params += Params.IsEmpty() ? TEXT("") : TEXT("&");
	Params += FString::Printf(TEXT("offset=%d"), offset);
	Params += Params.IsEmpty() ? TEXT("") : TEXT("&");
	Params += FString::Printf(TEXT("limit=%d"), limit);

	FString Url = FString::Printf(TEXT("%s/seasonpass/admin/namespaces/%s/seasons?%s"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace, *Params);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void SeasonDeleteSeason(const FString& SeasonId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf( TEXT("%s/seasonpass/admin/namespaces/%s/seasons/%s"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace, *SeasonId);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void SeasonPublishSeason(const FString& SeasonId, const THandler<FSeasonPassCreateSeasonResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf( TEXT("%s/seasonpass/admin/namespaces/%s/seasons/%s/publish"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace, *SeasonId);
	FString Content;
	AB_HTTP_PUT(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void SeasonForceUnpublishSeason(const FString& SeasonId, const THandler<FSeasonPassCreateSeasonResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf( TEXT("%s/seasonpass/admin/namespaces/%s/seasons/%s/unpublish?force=true"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace, *SeasonId);
	FString Content;
	AB_HTTP_PUT(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void SeasonCreatePass(const FString& SeasonId, const FSeasonPassCreatePassRequest& Req, const THandler<FSeasonPassCreatePassResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf( TEXT("%s/seasonpass/admin/namespaces/%s/seasons/%s/passes"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace, *SeasonId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Req, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void SeasonDeletePass(const FString& SeasonId, const FString& PassCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/seasonpass/admin/namespaces/%s/seasons/%s/passes/%s"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace, *SeasonId, *PassCode);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void SeasonCreateReward(const FString& SeasonId, const FSeasonPassCreateReward& Req, const THandler<FSeasonPassRewardResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/seasonpass/admin/namespaces/%s/seasons/%s/rewards"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace, *SeasonId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Req, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void SeasonDeleteReward(const FString& SeasonId, const FString& RewardCode, const FSimpleDelegate& OnSuccess,
						const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/seasonpass/admin/namespaces/%s/seasons/%s/rewards/%s"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace, *SeasonId, *RewardCode);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void SeasonCreateTier(const FString& SeasonId, const FSeasonPassCreateTierRequest& Req, const THandler<TArray<FAccelByteModelsSeasonPassTierJsonObject>>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/seasonpass/admin/namespaces/%s/seasons/%s/tiers"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace, *SeasonId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Req, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void SeasonModifyTier(const FString& SeasonId, const FString& TierId, const FSeasonPassTierRequest& Req, const THandler<TArray<FAccelByteModelsSeasonPassTierJsonObject>>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/seasonpass/admin/namespaces/%s/seasons/%s/tiers/%s"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace, *SeasonId, *TierId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Req, Content);
	AB_HTTP_PUT(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void SeasonDeleteTier(const FString& SeasonId, const FString& TierId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/seasonpass/admin/namespaces/%s/seasons/%s/tiers/%s"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace, *SeasonId, *TierId);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
