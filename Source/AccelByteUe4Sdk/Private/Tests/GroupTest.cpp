// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteGroupApi.h"
#include "Core/AccelByteCredentials.h"
#include "TestUtilities.h"
#include "UserTestAdmin.h"
#include "GroupTestAdmin.h"
#include "Chaos/AABB.h"
#include "Core/AccelByteMultiRegistry.h"

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::Group;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteGroupTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteGroupTest);

constexpr int32 AutomationFlagMaskGroup =
	EAutomationTestFlags::EditorContext |
	EAutomationTestFlags::ProductFilter |
	EAutomationTestFlags::CommandletContext | 
	EAutomationTestFlags::ClientContext;

/**
 * @brief Sets generic logs, passed from err fail via code+msg.
 * @param ErrorCode Pass from err
 * @param ErrorMessage Pass from err
 */
void GroupTestErrorHandler(const int32 ErrorCode, const FString& ErrorMessage)
{
	UE_LOG(LogAccelByteGroupTest, Error, TEXT("[GroupTest] Error code: %d\nError message: %s"), ErrorCode, *ErrorMessage);
}

void GroupTestWarnHandler(const FString& WarnMessage)
{
	UE_LOG(LogAccelByteGroupTest, Warning, TEXT("[GroupTest] Warn: %s"), *WarnMessage);
}

void GroupLogHandler(const FString& WarnMessage)
{
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] Info: %s"), *WarnMessage);
}

BEGIN_DEFINE_SPEC(FGroupTestSpec, "AccelByte.Tests.Group", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	const FString GroupTestGroupName = "SDK Test Group Name";
	const FString GroupTestUpdatedGroupName = "Updated " + GroupTestGroupName;
	const FString GroupTestGroupRegion = "SDK Test Group Region";
	const FString GroupTestUpdatedAttrTestStrKey = "testStr";

	const FString GroupTestRoleResourceName = "GROUP:INVITE";
	FString GroupTestRoleId; // Set later

	TArray<FTestUser> GroupTestUsers;
	FGroupConfigResponse GroupTestInitGroupConfig;	
	FString GroupTestConfigurationCode;
	FAccelByteModelsCreateGroupRequest GroupTestCreateGroupRequest;

	/** @brief AKA CreateGroupResponse */
	FAccelByteModelsGroupInformation GroupTestGroupInfo;
END_DEFINE_SPEC(FGroupTestSpec)


#pragma region Lambda Callbacks

#pragma region Lambda Callbacks -> Admin Lambda Callbacks
/** @brief Admin: Init test config with default vals. Ok if !bIsOk (already exists?); out OutInitGroupConfigResponse */
bool AdminGroupTestInitConfig(FGroupConfigResponse& OutInitGroupConfigResponse)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] AdminGroupTestInitConfig"));

	AdminInitGroupConfig(THandler<FGroupConfigResponse>::CreateLambda(
		[&](const FGroupConfigResponse& Result)
		{
			UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] AdminInitGroupConfig Responded"));
			OutInitGroupConfigResponse = Result;
			
			bIsOk = !OutInitGroupConfigResponse.ConfigurationCode.IsEmpty();
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](int32 ErrorCode, const FString& ErrorMessage)
		{
			GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true;
		}));

	WaitUntil(bIsDone, "Waiting for Admin to init default test config ...");
	return bIsOk; // User could already be created
}

/** @brief Gets a list of configs, but limits 1 and returns the 1st FGroupConfigResponse */
bool AdminGroupTestGetFirstConfig(FGroupConfigResponse& OutFirstGroupConfig)
{
	bool bIsOk = false;
	bool bIsDone = false;
	FGetGroupConfigListRequest GetGroupConfigListRequest;
	GetGroupConfigListRequest.Limit = 1;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] AdminGroupTestGetFirstConfig"));

	AdminGetGroupConfigList(
		GetGroupConfigListRequest,
		THandler<FGetGroupConfigListResponse>::CreateLambda(
			[&](const FGetGroupConfigListResponse& Result)
		{
			UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] AdminGetGroupConfigList Responded"));
			bIsOk = Result.Data.IsValidIndex(0);

			if (!bIsOk)
			{
				// Curry to general err handler
				GroupLogHandler("AdminGetGroupConfigList Result: !Result.Data.IsValidIndex(0); "
					"but this is ok because we'll init a new Group Config, instead.");
				bIsDone = true;
				return;
			}
			
			TArray<FGroupConfigResponse> FGroupConfigResArr = Result.Data;
			const FGroupConfigResponse FirstConfig = FGroupConfigResArr[0];
			OutFirstGroupConfig = FirstConfig;
				
			// ConfigurationCode is what we truly want from this.
			bIsOk = !FirstConfig.ConfigurationCode.IsEmpty();
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
		{
			GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true;
		}));
	
	WaitUntil(bIsDone, "Waiting for Admin to init default test config ...");
	return bIsOk; // User could already be created
}

/** @brief Deletes the test config we just made/used */
bool AdminGroupTestDeleteConfig(const FString& ConfigurationCode)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] AdminGroupTestDeleteConfig"));

	AdminDeleteGroupConfig(
		ConfigurationCode,
		FVoidHandler::CreateLambda([&bIsOk, &bIsDone]()
		{
			UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] AdminDeleteGroupConfig Success"));
			bIsOk = true;
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
		{
			GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true;
		}));
	
	WaitUntil(bIsDone, "Waiting for Admin to delete test config ...");
	return bIsOk;
}

/** @brief Admin: Create role of { Action<EAccelByteRoleAction>, ResourceName<FString> } */
bool AdminGroupTestCreateMemberRole(FCreateMemberRoleRequest const& RequestContent
	, FString & OutMemberRoleId)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] AdminGroupTestCreateMemberRole"));

	AdminCreateMemberRole(
		RequestContent,
		THandler<FCreateMemberRoleResponse>::CreateLambda(
			[&bIsOk, &bIsDone, &OutMemberRoleId](FCreateMemberRoleResponse Result)
		{
			UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] AdminDeleteGroupConfig Success"));
			OutMemberRoleId = Result.MemberRoleId;
				
			bIsOk = true;
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
		{
			GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true;
		}));
	
	WaitUntil(bIsDone, "Waiting for AdminDeleteGroupConfig ...");
	return bIsOk;
}
#pragma endregion Lambda Callbacks -> /Admin Lambda Callbacks

