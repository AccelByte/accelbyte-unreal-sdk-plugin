// Copyright (c) 2019-2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Api/AccelByteGameProfileApi.h"
#include "UserTestAdmin.h"
#include "TestUtilities.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteGameProfileTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteGameProfileTest);

int32 const AutomationFlagMaskGameProfile = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

auto const GameProfileTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteGameProfileTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

const auto GameProfileTestCreateGameProfileRequest = [](int32 AttributesLength, FString AttributeKeyPrefix, FString AttributeValuePrefix, int32 TagsLength, FString TagPrefix,
	FString AvatarUrl = "http://example.com/", FString Label = "DefaultLabel", FString ProfileName = "DefaultName")
{
	TMap<FString, FString> GPAttributes;
	for (int i = 0; i < AttributesLength; i++)
	{
		GPAttributes.Add(FString::Printf(TEXT("%s%d"), *AttributeKeyPrefix, i), FString::Printf(TEXT("%s%d"), *AttributeValuePrefix, i));
	}
	TArray<FString> GPTags;
	for (int i = 0; i < TagsLength; i++)
	{
		GPTags.Add(FString::Printf(TEXT("%s%d"), *TagPrefix, i));
	}
	FAccelByteModelsGameProfileRequest Request{ GPAttributes, AvatarUrl, Label, ProfileName, GPTags };
	return Request;
};

