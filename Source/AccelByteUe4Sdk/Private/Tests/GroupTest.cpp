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
	const FString GroupTestRoleId; // Set later

	TArray<FTestUser> GroupTestUsers;
	TArray<FApiClientPtr> GroupTestApiUserPtrArr;
	FGroupConfigResponse GroupTestInitGroupConfig;	
	FString GroupTestConfigurationCode;
	FAccelByteModelsCreateGroupRequest GroupTestCreateGroupRequest;

	/** @brief AKA CreateGroupResponse */
	FAccelByteModelsGroupInformation GroupTestGroupInfo;
END_DEFINE_SPEC(FGroupTestSpec)


#pragma region Lambda Callbacks

#pragma region Lambda Callbacks -> Admin Lambda Callbacks
/** @brief Admin: Init test config with default vals. Ok if !bIsOk (already exists?); out OutInitGroupConfigResponse */
const auto AdminGroupTestInitConfig = [](FGroupConfigResponse& OutInitGroupConfigResponse)
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
};

/** @brief Gets a list of configs, but limits 1 and returns the 1st FGroupConfigResponse */
const auto AdminGroupTestGetFirstConfig = [](FGroupConfigResponse& OutFirstGroupConfig)
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
};

/** @brief Deletes the test config we just made/used */
const auto AdminGroupTestDeleteConfig = [](const FString& ConfigurationCode)
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
};

/** @brief Admin: Create role of { Action<EAccelByteRoleAction>, ResourceName<FString> } */
const auto AdminGroupTestCreateMemberRole = [](
	const FCreateMemberRoleRequest& RequestContent,
	FString OutMemberRoleId)
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
};
#pragma endregion Lambda Callbacks -> /Admin Lambda Callbacks

/** @brief CreateGroup with GroupLeader; out OutGroupInformationResponse */
const auto GroupTestClientCreateGroup = [](
	const FApiClientPtr& GroupLeader,
	const FAccelByteModelsCreateGroupRequest& CreateGroupRequest,
	FAccelByteModelsGroupInformation& OutGroupInformationResponse)
{
	// Sanity check
	if (!GroupLeader)
		return false;
	if (CreateGroupRequest.ConfigurationCode.IsEmpty())
		return false;
	
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestClientCreateGroup with ConfigCode '%s'"),
		*CreateGroupRequest.ConfigurationCode);

	GroupLeader->Group.CreateGroup(
		CreateGroupRequest,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[&](const FAccelByteModelsGroupInformation Result)
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] CreateGroup Success"));
				OutGroupInformationResponse = Result;
				bIsOk = true;
				bIsDone = true;
			}),
			FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			}));
	
	WaitUntil(bIsDone, "Waiting for User to create group ...");
	return bIsOk;
};

/** @brief LoginWithUsername with TestUser */
const auto GroupTestLoginApiClient = [](const FTestUser& User)
{
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestLoginApiClient: %s"), *User.Email);
	bool bIsOk = false;
	bool bIsDone = false;

	const FApiClientPtr& ApiClient = FMultiRegistry::GetApiClient(User.Email); // Using email as key
	ApiClient->User.LoginWithUsername(
		User.Email,
		User.Password,
		FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] LoginWithUsername Success"));
				bIsOk = true;
				bIsDone = true;
			}),
			FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			}));
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting "
		"for Client (%s) to login ..."), *User.Email));
	return bIsOk;
};

/**
 * @brief AKA "SearchGroups"
 * @param GroupLeaderPtr 
 * @param GroupTestGroupListResponse
 * @param GroupTestGroupListRequest
 * @return bIsOk; out GroupTestGroupListResponse
 */
const auto GroupTestGetGroupList = [](
	const FString& GroupTestGroupName,
	// const FString& GroupTestGroupRegion,
	const FApiClientPtr& GroupLeaderPtr,
	FAccelByteModelsGroupInformation& OutGroupTestGroupListResponse)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestGetGroupList"));

	FAccelByteModelsGetGroupListRequest GetGroupListRequest;				
	GetGroupListRequest.GroupName = GroupTestGroupName; // "SDK Test Group Name"
	GetGroupListRequest.Offset = 0;
	GetGroupListRequest.Limit = 1;
			
	FAccelByteModelsGroupInformation GroupListResponse;
	
	GroupLeaderPtr->Group.GetGroupList(
		GetGroupListRequest,
		THandler<FAccelByteModelsGetGroupListResponse>::CreateLambda(
			[&](const FAccelByteModelsGetGroupListResponse Response)
		{
			UE_LOG(LogAccelByteGroupTest, Log, TEXT("[GroupTest] SearchGroups Responded"));
			OutGroupTestGroupListResponse = static_cast<FAccelByteModelsGroupInformation>(Response);
			
			const int32 NumGroups = Response.GroupMembers.Num();
			UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] SearchGroups found %d groups"), NumGroups);
			
			bIsOk = NumGroups > 0;			
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
		{
			GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true;
		}));
	
	WaitUntil(bIsDone, "Waiting for SearchGroups ...");
	return bIsOk;
};

