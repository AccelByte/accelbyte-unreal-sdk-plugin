// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteModelsCategory.h"
#include "AccelByteBlueprintsCategory.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsCategory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintError, int32, ErrorCode, FString, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetRootCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Category")
	static void GetRootCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FGetRootCategoriesSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Category")
	static void GetRootCategoriesEasy(FString Language, FGetRootCategoriesSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetCategorySuccess, const FAccelByteModelsFullCategoryInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Category")
	static void GetCategory(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString ParentPath, FString Language, FGetCategorySuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Category")
	static void GetCategoryEasy(FString ParentPath, FString Language, FGetCategorySuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetChildCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Category")
	static void GetChildCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Category")
	static void GetChildCategoriesEasy(FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetDescendantCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Category")
	static void GetDescendantCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetDescendantCategoriesSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Category")
	static void GetDescendantCategoriesEasy(FString Language, FString CategoryPath, FGetDescendantCategoriesSuccess OnSuccess, FBlueprintError OnError);

};