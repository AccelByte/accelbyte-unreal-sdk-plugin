// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteModelsItem.h"
#include "AccelByteBlueprintsItem.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsItem : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintError, int32, ErrorCode, FString, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetItemByIdSuccess, const FAccelByteModelsItemInfo&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Item")
	static void GetItemById(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString ItemId, FString Region, FString Language, FGetItemByIdSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Item")
	static void GetItemByIdEasy(FString ItemId, FString Region, FString Language, FGetItemByIdSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetItemsByCriteriaSuccess, const FAccelByteModelsItemPagingSlicedResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Item")
	static void GetItemsByCriteria(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size, FGetItemsByCriteriaSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | Item")
	static void GetItemsByCriteriaEasy(FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size, FGetItemsByCriteriaSuccess OnSuccess, FBlueprintError OnError);
};
