// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "LinkPlatform.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLinkPlatformOutputPin);

UCLASS()
class JUSTICESDK_API UAsyncLinkPlatform : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FLinkPlatformOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FLinkPlatformOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Identity"))
		static UAsyncLinkPlatform* LinkPlatform(FString PlatformID, FString Ticket);

	virtual void Activate() override;

private:
	FString PlatformID;
	FString Ticket;
};