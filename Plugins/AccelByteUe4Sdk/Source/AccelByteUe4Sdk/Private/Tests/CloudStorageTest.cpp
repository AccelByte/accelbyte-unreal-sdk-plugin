// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteCloudStorageApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteCredentials.h"
#include "TestUtilities.h"
#include "HAL/FileManager.h"

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::CloudStorage;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteCloudStorageTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteCloudStorageTest);

const int32 AutomationFlagMaskCloudStorage = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
void FlushHttpRequests();

FString Payloads[2] = { TEXT("PayloadOne"), TEXT("PayloadTwo") };
FString OriginalNames[2] = { TEXT("File1.txt"), TEXT("File2.txt") };
TArray<FString> Tags = { TEXT("Tag1"), TEXT("Tag2") };
TArray<FString> UpdateTags = { TEXT("Tag3"), TEXT("Tag4") };
FString Labels[2] = { TEXT("Label1"), TEXT("Label2") };
FAccelByteModelsSlot CreatedSlot;

const auto CloudStorageErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudStorageSetup, "AccelByte.Tests.CloudStorage.A.Setup", AutomationFlagMaskCloudStorage);
bool CloudStorageSetup::RunTest(const FString& Parameters)
{
	bool bClientLoginResult = false;
	FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginResult]()
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Client Login Succeess"));
		bClientLoginResult = true;
	}), CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bClientLoginResult, "Waiting for Login...");

	bool bUserLoginResult = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginResult]()
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("User Login Success"));
		bUserLoginResult = true;
	}), CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bUserLoginResult, "Waiting for Login...");

	check(bClientLoginResult);
	check(bUserLoginResult);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudStorageTearDown, "AccelByte.Tests.CloudStorage.Z.TearDown", AutomationFlagMaskCloudStorage);
bool CloudStorageTearDown::RunTest(const FString& Parameters)
{
	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	
	UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), CloudStorageErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteDone, "Waiting for deletion...");
	
	check(bDeleteSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudStorageCreateSlot, "AccelByte.Tests.CloudStorage.B.CreateSlot", AutomationFlagMaskCloudStorage);
bool CloudStorageCreateSlot::RunTest(const FString& Parameters)
{
	bool bSlotCreatedResult = false;
	FRegistry::CloudStorage.CreateSlot(UAccelByteBlueprintsTest::FStringToBytes(Payloads[0]), "create.txt", Tags, Labels[0], "customAttributeCreated", THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Create Slot Success"));
		CreatedSlot = Result;
		bSlotCreatedResult = true;
	}), nullptr, CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bSlotCreatedResult, "Waiting for slot created...");
	
	check(bSlotCreatedResult);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudStorageUpdateSlot, "AccelByte.Tests.CloudStorage.C.UpdateSlot", AutomationFlagMaskCloudStorage);
bool CloudStorageUpdateSlot::RunTest(const FString& Parameters)
{
	bool bSlotUpdatedResult = false;
	FRegistry::CloudStorage.UpdateSlot(CreatedSlot.SlotId, UAccelByteBlueprintsTest::FStringToBytes(Payloads[1]), "update.txt", UpdateTags, Labels[1], "customAttributeUpdated", THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Update Slot Success"));
		bSlotUpdatedResult = true;
	}), nullptr, CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bSlotUpdatedResult, "Waiting for slot updated...");
	
	check(bSlotUpdatedResult);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudStorageUpdateMetadataSlot, "AccelByte.Tests.CloudStorage.C.UpdateMetadata", AutomationFlagMaskCloudStorage);
