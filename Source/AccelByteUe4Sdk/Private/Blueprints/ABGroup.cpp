#include "Blueprints/ABGroup.h"

void UABGroup::SetApiClient(const FApiClientPtr NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}


#pragma region Group (multi-member actions)
void UABGroup::CreateGroup(
	const FAccelByteModelsCreateGroupRequest& RequestContent,
	const FCreateGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.CreateGroup(
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

void UABGroup::GetGroupList(
	const FAccelByteModelsGetGroupListRequest& RequestContent,
	const FSearchGroupsSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.GetGroupList(
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

void UABGroup::GetGroup(
	const FString& GroupId,
	const FGetGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.GetGroup(
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

void UABGroup::UpdateGroup(
	const FString& GroupId,
	const bool bCompletelyReplace,
	FAccelByteModelsGroupUpdatable& RequestContent,
	const FUpdateGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	if (!RequestContent.CustomAttributes.JsonObject.IsValid())
	{
		RequestContent.CustomAttributes.JsonObjectFromString(RequestContent.CustomAttributes.JsonString);
	}

	ApiClientPtr->Group.UpdateGroup(
		GroupId,
		bCompletelyReplace,
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

void UABGroup:: UpdateGroupCustomAttributes(
	const FString& GroupId,
	FAccelByteModelsUpdateGroupCustomAttributesRequest& RequestContent,
	const FUpdateGroupCustomAttributesSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	if (!RequestContent.CustomAttributes.JsonObject.IsValid())
	{
		RequestContent.CustomAttributes.JsonObjectFromString(RequestContent.CustomAttributes.JsonString);
	}

	ApiClientPtr->Group.UpdateGroupCustomAttributes(
		GroupId,
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

void UABGroup::DeleteGroup(
	const FString& GroupId,
	const FDeleteGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.DeleteGroup(
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

void UABGroup::UpdateGroupCustomRule(
	const FString& GroupId,
	FAccelByteModelsUpdateCustomRulesRequest& RequestContent,
	const FUpdateGroupCustomRuleSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	if (!RequestContent.GroupCustomRule.JsonObject.IsValid())
	{
		RequestContent.GroupCustomRule.JsonObjectFromString(RequestContent.GroupCustomRule.JsonString);
	}

	ApiClientPtr->Group.UpdateGroupCustomRule(
		GroupId,
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

void UABGroup::UpdateGroupPredefinedRule(
	const FString& GroupId,
	const EAccelByteAllowedAction& AllowedAction,
	const FAccelByteModelsUpdateGroupPredefinedRuleRequest& RequestContent,
	const FUpdateGroupPredefinedRuleSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.UpdateGroupPredefinedRule(
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

void UABGroup::DeleteGroupPredefinedRule(
	const FString& GroupId,
	const EAccelByteAllowedAction& AllowedAction,
	const FDeleteGroupPredefinedRuleSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.DeleteGroupPredefinedRule(
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
#pragma endregion /Group (multi-member actions)


#pragma region Group Member (individuals)
void UABGroup::AcceptGroupInvitation(
	const FString& GroupId, 
	const FAcceptGroupInvitationSuccess& OnSuccess, 
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.AcceptGroupInvitation(
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

void UABGroup::RejectGroupInvitation(
	const FString& GroupId, 
	const FRejectGroupInvitationSuccess& OnSuccess, 
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.RejectGroupInvitation(
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

void UABGroup::JoinGroup(
	const FString& GroupId, 
	const FJoinGroupSuccess& OnSuccess, 
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.JoinGroup(
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

void UABGroup::CancelJoinGroupRequest(
	const FString& GroupId, 
	const FCancelJoinGroupRequestSuccess& OnSuccess, 
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.CancelJoinGroupRequest(
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

void UABGroup::GetGroupMembersListByGroupId(
	const FString& GroupId, 
	const FAccelByteModelsGetGroupMembersListByGroupIdRequest& RequestContent,
	const FGetGroupMembersListByGroupIdSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.GetGroupMembersListByGroupId(
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

void UABGroup::LeaveGroup(const FLeaveGroupSuccess& OnSuccess, const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.LeaveGroup(
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

void UABGroup::GetUserGroupInfoByUserId(
	const FString& UserId,
	const FGetUserGroupInfoByUserIdSuccess& OnSuccess, 
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.GetUserGroupInfoByUserId(
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

void UABGroup::InviteUserToGroup(
	const FString UserId,
	const FInviteUserToGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.InviteUserToGroup(
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

void UABGroup::AcceptGroupJoinRequest(
	const FString UserId,
	const FAcceptGroupJoinRequestSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.AcceptGroupJoinRequest(
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

void UABGroup::RejectGroupJoinRequest(
	const FString UserId,
	const FRejectGroupJoinRequestSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.RejectGroupJoinRequest(
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

void UABGroup::KickGroupMember(
	const FString UserId,
	const FKickGroupMemberSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.KickGroupMember(
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
#pragma endregion /Group Member (individuals)


#pragma region Group Roles (permissions)
void UABGroup::GetMemberRoles(
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const FGetMemberRolesSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.GetMemberRoles(
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

void UABGroup::AssignMemberRole(
	const FString& MemberRoleId,
	const FAccelByteModelsUserIdWrapper& RequestContent,
	const FAssignMemberRoleSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.AssignMemberRole(
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

void UABGroup::DeleteMemberRole(
	const FString& MemberRoleId,
	const FAccelByteModelsUserIdWrapper& RequestContent,
	const FDeleteMemberRoleSuccess& OnSuccess,
	const FDErrorHandler& OnError) 
{
	ApiClientPtr->Group.DeleteMemberRole(
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
#pragma endregion /Group Roles (permissions)


#pragma region Group Member Request
void UABGroup::GetGroupJoinRequests(
	const FString& GroupId,
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const FGetGroupJoinRequestsSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	ApiClientPtr->Group.GetGroupJoinRequests(
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

void UABGroup::GetGroupInvitationRequests(
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const FGetGroupInvitationRequestsSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	ApiClientPtr->Group.GetGroupInvitationRequests(
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

#pragma endregion /Group MemberRequest