// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "Services/JusticeIdentity.h"
#include "RegisterNewPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRegisterPlayerOutputPin, const UUserCreateResponse*, CreateResponse, FString, ErrorMessage);

UCLASS()
class JUSTICESDK_API UAsyncRegisterNewPlayer : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FRegisterPlayerOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FRegisterPlayerOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UAsyncRegisterNewPlayer* RegisterNewPlayer(FString UserID, FString Password, FString DisplayName, UUserAuthTypeJustice AuthType);

	virtual void Activate() override;

private:
    FString UserID;
    FString Password;
    FString DisplayName;
	FUserAuthTypeJustice AuthType;
};