/** @brief CreateGroup with GroupLeader; out OutGroupInformationResponse */
bool GroupTestClientCreateGroup(FTestUser const& GroupLeader
	, FAccelByteModelsCreateGroupRequest const& CreateGroupRequest
	, FAccelByteModelsGroupInformation& OutGroupInformationResponse)
{
	// Sanity check
	if (CreateGroupRequest.ConfigurationCode.IsEmpty())
		return false;
	
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestClientCreateGroup with ConfigCode '%s'")
		, *CreateGroupRequest.ConfigurationCode);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(GroupLeader.Email);
	ApiClient->Group.CreateGroup(CreateGroupRequest
		, THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[&](const FAccelByteModelsGroupInformation Result)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] CreateGroup Success"));
				OutGroupInformationResponse = Result;
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, "Waiting for User to create group ...");
	return bIsOk;
}

/** @brief LoginWithUsername with TestUser */
bool GroupTestLoginApiClient(FTestUser const& TestUser)
{
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestLoginApiClient: %s"), *TestUser.Email);
	bool bIsOk = false;
	bool bIsDone = false;

	const FApiClientPtr& ApiClient = FMultiRegistry::GetApiClient(TestUser.Email); // Using email as key
	ApiClient->User.LoginWithUsername(TestUser.Email
		, TestUser.Password
		, FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] LoginWithUsername Success"));
				bIsOk = true;
				bIsDone = true;
			})
		, FCustomErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage, const FJsonObject& ErrorJson)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting "
		"for Client (%s) to login ..."), *TestUser.Email));
	return bIsOk;
}

/**
 * @brief AKA "SearchGroups"
 * @param GroupLeaderPtr 
 * @param GroupTestGroupListResponse
 * @param OutGroupTestGroupListResponse
 * @return bIsOk; out GroupTestGroupListResponse
 */
bool GroupTestGetGroupList(FString const& GroupTestGroupName
	//, const FString& GroupTestGroupRegion
	, FTestUser const& GroupLeader
	, FAccelByteModelsGroupInformation& OutGroupTestGroupListResponse)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestGetGroupList"));

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(GroupLeader.Email);
	FAccelByteModelsGetGroupListRequest GetGroupListRequest;				
	GetGroupListRequest.GroupName = GroupTestGroupName; // "SDK Test Group Name"
	GetGroupListRequest.Offset = 0;
	GetGroupListRequest.Limit = 1;
			
	FAccelByteModelsGroupInformation GroupListResponse;
	
	ApiClient->Group.GetGroupList(GetGroupListRequest
		, THandler<FAccelByteModelsGetGroupListResponse>::CreateLambda(
			[&](const FAccelByteModelsGetGroupListResponse Response)
			{
				UE_LOG(LogAccelByteGroupTest, Log, TEXT("[GroupTest] SearchGroups Responded"));
				OutGroupTestGroupListResponse = static_cast<FAccelByteModelsGroupInformation>(Response);
				
				const int32 NumGroups = Response.GroupMembers.Num();
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] SearchGroups found %d groups"), NumGroups);
				
				bIsOk = NumGroups > 0;			
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, "Waiting for SearchGroups ...");
	return bIsOk;
}

bool GroupTestGetGroupById(FTestUser const& TestUser
	, FString const& GroupId)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestGetGroupById (%s)"), *GroupId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.GetGroup(GroupId
		, THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[&](const FAccelByteModelsGroupInformation Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GetGroup (by id) Responded"));
				
				bIsOk = !Response.GroupId.IsEmpty();
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, "Waiting for GetGroup ...");
	return bIsOk;
}

/**
 *@brief Update the group name and ensure the response shows a different name.
 * - Empty strings will convert to null!
 */
bool GroupTestUpdateGroup(FTestUser const& GroupLeader
	, FString const& GroupId
	, bool bCompletelyReplace
	, FAccelByteModelsGroupUpdatable const& UpdatedGroupInfo
	, FAccelByteModelsGroupInformation& OutUpdateGroupInfo)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestUpdateGroup (GroupId %s)"), *GroupId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(GroupLeader.Email);
	// Empty strings will turn null!
	ApiClient->Group.UpdateGroup(GroupId
		, bCompletelyReplace
		, UpdatedGroupInfo
		, THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[&](const FAccelByteModelsGroupInformation Response)
			{
				UE_LOG(LogAccelByteGroupTest, Log, TEXT("[GroupTest] UpdateGroup Responded"));

				bIsOk = Response.GroupName != OutUpdateGroupInfo.GroupName;
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] UpdateGroup Success==%d"), bIsOk ? 1 : 0);
				
				OutUpdateGroupInfo = Response;
								
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, "Waiting for UpdateGroup ...");
	return bIsOk;
}

/** @brief Update GroupInformation.CustomAttributes. */
bool GroupTestUpdateCustomAttributes(FTestUser const& TestUser
	, FString const& GroupId
	, FAccelByteModelsUpdateGroupCustomAttributesRequest const& UpdatedCustomAttrRequest)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestUpdateCustomAttributes (%s)"), *GroupId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.UpdateGroupCustomAttributes(GroupId
		, UpdatedCustomAttrRequest
		, THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[&](const FAccelByteModelsGroupInformation Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestUpdateCustomAttributes Responded"));
					
				// OutGroupInfo = Response;
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for GroupTestUpdateCustomAttributes ...)")));
	return bIsOk;
}

bool GroupTestDeleteGroupById(FTestUser const& TestUser
	, FString const& GroupId)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestDeleteGroupById (%s)"), *GroupId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.DeleteGroup(GroupId
		, FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestDeleteGroupById Responded"));
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for GroupTestDeleteGroupById ...)")));
	return bIsOk;
}

bool GroupTestUpdateGroupCustomRule(FTestUser const& TestUser
	, FString const& GroupId
	, FAccelByteModelsUpdateCustomRulesRequest const& RequestContent)
{
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestUpdateGroupCustomRule (%s)"), *GroupId);
	bool bIsOk = false;
	bool bIsDone = false;

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.UpdateGroupCustomRule(GroupId
		, RequestContent
		, THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[&](const FAccelByteModelsGroupInformation Result)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestUpdateGroupCustomRule Responded"));
				
				bIsOk = !Result.GroupId.IsEmpty();
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for GroupTestUpdateGroupCustomRule ...)")));
	return bIsOk;
}

