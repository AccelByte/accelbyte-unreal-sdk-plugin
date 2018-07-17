// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/Category.h"
#include "Services/JusticeIdentity.h"
#include "GetChildCategory.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetChildCategoryOutputPin, const TArray<UCategory*>&, Categories);

UCLASS()
class JUSTICESDK_API UGetChildCategory : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
		FGetChildCategoryOutputPin OnSuccess;

	UPROPERTY(BlueprintAssignable)
		FGetChildCategoryOutputPin OnFailed;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Catalog"))
		static UGetChildCategory* GetChildCategory(FString Language, FString CategoryPath);

	virtual void Activate() override;

private:
    FString Language;
    FString CategoryPath;
};