const auto GroupTestGetGroupById = [](
	const FApiClientPtr& TestUserPtr,
	const FString& GroupId)
{
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestGetGroupById (%s)"), *GroupId);

	TestUserPtr->Group.GetGroup(
		GroupId,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[&](const FAccelByteModelsGroupInformation Response)
		{
			UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GetGroup (by id) Responded"));
			
			bIsOk = !Response.GroupId.IsEmpty();
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
		{
			GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true;
		}));
	
	WaitUntil(bIsDone, "Waiting for GetGroup ...");
	return bIsOk;
};

/**
 *@brief Update the group name and ensure the response shows a different name.
 * - Empty strings will convert to null!
 */
const auto GroupTestUpdateGroup = [](
		const FApiClientPtr& GroupLeaderPtr,
		const FString& GroupId,
		const bool bCompletelyReplace,
		const FAccelByteModelsGroupUpdatable& UpdatedGroupInfo,
		FAccelByteModelsGroupInformation& OutUpdateGroupInfo)
{
		bool bIsOk = false;
		bool bIsDone = false;
		UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] GroupTestUpdateGroup (GroupId %s)"), *GroupId);

		// Empty strings will turn null!
		GroupLeaderPtr->Group.UpdateGroup(
			GroupId,
			bCompletelyReplace,
			UpdatedGroupInfo,
			THandler<FAccelByteModelsGroupInformation>::CreateLambda(
				[&](const FAccelByteModelsGroupInformation Response)
			{
				UE_LOG(LogAccelByteGroupTest, Log, TEXT("[GroupTest] UpdateGroup Responded"));

				bIsOk = Response.GroupName != OutUpdateGroupInfo.GroupName;
				UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] UpdateGroup Success==%d"), bIsOk ? 1 : 0);
				
				OutUpdateGroupInfo = Response;
								
				bIsDone = true;
			}),
			FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
				bIsDone = true;
			}));
		
		WaitUntil(bIsDone, "Waiting for UpdateGroup ...");
		return bIsOk;
};

/** @brief Update GroupInformation.CustomAttributes. */
const auto GroupTestUpdateCustomAttributes = [](
	const FApiClientPtr& TestUserPtr,
	const FString& GroupId,
	const FAccelByteModelsUpdateGroupCustomAttributesRequest& UpdatedCustomAttrRequest)
	// FAccelByteModelsGroupInformation& OutGroupInfo)
{
	const FString FuncName = "GroupTestUpdateCustomAttributes";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *GroupId);

	TestUserPtr->Group.UpdateGroupCustomAttributes(
		GroupId,
		UpdatedCustomAttrRequest,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[&](const FAccelByteModelsGroupInformation Response)
		{
			UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s Responded"),
				*FuncName);
				
			// OutGroupInfo = Response;
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
};

const auto GroupTestDeleteGroupById = [](
	const FApiClientPtr& TestUserPtr,
	const FString& GroupId)
{
	const FString FuncName = "GroupTestDeleteGroupById";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *GroupId);

	TestUserPtr->Group.DeleteGroup(
		GroupId,
		FVoidHandler::CreateLambda([&]()
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
};

const auto GroupTestUpdateGroupCustomRule = [](
	const FApiClientPtr& TestUserPtr,
	const FString& GroupId,
	const FAccelByteModelsUpdateCustomRulesRequest& RequestContent)
{
	const FString FuncName = "GroupTestUpdateGroupCustomRule";
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *GroupId);
	bool bIsOk = false;
	bool bIsDone = false;

	TestUserPtr->Group.UpdateGroupCustomRule(
		GroupId,
		RequestContent,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[&](const FAccelByteModelsGroupInformation Result)
		{
			UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s Responded"), *FuncName);
			
			bIsOk = !Result.GroupId.IsEmpty();
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
		{
			GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true;
		}));
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for %s ...)"), *FuncName));
	return bIsOk;
};

const auto GroupTestUpdateGroupPredefinedRule = [](
	const FApiClientPtr& TestUserPtr,
	const FString& GroupId,
	const EAccelByteAllowedAction& AllowedAction,
	const FAccelByteModelsUpdateGroupPredefinedRuleRequest& RequestContent)
{
	const FString FuncName = "GroupTestUpdateGroupPredefinedRule";
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *GroupId);
	bool bIsOk = false;
	bool bIsDone = false;

	TestUserPtr->Group.UpdateGroupPredefinedRule(
		GroupId,
		AllowedAction,		
		RequestContent,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[&](const FAccelByteModelsGroupInformation Result)
		{
			UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s Responded"), *FuncName);
			
			bIsOk = !Result.GroupId.IsEmpty();
			bIsDone = true;
		}),
		FErrorHandler::CreateLambda([&bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
		{
			GroupTestErrorHandler(ErrorCode, ErrorMessage); // Curry to general err handler
			bIsDone = true;
		}));
	
	WaitUntil(bIsDone, FString::Printf(TEXT("[GroupTest] Waiting for %s ...)"), *FuncName));
	return bIsOk;
};

