// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Private/Models/UserProfileInfo.h"
#include "Private/Models/Category.h"
#include "Private/Models/ItemInfo.h"
#include "JusticeCatalogFunctions.generated.h"

//DECLARE_DYNAMIC_DELEGATE_ThreeParams(FGetRootCategoryCompleteDelegate, bool, IsSuccess, FString, ErrorStr, UCategory*, Categories);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FItemCompleteDynamicDelegate, bool, IsSuccess, FString, ErrorStr, const TArray<UItemInfo*>&, ItemArray);


UCLASS()
class JUSTICESDK_API UJusticeCatalogFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		
	
	//static void GetRootCategory(FGetRootCategoryCompleteDelegate OnComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeCatalogFunctions")
	static void GetItemByCriteria(FString CategoryPath, FItemCompleteDynamicDelegate OnComplete);

	
};
