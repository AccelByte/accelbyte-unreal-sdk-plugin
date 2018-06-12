// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "UnlinkPlatform.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUnlinkPlatformOutputPin);

UCLASS()
class JUSTICESDK_API UAsyncUnlinkPlatform : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FUnlinkPlatformOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FUnlinkPlatformOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UAsyncUnlinkPlatform* UnlinkPlatform(FString PlatformID);

	virtual void Activate() override;

private:
	FString PlatformID;
};