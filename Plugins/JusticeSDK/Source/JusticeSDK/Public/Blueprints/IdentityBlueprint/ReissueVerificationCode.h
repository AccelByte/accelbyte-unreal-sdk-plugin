// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "ReissueVerificationCode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReissueVerificationCodeOutputPin);

UCLASS()
class JUSTICESDK_API UAsyncReissueVerificationCode : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FReissueVerificationCodeOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FReissueVerificationCodeOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UAsyncReissueVerificationCode* ReissueVerificationCode(FString UserId, FString LoginId);

	virtual void Activate() override;

private:
    FString UserId;
    FString LoginId;
};