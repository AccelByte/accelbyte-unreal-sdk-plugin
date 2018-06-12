// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/LinkedPlatform.h"
#include "GetLinkedPlatform.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetLinkedPlatformOutputPin, const TArray<ULinkedPlatform*>&, LinkedPlatforms);

UCLASS()
class JUSTICESDK_API UAsyncGetLinkedPlatform : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FGetLinkedPlatformOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FGetLinkedPlatformOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UAsyncGetLinkedPlatform* GetLinkedPlatform();

	virtual void Activate() override;
};