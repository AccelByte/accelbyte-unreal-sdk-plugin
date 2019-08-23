// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteGameProfileApi.h"
#include "TestUtilities.h"
#include "HAL/FileManager.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteGameProfileTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteGameProfileTest);

const int32 AutomationFlagMaskGameProfile = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto GameProfileTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

const int GameProfileTestUserCount = 3; //minimum is three
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
	TArray<FString> Tags;
	for (int i = 0; i < TagsLength; i++)
	{
		Tags.Add(FString::Printf(TEXT("%s%d"), *TagPrefix, i));
	}

	FAccelByteModelsGameProfileRequest Request{ Attributes, AvatarUrl, Label, ProfileName, Tags };
	return Request;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameProfileSetup, "AccelByte.Tests.GameProfile.A.Setup", AutomationFlagMaskGameProfile);
bool GameProfileSetup::RunTest(const FString& Parameters)
{
	bool bClientLoginSuccess = false;
	bool UsersCreationSuccess[GameProfileTestUserCount];
	bool UsersLoginSuccess[GameProfileTestUserCount];

	
	int i = 0;
	for (; i < GameProfileTestUserCount; i++)
	{
		UsersCreationSuccess[i] = false;
		UsersLoginSuccess[i] = false;
		GameProfileUsers.Add(MakeShared<Api::User>(GameProfileCreds[i], FRegistry::Settings));

		bClientLoginSuccess = false;
		GameProfileUsers[i]->LoginWithClientCredentials(FSimpleDelegate::CreateLambda([&bClientLoginSuccess]()
		{
			bClientLoginSuccess = true;
			UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("Client Login Success"));
		}), GameProfileTestErrorHandler);
		FlushHttpRequests();
		Waiting(bClientLoginSuccess,"Waiting for Login...");
		check(bClientLoginSuccess);

		FString Email = FString::Printf(TEXT("GameProfileUE4Test+%d@example.com"), i);
		Email.ToLowerInline();
		FString Password = FString::Printf(TEXT("password%d"), i);
		FString DisplayName = FString::Printf(TEXT("GameProfileUE4%d"), i);
		FString Country = "US";
		const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 20));
		const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());


		GameProfileUsers[i]->Register(Email, Password, DisplayName, Country, format, THandler<FUserData>::CreateLambda([&](const FUserData& Response)
		{
			UsersCreationSuccess[i] = true;
			UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("Test GameProfile User %d/%d is successfuly created."), i+1, GameProfileTestUserCount);
		}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
		{
			if (Code == EHttpResponseCodes::Conflict)
			{
				UsersCreationSuccess[i] = true;
				UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("Test GameProfile User %d/%d is already created."), i+1, GameProfileTestUserCount);
			}
			else
			{
				UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("Test GameProfile User %d/%d can't be created"), i, GameProfileTestUserCount);
			}
		}));
		FlushHttpRequests();
		Waiting(UsersCreationSuccess[i],FString::Printf(TEXT("Waiting for game profile %d created..."), i));

		GameProfileUsers[i]->LoginWithUsername(
			Email,
			Password,
			FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("User %d Login with email Success"), i);
			UsersLoginSuccess[i] = true;	
		}), GameProfileTestErrorHandler);
		FlushHttpRequests();
		Waiting(UsersLoginSuccess[i],"Waiting for Login...");

		GameProfiles.Add(MakeShared<Api::GameProfile>(GameProfileCreds[i], FRegistry::Settings));
	}

	i = 0;
	for (; i < GameProfileTestUserCount; i++)
	{
		check(UsersLoginSuccess[i]);
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
		FlushHttpRequests();
		Waiting(bGetAllGameProfileSuccess,"Waiting for get all game profiles...");
		UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t%d game profile is found!"), GetAllGameProfileResult.Num());
		check(bGetAllGameProfileSuccess);

		for (int j = 0; j < GetAllGameProfileResult.Num(); j++)
		{
			UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t\tDELETING (%d/%d) GAME PROFILE IS STARTED"), j+1, GetAllGameProfileResult.Num());
			bool bDeleteGameProfileSuccess = false;
			GameProfiles[i]->DeleteGameProfile(GetAllGameProfileResult[j].profileId, FVoidHandler::CreateLambda([&bDeleteGameProfileSuccess]()
			{
				UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t\t\tsuccess"));
				bDeleteGameProfileSuccess = true;
			}), GameProfileTestErrorHandler);
			FlushHttpRequests();
			Waiting(bDeleteGameProfileSuccess,"Waiting for game profile deletion...");
			check(bDeleteGameProfileSuccess);
		}

		UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\tDELETING USER ACCOUNT IS STARTED"));
		bool bDeleteUsersSuccessful = false;
		DeleteUserById(GameProfileCreds[i].GetUserId(), FSimpleDelegate::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t\tsuccess"));
			bDeleteUsersSuccessful = true;
		}), GameProfileTestErrorHandler);
		FlushHttpRequests();
		Waiting(bDeleteUsersSuccessful,"Waiting for user deletion...");
		check(bDeleteUsersSuccessful);
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
	FlushHttpRequests();
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");

	// ASSERTION
	check(bCreateGameProfileSuccess)
	check(ActualResult.attributes.Num() == Request.attributes.Num());
	TArray<FString> Keys;
	Request.attributes.GetKeys(Keys);
	for (int i = 0; i < Keys.Num(); i++)
	{
		check(ActualResult.attributes[Keys[i]] == Request.attributes[Keys[i]]);
	}
	check(ActualResult.avatarUrl == Request.avatarUrl);
	check(ActualResult.label == Request.label);
	check(ActualResult.profileName == Request.profileName);
	for (int i = 0; i < Request.tags.Num(); i++)
	{
		check(ActualResult.tags[i] == Request.tags[i]);
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
	FlushHttpRequests();
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	check(bCreateGameProfileSuccess);

	// ACT
	FAccelByteModelsGameProfile GetResult;
	bool bGetGameProfileSuccess = false;
	GameProfiles[0]->GetGameProfile(CreateResult.profileId, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bGetGameProfileSuccess, &GetResult](const FAccelByteModelsGameProfile& Result){
		GetResult = Result;
		bGetGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetGameProfileSuccess,"Waiting for get game profile...");

	// ASSERTION
	check(bGetGameProfileSuccess);
	check(GetResult.attributes.Num() == Request.attributes.Num());
	TArray<FString> Keys;
	Request.attributes.GetKeys(Keys);
	for (int i = 0; i < Keys.Num(); i++)
	{
		check(GetResult.attributes[Keys[i]] == Request.attributes[Keys[i]]);
	}
	check(GetResult.avatarUrl == Request.avatarUrl);
	check(GetResult.label == Request.label);
	check(GetResult.profileName == Request.profileName);
	for (int i = 0; i < Request.tags.Num(); i++)
	{
		check(GetResult.tags[i] == Request.tags[i]);
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
	FlushHttpRequests();
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	check(bCreateGameProfileSuccess);

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
	FlushHttpRequests();
	Waiting(bUpdateGameProfileSuccess,"Waiting for game profile updated...");

	// ASSERTION COMPARE UPDATE RESULT WITH CREATED PROFILE
	check(bUpdateGameProfileSuccess);
	TArray<FString> CreateResultAttributeKeys;
	CreateResult.attributes.GetKeys(CreateResultAttributeKeys);
	TArray<FString> UpdateResultAttributeKeys;
	UpdateResult.attributes.GetKeys(UpdateResultAttributeKeys);
	check(CreateResultAttributeKeys.Num() != UpdateResultAttributeKeys.Num());
	check(CreateResult.tags.Num() != UpdateResult.tags.Num());
	check(CreateResult.avatarUrl != UpdateResult.avatarUrl);
	check(CreateResult.profileName != UpdateResult.profileName);
	check(CreateResult.label != UpdateResult.label);
	// ASSERTION COMPARE UPDATE RESULT WITH UPDATE REQUEST
	for (int i = 0; i < UpdateResultAttributeKeys.Num(); i++)
	{
		check(UpdateRequest.attributes[UpdateResultAttributeKeys[i]] == UpdateResult.attributes[UpdateResultAttributeKeys[i]]);
	}
	for (int i = 0; i < UpdateRequest.tags.Num(); i++)
	{
		check(UpdateRequest.tags[i] == UpdateResult.tags[i]);
	}
	check(UpdateRequest.avatarUrl == UpdateResult.avatarUrl);
	check(UpdateRequest.profileName == UpdateResult.profileName);
	check(UpdateRequest.label == UpdateResult.label);
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
	FlushHttpRequests();
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	check(bCreateGameProfileSuccess);

	// ACT
	bool bDeleteGameProfileSuccess = false;
	GameProfiles[0]->DeleteGameProfile(ActualResult.profileId, FVoidHandler::CreateLambda([&bDeleteGameProfileSuccess]() {
		bDeleteGameProfileSuccess = true;
	}), GameProfileTestErrorHandler);
	FlushHttpRequests();
	Waiting(bDeleteGameProfileSuccess,"Waiting for game profile deletion...");

	// ACT VERIFY WITH GET THE DELETED GAME_PROFILE ID
	// EXPECT not found OR found an empty game profile
	bool bGameProfileDoesntExist = false;
	GameProfiles[0]->GetGameProfile(ActualResult.profileId, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bGameProfileDoesntExist](const FAccelByteModelsGameProfile& Result) {
		bGameProfileDoesntExist = Result.profileId.IsEmpty();
	}), FErrorHandler::CreateLambda([&bGameProfileDoesntExist](int32 Code, FString Message) {
		bGameProfileDoesntExist = true;
	}));
	FlushHttpRequests();
	Waiting(bGameProfileDoesntExist,"Waiting for deleted game check...");

	// ASSERTION
	check(bDeleteGameProfileSuccess);
	check(bGameProfileDoesntExist);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(GameProfileGetAll, "AccelByte.Tests.GameProfile.F.GetAll_Success", AutomationFlagMaskGameProfile);
