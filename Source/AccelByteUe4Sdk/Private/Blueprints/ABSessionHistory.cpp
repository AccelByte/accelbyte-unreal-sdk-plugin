// Copyright (c) 2023 AccelByte, Inc. All rights reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABSessionHistory.h"

using namespace AccelByte;

void UABSessionHistory::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABSessionHistory::QueryGameSessionHistory(
	FDModelsV2SessionHistoriesResult const& OnSuccess,
	FDErrorHandler const& OnError,
	EAccelByteGeneralSortBy const& SortBy,
	int32 Offset,
	int32 Limit
)
{
	ApiClientPtr->SessionHistory.QueryGameSessionHistory(
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