// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "WebLoginRefresh.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWebLoginRefreshOutputPin, FString, ErrorMessage);

UCLASS()
class JUSTICESDK_API UWebLoginRefresh : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FWebLoginRefreshOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FWebLoginRefreshOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UWebLoginRefresh* WebLoginRefresh(FString UserRefreshToken);

	virtual void Activate() override;

private:
	FString UserRefreshToken;
};