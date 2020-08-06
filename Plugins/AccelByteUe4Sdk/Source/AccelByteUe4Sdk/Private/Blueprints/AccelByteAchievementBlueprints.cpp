// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteAchievementBlueprints.h"
#include "Api/AccelByteAchievementApi.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::Api::Achievement;
using AccelByte::FErrorHandler;
using AccelByte::THandler;

void UAccelByteBlueprintsAchievement::QueryAchievements(const FString& Language, const EAccelByteAchievementListSortBy& SortBy, const FQueryAchievementsSuccess& OnSuccess, const FDErrorHandler& OnError, const int32& Offset = 0, const int32& Limit = 20)
{
	FRegistry::Achievement.QueryAchievements(Language, SortBy, THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda([OnSuccess](const FAccelByteModelsPaginatedPublicAchievement& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}), Offset, Limit);
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

void UAccelByteBlueprintsAchievement::QueryUserAchievements(const EAccelByteAchievementListSortBy& SortBy, const FQueryUserAchievementsSuccess& OnSuccess, const FDErrorHandler& OnError, const int32& Offset = 0, const int32& Limit = 20)
{
	FRegistry::Achievement.QueryUserAchievements(SortBy, THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda([OnSuccess](const FAccelByteModelsPaginatedUserAchievement& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}), Offset, Limit);
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
