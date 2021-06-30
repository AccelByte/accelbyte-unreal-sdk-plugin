// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteGameProfileApi.h"
#include "TestUtilities.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteGameProfileTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteGameProfileTest);

static const int32 AutomationFlagMaskGameProfile = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

static const auto GameProfileTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteGameProfileTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

static TArray<TSharedPtr<FTestUser>> TestUsers;
static TArray<TSharedPtr<Credentials>> TestCredentials;
static const FString TestUID = TEXT("01a186ae"); // Arbitrary UID for this test
static const int TestUserNum = 3; // FIXME User 2 and 3 are only used by FGameProfileTestBatchGetPublicGameProfiles

const auto GameProfileTestCreateGameProfileRequest = [](int32 AttributesLength, FString AttributeKeyPrefix, FString AttributeValuePrefix, int32 TagsLength, FString TagPrefix, FString AvatarUrl = "http://example.com/", FString Label = "DefaultLabel", FString ProfileName = "DefaultName")
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameProfileTestSetup, "AccelByte.Tests.GameProfile.A.Setup", AutomationFlagMaskGameProfile);
bool FGameProfileTestSetup::RunTest(const FString& Parameters)
{
	AB_TEST_TRUE(SetupTestUsers(TestUID, TestUserNum, TestUsers, TestCredentials)) 

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameProfileTestTearDown, "AccelByte.Tests.GameProfile.Z.TearDown", AutomationFlagMaskGameProfile);
bool FGameProfileTestTearDown::RunTest(const FString& Parameters)
{
	AB_TEST_TRUE(CheckTestUsers(TestUsers, TestCredentials));

	for (int i = 0; i < TestUserNum; i++)
	{
		UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\tGETTING ALL GAME PROFILE IS STARTED"));
		// for each user, delete each game profile
		TArray<FAccelByteModelsGameProfile> GetAllGameProfileResult;
		bool bGetAllGameProfileSuccess = false;
		Api::GameProfile GameProfileApi(*TestCredentials[i], FRegistry::Settings);
		GameProfileApi.GetAllGameProfiles(THandler<TArray<FAccelByteModelsGameProfile>>::CreateLambda([&](const TArray<FAccelByteModelsGameProfile>& Result){
			UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t\tsuccess"));
			GetAllGameProfileResult = Result;
			bGetAllGameProfileSuccess = true;
		}), GameProfileTestErrorHandler);
		Waiting(bGetAllGameProfileSuccess,"Waiting for get all game profiles...");
		UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t%d game profile is found!"), GetAllGameProfileResult.Num());
		AB_TEST_TRUE(bGetAllGameProfileSuccess);

		for (int j = 0; j < GetAllGameProfileResult.Num(); j++)
		{
			UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t\tDELETING (%d/%d) GAME PROFILE IS STARTED"), j+1, GetAllGameProfileResult.Num());
			bool bDeleteGameProfileSuccess = false;
			GameProfileApi.DeleteGameProfile(GetAllGameProfileResult[j].profileId, FVoidHandler::CreateLambda([&bDeleteGameProfileSuccess]()
			{
				UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t\t\tsuccess"));
				bDeleteGameProfileSuccess = true;
			}), GameProfileTestErrorHandler);
			Waiting(bDeleteGameProfileSuccess,"Waiting for game profile deletion...");
			AB_TEST_TRUE(bDeleteGameProfileSuccess);
		}
	}

	AB_TEST_TRUE(TearDownTestUsers(TestCredentials))

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameProfileTestCreateGameProfile, "AccelByte.Tests.GameProfile.B.CreateGameProfile", AutomationFlagMaskGameProfile);
bool FGameProfileTestCreateGameProfile::RunTest(const FString& Parameters)
{
	AB_TEST_TRUE(CheckTestUsers(TestUsers, TestCredentials));

	// ARRANGEMENT
	FAccelByteModelsGameProfileRequest Request = GameProfileTestCreateGameProfileRequest(5, "key", "val", 3, "tag");

	// ACT
	FAccelByteModelsGameProfile ActualResult;
	bool bCreateGameProfileSuccess = false;
	Api::GameProfile GameProfileApi(*TestCredentials[0], FRegistry::Settings);
	GameProfileApi.CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&ActualResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result){
		ActualResult = Result;
		bCreateGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");

	// ASSERTION
	AB_TEST_TRUE(bCreateGameProfileSuccess)
	AB_TEST_EQUAL(ActualResult.attributes.Num(), Request.attributes.Num());
	TArray<FString> Keys;
	Request.attributes.GetKeys(Keys);
	for (int i = 0; i < Keys.Num(); i++)
	{
		AB_TEST_EQUAL(ActualResult.attributes[Keys[i]], Request.attributes[Keys[i]]);
	}
	AB_TEST_EQUAL(ActualResult.avatarUrl, Request.avatarUrl);
	AB_TEST_EQUAL(ActualResult.label, Request.label);
	AB_TEST_EQUAL(ActualResult.profileName, Request.profileName);
	for (int i = 0; i < Request.tags.Num(); i++)
	{
		AB_TEST_EQUAL(ActualResult.tags[i], Request.tags[i]);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameProfileTestGetGameProfile, "AccelByte.Tests.GameProfile.C.GetGameProfile", AutomationFlagMaskGameProfile);
bool FGameProfileTestGetGameProfile::RunTest(const FString& Parameters)
{
	AB_TEST_TRUE(CheckTestUsers(TestUsers, TestCredentials));

	// ARRANGEMENT
	FAccelByteModelsGameProfileRequest Request = GameProfileTestCreateGameProfileRequest(1, "key", "val", 1, "tag");
	FAccelByteModelsGameProfile CreateResult;
	bool bCreateGameProfileSuccess = false;
	Api::GameProfile GameProfileApi(*TestCredentials[0], FRegistry::Settings);
	GameProfileApi.CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&CreateResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result) {
		CreateResult = Result;
		bCreateGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	AB_TEST_TRUE(bCreateGameProfileSuccess);

	// ACT
	FAccelByteModelsGameProfile GetResult;
	bool bGetGameProfileSuccess = false;
	GameProfileApi.GetGameProfile(CreateResult.profileId, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bGetGameProfileSuccess, &GetResult](const FAccelByteModelsGameProfile& Result){
		GetResult = Result;
		bGetGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bGetGameProfileSuccess,"Waiting for get game profile...");

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
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameProfileTestUpdateGameProfile, "AccelByte.Tests.GameProfile.D.UpdateGameProfile", AutomationFlagMaskGameProfile);
bool FGameProfileTestUpdateGameProfile::RunTest(const FString& Parameters)
{
	AB_TEST_TRUE(CheckTestUsers(TestUsers, TestCredentials));

	// ARRANGEMENT CREATE
	FAccelByteModelsGameProfileRequest CreateRequest = GameProfileTestCreateGameProfileRequest(2, "key", "val", 1, "T", "http://example.com/old", "oldLabel", "oldName");
	FAccelByteModelsGameProfile CreateResult;
	bool bCreateGameProfileSuccess = false;
	Api::GameProfile GameProfileApi(*TestCredentials[0], FRegistry::Settings);
	GameProfileApi.CreateGameProfile(CreateRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([&CreateResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result) {
		CreateResult = Result;
		bCreateGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	AB_TEST_TRUE(bCreateGameProfileSuccess);

	// ARRANGEMENT UPDATE
	TArray<FString> OldAttributeKeys;
	CreateRequest.attributes.GetKeys(OldAttributeKeys);
	FAccelByteModelsGameProfileRequest UpdateRequest = GameProfileTestCreateGameProfileRequest((OldAttributeKeys.Num())+1, "Ukey", "Uval", (CreateRequest.tags.Num())+1, "Utag", "http://example.com/new", "newLabel", "newName");

	// ACT
	bool bUpdateGameProfileSuccess = false;
	FAccelByteModelsGameProfile UpdateResult;
	GameProfileApi.UpdateGameProfile(CreateResult.profileId, UpdateRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bUpdateGameProfileSuccess, &UpdateResult](const FAccelByteModelsGameProfile& Result){
		UpdateResult = Result;
		bUpdateGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bUpdateGameProfileSuccess,"Waiting for game profile updated...");

	// ASSERTION COMPARE UPDATE RESULT WITH CREATED PROFILE
	AB_TEST_TRUE(bUpdateGameProfileSuccess);
	TArray<FString> CreateResultAttributeKeys;
	CreateResult.attributes.GetKeys(CreateResultAttributeKeys);
	TArray<FString> UpdateResultAttributeKeys;
	UpdateResult.attributes.GetKeys(UpdateResultAttributeKeys);
	AB_TEST_NOT_EQUAL(CreateResultAttributeKeys.Num(), UpdateResultAttributeKeys.Num());
	AB_TEST_NOT_EQUAL(CreateResult.tags.Num(), UpdateResult.tags.Num());
	AB_TEST_NOT_EQUAL(CreateResult.avatarUrl, UpdateResult.avatarUrl);
	AB_TEST_NOT_EQUAL(CreateResult.profileName, UpdateResult.profileName);
	AB_TEST_NOT_EQUAL(CreateResult.label, UpdateResult.label);
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
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameProfileTestDeleteGameProfile, "AccelByte.Tests.GameProfile.E.DeleteGameProfile", AutomationFlagMaskGameProfile);
bool FGameProfileTestDeleteGameProfile::RunTest(const FString& Parameters)
{
	AB_TEST_TRUE(CheckTestUsers(TestUsers, TestCredentials));

	// ARRANGEMENT
	FAccelByteModelsGameProfileRequest Request = GameProfileTestCreateGameProfileRequest(1, "key", "val", 1, "tag");
	FAccelByteModelsGameProfile ActualResult;
	bool bCreateGameProfileSuccess = false;
	Api::GameProfile GameProfileApi(*TestCredentials[0], FRegistry::Settings);
	GameProfileApi.CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&ActualResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result) {
		ActualResult = Result;
		bCreateGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	AB_TEST_TRUE(bCreateGameProfileSuccess);

	// ACT
	bool bDeleteGameProfileSuccess = false;
	GameProfileApi.DeleteGameProfile(ActualResult.profileId, FVoidHandler::CreateLambda([&bDeleteGameProfileSuccess]() {
		bDeleteGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bDeleteGameProfileSuccess,"Waiting for game profile deletion...");

	// ACT VERIFY WITH GET THE DELETED GAME_PROFILE ID
	// EXPECT not found OR found an empty game profile
	bool bGameProfileDoesntExist = false;
	GameProfileApi.GetGameProfile(ActualResult.profileId, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bGameProfileDoesntExist](const FAccelByteModelsGameProfile& Result) {
		bGameProfileDoesntExist = Result.profileId.IsEmpty();
	}), FErrorHandler::CreateLambda([&bGameProfileDoesntExist](int32 Code, FString Message) {
		bGameProfileDoesntExist = true;
	}));
	Waiting(bGameProfileDoesntExist,"Waiting for deleted game check...");

	// ASSERTION
	AB_TEST_TRUE(bDeleteGameProfileSuccess);
	AB_TEST_TRUE(bGameProfileDoesntExist);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameProfileTestGetAllGameProfiles, "AccelByte.Tests.GameProfile.F.GetAllGameProfiles", AutomationFlagMaskGameProfile);
bool FGameProfileTestGetAllGameProfiles::RunTest(const FString& Parameters)
{
	AB_TEST_TRUE(CheckTestUsers(TestUsers, TestCredentials));

	// ARRANGEMENT
	Api::GameProfile GameProfileApi(*TestCredentials[0], FRegistry::Settings);

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
		GameProfileApi.CreateGameProfile(Requests[i], THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result) {
			CreateResults[i] = Result;
			bCreateGameProfileSuccesses[i] = true;
		}), GameProfileTestErrorHandler);
		Waiting(bCreateGameProfileSuccesses[i],"Waiting for game profile created...");
		AB_TEST_TRUE(bCreateGameProfileSuccesses[i]);
	}

	// ACT
	bool bGetAllGameProfilesSuccess = false;
	TArray<FAccelByteModelsGameProfile> GetAllResult;
	GameProfileApi.GetAllGameProfiles(THandler<TArray<FAccelByteModelsGameProfile>>::CreateLambda([&bGetAllGameProfilesSuccess, &GetAllResult](const TArray<FAccelByteModelsGameProfile>& Result) {
		bGetAllGameProfilesSuccess = true;
		GetAllResult = Result;
	}), GameProfileTestErrorHandler);
	Waiting(bGetAllGameProfilesSuccess,"Waiting for get all game profile...");

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
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameProfileTestGetGameProfileAttribute, "AccelByte.Tests.GameProfile.G.GetGameProfileAttribute", AutomationFlagMaskGameProfile);
bool FGameProfileTestGetGameProfileAttribute::RunTest(const FString& Parameters)
{
	AB_TEST_TRUE(CheckTestUsers(TestUsers, TestCredentials));

	// ARRANGEMENT
	FAccelByteModelsGameProfileRequest Request = GameProfileTestCreateGameProfileRequest(1, "key", "val", 1, "tag");
	TArray<FString> AttributeKeys;
	Request.attributes.GetKeys(AttributeKeys);
	FString TestKey = AttributeKeys[0];
	FAccelByteModelsGameProfile CreateResult;
	bool bCreateGameProfileSuccess = false;
	Api::GameProfile GameProfileApi(*TestCredentials[0], FRegistry::Settings);
	GameProfileApi.CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&CreateResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result) {
		CreateResult = Result;
		bCreateGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	AB_TEST_TRUE(bCreateGameProfileSuccess);

	// ACT
	FAccelByteModelsGameProfileAttribute GetResult;
	bool bGetGameProfileAttributeSuccess = false;
	GameProfileApi.GetGameProfileAttribute(CreateResult.profileId, TestKey, THandler<FAccelByteModelsGameProfileAttribute>::CreateLambda([&bGetGameProfileAttributeSuccess, &GetResult](const FAccelByteModelsGameProfileAttribute& Result) {
		GetResult = Result;
		bGetGameProfileAttributeSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bGetGameProfileAttributeSuccess,"Waiting for get game profile attribute...");

	// ASSERTION
	AB_TEST_TRUE(bGetGameProfileAttributeSuccess);
	AB_TEST_EQUAL(GetResult.value, Request.attributes[TestKey]);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameProfileTestUpdateGameProfileAttribute, "AccelByte.Tests.GameProfile.H.UpdateGameProfileAttribute", AutomationFlagMaskGameProfile);
bool FGameProfileTestUpdateGameProfileAttribute::RunTest(const FString& Parameters)
{
	AB_TEST_TRUE(CheckTestUsers(TestUsers, TestCredentials));

	// ARRANGEMENT
	FAccelByteModelsGameProfileRequest Request = GameProfileTestCreateGameProfileRequest(1, "key", "val", 1, "tag");
	FAccelByteModelsGameProfile CreateResult;
	bool bCreateGameProfileSuccess = false;
	Api::GameProfile GameProfileApi(*TestCredentials[0], FRegistry::Settings);
	GameProfileApi.CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&CreateResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result) {
		CreateResult = Result;
		bCreateGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	AB_TEST_TRUE(bCreateGameProfileSuccess);

	// ACT
	TArray<FString> AttributeKeys;
	Request.attributes.GetKeys(AttributeKeys);
	FString TestAttributeKey = AttributeKeys[0];
	FString UpdateAttributeValue = "updateVALUE";
	FAccelByteModelsGameProfile UpdateResult;
	FAccelByteModelsGameProfileAttribute AttributeRequest{ TestAttributeKey , UpdateAttributeValue };
	bool bUpdateGameProfileAttributeSuccess = false;
	GameProfileApi.UpdateGameProfileAttribute(CreateResult.profileId, AttributeRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result)
	{
		bUpdateGameProfileAttributeSuccess = true;
		UpdateResult = Result;
	}), GameProfileTestErrorHandler);
	Waiting(bUpdateGameProfileAttributeSuccess,"Waiting for game profile updated...");

	// ASSERTION
	AB_TEST_TRUE(bUpdateGameProfileAttributeSuccess);
	AB_TEST_EQUAL(UpdateResult.attributes[TestAttributeKey], UpdateAttributeValue);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FGameProfileTestBatchGetPublicGameProfiles, "AccelByte.Tests.GameProfile.I.BatchGetPublicGameProfiles", AutomationFlagMaskGameProfile);
bool FGameProfileTestBatchGetPublicGameProfiles::RunTest(const FString& Parameters)
{
	AB_TEST_TRUE(CheckTestUsers(TestUsers, TestCredentials));

	// ARRANGEMENT USER INDEX 1
	FAccelByteModelsGameProfileRequest Request1 = GameProfileTestCreateGameProfileRequest(1, "1key", "1val", 1, "1tag");
	FAccelByteModelsGameProfile CreateResult1;
	bool bCreateGameProfileSuccess1 = false;
	Api::GameProfile GameProfileApi1(*TestCredentials[1], FRegistry::Settings);
	GameProfileApi1.CreateGameProfile(Request1, THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result) {
		CreateResult1 = Result;
		bCreateGameProfileSuccess1 = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess1,"Waiting for game profile created...");
	AB_TEST_TRUE(bCreateGameProfileSuccess1);

	// ARRANGEMENT USER INDEX 2
	FAccelByteModelsGameProfileRequest Request2 = GameProfileTestCreateGameProfileRequest(1, "2key", "2val", 1, "2tag");
	FAccelByteModelsGameProfile CreateResult2;
	bool bCreateGameProfileSuccess2 = false;
	Api::GameProfile GameProfileApi2(*TestCredentials[2], FRegistry::Settings);
	GameProfileApi2.CreateGameProfile(Request2, THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result) {
		CreateResult2 = Result;
		bCreateGameProfileSuccess2 = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess2,"Waiting for game profile created...");
	AB_TEST_TRUE(bCreateGameProfileSuccess2);

	// ACT USER INDEX 0 GETTING USER INDEX 1 AND 2 PUBLIC GAME PROFILES
	TArray<FAccelByteModelsPublicGameProfile> GetBatchPublicGameProfilesResult;
	bool bBatchGetPublicGameProfiles = false;
	TArray<FString> ArrayOfUserId;
	ArrayOfUserId.Add(TestCredentials[1]->GetUserId());
	ArrayOfUserId.Add(TestCredentials[2]->GetUserId());
	Api::GameProfile GameProfileApi0(*TestCredentials[0], FRegistry::Settings);
	GameProfileApi0.BatchGetPublicGameProfiles(ArrayOfUserId, THandler<TArray<FAccelByteModelsPublicGameProfile>>::CreateLambda([&](const TArray<FAccelByteModelsPublicGameProfile>& Result)
	{
		bBatchGetPublicGameProfiles = true;
		GetBatchPublicGameProfilesResult = Result;
	}), GameProfileTestErrorHandler);
	Waiting(bBatchGetPublicGameProfiles,"Waiting for batch public game profiles...");

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
}
