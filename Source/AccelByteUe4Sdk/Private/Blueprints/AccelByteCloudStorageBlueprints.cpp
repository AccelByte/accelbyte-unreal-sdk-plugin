// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteCloudStorageBlueprints.h"
#include "Api/AccelByteCloudStorageApi.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::Api::CloudStorage;
using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::Settings;

void UAccelByteBlueprintsCloudStorage::GetAllSlots(const FGetAllSlotsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::CloudStorage.GetAllSlots(THandler<TArray<FAccelByteModelsSlot>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsSlot>& Slots)
	{
		OnSuccess.ExecuteIfBound(Slots);
	}),
	FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCloudStorage::GetSlot(const FString& SlotId, const FGetSlotSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::CloudStorage.GetSlot(SlotId, THandler<TArray<uint8>>::CreateLambda([OnSuccess](const TArray<uint8>& Data)
	{
		OnSuccess.ExecuteIfBound(Data);
	}),
	FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCloudStorage::CreateSlot(const TArray<uint8>& Data, const FString& FileName, const TArray<FString>& Tags, const FString& Label, const FString& CustomAttribute, const FCreateSlotsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::CloudStorage.CreateSlot(Data, FileName, Tags, Label, CustomAttribute, THandler<FAccelByteModelsSlot>::CreateLambda([OnSuccess](const FAccelByteModelsSlot& CreatedSlot)
	{
		OnSuccess.ExecuteIfBound(CreatedSlot);
	}), nullptr, 
	FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCloudStorage::UpdateSlot(const FString& SlotId, const FString& FileName, const TArray<uint8>& Data, const TArray<FString>& Tags, const FString& Label, const FString& CustomAttribute, const FUpdateSlotSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::CloudStorage.UpdateSlot(SlotId, Data, FileName, Tags, Label, CustomAttribute, THandler<FAccelByteModelsSlot>::CreateLambda([OnSuccess](const FAccelByteModelsSlot& UpdatedSlot)
	{
		OnSuccess.ExecuteIfBound(UpdatedSlot);
	}), nullptr,
	FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCloudStorage::UpdateSlotMetadata(const FString& SlotId, const FString& FileName, const TArray<FString>& Tags, const FString& Label, const FString& CustomAttribute, const FUpdateSlotMetadataSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::CloudStorage.UpdateSlotMetadata(SlotId, Tags, Label, CustomAttribute, THandler<FAccelByteModelsSlot>::CreateLambda([OnSuccess](const FAccelByteModelsSlot& UpdatedSlot)
	{
		OnSuccess.ExecuteIfBound(UpdatedSlot);
	}), nullptr,
	FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCloudStorage::DeleteSlot(const FString& SlotId, const FDeleteSlotSuccees& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::CloudStorage.DeleteSlot(SlotId, FVoidHandler::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
	FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}