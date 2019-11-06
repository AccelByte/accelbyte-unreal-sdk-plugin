// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Models/AccelByteEcommerceModels.h"
#include "AccelByteCategoryBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsCategory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetRootCategoriesSuccess, const TArray<FAccelByteModelsCategoryInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Category | Api ")
	static void GetRootCategories(const FString& Language, const FGetRootCategoriesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetCategorySuccess, const FAccelByteModelsCategoryInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Category | Api ")
	static void GetCategory(const FString& ParentPath, const FString& Language, const FGetCategorySuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetChildCategoriesSuccess, const TArray<FAccelByteModelsCategoryInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Category | Api ")
	static void GetChildCategories(const FString& Language, const FString& CategoryPath, const FGetChildCategoriesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetDescendantCategoriesSuccess, const TArray<FAccelByteModelsCategoryInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Category | Api ")
	static void GetDescendantCategories(const FString& Language, const FString& CategoryPath, const FGetDescendantCategoriesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
};
