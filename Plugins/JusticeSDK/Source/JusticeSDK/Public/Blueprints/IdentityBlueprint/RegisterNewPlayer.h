// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "RegisterNewPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRegisterPlayerOutputPin, const UUserCreateResponse*, CreateResponse);

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
		static UAsyncRegisterNewPlayer* RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, FString AuthType);

	virtual void Activate() override;

private:
    FString UserId;
    FString Password;
    FString DisplayName;
    FString AuthType;
};