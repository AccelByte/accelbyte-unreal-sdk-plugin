// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABAchievement.h"

using namespace AccelByte;

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
	int32 const& Limit,
	FString const& TagQuery,
	bool bGlobal
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
		Limit,
		TagQuery,
		bGlobal
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
	bool PreferUnlocked,
	FString const& TagQuery
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
		PreferUnlocked,
		TagQuery
	);
}

void UABAchievement::QueryUserAchievementsV2(EAccelByteGlobalAchievementListSortBy const& SortBy,
	FDModelsPaginatedUserAchievementResponse const& OnSuccess, FDErrorHandler const& OnError, int32 const& Offset,
	int32 const& Limit, bool PreferUnlocked, FString const& TagQuery)
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
		PreferUnlocked,
		TagQuery
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

void UABAchievement::QueryGlobalAchievements(
	FString const& AchievementCode,
	EAccelByteGlobalAchievementStatus const& AchievementStatus, 
	EAccelByteGlobalAchievementListSortBy const& SortBy, 
	FDModelsPaginatedUserGlobalAchievementResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 const& Offset, 
	int32 const& Limit)
{
	ApiClientPtr->Achievement.QueryGlobalAchievements(
		AchievementCode,
		AchievementStatus,
		SortBy,
		THandler<FAccelByteModelsPaginatedUserGlobalAchievement>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedUserGlobalAchievement const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int const Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
			), Offset, Limit);
}

void UABAchievement::QueryGlobalAchievementContributors(
	FString const& AchievementCode, 
	EAccelByteGlobalAchievementContributorsSortBy const& SortBy, 
	FDModelsPaginatedGlobalAchievementContributorsResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	int32 const& Offset, 
	int32 const& Limit)
{
	ApiClientPtr->Achievement.QueryGlobalAchievementContributors(
		AchievementCode,
		SortBy,
		THandler<FAccelByteModelsPaginatedGlobalAchievementContributors>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedGlobalAchievementContributors const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int const Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
			), Offset, Limit);
}

void UABAchievement::QueryGlobalAchievementUserContributed(
	FString const& AchievementCode, 
	EAccelByteGlobalAchievementContributorsSortBy const& SortBy, 
	FDModelsPaginatedGlobalAchievementUserContributedResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	int32 const& Offset, 
	int32 const& Limit)
{
	ApiClientPtr->Achievement.QueryGlobalAchievementUserContributed(
		AchievementCode,
		SortBy,
		THandler<FAccelByteModelsPaginatedGlobalAchievementUserContributed>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedGlobalAchievementUserContributed const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int const Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
			), Offset, Limit);
}

void UABAchievement::ClaimGlobalAchievements(
	FString const& AchievementCode,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->Achievement.ClaimGlobalAchievements(
		AchievementCode,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABAchievement::GetTags(
	FString const& Name,
	EAccelByteAchievementListSortBy const& SortBy,
	FDModelsPaginatedPublicTagResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 const& Offset, int32 const& Limit)
{
	ApiClientPtr->Achievement.GetTags(
		Name,
		SortBy,
		THandler<FAccelByteModelsPaginatedPublicTag>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedPublicTag const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int const Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),Offset,Limit);
}