bool GroupTestUpdateGroupPredefinedRule(FTestUser const& TestUser
	, FString const& GroupId
	, EAccelByteAllowedAction const AllowedAction
	, FAccelByteModelsUpdateGroupPredefinedRuleRequest const& RequestContent)
{
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestUpdateGroupPredefinedRule (%s)"), *GroupId);
	bool bIsOk = false;
	bool bIsDone = false;

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.UpdateGroupPredefinedRule(GroupId
		, AllowedAction		
		, RequestContent
		, THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[&](const FAccelByteModelsGroupInformation Result)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestUpdateGroupPredefinedRule Responded"));
				
				bIsOk = !Result.GroupId.IsEmpty();
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for GroupTestUpdateGroupPredefinedRule ...)")));
	return bIsOk;
}

bool GroupTestDeleteGroupPredefinedRule(FTestUser const& TestUser
	, FString const& GroupId
	, EAccelByteAllowedAction const AllowedAction)
{
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestDeleteGroupPredefinedRule (%s)"), *GroupId);
	bool bIsOk = false;
	bool bIsDone = false;

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.DeleteGroupPredefinedRule(GroupId
		, AllowedAction
		, FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestDeleteGroupPredefinedRule Responded"));
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for GroupTestDeleteGroupPredefinedRule ...)")));
	return bIsOk;
}

bool GroupTestGetUserGroupInfoByUserId(FTestUser const& TestUser, FString const& UserId)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestGetUserGroupInfoByUserId (%s)"), *UserId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.GetUserGroupInfoByUserId(UserId
		, THandler<FAccelByteModelsGetUserGroupInfoResponse>::CreateLambda(
			[&](const FAccelByteModelsGetUserGroupInfoResponse& Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %GroupTestGetUserGroupInfoByUserId Responded"));
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for %GroupTestGetUserGroupInfoByUserId ...)")));
	return bIsOk;
}

bool GroupTestAcceptGroupInvitation(FTestUser const& TestUser, FString const& GroupId)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestAcceptGroupInvitation (%s)"), *GroupId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.AcceptGroupInvitation(GroupId
		, THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[&](const FAccelByteModelsMemberRequestGroupResponse& Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestAcceptGroupInvitation Responded"));
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for GroupTestAcceptGroupInvitation ...)")));
	return bIsOk;
}

bool GroupTestGetGroupInvitationRequests(FTestUser const& TestUser
	, FAccelByteModelsLimitOffsetRequest const& RequestContent)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestGetGroupInvitationRequests"));

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.GetGroupInvitationRequests(RequestContent
		, THandler<FAccelByteModelsGetMemberRequestsListResponse>::CreateLambda(
			[&](const FAccelByteModelsGetMemberRequestsListResponse& Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestGetGroupInvitationRequestss Responded"));
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for GroupTestGetGroupInvitationRequests ...)")));
	return bIsOk;
}

bool GroupTestRejectGroupInvitation(FTestUser const& TestUser, FString const& GroupId)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestRejectGroupInvitation (%s)"), *GroupId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.RejectGroupInvitation(GroupId
		, THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[&](const FAccelByteModelsMemberRequestGroupResponse& Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestRejectGroupInvitation Responded"));
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for GroupTestRejectGroupInvitation ...)")));
	return bIsOk;
}

bool GroupTestLeaveGroup(FTestUser const& TestUser)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestLeaveGroup"));

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.LeaveGroup(THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[&](const FAccelByteModelsMemberRequestGroupResponse& Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestLeaveGroup Responded"));
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for GroupTestLeaveGroup ...)")));
	return bIsOk;
}

bool GroupTestInviteUserToGroup(FTestUser const& TestUser
	, FString const& UserId)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestInviteUserToGroup (%s)"), *UserId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.InviteUserToGroup(UserId
		, THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[&](const FAccelByteModelsMemberRequestGroupResponse& Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestInviteUserToGroup Responded"));
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for GroupTestInviteUserToGroup ...)")));
	return bIsOk;
}

