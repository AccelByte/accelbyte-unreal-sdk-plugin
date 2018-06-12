// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "DeviceLogin.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDeviceLoginOutputPin, const UOAuthTokenJustice*, Token, FString, ErrorMessage);

UCLASS()
class JUSTICESDK_API UAsyncDeviceLogin : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FDeviceLoginOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FDeviceLoginOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UAsyncDeviceLogin* DeviceLogin();

	virtual void Activate() override;

};