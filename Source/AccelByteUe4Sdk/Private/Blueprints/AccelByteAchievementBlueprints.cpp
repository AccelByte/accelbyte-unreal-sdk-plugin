// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteAchievementBlueprints.h"
#include "Api/AccelByteAchievementApi.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::Api::Achievement;
using AccelByte::FErrorHandler;
using AccelByte::THandler;

void UAccelByteBlueprintsAchievement::QueryAchievements(const FString& Language, const EAccelByteAchievementListSortBy& SortBy, const FQueryAchievementsSuccess& OnSuccess, const FDErrorHandler& OnError, const int32& Offset = 0, const int32& Limit = 20, const FString& TagQuery = TEXT(""))
{
	FRegistry::Achievement.QueryAchievements(Language, SortBy,THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda([OnSuccess](const FAccelByteModelsPaginatedPublicAchievement& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}), Offset, Limit,TagQuery);
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

void UAccelByteBlueprintsAchievement::QueryUserAchievements(const EAccelByteAchievementListSortBy& SortBy, const FQueryUserAchievementsSuccess& OnSuccess, const FDErrorHandler& OnError, const int32& Offset = 0, const int32& Limit = 20, bool preferUnlocked = true,const FString& TagQuery = TEXT(""))
{
	FRegistry::Achievement.QueryUserAchievements(SortBy, THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([OnSuccess](const FAccelByteModelsPaginatedUserAchievement& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}), Offset, Limit,preferUnlocked,TagQuery);
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

void UAccelByteBlueprintsAchievement::GetTags(const FString& Name, const EAccelByteAchievementListSortBy& SortBy, const FGetTagSuccess& OnSuccess, const FDErrorHandler& OnError, const int32& Offset, const int32& Limit)
{
	FRegistry::Achievement.GetTags(
		Name,SortBy,
		THandler<FAccelByteModelsPaginatedPublicTag>::CreateLambda([OnSuccess](const FAccelByteModelsPaginatedPublicTag& Result)
		{
			OnSuccess.ExecuteIfBound(Result);
		}),
		FErrorHandler::CreateLambda([OnError](const int32& ErrorCode,const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode,ErrorMessage);
		}),
		Offset,Limit);
}

