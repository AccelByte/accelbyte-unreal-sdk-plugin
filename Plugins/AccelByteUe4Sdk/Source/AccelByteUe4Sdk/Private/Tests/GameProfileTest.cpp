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

int32 const AutomationFlagMaskGameProfile = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

auto const GameProfileTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteGameProfileTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

int const GameProfileTestUserCount = 3; //minimum is three
Credentials GameProfileCreds[GameProfileTestUserCount];
TArray<TSharedPtr<Api::User>> GameProfileUsers;
TArray<TSharedPtr<Api::GameProfile>> GameProfiles;

FAccelByteModelsGameProfileRequest GenerateGameProfileRequest(int32 AttributesLength, FString AttributeKeyPrefix, FString AttributeValuePrefix, int32 TagsLength, FString TagPrefix, FString AvatarUrl = "http://example.com/", FString Label = "DefaultLabel", FString ProfileName = "DefaultName")
{
	TMap<FString, FString> Attributes;
	for (int i = 0; i < AttributesLength; i++)
	{
		Attributes.Add(FString::Printf(TEXT("%s%d"), *AttributeKeyPrefix, i), FString::Printf(TEXT("%s%d"), *AttributeValuePrefix, i));
	}
	TArray<FString> Tags_;
	for (int i = 0; i < TagsLength; i++)
	{
		Tags_.Add(FString::Printf(TEXT("%s%d"), *TagPrefix, i));
	}

	FAccelByteModelsGameProfileRequest Request{ Attributes, AvatarUrl, Label, ProfileName, Tags_ };
	return Request;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameProfileSetup, "AccelByte.Tests.GameProfile.A.Setup", AutomationFlagMaskGameProfile);
bool GameProfileSetup::RunTest(const FString& Parameters)
{
	bool SuccessfulUserLogins[GameProfileTestUserCount];

	
	int i = 0;
	for (bool SuccessfulUserCreations[GameProfileTestUserCount]; i < GameProfileTestUserCount; i++)
	{
		SuccessfulUserCreations[i] = false;
		SuccessfulUserLogins[i] = false;
		GameProfileUsers.Add(MakeShared<Api::User>(GameProfileCreds[i], FRegistry::Settings));

		FString Email = FString::Printf(TEXT("GameProfileUE4Test+%d@example.com"), i);
		Email.ToLowerInline();
		FString Password = FString::Printf(TEXT("Password%d"), i);
		FString DisplayName = FString::Printf(TEXT("GameProfileUE4%d"), i);
		FString Country = "US";
		const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 20));
		const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());


		GameProfileUsers[i]->Register(Email, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Response)
		{
			SuccessfulUserCreations[i] = true;
			UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("Test GameProfile User %d/%d is successfuly created."), i+1, GameProfileTestUserCount);
		}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
		{
			if (Code == static_cast<int32>(AccelByte::ErrorCodes::UserEmailAlreadyUsedException))
			{
				SuccessfulUserCreations[i] = true;
				UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("Test GameProfile User %d/%d is already created."), i+1, GameProfileTestUserCount);
			}
			else
			{
				UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("Test GameProfile User %d/%d can't be created"), i, GameProfileTestUserCount);
			}
		}));
		Waiting(SuccessfulUserCreations[i],FString::Printf(TEXT("Waiting for game profile %d created..."), i));

		GameProfileUsers[i]->LoginWithUsername(
			Email,
			Password,
			FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("User %d Login with email Success"), i);
			SuccessfulUserLogins[i] = true;	
		}), GameProfileTestErrorHandler);
		Waiting(SuccessfulUserLogins[i],"Waiting for Login...");

		GameProfiles.Add(MakeShared<Api::GameProfile>(GameProfileCreds[i], FRegistry::Settings));
	}

	i = 0;
	for (; i < GameProfileTestUserCount; i++)
	{
		AB_TEST_TRUE(SuccessfulUserLogins[i]);
		UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("User Login %d checked."), i);
	}
	UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("Setup Done."));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameProfileSetupTearDown, "AccelByte.Tests.GameProfile.Z.TearDown", AutomationFlagMaskGameProfile);
