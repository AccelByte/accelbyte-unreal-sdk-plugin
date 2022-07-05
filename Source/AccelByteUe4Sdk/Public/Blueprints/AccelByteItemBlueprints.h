// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Models/AccelByteEcommerceModels.h"
#include "AccelByteItemBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsItem : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetItemByIdSuccess, const FAccelByteModelsPopulatedItemInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Item | Api ")
	static void GetItemById(const FString& ItemId, const FString& Region, const FString& Language, const FGetItemByIdSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetItemByAppIdSuccess, const FAccelByteModelsItemInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Item | Api ")
	void GetItemByAppId(const FString& AppId, const FString& Region, const FString& Language, const FGetItemByAppIdSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetItemsByCriteriaSuccess, const FAccelByteModelsItemPagingSlicedResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Item | Api ")
	static void GetItemsByCriteria(const FAccelByteModelsItemCriteria& ItemCriteria, const int32& Offset, const int32& Limit, TArray<EAccelByteItemListSortBy> SortBy,
		const FGetItemsByCriteriaSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FSearchItemSuccess, const FAccelByteModelsItemPagingSlicedResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Item | Api ")
	static void SearchItem(const FString& Language, const FString& Keyword, int32 Page, int32 Size, const FString& Region, const FSearchItemSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
};