bool GameProfileGetAll::RunTest(const FString& Parameters)
{
	// ARRANGEMENT
	TSharedPtr<Api::GameProfile> CurrentGameProfile = GameProfiles[0];

	int GameProfileCreateAmount = 2;
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
		FlushHttpRequests();
		Waiting(bCreateGameProfileSuccesses[i],"Waiting for game profile created...");
		check(bCreateGameProfileSuccesses[i]);
	}

	// ACT
	bool bGetAllGameProfilesSuccess = false;
	TArray<FAccelByteModelsGameProfile> GetAllResult;
	GameProfiles[0]->GetAllGameProfiles(THandler<TArray<FAccelByteModelsGameProfile>>::CreateLambda([&bGetAllGameProfilesSuccess, &GetAllResult](const TArray<FAccelByteModelsGameProfile>& Result) {
		bGetAllGameProfilesSuccess = true;
		GetAllResult = Result;
	}), GameProfileTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetAllGameProfilesSuccess,"Waiting for get all game profile...");

	// ASSERTION
	check(bGetAllGameProfilesSuccess);
	check(GetAllResult.Num() >= GameProfileCreateAmount);
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
	check(!bGameProfilesFound.Contains(false));
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
	FlushHttpRequests();
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	check(bCreateGameProfileSuccess);

	// ACT
	FAccelByteModelsGameProfileAttribute GetResult;
	bool bGetGameProfileAttributeSuccess = false;
	GameProfiles[0]->GetGameProfileAttribute(CreateResult.profileId, TestKey, THandler<FAccelByteModelsGameProfileAttribute>::CreateLambda([&bGetGameProfileAttributeSuccess, &GetResult](const FAccelByteModelsGameProfileAttribute& Result) {
		GetResult = Result;
		bGetGameProfileAttributeSuccess = true;
	}), GameProfileTestErrorHandler);
	FlushHttpRequests();
	Waiting(bGetGameProfileAttributeSuccess,"Waiting for get game profile attribute...");

	// ASSERTION
	check(bGetGameProfileAttributeSuccess);
	check(GetResult.value == Request.attributes[TestKey]);
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
	FlushHttpRequests();
	Waiting(bCreateGameProfileSuccess,"Waiting for game profile created...");
	check(bCreateGameProfileSuccess);

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
	FlushHttpRequests();
	Waiting(bUpdateGameProfileAttributeSuccess,"Waiting for game profile updated...");

	// ASSERTION
	check(bUpdateGameProfileAttributeSuccess);
	check(UpdateResult.attributes[TestAttributeKey] == UpdateAttributeValue);
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
	FlushHttpRequests();
	Waiting(bCreateGameProfileSuccess1,"Waiting for game profile created...");
	check(bCreateGameProfileSuccess1);

	// ARRANGEMENT USER 2
	FAccelByteModelsGameProfileRequest Request2 = GenerateGameProfileRequest(1, "2key", "2val", 1, "2tag");
	FAccelByteModelsGameProfile CreateResult2;
	bool bCreateGameProfileSuccess2 = false;
	GameProfiles[2]->CreateGameProfile(Request2, THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result) {
		CreateResult2 = Result;
		bCreateGameProfileSuccess2 = true;
	}), GameProfileTestErrorHandler);
	FlushHttpRequests();
	Waiting(bCreateGameProfileSuccess2,"Waiting for game profile created...");
	check(bCreateGameProfileSuccess2);

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
	FlushHttpRequests();
	Waiting(bBatchGetPublicGameProfiles,"Waiting for batch public game profiles...");

	// ASSERTION
	TArray<bool> ArrayRequestedGameProfileFound;
	ArrayRequestedGameProfileFound.Init(false, 2);
	check(bBatchGetPublicGameProfiles);
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
	check(!ArrayRequestedGameProfileFound.Contains(false))
	return true;
}
