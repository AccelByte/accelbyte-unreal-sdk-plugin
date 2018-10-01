// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AccelByteModelsCategory.h"
#include "AccelByteBlueprintsCategory.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ACCELBYTEUE4SDK_API UAccelByteCatalogGetChildCategories : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorDelegate, int32, ErrorCode, FString, ErrorMessage);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetChildCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&, Result);
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "AccelByte | Services | Catalog")
	static UAccelByteCatalogGetChildCategories* GetChildCategories(FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, FBlueprintErrorDelegate OnError);
	virtual void Activate() override;
private:
	FString Language;
	FString CategoryPath;
	FGetChildCategoriesSuccess OnSuccess;
	FBlueprintErrorDelegate OnError;
};