bool GroupTestAcceptGroupJoinRequest(FTestUser const& TestUser, FString const& UserId)
{
	const FString FuncName = "GroupTestAcceptGroupJoinRequest";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *UserId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.AcceptGroupJoinRequest(UserId
		, THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[&](const FAccelByteModelsMemberRequestGroupResponse& Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s Responded"), *FuncName);
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for %s ...)"), *FuncName));
	return bIsOk;
}

bool GroupTestRejectGroupJoinRequest(FTestUser const& TestUser, FString const& UserId)
{
	const FString FuncName = "GroupTestRejectGroupJoinRequest";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *UserId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.RejectGroupJoinRequest(UserId
		, THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[&](const FAccelByteModelsMemberRequestGroupResponse& Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s Responded"), *FuncName);
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for %s ...)"), *FuncName));
	return bIsOk;
}

bool GroupTestGetJoinRequestsRequest(FTestUser const& TestUser
	, FString const& GroupId
	, FAccelByteModelsLimitOffsetRequest const& RequestContent)
{
	const FString FuncName = "GroupTestGetJoinRequestsRequest";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *GroupId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.GetGroupJoinRequests(GroupId
		, RequestContent
		, THandler<FAccelByteModelsGetMemberRequestsListResponse>::CreateLambda(
			[&](const FAccelByteModelsGetMemberRequestsListResponse& Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s Responded"), *FuncName);
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for %s ...)"), *FuncName));
	return bIsOk;
}

bool GroupTestCancelJoinGroupRequest(FTestUser const& TestUser, FString const& UserId)
{
	const FString FuncName = "GroupTestCancelJoinGroupRequest";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *UserId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.CancelJoinGroupRequest(
		UserId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[&](const FAccelByteModelsMemberRequestGroupResponse& Response)
		{
			UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s Responded"), *FuncName);
			
			bIsOk = true;
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
		{
			GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true;
		}));
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for %s ...)"), *FuncName));
	return bIsOk;
}

bool GroupTestGetGroupMembersListByGroupId(FTestUser const& TestUser
	, FString const& GroupId
	, FAccelByteModelsGetGroupMembersListByGroupIdRequest const& RequestContent)
{
	const FString FuncName = "GroupTestGetGroupMembersListByGroupId";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *GroupId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.GetGroupMembersListByGroupId(
		GroupId,
		RequestContent,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[&](const FAccelByteModelsMemberRequestGroupResponse& Response)
		{
			UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s Responded"), *FuncName);
			
			bIsOk = true;
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
		{
			GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true;
		}));
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for %s ...)"), *FuncName));
	return bIsOk;
}

bool GroupTestJoinGroup(FTestUser const& TestUser, FString const& GroupId)
{
	const FString FuncName = "GroupTestJoinGroup";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *GroupId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.JoinGroup(
		GroupId,
		THandler<FAccelByteModelsJoinGroupResponse>::CreateLambda(
			[&](const FAccelByteModelsJoinGroupResponse& Response)
		{
			UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s Responded"), *FuncName);
			
			bIsOk = true;
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
		{
			GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true;
		}));
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for %s ...)"), *FuncName));
	return bIsOk;
}

bool GroupTestKickGroupMember(FTestUser const& TestUser, FString const& UserId)
{
	const FString FuncName = "GroupTestKickGroupMember";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *UserId);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.KickGroupMember(
		UserId,
		THandler<FAccelByteModelsKickGroupMemberResponse>::CreateLambda(
			[&](const FAccelByteModelsKickGroupMemberResponse& Response)
		{
			UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s Responded"), *FuncName);
			
			bIsOk = true;
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
		{
			GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true;
		}));
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for %s ...)"), *FuncName));
	return bIsOk;
}

bool GroupTestGetMemberRoles(FTestUser const& TestUser
	, FAccelByteModelsLimitOffsetRequest const& RequestContent)
{
	const FString FuncName = "GroupTestGetMemberRoles";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s"), *FuncName);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.GetMemberRoles(RequestContent
		, THandler<FAccelByteModelsGetMemberRolesListResponse>::CreateLambda(
			[&](const FAccelByteModelsGetMemberRolesListResponse& Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s Responded"), *FuncName);
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for %s ...)"), *FuncName));
	return bIsOk;
}

/** @brief Must assign an existing role, created via AdminCreateMemberRole */
bool GroupTestAssignMemberRole(FTestUser const& TestUser
	, FString const& MemberRoleId
	, FAccelByteModelsUserIdWrapper const& RequestContent)
{
	const FString FuncName = "GroupTestAssignMemberRole";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s"), *FuncName);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.AssignMemberRole(MemberRoleId
		, RequestContent
		, THandler<FAccelByteModelsGetUserGroupInfoResponse>::CreateLambda(
			[&](const FAccelByteModelsGetUserGroupInfoResponse& Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s Responded"), *FuncName);
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for %s ...)"), *FuncName));
	return bIsOk;
}

/** @brief Must revoke an existing role, created via AdminCreateMemberRole */
bool GroupTestDeleteMemberRole(FTestUser const& TestUser
	, FString const& MemberRoleId
	, FAccelByteModelsUserIdWrapper const& RequestContent)
{
	const FString FuncName = "GroupTestDeleteMemberRole";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s"), *FuncName);

	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	ApiClient->Group.DeleteMemberRole(MemberRoleId
		, RequestContent
		, THandler<FAccelByteModelsGetUserGroupInfoResponse>::CreateLambda(
			[&](const FAccelByteModelsGetUserGroupInfoResponse& Response)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s Responded"), *FuncName);
				
				bIsOk = true;
				bIsDone = true;
			})
		, FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			})
		);
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for %s ...)"), *FuncName));
	return bIsOk;
}

#pragma endregion /Lambda Callbacks


#pragma region Utils
TArray<FAccelByteModelsRuleInformation> GetMockRuleInfoArr(const float RuleValue)
{
	TArray<FAccelByteModelsRuleInformation> RuleInfoDetailArr;
	
	FAccelByteModelsRuleInformation RuleInfoDetail0;
	{
		RuleInfoDetail0.RuleAttribute = "TestRuleAttribute";
		RuleInfoDetail0.RuleCriteria = EAccelByteRuleCriteria::MINIMUM;
		RuleInfoDetail0.RuleValue = RuleValue; // eg, 1	
	}
	
	RuleInfoDetailArr.Emplace(RuleInfoDetail0);
	return RuleInfoDetailArr;
}

/**
 * @brief Nested deep, this sets (and returns) test GroupRules,
 * preserving hierarchy for clarity, and returns the new FGroupRules obj.
 * @return FGroupRules GroupRules
 */
FAccelByteModelsGroupRules GetMockGroupRules()
{
	// ################################################## 
	//	-GroupRules:FGroupRules
	//		-GroupPredefinedRules:TArray<FRules>
	//			-Rule0:FRule
	//				-AllowedAction:EAllowedAction
	//				-RuleDetail:TArray<FRuleInformation>
	//					-FRuleInformation
	//						-RuleAttribute:FString
	//						-RuleValue:float
	//						-RuleCriteria:ERuleCriteria
	// ##################################################
	FAccelByteModelsGroupRules GroupRules;
	{
		// GroupRules.GroupCustomRule = {}; // Optional TODO
		TArray<FAccelByteModelsRules> GroupPredefinedRulesArr;
		{
			FAccelByteModelsRules PredefinedRule0;
			{
				// For CreateGroup request: Only joinGroup || inviteUser are accepted.
				PredefinedRule0.AllowedAction = EAccelByteAllowedAction::joinGroup;
				const TArray<FAccelByteModelsRuleInformation> RuleInfoDetailArr = GetMockRuleInfoArr(1);
				PredefinedRule0.RuleDetail = RuleInfoDetailArr;
			}
			GroupPredefinedRulesArr.Emplace(PredefinedRule0);
		}
		GroupRules.GroupPredefinedRules = GroupPredefinedRulesArr;
	}
	
	return GroupRules;
}

void SetMockGroupReqData(const FString& GroupName
	, const FString& GroupRegion
	, const FString& GroupConfigCode
	, const EAccelByteGroupType GroupType
	, FAccelByteModelsCreateGroupRequest& OutCreateGroupRequest)
{
	OutCreateGroupRequest.GroupDescription = "SDK Test Create Group Description";
	OutCreateGroupRequest.GroupIcon = "https://via.placeholder.com/256x256.jpg";
	OutCreateGroupRequest.GroupName = GroupName; // "SDK Test Group Name"
	OutCreateGroupRequest.GroupRegion = GroupRegion; // "SDK Test Group Region"
	OutCreateGroupRequest.GroupRules = GetMockGroupRules();
	OutCreateGroupRequest.ConfigurationCode = GroupConfigCode;
	OutCreateGroupRequest.GroupMaxMember = 4; // TNumericLimits<int32>().Max();
	OutCreateGroupRequest.GroupType = GroupType; // EAccelByteGroupType::OPEN;

	/*
	 * JsonCustomAttribute:
	 * {
	 *	  "testBool" : true
	 * }
	 */	 
	FJsonObjectWrapper JsonCustomAttribute;
	JsonCustomAttribute.JsonObject = MakeShared<FJsonObject>();
	JsonCustomAttribute.JsonObject->SetBoolField("testBool", true);
	
	OutCreateGroupRequest.CustomAttributes = JsonCustomAttribute;
}

