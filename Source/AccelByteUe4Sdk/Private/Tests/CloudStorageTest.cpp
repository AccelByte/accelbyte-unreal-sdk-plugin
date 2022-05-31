// Copyright (c) 2018-2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteCloudStorageApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteMultiRegistry.h"
#include "TestUtilities.h"
#include "UserTestAdmin.h"

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteCloudStorageTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteCloudStorageTest);

const int32 AutomationFlagMaskCloudStorage = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto CloudStorageErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteCloudStorageTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

BEGIN_DEFINE_SPEC(FCloudStorageTest, "AccelByte.Tests.CloudStorage", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	FTestUser TestUser;
	FString Payloads[2] = { TEXT("PayloadOne"), TEXT("PayloadTwo") };
	FString OriginalNames[2] = { TEXT("File1.txt"), TEXT("File2.txt") };
	TArray<FString> Tags = { TEXT("Tag1"), TEXT("Tag2") };
	TArray<FString> UpdateTags = { TEXT("Tag3"), TEXT("Tag4") };
	FString Labels[2] = { TEXT("Label1"), TEXT("Label2") };
	FAccelByteModelsSlot CreatedSlot;
END_DEFINE_SPEC(FCloudStorageTest)

void FCloudStorageTest::Define()
{
	const auto setupOnce = [this]()
	{
		AB_TEST_TRUE(SetupTestUser(TestUser));
		
		return true;
	};

	BeforeEach(setupOnce);

	const auto teardown = [this]()
	{		
		AB_TEST_TRUE(TeardownTestUser(TestUser));

		return true;
	};

	AfterEach(teardown);

	Describe("Create_Update_Get_Delete_Slot", [this]()
	{
		It("Should run CRUD test then return true", [this]()
		{
			FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
			
			bool bSlotCreatedResult = false;
			ApiClient->CloudStorage.CreateSlot(UAccelByteBlueprintsTest::FStringToBytes(Payloads[0]), "create.txt", Tags, Labels[0], "customAttributeCreated",
				THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
			{
				UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Create Slot Success"));
				CreatedSlot = Result;
				bSlotCreatedResult = true;
			}), nullptr, CloudStorageErrorHandler);

			WaitUntil(bSlotCreatedResult, "Waiting for slot created...");
			AB_TEST_TRUE(bSlotCreatedResult);
			
			UpdateTags[0] = (TEXT("tagOne"));
			UpdateTags[1] = (TEXT("tagTwo"));
			FString UpdateLabel = "metadataLabel";
			FString UpdateCustomAttribute = TEXT("This is a custom attribute");

			bool bMetadataUpdatedResult = false;
			ApiClient->CloudStorage.UpdateSlotMetadata(CreatedSlot.SlotId, UpdateTags, UpdateLabel, UpdateCustomAttribute, THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
			{
				UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Update Metadata Success"));
				bMetadataUpdatedResult = true;
			}), nullptr, CloudStorageErrorHandler);
			WaitUntil(bMetadataUpdatedResult, "Waiting for meta updated...");
			AB_TEST_TRUE(bMetadataUpdatedResult);

			bool bGetSlotAfterUpdateResult = false;
			ApiClient->CloudStorage.GetAllSlots(THandler<TArray<FAccelByteModelsSlot>>::CreateLambda([&](TArray<FAccelByteModelsSlot> Result)
			{
				for (int i = 0; i < Result.Num(); i++)
				{
					if (Result[i].SlotId == CreatedSlot.SlotId)
					{
						if (Result[i].Label != UpdateLabel)
						{
							break;
						}
						if (!Result[i].Tags.Contains(UpdateTags[0]))
						{
							break;
						}
						if (!Result[i].Tags.Contains(UpdateTags[1]))
						{
							break;
						}
						if (Result[i].CustomAttribute != UpdateCustomAttribute)
						{
							break;
						}
						bGetSlotAfterUpdateResult = true;
					}
				}
			}), CloudStorageErrorHandler);
			WaitUntil(bGetSlotAfterUpdateResult, "Waiting for get slot...");
			AB_TEST_TRUE(bGetSlotAfterUpdateResult);

			bool bSlotUpdatedResult = false;
			ApiClient->CloudStorage.UpdateSlot(CreatedSlot.SlotId, UAccelByteBlueprintsTest::FStringToBytes(Payloads[1]), "update.txt", UpdateTags, Labels[1], "customAttributeUpdated", THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
			{
				UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Update Slot Success"));
				bSlotUpdatedResult = true;
			}), nullptr, CloudStorageErrorHandler);

			WaitUntil(bSlotUpdatedResult, "Waiting for slot updated...");
			AB_TEST_TRUE(bSlotUpdatedResult);

			bool bGetSlotResult = false;
			bool bSlotContentUpdated = false;
			ApiClient->CloudStorage.GetSlot(CreatedSlot.SlotId, THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& Data)
			{
				bGetSlotResult = true;
				bSlotContentUpdated = (UAccelByteBlueprintsTest::BytesToFString(Data) == Payloads[1]);
			}), CloudStorageErrorHandler);
			WaitUntil(bGetSlotResult, "Waiting for get slot...");

			AB_TEST_TRUE(bGetSlotResult);
			

			bool bGetAllSlotsResult = false;
			TArray<FAccelByteModelsSlot> Results;
			ApiClient->CloudStorage.GetAllSlots(THandler<TArray<FAccelByteModelsSlot>>::CreateLambda([&](const TArray<FAccelByteModelsSlot>& Slots)
			{
				UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Get All Slots Success"));
				Results = Slots;
				bGetAllSlotsResult = true;
			}), CloudStorageErrorHandler);
			WaitUntil(bGetAllSlotsResult, "Waiting for get all slot...");
			AB_TEST_TRUE(bGetAllSlotsResult);

			TArray<bool> bDeleteSlotResults;
			for (int i = 0; i < Results.Num(); i++)
			{
				bDeleteSlotResults.Add(false);
				ApiClient->CloudStorage.DeleteSlot(Results[i].SlotId, FVoidHandler::CreateLambda([&]()
				{
					UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Delete Slot %d / %d Success"), i+1, Results.Num());
					bDeleteSlotResults[i] = true;
				}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
				{
					bDeleteSlotResults[i] = false;
				}));
				WaitUntil(bDeleteSlotResults[i], "Waiting for slot deletion...");
			}
			AB_TEST_FALSE(bDeleteSlotResults.Contains(false));

			return true;
		});
	});
}