bool CloudStorageUpdateMetadataSlot::RunTest(const FString& Parameters)
{
	FString UpdateFileName = "metadataUpdate.txt";
	TArray<FString> UpdateTags;
	UpdateTags.Add("tagOne");
	UpdateTags.Add("tagTwo");
	FString UpdateLabel = "metadataLabel";
	FString UpdateCustomAttribute = TEXT("This is a custom attribute");

	bool bMetadataUpdatedResult = false;
	FRegistry::CloudStorage.UpdateSlotMetadata(CreatedSlot.SlotId, UpdateFileName, UpdateTags, UpdateLabel, UpdateCustomAttribute, THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Update Metadata Success"));
		bMetadataUpdatedResult = true;
	}), nullptr, CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bMetadataUpdatedResult, "Waiting for meta updated...");
	check(bMetadataUpdatedResult);

	bool bGetSlotAfterUpdateResult = false;
	FRegistry::CloudStorage.GetAllSlots(THandler<TArray<FAccelByteModelsSlot>>::CreateLambda([&](TArray<FAccelByteModelsSlot> Result)
	{
		for (int i = 0; i < Result.Num(); i++)
		{
			if (Result[i].SlotId == CreatedSlot.SlotId)
			{
				if (Result[i].Label != UpdateLabel) break;
				if (!Result[i].Tags.Contains(UpdateTags[0])) break;
				if (!Result[i].Tags.Contains(UpdateTags[1])) break;
				if (Result[i].CustomAttribute != UpdateCustomAttribute) break;
				bGetSlotAfterUpdateResult = true;
			}
		}
	}), CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetSlotAfterUpdateResult, "Waiting for get slot...");
	check(bGetSlotAfterUpdateResult);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudStorageGetAllSlots, "AccelByte.Tests.CloudStorage.D.GetAllSlots", AutomationFlagMaskCloudStorage);
bool CloudStorageGetAllSlots::RunTest(const FString& Parameter)
{
	bool bGetAllSlotsResult = false;
	TArray<FAccelByteModelsSlot> Results;
	FRegistry::CloudStorage.GetAllSlots(THandler<TArray<FAccelByteModelsSlot>>::CreateLambda([&](const TArray<FAccelByteModelsSlot>& Slots)
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Get All Slots Success"));
		Results = Slots;
		bGetAllSlotsResult = true;
	}), CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetAllSlotsResult, "Waiting for get all slot...");

	bool bUpdatedSlotFound = false;
	for (int i = 0; i < Results.Num(); i++)
	{
		if (Results[i].SlotId == CreatedSlot.SlotId)
		{
			bUpdatedSlotFound = true;
		}
	}

	check(bGetAllSlotsResult);
	check(Results.Num() > 0);
	check(bUpdatedSlotFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudStorageGetSlot, "AccelByte.Tests.CloudStorage.E.GetSlot", AutomationFlagMaskCloudStorage);
bool CloudStorageGetSlot::RunTest(const FString& Parameters)
{
	bool bGetSlotResult = false;
	bool bSlotContentUpdated = false;
	FRegistry::CloudStorage.GetSlot(CreatedSlot.SlotId, THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& Data)
	{
		bGetSlotResult = true;
		bSlotContentUpdated = (UAccelByteBlueprintsTest::BytesToFString(Data) == Payloads[1]);
	}), CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetSlotResult, "Waiting for get slot...");

	check(bGetSlotResult);
	check(bSlotContentUpdated);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudStorageDeleteSlot, "AccelByte.Tests.CloudStorage.F.DeleteSlot", AutomationFlagMaskCloudStorage);
bool CloudStorageDeleteSlot::RunTest(const FString& Parameters)
{
	bool bGetAllSlotsResult = false;
	TArray<FAccelByteModelsSlot> Results;
	FRegistry::CloudStorage.GetAllSlots(THandler<TArray<FAccelByteModelsSlot>>::CreateLambda([&](const TArray<FAccelByteModelsSlot>& Slots)
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Get All Slots Success"));
		Results = Slots;
		bGetAllSlotsResult = true;
	}), CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	Waiting(bGetAllSlotsResult, "Waiting for get all slot...");
	check(bGetAllSlotsResult);

	TArray<bool> bDeleteSlotResults;
	for (int i = 0; i < Results.Num(); i++)
	{
		FRegistry::CloudStorage.DeleteSlot(Results[i].SlotId, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Delete Slot %d / %d Success"), i+1, Results.Num());
			bDeleteSlotResults.Add(true);
		}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
		{
			bDeleteSlotResults.Add(false);
		}));
		FHttpModule::Get().GetHttpManager().Flush(false);
		Waiting(bDeleteSlotResults[i], "Waiting for slot deletion...");
	}
	check(!bDeleteSlotResults.Contains(false));

	return true;
}
