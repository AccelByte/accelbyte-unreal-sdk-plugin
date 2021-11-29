// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "TestUtilities.h"
#include "UGCTestAdmin.h"
#include "Api/AccelByteUGCApi.h"
#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteUtilities.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "Misc/AutomationTest.h"
#include "UserTestAdmin.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteUGCTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteUGCTest);

const int32 AutomationFlagMaskUGC = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
void FlushHttpRequests();
void AdminCreateUGCType(const FString& Type, const TArray<FString>& SubType, const THandler<FAccelByteModelsUGCTypeResponse>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateUGCTags(const FString& Tag, const THandler<FAccelByteModelsUGCTagResponse>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteUGCType(const FString& TypeId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteUGCTag(const FString& TagId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

// Setup variables.
FString UGCChannelName = TEXT("Integration Test Channel UE4");
FString UGCChannelId;
TArray<FString> UGCTags = { TEXT("Sedan"), TEXT("Minibus"), TEXT("Sport") };
TArray<FString> UGCTagIds;
FString UGCType = TEXT("Vehicle");
TArray<FString> UGCSubTypes = { TEXT("Body"), TEXT("Wheel"), TEXT("Vynil") };
FString UGCTypeId;
TArray<uint8> UGCPreviewBytes = { 154, 221, 92, 161, 163, 42, 108, 18, 111, 191, 203, 239, 249, 43, 248, 255, 191, 132, 105, 26 };
TArray<uint8> UGCContentBytes = { 106, 35, 188, 171, 106, 138, 197, 77, 94, 182, 18, 99, 9, 245, 110, 45, 197, 22, 35, 171 };
FAccelByteModelsUGCRequest UGCCreateContentRequest = {
	"png", "png", "Integration Test UE4", FBase64::Encode(UGCPreviewBytes), UGCType, UGCSubTypes[0], {UGCTags[0], UGCTags[1]}
};
FAccelByteModelsUGCRequest UGCModifyContentRequest0 = {
	"txt", "txt", "MODIFIED-0 Integration Test UE4", "", UGCType, UGCSubTypes[1], {UGCTags[1], UGCTags[2]}
};
FAccelByteModelsUGCRequest UGCModifyContentRequest1 = {
	"png", "png", "MODIFIED-1 Integration Test UE4", "", UGCType, UGCSubTypes[0], {UGCTags[0], UGCTags[2]}
};

FAccelByteModelsUGCRequest UGCCreateContentRequestEmptyFileExtension = {
	"application/octet-stream", "", "Integration Test UE4 Empty File Extension", FBase64::Encode(UGCPreviewBytes), UGCType, UGCSubTypes[0], {UGCTags[0], UGCTags[1]}
};
FAccelByteModelsUGCRequest UGCModifyContentRequest2 = {
	"application/octet-stream", "", "MODIFIED-2 Integration Test UE4", "", UGCType, UGCSubTypes[0], {UGCTags[0], UGCTags[2]}
};

FAccelByteModelsUGCRequest UGCCreateContentRequestEmptyFileExtensionAndContentType = {
	"", "", "Integration Test UE4 Empty File Extension and ContentType", FBase64::Encode(UGCPreviewBytes), UGCType, UGCSubTypes[0], {UGCTags[0], UGCTags[1]}
};
FAccelByteModelsUGCRequest UGCModifyContentRequest3 = {
	"", "", "MODIFIED-3 Integration Test UE4", "", UGCType, UGCSubTypes[0], {UGCTags[0], UGCTags[2]}
};

FAccelByteModelsUGCRequest UGCCreateContentRequestEmptyContentType = {
	"", "png", "Integration Test UE4 Empty ContentType", FBase64::Encode(UGCPreviewBytes), UGCType, UGCSubTypes[0], {UGCTags[0], UGCTags[1]}
};
FAccelByteModelsUGCRequest UGCModifyContentRequest4 = {
	"", "txt", "MODIFIED-4 Integration Test UE4", "", UGCType, UGCSubTypes[0], {UGCTags[0], UGCTags[2]}
};

FApiClientPtr UGCUser2Registry;

const auto UGCOnError = FErrorHandler::CreateLambda([](int32 Code, const FString& Message)
{
	UE_LOG(LogAccelByteUGCTest, Error, TEXT("Error code: %d\nError message:%s"), Code, *Message);
});

bool UGCCheckContainChannel(const FString& ExpectedChannelId, const TArray<FAccelByteModelsUGCChannelResponse>& Channels)
{
	for (const FAccelByteModelsUGCChannelResponse& Channel : Channels)
	{
		if (Channel.Id.Equals(ExpectedChannelId))
		{
			return true;
		}
	}
	return false;
};

bool UGCCheckContainTag(const FString& ExpectedTagName, const FString& ExpectedTagId, const TArray<FAccelByteModelsUGCTagResponse>& Tags)
{
	for(const FAccelByteModelsUGCTagResponse& Tag : Tags)
	{
		if(Tag.Tag.Equals(ExpectedTagName) && Tag.Id.Equals(ExpectedTagId))
		{
			return true;
		}
	}
	return false;
};

bool UGCCheckContainTag(const FString& ExpectedTagName,const TArray<FString>& Tags)
{
	for (const FString& Tag : Tags)
	{
		if (Tag.Equals(ExpectedTagName))
		{
			return true;
		}
	}
	return false;
};

bool UGCCheckContainSubType(const FString& ExpectedSubTypeName, const TArray<FString>& SubTypes)
{
	for (const FString& SubType : SubTypes)
	{
		if (SubType.Equals(ExpectedSubTypeName))
		{
			return true;
		}
	}
	return false;
};

bool UGCCheckContainType(const FString& ExpectedTypeName, const FString& ExpectedTypeId, const TArray<FAccelByteModelsUGCTypeResponse>& Types)
{
	for(const FAccelByteModelsUGCTypeResponse& Type : Types)
	{
		if(Type.Type.Equals(ExpectedTypeName) && Type.Id.Equals(ExpectedTypeId))
		{
			return (UGCCheckContainSubType(UGCSubTypes[0], Type.Subtype))
				&& (UGCCheckContainSubType(UGCSubTypes[1], Type.Subtype))
				&& (UGCCheckContainSubType(UGCSubTypes[2], Type.Subtype));
		}
	}
	return false;
};

bool UGCCheckContentEqual(const FAccelByteModelsUGCContentResponse& ContentResponse, const FAccelByteModelsUGCResponse& Response)
{
	if(!Response.Id.Equals(ContentResponse.Id)) { UE_LOG(LogAccelByteUGCTest, Log, TEXT("Id1 : %s != Id2 : %s"), *Response.Id, *ContentResponse.Id); return false; }
	if(!Response.Name.Equals(ContentResponse.Name)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("Name1 : %s != Name2 : %s"), *Response.Name, *ContentResponse.Name); return false; }
	if(!Response.Namespace.Equals(ContentResponse.Namespace)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("Namespace1 : %s != Namespace2 : %s"), *Response.Namespace, *ContentResponse.Namespace); return false; }
	if(!Response.Type.Equals(ContentResponse.Type)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("Type1 : %s != Type2 : %s"), *Response.Type, *ContentResponse.Type); return false; }
	if(!Response.ChannelId.Equals(ContentResponse.ChannelId)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("ChannelId1 : %s != ChannelId2 : %s"), *Response.ChannelId, *ContentResponse.ChannelId); return false; }
	if(!Response.CreatorName.Equals(ContentResponse.CreatorName)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("CreatorName1 : %s != CreatorName2 : %s"), *Response.CreatorName, *ContentResponse.CreatorName); return false; }
	if(!Response.FileExtension.Equals(ContentResponse.FileExtension)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("FileExtension1 : %s != FileExtension2 : %s"), *Response.FileExtension, *ContentResponse.FileExtension); return false; }
	if(!Response.ShareCode.Equals(ContentResponse.ShareCode)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("ShareCode1 : %s != ShareCode2 : %s"), *Response.ShareCode, *ContentResponse.ShareCode); return false; }
	if(!Response.SubType.Equals(ContentResponse.SubType)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("SubType1 : %s != SubType2 : %s"), *Response.SubType, *ContentResponse.SubType); return false; };
	if(!Response.UserId.Equals(ContentResponse.UserId)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("UserId1 : %s != UserId2 : %s"), *Response.UserId, *ContentResponse.UserId); return false; }

	if(Response.CreatedTime != ContentResponse.CreatedTime) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("CreatedTime1 : %s != CreatedTime2 : %s"), *Response.CreatedTime.ToString(), *ContentResponse.CreatedTime.ToString());return false; }
	if(Response.IsOfficial != ContentResponse.IsOfficial) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("IsOfficial1 : %s != IsOfficial2 : %s"), Response.IsOfficial ? TEXT("true") : TEXT("false"), ContentResponse.IsOfficial ? TEXT("true") : TEXT("false")) ;return false; }
	
	for(const FString& Tag : Response.Tags)
	{
		bool bIsFound = false;
		for(const FString& Tag2 : ContentResponse.Tags)
		{
			if(Tag.Equals(Tag2))
			{
				bIsFound = true;
				break;
			}
		}
		if(!bIsFound) { UE_LOG(LogTemp, Warning, TEXT("Tag : %s is not found"), *Tag); return false; }
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCSetup, "AccelByte.Tests.UGC.A.Setup", AutomationFlagMaskUGC)
bool UGCSetup::RunTest(const FString& Parameters)
{
	// Clean up previous user info.
	bool bUGCLoginUserSuccess;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUGCLoginUserSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("User login Success"));
		bUGCLoginUserSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bUGCLoginUserSuccess, "Waiting for login...");

	AB_TEST_TRUE(bUGCLoginUserSuccess);

	bool bUGCDeleteUserSuccess = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bUGCDeleteUserSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete user Success"));
		bUGCDeleteUserSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bUGCDeleteUserSuccess, "Waiting for user deletion...");

	AB_TEST_TRUE(bUGCDeleteUserSuccess);

	// Login again.
	bUGCLoginUserSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUGCLoginUserSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("User login Success"));
		bUGCLoginUserSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bUGCLoginUserSuccess, "Waiting for login...");

	AB_TEST_TRUE(bUGCLoginUserSuccess);
	
	// Setup UGC configuration.
	// Clean up tag test.
	FAccelByteModelsUGCTagsPagingResponse GetTagsResponse;
	bool bGetTagsSuccess = false;
	FRegistry::UGC.GetTags(THandler<FAccelByteModelsUGCTagsPagingResponse>::CreateLambda([&GetTagsResponse, &bGetTagsSuccess](const FAccelByteModelsUGCTagsPagingResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get tags Success"));
		GetTagsResponse = Response;
		bGetTagsSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetTagsSuccess, "Waiting for getting tags...");

	for(const FString& UGCTag : UGCTags)
	{
		for(const FAccelByteModelsUGCTagResponse& ResponseTag : GetTagsResponse.Data)
		{
			if(ResponseTag.Tag.Equals(UGCTag))
			{
				bool bDeleteTagSuccess = false;
				AdminDeleteUGCTag(ResponseTag.Id, FVoidHandler::CreateLambda([&bDeleteTagSuccess]()
				{
					UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete tag Success"));
					bDeleteTagSuccess = true;
				}), UGCOnError);
				FlushHttpRequests();
				WaitUntil(bDeleteTagSuccess, "Waiting for deleting tag...");

				AB_TEST_TRUE(bDeleteTagSuccess);
			}
		}
	}

	// Clean up type test.
	bool bGetTypesSuccess = false;
	FAccelByteModelsUGCTypesPagingResponse GetTypesResponse;
	FRegistry::UGC.GetTypes(THandler<FAccelByteModelsUGCTypesPagingResponse>::CreateLambda([&GetTypesResponse, &bGetTypesSuccess](const FAccelByteModelsUGCTypesPagingResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get types Success"));
		GetTypesResponse = Response;
		bGetTypesSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetTypesSuccess, "Waiting for getting types...");

	for (const FAccelByteModelsUGCTypeResponse& ResponseType : GetTypesResponse.Data)
	{
		if (ResponseType.Type.Equals(UGCType))
		{
			bool bDeleteTypeSuccess = false;
			AdminDeleteUGCType(ResponseType.Id, FVoidHandler::CreateLambda([&bDeleteTypeSuccess]()
			{
				UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete type Success"));
				bDeleteTypeSuccess = true;
			}), UGCOnError);
			FlushHttpRequests();
			WaitUntil(bDeleteTypeSuccess, "Waiting for deleting type...");

			AB_TEST_TRUE(bDeleteTypeSuccess);
		}
	}

	// Clean up channel.
	bool bGetChannelsDone = false;
	FString PreviousChannelId;
	FRegistry::UGC.GetChannels(THandler<FAccelByteModelsUGCChannelsPagingResponse>::CreateLambda([&bGetChannelsDone, &PreviousChannelId](const FAccelByteModelsUGCChannelsPagingResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get channels Success"));
		for (const FAccelByteModelsUGCChannelResponse& Channel : Response.Data)
		{
			if (Channel.Name.Equals(UGCChannelName))
			{
				PreviousChannelId = Channel.Id;
				break;
			}
		}
		bGetChannelsDone = true;
	}), FErrorHandler::CreateLambda([&bGetChannelsDone](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
		bGetChannelsDone = true;
	}));
	FlushHttpRequests();
	WaitUntil(bGetChannelsDone, "Waiting for getting channels...");

	AB_TEST_TRUE(bGetChannelsDone);

	if (!PreviousChannelId.IsEmpty())
	{
		bool bDeleteChannelDone;
		FRegistry::UGC.DeleteChannel(PreviousChannelId, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete channel Success"));
			bDeleteChannelDone = true;
		}), FErrorHandler::CreateLambda([&bDeleteChannelDone](int32 Code, const FString& Message)
		{
			UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
			bDeleteChannelDone = true;
		}));
		FlushHttpRequests();
		WaitUntil(bDeleteChannelDone, "Waiting for deleting channel...");

		AB_TEST_TRUE(bDeleteChannelDone);
	}

	// Create type.
	bool bCreateTypeSuccess;
	AdminCreateUGCType(UGCType, UGCSubTypes, THandler<FAccelByteModelsUGCTypeResponse>::CreateLambda([&bCreateTypeSuccess](const FAccelByteModelsUGCTypeResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create type Success"));
		UGCTypeId = Response.Id;
		bCreateTypeSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bCreateTypeSuccess, "Waiting for creating type...");

	AB_TEST_TRUE(bCreateTypeSuccess);
	
	// Create tags.
	for(const FString& UGCTag : UGCTags)
	{
		bool bCreateTagSuccess = false;
		AdminCreateUGCTags(UGCTag, THandler<FAccelByteModelsUGCTagResponse>::CreateLambda([&bCreateTagSuccess](const FAccelByteModelsUGCTagResponse& Response)
		{
			UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create tag Success"));
			UGCTagIds.Add(Response.Id);
			bCreateTagSuccess = true;
		}), UGCOnError);
		FlushHttpRequests();
		WaitUntil(bCreateTagSuccess, "Waiting for creating tag...");

		AB_TEST_TRUE(bCreateTagSuccess);
	}
	
	// Login or registration for User2.
	FString User2EmailAddress = "ugc+test+ue4sdk@game.test";
	FString User2Password = "1Password1";
	FString User2DisplayName = "ugctestue4sdk";
	bool bDoneSearchUser = false;
	bool bIsUser2Exist = false;
	FPagedPublicUsersInfo ReceivedUserData;
	FRegistry::User.SearchUsers(User2DisplayName, EAccelByteSearchType::DISPLAYNAME,
		THandler<FPagedPublicUsersInfo>::CreateLambda([&bDoneSearchUser, &ReceivedUserData](const FPagedPublicUsersInfo& Result)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Search user Success"));
		ReceivedUserData = Result;
		bDoneSearchUser = true;
	}), UGCOnError);
	WaitUntil(bDoneSearchUser, "Waiting for searching user...");
	AB_TEST_TRUE(bDoneSearchUser);

	if (ReceivedUserData.Data.Num() > 0)
	{
		bIsUser2Exist = true;
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("User2 is exist"));
	}

	UGCUser2Registry = FMultiRegistry::GetApiClient(TEXT("User2"));;
	bool bLoginSuccess = false;
	if (!bIsUser2Exist)
	{
		// Register new user2.
		bool bRegisterSuccess = false;
		const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 27));
		const FString Format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
		UGCUser2Registry->User.Register(User2EmailAddress, User2Password, User2DisplayName, TEXT("US"), Format,
			THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccess](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteUGCTest, Log, TEXT("Success Register User"));
			bRegisterSuccess = true;
		}), UGCOnError);

		WaitUntil(bRegisterSuccess, "Waiting for registering user2...");
		AB_TEST_TRUE(bRegisterSuccess);
	}

	UGCUser2Registry->User.LoginWithUsername(User2EmailAddress, User2Password, FVoidHandler::CreateLambda([&bLoginSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Login user2 Success"));
		bLoginSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bLoginSuccess, "Waiting for login user2...");

	AB_TEST_TRUE(bLoginSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCTeardown, "AccelByte.Tests.UGC.Z.Teardown", AutomationFlagMaskUGC)
bool UGCTeardown::RunTest(const FString& Parameters)
{
	bool bDeleteTypeSuccess;
	AdminDeleteUGCType(UGCTypeId, FVoidHandler::CreateLambda([&bDeleteTypeSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete type Success"));
		bDeleteTypeSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteTypeSuccess, "Waiting for deleting type...");

	AB_TEST_TRUE(bDeleteTypeSuccess);
	
	for(const FString& UGCTagId : UGCTagIds)
	{
		bool bDeleteTagSuccess = false;
		AdminDeleteUGCTag(UGCTagId, FVoidHandler::CreateLambda([&bDeleteTagSuccess]()
		{
			UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete tag Success"));
			bDeleteTagSuccess = true;
		}), UGCOnError);
		FlushHttpRequests();
		WaitUntil(bDeleteTagSuccess, "Waiting for deleting tag...");

		AB_TEST_TRUE(bDeleteTagSuccess);
	}

	bool bDeleteChannelDone = false;
	FRegistry::UGC.DeleteChannel(UGCChannelId, FVoidHandler::CreateLambda([&bDeleteChannelDone]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete channel Success"));
		bDeleteChannelDone = true;
	}), FErrorHandler::CreateLambda([&bDeleteChannelDone](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
		bDeleteChannelDone = true;
	}));
	FlushHttpRequests();
	WaitUntil(bDeleteChannelDone, "Waiting for deleting channel...");

	AB_TEST_TRUE(bDeleteChannelDone);
	
	bool bDeleteUserSuccess;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete user Success"));
		bDeleteUserSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteUserSuccess, "Waiting for deleting user...");

	AB_TEST_TRUE(bDeleteUserSuccess);
	
	bool bDeleteUser2Success = false;
	AdminDeleteUser(UGCUser2Registry->CredentialsRef->GetUserId(), FSimpleDelegate::CreateLambda([&bDeleteUser2Success]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete user2 Success"));
		bDeleteUser2Success = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteUserSuccess, "Waiting for deleting User2...");
	
	UGCUser2Registry->User.ForgetAllCredentials();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_Get_Delete_Channel, "AccelByte.Tests.UGC.B.Create_Get_Delete_Channel", AutomationFlagMaskUGC);
bool UGCCreate_Get_Delete_Channel::RunTest(const FString& Parameters)
{
	bool bCreateChannelSuccess = false;
	FString CreatedChannelName;
	FRegistry::UGC.CreateChannel(UGCChannelName, THandler<FAccelByteModelsUGCChannelResponse>::CreateLambda([&bCreateChannelSuccess, &CreatedChannelName](const FAccelByteModelsUGCChannelResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create channel Succes"));
		CreatedChannelName = Response.Name;
		UGCChannelId = Response.Id;
		bCreateChannelSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bCreateChannelSuccess, "Waiting for creating channel...");

	AB_TEST_TRUE(bCreateChannelSuccess);
	AB_TEST_EQUAL(CreatedChannelName, UGCChannelName);

	bool bGetChannelsSuccess = false;
	FAccelByteModelsUGCChannelsPagingResponse GetChannelsResponse;
	FRegistry::UGC.GetChannels(THandler<FAccelByteModelsUGCChannelsPagingResponse>::CreateLambda([&bGetChannelsSuccess, &GetChannelsResponse](const FAccelByteModelsUGCChannelsPagingResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get channels Success"));
		GetChannelsResponse = Response;
		bGetChannelsSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetChannelsSuccess, "Waiting for getting channels...");

	AB_TEST_TRUE(bGetChannelsSuccess);
	AB_TEST_TRUE(UGCCheckContainChannel(UGCChannelId, GetChannelsResponse.Data));

	bool bDeleteChannelSuccess = false;
	FRegistry::UGC.DeleteChannel(UGCChannelId, FVoidHandler::CreateLambda([&bDeleteChannelSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete channel Success"));
		bDeleteChannelSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteChannelSuccess, "Waiting for deleting channel...");

	AB_TEST_TRUE(bDeleteChannelSuccess);

	bool bGetChannelsNotFoundSuccess = false;
	FAccelByteModelsUGCChannelsPagingResponse GetChannelsNotFoundResponse;
	FRegistry::UGC.GetChannels(THandler<FAccelByteModelsUGCChannelsPagingResponse>::CreateLambda([&bGetChannelsNotFoundSuccess, &GetChannelsNotFoundResponse](const FAccelByteModelsUGCChannelsPagingResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get channels Success"));
		GetChannelsNotFoundResponse = Response;
		bGetChannelsNotFoundSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetChannelsSuccess, "Waiting for getting channels...");

	AB_TEST_TRUE(bGetChannelsNotFoundSuccess);
	AB_TEST_FALSE(UGCCheckContainChannel(UGCChannelId, GetChannelsNotFoundResponse.Data));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCGetTags, "AccelByte.Tests.UGC.B.GetTags", AutomationFlagMaskUGC);
bool UGCGetTags::RunTest(const FString& Parameters)
{
	bool bGetTagsSuccess = false;
	FAccelByteModelsUGCTagsPagingResponse GetTagsResponse;
	FRegistry::UGC.GetTags(THandler<FAccelByteModelsUGCTagsPagingResponse>::CreateLambda([&bGetTagsSuccess, &GetTagsResponse](const FAccelByteModelsUGCTagsPagingResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get tags Success"));
		GetTagsResponse = Response;
		bGetTagsSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetTagsSuccess, "Waiting for getting tags...");

	AB_TEST_TRUE(bGetTagsSuccess);
	AB_TEST_TRUE(UGCCheckContainTag(UGCTags[0], UGCTagIds[0], GetTagsResponse.Data));
	AB_TEST_TRUE(UGCCheckContainTag(UGCTags[1], UGCTagIds[1], GetTagsResponse.Data));
	AB_TEST_TRUE(UGCCheckContainTag(UGCTags[2], UGCTagIds[2], GetTagsResponse.Data));
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCGetTypes, "AccelByte.Tests.UGC.B.GetTypes", AutomationFlagMaskUGC);
bool UGCGetTypes::RunTest(const FString& Parameters)
{
	bool bGetTypesSuccess;
	FAccelByteModelsUGCTypesPagingResponse GetTypesResponse;
	FRegistry::UGC.GetTypes(THandler<FAccelByteModelsUGCTypesPagingResponse>::CreateLambda([&bGetTypesSuccess, &GetTypesResponse](const FAccelByteModelsUGCTypesPagingResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get types Success"));
		GetTypesResponse = Response;
		bGetTypesSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetTypesSuccess, "Waiting for getting types...");

	AB_TEST_TRUE(bGetTypesSuccess);
	AB_TEST_TRUE(UGCCheckContainType(UGCType, UGCTypeId, GetTypesResponse.Data));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_Get_Delete_Content_As_Bytes, "AccelByte.Tests.UGC.C.Create_Get_Delete_Content_As_Bytes", AutomationFlagMaskUGC)
bool UGCCreate_Get_Delete_Content_As_Bytes::RunTest(const FString& Parameters)
{
	// Create channel.
	bool bCreateChannelSuccess = false;
	FString CreatedChannelName;
	FRegistry::UGC.CreateChannel(UGCChannelName, THandler<FAccelByteModelsUGCChannelResponse>::CreateLambda([&bCreateChannelSuccess, &CreatedChannelName](const FAccelByteModelsUGCChannelResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create channel Succes"));
		CreatedChannelName = Response.Name;
		UGCChannelId = Response.Id;
		bCreateChannelSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bCreateChannelSuccess, "Waiting for creating channel...");

	AB_TEST_TRUE(bCreateChannelSuccess);
	AB_TEST_EQUAL(CreatedChannelName, UGCChannelName);

	// Create content.
	bool bCreateContentSuccess = false;
	FAccelByteModelsUGCResponse CreateContentResponse;
	FRegistry::UGC.CreateContent(UGCChannelId, UGCCreateContentRequest.Name, UGCCreateContentRequest.Type, UGCCreateContentRequest.SubType, UGCCreateContentRequest.Tags, UGCPreviewBytes,
		UGCCreateContentRequest.FileExtension, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&bCreateContentSuccess, &CreateContentResponse](const FAccelByteModelsUGCResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create content Success"));
		CreateContentResponse = Response;
		bCreateContentSuccess = true;
	}), UGCOnError, UGCCreateContentRequest.ContentType);
	FlushHttpRequests();
	WaitUntil(bCreateContentSuccess, "Waiting for creating content...");

	AB_TEST_TRUE(bCreateContentSuccess);
	AB_TEST_EQUAL(CreateContentResponse.ChannelId, UGCChannelId);
	AB_TEST_EQUAL(CreateContentResponse.ContentType, UGCCreateContentRequest.ContentType);
	AB_TEST_EQUAL(CreateContentResponse.FileExtension, UGCCreateContentRequest.FileExtension);
	AB_TEST_EQUAL(CreateContentResponse.Name, UGCCreateContentRequest.Name);
	AB_TEST_EQUAL(CreateContentResponse.Preview, UGCCreateContentRequest.Preview);
	AB_TEST_EQUAL(CreateContentResponse.Type, UGCCreateContentRequest.Type);
	AB_TEST_TRUE(UGCCheckContainTag(CreateContentResponse.Tags[0], CreateContentResponse.Tags));
	AB_TEST_TRUE(UGCCheckContainTag(CreateContentResponse.Tags[1], CreateContentResponse.Tags));

	// Get content.
	bool bGetContentSuccess = false;
	FAccelByteModelsUGCContentResponse GetContentResponse;
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentSuccess, &GetContentResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content Success"));
		GetContentResponse = Response;
		bGetContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContentSuccess, "Waiting for getting content...");

	// Delete content.
	bool bDeleteContentSuccess = false;
	FRegistry::UGC.DeleteContent(UGCChannelId, CreateContentResponse.Id, FVoidHandler::CreateLambda([&bDeleteContentSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete content Success"));
		bDeleteContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteContentSuccess, "Waiting for deleting Content...");

	AB_TEST_TRUE(bDeleteContentSuccess);

	// Delete Channel.
	bool bDeleteChannelSuccess = false;
	FRegistry::UGC.DeleteChannel(UGCChannelId, FVoidHandler::CreateLambda([&bDeleteChannelSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete channel Success"));
		bDeleteChannelSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteChannelSuccess, "Waiting for deleting channel...");

	AB_TEST_TRUE(bDeleteChannelSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_Get_Modify_Delete_Content_As_String, "AccelByte.Tests.UGC.C.Create_Get_Modify_Delete_Content_As_String", AutomationFlagMaskUGC)
bool UGCCreate_Get_Modify_Delete_Content_As_String::RunTest(const FString& Parameters)
{
	// Create channel.
	bool bCreateChannelSuccess = false;
	FString CreatedChannelName;
	FRegistry::UGC.CreateChannel(UGCChannelName, THandler<FAccelByteModelsUGCChannelResponse>::CreateLambda([&bCreateChannelSuccess, &CreatedChannelName](const FAccelByteModelsUGCChannelResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create channel Succes"));
		CreatedChannelName = Response.Name;
		UGCChannelId = Response.Id;
		bCreateChannelSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bCreateChannelSuccess, "Waiting for creating channel...");

	AB_TEST_TRUE(bCreateChannelSuccess);
	AB_TEST_EQUAL(CreatedChannelName, UGCChannelName);

	// Create content.
	bool bCreateContentSuccess = false;
	FAccelByteModelsUGCResponse CreateContentResponse;
	FRegistry::UGC.CreateContent(UGCChannelId, UGCCreateContentRequest,
		THandler<FAccelByteModelsUGCResponse>::CreateLambda([&bCreateContentSuccess, &CreateContentResponse](const FAccelByteModelsUGCResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create content Success"));
		CreateContentResponse = Response;
		bCreateContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bCreateContentSuccess, "Waiting for creating content...");

	AB_TEST_TRUE(bCreateContentSuccess);
	AB_TEST_EQUAL(CreateContentResponse.ContentType, UGCCreateContentRequest.ContentType);
	AB_TEST_EQUAL(CreateContentResponse.ChannelId, UGCChannelId);
	AB_TEST_EQUAL(CreateContentResponse.FileExtension, UGCCreateContentRequest.FileExtension);
	AB_TEST_EQUAL(CreateContentResponse.Name, UGCCreateContentRequest.Name);
	AB_TEST_EQUAL(CreateContentResponse.Preview, UGCCreateContentRequest.Preview);
	AB_TEST_EQUAL(CreateContentResponse.Type, UGCCreateContentRequest.Type);
	AB_TEST_TRUE(UGCCheckContainTag(CreateContentResponse.Tags[0], CreateContentResponse.Tags));
	AB_TEST_TRUE(UGCCheckContainTag(CreateContentResponse.Tags[1], CreateContentResponse.Tags));

	// Upload content payload based using the create content's payload url.
	bool bUploadSuccess = false;
	FAccelByteNetUtilities::UploadTo(CreateContentResponse.PayloadUrl[0].Url, UGCContentBytes,
		FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Upload progress : %d / %d"), BytesSent, Request->GetContentLength());
	}),
	FVoidHandler::CreateLambda([&bUploadSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Upload content Success"));
		bUploadSuccess = true;
	}), UGCOnError, CreateContentResponse.ContentType);
	FlushHttpRequests();
	WaitUntil(bUploadSuccess, "Waiting for uploading content...");

	AB_TEST_TRUE(bUploadSuccess);

	// Get content by content id.
	bool bGetContentSuccess = false;
	FAccelByteModelsUGCContentResponse GetContentResponse;
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentSuccess, &GetContentResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by content id Success"));
		GetContentResponse = Response;
		bGetContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContentSuccess, "Waiting for getting content by content id...");

	AB_TEST_TRUE(bGetContentSuccess);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContentResponse, CreateContentResponse));

	// Download content payload using get content's payload url.
	bool bDownloadSuccess = false;
	TArray<uint8> DownloadBytes;
	FAccelByteNetUtilities::DownloadFrom(GetContentResponse.PayloadUrl[0].Url, FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived)
	{
		auto Response = Request->GetResponse();
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download progress : %d / %d"), BytesReceived, Response->GetContentLength());
	}), THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& RawData)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download content Success"));
		DownloadBytes = RawData;
		bDownloadSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDownloadSuccess, "Waiting for downloading content...");

	AB_TEST_TRUE(bDownloadSuccess);
	AB_TEST_EQUAL(DownloadBytes, UGCContentBytes);

	// Get content by share code by its content creator.
	bool bGetContenttByShareCodeSuccess = false;
	FAccelByteModelsUGCContentResponse GetContenttByShareCodeResponse;
	FRegistry::UGC.GetContentByShareCode(CreateContentResponse.ShareCode,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContenttByShareCodeSuccess, &GetContenttByShareCodeResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by share code by its content creator Success"));
		GetContenttByShareCodeResponse = Response;
		bGetContenttByShareCodeSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContenttByShareCodeSuccess, "Waiting for getting content by share code by its content creator...");

	AB_TEST_TRUE(bGetContenttByShareCodeSuccess);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContenttByShareCodeResponse, CreateContentResponse));

	// Get content by share code by User2.
	bool bGetContenttByShareCodeUser2Success = false;
	FAccelByteModelsUGCContentResponse GetContentByShareCodeUser2Response;
	UGCUser2Registry->UGC.GetContentByShareCode(CreateContentResponse.ShareCode,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContenttByShareCodeUser2Success, &GetContentByShareCodeUser2Response](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by share code by User2 Success"));
		GetContentByShareCodeUser2Response = Response;
		bGetContenttByShareCodeUser2Success = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContenttByShareCodeUser2Success, "Waiting for getting content by share code by User2...");

	AB_TEST_TRUE(bGetContenttByShareCodeUser2Success);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContentByShareCodeUser2Response, CreateContentResponse));

	// Download content payload using get content by share coded's payload url.
	bDownloadSuccess = false;
	DownloadBytes.Empty();
	FAccelByteNetUtilities::DownloadFrom(GetContentResponse.PayloadUrl[0].Url, FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived)
	{
		auto Response = Request->GetResponse();
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download progress : %d / %d"), BytesReceived, Response->GetContentLength());
	}), THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& RawData)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download content Success"));
		DownloadBytes = RawData;
		bDownloadSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDownloadSuccess, "Waiting for downloading content...");

	AB_TEST_TRUE(bDownloadSuccess);
	AB_TEST_EQUAL(DownloadBytes, UGCContentBytes);

	// Get content preview as string.
	bool bGetPreviewSuccess = false;
	FAccelByteModelsUGCPreview GetPreviewResponse;
	FRegistry::UGC.GetContentPreview(CreateContentResponse.Id, THandler<FAccelByteModelsUGCPreview>::CreateLambda([&bGetPreviewSuccess, &GetPreviewResponse](const FAccelByteModelsUGCPreview& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content preview as string Success"));
		GetPreviewResponse = Response;
		bGetPreviewSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetPreviewSuccess, "Waiting for getting content as string...");

	AB_TEST_TRUE(bGetPreviewSuccess); 
	AB_TEST_EQUAL(GetPreviewResponse.Preview, UGCCreateContentRequest.Preview);

	// Get content preview as TArray<uint8>.
	bool bGetPreviewBytesSuccess = false;
	TArray<uint8> GetPreviewBytesResponse;
	FRegistry::UGC.GetContentPreview(CreateContentResponse.Id, THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& GetResponse)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content preview as TArray<uint8> Success"));
		GetPreviewBytesResponse = GetResponse;
		bGetPreviewBytesSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetPreviewBytesSuccess, "Waiting for getting content as TArray<uint8>...");

	AB_TEST_TRUE(bGetPreviewBytesSuccess);
	AB_TEST_EQUAL(GetPreviewBytesResponse, UGCPreviewBytes);
	
	// Modify content.
	bool bModifyContentSuccess = false;
	FAccelByteModelsUGCResponse ModifyContentResponse;
	FRegistry::UGC.ModifyContent(UGCChannelId, CreateContentResponse.Id, UGCModifyContentRequest0,
		THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](const FAccelByteModelsUGCResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Modify content Success"));
		ModifyContentResponse = Response;
		bModifyContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bModifyContentSuccess, "Waiting for modifying content...");

	AB_TEST_TRUE(bModifyContentSuccess);

	// Get content by content id for modifying confirmation.
	bGetContentSuccess = false;
	GetContentResponse = FAccelByteModelsUGCContentResponse();
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentSuccess, &GetContentResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by content id Success"));
		GetContentResponse = Response;
		bGetContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContentSuccess, "Waiting for getting content by content id...");

	AB_TEST_TRUE(bGetContentSuccess);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContentResponse, ModifyContentResponse));

	bModifyContentSuccess = false;
	ModifyContentResponse = FAccelByteModelsUGCResponse();
	FRegistry::UGC.ModifyContent(UGCChannelId, CreateContentResponse.Id, UGCModifyContentRequest1.Name, UGCModifyContentRequest1.Type, UGCModifyContentRequest1.SubType, UGCModifyContentRequest1.Tags,
		UGCPreviewBytes, UGCModifyContentRequest1.FileExtension, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](FAccelByteModelsUGCResponse CreateResponse)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Modify content Success"));
		ModifyContentResponse = CreateResponse;
		bModifyContentSuccess = true;
	}), UGCOnError, UGCModifyContentRequest1.ContentType);
	FlushHttpRequests();
	WaitUntil(bModifyContentSuccess, "Waiting for modifying content...");

	AB_TEST_TRUE(bModifyContentSuccess);

	// Get content by content id for modifying confirmation.
	bGetContentSuccess = false;
	GetContentResponse = FAccelByteModelsUGCContentResponse();
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentSuccess, &GetContentResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by content id Success"));
		GetContentResponse = Response;
		bGetContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContentSuccess, "Waiting for getting content by content id...");

	AB_TEST_TRUE(bGetContentSuccess);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContentResponse, ModifyContentResponse));

	// Delete content.
	bool bDeleteContentSuccess = false;
	FRegistry::UGC.DeleteContent(UGCChannelId, CreateContentResponse.Id, FVoidHandler::CreateLambda([&bDeleteContentSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete content Success"));
		bDeleteContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteContentSuccess, "Waiting for deleting Content...");

	AB_TEST_TRUE(bDeleteContentSuccess);

	// Check getting content again.
	bool bGetContentNotFoundSuccess = false;
	bool bGetContentNotFoundDone = false;
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentNotFoundDone](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content Success"));
		bGetContentNotFoundDone = true;
	}), FErrorHandler::CreateLambda([&bGetContentNotFoundSuccess, &bGetContentNotFoundDone](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
		bGetContentNotFoundSuccess = true;
		bGetContentNotFoundDone = true;
	}));
	FlushHttpRequests();
	WaitUntil(bGetContentNotFoundDone, "Waiting for getting content...");

	AB_TEST_TRUE(bGetContentNotFoundSuccess);
	AB_TEST_TRUE(bGetContentNotFoundDone);

	// Delete Channel.
	bool bDeleteChannelSuccess = false;
	FRegistry::UGC.DeleteChannel(UGCChannelId, FVoidHandler::CreateLambda([&bDeleteChannelSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete channel Success"));
		bDeleteChannelSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteChannelSuccess, "Waiting for deleting channel...");

	AB_TEST_TRUE(bDeleteChannelSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_Get_Modify_Delete_Content_Empty_FileExtension, "AccelByte.Tests.UGC.C.Create_Get_Modify_Delete_Content_Empty_FileExtension", AutomationFlagMaskUGC)
bool UGCCreate_Get_Modify_Delete_Content_Empty_FileExtension::RunTest(const FString& Parameters)
{
	// Create channel.
	bool bCreateChannelSuccess = false;
	FString CreatedChannelName;
	FRegistry::UGC.CreateChannel(UGCChannelName, THandler<FAccelByteModelsUGCChannelResponse>::CreateLambda([&bCreateChannelSuccess, &CreatedChannelName](const FAccelByteModelsUGCChannelResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create channel Succes"));
		CreatedChannelName = Response.Name;
		UGCChannelId = Response.Id;
		bCreateChannelSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bCreateChannelSuccess, "Waiting for creating channel...");

	AB_TEST_TRUE(bCreateChannelSuccess);
	AB_TEST_EQUAL(CreatedChannelName, UGCChannelName);

	// Create content.
	bool bCreateContentSuccess = false;
	FAccelByteModelsUGCResponse CreateContentResponse;
	FRegistry::UGC.CreateContent(UGCChannelId, UGCCreateContentRequestEmptyFileExtension,
		THandler<FAccelByteModelsUGCResponse>::CreateLambda([&bCreateContentSuccess, &CreateContentResponse](const FAccelByteModelsUGCResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create content Success"));
		CreateContentResponse = Response;
		bCreateContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bCreateContentSuccess, "Waiting for creating content...");

	AB_TEST_TRUE(bCreateContentSuccess);
	AB_TEST_EQUAL(CreateContentResponse.ChannelId, UGCChannelId);
	AB_TEST_EQUAL(CreateContentResponse.FileExtension, UGCCreateContentRequestEmptyFileExtension.FileExtension);
	AB_TEST_EQUAL(CreateContentResponse.ContentType, UGCCreateContentRequestEmptyFileExtension.ContentType);
	AB_TEST_EQUAL(CreateContentResponse.Name, UGCCreateContentRequestEmptyFileExtension.Name);
	AB_TEST_EQUAL(CreateContentResponse.Preview, UGCCreateContentRequestEmptyFileExtension.Preview);
	AB_TEST_EQUAL(CreateContentResponse.Type, UGCCreateContentRequestEmptyFileExtension.Type);
	AB_TEST_TRUE(UGCCheckContainTag(CreateContentResponse.Tags[0], CreateContentResponse.Tags));
	AB_TEST_TRUE(UGCCheckContainTag(CreateContentResponse.Tags[1], CreateContentResponse.Tags));

	// Upload content payload based using the create content's payload url.
	bool bUploadSuccess = false;
	FAccelByteNetUtilities::UploadTo(CreateContentResponse.PayloadUrl[0].Url, UGCContentBytes,
		FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Upload progress : %d / %d"), BytesSent, Request->GetContentLength());
	}),
	FVoidHandler::CreateLambda([&bUploadSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Upload content Success"));
		bUploadSuccess = true;
	}), UGCOnError, CreateContentResponse.ContentType);
	FlushHttpRequests();
	WaitUntil(bUploadSuccess, "Waiting for uploading content...");

	AB_TEST_TRUE(bUploadSuccess);

	// Get content by content id.
	bool bGetContentSuccess = false;
	FAccelByteModelsUGCContentResponse GetContentResponse;
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentSuccess, &GetContentResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by content id Success"));
		GetContentResponse = Response;
		bGetContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContentSuccess, "Waiting for getting content by content id...");

	AB_TEST_TRUE(bGetContentSuccess);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContentResponse, CreateContentResponse));

	// Download content payload using get content's payload url.
	bool bDownloadSuccess = false;
	TArray<uint8> DownloadBytes;
	FAccelByteNetUtilities::DownloadFrom(GetContentResponse.PayloadUrl[0].Url, FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived)
	{
		auto Response = Request->GetResponse();
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download progress : %d / %d"), BytesReceived, Response->GetContentLength());
	}), THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& RawData)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download content Success"));
		DownloadBytes = RawData;
		bDownloadSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDownloadSuccess, "Waiting for downloading content...");

	AB_TEST_TRUE(bDownloadSuccess);
	AB_TEST_EQUAL(DownloadBytes, UGCContentBytes);

	// Get content by share code by its content creator.
	bool bGetContenttByShareCodeSuccess = false;
	FAccelByteModelsUGCContentResponse GetContenttByShareCodeResponse;
	FRegistry::UGC.GetContentByShareCode(CreateContentResponse.ShareCode,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContenttByShareCodeSuccess, &GetContenttByShareCodeResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by share code by its content creator Success"));
		GetContenttByShareCodeResponse = Response;
		bGetContenttByShareCodeSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContenttByShareCodeSuccess, "Waiting for getting content by share code by its content creator...");

	AB_TEST_TRUE(bGetContenttByShareCodeSuccess);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContenttByShareCodeResponse, CreateContentResponse));

	// Get content by share code by User2.
	bool bGetContenttByShareCodeUser2Success = false;
	FAccelByteModelsUGCContentResponse GetContentByShareCodeUser2Response;
	UGCUser2Registry->UGC.GetContentByShareCode(CreateContentResponse.ShareCode,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContenttByShareCodeUser2Success, &GetContentByShareCodeUser2Response](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by share code by User2 Success"));
		GetContentByShareCodeUser2Response = Response;
		bGetContenttByShareCodeUser2Success = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContenttByShareCodeUser2Success, "Waiting for getting content by share code by User2...");

	AB_TEST_TRUE(bGetContenttByShareCodeUser2Success);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContentByShareCodeUser2Response, CreateContentResponse));

	// Download content payload using get content by share coded's payload url.
	bDownloadSuccess = false;
	DownloadBytes.Empty();
	FAccelByteNetUtilities::DownloadFrom(GetContentResponse.PayloadUrl[0].Url, FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived)
	{
		auto Response = Request->GetResponse();
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download progress : %d / %d"), BytesReceived, Response->GetContentLength());
	}), THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& RawData)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download content Success"));
		DownloadBytes = RawData;
		bDownloadSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDownloadSuccess, "Waiting for downloading content...");

	AB_TEST_TRUE(bDownloadSuccess);
	AB_TEST_EQUAL(DownloadBytes, UGCContentBytes);

	// Get content preview as string.
	bool bGetPreviewSuccess = false;
	FAccelByteModelsUGCPreview GetPreviewResponse;
	FRegistry::UGC.GetContentPreview(CreateContentResponse.Id, THandler<FAccelByteModelsUGCPreview>::CreateLambda([&bGetPreviewSuccess, &GetPreviewResponse](const FAccelByteModelsUGCPreview& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content preview as string Success"));
		GetPreviewResponse = Response;
		bGetPreviewSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetPreviewSuccess, "Waiting for getting content as string...");

	AB_TEST_TRUE(bGetPreviewSuccess); 
	AB_TEST_EQUAL(GetPreviewResponse.Preview, UGCCreateContentRequest.Preview);

	// Get content preview as TArray<uint8>.
	bool bGetPreviewBytesSuccess = false;
	TArray<uint8> GetPreviewBytesResponse;
	FRegistry::UGC.GetContentPreview(CreateContentResponse.Id, THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& GetResponse)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content preview as TArray<uint8> Success"));
		GetPreviewBytesResponse = GetResponse;
		bGetPreviewBytesSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetPreviewBytesSuccess, "Waiting for getting content as TArray<uint8>...");

	AB_TEST_TRUE(bGetPreviewBytesSuccess);
	AB_TEST_EQUAL(GetPreviewBytesResponse, UGCPreviewBytes);

	// Modify content.
	bool bModifyContentSuccess = false;
	FAccelByteModelsUGCResponse ModifyContentResponse;
	FRegistry::UGC.ModifyContent(UGCChannelId, CreateContentResponse.Id, UGCModifyContentRequest2,
		THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](const FAccelByteModelsUGCResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Modify content Success"));
		ModifyContentResponse = Response;
		bModifyContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bModifyContentSuccess, "Waiting for modifying content...");

	AB_TEST_TRUE(bModifyContentSuccess);

	// Get content by content id for modifying confirmation.
	bGetContentSuccess = false;
	GetContentResponse = FAccelByteModelsUGCContentResponse();
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentSuccess, &GetContentResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by content id Success"));
		GetContentResponse = Response;
		bGetContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContentSuccess, "Waiting for getting content by content id...");

	AB_TEST_TRUE(bGetContentSuccess);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContentResponse, ModifyContentResponse));

	// Delete content.
	bool bDeleteContentSuccess = false;
	FRegistry::UGC.DeleteContent(UGCChannelId, CreateContentResponse.Id, FVoidHandler::CreateLambda([&bDeleteContentSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete content Success"));
		bDeleteContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteContentSuccess, "Waiting for deleting Content...");

	AB_TEST_TRUE(bDeleteContentSuccess);

	// Check getting content again.
	bool bGetContentNotFoundSuccess = false;
	bool bGetContentNotFoundDone = false;
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentNotFoundDone](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content Success"));
		bGetContentNotFoundDone = true;
	}), FErrorHandler::CreateLambda([&bGetContentNotFoundSuccess, &bGetContentNotFoundDone](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
		bGetContentNotFoundSuccess = true;
		bGetContentNotFoundDone = true;
	}));
	FlushHttpRequests();
	WaitUntil(bGetContentNotFoundDone, "Waiting for getting content...");

	AB_TEST_TRUE(bGetContentNotFoundSuccess);
	AB_TEST_TRUE(bGetContentNotFoundDone);

	// Delete Channel.
	bool bDeleteChannelSuccess = false;
	FRegistry::UGC.DeleteChannel(UGCChannelId, FVoidHandler::CreateLambda([&bDeleteChannelSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete channel Success"));
		bDeleteChannelSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteChannelSuccess, "Waiting for deleting channel...");

	AB_TEST_TRUE(bDeleteChannelSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_Get_Modify_Delete_Content_Empty_ContentType, "AccelByte.Tests.UGC.C.Create_Get_Modify_Delete_Content_Empty_ContentType", AutomationFlagMaskUGC)
bool UGCCreate_Get_Modify_Delete_Content_Empty_ContentType::RunTest(const FString& Parameters)
{
	// Create channel.
	bool bCreateChannelSuccess = false;
	FString CreatedChannelName;
	FRegistry::UGC.CreateChannel(UGCChannelName, THandler<FAccelByteModelsUGCChannelResponse>::CreateLambda([&bCreateChannelSuccess, &CreatedChannelName](const FAccelByteModelsUGCChannelResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create channel Succes"));
		CreatedChannelName = Response.Name;
		UGCChannelId = Response.Id;
		bCreateChannelSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bCreateChannelSuccess, "Waiting for creating channel...");

	AB_TEST_TRUE(bCreateChannelSuccess);
	AB_TEST_EQUAL(CreatedChannelName, UGCChannelName);

	// Create content.
	bool bCreateContentSuccess = false;
	FAccelByteModelsUGCResponse CreateContentResponse;
	FRegistry::UGC.CreateContent(UGCChannelId, UGCCreateContentRequestEmptyContentType,
		THandler<FAccelByteModelsUGCResponse>::CreateLambda([&bCreateContentSuccess, &CreateContentResponse](const FAccelByteModelsUGCResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create content Success"));
		CreateContentResponse = Response;
		bCreateContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bCreateContentSuccess, "Waiting for creating content...");

	AB_TEST_TRUE(bCreateContentSuccess);
	AB_TEST_EQUAL(CreateContentResponse.ChannelId, UGCChannelId);
	AB_TEST_EQUAL(CreateContentResponse.FileExtension, UGCCreateContentRequestEmptyContentType.FileExtension);
	AB_TEST_EQUAL(CreateContentResponse.ContentType, "application/octet-stream");
	AB_TEST_EQUAL(CreateContentResponse.Name, UGCCreateContentRequestEmptyContentType.Name);
	AB_TEST_EQUAL(CreateContentResponse.Preview, UGCCreateContentRequestEmptyContentType.Preview);
	AB_TEST_EQUAL(CreateContentResponse.Type, UGCCreateContentRequestEmptyContentType.Type);
	AB_TEST_TRUE(UGCCheckContainTag(CreateContentResponse.Tags[0], CreateContentResponse.Tags));
	AB_TEST_TRUE(UGCCheckContainTag(CreateContentResponse.Tags[1], CreateContentResponse.Tags));

	// Upload content payload based using the create content's payload url.
	bool bUploadSuccess = false;
	FAccelByteNetUtilities::UploadTo(CreateContentResponse.PayloadUrl[0].Url, UGCContentBytes,
		FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Upload progress : %d / %d"), BytesSent, Request->GetContentLength());
	}),
	FVoidHandler::CreateLambda([&bUploadSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Upload content Success"));
		bUploadSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bUploadSuccess, "Waiting for uploading content...");

	AB_TEST_TRUE(bUploadSuccess);

	// Get content by content id.
	bool bGetContentSuccess = false;
	FAccelByteModelsUGCContentResponse GetContentResponse;
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentSuccess, &GetContentResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by content id Success"));
		GetContentResponse = Response;
		bGetContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContentSuccess, "Waiting for getting content by content id...");

	AB_TEST_TRUE(bGetContentSuccess);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContentResponse, CreateContentResponse));

	// Download content payload using get content's payload url.
	bool bDownloadSuccess = false;
	TArray<uint8> DownloadBytes;
	FAccelByteNetUtilities::DownloadFrom(GetContentResponse.PayloadUrl[0].Url, FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived)
	{
		auto Response = Request->GetResponse();
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download progress : %d / %d"), BytesReceived, Response->GetContentLength());
	}), THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& RawData)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download content Success"));
		DownloadBytes = RawData;
		bDownloadSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDownloadSuccess, "Waiting for downloading content...");

	AB_TEST_TRUE(bDownloadSuccess);
	AB_TEST_EQUAL(DownloadBytes, UGCContentBytes);

	// Get content by share code by its content creator.
	bool bGetContenttByShareCodeSuccess = false;
	FAccelByteModelsUGCContentResponse GetContenttByShareCodeResponse;
	FRegistry::UGC.GetContentByShareCode(CreateContentResponse.ShareCode,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContenttByShareCodeSuccess, &GetContenttByShareCodeResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by share code by its content creator Success"));
		GetContenttByShareCodeResponse = Response;
		bGetContenttByShareCodeSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContenttByShareCodeSuccess, "Waiting for getting content by share code by its content creator...");

	AB_TEST_TRUE(bGetContenttByShareCodeSuccess);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContenttByShareCodeResponse, CreateContentResponse));

	// Get content by share code by User2.
	bool bGetContenttByShareCodeUser2Success = false;
	FAccelByteModelsUGCContentResponse GetContentByShareCodeUser2Response;
	UGCUser2Registry->UGC.GetContentByShareCode(CreateContentResponse.ShareCode,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContenttByShareCodeUser2Success, &GetContentByShareCodeUser2Response](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by share code by User2 Success"));
		GetContentByShareCodeUser2Response = Response;
		bGetContenttByShareCodeUser2Success = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContenttByShareCodeUser2Success, "Waiting for getting content by share code by User2...");

	AB_TEST_TRUE(bGetContenttByShareCodeUser2Success);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContentByShareCodeUser2Response, CreateContentResponse));

	// Download content payload using get content by share coded's payload url.
	bDownloadSuccess = false;
	DownloadBytes.Empty();
	FAccelByteNetUtilities::DownloadFrom(GetContentResponse.PayloadUrl[0].Url, FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived)
	{
		auto Response = Request->GetResponse();
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download progress : %d / %d"), BytesReceived, Response->GetContentLength());
	}), THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& RawData)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download content Success"));
		DownloadBytes = RawData;
		bDownloadSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDownloadSuccess, "Waiting for downloading content...");

	AB_TEST_TRUE(bDownloadSuccess);
	AB_TEST_EQUAL(DownloadBytes, UGCContentBytes);

	// Get content preview as string.
	bool bGetPreviewSuccess = false;
	FAccelByteModelsUGCPreview GetPreviewResponse;
	FRegistry::UGC.GetContentPreview(CreateContentResponse.Id, THandler<FAccelByteModelsUGCPreview>::CreateLambda([&bGetPreviewSuccess, &GetPreviewResponse](const FAccelByteModelsUGCPreview& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content preview as string Success"));
		GetPreviewResponse = Response;
		bGetPreviewSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetPreviewSuccess, "Waiting for getting content as string...");

	AB_TEST_TRUE(bGetPreviewSuccess); 
	AB_TEST_EQUAL(GetPreviewResponse.Preview, UGCCreateContentRequest.Preview);

	// Get content preview as TArray<uint8>.
	bool bGetPreviewBytesSuccess = false;
	TArray<uint8> GetPreviewBytesResponse;
	FRegistry::UGC.GetContentPreview(CreateContentResponse.Id, THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& GetResponse)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content preview as TArray<uint8> Success"));
		GetPreviewBytesResponse = GetResponse;
		bGetPreviewBytesSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetPreviewBytesSuccess, "Waiting for getting content as TArray<uint8>...");

	AB_TEST_TRUE(bGetPreviewBytesSuccess);
	AB_TEST_EQUAL(GetPreviewBytesResponse, UGCPreviewBytes);

	// Modify content.
	bool bModifyContentSuccess = false;
	FAccelByteModelsUGCResponse ModifyContentResponse;
	FRegistry::UGC.ModifyContent(UGCChannelId, CreateContentResponse.Id, UGCModifyContentRequest4,
		THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](const FAccelByteModelsUGCResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Modify content Success"));
		ModifyContentResponse = Response;
		bModifyContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bModifyContentSuccess, "Waiting for modifying content...");

	AB_TEST_TRUE(bModifyContentSuccess);

	// Get content by content id for modifying confirmation.
	bGetContentSuccess = false;
	GetContentResponse = FAccelByteModelsUGCContentResponse();
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentSuccess, &GetContentResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by content id Success"));
		GetContentResponse = Response;
		bGetContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContentSuccess, "Waiting for getting content by content id...");

	AB_TEST_TRUE(bGetContentSuccess);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContentResponse, ModifyContentResponse));

	// Delete content.
	bool bDeleteContentSuccess = false;
	FRegistry::UGC.DeleteContent(UGCChannelId, CreateContentResponse.Id, FVoidHandler::CreateLambda([&bDeleteContentSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete content Success"));
		bDeleteContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteContentSuccess, "Waiting for deleting Content...");

	AB_TEST_TRUE(bDeleteContentSuccess);

	// Check getting content again.
	bool bGetContentNotFoundSuccess = false;
	bool bGetContentNotFoundDone = false;
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentNotFoundDone](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content Success"));
		bGetContentNotFoundDone = true;
	}), FErrorHandler::CreateLambda([&bGetContentNotFoundSuccess, &bGetContentNotFoundDone](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
		bGetContentNotFoundSuccess = true;
		bGetContentNotFoundDone = true;
	}));
	FlushHttpRequests();
	WaitUntil(bGetContentNotFoundDone, "Waiting for getting content...");

	AB_TEST_TRUE(bGetContentNotFoundSuccess);
	AB_TEST_TRUE(bGetContentNotFoundDone);

	// Delete Channel.
	bool bDeleteChannelSuccess = false;
	FRegistry::UGC.DeleteChannel(UGCChannelId, FVoidHandler::CreateLambda([&bDeleteChannelSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete channel Success"));
		bDeleteChannelSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteChannelSuccess, "Waiting for deleting channel...");

	AB_TEST_TRUE(bDeleteChannelSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_Get_Modify_Delete_Content_Empty_FileExtensionAndContentType, "AccelByte.Tests.UGC.C.Create_Get_Modify_Delete_Content_Empty_FileExtensionAndContentType", AutomationFlagMaskUGC)
bool UGCCreate_Get_Modify_Delete_Content_Empty_FileExtensionAndContentType::RunTest(const FString& Parameters)
{
	// Create channel.
	bool bCreateChannelSuccess = false;
	FString CreatedChannelName;
	FRegistry::UGC.CreateChannel(UGCChannelName, THandler<FAccelByteModelsUGCChannelResponse>::CreateLambda([&bCreateChannelSuccess, &CreatedChannelName](const FAccelByteModelsUGCChannelResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create channel Succes"));
		CreatedChannelName = Response.Name;
		UGCChannelId = Response.Id;
		bCreateChannelSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bCreateChannelSuccess, "Waiting for creating channel...");

	AB_TEST_TRUE(bCreateChannelSuccess);
	AB_TEST_EQUAL(CreatedChannelName, UGCChannelName);

	// Create content.
	bool bCreateContentSuccess = false;
	FAccelByteModelsUGCResponse CreateContentResponse;
	FRegistry::UGC.CreateContent(UGCChannelId, UGCCreateContentRequestEmptyFileExtensionAndContentType,
		THandler<FAccelByteModelsUGCResponse>::CreateLambda([&bCreateContentSuccess, &CreateContentResponse](const FAccelByteModelsUGCResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create content Success"));
		CreateContentResponse = Response;
		bCreateContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bCreateContentSuccess, "Waiting for creating content...");

	AB_TEST_TRUE(bCreateContentSuccess);
	AB_TEST_EQUAL(CreateContentResponse.ChannelId, UGCChannelId);
	AB_TEST_EQUAL(CreateContentResponse.ContentType, "application/octet-stream");
	AB_TEST_EQUAL(CreateContentResponse.FileExtension, UGCCreateContentRequestEmptyFileExtensionAndContentType.FileExtension);
	AB_TEST_EQUAL(CreateContentResponse.Name, UGCCreateContentRequestEmptyFileExtensionAndContentType.Name);
	AB_TEST_EQUAL(CreateContentResponse.Preview, UGCCreateContentRequestEmptyFileExtensionAndContentType.Preview);
	AB_TEST_EQUAL(CreateContentResponse.Type, UGCCreateContentRequestEmptyFileExtensionAndContentType.Type);
	AB_TEST_TRUE(UGCCheckContainTag(CreateContentResponse.Tags[0], CreateContentResponse.Tags));
	AB_TEST_TRUE(UGCCheckContainTag(CreateContentResponse.Tags[1], CreateContentResponse.Tags));

	// Upload content payload based using the create content's payload url.
	bool bUploadSuccess = false;
	FAccelByteNetUtilities::UploadTo(CreateContentResponse.PayloadUrl[0].Url, UGCContentBytes,
		FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived )
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Upload progress : %d / %d"), BytesSent, Request->GetContentLength());
	}),
	FVoidHandler::CreateLambda([&bUploadSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Upload content Success"));
		bUploadSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bUploadSuccess, "Waiting for uploading content...");

	AB_TEST_TRUE(bUploadSuccess);

	// Get content by content id.
	bool bGetContentSuccess = false;
	FAccelByteModelsUGCContentResponse GetContentResponse;
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentSuccess, &GetContentResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by content id Success"));
		GetContentResponse = Response;
		bGetContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContentSuccess, "Waiting for getting content by content id...");

	AB_TEST_TRUE(bGetContentSuccess);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContentResponse, CreateContentResponse));

	// Download content payload using get content's payload url.
	bool bDownloadSuccess = false;
	TArray<uint8> DownloadBytes;
	FAccelByteNetUtilities::DownloadFrom(GetContentResponse.PayloadUrl[0].Url, FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived)
	{
		auto Response = Request->GetResponse();
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download progress : %d / %d"), BytesReceived, Response->GetContentLength());
	}), THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& RawData)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download content Success"));
		DownloadBytes = RawData;
		bDownloadSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDownloadSuccess, "Waiting for downloading content...");

	AB_TEST_TRUE(bDownloadSuccess);
	AB_TEST_EQUAL(DownloadBytes, UGCContentBytes);

	// Get content by share code by its content creator.
	bool bGetContenttByShareCodeSuccess = false;
	FAccelByteModelsUGCContentResponse GetContenttByShareCodeResponse;
	FRegistry::UGC.GetContentByShareCode(CreateContentResponse.ShareCode,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContenttByShareCodeSuccess, &GetContenttByShareCodeResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by share code by its content creator Success"));
		GetContenttByShareCodeResponse = Response;
		bGetContenttByShareCodeSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContenttByShareCodeSuccess, "Waiting for getting content by share code by its content creator...");

	AB_TEST_TRUE(bGetContenttByShareCodeSuccess);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContenttByShareCodeResponse, CreateContentResponse));

	// Get content by share code by User2.
	bool bGetContenttByShareCodeUser2Success = false;
	FAccelByteModelsUGCContentResponse GetContentByShareCodeUser2Response;
	UGCUser2Registry->UGC.GetContentByShareCode(CreateContentResponse.ShareCode,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContenttByShareCodeUser2Success, &GetContentByShareCodeUser2Response](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by share code by User2 Success"));
		GetContentByShareCodeUser2Response = Response;
		bGetContenttByShareCodeUser2Success = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContenttByShareCodeUser2Success, "Waiting for getting content by share code by User2...");

	AB_TEST_TRUE(bGetContenttByShareCodeUser2Success);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContentByShareCodeUser2Response, CreateContentResponse));

	// Download content payload using get content by share coded's payload url.
	bDownloadSuccess = false;
	DownloadBytes.Empty();
	FAccelByteNetUtilities::DownloadFrom(GetContentResponse.PayloadUrl[0].Url, FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived)
	{
		auto Response = Request->GetResponse();
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download progress : %d / %d"), BytesReceived, Response->GetContentLength());
	}), THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& RawData)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download content Success"));
		DownloadBytes = RawData;
		bDownloadSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDownloadSuccess, "Waiting for downloading content...");

	AB_TEST_TRUE(bDownloadSuccess);
	AB_TEST_EQUAL(DownloadBytes, UGCContentBytes);

	// Get content preview as string.
	bool bGetPreviewSuccess = false;
	FAccelByteModelsUGCPreview GetPreviewResponse;
	FRegistry::UGC.GetContentPreview(CreateContentResponse.Id, THandler<FAccelByteModelsUGCPreview>::CreateLambda([&bGetPreviewSuccess, &GetPreviewResponse](const FAccelByteModelsUGCPreview& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content preview as string Success"));
		GetPreviewResponse = Response;
		bGetPreviewSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetPreviewSuccess, "Waiting for getting content as string...");

	AB_TEST_TRUE(bGetPreviewSuccess); 
	AB_TEST_EQUAL(GetPreviewResponse.Preview, UGCCreateContentRequest.Preview);

	// Get content preview as TArray<uint8>.
	bool bGetPreviewBytesSuccess = false;
	TArray<uint8> GetPreviewBytesResponse;
	FRegistry::UGC.GetContentPreview(CreateContentResponse.Id, THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& GetResponse)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content preview as TArray<uint8> Success"));
		GetPreviewBytesResponse = GetResponse;
		bGetPreviewBytesSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetPreviewBytesSuccess, "Waiting for getting content as TArray<uint8>...");

	AB_TEST_TRUE(bGetPreviewBytesSuccess);
	AB_TEST_EQUAL(GetPreviewBytesResponse, UGCPreviewBytes);
	
	// Modify content.
	bool bModifyContentSuccess = false;
	bModifyContentSuccess = false;
	FAccelByteModelsUGCResponse ModifyContentResponse;
	FRegistry::UGC.ModifyContent(UGCChannelId, CreateContentResponse.Id, UGCModifyContentRequest3.Name, UGCModifyContentRequest3.Type,
		UGCModifyContentRequest3.SubType, UGCModifyContentRequest3.Tags,
		UGCPreviewBytes, UGCModifyContentRequest3.FileExtension, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](FAccelByteModelsUGCResponse CreateResponse)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Modify content Success"));
		ModifyContentResponse = CreateResponse;
		bModifyContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bModifyContentSuccess, "Waiting for modifying content...");

	AB_TEST_TRUE(bModifyContentSuccess);

	// Get content by content id for modifying confirmation.
	bGetContentSuccess = false;
	GetContentResponse = FAccelByteModelsUGCContentResponse();
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentSuccess, &GetContentResponse](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content by content id Success"));
		GetContentResponse = Response;
		bGetContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bGetContentSuccess, "Waiting for getting content by content id...");

	AB_TEST_TRUE(bGetContentSuccess);
	AB_TEST_TRUE(UGCCheckContentEqual(GetContentResponse, ModifyContentResponse));

	// Delete content.
	bool bDeleteContentSuccess = false;
	FRegistry::UGC.DeleteContent(UGCChannelId, CreateContentResponse.Id, FVoidHandler::CreateLambda([&bDeleteContentSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete content Success"));
		bDeleteContentSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteContentSuccess, "Waiting for deleting Content...");

	AB_TEST_TRUE(bDeleteContentSuccess);

	// Check getting content again.
	bool bGetContentNotFoundSuccess = false;
	bool bGetContentNotFoundDone = false;
	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentNotFoundDone](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content Success"));
		bGetContentNotFoundDone = true;
	}), FErrorHandler::CreateLambda([&bGetContentNotFoundSuccess, &bGetContentNotFoundDone](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
		bGetContentNotFoundSuccess = true;
		bGetContentNotFoundDone = true;
	}));
	FlushHttpRequests();
	WaitUntil(bGetContentNotFoundDone, "Waiting for getting content...");

	AB_TEST_TRUE(bGetContentNotFoundSuccess);
	AB_TEST_TRUE(bGetContentNotFoundDone);

	// Delete Channel.
	bool bDeleteChannelSuccess = false;
	FRegistry::UGC.DeleteChannel(UGCChannelId, FVoidHandler::CreateLambda([&bDeleteChannelSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete channel Success"));
		bDeleteChannelSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	WaitUntil(bDeleteChannelSuccess, "Waiting for deleting channel...");

	AB_TEST_TRUE(bDeleteChannelSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreateContentEmptyChannelId, "AccelByte.Tests.UGC.C.CreateContent_EmptyChannelId", AutomationFlagMaskUGC)
bool UGCCreateContentEmptyChannelId::RunTest(const FString& Parameters)
{
	bool bCreateContentSuccess = false;
	bool bCreateContentDone = false;
	FRegistry::UGC.CreateContent(TEXT(""), UGCCreateContentRequest, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&bCreateContentSuccess, &bCreateContentDone](const FAccelByteModelsUGCResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create content Success"));
		bCreateContentSuccess = true;
		bCreateContentDone = true;
	}), FErrorHandler::CreateLambda([&bCreateContentDone](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
		bCreateContentDone = true;
	}));
	FlushHttpRequests();
	WaitUntil(bCreateContentDone, "Waiting for creating content...");

	AB_TEST_TRUE(bCreateContentDone);
	AB_TEST_FALSE(bCreateContentSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreateContentInvalidChannelId, "AccelByte.Tests.UGC.C.CreateContent_InvalidId", AutomationFlagMaskUGC)
bool UGCCreateContentInvalidChannelId::RunTest(const FString& Parameters)
{
	bool bCreateContentSuccess = false;
	bool bCreateContentDone = false;
	FRegistry::UGC.CreateContent(TEXT("InvalidChannelId"), UGCCreateContentRequest, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&bCreateContentSuccess, &bCreateContentDone](const FAccelByteModelsUGCResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create content Success"));
		bCreateContentSuccess = true;
		bCreateContentDone = true;
	}), FErrorHandler::CreateLambda([&bCreateContentDone](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
		bCreateContentDone = true;
	}));
	FlushHttpRequests();
	WaitUntil(bCreateContentDone, "Waiting for creating content...");

	AB_TEST_TRUE(bCreateContentDone);
	AB_TEST_FALSE(bCreateContentSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCGetContentEmptyContentId, "AccelByte.Tests.UGC.C.GetContent_EmptyContentId", AutomationFlagMaskUGC)
bool UGCGetContentEmptyContentId::RunTest(const FString& Parameters)
{
	bool bGetContentDone = false;
	bool bGetContentSuccess = false;
	FRegistry::UGC.GetContentByContentId(TEXT(""),
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentDone, &bGetContentSuccess](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get content Success"));
		bGetContentSuccess = true;
		bGetContentDone = true;
	}), FErrorHandler::CreateLambda([&bGetContentDone](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
		bGetContentDone = true;
	}));
	FlushHttpRequests();
	WaitUntil(bGetContentDone, "Waiting for getting content by empty content id...");

	AB_TEST_TRUE(bGetContentDone);
	AB_TEST_FALSE(bGetContentSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCGetContentInvalidContentId, "AccelByte.Tests.UGC.C.GetContent_InvalidContentId", AutomationFlagMaskUGC)
