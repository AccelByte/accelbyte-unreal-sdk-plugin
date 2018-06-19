// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "UserLogout.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLogoutOutputPin, FString, ErrorMessage);

UCLASS()
class JUSTICESDK_API UAsyncUserLogout : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FLogoutOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FLogoutOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UAsyncUserLogout* UserLogout();

	virtual void Activate() override;
};