﻿// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

#include "TestUtilities.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteRegistry.h"
#include "Models/AccelByteSeasonPassModels.h"

#include "SeasonPassTestAdmin.generated.h"

using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::FVoidHandler;


USTRUCT()
struct FSeasonPassCreateRequest
{
	GENERATED_BODY()
	UPROPERTY()
	FString Name;
	UPROPERTY()
	FString Start;
	UPROPERTY()
	FString End;
	UPROPERTY()
	FString DefaultLanguage;
	UPROPERTY()
	int32 DefaultRequiredExp;
	UPROPERTY()
	FString DraftStoreId;
	UPROPERTY()
	FString TierItemId;
	UPROPERTY()
	bool AutoClaim;
	UPROPERTY()
	FAccelByteSeasonPassExcessStrategy ExcessStrategy;
	UPROPERTY()
	TMap<FString, FLocalization> Localizations;
	UPROPERTY()
	TArray<FAccelByteModelsItemImage> Images;
};

USTRUCT()
struct FSeasonPassCreateSeasonResponse : public FSeasonPassCreateRequest
{
	GENERATED_BODY()
	UPROPERTY()
	FString Id;
	UPROPERTY()
	FString Namespace;
	UPROPERTY()
	FString TierItemName;
	UPROPERTY()
	TArray<FString> PassCodes;
	UPROPERTY()
	EAccelByteSeasonPassStatus Status;
	UPROPERTY()
	FDateTime PublishedAt;
	UPROPERTY()
	FDateTime CreatedAt;
	UPROPERTY()
	FDateTime UpdatedAt;
};

USTRUCT()
struct FSeasonPassCreateGetSeasonResponse
{
	GENERATED_BODY()
	UPROPERTY()
	FString Id;
	UPROPERTY()
	FString Namespace;
	UPROPERTY()
	FString Name;
	UPROPERTY()
	FDateTime Start;
	UPROPERTY()
	FDateTime End;
	UPROPERTY()
	FString DefaultLanguage;
	UPROPERTY()
	TArray<FString> PassCodes;
	UPROPERTY()
	EAccelByteSeasonPassStatus Status;
	UPROPERTY()
	FDateTime PublishedAt;
	UPROPERTY()
	FDateTime CreatedAt;
	UPROPERTY()
	FDateTime UpdatedAt;
};

USTRUCT()
struct FSeasonPassCreateGetSeasonsPagingResponse
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<FSeasonPassCreateGetSeasonResponse> Data;
	UPROPERTY()
	FAccelByteModelsPaging Paging;
};

USTRUCT()
struct FSeasonPassCreatePassRequest
{
	GENERATED_BODY()
	UPROPERTY()
	FString Code;
	UPROPERTY()
	int32 DisplayOrder;
	UPROPERTY()
	bool AutoEnroll;
	UPROPERTY()
	FString PassItemId;
	UPROPERTY()
	TMap<FString, FLocalization> Localizations;
	UPROPERTY()
	TArray<FAccelByteModelsItemImage> Images;
};

USTRUCT()
struct FSeasonPassCreatePassResponse : public FSeasonPassCreatePassRequest
{
	GENERATED_BODY()
	UPROPERTY()
	FString SeasonId;
	UPROPERTY()
	FString PassItemName;
	UPROPERTY()
	FDateTime CreatedAt;
	UPROPERTY()
	FDateTime UpdatedAt;
};

USTRUCT()
struct FSeasonPassCreateReward
{
	GENERATED_BODY()
	UPROPERTY()
	FString Code;
	UPROPERTY()
	FString Type; // this should be enum, but backend currently supported only ITEM
	UPROPERTY()
	FString ItemId;
	UPROPERTY()
	int32 Quantity;
	UPROPERTY()
	FAccelByteModelsItemImage Image;
};

USTRUCT()
struct FSeasonPassRewardResponse : public FSeasonPassCreateReward
{
	GENERATED_BODY()
	UPROPERTY()
	FString Namespace;
	UPROPERTY()
	FString SeasonId;
	UPROPERTY()
	FString ItemName;
};

USTRUCT()
struct FSeasonPassTierRequest
{
	GENERATED_BODY()
	UPROPERTY()
	int32 RequiredExp;
	UPROPERTY()
	FJsonObjectWrapper Rewards;
};

USTRUCT()
struct FSeasonPassCreateTierRequest
{
	GENERATED_BODY()
	UPROPERTY()
	int32 Index;
	UPROPERTY()
	int32 Quantity;
	UPROPERTY()
	FSeasonPassTierRequest Tier;
};

void SeasonCreateSeason(const FSeasonPassCreateRequest& Req, const THandler<FSeasonPassCreateSeasonResponse>& OnSuccess,
						const FErrorHandler& OnError);
void SeasonQuerySeason(const TArray<EAccelByteSeasonPassStatus>& Status,
						const THandler<FSeasonPassCreateGetSeasonsPagingResponse>& OnSuccess,
						const FErrorHandler& OnError, int32 offset = 0, int32 limit = 20);
void SeasonDeleteSeason(const FString& SeasonId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void SeasonPublishSeason(const FString& SeasonId, const THandler<FSeasonPassCreateSeasonResponse>& OnSuccess,
						const FErrorHandler& OnError);
void SeasonForceUnpublishSeason(const FString& SeasonId, const THandler<FSeasonPassCreateSeasonResponse>& OnSuccess,
								const FErrorHandler& OnError);
void SeasonCreatePass(const FString& SeasonId, const FSeasonPassCreatePassRequest& Req,
					const THandler<FSeasonPassCreatePassResponse>& OnSuccess, const FErrorHandler& OnError);
void SeasonDeletePass(const FString& SeasonId, const FString& PassCode, const FSimpleDelegate& OnSuccess,
					const FErrorHandler& OnError);
void SeasonCreateReward(const FString& SeasonId, const FSeasonPassCreateReward& Req,
						const THandler<FSeasonPassRewardResponse>& OnSuccess, const FErrorHandler& OnError);
void SeasonDeleteReward(const FString& SeasonId, const FString& RewardCode, const FSimpleDelegate& OnSuccess,
						const FErrorHandler& OnError);
void SeasonCreateTier(const FString& SeasonId, const FSeasonPassCreateTierRequest& Req,
					const THandler<TArray<FAccelByteModelsSeasonPassTierJsonObject>>& OnSuccess,
					const FErrorHandler& OnError);
void SeasonModifyTier(const FString& SeasonId, const FString& TierId, const FSeasonPassTierRequest& Req,
					const THandler<TArray<FAccelByteModelsSeasonPassTierJsonObject>>& OnSuccess,
					const FErrorHandler& OnError);
void SeasonDeleteTier(const FString& SeasonId, const FString& TierId, const FSimpleDelegate& OnSuccess,
					const FErrorHandler& OnError);