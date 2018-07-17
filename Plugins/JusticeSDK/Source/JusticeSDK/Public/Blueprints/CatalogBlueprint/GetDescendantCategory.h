// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/Category.h"
#include "Services/JusticeIdentity.h"
#include "GetDescendantCategory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetDescendantCategoryOutputPin, const TArray<UCategory*>&, Categories);

UCLASS()
class JUSTICESDK_API UGetDescendantCategory : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FGetDescendantCategoryOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FGetDescendantCategoryOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Catalog"))
		static UGetDescendantCategory* GetDescendantCategory(FString Language, FString CategoryPath);

	virtual void Activate() override;

private:
    FString Language;
    FString CategoryPath;
};