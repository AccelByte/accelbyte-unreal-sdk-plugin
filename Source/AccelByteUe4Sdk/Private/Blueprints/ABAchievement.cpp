// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABAchievement.h"

void UABAchievement::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABAchievement::QueryAchievements(
	FString const& Language,
	EAccelByteAchievementListSortBy const& SortBy,
	FDModelsPaginatedPublicAchievementResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 const& Offset,
	int32 const& Limit
)
{
	ApiClientPtr->Achievement.QueryAchievements(
		Language,
		SortBy,
		THandler<FAccelByteModelsPaginatedPublicAchievement>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedPublicAchievement const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		Offset,
		Limit
	);
}

void UABAchievement::GetAchievement(
	FString const& AchievementCode,
	FDModelsMultiLanguageAchievementResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->Achievement.GetAchievement(
		AchievementCode,
		THandler<FAccelByteModelsMultiLanguageAchievement>::CreateLambda(
			[OnSuccess](FAccelByteModelsMultiLanguageAchievement const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABAchievement::QueryUserAchievements(
	EAccelByteAchievementListSortBy const& SortBy,
	FDModelsPaginatedUserAchievementResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 const& Offset,
	int32 const& Limit,
	bool PreferUnlocked
)
{
	ApiClientPtr->Achievement.QueryUserAchievements(
		SortBy,
		THandler<FAccelByteModelsPaginatedUserAchievement>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedUserAchievement const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		Offset,
		Limit,
		PreferUnlocked
	);
}

void UABAchievement::UnlockAchievement(
	FString const& AchievementCode,
	FDHandler const OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->Achievement.UnlockAchievement(
		AchievementCode,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}