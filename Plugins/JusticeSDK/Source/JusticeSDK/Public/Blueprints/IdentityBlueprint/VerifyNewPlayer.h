// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "VerifyNewPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVerifyNewPlayerOutputPin);

UCLASS()
class JUSTICESDK_API UAsyncVerifyNewPlayer : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FVerifyNewPlayerOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FVerifyNewPlayerOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UAsyncVerifyNewPlayer* VerifyNewPlayer(FString UserId, FString VerificationCode);

	virtual void Activate() override;

private:
    FString UserId;
    FString VerificationCode;
};