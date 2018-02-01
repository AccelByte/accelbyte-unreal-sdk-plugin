// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreMinimal.h"
#include "JusticeBlueprintFunction.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FLoginCompleteMulticastDelegate, bool, IsSuccess, FString, Token, FString, ErrorStr);

UCLASS()
class ONLINESUBSYSTEMJUSTICE_API UJusticeBlueprintFunction : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "JusticeBlueprintFunction")
		static bool Logout();

	UFUNCTION(BlueprintCallable, Category = "JusticeBlueprintFunction")
		static bool Login(FString UserId, FString Password, FLoginCompleteMulticastDelegate onComplete);

	UFUNCTION(BlueprintCallable, Category = "JusticeBlueprintFunction")
		static UFJusticeComponent* GetJusticeComponent();

};

