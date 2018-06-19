// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OAuthTokenJustice.h"
#include "Services/JusticeIdentity.h"
#include "GetItemByQuery.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetItemByQueryOutputPin, const TArray<UItemInfo*>&, ItemArray);

UCLASS()
class JUSTICESDK_API UGetItemByQuery : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FGetItemByQueryOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FGetItemByQueryOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Catalog"))
		static UGetItemByQuery* GetItemByQuery(FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size);

	virtual void Activate() override;

private:
    FString Language;
    FString Region;
    FString CategoryPath;
    FString ItemType;
    FString Status;
    int32 Page;
    int32 Size;
};