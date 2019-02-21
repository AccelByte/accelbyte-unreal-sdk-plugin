// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "AccelByteUserApi.h"
#include "AccelByteCloudStorageApi.h"
#include "AccelByteRegistry.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"
#include "TestUtilities.h"
#include "FileManager.h"

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
FString Tags[2] = { TEXT("Tag1"), TEXT("Tag2") };
FString Labels[2] = { TEXT("Label1"), TEXT("Label2") };
FAccelByteModelsSlot CreatedSlot;

const auto CloudStorageErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %s\nError message:%s"), ErrorCode, *ErrorMessage);
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudStorageSetup, "AccelByte.Tests.CloudStorage.A.Setup", AutomationFlagMaskCloudStorage);
bool CloudStorageSetup::RunTest(const FString& Parameters)
{
	bool bClientLoginResult = false;
	User::LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginResult]()
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Client Login Succeess"));
		bClientLoginResult = true;
	}), CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

	bool bUserLoginResult = false;
	User::LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginResult]()
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("User Login Success"));
		bUserLoginResult = true;
	}), CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

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
	
	check(bDeleteSuccessful);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudStorageCreateSlot, "AccelByte.Tests.CloudStorage.B.CreateSlot", AutomationFlagMaskCloudStorage);
bool CloudStorageCreateSlot::RunTest(const FString& Parameters)
{
	bool bSlotCreatedResult = false;
	CloudStorage::CreateSlot(UAccelByteBlueprintsTest::FStringToBytes(Payloads[0]), "create.txt", Tags[0], Labels[0], THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Create Slot Success"));
		CreatedSlot = Result;
		bSlotCreatedResult = true;
	}), nullptr, CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	
	check(bSlotCreatedResult);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudStorageUpdateSlot, "AccelByte.Tests.CloudStorage.C.UpdateSlot", AutomationFlagMaskCloudStorage);
bool CloudStorageUpdateSlot::RunTest(const FString& Parameters)
{
	bool bSlotUpdatedResult = false;
	CloudStorage::UpdateSlot(CreatedSlot.SlotId, UAccelByteBlueprintsTest::FStringToBytes(Payloads[1]), "update.txt", Tags[1], Labels[1], THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Update Slot Success"));
		bSlotUpdatedResult = true;
	}), nullptr, CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);
	
	check(bSlotUpdatedResult);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudStorageGetAllSlots, "AccelByte.Tests.CloudStorage.D.GetAllSlots", AutomationFlagMaskCloudStorage);
bool CloudStorageGetAllSlots::RunTest(const FString& Parameter)
{
	bool bGetAllSlotsResult = false;
	TArray<FAccelByteModelsSlot> Results;
	CloudStorage::GetAllSlots(THandler<TArray<FAccelByteModelsSlot>>::CreateLambda([&](const TArray<FAccelByteModelsSlot>& Slots)
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Get All Slots Success"));
		Results = Slots;
		bGetAllSlotsResult = true;
	}), CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

	bool bUpdatedSlotFound = false;
	for (int i = 0; i < Results.Num(); i++)
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("\nResult=%s\nLabels=%s"), *Results[i].Label, *Labels[1]);
		if (Results[i].Label == Labels[1])
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
	CloudStorage::GetSlot(CreatedSlot.SlotId, CloudStorage::FGetSlotSuccess::CreateLambda([&](const TArray<uint8>& Data)
	{
		bGetSlotResult = true;
		bSlotContentUpdated = (UAccelByteBlueprintsTest::BytesToFString(Data) == Payloads[1]);
	}), CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

	check(bGetSlotResult);
	check(bSlotContentUpdated);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CloudStorageDeleteSlot, "AccelByte.Tests.CloudStorage.F.DeleteSlot", AutomationFlagMaskCloudStorage);
bool CloudStorageDeleteSlot::RunTest(const FString& Parameters)
{
	bool bDeleteSlotResult = false;
	CloudStorage::DeleteSlot(CreatedSlot.SlotId, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Delete Slot Success"));
		bDeleteSlotResult = true;
	}), CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

	bool bGetAllSlotsResult = false;
	TArray<FAccelByteModelsSlot> Results;
	CloudStorage::GetAllSlots(THandler<TArray<FAccelByteModelsSlot>>::CreateLambda([&](const TArray<FAccelByteModelsSlot>& Slots)
	{
		UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Get All Slots Success"));
		Results = Slots;
		bGetAllSlotsResult = true;
	}), CloudStorageErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

	bool bSlotNotFound = true;
	for (int i = 0; i < Results.Num(); i++)
	{
		if (Results[i].Label == Labels[1])
		{
			bSlotNotFound = false;
		}
	}

	check(bDeleteSlotResult);
	check(bGetAllSlotsResult);
	check(bSlotNotFound);
	return true;
}