bool InitSetupConfig(FGroupConfigResponse& OutGroupTestInitGroupConfig
	, FString& OutGroupTestConfigurationCode)
{
	// !ConfigurationCode, likely because there was already an existing Config. Instead, use Admin to GET the 1st config.
	bool bGotConfig = AdminGroupTestGetFirstConfig(OutGroupTestInitGroupConfig);
	if (!bGotConfig || OutGroupTestInitGroupConfig.ConfigurationCode.IsEmpty())
	{
		// No existing Group Config - Init a new one, instead.
		bGotConfig = AdminGroupTestInitConfig(OutGroupTestInitGroupConfig);
	}

	OutGroupTestConfigurationCode = OutGroupTestInitGroupConfig.ConfigurationCode;
	GroupLogHandler(FString::Printf(TEXT("[GroupTest] ConfigCode(str)=='%s'"), *OutGroupTestConfigurationCode));
	
	return bGotConfig;
}

bool CallUpdateCustomAttributes(const TArray<FTestUser>& GroupTestUsers
	, const FString& GroupTestUpdatedAttrTestStrKey
	, const FString& GroupId)
{
	/*
	 *  "body":
	 *  {
	 *	    "testStr" : "New test string"
	 *  }
	 */				
	FJsonObjectWrapper UpdatedCustomAttributes;
	UpdatedCustomAttributes.JsonObject = MakeShared<FJsonObject>();
	UpdatedCustomAttributes.JsonObject->SetStringField(GroupTestUpdatedAttrTestStrKey, "updated test val");
				
	FAccelByteModelsUpdateGroupCustomAttributesRequest UpdateCustomAttrRequest;
	UpdateCustomAttrRequest.CustomAttributes = UpdatedCustomAttributes;
								
	const bool bIsDone = GroupTestUpdateCustomAttributes(
		GroupTestUsers[0],
		GroupId,
		UpdateCustomAttrRequest);

	return bIsDone;
}

bool InitTeardown(TArray<FTestUser>& GroupTestUsers
	, const FString& GroupTestConfigurationCode)
{
	// 1: Should teardown test users
	// Teardown: Always delete test users after it is done
	if (GroupTestUsers.Num() < 1)
		return true; // Already toredown
		
	const bool bTeardownUsersSuccess = TeardownTestUsers(GroupTestUsers);
	if (!bTeardownUsersSuccess)
		return false; // Err

	// 2: Should teardown test config
	// Teardown: Also delete the test config
	if (GroupTestConfigurationCode.IsEmpty())
		return true; // Already toredown
		
	const bool bTeardownConfigSuccess = AdminGroupTestDeleteConfig(GroupTestConfigurationCode);
	if (!bTeardownConfigSuccess)
		return false; // Err

	// Successful teardown
	return true;
}

bool GroupTestPreInitCreateGroup(FTestUser const& GroupLeader
	, FString const& GroupName
	, FString const& GroupRegion
	, EAccelByteGroupType GroupType
	, FString const& ConfigCode
	, FAccelByteModelsCreateGroupRequest& OutGroupCreateGroupReq
	, FAccelByteModelsGroupInformation& OutGroupInfoRes)
{ 
	// 1: Should create a new group request obj. OPEN group type.
	SetMockGroupReqData(GroupName
		, GroupRegion
		, ConfigCode
		, GroupType
		, OutGroupCreateGroupReq);
	
	if (ConfigCode.IsEmpty())
		return false; // We can only CreateGroup with a ConfigCode

	// -----------------
	// 2: Should POST CreateGroup with TestUser[0]
	// GroupTestUsers[0] is creating new group as leader
	const bool bCreatedGroup = GroupTestClientCreateGroup(GroupLeader
		, OutGroupCreateGroupReq
		, OutGroupInfoRes);
 
	return bCreatedGroup;
}

bool GroupTestPreInitAll(int32 const NumTestUsers
	, bool const bCreateGroupNow
	, FString const& GroupTestGroupName
	, FString const& GroupTestGroupRegion
	, EAccelByteGroupType const GroupType
	, TArray<FTestUser>& OutGroupTestUsers
	, FGroupConfigResponse& OutGroupTestInitGroupConfig
	, FString& OutGroupTestConfigurationCode
	, FAccelByteModelsCreateGroupRequest& OutGroupTestCreateGroupRequest
	, FAccelByteModelsGroupInformation& OutGroupTestGroupInfo)
{
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("\n-------------\n"
		"[GroupTest] @ GroupTestPreInitAll (%d test users)"), NumTestUsers);

	// --------------------
	// 1: Should GET 1st Group Config (or POST init a new Config) for ConfigurationCode"
	const bool bGotConfig = InitSetupConfig(OutGroupTestInitGroupConfig,OutGroupTestConfigurationCode);	
	if (!bGotConfig)
		return false;

	// --------------------
	// 2: Should create test users to manage groups
	const bool bSetupUsers = SetupTestUsers(NumTestUsers, OutGroupTestUsers);
	const bool bIsValidTestUsers =
		bSetupUsers &&
		OutGroupTestUsers.Num() == NumTestUsers;
	
	if (!bIsValidTestUsers)
		return false;
	if (!bCreateGroupNow || GroupType == EAccelByteGroupType::NONE)
	{
		// This is going to repeat, so make it clear this is the end of init
		UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] /END InitSetupTestUsers\n"));
		return bIsValidTestUsers;
	}

	// --------------------
	// 3: Create group
	const bool bCreatedGroup = GroupTestPreInitCreateGroup(OutGroupTestUsers[0]
		, GroupTestGroupName
		, GroupTestGroupRegion
		, GroupType
		, OutGroupTestConfigurationCode
		, OutGroupTestCreateGroupRequest
		, OutGroupTestGroupInfo);

	// This is going to repeat, so make it clear this is the end of init
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] /END InitCreateGroup\n"));
	return bCreatedGroup;
}

/**
 * @brief Creates a mock CreateRole request obj with max permissions (DeleteUpdateReadCreate=15)
 * @param RoleName 
 * @param ResourceName 
 * @return 
 */