const auto GroupTestDeleteGroupPredefinedRule = [](
	const FApiClientPtr& TestUserPtr,
	const FString& GroupId,
	const EAccelByteAllowedAction& AllowedAction)
{
	const FString FuncName = "GroupTestDeleteGroupPredefinedRule";
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *GroupId);
	bool bIsOk = false;
	bool bIsDone = false;

	TestUserPtr->Group.DeleteGroupPredefinedRule(
		GroupId,
		AllowedAction,
		FVoidHandler::CreateLambda([&]()
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
};

const auto GroupTestGetUserGroupInfoByUserId = [](const FApiClientPtr& TestUserPtr, const FString& UserId)
{
	const FString FuncName = "GroupTestGetUserGroupInfoByUserId";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *UserId);

	TestUserPtr->Group.GetUserGroupInfoByUserId(
		UserId,
		THandler<FAccelByteModelsGetUserGroupInfoResponse>::CreateLambda(
			[&](const FAccelByteModelsGetUserGroupInfoResponse& Response)
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
};

const auto GroupTestAcceptGroupInvitation = [](const FApiClientPtr& TestUserPtr, const FString& GroupId)
{
	const FString FuncName = "GroupTestAcceptGroupInvitation";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *GroupId);

	TestUserPtr->Group.AcceptGroupInvitation(
		GroupId,
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
};

const auto GroupTestGetGroupInvitationRequests = [](
	const FApiClientPtr& TestUserPtr,
	const FAccelByteModelsLimitOffsetRequest& RequestContent)
{
	const FString FuncName = "GroupTestGetGroupInvitationRequests";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s"), *FuncName);

	TestUserPtr->Group.GetGroupInvitationRequests(
		RequestContent,
		THandler<FAccelByteModelsGetMemberRequestsListResponse>::CreateLambda(
			[&](const FAccelByteModelsGetMemberRequestsListResponse& Response)
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
};

const auto GroupTestRejectGroupInvitation = [](const FApiClientPtr& TestUserPtr, const FString& GroupId)
{
	const FString FuncName = "GroupTestRejectGroupInvitation";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *GroupId);

	TestUserPtr->Group.RejectGroupInvitation(
		GroupId,
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
};

const auto GroupTestLeaveGroup = [](const FApiClientPtr& TestUserPtr)
{
	const FString FuncName = "GroupTestLeaveGroup";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s"), *FuncName);

	TestUserPtr->Group.LeaveGroup(
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
};

const auto GroupTestInviteUserToGroup = [](const FApiClientPtr& TestUserPtr, const FString& UserId)
{
	const FString FuncName = "GroupTestInviteUserToGroup";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *UserId);

	TestUserPtr->Group.InviteUserToGroup(
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
};

const auto GroupTestAcceptGroupJoinRequest = [](const FApiClientPtr& TestUserPtr, const FString& UserId)
{
	const FString FuncName = "GroupTestAcceptGroupJoinRequest";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *UserId);

	TestUserPtr->Group.AcceptGroupJoinRequest(
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
};

const auto GroupTestRejectGroupJoinRequest = [](const FApiClientPtr& TestUserPtr, const FString& UserId)
{
	const FString FuncName = "GroupTestRejectGroupJoinRequest";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *UserId);

	TestUserPtr->Group.RejectGroupJoinRequest(
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
};

const auto GroupTestGetJoinRequestsRequest = [](
	const FApiClientPtr& TestUserPtr,
	const FString& GroupId,
	const FAccelByteModelsLimitOffsetRequest RequestContent)
{
	const FString FuncName = "GroupTestGetJoinRequestsRequest";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *GroupId);

	TestUserPtr->Group.GetGroupJoinRequests(
		GroupId,
		RequestContent,
		THandler<FAccelByteModelsGetMemberRequestsListResponse>::CreateLambda(
			[&](const FAccelByteModelsGetMemberRequestsListResponse& Response)
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
};

const auto GroupTestCancelJoinGroupRequest = [](const FApiClientPtr& TestUserPtr, const FString& UserId)
{
	const FString FuncName = "GroupTestCancelJoinGroupRequest";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *UserId);

	TestUserPtr->Group.CancelJoinGroupRequest(
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
};

const auto GroupTestGetGroupMembersListByGroupId = [](
	const FApiClientPtr& TestUserPtr,
	const FString& GroupId,
	const FAccelByteModelsGetGroupMembersListByGroupIdRequest& RequestContent)
{
	const FString FuncName = "GroupTestGetGroupMembersListByGroupId";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *GroupId);

	TestUserPtr->Group.GetGroupMembersListByGroupId(
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
};

const auto GroupTestJoinGroup = [](const FApiClientPtr& TestUserPtr, const FString& GroupId)
{
	const FString FuncName = "GroupTestJoinGroup";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *GroupId);

	TestUserPtr->Group.JoinGroup(
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
};

const auto GroupTestKickGroupMember = [](const FApiClientPtr& TestUserPtr, const FString& UserId)
{
	const FString FuncName = "GroupTestKickGroupMember";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s (%s)"), *FuncName, *UserId);

	TestUserPtr->Group.KickGroupMember(
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
};

const auto GroupTestGetMemberRoles = [](
	const FApiClientPtr& TestUserPtr,
	const FAccelByteModelsLimitOffsetRequest& RequestContent)
{
	const FString FuncName = "GroupTestGetMemberRoles";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s"), *FuncName);

	TestUserPtr->Group.GetMemberRoles(
		RequestContent,
		THandler<FAccelByteModelsGetMemberRolesListResponse>::CreateLambda(
			[&](const FAccelByteModelsGetMemberRolesListResponse& Response)
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
};

/** @brief Must assign an existing role, created via AdminCreateMemberRole */
const auto GroupTestAssignMemberRole = [](
	const FApiClientPtr& TestUserPtr,
	const FString& MemberRoleId,
	const FAccelByteModelsUserIdWrapper& RequestContent)
{
	const FString FuncName = "GroupTestAssignMemberRole";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s"), *FuncName);

	TestUserPtr->Group.AssignMemberRole(
		MemberRoleId,
		RequestContent,
		THandler<FAccelByteModelsGetUserGroupInfoResponse>::CreateLambda(
			[&](const FAccelByteModelsGetUserGroupInfoResponse& Response)
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
};

/** @brief Must revoke an existing role, created via AdminCreateMemberRole */
const auto GroupTestDeleteMemberRole = [](
	const FApiClientPtr& TestUserPtr,
	const FString& MemberRoleId,
	const FAccelByteModelsUserIdWrapper& RequestContent)
{
	const FString FuncName = "GroupTestDeleteMemberRole";
	bool bIsOk = false;
	bool bIsDone = false;
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] %s"), *FuncName);

	TestUserPtr->Group.DeleteMemberRole(
		MemberRoleId,
		RequestContent,
		THandler<FAccelByteModelsGetUserGroupInfoResponse>::CreateLambda(
			[&](const FAccelByteModelsGetUserGroupInfoResponse& Response)
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
};

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

void SetMockGroupReqData(
	const FString& GroupName,
	const FString& GroupRegion,
	const FString& GroupConfigCode,
	const EAccelByteGroupType GroupType,
	FAccelByteModelsCreateGroupRequest& OutCreateGroupRequest)
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

bool InitSetupConfig(
	FGroupConfigResponse& OutGroupTestInitGroupConfig,
	FString& OutGroupTestConfigurationCode)
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

bool InitSetupTestUsers(
	const int32 NumTestUsers,
	TArray<FTestUser>& OutGroupTestUsers,
	TArray<FApiClientPtr>& OutGroupTestApiUserPtrArr)
{
	// Create, register and login test user(s) with random details to avoid test collision
	const bool bSetupUsers = SetupTestUsers(NumTestUsers, OutGroupTestUsers);

	// Get ref to the client pointers since this is used more often than raw Users
	OutGroupTestApiUserPtrArr.Reset();
	for (const FTestUser& User : OutGroupTestUsers)
	{
		OutGroupTestApiUserPtrArr.Emplace(FMultiRegistry::GetApiClient(User.Email));
	}
			
	// Multi-user login
	for (const auto& User : OutGroupTestUsers)
	{
		if (!GroupTestLoginApiClient(User))
			return false;
	}
	
	return bSetupUsers;
}

bool CallUpdateCustomAttributes(
	const FString& GroupTestUpdatedAttrTestStrKey,
	const TArray<FApiClientPtr>& GroupTestApiUserPtrArr,
	const FString& GroupId)
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
		GroupTestApiUserPtrArr[0],
		GroupId,
		UpdateCustomAttrRequest);

	return bIsDone;
}

bool InitTeardown(TArray<FTestUser>& GroupTestUsers, const FString& GroupTestConfigurationCode)
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

bool GroupTestPreInitCreateGroup(
	 const FString& GroupName,
	 const FString& GroupRegion,
	 const EAccelByteGroupType GroupType,
	 const FString& ConfigCode,
	 const FApiClientPtr& GroupTestApiUserPtrArr, // Group leader
	 FAccelByteModelsCreateGroupRequest& OutGroupCreateGroupReq,
	 FAccelByteModelsGroupInformation& OutGroupInfoRes)
{ 
	// 1: Should create a new group request obj. OPEN group type.
	SetMockGroupReqData(
		GroupName,
		GroupRegion,
		ConfigCode,
		GroupType,
		OutGroupCreateGroupReq);
	
	if (ConfigCode.IsEmpty())
		return false; // We can only CreateGroup with a ConfigCode

	// -----------------
	// 2: Should POST CreateGroup with TestUser[0]
	// GroupTestUsers[0] is creating new group as leader
	const bool bCreatedGroup = GroupTestClientCreateGroup(
		GroupTestApiUserPtrArr,
		OutGroupCreateGroupReq,
		OutGroupInfoRes);
 
	return bCreatedGroup;
}

const auto GroupTestPreInitAll = [](
	const int32 NumTestUsers,
	const bool bCreateGroupNow,
	const FString& GroupTestGroupName,
	const FString& GroupTestGroupRegion,
	const EAccelByteGroupType GroupType,
	TArray<FTestUser>& OutGroupTestUsers,
	FGroupConfigResponse& OutGroupTestInitGroupConfig,
	FString& OutGroupTestConfigurationCode,
	TArray<FApiClientPtr>& OutGroupTestApiUserPtrs,
	FAccelByteModelsCreateGroupRequest& OutGroupTestCreateGroupRequest,
	FAccelByteModelsGroupInformation& OutGroupTestGroupInfo)
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
	const bool bSetupUsers = InitSetupTestUsers(NumTestUsers, OutGroupTestUsers, OutGroupTestApiUserPtrs);
	const bool bIsValidTestUsers =
		bSetupUsers &&
		OutGroupTestUsers.Num() == NumTestUsers &&
		OutGroupTestApiUserPtrs.Num() == NumTestUsers;
	
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
	const bool bCreatedGroup = GroupTestPreInitCreateGroup(
		GroupTestGroupName,
		GroupTestGroupRegion,
		GroupType,
		OutGroupTestConfigurationCode,
		OutGroupTestApiUserPtrs[0],
		OutGroupTestCreateGroupRequest,
		OutGroupTestGroupInfo);

	// This is going to repeat, so make it clear this is the end of init
	UE_LOG(LogAccelByteGroupTest, Display, TEXT("[GroupTest] /END InitSetupTestUsers\n"));
	return bCreatedGroup;
};

/**
 * @brief Creates a mock CreateRole request obj with max permissions (DeleteUpdateReadCreate=15)
 * @param RoleName 
 * @param ResourceName 
 * @return 
 */
FCreateMemberRoleRequest GroupTestCreateRoleAllPerms(const FString& RoleName, const FString& ResourceName)
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
bool GroupTestMockAssignRole(
	const TArray<FTestUser>& TestUsers,
	const TArray<FApiClientPtr>& ApiUserPtrArr,
	const FString& GroupId,
	const FString& RoleName,
	const FString& RoleResourceName,
	const FString& OutRoleId)
{
	// TestUser[1] will join [0]'s OPEN group
	const bool bSentJoinRequest = GroupTestJoinGroup(
		ApiUserPtrArr[1],
		GroupId);

	if (!bSentJoinRequest)
		return false;

	// -------------
	// Then Admin: Create an arbitrary role -> Get the RoleId
	const FCreateMemberRoleRequest CreateRoleRequest = GroupTestCreateRoleAllPerms(
		RoleName,
		RoleResourceName);
				
	const bool bCreatedRole = AdminGroupTestCreateMemberRole(CreateRoleRequest, OutRoleId);
	if (!bCreatedRole || OutRoleId.IsEmpty())
		return false;
		
	// -------------
	// Then TestUser[0] assigns the recently-made group role to [1]
	FAccelByteModelsUserIdWrapper RequestContent;
	RequestContent.UserId = TestUsers[1].Credentials.GetUserId();
		
	const bool bIsSuccess = GroupTestAssignMemberRole(
		ApiUserPtrArr[0],
		RoleName,
		RequestContent);

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
			
			const bool bIsSuccess = GroupTestPreInitAll(
				NumTestUsers,
				bCreateGroupNow,
				GroupTestGroupName,
				GroupTestGroupRegion,
				GroupType,
				GroupTestUsers,
				GroupTestInitGroupConfig,
				GroupTestConfigurationCode,
				GroupTestApiUserPtrArr,
				GroupTestCreateGroupRequest,
				GroupTestGroupInfo);

			Done.Execute();
			return bIsSuccess;
		});
		// ===================================================================
		
		LatentIt("01: Should GET::SearchGroups for list of groups", [this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = GroupTestGetGroupList(
				GroupTestGroupName,
				// GroupTestGroupRegion,
				GroupTestApiUserPtrArr[0],
				GroupTestGroupInfo);				
			
			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("02: Should GET::GetGroup by GroupId", [this](const FDoneDelegate& Done)
		{			
			const bool bIsSuccess = GroupTestGetGroupById(
				GroupTestApiUserPtrArr[0],
				GroupTestGroupInfo.GroupId);
					
			Done.Execute();
			return bIsSuccess;
		});
		
		LatentIt("03: Should PUT::UpdateGroup:GroupName", [this](const FDoneDelegate& Done)
		{
			FAccelByteModelsGroupUpdatable UpdateGroupRequest; // Available 				
			UpdateGroupRequest.GroupName = GroupTestUpdatedGroupName; // "Updated SDK Test Group Name"
			constexpr bool bCompletelyReplace = false;
			
			const bool bIsSuccess = GroupTestUpdateGroup(
				GroupTestApiUserPtrArr[0],
				GroupTestGroupInfo.GroupId,
				bCompletelyReplace,
				UpdateGroupRequest,
				GroupTestGroupInfo);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("04: Should PUT::GroupTestUpdateCustomAttributes:CustomAttributes with a new string",
			[this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = CallUpdateCustomAttributes(
				GroupTestUpdatedAttrTestStrKey,
				GroupTestApiUserPtrArr,
				GroupTestGroupInfo.GroupId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("05: Should DELETE::DeleteGroup; returns void", [this](const FDoneDelegate& Done)
		{
			// This is how Group Admin's (group leaders) leave/disband their group
			const bool bIsSuccess = GroupTestDeleteGroupById(
				GroupTestApiUserPtrArr[0],
				GroupTestGroupInfo.GroupId);

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
	
			const bool bIsSuccess = GroupTestUpdateGroupCustomRule(
				GroupTestApiUserPtrArr[0],
				GroupTestGroupInfo.GroupId,
				RequestContent);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("07: Should PUT::UpdateGroupPredefinedRule with new arbitrary bool",
			[this](const FDoneDelegate& Done)
		{
			FAccelByteModelsUpdateGroupPredefinedRuleRequest RequestContent;
			RequestContent.RuleDetail = GetMockRuleInfoArr(2);
			
			const bool bIsSuccess = GroupTestUpdateGroupPredefinedRule(
				GroupTestApiUserPtrArr[0],
				GroupTestGroupInfo.GroupId,
				EAccelByteAllowedAction::createGroup,
				RequestContent);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("08: Should DELETE::DeleteGroupPredefinedRule; returns void",
			[this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = GroupTestDeleteGroupPredefinedRule(
				GroupTestApiUserPtrArr[0],
				GroupTestGroupInfo.GroupId,
				EAccelByteAllowedAction::createGroup);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("09: Should POST::AcceptGroupInvitation; returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[0] invites [1] to group
			const bool bUser0Invited1 = GroupTestInviteUserToGroup(
				GroupTestApiUserPtrArr[0],
				GroupTestUsers[1].Credentials.GetUserId());

			if (!bUser0Invited1)
			{
				Done.Execute();
				return false;
			}

			// -----------
			// TestUser[1] will accept [0]'s invitation to group up >>
			const bool bIsSuccess = GroupTestAcceptGroupInvitation(
				GroupTestApiUserPtrArr[1],
				GroupTestGroupInfo.GroupId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("10: Should POST::RejectGroupInvitation (after TestUser[1] invites [0]); returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[0] invites [1] to their OPEN group
			const bool bTestUser0Invited1 = GroupTestInviteUserToGroup(
				GroupTestApiUserPtrArr[0],
				GroupTestUsers[1].Credentials.GetUserId());

			if (!bTestUser0Invited1)
			{
				Done.Execute();
				return false;
			}

			// ------------------
			// TestUser[1] will reject TestUser[0]'s invitation to group up >>
			const bool bIsSuccess = GroupTestRejectGroupInvitation(
				GroupTestApiUserPtrArr[1],
				GroupTestGroupInfo.GroupId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("11: Should POST::JoinGroup (OPEN); returns group+user ids & status",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will join [0]'s OPEN group (OPEN groups don't require a request to join)
			const bool bIsSuccess = GroupTestJoinGroup(
				GroupTestApiUserPtrArr[1],
				GroupTestGroupInfo.GroupId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("12: Should POST::LeaveGroup (after [1] joins [0]'s group); returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will join [0]'s OPEN group.
			// Admins (leaders) can't LeaveGroup(): If a Group Admin like [0] wants to leave their group, use DeleteGroup
			const bool bJoinedGroup = GroupTestJoinGroup(
				GroupTestApiUserPtrArr[1],
				GroupTestGroupInfo.GroupId);

			if (!bJoinedGroup)
			{
				Done.Execute();
				return false;
			}

			// -----------------
			// TestUser[1] will leave [0]'s group >>
			const bool bIsSuccess = GroupTestLeaveGroup(GroupTestApiUserPtrArr[1]);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("13: Should GET::GetUserGroupInfoByUserId, "
			"where TestUser[0] gets [0]'s own group info; returns group info",
			[this](const FDoneDelegate& Done)
		{				
			const bool bIsSuccess = GroupTestGetUserGroupInfoByUserId(
				GroupTestApiUserPtrArr[0],
				GroupTestGroupInfo.GroupMembers[0].UserId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("14: Should POST::InviteUserToGroup, where TestUser[0] invites [1]; returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = GroupTestInviteUserToGroup(
				GroupTestApiUserPtrArr[0],
				GroupTestUsers[1].Credentials.GetUserId());

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("15: Should GET::GetGroupMembersListByGroupId, "
			"where TestUser[0] invites [1]; returns paginated response",
			[this](const FDoneDelegate& Done)
		{
			// Defaults are ok - just paginated options 
			const FAccelByteModelsGetGroupMembersListByGroupIdRequest GetGroupMembersListByGroupIdRequest;
				
			const bool bIsSuccess = GroupTestGetGroupMembersListByGroupId(
				GroupTestApiUserPtrArr[0],
				GroupTestGroupInfo.GroupId,
				GetGroupMembersListByGroupIdRequest);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("16: Should POST::KickGroupMember; returns group+userKicked ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will join [0]'s OPEN group (won't required a request to join)
			const bool bJoinedGroup = GroupTestJoinGroup(
				GroupTestApiUserPtrArr[1],
				GroupTestGroupInfo.GroupId);

			if (!bJoinedGroup)
			{
				Done.Execute();
				return false;
			}

			// ---------------
			// TestUser[1] will kick [0] out of the group
			const bool bIsSuccess = GroupTestKickGroupMember(
				GroupTestApiUserPtrArr[0],
				GroupTestUsers[1].Credentials.GetUserId());

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("17: Should GET::GetMemberRoles (from group member); returns paginated id, name, perms",
			[this](const FDoneDelegate& Done)
		{
			constexpr FAccelByteModelsLimitOffsetRequest RequestContent; // { Limit=1, Offset=0 }
				
			const bool bIsSuccess = GroupTestGetMemberRoles(
				GroupTestApiUserPtrArr[0],
				RequestContent);
					
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
			const bool bIsSuccess =  GroupTestMockAssignRole(
				GroupTestUsers,
				GroupTestApiUserPtrArr,
				GroupTestGroupInfo.GroupId,
				GroupTestRoleResourceName,
				GroupTestRoleResourceName,
				GroupTestRoleId); // out
				
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
			const bool bUser1WasAssignedRoleBy0 =  GroupTestMockAssignRole(
				GroupTestUsers,
				GroupTestApiUserPtrArr,
				GroupTestGroupInfo.GroupId,
				GroupTestRoleResourceName,
				GroupTestRoleResourceName,
				GroupTestRoleId); // out

			if (!bUser1WasAssignedRoleBy0)
			{
				Done.Execute();
				return false;
			}

			// -------------
			// Then TestUser[0] removes [1]'s new role
			FAccelByteModelsUserIdWrapper UserIdWrapper;
			UserIdWrapper.UserId = GroupTestUsers[1].Credentials.GetUserId();
				
			const bool bIsSuccess = GroupTestDeleteMemberRole(
				GroupTestApiUserPtrArr[0],
            	GroupTestRoleId,
            	UserIdWrapper);
				
			Done.Execute();
			return bIsSuccess;
		});
		
		LatentIt("20: Should POST::GetGroupInvitationRequests; where TestUser[0] invites [1], "
			"then [1] gets their own list of invites; returns a list of invites, containing the one from [0]",
        	[this](const FDoneDelegate& Done)
        {
        	// TestUser[0] invites [1] to group
        	const bool bUser0Invited1 = GroupTestInviteUserToGroup(
        		GroupTestApiUserPtrArr[0],
        		GroupTestUsers[1].Credentials.GetUserId());

        	if (!bUser0Invited1)
        	{
        		Done.Execute();
        		return false;
        	}

        	// -----------
        	// TestUser[1] will get their own list of invites (which should contain [0]s invite)
        	constexpr FAccelByteModelsLimitOffsetRequest RequestContent;     		
        	const bool bIsSuccess = GroupTestGetGroupInvitationRequests(
        		GroupTestApiUserPtrArr[1],
        		RequestContent);

        	Done.Execute();
        	return bIsSuccess;
        });

		// ===================================================================		
		LatentAfterEach([this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = InitTeardown(
				GroupTestUsers,
				GroupTestConfigurationCode);

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
			
			const bool bIsSuccess = GroupTestPreInitAll(
				NumTestUsers,
				bCreateGroupNow,
				GroupTestGroupName,
				GroupTestGroupRegion,
				GroupType,
				GroupTestUsers,
				GroupTestInitGroupConfig,
				GroupTestConfigurationCode,
				GroupTestApiUserPtrArr,
				GroupTestCreateGroupRequest,
				GroupTestGroupInfo);

			Done.Execute();
			return bIsSuccess;
		});
		// ===================================================================
		
		LatentIt("01: Should POST::CancelJoinGroupRequest, where TestUser[1] cancels "
			"PUBLIC join group req to [1]'s PUBLIC group; returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will send a REQUEST to join [0]'s PUBLIC group
			const bool bSentJoinRequest = GroupTestJoinGroup(
				GroupTestApiUserPtrArr[1],
				GroupTestGroupInfo.GroupId);

			if (!bSentJoinRequest)
			{
				Done.Execute();
				return false;
			}

			// -------------
			// TestUser[1] no longer wants to join, so cancels their join request to [0]'s PUBLIC group.
			const bool bIsSuccess = GroupTestCancelJoinGroupRequest(
				GroupTestApiUserPtrArr[1],
				GroupTestGroupInfo.GroupId);

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("02: Should POST::AcceptGroupJoinRequest, where TestUser[1] (in a PUBLIC group) "
			"accepts join group req from [0]; returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will send a REQUEST to join [0]'s PUBLIC group
			const bool bSentJoinRequest = GroupTestJoinGroup(
				GroupTestApiUserPtrArr[1],
				GroupTestGroupInfo.GroupId);

			if (!bSentJoinRequest)
			{
				Done.Execute();
				return false;
			}

			// -------------
			// TestUser[0] accepts [1]'s request to join [0]'s PUBLIC group.
			const bool bIsSuccess = GroupTestAcceptGroupJoinRequest(
				GroupTestApiUserPtrArr[0],
				GroupTestUsers[1].Credentials.GetUserId());

			Done.Execute();
			return bIsSuccess;
		});

		
		LatentIt("03: Should POST::RejectGroupJoinRequest, where TestUser[1] (in a PUBLIC group), "
			"then rejects join group req from [0]; returns group+user ids",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will send a REQUEST to join [0]'s PUBLIC group
			const bool bSentJoinRequest = GroupTestJoinGroup(
				GroupTestApiUserPtrArr[1],
				GroupTestGroupInfo.GroupId);

			if (!bSentJoinRequest)
			{
				Done.Execute();
				return false;
			}

			// -------------
			// TestUser[0] rejects [1]'s request to join [0]'s PUBLIC group.
			const bool bIsSuccess = GroupTestRejectGroupJoinRequest(
				GroupTestApiUserPtrArr[0],
				GroupTestUsers[1].Credentials.GetUserId());

			Done.Execute();
			return bIsSuccess;
		});

		LatentIt("04: Should POST::GetGroupJoinRequests, where TestUser[1] (in a PUBLIC group), "
			"then [0] gets join requests; returns a list of group join requests",
			[this](const FDoneDelegate& Done)
		{
			// TestUser[1] will send a REQUEST to join [0]'s PUBLIC group
			const bool bSentJoinRequest = GroupTestJoinGroup(
				GroupTestApiUserPtrArr[1],
				GroupTestGroupInfo.GroupId);

			if (!bSentJoinRequest)
			{
				Done.Execute();
				return false;
			}

			// -------------
			// Then [0] gets join requests
			constexpr FAccelByteModelsLimitOffsetRequest RequestContent;
			const bool bIsSuccess = GroupTestGetJoinRequestsRequest(
				GroupTestApiUserPtrArr[0],
				GroupTestGroupInfo.GroupId,
				RequestContent);

			Done.Execute();
			return bIsSuccess;
		});
				
		// ===================================================================
		LatentAfterEach([this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = InitTeardown(
				GroupTestUsers,
				GroupTestConfigurationCode);

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
			
			const bool bIsSuccess = GroupTestPreInitAll(
				NumTestUsers,
				bCreateGroupNow,
				GroupTestGroupName,
				GroupTestGroupRegion,
				GroupType,
				GroupTestUsers,
				GroupTestInitGroupConfig,
				GroupTestConfigurationCode,
				GroupTestApiUserPtrArr,
				GroupTestCreateGroupRequest,
				GroupTestGroupInfo);

			Done.Execute();
			return bIsSuccess;
		});
		// ===================================================================

		LatentIt("01: Should POST::CreateGroupcreate, where TestUser[0] creates OPEN group; returns group info",
			[this](const FDoneDelegate& Done)
		{
			constexpr EAccelByteGroupType GroupType = EAccelByteGroupType::OPEN;
				
			const bool bIsSuccess = GroupTestPreInitCreateGroup(
				GroupTestGroupName,
				GroupTestGroupRegion,
				GroupType,
				GroupTestConfigurationCode,
				GroupTestApiUserPtrArr[0],
				GroupTestCreateGroupRequest,
				GroupTestGroupInfo);
	
			Done.Execute();
			return bIsSuccess;
		});

		// ===================================================================
		LatentAfterEach([this](const FDoneDelegate& Done)
		{
			const bool bIsSuccess = InitTeardown(
				GroupTestUsers,
				GroupTestConfigurationCode);

			Done.Execute();
			return bIsSuccess;
		});
	});
}
