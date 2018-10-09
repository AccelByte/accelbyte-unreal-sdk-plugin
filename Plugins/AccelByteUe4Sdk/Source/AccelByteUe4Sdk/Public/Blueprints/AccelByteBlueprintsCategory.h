// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteModelsCategory.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"
#include "AccelByteBlueprintsCategory.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsCategory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintError, int32, ErrorCode, FString, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetRootCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Categories")
	/**
	 * @brief This function gets root categories that exist in the same namespace with user's namespace.
	 *
	 * @param Language Optional.
	 * @param OnSuccess Required, but can be nullptr. This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsFullCategoryInfo>&.
	 * @param OnError Required, but can be nullptr. This will be called when the operation failed.
	 */
	static void GetRootCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FGetRootCategoriesSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Categories")
	static void GetRootCategoriesEasy(FString Language, FGetRootCategoriesSuccess OnSuccess, FBlueprintError OnError){GetRootCategories(AccelByte::Settings::ServerBaseUrl, AccelByte::CredentialStore.GetUserAccessToken(), AccelByte::Settings::Namespace, Language, OnSuccess, OnError); }

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Categories")
	/**
	 * @brief This function gets the category from a store in a namespace.
	 *
	 * @param ParentPath Required.
	 * @param Language Optional.
	 * @param OnSuccess Required, but can be nullptr. This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsFullCategoryInfo>&.
	 * @param OnError Required, but can be nullptr. This will be called when the operation failed.
	 */
	static void GetCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString ParentPath, FString Language, FGetCategoriesSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Categories")
	static void GetCategoriesEasy(FString ParentPath, FString Language, FGetCategoriesSuccess OnSuccess, FBlueprintError OnError){GetCategories(AccelByte::Settings::ServerBaseUrl, AccelByte::CredentialStore.GetUserAccessToken(), AccelByte::Settings::Namespace, ParentPath, Language, OnSuccess, OnError); }

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetChildCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Categories")
	/**
	 * @brief This function gets the subcategories from a category.
	 *
	 * @param Language Optional.
	 * @param CategoryPath Required. Parent path.
	 * @param OnSuccess Required, but can be nullptr. This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsFullCategoryInfo>&.
	 * @param OnError Required, but can be nullptr. This will be called when the operation failed.
	 */
	static void GetChildCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Categories")
	static void GetChildCategoriesEasy(FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, FBlueprintError OnError){ GetChildCategories(AccelByte::Settings::ServerBaseUrl, AccelByte::CredentialStore.GetUserAccessToken(), AccelByte::Settings::Namespace, Language, CategoryPath, OnSuccess, OnError); };

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetDescendantCategoriesSuccess, const TArray<FAccelByteModelsFullCategoryInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Categories")
	/**
	 * @brief Get every subcategories from a category.
	 *
	 * @param Language Optional.
	 * @param CategoryPath Required.
	 * @param OnSuccess Required, but can be nullptr. This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsFullCategoryInfo>&.
	 * @param OnError Required, but can be nullptr. This will be called when the operation failed.
	 */
	static void GetDescendantCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetDescendantCategoriesSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Categories")
	static void GetDescendantCategoriesEasy(FString Language, FString CategoryPath, FGetDescendantCategoriesSuccess OnSuccess, FBlueprintError OnError) { GetDescendantCategories(AccelByte::Settings::ServerBaseUrl, AccelByte::CredentialStore.GetUserAccessToken(), AccelByte::Settings::Namespace, Language, CategoryPath, OnSuccess, OnError); }

};