FCreateMemberRoleRequest GroupTestCreateRoleAllPerms(FString const& RoleName, FString const& ResourceName)
{
	FCreateMemberRoleRequest CreateRoleRequest;
	CreateRoleRequest.MemberRoleName = RoleName;

	FRolePermission RolePermission;
	RolePermission.SetAction(EAccelByteRoleAction::DeleteUpdateReadCreate); // All Permissions
	RolePermission.ResourceName = ResourceName;
				
	CreateRoleRequest.MemberRolePermissions.Emplace(RolePermission);

	return CreateRoleRequest;
}

 /**
 * @brief TestUser[0] assigns [1] a role by RoleId.
 * - Results in TestUser[0] & [1] being in the same group with test role assigned to [1].
 * 
 * 1. TestUser[1] will join [0]'s OPEN group
 * 2. Admin: Create an arbitrary role
 * 3. TestUser[0] assigns the recently-made group role to [1]
 * 
 * @param TestUsers 
 * @param ApiUserPtrArr 
 * @param GroupId 
 * @param RoleName 
 * @param RoleResourceName
 * @param OutRoleId 
 * @return IsSuccess: TestUser[1] was assigned a group role (by TestUser[0])
 */
bool GroupTestMockAssignRole(TArray<FTestUser> const& TestUsers
	, FString const& GroupId
	, FString const& RoleName
	, FString const& RoleResourceName
	, FString& OutRoleId)
{
	// TestUser[1] will join [0]'s OPEN group
	const bool bSentJoinRequest = GroupTestJoinGroup(TestUsers[1]
		, GroupId);

	if (!bSentJoinRequest)
		return false;

	// -------------
	// Then Admin: Create an arbitrary role -> Get the RoleId
	const FCreateMemberRoleRequest CreateRoleRequest = GroupTestCreateRoleAllPerms(RoleName
		, RoleResourceName);
				
	const bool bCreatedRole = AdminGroupTestCreateMemberRole(CreateRoleRequest, OutRoleId);
	if (!bCreatedRole || OutRoleId.IsEmpty())
		return false;
		
	// -------------
	// Then TestUser[0] assigns the recently-made group role to [1]
	FAccelByteModelsUserIdWrapper RequestContent;
	RequestContent.UserId = TestUsers[1].UserId;
		
	const bool bIsSuccess = GroupTestAssignMemberRole(TestUsers[0]
		, OutRoleId
		, RequestContent);

	return bIsSuccess;
}
#pragma endregion /Utils