bool GameProfileSetupTearDown::RunTest(const FString& Parameters)
{
	for (int i = 0; i < GameProfileTestUserCount; i++)
	{
		UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("DELETING (%d/%d) user account"), i + 1, GameProfileTestUserCount);
		UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\tGETTING ALL GAME PROFILE IS STARTED"));
		// for each user, delete each game profile
		TArray<FAccelByteModelsGameProfile> GetAllGameProfileResult;
		bool bGetAllGameProfileSuccess = false;
		GameProfiles[i]->GetAllGameProfiles(THandler<TArray<FAccelByteModelsGameProfile>>::CreateLambda([&](const TArray<FAccelByteModelsGameProfile>& Result){
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
			GameProfiles[i]->DeleteGameProfile(GetAllGameProfileResult[j].profileId, FVoidHandler::CreateLambda([&bDeleteGameProfileSuccess]()
			{
				UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t\t\tsuccess"));
				bDeleteGameProfileSuccess = true;
			}), GameProfileTestErrorHandler);
			Waiting(bDeleteGameProfileSuccess,"Waiting for game profile deletion...");
			AB_TEST_TRUE(bDeleteGameProfileSuccess);
		}

		UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\tDELETING USER ACCOUNT IS STARTED"));
		bool bDeleteUsersSuccessful = false;
		DeleteUserById(GameProfileCreds[i].GetUserId(), FSimpleDelegate::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t\tsuccess"));
			bDeleteUsersSuccessful = true;
		}), GameProfileTestErrorHandler);
		Waiting(bDeleteUsersSuccessful,"Waiting for user deletion...");
		AB_TEST_TRUE(bDeleteUsersSuccessful);
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameProfileCreate, "AccelByte.Tests.GameProfile.B.Create_Success", AutomationFlagMaskGameProfile);
bool GameProfileCreate::RunTest(const FString& Parameters)
{
	// ARRANGEMENT
	FAccelByteModelsGameProfileRequest Request = GenerateGameProfileRequest(5, "key", "val", 3, "tag");

	// ACT
	FAccelByteModelsGameProfile ActualResult;
	bool bCreateGameProfileSuccess = false;
	GameProfiles[0]->CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&ActualResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result){
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameProfileGet, "AccelByte.Tests.GameProfile.C.Get_Success", AutomationFlagMaskGameProfile);
bool GameProfileGet::RunTest(const FString& Parameters)
{
	// ARRANGEMENT
	FAccelByteModelsGameProfileRequest Request = GenerateGameProfileRequest(1, "key", "val", 1, "tag");
	FAccelByteModelsGameProfile CreateResult;
	bool bCreateGameProfileSuccess = false;
	GameProfiles[0]->CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&CreateResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result) {
		CreateResult = Result;
		bCreateGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	AB_TEST_TRUE(bCreateGameProfileSuccess);

	// ACT
	FAccelByteModelsGameProfile GetResult;
	bool bGetGameProfileSuccess = false;
	GameProfiles[0]->GetGameProfile(CreateResult.profileId, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bGetGameProfileSuccess, &GetResult](const FAccelByteModelsGameProfile& Result){
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameProfileUpdate, "AccelByte.Tests.GameProfile.D.Update_Success", AutomationFlagMaskGameProfile);
bool GameProfileUpdate::RunTest(const FString& Parameters)
{
	// ARRANGEMENT CREATE
	FAccelByteModelsGameProfileRequest CreateRequest = GenerateGameProfileRequest(2, "key", "val", 1, "T", "http://example.com/old", "oldLabel", "oldName");
	FAccelByteModelsGameProfile CreateResult;
	bool bCreateGameProfileSuccess = false;
	GameProfiles[0]->CreateGameProfile(CreateRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([&CreateResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result) {
		CreateResult = Result;
		bCreateGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	AB_TEST_TRUE(bCreateGameProfileSuccess);

	// ARRANGEMENT UPDATE
	TArray<FString> OldAttributeKeys;
	CreateRequest.attributes.GetKeys(OldAttributeKeys);
	FAccelByteModelsGameProfileRequest UpdateRequest = GenerateGameProfileRequest((OldAttributeKeys.Num())+1, "Ukey", "Uval", (CreateRequest.tags.Num())+1, "Utag", "http://example.com/new", "newLabel", "newName");

	// ACT
	bool bUpdateGameProfileSuccess = false;
	FAccelByteModelsGameProfile UpdateResult;
	GameProfiles[0]->UpdateGameProfile(CreateResult.profileId, UpdateRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bUpdateGameProfileSuccess, &UpdateResult](const FAccelByteModelsGameProfile& Result){
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameProfileDelete, "AccelByte.Tests.GameProfile.E.Delete_Success", AutomationFlagMaskGameProfile);
bool GameProfileDelete::RunTest(const FString& Parameters)
{
	// ARRANGEMENT
	FAccelByteModelsGameProfileRequest Request = GenerateGameProfileRequest(1, "key", "val", 1, "tag");
	FAccelByteModelsGameProfile ActualResult;
	bool bCreateGameProfileSuccess = false;
	GameProfiles[0]->CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&ActualResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result) {
		ActualResult = Result;
		bCreateGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	AB_TEST_TRUE(bCreateGameProfileSuccess);

	// ACT
	bool bDeleteGameProfileSuccess = false;
	GameProfiles[0]->DeleteGameProfile(ActualResult.profileId, FVoidHandler::CreateLambda([&bDeleteGameProfileSuccess]() {
		bDeleteGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bDeleteGameProfileSuccess,"Waiting for game profile deletion...");

	// ACT VERIFY WITH GET THE DELETED GAME_PROFILE ID
	// EXPECT not found OR found an empty game profile
	bool bGameProfileDoesntExist = false;
	GameProfiles[0]->GetGameProfile(ActualResult.profileId, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bGameProfileDoesntExist](const FAccelByteModelsGameProfile& Result) {
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameProfileGetAll, "AccelByte.Tests.GameProfile.F.GetAll_Success", AutomationFlagMaskGameProfile);
bool GameProfileGetAll::RunTest(const FString& Parameters)
{
	// ARRANGEMENT
	TSharedPtr<Api::GameProfile> CurrentGameProfile = GameProfiles[0];

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
		Requests[i] = GenerateGameProfileRequest(1, FString::Printf(TEXT("%dkey"), i), FString::Printf(TEXT("%dval"), i), 1, FString::Printf(TEXT("%dtag"), i));
		CurrentGameProfile->CreateGameProfile(Requests[i], THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result) {
			CreateResults[i] = Result;
			bCreateGameProfileSuccesses[i] = true;
		}), GameProfileTestErrorHandler);
		Waiting(bCreateGameProfileSuccesses[i],"Waiting for game profile created...");
		AB_TEST_TRUE(bCreateGameProfileSuccesses[i]);
	}

	// ACT
	bool bGetAllGameProfilesSuccess = false;
	TArray<FAccelByteModelsGameProfile> GetAllResult;
	GameProfiles[0]->GetAllGameProfiles(THandler<TArray<FAccelByteModelsGameProfile>>::CreateLambda([&bGetAllGameProfilesSuccess, &GetAllResult](const TArray<FAccelByteModelsGameProfile>& Result) {
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameProfileGetAttribute, "AccelByte.Tests.GameProfile.G.GetAttribute_Success", AutomationFlagMaskGameProfile);
bool GameProfileGetAttribute::RunTest(const FString& Parameters)
{
	// ARRANGEMENT
	FAccelByteModelsGameProfileRequest Request = GenerateGameProfileRequest(1, "key", "val", 1, "tag");
	TArray<FString> AttributeKeys;
	Request.attributes.GetKeys(AttributeKeys);
	FString TestKey = AttributeKeys[0];
	FAccelByteModelsGameProfile CreateResult;
	bool bCreateGameProfileSuccess = false;
	GameProfiles[0]->CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&CreateResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result) {
		CreateResult = Result;
		bCreateGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	AB_TEST_TRUE(bCreateGameProfileSuccess);

	// ACT
	FAccelByteModelsGameProfileAttribute GetResult;
	bool bGetGameProfileAttributeSuccess = false;
	GameProfiles[0]->GetGameProfileAttribute(CreateResult.profileId, TestKey, THandler<FAccelByteModelsGameProfileAttribute>::CreateLambda([&bGetGameProfileAttributeSuccess, &GetResult](const FAccelByteModelsGameProfileAttribute& Result) {
		GetResult = Result;
		bGetGameProfileAttributeSuccess = true;
	}), GameProfileTestErrorHandler);
	Waiting(bGetGameProfileAttributeSuccess,"Waiting for get game profile attribute...");

	// ASSERTION
	AB_TEST_TRUE(bGetGameProfileAttributeSuccess);
	AB_TEST_EQUAL(GetResult.value, Request.attributes[TestKey]);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameProfileUpdateAttribute, "AccelByte.Tests.GameProfile.H.UpdateAttribute_Success", AutomationFlagMaskGameProfile);
bool GameProfileUpdateAttribute::RunTest(const FString& Parameters)
{
	// ARRANGEMENT
	FAccelByteModelsGameProfileRequest Request = GenerateGameProfileRequest(1, "key", "val", 1, "tag");
	FAccelByteModelsGameProfile CreateResult;
	bool bCreateGameProfileSuccess = false;
	GameProfiles[0]->CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&CreateResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result) {
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
	GameProfiles[0]->UpdateGameProfileAttribute(CreateResult.profileId, AttributeRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result)
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameProfileGetBatchPublic, "AccelByte.Tests.GameProfile.I.BatchGetPublicGameProfiles_Success", AutomationFlagMaskGameProfile);
bool GameProfileGetBatchPublic::RunTest(const FString& Parameters)
{
	// ARRANGEMENT USER 1
	FAccelByteModelsGameProfileRequest Request1 = GenerateGameProfileRequest(1, "1key", "1val", 1, "1tag");
	FAccelByteModelsGameProfile CreateResult1;
	bool bCreateGameProfileSuccess1 = false;
	GameProfiles[1]->CreateGameProfile(Request1, THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result) {
		CreateResult1 = Result;
		bCreateGameProfileSuccess1 = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess1,"Waiting for game profile created...");
	AB_TEST_TRUE(bCreateGameProfileSuccess1);

	// ARRANGEMENT USER 2
	FAccelByteModelsGameProfileRequest Request2 = GenerateGameProfileRequest(1, "2key", "2val", 1, "2tag");
	FAccelByteModelsGameProfile CreateResult2;
	bool bCreateGameProfileSuccess2 = false;
	GameProfiles[2]->CreateGameProfile(Request2, THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result) {
		CreateResult2 = Result;
		bCreateGameProfileSuccess2 = true;
	}), GameProfileTestErrorHandler);
	Waiting(bCreateGameProfileSuccess2,"Waiting for game profile created...");
	AB_TEST_TRUE(bCreateGameProfileSuccess2);

	// ACT
	TArray<FAccelByteModelsPublicGameProfile> GetBatchPublicGameProfilesResult;
	bool bBatchGetPublicGameProfiles = false;
	TArray<FString> ArrayOfUserId;
	ArrayOfUserId.Add(GameProfileCreds[1].GetUserId());
	ArrayOfUserId.Add(GameProfileCreds[2].GetUserId());
	GameProfiles[0]->BatchGetPublicGameProfiles(ArrayOfUserId, THandler<TArray<FAccelByteModelsPublicGameProfile>>::CreateLambda([&](const TArray<FAccelByteModelsPublicGameProfile>& Result)
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