bool UGCGetContentInvalidContentId::RunTest(const FString& Parameters)
{
	bool bGetContentDone = false;
	bool bGetContentSuccess = false;
	FRegistry::UGC.GetContentByContentId(TEXT("InvalidContentId"),
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentDone, &bGetContentSuccess](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Content Success"));
		bGetContentSuccess = true;
		bGetContentDone = true;
	}), FErrorHandler::CreateLambda([&bGetContentDone](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
		bGetContentDone = true;
	}));
	FlushHttpRequests();
	WaitUntil(bGetContentDone, "Waiting for getting content by invalid content id...");

	AB_TEST_TRUE(bGetContentDone);
	AB_TEST_FALSE(bGetContentSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCGetContentEmptyShareCode, "AccelByte.Tests.UGC.C.GetContent_EmptyShareCode", AutomationFlagMaskUGC)
bool UGCGetContentEmptyShareCode::RunTest(const FString& Parameters)
{
	bool bGetContentDone = false;
	bool bGetContentSuccess = false;
	FRegistry::UGC.GetContentByShareCode(TEXT(""),
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentDone, &bGetContentSuccess](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Content Success"));
		bGetContentSuccess = true;
		bGetContentDone = true;
	}), FErrorHandler::CreateLambda([&bGetContentDone](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
		bGetContentDone = true;
	}));
	FlushHttpRequests();
	WaitUntil(bGetContentDone, "Waiting for getting content by empty share code...");

	AB_TEST_TRUE(bGetContentDone);
	AB_TEST_FALSE(bGetContentSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCGetContentInvalidShareCode, "AccelByte.Tests.UGC.C.GetContent_InvalidShareCode", AutomationFlagMaskUGC)
bool UGCGetContentInvalidShareCode::RunTest(const FString& Parameters)
{
	bool bGetContentDone = false;
	bool bGetContentSuccess = false;
	FRegistry::UGC.GetContentByShareCode(TEXT("InvalidShareCode"),
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&bGetContentDone, &bGetContentSuccess](const FAccelByteModelsUGCContentResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Content Success"));
		bGetContentSuccess = true;
		bGetContentDone = true;
	}), FErrorHandler::CreateLambda([&bGetContentDone](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
		bGetContentDone = true;
	}));
	FlushHttpRequests();
	WaitUntil(bGetContentDone, "Waiting for getting content by invalid share code...");

	AB_TEST_TRUE(bGetContentDone);
	AB_TEST_FALSE(bGetContentSuccess);

	return true;
}