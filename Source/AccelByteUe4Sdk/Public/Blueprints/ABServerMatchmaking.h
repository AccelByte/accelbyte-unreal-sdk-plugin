// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteMatchmakingModels.h"
#include "Core/AccelByteServerApiClient.h"
#include "ABServerMatchmaking.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FDMatchmakingResultDelegate, FAccelByteModelsMatchmakingResult, Response);

UCLASS(Blueprintable, BlueprintType)
class UABServerMatchmaking final : public UObject
{
	GENERATED_BODY()
public:
	void SetServerApiClient(AccelByte::FServerApiClientPtr const& NewServerApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Matchmaking | Api")
	void EnqueueJoinableSession(FAccelByteModelsMatchmakingResult const& MatchmakingResult, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Matchmaking | Api")
	void DequeueJoinableSession(const FString& MatchId, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Matchmaking | Api")
	void AddUserToSession(FString const& ChannelName, FString const& MatchId, FString const& UserId, FString const& OptionalPartyId, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Matchmaking | Api")
	void RemoveUserFromSession(FString const& ChannelName, FString const& MatchId, FString const& UserId, FAccelByteModelsMatchmakingResult const& OptionalBody, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Matchmaking | Api")
	void QuerySessionStatus(FString const& MatchId, FDMatchmakingResultDelegate OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Matchmaking | Api")
	void RebalanceMatchmakingBasedOnMMR(FString const& MatchId, FDMatchmakingResultDelegate OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Matchmaking | Api")
	void ActivateSessionStatusPolling(FString const& MatchId, int IntervalSec, FDMatchmakingResultDelegate OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Matchmaking | Api")
	void DeactivateStatusPolling();

private:
	AccelByte::FServerApiClientPtr ApiClientPtr;
};