void FGroupTestSpec::Define()
{	
	Describe("GroupMgmt.A: Always create admin config + 2 test users + [0] creates OPEN group", [this]()
	{
		LatentBeforeEach([this](const FDoneDelegate& Done)
		{
			constexpr int32 NumTestUsers = 2;
			constexpr bool bCreateGroupNow = true;
			constexpr EAccelByteGroupType GroupType = EAccelByteGroupType::OPEN;
			
			const bool bIsSuccess = GroupTestPreInitAll(NumTestUsers
				, bCreateGroupNow
				, GroupTestGroupName
				, GroupTestGroupRegion
				, GroupType
				, GroupTestUsers
				, GroupTestInitGroupConfig
				, GroupTestConfigurationCode
				, GroupTestCreateGroupRequest
				, GroupTestGroupInfo);

			Done.Execute();
			return bIsSuccess;
		});
		// ===================================================================
		
		LatentIt("01: Should GET::SearchGroups for list of groups", [this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = GroupTestGetGroupList(GroupTestGroupName
				//, GroupTestGroupRegion,
				, GroupTestUsers[0]
				, GroupTestGroupInfo);				
			
			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("02: Should GET::GetGroup by GroupId", [this](const FDoneDelegate& Done)
		{			
			const bool bIsSuccess = GroupTestGetGroupById(GroupTestUsers[0]
				, GroupTestGroupInfo.GroupId);
					
			Done.Execute();
			return bIsSuccess;
		});
		
		LatentIt("03: Should PUT::UpdateGroup:GroupName", [this](const FDoneDelegate& Done)
		{
			FAccelByteModelsGroupUpdatable UpdateGroupRequest; // Available 				
			UpdateGroupRequest.GroupName = GroupTestUpdatedGroupName; // "Updated SDK Test Group Name"
			constexpr bool bCompletelyReplace = false;
			
			const bool bIsSuccess = GroupTestUpdateGroup(GroupTestUsers[0]
				, GroupTestGroupInfo.GroupId
				, bCompletelyReplace
				, UpdateGroupRequest
				, GroupTestGroupInfo);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("04: Should PUT::GroupTestUpdateCustomAttributes:CustomAttributes with a new string",
			[this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = CallUpdateCustomAttributes(GroupTestUsers
				, GroupTestUpdatedAttrTestStrKey
				, GroupTestGroupInfo.GroupId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("05: Should DELETE::DeleteGroup; returns void", [this](const FDoneDelegate& Done)
		{
			// This is how Group Admin's (group leaders) leave/disband their group
			const bool bIsSuccess = GroupTestDeleteGroupById(GroupTestUsers[0]
				, GroupTestGroupInfo.GroupId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("06: Should PUT::UpdateGroupCustomRule with new arbitrary bool",
			[this](const FDoneDelegate& Done)
		{
			FJsonObjectWrapper ArbitraryJson;
				ArbitraryJson.JsonObject = MakeShared<FJsonObject>();
				ArbitraryJson.JsonObject->SetBoolField("isTestCustomGroupRule", true);

			FAccelByteModelsUpdateCustomRulesRequest RequestContent;
			RequestContent.GroupCustomRule = ArbitraryJson;
	
			const bool bIsSuccess = GroupTestUpdateGroupCustomRule(GroupTestUsers[0]
				, GroupTestGroupInfo.GroupId
				, RequestContent);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("07: Should PUT::UpdateGroupPredefinedRule with new arbitrary bool",
			[this](const FDoneDelegate& Done)
		{
			FAccelByteModelsUpdateGroupPredefinedRuleRequest RequestContent;
			RequestContent.RuleDetail = GetMockRuleInfoArr(2);
			
			const bool bIsSuccess = GroupTestUpdateGroupPredefinedRule(GroupTestUsers[0]
				, GroupTestGroupInfo.GroupId
				, EAccelByteAllowedAction::createGroup
				, RequestContent);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("08: Should DELETE::DeleteGroupPredefinedRule; returns void",
			[this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = GroupTestDeleteGroupPredefinedRule(GroupTestUsers[0]
				, GroupTestGroupInfo.GroupId
				, EAccelByteAllowedAction::createGroup);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("09: Should POST::AcceptGroupInvitation; returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[0] invites [1] to group
			const bool bUser0Invited1 = GroupTestInviteUserToGroup(GroupTestUsers[0]
				, GroupTestUsers[1].UserId);

			if (!bUser0Invited1)
			{
				Done.Execute();
				return false;
			}

			// -----------
			// TestUser[1] will accept [0]'s invitation to group up >>
			const bool bIsSuccess = GroupTestAcceptGroupInvitation(
				GroupTestUsers[1],
				GroupTestGroupInfo.GroupId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("10: Should POST::RejectGroupInvitation (after TestUser[1] invites [0]); returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[0] invites [1] to their OPEN group
			const bool bTestUser0Invited1 = GroupTestInviteUserToGroup(GroupTestUsers[0]
				, GroupTestUsers[1].UserId);

			if (!bTestUser0Invited1)
			{
				Done.Execute();
				return false;
			}

			// ------------------
			// TestUser[1] will reject TestUser[0]'s invitation to group up >>
			const bool bIsSuccess = GroupTestRejectGroupInvitation(GroupTestUsers[1]
				, GroupTestGroupInfo.GroupId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("11: Should POST::JoinGroup (OPEN); returns group+user ids & status",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will join [0]'s OPEN group (OPEN groups don't require a request to join)
			const bool bIsSuccess = GroupTestJoinGroup(GroupTestUsers[1]
				, GroupTestGroupInfo.GroupId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("12: Should POST::LeaveGroup (after [1] joins [0]'s group); returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will join [0]'s OPEN group.
			// Admins (leaders) can't LeaveGroup(): If a Group Admin like [0] wants to leave their group, use DeleteGroup
			const bool bJoinedGroup = GroupTestJoinGroup(GroupTestUsers[1]
				, GroupTestGroupInfo.GroupId);

			if (!bJoinedGroup)
			{
				Done.Execute();
				return false;
			}

			// -----------------
			// TestUser[1] will leave [0]'s group >>
			const bool bIsSuccess = GroupTestLeaveGroup(GroupTestUsers[1]);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("13: Should GET::GetUserGroupInfoByUserId, "
			"where TestUser[0] gets [0]'s own group info; returns group info",
			[this](const FDoneDelegate& Done)
		{				
			bool bIsSuccess = GroupTestGetUserGroupInfoByUserId(GroupTestUsers[0]
				, GroupTestGroupInfo.GroupMembers[0].UserId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("14: Should POST::InviteUserToGroup, where TestUser[0] invites [1]; returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = GroupTestInviteUserToGroup(GroupTestUsers[0]
				, GroupTestUsers[1].UserId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("15: Should GET::GetGroupMembersListByGroupId, "
			"where TestUser[0] invites [1]; returns paginated response",
			[this](const FDoneDelegate& Done)
		{
			// Defaults are ok - just paginated options 
			const FAccelByteModelsGetGroupMembersListByGroupIdRequest GetGroupMembersListByGroupIdRequest;
				
			const bool bIsSuccess = GroupTestGetGroupMembersListByGroupId(GroupTestUsers[0]
				, GroupTestGroupInfo.GroupId
				, GetGroupMembersListByGroupIdRequest);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("16: Should POST::KickGroupMember; returns group+userKicked ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will join [0]'s OPEN group (won't required a request to join)
			const bool bJoinedGroup = GroupTestJoinGroup(GroupTestUsers[1]
				, GroupTestGroupInfo.GroupId);

			if (!bJoinedGroup)
			{
				Done.Execute();
				return false;
			}

			// ---------------
			// TestUser[1] will kick [0] out of the group
			const bool bIsSuccess = GroupTestKickGroupMember(GroupTestUsers[0]
				, GroupTestUsers[1].UserId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("17: Should GET::GetMemberRoles (from group member); returns paginated id, name, perms",
			[this](const FDoneDelegate& Done)
		{
			constexpr FAccelByteModelsLimitOffsetRequest RequestContent; // { Limit=1, Offset=0 }
				
			const bool bIsSuccess = GroupTestGetMemberRoles(GroupTestUsers[0]
				, RequestContent);
					
			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("18: Should POST::AssignRoleToMember, where TestUser[1] (in an OPEN group) "
			"joins [0]'s group - then [0] assigns role to [1]; returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will join [0]'s OPEN group
			// Then Admin: Create an arbitrary role
			// Then TestUser[0] assigns the recently-made group role to [1]
			const bool bIsSuccess =  GroupTestMockAssignRole(GroupTestUsers
				, GroupTestGroupInfo.GroupId
				, GroupTestRoleResourceName
				, GroupTestRoleResourceName
				, GroupTestRoleId); // out
				
			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("19: Should DELETE::RemoveRoleFromMember, where TestUser[1] (in an OPEN group) "
			"joins [0]'s group - then [0] assigns role to [1] - then [0] removes [1]'s new role; "
			"returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will join [0]'s OPEN group
			// Then Admin: Create an arbitrary role
			// Then TestUser[0] assigns the recently-made group role to [1]
			const bool bUser1WasAssignedRoleBy0 =  GroupTestMockAssignRole(GroupTestUsers
				, GroupTestGroupInfo.GroupId
				, GroupTestRoleResourceName
				, GroupTestRoleResourceName
				, GroupTestRoleId); // out

			if (!bUser1WasAssignedRoleBy0)
			{
				Done.Execute();
				return false;
			}

			// -------------
			// Then TestUser[0] removes [1]'s new role
			FAccelByteModelsUserIdWrapper UserIdWrapper;
			UserIdWrapper.UserId = GroupTestUsers[1].UserId;
				
			const bool bIsSuccess = GroupTestDeleteMemberRole(GroupTestUsers[0]
            	, GroupTestRoleId
            	, UserIdWrapper);
				
			Done.Execute();
			return bIsSuccess;
		});
		
		LatentIt("20: Should POST::GetGroupInvitationRequests; where TestUser[0] invites [1], "
			"then [1] gets their own list of invites; returns a list of invites, containing the one from [0]",
        	[this](const FDoneDelegate& Done)
        {
        	// TestUser[0] invites [1] to group
        	const bool bUser0Invited1 = GroupTestInviteUserToGroup(GroupTestUsers[0]
        		, GroupTestUsers[1].UserId);

        	if (!bUser0Invited1)
        	{
        		Done.Execute();
        		return false;
        	}

        	// -----------
        	// TestUser[1] will get their own list of invites (which should contain [0]s invite)
        	constexpr FAccelByteModelsLimitOffsetRequest RequestContent;     		
        	const bool bIsSuccess = GroupTestGetGroupInvitationRequests(GroupTestUsers[1]
        		, RequestContent);

        	Done.Execute();
        	return bIsSuccess;
        });

		// ===================================================================		
		LatentAfterEach([this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = InitTeardown(GroupTestUsers
				, GroupTestConfigurationCode);

			Done.Execute();
			return bIsSuccess;
		});
	});

		
	Describe("GroupMgmt.B: Always create admin config + 2 test users + [0] creates PUBLIC group", [this]()
	{
		LatentBeforeEach([this](const FDoneDelegate& Done)
		{
			constexpr int32 NumTestUsers = 2;
			constexpr bool bCreateGroupNow = true;
			constexpr EAccelByteGroupType GroupType = EAccelByteGroupType::PUBLIC;
			
			const bool bIsSuccess = GroupTestPreInitAll(NumTestUsers
				, bCreateGroupNow
				, GroupTestGroupName
				, GroupTestGroupRegion
				, GroupType
				, GroupTestUsers
				, GroupTestInitGroupConfig
				, GroupTestConfigurationCode
				, GroupTestCreateGroupRequest
				, GroupTestGroupInfo);

			Done.Execute();
			return bIsSuccess;
		});
		// ===================================================================
		
		LatentIt("01: Should POST::CancelJoinGroupRequest, where TestUser[1] cancels "
			"PUBLIC join group req to [1]'s PUBLIC group; returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will send a REQUEST to join [0]'s PUBLIC group
			const bool bSentJoinRequest = GroupTestJoinGroup(GroupTestUsers[1]
				, GroupTestGroupInfo.GroupId);

			if (!bSentJoinRequest)
			{
				Done.Execute();
				return false;
			}

			// -------------
			// TestUser[1] no longer wants to join, so cancels their join request to [0]'s PUBLIC group.
			const bool bIsSuccess = GroupTestCancelJoinGroupRequest(GroupTestUsers[1]
				, GroupTestGroupInfo.GroupId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("02: Should POST::AcceptGroupJoinRequest, where TestUser[1] (in a PUBLIC group) "
			"accepts join group req from [0]; returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will send a REQUEST to join [0]'s PUBLIC group
			const bool bSentJoinRequest = GroupTestJoinGroup(GroupTestUsers[1]
				, GroupTestGroupInfo.GroupId);

			if (!bSentJoinRequest)
			{
				Done.Execute();
				return false;
			}

			// -------------
			// TestUser[0] accepts [1]'s request to join [0]'s PUBLIC group.
			const bool bIsSuccess = GroupTestAcceptGroupJoinRequest(GroupTestUsers[0]
				, GroupTestUsers[1].UserId);

			Done.Execute();
			return bIsSuccess;
		});

		
		LatentIt("03: Should POST::RejectGroupJoinRequest, where TestUser[1] (in a PUBLIC group), "
			"then rejects join group req from [0]; returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will send a REQUEST to join [0]'s PUBLIC group
			const bool bSentJoinRequest = GroupTestJoinGroup(GroupTestUsers[1]
				, GroupTestGroupInfo.GroupId);

			if (!bSentJoinRequest)
			{
				Done.Execute();
				return false;
			}

			// -------------
			// TestUser[0] rejects [1]'s request to join [0]'s PUBLIC group.
			const bool bIsSuccess = GroupTestRejectGroupJoinRequest(GroupTestUsers[0]
				, GroupTestUsers[1].UserId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("04: Should POST::GetGroupJoinRequests, where TestUser[1] (in a PUBLIC group), "
			"then [0] gets join requests; returns a list of group join requests",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will send a REQUEST to join [0]'s PUBLIC group
			const bool bSentJoinRequest = GroupTestJoinGroup(GroupTestUsers[1]
				, GroupTestGroupInfo.GroupId);

			if (!bSentJoinRequest)
			{
				Done.Execute();
				return false;
			}

			// -------------
			// Then [0] gets join requests
			constexpr FAccelByteModelsLimitOffsetRequest RequestContent;
			const bool bIsSuccess = GroupTestGetJoinRequestsRequest(GroupTestUsers[0]
				, GroupTestGroupInfo.GroupId
				, RequestContent);

			Done.Execute();
			return bIsSuccess;
		});
				
		// ===================================================================
		LatentAfterEach([this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = InitTeardown(GroupTestUsers
				, GroupTestConfigurationCode);

			Done.Execute();
			return bIsSuccess;
		});
	});
	
	Describe("GroupMgmt.C: Always Create admin config + 2 test users (NO group)", [this]()
	{
		LatentBeforeEach([this](const FDoneDelegate& Done)
		{
			constexpr int32 NumTestUsers = 2;
			constexpr bool bCreateGroupNow = false;
			constexpr EAccelByteGroupType GroupType = EAccelByteGroupType::NONE;
			
			const bool bIsSuccess = GroupTestPreInitAll(NumTestUsers
				, bCreateGroupNow
				, GroupTestGroupName
				, GroupTestGroupRegion
				, GroupType
				, GroupTestUsers
				, GroupTestInitGroupConfig
				, GroupTestConfigurationCode
				, GroupTestCreateGroupRequest
				, GroupTestGroupInfo);

			Done.Execute();
			return bIsSuccess;
		});
		// ===================================================================

		LatentIt("01: Should POST::CreateGroupcreate, where TestUser[0] creates OPEN group; returns group info",
			[this](const FDoneDelegate& Done)
		{
			constexpr EAccelByteGroupType GroupType = EAccelByteGroupType::OPEN;
				
			const bool bIsSuccess = GroupTestPreInitCreateGroup(GroupTestUsers[0]
				, GroupTestGroupName
				, GroupTestGroupRegion
				, GroupType
				, GroupTestConfigurationCode
				, GroupTestCreateGroupRequest
				, GroupTestGroupInfo);
	
			Done.Execute();
			return bIsSuccess;
		});

		// ===================================================================
		LatentAfterEach([this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = InitTeardown(GroupTestUsers
				, GroupTestConfigurationCode);

			Done.Execute();
			return bIsSuccess;
		});
	});
}
