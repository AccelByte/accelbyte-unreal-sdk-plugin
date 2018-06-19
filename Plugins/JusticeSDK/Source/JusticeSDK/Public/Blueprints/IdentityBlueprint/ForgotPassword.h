// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "ForgotPassword.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FForgotPasswordOutputPin, FString, ErrorMessage);

UCLASS()
class JUSTICESDK_API UAsyncForgotPassword : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FForgotPasswordOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FForgotPasswordOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UAsyncForgotPassword* ForgotPassword(FString LoginID);

	virtual void Activate() override;

private:
    FString LoginID;
};