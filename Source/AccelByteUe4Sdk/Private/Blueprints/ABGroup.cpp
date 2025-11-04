#include "Blueprints/ABGroup.h"

using namespace AccelByte;

void UABGroup::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

#pragma region Group V1

#pragma region Group (multi-member actions)
void UABGroup::CreateGroup(
	const FAccelByteModelsCreateGroupRequest& RequestContent,
	const FCreateGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->CreateGroup(
		RequestContent,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGroupInformation Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::GetGroupList(
	const FAccelByteModelsGetGroupListRequest& RequestContent,
	const FSearchGroupsSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetGroupList(
		RequestContent,
		THandler<FAccelByteModelsGetGroupListResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGetGroupListResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::GetGroup(
	const FString& GroupId,
	const FGetGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetGroup(
		GroupId,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGroupInformation Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::UpdateGroup(
	const FString& GroupId,
	const bool bCompletelyReplace,
	const FAccelByteModelsGroupUpdatable& RequestContent,
	const FUpdateGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	FAccelByteModelsGroupUpdatable RequestContentCopy = RequestContent;
	if (!RequestContentCopy.CustomAttributes.JsonObject.IsValid())
	{
		RequestContentCopy.CustomAttributes.JsonObjectFromString(RequestContentCopy.CustomAttributes.JsonString);
	}

	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->UpdateGroup(
		GroupId,
		bCompletelyReplace,
		RequestContentCopy,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGroupInformation Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup:: UpdateGroupCustomAttributes(
	const FString& GroupId,
	const FAccelByteModelsUpdateGroupCustomAttributesRequest& RequestContent,
	const FUpdateGroupCustomAttributesSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	FAccelByteModelsUpdateGroupCustomAttributesRequest RequestContentCopy = RequestContent;
	if (!RequestContentCopy.CustomAttributes.JsonObject.IsValid())
	{
		RequestContentCopy.CustomAttributes.JsonObjectFromString(RequestContentCopy.CustomAttributes.JsonString);
	}

	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->UpdateGroupCustomAttributes(
		GroupId,
		RequestContentCopy,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGroupInformation Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::DeleteGroup(
	const FString& GroupId,
	const FDeleteGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->DeleteGroup(
	GroupId,
	FVoidHandler::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
	FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::UpdateGroupCustomRule(
	const FString& GroupId,
	const FAccelByteModelsUpdateCustomRulesRequest& RequestContent,
	const FUpdateGroupCustomRuleSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	FAccelByteModelsUpdateCustomRulesRequest RequestContentCopy = RequestContent;
	if (!RequestContentCopy.GroupCustomRule.JsonObject.IsValid())
	{
		RequestContentCopy.GroupCustomRule.JsonObjectFromString(RequestContentCopy.GroupCustomRule.JsonString);
	}

	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->UpdateGroupCustomRule(
		GroupId,
		RequestContentCopy,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGroupInformation Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::UpdateGroupPredefinedRule(
	const FString& GroupId,
	const EAccelByteAllowedAction& AllowedAction,
	const FAccelByteModelsUpdateGroupPredefinedRuleRequest& RequestContent,
	const FUpdateGroupPredefinedRuleSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->UpdateGroupPredefinedRule(
		GroupId,
		AllowedAction,
		RequestContent,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGroupInformation Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::DeleteGroupPredefinedRule(
	const FString& GroupId,
	const EAccelByteAllowedAction& AllowedAction,
	const FDeleteGroupPredefinedRuleSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->DeleteGroupPredefinedRule(
		GroupId,
		AllowedAction,
		FVoidHandler::CreateLambda([OnSuccess]()
		{
			OnSuccess.ExecuteIfBound();
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}
#pragma endregion /Group (multi-member actions)


#pragma region Group Member (individuals)
void UABGroup::AcceptGroupInvitation(
	const FString& GroupId, 
	const FAcceptGroupInvitationSuccess& OnSuccess, 
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->AcceptGroupInvitation(
		GroupId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::RejectGroupInvitation(
	const FString& GroupId, 
	const FRejectGroupInvitationSuccess& OnSuccess, 
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->RejectGroupInvitation(
		GroupId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::JoinGroup(
	const FString& GroupId, 
	const FJoinGroupSuccess& OnSuccess, 
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->JoinGroup(
		GroupId,
		THandler<FAccelByteModelsJoinGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsJoinGroupResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::CancelJoinGroupRequest(
	const FString& GroupId, 
	const FCancelJoinGroupRequestSuccess& OnSuccess, 
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->CancelJoinGroupRequest(
		GroupId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::GetGroupMembersListByGroupId(
	const FString& GroupId, 
	const FAccelByteModelsGetGroupMembersListByGroupIdRequest& RequestContent,
	const FGetGroupMembersListByGroupIdSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetGroupMembersListByGroupId(
		GroupId,
		RequestContent,
		THandler<FAccelByteModelsGetGroupMemberListResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGetGroupMemberListResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::LeaveGroup(const FLeaveGroupSuccess& OnSuccess, const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->LeaveGroup(
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::GetUserGroupInfoByUserId(
	const FString& UserId,
	const FGetUserGroupInfoByUserIdSuccess& OnSuccess, 
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetUserGroupInfoByUserId(
		UserId,
		THandler<FAccelByteModelsGetUserGroupInfoResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGetUserGroupInfoResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::InviteUserToGroup(
	const FString& UserId,
	const FInviteUserToGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->InviteUserToGroup(
		UserId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::AcceptGroupJoinRequest(
	const FString& UserId,
	const FAcceptGroupJoinRequestSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->AcceptGroupJoinRequest(
		UserId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::RejectGroupJoinRequest(
	const FString& UserId,
	const FRejectGroupJoinRequestSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->RejectGroupJoinRequest(
		UserId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::KickGroupMember(
	const FString& UserId,
	const FKickGroupMemberSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->KickGroupMember(
		UserId,
		THandler<FAccelByteModelsKickGroupMemberResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsKickGroupMemberResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}
#pragma endregion /Group Member (individuals)


#pragma region Group Roles (permissions)
void UABGroup::GetMemberRoles(
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const FGetMemberRolesSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetMemberRoles(
		RequestContent,
		THandler<FAccelByteModelsGetMemberRolesListResponse>::CreateLambda(
		[OnSuccess](const FAccelByteModelsGetMemberRolesListResponse Response)
	{
		OnSuccess.ExecuteIfBound(Response);
	}),
	FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::AssignMemberRole(
	const FString& MemberRoleId,
	const FAccelByteModelsUserIdWrapper& RequestContent,
	const FAssignMemberRoleSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->AssignMemberRole(
	MemberRoleId,
	RequestContent,
	THandler<FAccelByteModelsGetUserGroupInfoResponse>::CreateLambda(
		[OnSuccess](const FAccelByteModelsGetUserGroupInfoResponse Response)
	{
		OnSuccess.ExecuteIfBound(Response);
	}),
	FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::DeleteMemberRole(
	const FString& MemberRoleId,
	const FAccelByteModelsUserIdWrapper& RequestContent,
	const FDeleteMemberRoleSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->DeleteMemberRole(
		MemberRoleId,
		RequestContent,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
	FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}
#pragma endregion /Group Roles (permissions)


#pragma region Group Member Request
void UABGroup::GetGroupJoinRequests(
	const FString& GroupId,
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const FGetGroupJoinRequestsSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetGroupJoinRequests(
	GroupId,
	RequestContent,
	THandler<FAccelByteModelsGetMemberRequestsListResponse>::CreateLambda(
		[OnSuccess](const FAccelByteModelsGetMemberRequestsListResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
	FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::GetGroupInvitationRequests(
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const FGetGroupInvitationRequestsSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetGroupInvitationRequests(
	RequestContent,
	THandler<FAccelByteModelsGetMemberRequestsListResponse>::CreateLambda(
		[OnSuccess](const FAccelByteModelsGetMemberRequestsListResponse Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
	FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

#pragma endregion /Group MemberRequest

#pragma endregion Group V1

#pragma region Group V2

#pragma region Group V2 (multi-member actions)

void UABGroup::CreateV2Group(
	const FAccelByteModelsCreateGroupRequest& RequestContent, 
	const FCreateGroupSuccess& OnSuccess, 
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->CreateV2Group(
		RequestContent,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGroupInformation& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::GetGroupsByGroupIds(
	const TArray<FString>& GroupIds,
	const FGetGroupsByGroupIdsSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetGroupsByGroupIds(
		GroupIds,
		THandler<FAccelByteModelsGetGroupListResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGetGroupListResponse& Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode,ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::UpdateV2Group(
	const FString& GroupId,
	const FAccelByteModelsUpdateGroupRequest& RequestContent,
	const FUpdateGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->UpdateV2Group(
		GroupId,
		RequestContent,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
		[OnSuccess](const FAccelByteModelsGroupInformation& Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode,ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::DeleteV2Group(
	const FString& GroupId,
	const FDeleteGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->DeleteV2Group(
		GroupId,
		FVoidHandler::CreateLambda([OnSuccess]()
		{
			OnSuccess.ExecuteIfBound();
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode,ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::UpdateV2GroupCustomAttributes(
	const FString& GroupId,
	const FAccelByteModelsUpdateGroupCustomAttributesRequest& RequestContent,
	const FUpdateGroupCustomAttributesSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->UpdateV2GroupCustomAttributes(
		GroupId,RequestContent,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda([OnSuccess](const FAccelByteModelsGroupInformation& Response)
		{
			OnSuccess.ExecuteIfBound(Response);
		}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode,ErrorMessage);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::GetGroupInviteRequestList(
	const FString& GroupId,
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const FGetGroupInvitationRequestsSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetGroupInviteRequestList(
		GroupId,RequestContent,
		THandler<FAccelByteModelsGetMemberRequestsListResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGetMemberRequestsListResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode,ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}


void UABGroup::UpdateV2GroupCustomRule(
	const FString& GroupId,
	const FAccelByteModelsUpdateCustomRulesRequest& RequestContent,
	const FUpdateGroupCustomRuleSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->UpdateV2GroupCustomRule(GroupId,RequestContent,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGroupInformation& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode,ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::DeleteV2GroupPredefinedRule(
	const FString& GroupId,
	const EAccelByteAllowedAction& AllowedAction,
	const FDeleteGroupPredefinedRuleSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->DeleteV2GroupPredefinedRule(
		GroupId,AllowedAction,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
			FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode,ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::GetUserGroupStatusInfo(
	const FString& UserId,
	const FString& GroupId,
	const FGetUserGroupInfoSuccess& OnSuccess,
	const FDErrorHandler OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetUserGroupStatusInfo(
		UserId,GroupId,
		THandler<FAccelByteModelsGetUserGroupInfoResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGetUserGroupInfoResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode,ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::UpdateV2GroupPredefinedRule(
	const FString& GroupId,
	const EAccelByteAllowedAction& AllowedAction,
	const FAccelByteModelsUpdateGroupPredefinedRuleRequest& RequestContent, 
	const FUpdateGroupPredefinedRuleSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->UpdateV2GroupPredefinedRule(
		GroupId,AllowedAction,RequestContent,
		THandler<FAccelByteModelsGroupInformation>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGroupInformation& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode,ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::GetGroupJoinRequestList(
	const FString& GroupId,
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const FGetGroupJoinRequestsSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetGroupJoinRequestList(
		GroupId,RequestContent,
		THandler<FAccelByteModelsGetMemberRequestsListResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGetMemberRequestsListResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode,ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::CancelGroupMemberInvitation(
	const FString& UserId,
	const FString& GroupId,
	const FCanceGroupInviteRequestSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->CancelGroupMemberInvitation(
		UserId,GroupId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode,ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}


#pragma endregion Group V2 (multi-member actions)

#pragma region Group V2 Member (individuals)

void UABGroup::AcceptV2GroupInvitation(
	const FString& GroupId, 
	const FAcceptGroupInvitationSuccess& OnSuccess, 
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->AcceptV2GroupInvitation(
		GroupId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::RejectV2GroupInvitation(
	const FString& GroupId, 
	const FRejectGroupInvitationSuccess& OnSuccess, 
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->RejectV2GroupInvitation(
		GroupId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::JoinV2Group(
	const FString& GroupId, 
	const FJoinGroupSuccess& OnSuccess, 
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->JoinV2Group(
		GroupId,
		THandler<FAccelByteModelsJoinGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsJoinGroupResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::LeaveV2Group(
	const FString& GroupId, 
	const FLeaveGroupSuccess& OnSuccess, 
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->LeaveV2Group(
		GroupId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::InviteUserToV2Group(
	const FString& UserId, 
	const FString& GroupId, 
	const FInviteUserToGroupSuccess& OnSuccess, 
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->InviteUserToV2Group(
		UserId,
		GroupId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::AcceptV2GroupJoinRequest(
	const FString& UserId,
	const FString& GroupId, 
	const FAcceptGroupJoinRequestSuccess& OnSuccess, 
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->AcceptV2GroupJoinRequest(
		UserId,
		GroupId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::RejectV2GroupJoinRequest(
	const FString& UserId, 
	const FString& GroupId, 
	const FRejectGroupJoinRequestSuccess& OnSuccess, 
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->RejectV2GroupJoinRequest(
		UserId,
		GroupId,
		THandler<FAccelByteModelsMemberRequestGroupResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMemberRequestGroupResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::KickV2GroupMember(
	const FString& UserId, 
	const FString& GroupId, 
	const FKickGroupMemberSuccess& OnSuccess, 
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->KickV2GroupMember(
		UserId,
		GroupId,
		THandler<FAccelByteModelsKickGroupMemberResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsKickGroupMemberResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::GetMyJoinedGroupInfo(
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const FGetMyJoinedGroupInfoSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetMyJoinedGroupInfo(
		RequestContent,
		THandler<FAccelByteModelsGetGroupMemberListResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGetGroupMemberListResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::GetMyJoinGroupRequest(
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const FGetMyJoinGroupRequestSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetMyJoinGroupRequest(
		RequestContent,
		THandler<FAccelByteModelsGetMemberRequestsListResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGetMemberRequestsListResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::AssignV2MemberRole(
	const FString& MemberRoleId, 
	const FString& GroupId, 
	const FAccelByteModelsUserIdWrapper& RequestContent,
	const FAssignMemberRoleSuccess& OnSuccess, 
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->AssignV2MemberRole(
		MemberRoleId,
		GroupId,
		RequestContent,
		THandler<FAccelByteModelsGetUserGroupInfoResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGetUserGroupInfoResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::DeleteV2MemberRole(
	const FString& MemberRoleId, 
	const FString& GroupId, 
	const FAccelByteModelsUserIdWrapper& RequestContent,
	const FDeleteMemberRoleSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->DeleteV2MemberRole(
		MemberRoleId,
		GroupId,
		RequestContent,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
			FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABGroup::GetAllMemberRoles(
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const FGetMemberRolesSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	const auto GroupPtr = ApiClientPtr->GetGroupApi().Pin();
	if (GroupPtr.IsValid())
	{
		GroupPtr->GetAllMemberRoles(
		RequestContent,
		THandler<FAccelByteModelsGetMemberRolesListResponse>::CreateLambda(
			[OnSuccess](const FAccelByteModelsGetMemberRolesListResponse& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](const int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}


#pragma endregion Group V2 Member (individuals)

#pragma endregion Group V2

