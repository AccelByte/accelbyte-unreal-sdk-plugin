// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Private/Models/UserProfileJustice.h"

#include "JusticePlatformFunctions.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FProfileReqestCompleteDelegate, bool, IsSuccess, FString, ErrorStr, UUserProfileJustice*, PlayerProfile);

UCLASS()
class JUSTICESDK_API UJusticePlatformFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
		
	UFUNCTION(BlueprintCallable, Category = "JusticePlatformFunctions")
	static void RequestCurrentPlayerProfile(FProfileReqestCompleteDelegate OnComplete);
	
};
