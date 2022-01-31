// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABServerAchievement.h"

void UABServerAchievement::SetServerApiClient(FServerApiClientPtr NewServerApiClientPtr)
{
	ApiClientPtr = NewServerApiClientPtr;
}

void UABServerAchievement::UnlockAchievement(
	FString const& UserId,
	FString const& AchievementCode,
	FDHandler OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerAchievement.UnlockAchievement(
		UserId,
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