BEGIN_DEFINE_SPEC(FGameProfileTestSpec, "AccelByte.Tests.GameProfile", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	TArray<FTestUser> GameProfileTestUsers;
	const int GameProfileTestUserNum = 3; // FIXME User 2 and 3 are only used by FGameProfileTestBatchGetPublicGameProfiles
	FAccelByteModelsGameProfileRequest Request = GameProfileTestCreateGameProfileRequest(1, "key", "val", 1, "tag");
	FAccelByteModelsGameProfile CreateProfileResult;
END_DEFINE_SPEC(FGameProfileTestSpec)

void FGameProfileTestSpec::Define()
{
	const auto setupOnce = [this]()
	{
		AB_TEST_TRUE(SetupTestUsers(GameProfileTestUserNum, GameProfileTestUsers));

		FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(GameProfileTestUsers[0].Email);
		// ARRANGEMENT
		bool bCreateGameProfileSuccess = false;
		FAccelByteModelsGameProfile CreateResult;
			
		ApiClient->GameProfile.CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&CreateResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result) {
			CreateResult = Result;
			bCreateGameProfileSuccess = true;
		}), GameProfileTestErrorHandler);
		WaitUntil(bCreateGameProfileSuccess,"Waiting for game profile created...");
		AB_TEST_TRUE(bCreateGameProfileSuccess);

		CreateProfileResult = CreateResult;
		
		return true;
	};

	BeforeEach(setupOnce);

	const auto teardown = [this]()
	{
		for (int i = 0; i < GameProfileTestUserNum; i++)
		{
			UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\tGETTING ALL GAME PROFILE IS STARTED"));
			// for each user, delete each game profile
			TArray<FAccelByteModelsGameProfile> GetAllGameProfileResult;
			bool bGetAllGameProfileSuccess = false;
			FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(GameProfileTestUsers[i].Email);
			ApiClient->GameProfile.GetAllGameProfiles(THandler<TArray<FAccelByteModelsGameProfile>>::CreateLambda([&](const TArray<FAccelByteModelsGameProfile>& Result){
				UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t\tsuccess"));
				GetAllGameProfileResult = Result;
				bGetAllGameProfileSuccess = true;
			}), GameProfileTestErrorHandler);
			WaitUntil(bGetAllGameProfileSuccess,"Waiting for get all game profiles...");
			UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t%d game profile is found!"), GetAllGameProfileResult.Num());
			AB_TEST_TRUE(bGetAllGameProfileSuccess);

			for (int j = 0; j < GetAllGameProfileResult.Num(); j++)
			{
				UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t\tDELETING (%d/%d) GAME PROFILE IS STARTED"), j+1, GetAllGameProfileResult.Num());
				bool bDeleteGameProfileSuccess = false;
				ApiClient->GameProfile.DeleteGameProfile(GetAllGameProfileResult[j].profileId, FVoidHandler::CreateLambda([&bDeleteGameProfileSuccess]()
				{
					UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t\t\tsuccess"));
					bDeleteGameProfileSuccess = true;
				}), GameProfileTestErrorHandler);
				WaitUntil(bDeleteGameProfileSuccess,"Waiting for game profile deletion...");
				AB_TEST_TRUE(bDeleteGameProfileSuccess);
			}
		}
		
		AB_TEST_TRUE(TeardownTestUsers(GameProfileTestUsers));

		return true;
	};

	AfterEach(teardown);

	Describe("GetGameProfile", [this]()
	{
		It("Should get game profile", [this]()
		{
			// ARRANGEMENT
			FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(GameProfileTestUsers[0].Email);
			
			// ACT
			FAccelByteModelsGameProfile GetResult;
			bool bGetGameProfileSuccess = false;
			ApiClient->GameProfile.GetGameProfile(CreateProfileResult.profileId, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bGetGameProfileSuccess, &GetResult](const FAccelByteModelsGameProfile& Result){
				GetResult = Result;
				bGetGameProfileSuccess = true;
			}), GameProfileTestErrorHandler);
			WaitUntil(bGetGameProfileSuccess,"Waiting for get game profile...");

			// ASSERTION
			AB_TEST_TRUE(bGetGameProfileSuccess);
			AB_TEST_EQUAL(GetResult.attributes.Num(), Request.attributes.Num());
			TArray<FString> Keys;
			Request.attributes.GetKeys(Keys);
			for (int i = 0; i < Keys.Num(); i++)
			{
				AB_TEST_EQUAL(GetResult.attributes[Keys[i]], Request.attributes[Keys[i]]);
			}
			AB_TEST_EQUAL(GetResult.avatarUrl, Request.avatarUrl);
			AB_TEST_EQUAL(GetResult.label, Request.label);
			AB_TEST_EQUAL(GetResult.profileName, Request.profileName);
			for (int i = 0; i < Request.tags.Num(); i++)
			{
				AB_TEST_EQUAL(GetResult.tags[i], Request.tags[i]);
			}

			return true;
		});
	});

	Describe("UpdateGameProfile", [this]()
	{
		It("Should update game profile", [this]()
		{
			// ARRANGEMENT CREATE
			FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(GameProfileTestUsers[0].Email);
			
			// ARRANGEMENT UPDATE
			TArray<FString> OldAttributeKeys;
			Request.attributes.GetKeys(OldAttributeKeys);
			FAccelByteModelsGameProfileRequest UpdateRequest = GameProfileTestCreateGameProfileRequest((OldAttributeKeys.Num())+1, "Ukey", "Uval", (Request.tags.Num())+1, "Utag", "http://example.com/new", "newLabel", "newName");

			// ACT
			bool bUpdateGameProfileSuccess = false;
			FAccelByteModelsGameProfile UpdateResult;
			ApiClient->GameProfile.UpdateGameProfile(CreateProfileResult.profileId, UpdateRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bUpdateGameProfileSuccess, &UpdateResult](const FAccelByteModelsGameProfile& Result){
				UpdateResult = Result;
				bUpdateGameProfileSuccess = true;
			}), GameProfileTestErrorHandler);
			WaitUntil(bUpdateGameProfileSuccess,"Waiting for game profile updated...");

			// ASSERTION COMPARE UPDATE RESULT WITH CREATED PROFILE
			AB_TEST_TRUE(bUpdateGameProfileSuccess);
			TArray<FString> CreateResultAttributeKeys;
			CreateProfileResult.attributes.GetKeys(CreateResultAttributeKeys);
			TArray<FString> UpdateResultAttributeKeys;
			UpdateResult.attributes.GetKeys(UpdateResultAttributeKeys);
			AB_TEST_NOT_EQUAL(CreateResultAttributeKeys.Num(), UpdateResultAttributeKeys.Num());
			AB_TEST_NOT_EQUAL(CreateProfileResult.tags.Num(), UpdateResult.tags.Num());
			AB_TEST_NOT_EQUAL(CreateProfileResult.avatarUrl, UpdateResult.avatarUrl);
			AB_TEST_NOT_EQUAL(CreateProfileResult.profileName, UpdateResult.profileName);
			AB_TEST_NOT_EQUAL(CreateProfileResult.label, UpdateResult.label);
			// ASSERTION COMPARE UPDATE RESULT WITH UPDATE REQUEST
			for (int i = 0; i < UpdateResultAttributeKeys.Num(); i++)
			{
				AB_TEST_EQUAL(UpdateResult.attributes[UpdateResultAttributeKeys[i]], UpdateRequest.attributes[UpdateResultAttributeKeys[i]]);
			}
			for (int i = 0; i < UpdateRequest.tags.Num(); i++)
			{
				AB_TEST_EQUAL(UpdateRequest.tags[i], UpdateResult.tags[i]);
			}
			AB_TEST_EQUAL(UpdateResult.avatarUrl, UpdateRequest.avatarUrl);
			AB_TEST_EQUAL(UpdateResult.profileName, UpdateRequest.profileName);
			AB_TEST_EQUAL(UpdateResult.label, UpdateRequest.label);

			return true;
			
		});
	});

	Describe("DeleteGameProfile", [this]()
	{
		It("Should delete game profile, must return not found or empty profile", [this]()
		{
			// ARRANGEMENT
			FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(GameProfileTestUsers[0].Email);
			
			// ACT
			bool bDeleteGameProfileSuccess = false;
			ApiClient->GameProfile.DeleteGameProfile(CreateProfileResult.profileId, FVoidHandler::CreateLambda([&bDeleteGameProfileSuccess]() {
				bDeleteGameProfileSuccess = true;
			}), GameProfileTestErrorHandler);
			WaitUntil(bDeleteGameProfileSuccess,"Waiting for game profile deletion...");

			// ACT VERIFY WITH GET THE DELETED GAME_PROFILE ID
			// EXPECT not found OR found an empty game profile
			bool bGameProfileDoesntExist = false;
			ApiClient->GameProfile.GetGameProfile(CreateProfileResult.profileId, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bGameProfileDoesntExist](const FAccelByteModelsGameProfile& Result) {
				bGameProfileDoesntExist = Result.profileId.IsEmpty();
			}), FErrorHandler::CreateLambda([&bGameProfileDoesntExist](int32 Code, FString Message) {
				bGameProfileDoesntExist = true;
			}));
			WaitUntil(bGameProfileDoesntExist,"Waiting for deleted game check...");

			// ASSERTION
			AB_TEST_TRUE(bDeleteGameProfileSuccess);
			AB_TEST_TRUE(bGameProfileDoesntExist);

			return true;
			
		});
	});

	Describe("GetAllGameProfiles", [this]()
	{
		It("Should create then get many game profile", [this]()
		{
			// ARRANGEMENT
			FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(GameProfileTestUsers[0].Email);
			
			const int GameProfileCreateAmount = 2;
			TArray<FAccelByteModelsGameProfileRequest> Requests;
			Requests.SetNum(GameProfileCreateAmount);
			TArray<FAccelByteModelsGameProfile> CreateResults;
			CreateResults.SetNum(GameProfileCreateAmount);
			TArray<bool> bCreateGameProfileSuccesses;
			bCreateGameProfileSuccesses.Init(false, GameProfileCreateAmount);

			for (int i = 0; i < GameProfileCreateAmount; i++)
			{
				bCreateGameProfileSuccesses[i] = false;
				Requests[i] = GameProfileTestCreateGameProfileRequest(1, FString::Printf(TEXT("%dkey"), i), FString::Printf(TEXT("%dval"), i), 1, FString::Printf(TEXT("%dtag"), i));
				ApiClient->GameProfile.CreateGameProfile(Requests[i], THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result) {
					CreateResults[i] = Result;
					bCreateGameProfileSuccesses[i] = true;
				}), GameProfileTestErrorHandler);
				WaitUntil(bCreateGameProfileSuccesses[i],"Waiting for game profile created...");
				AB_TEST_TRUE(bCreateGameProfileSuccesses[i]);
			}

			// ACT
			bool bGetAllGameProfilesSuccess = false;
			TArray<FAccelByteModelsGameProfile> GetAllResult;
			ApiClient->GameProfile.GetAllGameProfiles(THandler<TArray<FAccelByteModelsGameProfile>>::CreateLambda([&bGetAllGameProfilesSuccess, &GetAllResult](const TArray<FAccelByteModelsGameProfile>& Result) {
				bGetAllGameProfilesSuccess = true;
				GetAllResult = Result;
			}), GameProfileTestErrorHandler);
			WaitUntil(bGetAllGameProfilesSuccess,"Waiting for get all game profile...");

			// ASSERTION
			AB_TEST_TRUE(bGetAllGameProfilesSuccess);
			AB_TEST_TRUE(GetAllResult.Num() >= GameProfileCreateAmount);
			TArray<bool> bGameProfilesFound;
			bGameProfilesFound.Init(false, GameProfileCreateAmount);
			
			for (int i = 0; i < GameProfileCreateAmount; i++)
			{
				bGameProfilesFound[i] = false;
				for (int j = 0; j < GetAllResult.Num(); j++)
				{
					if (CreateResults[i].profileId == GetAllResult[j].profileId)
					{
						bGameProfilesFound[i] = true;
					}
				}
			}
			AB_TEST_FALSE(bGameProfilesFound.Contains(false));

			return true;
			
		});
	});

	Describe("GetGameProfileAttribute", [this]()
	{
		It("Should get game profile attribute", [this]()
		{
			// ARRANGEMENT
			TArray<FString> AttributeKeys;
			Request.attributes.GetKeys(AttributeKeys);
			FString TestKey = AttributeKeys[0];
			FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(GameProfileTestUsers[0].Email);
			
			// ACT
			FAccelByteModelsGameProfileAttribute GetResult;
			bool bGetGameProfileAttributeSuccess = false;
			ApiClient->GameProfile.GetGameProfileAttribute(CreateProfileResult.profileId, TestKey, THandler<FAccelByteModelsGameProfileAttribute>::CreateLambda([&bGetGameProfileAttributeSuccess, &GetResult](const FAccelByteModelsGameProfileAttribute& Result) {
				GetResult = Result;
				bGetGameProfileAttributeSuccess = true;
			}), GameProfileTestErrorHandler);
			WaitUntil(bGetGameProfileAttributeSuccess,"Waiting for get game profile attribute...");

			// ASSERTION
			AB_TEST_TRUE(bGetGameProfileAttributeSuccess);
			AB_TEST_EQUAL(GetResult.value, Request.attributes[TestKey]);

			return true;
			
		});
	});

	Describe("UpdateGameProfileAttribute", [this]()
	{
		It("Should update game profile attributes", [this]()
		{
			// ARRANGEMENT
			FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(GameProfileTestUsers[0].Email);
			
			// ACT
			TArray<FString> AttributeKeys;
			Request.attributes.GetKeys(AttributeKeys);
			FString TestAttributeKey = AttributeKeys[0];
			FString UpdateAttributeValue = "updateVALUE";
			FAccelByteModelsGameProfile UpdateResult;
			FAccelByteModelsGameProfileAttribute AttributeRequest{ TestAttributeKey , UpdateAttributeValue };
			bool bUpdateGameProfileAttributeSuccess = false;
			ApiClient->GameProfile.UpdateGameProfileAttribute(CreateProfileResult.profileId, AttributeRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result)
			{
				bUpdateGameProfileAttributeSuccess = true;
				UpdateResult = Result;
			}), GameProfileTestErrorHandler);
			WaitUntil(bUpdateGameProfileAttributeSuccess,"Waiting for game profile updated...");

			// ASSERTION
			AB_TEST_TRUE(bUpdateGameProfileAttributeSuccess);
			AB_TEST_EQUAL(UpdateResult.attributes[TestAttributeKey], UpdateAttributeValue);

			return true;
			
		});
	});

	Describe("BatchGetPublicGameProfiles", [this]()
	{
		It("Should get data of public game profile", [this]()
		{
			// ARRANGEMENT USER INDEX 1
			FAccelByteModelsGameProfileRequest Request1 = GameProfileTestCreateGameProfileRequest(1, "1key", "1val", 1, "1tag");
			FAccelByteModelsGameProfile CreateResult1;
			bool bCreateGameProfileSuccess1 = false;
			FApiClientPtr ApiClient1 = FMultiRegistry::GetApiClient(GameProfileTestUsers[1].Email);
			ApiClient1->GameProfile.CreateGameProfile(Request1, THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result) {
				CreateResult1 = Result;
				bCreateGameProfileSuccess1 = true;
			}), GameProfileTestErrorHandler);
			WaitUntil(bCreateGameProfileSuccess1,"Waiting for game profile created...");
			AB_TEST_TRUE(bCreateGameProfileSuccess1);

			// ARRANGEMENT USER INDEX 2
			FAccelByteModelsGameProfileRequest Request2 = GameProfileTestCreateGameProfileRequest(1, "2key", "2val", 1, "2tag");
			FAccelByteModelsGameProfile CreateResult2;
			bool bCreateGameProfileSuccess2 = false;
			FApiClientPtr ApiClient2 = FMultiRegistry::GetApiClient(GameProfileTestUsers[2].Email);
			ApiClient2->GameProfile.CreateGameProfile(Request2, THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result) {
				CreateResult2 = Result;
				bCreateGameProfileSuccess2 = true;
			}), GameProfileTestErrorHandler);
			WaitUntil(bCreateGameProfileSuccess2,"Waiting for game profile created...");
			AB_TEST_TRUE(bCreateGameProfileSuccess2);

			// ACT USER INDEX 0 GETTING USER INDEX 1 AND 2 PUBLIC GAME PROFILES
			TArray<FAccelByteModelsPublicGameProfile> GetBatchPublicGameProfilesResult;
			bool bBatchGetPublicGameProfiles = false;
			TArray<FString> ArrayOfUserId;
			ArrayOfUserId.Add(GameProfileTestUsers[1].UserId);
			ArrayOfUserId.Add(GameProfileTestUsers[2].UserId);
			FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(GameProfileTestUsers[0].Email);
			ApiClient->GameProfile.BatchGetPublicGameProfiles(ArrayOfUserId, THandler<TArray<FAccelByteModelsPublicGameProfile>>::CreateLambda([&](const TArray<FAccelByteModelsPublicGameProfile>& Result)
			{
				bBatchGetPublicGameProfiles = true;
				GetBatchPublicGameProfilesResult = Result;
			}), GameProfileTestErrorHandler);
			WaitUntil(bBatchGetPublicGameProfiles,"Waiting for batch public game profiles...");

			// ASSERTION
			TArray<bool> ArrayRequestedGameProfileFound;
			ArrayRequestedGameProfileFound.Init(false, 2);
			AB_TEST_TRUE(bBatchGetPublicGameProfiles);
			for (int i = 0; i < 2; i++)
			{
				ArrayRequestedGameProfileFound[i] = false;
				for (int j = 0; j < GetBatchPublicGameProfilesResult.Num(); j++)
				{
					for (int k = 0; k < GetBatchPublicGameProfilesResult[j].gameProfiles.Num(); k++)
					{
						if (GetBatchPublicGameProfilesResult[j].gameProfiles[k].profileId == CreateResult1.profileId)
						{
							ArrayRequestedGameProfileFound[i] = true;
						}
						else if (GetBatchPublicGameProfilesResult[j].gameProfiles[k].profileId == CreateResult2.profileId)
						{
							ArrayRequestedGameProfileFound[i] = true;
						}
					}
				}
			}
			AB_TEST_FALSE(ArrayRequestedGameProfileFound.Contains(false))

			return true;
			
		});
	});
	
}
