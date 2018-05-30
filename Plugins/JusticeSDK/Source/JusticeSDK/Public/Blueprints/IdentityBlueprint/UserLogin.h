// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "UserLogin.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoginOutputPin, const UOAuthTokenJustice*, Token);

UCLASS()
class JUSTICESDK_API UAsyncUserLogin : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FLoginOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FLoginOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UAsyncUserLogin* UserLogin(FString UserId, FString Password);

	virtual void Activate() override;

private:
	FString UserId;
	FString Password;	
};