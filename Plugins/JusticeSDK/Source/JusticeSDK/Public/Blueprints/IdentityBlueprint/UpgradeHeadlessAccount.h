// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "UpgradeHeadlessAccount.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpgradeHeadlessAccounOutputPin, FString, ErrorMessage);

UCLASS()
class JUSTICESDK_API UUpgradeHeadlessAccount : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FUpgradeHeadlessAccounOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FUpgradeHeadlessAccounOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UUpgradeHeadlessAccount* UpgradeHeadlessAccount(FString Email, FString Password);

	virtual void Activate() override;

private:
	FString Email;
	FString Password;
};