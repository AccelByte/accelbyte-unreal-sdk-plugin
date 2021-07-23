// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "TestUtilities.h"
#include "UGCTestAdmin.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteUGCApi.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteRegistry.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteUGCTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteUGCTest);

const int32 AutomationFlagMaskUGC = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
void FlushHttpRequests();
void UGCCreateType(const FString& Type, const TArray<FString>& SubType, const THandler<FAccelByteModelsUGCTypeResponse>& OnSuccess, const FErrorHandler& OnError);
void UGCCreateTags(const FString& Tag, const THandler<FAccelByteModelsUGCTagResponse>& OnSuccess, const FErrorHandler& OnError);
void UGCDeleteType(const FString& TypeId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
void UGCDeleteTag(const FString& TagId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

// Setup variables
FString UGCChannelName = TEXT("Integration Test Channel UE4");
FString UGCType = TEXT("Vehicle");
TArray<FString> UGCSubType = {TEXT("Body"), TEXT("Wheel"), TEXT("Vynil")};
TArray<FString> UGCTags = {TEXT("Sedan"), TEXT("Minibus"), TEXT("Sport")};
FString UGCInvalidChannelId = TEXT("InvalidChannelId");
FString UGCInvalidContentId = TEXT("InvalidContentId");
FAccelByteModelsUGCRequest UGCCreateContentRequest = {
	"png", "Integration Test UE4", "", UGCType, UGCSubType[0], {UGCTags[0], UGCTags[1]}
};
FAccelByteModelsUGCRequest UGCModifyContentRequest = {
	"txt", "MODIFIED Integration Test UE4", "", UGCType, UGCSubType[1], {UGCTags[1], UGCTags[2]}
};
FString ModifiedUploadedContent = TEXT("Integration UE4 Upload Modified Test");
FAccelByteModelsUGCChannelResponse UGCCreatedChannel;
FAccelByteModelsUGCTypeResponse UGCCreatedType;
TArray<FAccelByteModelsUGCTagResponse> UGCCreatedTags;
TArray<uint8> UGCPreviewBytes;
FString UGCPreviewString;
FString UGCDownloadString;

const auto UGCOnError = FErrorHandler::CreateLambda([](int32 Code, const FString& Message)
{
	UE_LOG(LogAccelByteUGCTest, Error, TEXT("Error code: %d\nError message:%s"), Code, *Message);
});

bool UGCCheckContainsChannel(const FAccelByteModelsUGCChannelResponse& Compare1, const FAccelByteModelsUGCChannelsPagingResponse& Compare2)
{
	for(auto Channel : Compare2.Data)
	{
		if(Channel.Id.Equals(Compare1.Id))
		{
			return true;
		}
	}
	return false;
};

bool UGCCheckFoundCreatedTags(const FAccelByteModelsUGCTagsPagingResponse& Compare)
{
	for(auto CreatedTag : UGCCreatedTags)
	{
		bool bIsFound = false;
		for(auto Tag : Compare.Data)
		{
			if(CreatedTag.Id == Tag.Id)
			{
				bIsFound = true;
				break;
			}
		}
		if(!bIsFound)
		{
			return false;
		}
	}
	return true;
};

bool UGCCheckFoundCreatedType(const FAccelByteModelsUGCTypesPagingResponse& Compare)
{
	for(auto Type : Compare.Data)
	{
		if(Type.Id.Equals(UGCCreatedType.Id))
		{
			return true;
		}
	}
	return false;
};

bool UGCCheckContentEqual(const FAccelByteModelsUGCResponse& Compare1, const FAccelByteModelsUGCContentResponse& Compare2)
{
	if(!Compare1.Id.Equals(Compare2.Id)) { UE_LOG(LogAccelByteUGCTest, Log, TEXT("Id1 : %s != Id2 : %s"), *Compare1.Id, *Compare2.Id); return false; }
	if(!Compare1.Name.Equals(Compare2.Name)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("Name1 : %s != Name2 : %s"), *Compare1.Name, *Compare2.Name); return false; }
	if(!Compare1.Namespace.Equals(Compare2.Namespace)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("Namespace1 : %s != Namespace2 : %s"), *Compare1.Namespace, *Compare2.Namespace); return false; }
	if(!Compare1.Type.Equals(Compare2.Type)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("Type1 : %s != Type2 : %s"), *Compare1.Type, *Compare2.Type); return false; }
	if(!Compare1.ChannelId.Equals(Compare2.ChannelId)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("ChannelId1 : %s != ChannelId2 : %s"), *Compare1.ChannelId, *Compare2.ChannelId); return false; }
	if(!Compare1.CreatorName.Equals(Compare2.CreatorName)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("CreatorName1 : %s != CreatorName2 : %s"), *Compare1.CreatorName, *Compare2.CreatorName); return false; }
	if(!Compare1.FileExtension.Equals(Compare2.FileExtension)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("FileExtension1 : %s != FileExtension2 : %s"), *Compare1.FileExtension, *Compare2.FileExtension); return false; }
	if(!Compare1.ShareCode.Equals(Compare2.ShareCode)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("ShareCode1 : %s != ShareCode2 : %s"), *Compare1.ShareCode, *Compare2.ShareCode); return false; }
	if(!Compare1.SubType.Equals(Compare2.SubType)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("SubType1 : %s != SubType2 : %s"), *Compare1.SubType, *Compare2.SubType); return false; };
	if(!Compare1.UserId.Equals(Compare2.UserId)) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("UserId1 : %s != UserId2 : %s"), *Compare1.UserId, *Compare2.UserId); return false; }

	if(Compare1.CreatedTime != Compare2.CreatedTime) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("CreatedTime1 : %s != CreatedTime2 : %s"), *Compare1.CreatedTime.ToString(), *Compare2.CreatedTime.ToString());return false; }
	if(Compare1.IsOfficial != Compare2.IsOfficial) { UE_LOG(LogAccelByteUGCTest, Warning, TEXT("IsOfficial1 : %s != IsOfficial2 : %s"), Compare1.IsOfficial ? TEXT("true") : TEXT("false"), Compare2.IsOfficial ? TEXT("true") : TEXT("false")) ;return false; }
	
	for(const FString& Tag : Compare1.Tags)
	{
		bool bIsFound = false;
		for(const FString& Tag2 : Compare2.Tags)
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
	Waiting(bUGCLoginUserSuccess, "Waiting for login...");

	AB_TEST_TRUE(bUGCLoginUserSuccess);

	bool bUGCDeleteUserSuccess = false;
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bUGCDeleteUserSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete user Success"));
		bUGCDeleteUserSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	Waiting(bUGCDeleteUserSuccess, "Waiting for user deletion...");

	AB_TEST_TRUE(bUGCDeleteUserSuccess);

	// Login again.
	bUGCLoginUserSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUGCLoginUserSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("User login Success"));
		bUGCLoginUserSuccess = true;
	}), UGCOnError);
	FlushHttpRequests();
	Waiting(bUGCLoginUserSuccess, "Waiting for login...");

	AB_TEST_TRUE(bUGCLoginUserSuccess);
	
	// Setup UGC configuration.

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
	Waiting(bGetTypesSuccess, "Waiting for getting types...");

	for(auto ResponseType : GetTypesResponse.Data)
	{
		if(ResponseType.Type.Equals(UGCType))
		{
			bool bDeleteTypeSuccess = false;
			UGCDeleteType(ResponseType.Id, FVoidHandler::CreateLambda([&bDeleteTypeSuccess]()
			{
				UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete type Success"));
				bDeleteTypeSuccess = true;
			}), UGCOnError);
			Waiting(bDeleteTypeSuccess, "Waiting for deleting type...");

			AB_TEST_TRUE(bDeleteTypeSuccess);
		}
	}
	
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
	Waiting(bGetTagsSuccess, "Waiting for getting tags...");

	for(auto UGCTag : UGCTags)
	{
		for(auto ResponseTag : GetTagsResponse.Data)
		{
			if(ResponseTag.Tag.Equals(UGCTag))
			{
				bool bDeleteTagSuccess = false;
				UGCDeleteTag(ResponseTag.Id, FVoidHandler::CreateLambda([&bDeleteTagSuccess]()
				{
					UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete tag Success"));
					bDeleteTagSuccess = true;
				}), UGCOnError);
				Waiting(bDeleteTagSuccess, "Waiting for deleting tag...");

				AB_TEST_TRUE(bDeleteTagSuccess);
			}
		}
	}

	// Create type.
	bool bCreateTypeSuccess;
	UGCCreateType(UGCType, UGCSubType, THandler<FAccelByteModelsUGCTypeResponse>::CreateLambda([&bCreateTypeSuccess](const FAccelByteModelsUGCTypeResponse& Response)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create type Success"));
		UGCCreatedType = Response;
		bCreateTypeSuccess = true;
	}), UGCOnError);
	Waiting(bCreateTypeSuccess, "Waiting for creating type...");

	AB_TEST_TRUE(bCreateTypeSuccess);
	
	for(const FString& UGCTag : UGCTags)
	{
		bool bCreateTagSuccess = false;
		UGCCreateTags(UGCTag, THandler<FAccelByteModelsUGCTagResponse>::CreateLambda([&bCreateTagSuccess](const FAccelByteModelsUGCTagResponse& Response)
		{
			UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create tag Success"));
			UGCCreatedTags.Add(Response);
			bCreateTagSuccess = true;
		}), UGCOnError);
		Waiting(bCreateTagSuccess, "Waiting for creating tag...");

		AB_TEST_TRUE(bCreateTagSuccess);
	}
	
	bool bDeleteChannelDone;
	FRegistry::UGC.DeleteChannel(UGCCreatedChannel.Id, FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete channel Success"));
		bDeleteChannelDone = true;
	}), FErrorHandler::CreateLambda([&bDeleteChannelDone](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
		bDeleteChannelDone = true;
	}));
	Waiting(bDeleteChannelDone, "Waiting for deleting channel...");

	AB_TEST_TRUE(bDeleteChannelDone);
	
	//bool bCreateChannelSuccess;
	//FRegistry::UGC.CreateChannel(UGCChannelName, THandler<FAccelByteModelsUGCChannelResponse>::CreateLambda([&](const FAccelByteModelsUGCChannelResponse& Response)
	//{
	//	UE_LOG(LogAccelByteUGCTest, Log, TEXT("Success Create channel"));
	//	UGCCreatedChannel = Response;
	//	bCreateChannelSuccess = true;
	//}), UGCOnError);
	//Waiting(bCreateChannelSuccess, "Waiting to create channel ...");
	//AB_TEST_TRUE(bCreateChannelSuccess);

	//FString FilePath = FString::Printf(TEXT("%s/AccelByteUe4Sdk/Resources/Icon128.png"), *FPaths::ConvertRelativePathToFull(FPaths::ProjectPluginsDir()));
	//bool bLoadIconPreviewToBytes = FFileHelper::LoadFileToArray(UGCPreviewBytes, *FilePath, 0);
	//UGCPreviewString = FBase64::Encode(UGCPreviewBytes);

	//AB_TEST_TRUE(bLoadIconPreviewToBytes);
	//AB_TEST_TRUE(!UGCPreviewString.IsEmpty());

	//UGCCreateContentRequest.Preview = UGCPreviewString;
	//UGCModifyContentRequest.Preview = UGCPreviewString;
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCTeardown, "AccelByte.Tests.UGC.Z.Teardown", AutomationFlagMaskUGC)
bool UGCTeardown::RunTest(const FString& Parameters)
{
	bool bDeleteTypeSuccess;
	UGCDeleteType(UGCCreatedType.Id, FVoidHandler::CreateLambda([&bDeleteTypeSuccess]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete type Success"));
		bDeleteTypeSuccess = true;
	}), UGCOnError);
	Waiting(bDeleteTypeSuccess, "Waiting for deleting type...");

	AB_TEST_TRUE(bDeleteTypeSuccess);
	
	for(auto UGCCreatedTag : UGCCreatedTags)
	{
		bool bDeleteTagSuccess = false;
		UGCDeleteTag(UGCCreatedTag.Id, FVoidHandler::CreateLambda([&bDeleteTagSuccess]()
		{
			UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete tag Success"));
			bDeleteTagSuccess = true;
		}), UGCOnError);
		Waiting(bDeleteTagSuccess, "Waiting for deleting tag...");

		AB_TEST_TRUE(bDeleteTagSuccess);
	}

	//bool bDeleteChannelSuccess;
	//FRegistry::UGC.DeleteChannel(UGCCreatedChannel.Id, FVoidHandler::CreateLambda([&]()
	//{
	//	UE_LOG(LogAccelByteUGCTest, Log, TEXT("Success Delete Channel"));
	//	UGCCreatedChannel = {};
	//	bDeleteChannelSuccess = true;
	//}), UGCOnError);
	//Waiting(bDeleteChannelSuccess, "Waiting to delete channel ...");
	//AB_TEST_TRUE(bDeleteChannelSuccess);
	
	bool bDeleteUserSuccess;
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete user Success"));
		bDeleteUserSuccess = true;
	}), UGCOnError);
	Waiting(bDeleteUserSuccess, "Waiting for deleting user...");

	AB_TEST_TRUE(bDeleteUserSuccess);
	
	return true;
}

