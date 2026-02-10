// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "Blueprints/AccelByteBPApiClient.h"
#include "Core/AccelByteInstance.h"

#include "AccelByteInstanceBlueprints.generated.h"

UCLASS(BlueprintType, Blueprintable)
class ACCELBYTEUE4SDK_API UAccelByteInstance : public UObject
{
	GENERATED_BODY()

public:
	UAccelByteInstance();
	
	UAccelByteInstance(FAccelByteInstancePtr InAccelByteInstance);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | AccelByteInstance")
	static UAccelByteInstance* CreateAccelByteInstance();

	UFUNCTION(BlueprintCallable, Category = "AccelByte | AccelByteInstance")
	UABApiClient* GetApiClient(FString const Key);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | AccelByteInstance")
	UServerApiClient* GetServerApiClient(FString const Key);
	
	FAccelByteInstancePtr GetAccelByteInstance();

	void SetAccelByteInstance(FAccelByteInstancePtr InAccelByteInstance);

private:
	FAccelByteInstancePtr AccelByteInstance;
};