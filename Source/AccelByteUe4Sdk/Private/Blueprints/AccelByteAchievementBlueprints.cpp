// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteAchievementBlueprints.h"
#include "Api/AccelByteAchievementApi.h"
#include "Core/AccelByteRegistry.h"

using namespace AccelByte;
using AccelByte::Api::Achievement;
using AccelByte::FErrorHandler;
using AccelByte::THandler;

void UAccelByteBlueprintsAchievement::QueryAchievements(const FString& Language, const EAccelByteAchievementListSortBy& SortBy, const FQueryAchievementsSuccess& OnSuccess, const FDErrorHandler& OnError, const int32& Offset = 0, const int32& Limit = 20, const FString& TagQuery = TEXT(""), bool bGlobal = false)
{
	FRegistry::Achievement.QueryAchievements(Language, SortBy, THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda([OnSuccess](const FAccelByteModelsPaginatedPublicAchievement& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}), Offset, Limit, TagQuery, bGlobal);
}

void UAccelByteBlueprintsAchievement::GetAchievement(const FString& AchievementCode, const FGetAchievementSuccess& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::Achievement.GetAchievement(AchievementCode, THandler<FAccelByteModelsMultiLanguageAchievement>::CreateLambda([OnSuccess](const FAccelByteModelsMultiLanguageAchievement& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsAchievement::QueryUserAchievements(const EAccelByteAchievementListSortBy& SortBy, const FQueryUserAchievementsSuccess& OnSuccess, const FDErrorHandler& OnError, const int32& Offset = 0, const int32& Limit = 20, bool preferUnlocked = true, const FString& TagQuery = TEXT(""))
{
	FRegistry::Achievement.QueryUserAchievements(SortBy, THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([OnSuccess](const FAccelByteModelsPaginatedUserAchievement& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}), Offset, Limit, preferUnlocked, TagQuery);
}

void UAccelByteBlueprintsAchievement::UnlockAchievement(const FString& AchievementCode, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::Achievement.UnlockAchievement(AchievementCode, FVoidHandler::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsAchievement::QueryGlobalAchievements(FString const& AchievementCode, EAccelByteGlobalAchievementStatus const& AchievementStatus, EAccelByteGlobalAchievementListSortBy const& SortBy, FQueryUserGlobalAchievementsSuccess const& OnSuccess, FDErrorHandler const& OnError, int32 const& Offset, int32 const& Limit)
{
	FRegistry::Achievement.QueryGlobalAchievements(AchievementCode, AchievementStatus, SortBy, THandler<FAccelByteModelsPaginatedUserGlobalAchievement>::CreateLambda([OnSuccess](const FAccelByteModelsPaginatedUserGlobalAchievement& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
	FErrorHandler::CreateLambda([OnError](const int32& ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}), Offset, Limit);
}

void UAccelByteBlueprintsAchievement::QueryGlobalAchievementContributors(FString const& AchievementCode, EAccelByteGlobalAchievementContributorsSortBy const& SortBy, FQueryGlobalAchievementContributorsSuccess const& OnSuccess, FDErrorHandler const& OnError, int32 const& Offset, int32 const& Limit)
{
	FRegistry::Achievement.QueryGlobalAchievementContributors(AchievementCode, SortBy, THandler<FAccelByteModelsPaginatedGlobalAchievementContributors>::CreateLambda([OnSuccess](const FAccelByteModelsPaginatedGlobalAchievementContributors& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
	FErrorHandler::CreateLambda([OnError](const int32& ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}), Offset, Limit);
}

void UAccelByteBlueprintsAchievement::QueryGlobalAchievementUserContributed(FString const& AchievementCode, EAccelByteGlobalAchievementContributorsSortBy const& SortBy, FQueryGlobalAchievementUserContributedSuccess const& OnSuccess, FDErrorHandler const& OnError, int32 const& Offset, int32 const& Limit)
{
	FRegistry::Achievement.QueryGlobalAchievementUserContributed(AchievementCode, SortBy, THandler<FAccelByteModelsPaginatedGlobalAchievementUserContributed>::CreateLambda([OnSuccess](const FAccelByteModelsPaginatedGlobalAchievementUserContributed& Result)
		{
			OnSuccess.ExecuteIfBound(Result);
		}),
		FErrorHandler::CreateLambda([OnError](const int32& ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}), Offset, Limit);
}

void UAccelByteBlueprintsAchievement::ClaimGlobalAchievements(FString const& AchievementCode, FDHandler const& OnSuccess, FDErrorHandler const& OnError)
{
	FRegistry::Achievement.ClaimGlobalAchievements(AchievementCode, FVoidHandler::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
	FErrorHandler::CreateLambda([OnError](const int32& ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsAchievement::GetTags(const FString& Name, const EAccelByteAchievementListSortBy& SortBy, const FGetTagSuccess& OnSuccess, const FDErrorHandler& OnError, const int32& Offset, const int32& Limit)
{
	FRegistry::Achievement.GetTags(Name, SortBy, THandler<FAccelByteModelsPaginatedPublicTag>::CreateLambda([OnSuccess](const FAccelByteModelsPaginatedPublicTag& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
	FErrorHandler::CreateLambda([OnError](const int32& ErrorCode,const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode,ErrorMessage);
	}), Offset,Limit);
}