//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_Get_Delete_Channels, "AccelByte.Tests.UGC.B.Create_Get_Delete_Channels", AutomationFlagMaskUGC);
//bool UGCCreate_Get_Delete_Channels::RunTest(const FString& Parameters)
//{
//	bool bCreateChannel2Succes = false;
//	FAccelByteModelsUGCChannelResponse ChannelResponse2;
//	FRegistry::UGC.CreateChannel(TEXT("Integration TEST Creation Channel 2"), THandler<FAccelByteModelsUGCChannelResponse>::CreateLambda([&bCreateChannel2Succes, &ChannelResponse2](const FAccelByteModelsUGCChannelResponse& Response)
//	{
//		bCreateChannel2Succes = true;
//		ChannelResponse2 = Response;
//	}), UGCOnError);
//	Waiting(bCreateChannel2Succes, "Waiting to create channel ...");
//	AB_TEST_TRUE(bCreateChannel2Succes);
//
//	bool bGetChannel2Success = false;
//	FAccelByteModelsUGCChannelsPagingResponse GetChannelResponse2;
//	FRegistry::UGC.GetChannels(THandler<FAccelByteModelsUGCChannelsPagingResponse>::CreateLambda([&bGetChannel2Success, &GetChannelResponse2](const FAccelByteModelsUGCChannelsPagingResponse& Response)
//	{
//		bGetChannel2Success = true;
//		GetChannelResponse2 = Response;
//	}), UGCOnError);
//	Waiting(bGetChannel2Success, "Waiting to get channel");
//	AB_TEST_TRUE(bGetChannel2Success);
//	AB_TEST_TRUE(UGCCheckContainsChannel(ChannelResponse2, GetChannelResponse2));
//
//	bool bDeleteChannel2Success = false;
//	FRegistry::UGC.DeleteChannel(ChannelResponse2.Id, FVoidHandler::CreateLambda([&bDeleteChannel2Success]()
//	{
//		bDeleteChannel2Success = true;
//	}), UGCOnError);
//	Waiting(bDeleteChannel2Success, "Waiting to delete channel");
//	AB_TEST_TRUE(bDeleteChannel2Success);
//	
//	return true;
//}
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCGetChannels, "AccelByte.Tests.UGC.B.GetChannels", AutomationFlagMaskUGC);
//bool UGCGetChannels::RunTest(const FString& Parameters)
//{
//	bool bGetChannelSuccess;
//	FAccelByteModelsUGCChannelsPagingResponse GetResponse;
//	FRegistry::UGC.GetChannels(THandler<FAccelByteModelsUGCChannelsPagingResponse>::CreateLambda([&](const FAccelByteModelsUGCChannelsPagingResponse& Response)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Channel Success"));
//		bGetChannelSuccess = true;
//		GetResponse = Response;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bGetChannelSuccess, "Getting channels");
//	AB_TEST_TRUE(bGetChannelSuccess);
//	AB_TEST_TRUE(UGCCheckContainsChannel(UGCCreatedChannel, GetResponse));
//
//	return true;
//}
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCGetTags, "AccelByte.Tests.UGC.B.GetTags", AutomationFlagMaskUGC);
//bool UGCGetTags::RunTest(const FString& Parameters)
//{
//	bool bGetTagsSuccess;
//	FAccelByteModelsUGCTagsPagingResponse GetResponse;
//	FRegistry::UGC.GetTags(THandler<FAccelByteModelsUGCTagsPagingResponse>::CreateLambda([&](const FAccelByteModelsUGCTagsPagingResponse& Response)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Tags Success"));
//		bGetTagsSuccess = true;
//		GetResponse = Response;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bGetTagsSuccess, "Getting Tags");
//	AB_TEST_TRUE(bGetTagsSuccess);
//	AB_TEST_TRUE(UGCCheckFoundCreatedTags(GetResponse));
//	
//	return true;
//}
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCGetTypes, "AccelByte.Tests.UGC.B.GetTypes", AutomationFlagMaskUGC);
//bool UGCGetTypes::RunTest(const FString& Parameters)
//{
//	bool bGetTypesSuccess;
//	FAccelByteModelsUGCTypesPagingResponse GetResponse;
//	FRegistry::UGC.GetTypes(THandler<FAccelByteModelsUGCTypesPagingResponse>::CreateLambda([&](const FAccelByteModelsUGCTypesPagingResponse& Response)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Types Success"));
//		bGetTypesSuccess = true;
//		GetResponse = Response;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bGetTypesSuccess, "Getting types");
//	AB_TEST_TRUE(bGetTypesSuccess);
//	AB_TEST_TRUE(UGCCheckFoundCreatedType(GetResponse));
//
//	
//	return true;
//}
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_Get_DeleteContent, "AccelByte.Tests.UGC.C.Create_Get_DeleteContent", AutomationFlagMaskUGC)
//bool UGCCreate_Get_DeleteContent::RunTest(const FString& Parameters)
//{
//	FAccelByteModelsUGCResponse CreateContentResponse;
//	bool bCreateContentSuccess;
//	FRegistry::UGC.CreateContent(UGCCreatedChannel.Id, UGCCreateContentRequest, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](const FAccelByteModelsUGCResponse& CreateResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create Content Success"));
//		CreateContentResponse = CreateResponse;
//		bCreateContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bCreateContentSuccess, "Waiting to Create Content...");
//	AB_TEST_TRUE(bCreateContentSuccess);
//	AB_TEST_TRUE(!CreateContentResponse.Id.IsEmpty());
//
//	FAccelByteModelsUGCContentResponse GetContentResponse;
//	bool bGetContentSuccess; 
//	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id, THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&](const FAccelByteModelsUGCContentResponse& GetResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Content Success"));
//		GetContentResponse = GetResponse;
//		bGetContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bGetContentSuccess, "Waiting to Get Content...");
//	AB_TEST_TRUE(bGetContentSuccess);
//	AB_TEST_TRUE(UGCCheckContentEqual(CreateContentResponse, GetContentResponse));
//
//	bool bDeleteContentSuccess = false;
//	FRegistry::UGC.DeleteContent(UGCCreatedChannel.Id, CreateContentResponse.Id, FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete Content Success"));
//		bDeleteContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bDeleteContentSuccess, "Waiting to Delete Content...");
//	AB_TEST_TRUE(bDeleteContentSuccess);
//
//	return true;
//}
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_GetPreview_DeleteContent, "AccelByte.Tests.UGC.C.Create_GetPreview_DeleteContent", AutomationFlagMaskUGC)
//bool UGCCreate_GetPreview_DeleteContent::RunTest(const FString& Parameters)
//{
//	FAccelByteModelsUGCResponse CreateContentResponse;
//	bool bCreateContentSuccess;
//	FRegistry::UGC.CreateContent(UGCCreatedChannel.Id, UGCCreateContentRequest, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](const FAccelByteModelsUGCResponse& CreateResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create Content Success"));
//		CreateContentResponse = CreateResponse;
//		bCreateContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bCreateContentSuccess, "Waiting to Create Content...");
//	AB_TEST_TRUE(bCreateContentSuccess);
//	AB_TEST_TRUE(!CreateContentResponse.Id.IsEmpty());
//
//	FAccelByteModelsUGCPreview GetPreviewResponse;
//	bool bGetPreviewSuccess; 
//	FRegistry::UGC.GetContentPreview(CreateContentResponse.Id, THandler<FAccelByteModelsUGCPreview>::CreateLambda([&](const FAccelByteModelsUGCPreview& GetResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Content Success"));
//		GetPreviewResponse = GetResponse;
//		bGetPreviewSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bGetPreviewSuccess, "Waiting to Get Content...");
//	AB_TEST_TRUE(bGetPreviewSuccess);
//	AB_TEST_EQUAL(UGCPreviewString, GetPreviewResponse.Preview);
//
//	TArray<uint8> GetPreviewBytesResponse;
//	bool bGetPreviewBytesSuccess; 
//	FRegistry::UGC.GetContentPreview(CreateContentResponse.Id, THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& GetResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Content Success"));
//		GetPreviewBytesResponse = GetResponse;
//		bGetPreviewBytesSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bGetPreviewBytesSuccess, "Waiting to Get Content...");
//	AB_TEST_TRUE(bGetPreviewBytesSuccess);
//	AB_TEST_EQUAL(GetPreviewBytesResponse, UGCPreviewBytes);
//	
//	bool bDeleteContentSuccess = false;
//	FRegistry::UGC.DeleteContent(UGCCreatedChannel.Id, CreateContentResponse.Id, FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete Content Success"));
//		bDeleteContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bDeleteContentSuccess, "Waiting to Delete Content...");
//	AB_TEST_TRUE(bDeleteContentSuccess);
//
//	return true;
//}
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreatePreviewBytes_Get_DeleteContent, "AccelByte.Tests.UGC.C.CreatePreviewBytes_Get_DeleteContent", AutomationFlagMaskUGC)
//bool UGCCreatePreviewBytes_Get_DeleteContent::RunTest(const FString& Parameters)
//{
//	FAccelByteModelsUGCResponse CreateContentResponse;
//	bool bCreateContentSuccess;
//	FRegistry::UGC.CreateContent(UGCCreatedChannel.Id, UGCCreateContentRequest.Name, UGCCreateContentRequest.Type, UGCCreateContentRequest.SubType,
//		UGCCreateContentRequest.Tags, UGCPreviewBytes, UGCCreateContentRequest.FileExtension, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](const FAccelByteModelsUGCResponse& CreateResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create Content Success"));
//		CreateContentResponse = CreateResponse;
//		bCreateContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bCreateContentSuccess, "Waiting to Create Content...");
//	AB_TEST_TRUE(bCreateContentSuccess);
//	AB_TEST_TRUE(!CreateContentResponse.Id.IsEmpty());
//
//	FAccelByteModelsUGCContentResponse GetContentResponse;
//	bool bGetContentSuccess; 
//	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id, THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&](const FAccelByteModelsUGCContentResponse& GetResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Content Success"));
//		GetContentResponse = GetResponse;
//		bGetContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bGetContentSuccess, "Waiting to Get Content...");
//	AB_TEST_TRUE(bGetContentSuccess);
//	AB_TEST_TRUE(UGCCheckContentEqual(CreateContentResponse, GetContentResponse));
//
//	bool bDeleteContentSuccess = false;
//	FRegistry::UGC.DeleteContent(UGCCreatedChannel.Id, CreateContentResponse.Id, FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete Content Success"));
//		bDeleteContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bDeleteContentSuccess, "Waiting to Delete Content...");
//	AB_TEST_TRUE(bDeleteContentSuccess);
//
//	return true;
//}
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_GetByShareCode_Delete_Content, "AccelByte.Tests.UGC.C.Create_GetByShareCode_Delete_Content", AutomationFlagMaskUGC)
//bool UGCCreate_GetByShareCode_Delete_Content::RunTest(const FString& Parameters)
//{
//	FAccelByteModelsUGCResponse CreateContentResponse;
//	bool bCreateContentSuccess;
//	FRegistry::UGC.CreateContent(UGCCreatedChannel.Id, UGCCreateContentRequest, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](const FAccelByteModelsUGCResponse& CreateResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create Content Success"));
//		CreateContentResponse = CreateResponse;
//		bCreateContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bCreateContentSuccess, "Waiting to Create Content...");
//	AB_TEST_TRUE(bCreateContentSuccess);
//	AB_TEST_TRUE(!CreateContentResponse.Id.IsEmpty());
//
//	FAccelByteModelsUGCContentResponse GetContentResponse;
//	bool bGetContentSuccess; 
//	FRegistry::UGC.GetContentByShareCode(CreateContentResponse.ShareCode, THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&](const FAccelByteModelsUGCContentResponse& GetResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Content Success"));
//		GetContentResponse = GetResponse;
//		bGetContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bGetContentSuccess, "Waiting to Get Content...");
//	AB_TEST_TRUE(bGetContentSuccess);
//	AB_TEST_TRUE(UGCCheckContentEqual(CreateContentResponse, GetContentResponse));
//
//	bool bDeleteContentSuccess = false;
//	FRegistry::UGC.DeleteContent(UGCCreatedChannel.Id, CreateContentResponse.Id, FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete Content Success"));
//		bDeleteContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bDeleteContentSuccess, "Waiting to Delete Content...");
//	AB_TEST_TRUE(bDeleteContentSuccess);
//
//	return true;
//}
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_Modify_Delete_Content, "AccelByte.Tests.UGC.C.Create_Modify_Delete_Content", AutomationFlagMaskUGC)
//bool UGCCreate_Modify_Delete_Content::RunTest(const FString& Parameters)
//{
//	FAccelByteModelsUGCResponse CreateContentResponse;
//	bool bCreateContentSuccess;
//	FRegistry::UGC.CreateContent(UGCCreatedChannel.Id, UGCCreateContentRequest, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](const FAccelByteModelsUGCResponse& CreateResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create Content Success"));
//		CreateContentResponse = CreateResponse;
//		bCreateContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bCreateContentSuccess, "Waiting to Create Content...");
//	AB_TEST_TRUE(bCreateContentSuccess);
//	AB_TEST_TRUE(!CreateContentResponse.Id.IsEmpty());
//
//	FAccelByteModelsUGCResponse ModifyContentResponse;
//	bool bModifyContentSuccess;
//	FRegistry::UGC.ModifyContent(UGCCreatedChannel.Id, CreateContentResponse.Id, UGCModifyContentRequest,
//		THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](const FAccelByteModelsUGCResponse& CreateResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create Content Success"));
//		ModifyContentResponse = CreateResponse;
//		bModifyContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bModifyContentSuccess, "Waiting to Create Content...");
//	AB_TEST_TRUE(bModifyContentSuccess);
//
//	FAccelByteModelsUGCContentResponse GetContentResponse;
//	bool bGetContentSuccess; 
//	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id, THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&](const FAccelByteModelsUGCContentResponse& GetResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Content Success"));
//		GetContentResponse = GetResponse;
//		bGetContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bGetContentSuccess, "Waiting to Get Content...");
//	AB_TEST_TRUE(bGetContentSuccess);
//	AB_TEST_TRUE(UGCCheckContentEqual(ModifyContentResponse, GetContentResponse));
//
//	bool bDeleteContentSuccess = false;
//	FRegistry::UGC.DeleteContent(UGCCreatedChannel.Id, CreateContentResponse.Id, FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete Content Success"));
//		bDeleteContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bDeleteContentSuccess, "Waiting to Delete Content...");
//	AB_TEST_TRUE(bDeleteContentSuccess);
//
//	return true;
//}
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_ModifyPreviewBytes_Delete_Content, "AccelByte.Tests.UGC.C.Create_ModifyPreviewBytes_Delete_Content", AutomationFlagMaskUGC)
//bool UGCCreate_ModifyPreviewBytes_Delete_Content::RunTest(const FString& Parameters)
//{
//	FAccelByteModelsUGCResponse CreateContentResponse;
//	bool bCreateContentSuccess;
//	FRegistry::UGC.CreateContent(UGCCreatedChannel.Id, UGCCreateContentRequest, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](const FAccelByteModelsUGCResponse& CreateResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create Content Success"));
//		CreateContentResponse = CreateResponse;
//		bCreateContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bCreateContentSuccess, "Waiting to Create Content...");
//	AB_TEST_TRUE(bCreateContentSuccess);
//	AB_TEST_TRUE(!CreateContentResponse.Id.IsEmpty());
//
//	FAccelByteModelsUGCResponse ModifyContentResponse;
//	bool bModifyContentSuccess;
//	FRegistry::UGC.ModifyContent(UGCCreatedChannel.Id, CreateContentResponse.Id, UGCModifyContentRequest.Name, UGCModifyContentRequest.Type,
//		UGCModifyContentRequest.SubType, UGCModifyContentRequest.Tags, UGCPreviewBytes, UGCModifyContentRequest.FileExtension,
//		THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](FAccelByteModelsUGCResponse CreateResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create Content Success"));
//		ModifyContentResponse = CreateResponse;
//		bModifyContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bModifyContentSuccess, "Waiting to Create Content...");
//	AB_TEST_TRUE(bModifyContentSuccess);
//
//	FAccelByteModelsUGCContentResponse GetContentResponse;
//	bool bGetContentSuccess; 
//	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id, THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&](const FAccelByteModelsUGCContentResponse& GetResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Content Success"));
//		GetContentResponse = GetResponse;
//		bGetContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bGetContentSuccess, "Waiting to Get Content...");
//	AB_TEST_TRUE(bGetContentSuccess);
//	AB_TEST_TRUE(UGCCheckContentEqual(ModifyContentResponse, GetContentResponse));
//
//	bool bDeleteContentSuccess = false;
//	FRegistry::UGC.DeleteContent(UGCCreatedChannel.Id, CreateContentResponse.Id, FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete Content Success"));
//		bDeleteContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bDeleteContentSuccess, "Waiting to Delete Content...");
//	AB_TEST_TRUE(bDeleteContentSuccess);
//
//	return true;
//}
//
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_Upload_Download_DeleteContent, "AccelByte.Tests.UGC.D.Create_Upload_Download_DeleteContent", AutomationFlagMaskUGC)
//bool UGCCreate_Upload_Download_DeleteContent::RunTest(const FString& Parameters)
//{
//	FAccelByteModelsUGCResponse CreateContentResponse;
//	bool bCreateContentSuccess;
//	FRegistry::UGC.CreateContent(UGCCreatedChannel.Id, UGCCreateContentRequest, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](const FAccelByteModelsUGCResponse& CreateResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create Content Success"));
//		CreateContentResponse = CreateResponse;
//		bCreateContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bCreateContentSuccess, "Waiting to Create Content...");
//	AB_TEST_TRUE(bCreateContentSuccess);
//	AB_TEST_TRUE(!CreateContentResponse.Id.IsEmpty());
//
//	bool bUploadSuccess;
//	FAccelByteNetUtilities::UploadTo(CreateContentResponse.PayloadUrl[0].Url, UGCPreviewBytes,
//		FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived )
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Upload progress : %d / %d"), BytesSent, Request->GetContentLength());
//	}),
//	FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Upload Content Success"));
//		bUploadSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bUploadSuccess, "Waiting to Upload Content...");
//	AB_TEST_TRUE(bUploadSuccess);
//	
//	FAccelByteModelsUGCContentResponse GetContentResponse;
//	bool bGetContentSuccess; 
//	FRegistry::UGC.GetContentByContentId(CreateContentResponse.Id, THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&](const FAccelByteModelsUGCContentResponse& GetResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Content Success"));
//		GetContentResponse = GetResponse;
//		bGetContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bGetContentSuccess, "Waiting to Get Content...");
//	AB_TEST_TRUE(bGetContentSuccess);
//	AB_TEST_TRUE(UGCCheckContentEqual(CreateContentResponse, GetContentResponse));
//
//	TArray<uint8> DownloadBytes;
//	bool bDownloadSuccess;
//	FAccelByteNetUtilities::DownloadFrom(GetContentResponse.PayloadUrl[0].Url, FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived )
//	{
//		auto Response = Request->GetResponse();
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download progress : %d / %d"), BytesReceived, Response->GetContentLength());
//	}), THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& RawData)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download Content Success"));
//		DownloadBytes = RawData;
//		bDownloadSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bDownloadSuccess, "Waiting to Download Content...");
//	AB_TEST_TRUE(bDownloadSuccess);
//	AB_TEST_EQUAL(DownloadBytes, UGCPreviewBytes);
//	
//	bool bDeleteContentSuccess = false;
//	FRegistry::UGC.DeleteContent(UGCCreatedChannel.Id, CreateContentResponse.Id, FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete Content Success"));
//		bDeleteContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bDeleteContentSuccess, "Waiting to Delete Content...");
//	AB_TEST_TRUE(bDeleteContentSuccess);
//
//	return true;
//}
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreate_Upload_Share_Download_DeleteContent, "AccelByte.Tests.UGC.D.Create_Upload_Share_Download_DeleteContent", AutomationFlagMaskUGC)
//bool UGCCreate_Upload_Share_Download_DeleteContent::RunTest(const FString& Parameters)
//{
//	FAccelByteModelsUGCResponse CreateContentResponse;
//	bool bCreateContentSuccess;
//	FRegistry::UGC.CreateContent(UGCCreatedChannel.Id, UGCCreateContentRequest, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](const FAccelByteModelsUGCResponse& CreateResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create Content Success"));
//		CreateContentResponse = CreateResponse;
//		bCreateContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bCreateContentSuccess, "Waiting to Create Content...");
//	AB_TEST_TRUE(bCreateContentSuccess);
//	AB_TEST_TRUE(!CreateContentResponse.Id.IsEmpty());
//
//	bool bUploadSuccess;
//	FAccelByteNetUtilities::UploadTo(CreateContentResponse.PayloadUrl[0].Url, UGCPreviewBytes,
//		FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived )
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Upload progress : %d / %d"), BytesSent, Request->GetContentLength());
//	}),
//	FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Upload Content Success"));
//		bUploadSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bUploadSuccess, "Waiting to Upload Content...");
//	AB_TEST_TRUE(bUploadSuccess);
//
//	// Registration for User2
//	TSharedPtr<FApiClient> User2Registry = FMultiRegistry::GetApiClient(TEXT("User2"));
//	const FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
//	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
//	EmailAddress.ToLowerInline();
//	FString Password = "1Password1";
//	const FString Country = "US";
//	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
//	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
//	
//	bool bRegisterSuccessful = false;
//	bool bRegisterDone = false;
//	UE_LOG(LogAccelByteUGCTest, Log, TEXT("CreateEmailAccount"));
//	User2Registry->User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("    Success"));
//		bRegisterSuccessful = true;
//		bRegisterDone = true;
//	}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
//	{
//		UE_LOG(LogAccelByteUGCTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
//		bRegisterDone = true;
//	}));
//
//	FlushHttpRequests();
//	Waiting(bRegisterDone, "Waiting for Registered...");
//
//	if(!bRegisterSuccessful) return false;
//	
//	bool bLoginSuccessful = false;
//	UE_LOG(LogAccelByteUGCTest, Log, TEXT("LoginWithUsernameAndPassword"));
//	User2Registry->User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("    Success"));
//		bLoginSuccessful = true;
//	}), UGCOnError);
//
//	FlushHttpRequests();
//	Waiting(bLoginSuccessful, "Waiting for Login...");
//	
//	FAccelByteModelsUGCContentResponse GetContentResponse;
//	bool bGetContentSuccess; 
//	User2Registry->UGC.GetContentByShareCode(CreateContentResponse.ShareCode, THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&](const FAccelByteModelsUGCContentResponse& GetResponse)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Content Success"));
//		GetContentResponse = GetResponse;
//		bGetContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bGetContentSuccess, "Waiting to Get Content...");
//	AB_TEST_TRUE(bGetContentSuccess);
//	AB_TEST_TRUE(UGCCheckContentEqual(CreateContentResponse, GetContentResponse));
//	
//	TArray<uint8> DownloadBytes;
//	bool bDownloadSuccess;
//	FAccelByteNetUtilities::DownloadFrom(GetContentResponse.PayloadUrl[0].Url, FHttpRequestProgressDelegate::CreateLambda([](const FHttpRequestPtr& Request, int32 BytesSent, int32 BytesReceived )
//	{
//		auto Response = Request->GetResponse();
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download progress : %d / %d"), BytesReceived, Response->GetContentLength());
//	}), THandler<TArray<uint8>>::CreateLambda([&](const TArray<uint8>& RawData)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Download Content Success"));
//		DownloadBytes = RawData;
//		bDownloadSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bDownloadSuccess, "Waiting to Download Content...");
//	AB_TEST_TRUE(bDownloadSuccess);
//	AB_TEST_EQUAL(DownloadBytes, UGCPreviewBytes);
//	
//	bool bDeleteContentSuccess = false;
//	FRegistry::UGC.DeleteContent(UGCCreatedChannel.Id, CreateContentResponse.Id, FVoidHandler::CreateLambda([&]()
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Delete Content Success"));
//		bDeleteContentSuccess = true;
//	}), UGCOnError);
//	FlushHttpRequests();
//	Waiting(bDeleteContentSuccess, "Waiting to Delete Content...");
//	AB_TEST_TRUE(bDeleteContentSuccess);
//
//
//	bool bDeleteUserSuccess = false;
//	DeleteUserById(User2Registry->Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bDeleteUserSuccess]()
//	{
//		bDeleteUserSuccess = true;
//	}), UGCOnError);
//	Waiting(bDeleteUserSuccess, "Waiting Delete User2...");
//
//	User2Registry->User.ForgetAllCredentials();
//
//	return true;
//}
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCCreateContentInvalidChannelId, "AccelByte.Tests.UGC.E.CreateContentInvalidChannelId", AutomationFlagMaskUGC)
//bool UGCCreateContentInvalidChannelId::RunTest(const FString& Parameters)
//{
//	bool bInvalidChannelIdFailed = false;
//	FRegistry::UGC.CreateContent(UGCInvalidChannelId, UGCCreateContentRequest, THandler<FAccelByteModelsUGCResponse>::CreateLambda([&](const FAccelByteModelsUGCResponse& Response)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Create Content Success"));
//	}), FErrorHandler::CreateLambda([&](int32 Code, const FString& Message)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("EXPECTED! Error code: %d\nError message:%s"), Code, *Message);
//		bInvalidChannelIdFailed = true;
//	}));
//	FlushHttpRequests();
//	Waiting(bInvalidChannelIdFailed, "Waiting to Failing Create Content...");
//	AB_TEST_TRUE(bInvalidChannelIdFailed);
//	
//	return true;
//}
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(UGCGetContentInvalidContentId, "AccelByte.Tests.UGC.E.GetContentInvalidContentId", AutomationFlagMaskUGC)
//bool UGCGetContentInvalidContentId::RunTest(const FString& Parameters)
//{
//	bool bInvalidContentIdFailed = false;
//	FRegistry::UGC.GetContentByContentId(UGCInvalidContentId, THandler<FAccelByteModelsUGCContentResponse>::CreateLambda([&](const FAccelByteModelsUGCContentResponse& Response)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("Get Content Success"));
//	}), FErrorHandler::CreateLambda([&](int32 Code, const FString& Message)
//	{
//		UE_LOG(LogAccelByteUGCTest, Log, TEXT("EXPECTED! Error code: %d\nError message:%s"), Code, *Message);
//		bInvalidContentIdFailed = true;
//	}));
//	FlushHttpRequests();
//	Waiting(bInvalidContentIdFailed, "Waiting to Failing Get Content...");
//	AB_TEST_TRUE(bInvalidContentIdFailed);
//	
//	return true;
//}
