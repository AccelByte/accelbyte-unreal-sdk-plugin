// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/ItemInfo.h"
#include "Services/JusticeIdentity.h"
#include "GetItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetItemOutputPin, const UItemInfo*, Item);

UCLASS()
class JUSTICESDK_API UGetItem : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FGetItemOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FGetItemOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Catalog"))
		static UGetItem* GetItem(FString ItemID, FString Region, FString Language);

	virtual void Activate() override;

private:
    FString ItemID;
    FString Region;
    FString Language;
};