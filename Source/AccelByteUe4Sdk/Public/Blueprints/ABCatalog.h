// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ABCatalog.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

USTRUCT(BlueprintType)
struct FArrayModelsCategoryInfoResponse
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	TArray<FAccelByteModelsCategoryInfo> Content;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayModelsCategoryInfoResponse, FArrayModelsCategoryInfoResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsCategoryInfoResponse, FAccelByteModelsCategoryInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPopulatedItemInfoResponse, FAccelByteModelsPopulatedItemInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsItemInfoResponse, FAccelByteModelsItemInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsItemPagingSlicedResultResponse, FAccelByteModelsItemPagingSlicedResult, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsItemDynamicDataResponse, FAccelByteModelsItemDynamicData, Response);

#pragma endregion 

//Combination of Category and Item API
UCLASS(Blueprintable, BlueprintType)
class UABCatalog : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

//Category
public: 
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Catalog")
	void GetRootCategories(FString const& Language, FDArrayModelsCategoryInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Catalog")
	void GetCategory(FString const& CategoryPath, FString const& Language, FDModelsCategoryInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Catalog")
	void GetChildCategories(FString const& CategoryPath, FString const& Language, FDArrayModelsCategoryInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Catalog")
	void GetDescendantCategories(FString const& CategoryPath, FString const& Language, FDArrayModelsCategoryInfoResponse OnSuccess, FDErrorHandler OnError);

//Item
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Catalog")
	void GetItemById(FString const& ItemId, FString const& Language, FString const& Region, const FString& StoreId, bool bPopulateBundle, FDModelsPopulatedItemInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Catalog")
	void GetItemByAppId(FString const& AppId, FString const& Language, FString const& Region, FDModelsItemInfoResponse OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Catalog")
	void GetItemsByCriteria(FAccelByteModelsItemCriteria const& ItemCriteria, int32 const& Offset, int32 const& Limit, TArray<EAccelByteItemListSortBy> SortBy, FDModelsItemPagingSlicedResultResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Catalog")
	void SearchItem(FString const& Language, FString const& Keyword, int32 const& Offset, int32 const& Limit, FString const& Region, FDModelsItemPagingSlicedResultResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Catalog")
	void GetItemDynamicData(FString const& ItemId, FDModelsItemDynamicDataResponse OnSuccess, FDErrorHandler OnError);

private:
	FApiClientPtr ApiClientPtr;
};
