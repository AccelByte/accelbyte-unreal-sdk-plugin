// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Models/AccelByteCloudStorageModels.h"
#include "AccelByteCloudStorageBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsCloudStorage : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetAllSlotsSuccess, const TArray<FAccelByteModelsSlot>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudStorage | Api ")
	static void GetAllSlots(const FGetAllSlotsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetSlotSuccess, const TArray<uint8>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudStorage | Api ")
	static void GetSlot(const FString& SlotId, const FGetSlotSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateSlotsSuccess, const FAccelByteModelsSlot&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudStorage | Api ")
	static void CreateSlot(const TArray<uint8>& Data, const FString& FileName, const TArray<FString>& Tags, const FString& Label, const FString& CustomAttribute, const FCreateSlotsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateSlotSuccess, const FAccelByteModelsSlot&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudStorage | Api ")
	static void UpdateSlot(const FString& SlotId, const FString& FileName, const TArray<uint8>& Data, const TArray<FString>& Tags, const FString& Label, const FString& CustomAttribute, const FUpdateSlotSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateSlotMetadataSuccess, const FAccelByteModelsSlot&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudStorage | Api ", meta = (DeprecatedFunction,
		DeprecationMessage = "Filename parameter will be igonred"))
	static void UpdateSlotMetadata(const FString& SlotId, const FString& FileName, const TArray<FString>& Tags, const FString& Label, const FString& CustomAttribute, const FUpdateSlotMetadataSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
	
	DECLARE_DYNAMIC_DELEGATE(FDeleteSlotSuccees);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudStorage | Api ")
	static void DeleteSlot(const FString& SlotId, const FDeleteSlotSuccees& OnSuccess, const FBlueprintErrorHandler& OnError);
};
