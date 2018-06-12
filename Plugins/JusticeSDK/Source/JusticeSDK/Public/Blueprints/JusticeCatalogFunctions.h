// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Models/UserProfileInfo.h"
#include "Models/Category.h"
#include "Models/ItemInfo.h"
#include "JusticeCatalogFunctions.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FItemCompleteDynamicDelegate, bool, IsSuccess, FString, ErrorStr, const TArray<UItemInfo*>&, ItemArray);

UCLASS()
class JUSTICESDK_API UJusticeCatalogFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "JusticeCatalogFunctions")
	static void GetItemByQuery(FString language, FString region, FString CategoryPath, FString itemType, FString status, int page, int size, FItemCompleteDynamicDelegate OnComplete);
	
};