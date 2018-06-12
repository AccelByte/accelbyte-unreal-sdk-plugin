// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "Services/JusticeIdentity.h"
#include "VerifyNewPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVerifyNewPlayerOutputPin, FString, ErrorMessage);

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
		static UAsyncVerifyNewPlayer* VerifyNewPlayer(FString UserID, FString VerificationCode, UUserAuthTypeJustice AuthType);

	virtual void Activate() override;

private:
    FString UserID;
    FString VerificationCode;
	FUserAuthTypeJustice AuthType;
};