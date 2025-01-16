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
	const auto AchievementPtr = ApiClientPtr->GetAchievementApi().Pin();
	if (AchievementPtr.IsValid())
	{
		AchievementPtr->QueryAchievements(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAchievement::GetAchievement(
	FString const& AchievementCode,
	FDModelsMultiLanguageAchievementResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto AchievementPtr = ApiClientPtr->GetAchievementApi().Pin();
	if (AchievementPtr.IsValid())
	{
		AchievementPtr->GetAchievement(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
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
	const auto AchievementPtr = ApiClientPtr->GetAchievementApi().Pin();
	if (AchievementPtr.IsValid())
	{
		AchievementPtr->QueryUserAchievements(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAchievement::QueryUserAchievementsV2(EAccelByteGlobalAchievementListSortBy const& SortBy,
	FDModelsPaginatedUserAchievementResponse const& OnSuccess, FDErrorHandler const& OnError, int32 const& Offset,
	int32 const& Limit, bool PreferUnlocked, FString const& TagQuery)
{
	const auto AchievementPtr = ApiClientPtr->GetAchievementApi().Pin();
	if (AchievementPtr.IsValid())
	{
		AchievementPtr->QueryUserAchievements(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAchievement::UnlockAchievement(
	FString const& AchievementCode,
	FDHandler const OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto AchievementPtr = ApiClientPtr->GetAchievementApi().Pin();
	if (AchievementPtr.IsValid())
	{
		AchievementPtr->UnlockAchievement(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
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
	const auto AchievementPtr = ApiClientPtr->GetAchievementApi().Pin();
	if (AchievementPtr.IsValid())
	{
		AchievementPtr->QueryGlobalAchievements(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAchievement::QueryGlobalAchievementContributors(
	FString const& AchievementCode, 
	EAccelByteGlobalAchievementContributorsSortBy const& SortBy, 
	FDModelsPaginatedGlobalAchievementContributorsResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	int32 const& Offset, 
	int32 const& Limit)
{
	const auto AchievementPtr = ApiClientPtr->GetAchievementApi().Pin();
	if (AchievementPtr.IsValid())
	{
		AchievementPtr->QueryGlobalAchievementContributors(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAchievement::QueryGlobalAchievementUserContributed(
	FString const& AchievementCode, 
	EAccelByteGlobalAchievementContributorsSortBy const& SortBy, 
	FDModelsPaginatedGlobalAchievementUserContributedResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	int32 const& Offset, 
	int32 const& Limit)
{
	const auto AchievementPtr = ApiClientPtr->GetAchievementApi().Pin();
	if (AchievementPtr.IsValid())
	{
		AchievementPtr->QueryGlobalAchievementUserContributed(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAchievement::ClaimGlobalAchievements(
	FString const& AchievementCode,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto AchievementPtr = ApiClientPtr->GetAchievementApi().Pin();
	if (AchievementPtr.IsValid())
	{
		AchievementPtr->ClaimGlobalAchievements(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAchievement::GetTags(
	FString const& Name,
	EAccelByteAchievementListSortBy const& SortBy,
	FDModelsPaginatedPublicTagResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 const& Offset, int32 const& Limit)
{
	const auto AchievementPtr = ApiClientPtr->GetAchievementApi().Pin();
	if (AchievementPtr.IsValid())
	{
		AchievementPtr->GetTags(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}
