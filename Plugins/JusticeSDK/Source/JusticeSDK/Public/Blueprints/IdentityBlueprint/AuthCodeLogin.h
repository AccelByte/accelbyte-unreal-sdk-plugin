// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "AuthCodeLogin.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuthCodeLoginOutputPin, FString, ErrorMessage);

UCLASS()
class JUSTICESDK_API UAsyncAuthCodeLogin : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FAuthCodeLoginOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FAuthCodeLoginOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UAsyncAuthCodeLogin* AuthCodeLogin();

	virtual void Activate() override;

private:
	FString AuthCode;
	FString RedirectURI;	
};