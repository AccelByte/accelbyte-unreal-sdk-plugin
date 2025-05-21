// Copyright (c) 2023 AccelByte, Inc. All rights reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABSessionHistory.h"

using namespace AccelByte;

void UABSessionHistory::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

// This blueprint is deprecated and will be removed on AGS 2025.4. 
void UABSessionHistory::QueryGameSessionHistory(
	FDModelsV2SessionHistoriesResult const& OnSuccess,
	FDErrorHandler const& OnError,
	EAccelByteGeneralSortBy const& SortBy,
	int32 Offset,
	int32 Limit
)
{
	const auto SessionHistoryPtr = ApiClientPtr->GetSessionHistoryApi().Pin();
	if (SessionHistoryPtr.IsValid())
	{
		SessionHistoryPtr->QueryGameSessionHistory(
		THandler<FAccelByteModelsGameSessionHistoriesResult>::CreateLambda(
			[OnSuccess](FAccelByteModelsGameSessionHistoriesResult const& Response)
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
			SortBy,
			Offset,
			Limit
		);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}