﻿// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteGroupModels.h"

#include "ABGroup.generated.h"

#pragma region Dynamic Delegates
// Group (multi-member actions)
DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateGroupSuccess, const FAccelByteModelsGroupInformation&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSearchGroupsSuccess, const FAccelByteModelsGetGroupListResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetGroupsByGroupIdsSuccess, const FAccelByteModelsGetGroupListResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetGroupSuccess, const FAccelByteModelsGroupInformation&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGroupSuccess, const FAccelByteModelsGroupInformation&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGroupCustomAttributesSuccess, const FAccelByteModelsGroupInformation&, Response);
DECLARE_DYNAMIC_DELEGATE(FDeleteGroupSuccess);
DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGroupCustomRuleSuccess, const FAccelByteModelsGroupInformation&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGroupPredefinedRuleSuccess, const FAccelByteModelsGroupInformation&, Response);
DECLARE_DYNAMIC_DELEGATE(FDeleteGroupPredefinedRuleSuccess);

// Group Member (individuals)
DECLARE_DYNAMIC_DELEGATE_OneParam(FAcceptGroupInvitationSuccess, const FAccelByteModelsMemberRequestGroupResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FRejectGroupInvitationSuccess, const FAccelByteModelsMemberRequestGroupResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FJoinGroupSuccess, const FAccelByteModelsJoinGroupResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FCancelJoinGroupRequestSuccess, const FAccelByteModelsMemberRequestGroupResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FCanceGroupInviteRequestSuccess, const FAccelByteModelsMemberRequestGroupResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetGroupMembersListByGroupIdSuccess, const FAccelByteModelsGetGroupMemberListResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FLeaveGroupSuccess, const FAccelByteModelsMemberRequestGroupResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserGroupInfoByUserIdSuccess, const FAccelByteModelsGetUserGroupInfoResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FInviteUserToGroupSuccess, const FAccelByteModelsMemberRequestGroupResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FAcceptGroupJoinRequestSuccess, const FAccelByteModelsMemberRequestGroupResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FRejectGroupJoinRequestSuccess, const FAccelByteModelsMemberRequestGroupResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FKickGroupMemberSuccess, const FAccelByteModelsKickGroupMemberResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserGroupInfoSuccess, const FAccelByteModelsGetUserGroupInfoResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetMyJoinedGroupInfoSuccess, const FAccelByteModelsGetGroupMemberListResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetMyJoinGroupRequestSuccess, const FAccelByteModelsGetMemberRequestsListResponse&, Response);

// Group Roles (permissions)
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetMemberRolesSuccess, const FAccelByteModelsGetMemberRolesListResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FAssignMemberRoleSuccess, const FAccelByteModelsGetUserGroupInfoResponse&, Response);
DECLARE_DYNAMIC_DELEGATE(FDeleteMemberRoleSuccess);

// Group Member Request
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetGroupJoinRequestsSuccess, const FAccelByteModelsGetMemberRequestsListResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetGroupInvitationRequestsSuccess, const FAccelByteModelsGetMemberRequestsListResponse&, Response);
#pragma endregion /Dynamic Delegates


/** @brief Provide APIs to access Group [Management] service.
 * - While authed, Namespace is automatically passed to all Requests.
 */
UCLASS(Blueprintable, BlueprintType)
class UABGroup final : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(AccelByte::FApiClientPtr const& NewApiClientPtr);

#pragma region Group V1	
	#pragma region Group (multi-member actions)
	/**
	 * @brief AKA "SearchGroups"; get list of groups.
	 * - Required valid user authentication.
	 * - Only shows OPEN and PUBLIC group types.
	 * - Can search based on the group name by filling the "groupName" query param.
	 *
	 * Action code:: 73303
	 * 
	 * @param RequestContent 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FGetGroupConfigListResponse&.
	 * - Result is simply a FAccelByteModelsGroupInformationResponse + pagination info.
	 * @param OnError This will be called when the operation failed.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void GetGroupList(
		const FAccelByteModelsGetGroupListRequest& RequestContent,
		const FSearchGroupsSuccess& OnSuccess,
		const FDErrorHandler& OnError);
	
	/**
	 * @brief Creates a new group.
	 * - Requires valid user authentication.
	 * - There are some fields that needs to be fulfilled.
	 *   - groupDescription: the description of the group (optional).
	 *   - groupIcon: group icon URL link (optional).
	 *   - groupName: name of the group.
	 *   - groupRegion: region of the group.
	 *   - groupRules: rules for specific group. It consists of groupCustomRule that can be
	 *       used to save custom rule, and groupPredefinedRules that has similar usage with configuration,
	 *       but this rule only works in specific group.
	 *   - allowedAction: available action in group service. It consist of joinGroup and inviteGroup.
		 - ruleAttribute: attribute of the player that needs to be checked.
		 - ruleCriteria: criteria of the value. The value will be in enum of EQUAL, MINIMUM, MAXIMUM.
		 - ruleValue: value that needs to be checked.
		 - customAttributes: additional custom group attributes (optional).
	 *
	 * Action code:: 73304
	 * 
	 * @param RequestContent New group detail request.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGroupInformationResponse&.
	 * @param OnError This will be called when the operation failed.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void CreateGroup(
		const FAccelByteModelsCreateGroupRequest& RequestContent,
		const FCreateGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Get single single group info (by groupId).
	 * - Required valid user authentication.
	 * - Gets single group information.
	 * 
	 * Action code: 73306
	 * 
	 * @param GroupId 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGroupInformationResponse&.
	 * @param OnError This will be called when the operation failed.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void GetGroup(
		const FString& GroupId,
		const FGetGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Update existing group.
	 * - Required valid user authentication.
	 * - Required Member Role Permission: "GROUP [UPDATE]".
	 * - Checks the group ID of the user based on the access token and
	 *     compares it with the group ID in path parameter.
	 * - Checks the member role of the user based on the access token.
	 * - Supports partial update (empty elements will convert to null)
	 * 
	 * Action Code: 73307
	 * 
	 * @param GroupId
	 * @param bCompletelyReplace Instead of partially-update, do you want to completely replace ALL info?
	 * @param RequestContent
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGroupInformationResponse&.
	 * @param OnError This will be called when the operation failed.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void UpdateGroup(
		const FString& GroupId,
		const bool bCompletelyReplace,
		FAccelByteModelsGroupUpdatable& RequestContent,
		const FUpdateGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Delete existing group.
	 * - Checks if groupID exists before deleting.
	 * - Required Member Role Permission: "GROUP [DELETE]".
	 * 
	 * Action code:: 73305
	 * 
	 * @param GroupId 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGroupInformationResponse&.
	 * @param OnError This will be called when the operation failed.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void DeleteGroup(
		const FString& GroupId,
		const FDeleteGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Replaces current group custom attributes entirely.
	 * - Requires valid user authentication.
	 * - Required Member Role Permission: "GROUP [UPDATE]".
	 * - Checks group ID of the user based on the access token and compares with group ID in path param.
	 * - Checks the member role of the user based on the access token.
	 *   
	 * Action code:: 73311
	 * 
	 * @param GroupId 
	 * @param RequestContent Arbitrary {}.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGroupInformationResponse&.
	 * @param OnError This will be called when the operation failed.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void UpdateGroupCustomAttributes(
		const FString& GroupId,
		FAccelByteModelsUpdateGroupCustomAttributesRequest& RequestContent,
		const FUpdateGroupCustomAttributesSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Update custom (arbitrary) group rule.
	 * - Required valid user authentication.
	 * - Checks group ID of the user (via access token) and compares with group ID in path param.
	 * - Checks member role of the user based on access token.
	 * 
	 * Action code:: 73308
	 * 
	 * @param GroupId
	 * @param RequestContent Arbitrary {}.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGroupInformationResponse&.
	 * @param OnError Called upon failed op.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void UpdateGroupCustomRule(
		const FString& GroupId,
		FAccelByteModelsUpdateCustomRulesRequest& RequestContent,
		const FUpdateGroupCustomRuleSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	/**
	 * @brief Update predefined group rule.
	 * - Required valid user authentication.
	 * - Required Member Role Permission: "GROUP [UPDATE]".
	 * - Checks group ID of the user (via access token) and compares with group ID in path param.
	 * - Checks member role of the user based on access token.
	 * - If rule action is not defined in the group, it will be added immediately.
	 * 
	 * Action code:: 73310
	 * 
	 * @param GroupId 
	 * @param AllowedAction eg: createGroup, joinedGroup.
	 * @param RequestContent RuleDetail { RuleAttribute, RuleCriteria, RuleValue }.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGroupInformationResponse&.
	 * @param OnError Called upon failed op.
	 */
	void UpdateGroupPredefinedRule(
		const FString& GroupId,
		const EAccelByteAllowedAction& AllowedAction,
		const FAccelByteModelsUpdateGroupPredefinedRuleRequest& RequestContent,
		const FUpdateGroupPredefinedRuleSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Delete predefined group rule, based on the allowed action.
	 * - Required valid user authentication.
	 * - Required Member Role Permission: "GROUP [UPDATE]".
	 * - Checks the group ID of the user based on the access token, then
	 *    compares it with the group ID in path parameter.
	 * - Checks the member role of the user based on the access token.
	 * 
	 * Action code:: 73309
	 * 
	 * @param GroupId 
	 * @param AllowedAction eg: createGroup, joinedGroup.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Void Result
	 * @param OnError Called upon failed op.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void DeleteGroupPredefinedRule(
		const FString& GroupId,
		const EAccelByteAllowedAction& AllowedAction,
		const FDeleteGroupPredefinedRuleSuccess& OnSuccess,
		const FDErrorHandler& OnError);
	#pragma endregion /Group (multi-member actions)
	
	#pragma region Group Member (individuals)
	/**
	* @brief Accepts an invitation from a 3rd-party group's group member to group up.
	* - Required valid user authentication.
	* - If specific user is !invited in the specific group ID, throw errorif the user !invited yet.
	* - Checks if the user who access this endpoint already joined the specific group.
	* - Deletes all requests (invite / join request) for the user who accesses this endpoint.
	* - Existing members will receive notification of the newly-accepted member.
	* 
	* Action code: 73401
	* 
	* @param GroupId of the group that invited you, that you are accepting the invite to.
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void AcceptGroupInvitation(
		const FString& GroupId,
		const FAcceptGroupInvitationSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Rejects an invitation from a 3rd-party group's group member to group up.
	* - Required valid user authentication.
	* - If specific user is !invited in the specific group ID, throw errorif the user !invited yet.
	* - Checks if the user who access this endpoint already joined the specific group.
	* 
	* Action code: 73402
	* 
	* @param GroupId of the group that invited you, that you are rejecting the invite from.
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void RejectGroupInvitation(
		const FString& GroupId,
		const FRejectGroupInvitationSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Join into specific group and become a group member.
	* - Required valid user authentication.
	* - Checks the the the group type based on the groupID.
	* - Checks if the user who access this endpoint already joined the specific group.
	* - Returns status field, for whether the user JOINED or REQUESTED to join the specific group.
	* 
	* - More Info:
	*   - User cannot join to the group with PRIVATE type.
	*   - Joining PUBLIC group type will create join request and need approval.
	*       from the privileged group member to accept the request to become the member.
	*   - Joining OPEN group type will make this user become member of that group immediately.
	* 
	* Action code: 73403
	* 
	* @param GroupId of the group you want to join.
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void JoinGroup(
		const FString& GroupId,
		const FJoinGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Cancel the Join group request.
	* - Required valid user authentication.
	* 
	* Action code: 73411
	* 
	* @param GroupId of the !Open group type you asked to join, but now want to cancel.
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void CancelJoinGroupRequest(
        const FString& GroupId,
        const FCancelJoinGroupRequestSuccess& OnSuccess,
        const FDErrorHandler& OnError);

	/**
	* @brief Get list of group members (by GroupId).
	* - Required valid user authentication.
	* 
	* Action code: 73410
	* 
	* @param GroupId of the group you want to get a members list from.
	* @param RequestContent
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void GetGroupMembersListByGroupId(
		const FString& GroupId,
		const FAccelByteModelsGetGroupMembersListByGroupIdRequest& RequestContent,
		const FGetGroupMembersListByGroupIdSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Leave the group you're currently in.
	* - Required valid user authentication.
	* - Admin is not allowed to leave the group.
	* - Will also give response if user does not belong to any group.
	* - Admin is not allowed to leave the group.
	*   - If an Admin wants to leave the group, see DeleteGroup.
	* - Still gives response if the user does not belong to any group.
	* 
	* Action code: 73404
	* 
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void LeaveGroup(
		const FLeaveGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);
	
	/**
	 * @brief Get list of group members by group id.
	 * - Required valid user auth.
	 * - get user group information by UserId.
	 * - If user !belongs to any group, a warning will return.
	 *
	 * - Group Member Status:
	 *   - JOIN: status of user requested to join group.
	 *   - INVITE: status of user invited to a group.
	 *   - JOINED: status of user already joined to a group.
	 * 
	 * Action code:: 73405
	 *
	 * @param UserId of the selected user; you want want to get this user's list of group members.
	 * @param OnSuccess Called upon successful op.
	 * @param OnError Called upon failed op.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void GetUserGroupInfoByUserId(
		const FString& UserId,
		const FGetUserGroupInfoByUserIdSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Invite the other user to your group.
	 * - Required valid user auth.
	 * - Required Member Role Permission: "GROUP:INVITE [CREATE]".
	 * - If specific user already has the join request, response will notify they have a pending accept / reject.
	 * - Invited user will receive notification through lobby.
	 * 
	 * Action code:: 73406
	 *
	 * @param UserId of the user you want to invite to your group.
	 * @param OnSuccess Called upon successful op.
	 * @param OnError Called upon failed op.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void InviteUserToGroup(
		const FString UserId,
		const FInviteUserToGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Accept [other] user's group join request.
	* - Required valid user auth.
	* - Required Member Role Permission: "GROUP:JOIN [CREATE]".
	* - If specific user was not asked to join this specific group,
	*     response will return error that they "need a join request".
	* - Will also check if specific user *already* joined the specific group.
	* - This also works when a user invites themselves (JoinGroup) to join a PUBLIC group.
	*     - Rather than an OPEN group (where players can just join without permission).
	* 
	* Action code: 73407
	*
	* @param UserId of the user who wants to join your group.
	* @param OnSuccess Called upon successful op.
	* - Returns { GroupId, UserId }
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void AcceptGroupJoinRequest(
		const FString UserId,
		const FAcceptGroupJoinRequestSuccess& OnSuccess,
		const FDErrorHandler& OnError);
	
	/**
	* @brief Reject [other] user's group join request.
	* - Required valid user auth.
	* - Required Member Role Permission: "GROUP:JOIN [CREATE]".
	* - If specific user was not asked to join this specific group,
	*     response will return error that they "need a join request".
	* - Will also check if specific user *already* joined the specific group.
	* - This also works when a user invites themselves (JoinGroup) to join a PUBLIC group.
	*     - Rrather than an OPEN group players can just join.
	* 
	* Action code: 73408
	*
	* @param UserId of the user you do NOT want to join your group.
	* @param OnSuccess Called upon successful op.
	* - Returns { GroupId, UserId }
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void RejectGroupJoinRequest(
		const FString UserId,
		const FRejectGroupJoinRequestSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Kick a group member out of the group.
	* - Required valid user auth.
	* - Required Member Role Permission: "GROUP:KICK [CREATE]".
	* - Validates the kicker's: member, group info and role perms.
	* 
	* Action code: 73409
	*
	* @param UserId of the user you want to kick from your group.
	* @param OnSuccess Called upon successful op.
	* - Returns { GroupId, KickedUserId }
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void KickGroupMember(
		const FString UserId,
		const FKickGroupMemberSuccess& OnSuccess,
		const FDErrorHandler& OnError);
	#pragma endregion /Group Member (individuals)

	#pragma region Group Roles (permissions)
	/**
	* @brief Get list of [group] member roles.
	* - Required Member Role Permission: "GROUP:ROLE [READ]".
	* 
	* Action code: 73201
	*
	* @param RequestContent { Limit=1, Offset=0 }
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void GetMemberRoles(
		const FAccelByteModelsLimitOffsetRequest& RequestContent,
		const FGetMemberRolesSuccess& OnSuccess,
		const FDErrorHandler& OnError);
	
	/**
	* @brief Assign a role to a group member.
	* - AKA AddMemberRole.
	* - Required Member Role Permission: "GROUP:ROLE [UPDATE]".
	* 
	* Action code: 73204
	*
	* @param MemberRoleId of the role you want to assign.
	* @param RequestContent { UserId } of the user you want to assign the role to.
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void AssignMemberRole(
		const FString& MemberRoleId,
		const FAccelByteModelsUserIdWrapper& RequestContent,
		const FAssignMemberRoleSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Remove a role from a group member.
	* - AKA RevokeMemberRole, RemoveMemberRole.
	* - Required Member Role Permission: "GROUP:ROLE [UPDATE]".
	* 
	* Action code: 73204
	*
	* @param MemberRoleId of the role you want to delete.
	* @param RequestContent { UserId } of the user you want to delete the role from.
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void DeleteMemberRole(
		const FString& MemberRoleId,
		const FAccelByteModelsUserIdWrapper& RequestContent,
		const FDeleteMemberRoleSuccess& OnSuccess,
		const FDErrorHandler& OnError);
	#pragma endregion /Group Roles (permissions)

	#pragma region Group Member Request
	/**
	 * @brief Get list of join requests in a specific group.
	 * - Required valid user authentication.
	 * - Required Member Role Permission: "GROUP:JOIN [READ]".
	 * - Group members needs to have permission (+belong to the group) to access this endpoint.
	 * 
	 * Action code:: 73501
	 * 
	 * @param GroupId of the group you want to see the join requests from.
	 * @param RequestContent { Limit=1, Offset=0 } for pagination.
	 * @param OnSuccess Paginated.
	 * @param OnError 
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group")
	void GetGroupJoinRequests(
		const FString& GroupId,
		const FAccelByteModelsLimitOffsetRequest& RequestContent,
		const FGetGroupJoinRequestsSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Get group invitation request list for the user calling this endpoint.
	 * - Required valid user authentication.
	 * - Checks any group invitation for this user.
	 *
	 * Action code:: 73502
	 *
	 * @param RequestContent { Limit=1, Offset=0 } for pagination.
	 * @param OnSuccess Paginated.
	 * @param OnError
	 */
	void GetGroupInvitationRequests(
		const FAccelByteModelsLimitOffsetRequest& RequestContent,
		const FGetGroupInvitationRequestsSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	#pragma endregion /Group MemberRequest
#pragma endregion Group V1	

#pragma region Group V2

	#pragma region Group V2 (multi-member actions)

	/**
	 * @brief Creates a new group.
	 *
	 * - Requires valid user authentication.
	 *
	 * @param RequestContent New group detail request.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGroupInformationResponse&.
	 * @param OnError This will be called when the operation failed.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void CreateV2Group(
		const FAccelByteModelsCreateGroupRequest& RequestContent,
		const FCreateGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Gets a collection of group information.
	 * - Requires valid user authentication
	 *
	 * @param GroupIds A list of group ids to get the group information from.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGetGroupListResponse&. Support paging.
	 * @param OnError This will be called when the operation failed.
	 */
	UFUNCTION(BlueprintCallable, Category= "AccelByte | Group | Api")
	void GetGroupsByGroupIds(
		const TArray<FString> GroupIds,
		const FGetGroupsByGroupIdsSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Update existing group.
	 * - Required valid user authentication.
	 * - Required Member Role Permission: "GROUP [UPDATE]".
	 * - Checks the group ID of the user based on the access token and
	 *     compares it with the group ID in path parameter.
	 * - Checks the member role of the user based on the access token.
	 * - Supports partial update (empty elements will convert to null)
	 * 
	 * @param GroupId
	 * @param RequestContent
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGroupInformationResponse&.
	 * @param OnError This will be called when the operation failed.
	 */
	UFUNCTION(BlueprintCallable, Category= "AccelByte | Group | Api")
	void UpdateV2Group(
		const FString& GroupId,
		const FAccelByteModelsUpdateGroupRequest& RequestContent,
		const FUpdateGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Delete existing group.
	 * - Checks if groupID exists before deleting.
	 * - Required Member Role Permission: "GROUP [DELETE]".
	 * 
	 * @param GroupId 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGroupInformationResponse&.
	 * @param OnError This will be called when the operation failed.
	 */
	UFUNCTION(BlueprintCallable, Category= "AccelByte | Group | Api")
	void DeleteV2Group(
		const FString& GroupId,
		const FDeleteGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Replaces current group custom attributes entirely.
	 * - Requires valid user authentication.
	 * - Required Member Role Permission: "GROUP [UPDATE]".
	 * - Checks group ID of the user based on the access token and compares with group ID in path param.
	 * - Checks the member role of the user based on the access token.
	 *   
	 * @param GroupId 
	 * @param RequestContent Arbitrary {}.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGroupInformationResponse&.
	 * @param OnError This will be called when the operation failed.
	 */
	UFUNCTION(BlueprintCallable, Category= "AccelByte | Group | Api")
	void UpdateV2GroupCustomAttributes(
		const FString& GroupId,
		const FAccelByteModelsUpdateGroupCustomAttributesRequest& RequestContent,
		const FUpdateGroupCustomAttributesSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Update custom (arbitrary) group rule.
	 * - Required valid user authentication.
	 * - Checks group ID of the user (via access token) and compares with group ID in path param.
	 * - Checks member role of the user based on access token.
	 * 
	 * @param GroupId
	 * @param RequestContent Arbitrary {}.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGroupInformationResponse&.
	 * @param OnError Called upon failed op.
	 */
	UFUNCTION(BlueprintCallable, Category= "AccelByte | Group | Api")
	void UpdateV2GroupCustomRule(
			const FString& GroupId,
			const FAccelByteModelsUpdateCustomRulesRequest& RequestContent,
			const FUpdateGroupCustomRuleSuccess& OnSuccess,
			const FDErrorHandler& OnError);

	/**
	 * @brief Update predefined group rule.
	 * - Required valid user authentication.
	 * - Required Member Role Permission: "GROUP [UPDATE]".
	 * - Checks group ID of the user (via access token) and compares with group ID in path param.
	 * - Checks member role of the user based on access token.
	 * - If rule action is not defined in the group, it will be added immediately.
	 * 
	 * @param GroupId 
	 * @param AllowedAction eg: createGroup, joinedGroup.
	 * @param RequestContent RuleDetail { RuleAttribute, RuleCriteria, RuleValue }.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is const FAccelByteModelsGroupInformationResponse&.
	 * @param OnError Called upon failed op.
	 */
	UFUNCTION(BlueprintCallable, Category= "AccelByte | Group | Api")
	void UpdateV2GroupPredefinedRule(
		const FString& GroupId,
		const EAccelByteAllowedAction& AllowedAction,
		const FAccelByteModelsUpdateGroupPredefinedRuleRequest& RequestContent,
		const FUpdateGroupPredefinedRuleSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Delete predefined group rule, based on the allowed action.
	 * - Required valid user authentication.
	 * - Required Member Role Permission: "GROUP [UPDATE]".
	 * - Checks the group ID of the user based on the access token, then
	 *    compares it with the group ID in path parameter.
	 * - Checks the member role of the user based on the access token.
	 * 
	 * @param GroupId 
	 * @param AllowedAction eg: createGroup, joinedGroup.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Void Result
	 * @param OnError Called upon failed op.
	 */
	UFUNCTION(BlueprintCallable, Category= "AccelByte | Group | Api")
	void DeleteV2GroupPredefinedRule(
			const FString& GroupId,
			const EAccelByteAllowedAction& AllowedAction,
			const FDeleteGroupPredefinedRuleSuccess& OnSuccess,
			const FDErrorHandler& OnError);

	/**
	 * @brief Gets user group status information.
	 * - Required valid user authentication
	 * - Required Member Role Permission: "GROUP [READ]"
	 * 
	 * @param UserId 
	 * @param GroupId The group id the user belongs to.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is FAccelByteModelsGetUserGroupInfoResponse&.
	 * @param OnError This will be called when the operation fail.
	 */
	UFUNCTION(BlueprintCallable, Category= "AccelByte | Group | Api")
	void GetUserGroupStatusInfo(
		const FString& UserId,
		const FString& GroupId,
		const FGetUserGroupInfoSuccess& OnSuccess,
		const FDErrorHandler OnError);

	/**
	 * @brief Get Group Invite Request List for specific group. Group members needs to have permission and also belong to the group to access this endpoint
	 * - Required valid user authentication
	 * - Required Member Role Permission: "GROUP:JOIN [READ]"
	 * 
	 * @param GroupId 
	 * @param RequestContent { Limit=1, Offset=0 } for pagination.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is FAccelByteModelsGetMemberRequestsListResponse.
	 * @param OnError This will be called when the operation fail.
	 */
	UFUNCTION(BlueprintCallable, Category= "AccelByte | Group | Api")
	void GetGroupInviteRequestList(
		const FString& GroupId,
		const FAccelByteModelsLimitOffsetRequest& RequestContent,
		const FGetGroupInvitationRequestsSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Get Group Join Request List for specific group. Group members needs to have permission and also belong to the group to access this endpoint
	 * - Required valid user authentication
	 * - Required Member Role Permission: "GROUP:JOIN [READ]"
	 * 
	 * @param GroupId 
	 * @param RequestContent { Limit=1, Offset=0 } for pagination.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is FAccelByteModelsGetMemberRequestsListResponse
	 * @param OnError This will be called when the operation fail.
	 */
	UFUNCTION(BlueprintCallable, Category= "AccelByte | Group | Api")
	void GetGroupJoinRequestList(
			const FString& GroupId,
			const FAccelByteModelsLimitOffsetRequest& RequestContent,
			const FGetGroupJoinRequestsSuccess& OnSuccess,
			const FDErrorHandler& OnError);

	/**
	 * @brief Cancel invitation to specific user
	 * - Required valid user authentication
	 * - Required Member Role Permission: "GROUP:INVITE [DELETE]"
	 * 
	 * @param UserId user id to which the invitation will be cancelled.
	 * @param GroupId group id which sent the invitation.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * - Result is FAccelByteModelsMemberRequestGroupResponse
	 * @param OnError This will be called when the operation fail.
	 */
	UFUNCTION(BlueprintCallable, Category= "AccelByte | Group | Api")
	void CancelGroupMemberInvitation(
			const FString& UserId,
			const FString& GroupId,
			const FCanceGroupInviteRequestSuccess& OnSuccess,
			const FDErrorHandler& OnError);
	
	#pragma endregion Group V2 (multi-member actions)

	#pragma region Group V2 Member (individuals)

	/**
	* @brief Accepts an invitation from a 3rd-party group's group member to group up.
	*
	* - Required valid user authentication.
	* - If specific user is !invited in the specific group ID, throw errorif the user !invited yet.
	* - Checks if the user who access this endpoint already joined the specific group.
	* - Deletes all requests (invite / join request) for the user who accesses this endpoint.
	* - Existing members will receive notification of the newly-accepted member.
	* 
	* @param GroupId of the group that invited you, that you are accepting the invite to.
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void AcceptV2GroupInvitation(
		const FString& GroupId,
		const FAcceptGroupInvitationSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Rejects an invitation from a 3rd-party group's group member to group up.
	* - Required valid user authentication.
	* - If specific user is !invited in the specific group ID, throw errorif the user !invited yet.
	* - Checks if the user who access this endpoint already joined the specific group.
	* 
	* @param GroupId of the group that invited you, that you are rejecting the invite from.
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void RejectV2GroupInvitation(
		const FString& GroupId,
		const FRejectGroupInvitationSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Join into specific group and become a group member.
	* - Required valid user authentication.
	* - Checks the the the group type based on the groupID.
	* - Checks if the user who access this endpoint already joined the specific group.
	* - Returns status field, for whether the user JOINED or REQUESTED to join the specific group.
	* 
	* - More Info:
	*   - User cannot join to the group with PRIVATE type.
	*   - Joining PUBLIC group type will create join request and need approval.
	*       from the privileged group member to accept the request to become the member.
	*   - Joining OPEN group type will make this user become member of that group immediately.
	* 
	* Action code: 73403
	*
	* @param GroupId of the group you want to join.
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void JoinV2Group(
		const FString& GroupId,
		const FJoinGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Leave the group you're currently in.
	* - Required valid user authentication.
	* - Admin is not allowed to leave the group.
	* - Will also give response if user does not belong to any group.
	* - Admin is not allowed to leave the group.
	*   - If an Admin wants to leave the group, see DeleteGroup.
	* - Still gives response if the user does not belong to any group.
	* 
	* @param GroupId of the group you want to leave.
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void LeaveV2Group(
		const FString& GroupId,
		const FLeaveGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Invite the other user to your group.
	 * - Required valid user auth.
	 * - Required Member Role Permission: "GROUP:INVITE [CREATE]".
	 * - If specific user already has the join request, response will notify they have a pending accept / reject.
	 * - Invited user will receive notification through lobby.
	 *
	 * @param UserId of the user you want to invite to your group.
	 * @param GroupId of the group.
	 * @param OnSuccess Called upon successful op.
	 * @param OnError Called upon failed op.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void InviteUserToV2Group(
		const FString& UserId,
		const FString& GroupId,
		const FInviteUserToGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Accept [other] user's group join request.
	* - Required valid user auth.
	* - Required Member Role Permission: "GROUP:JOIN [CREATE]".
	* - If specific user was not asked to join this specific group,
	*     response will return error that they "need a join request".
	* - Will also check if specific user *already* joined the specific group.
	* - This also works when a user invites themselves (JoinGroup) to join a PUBLIC group.
	*     - Rather than an OPEN group (where players can just join without permission).
	* 
	* @param UserId of the user who wants to join your group.
	* @param GroupId of the group.
	* @param OnSuccess Called upon successful op.
	* - Returns { GroupId, UserId }
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void AcceptV2GroupJoinRequest(
		const FString& UserId,
		const FString& GroupId,
		const FAcceptGroupJoinRequestSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Reject [other] user's group join request.
	* - Required valid user auth.
	* - Required Member Role Permission: "GROUP:JOIN [CREATE]".
	* - If specific user was not asked to join this specific group,
	*     response will return error that they "need a join request".
	* - Will also check if specific user *already* joined the specific group.
	* - This also works when a user invites themselves (JoinGroup) to join a PUBLIC group.
	*     - Rrather than an OPEN group players can just join.
	* 
	* @param UserId of the user you do NOT want to join your group.
	* @param GroupId of the group.
	* @param OnSuccess Called upon successful op.
	* - Returns { GroupId, UserId }
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void RejectV2GroupJoinRequest(
		const FString& UserId,
		const FString& GroupId,
		const FRejectGroupJoinRequestSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Kick a group member out of the group.
	* - Required valid user auth.
	* - Required Member Role Permission: "GROUP:KICK [CREATE]".
	* - Validates the kicker's: member, group info and role perms.
	*
	* @param UserId of the user you want to kick from your group.
	* @param OnSuccess Called upon successful op.
	* - Returns { GroupId, KickedUserId }
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void KickV2GroupMember(
		const FString& UserId,
		const FString& GroupId,
		const FKickGroupMemberSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	 * @brief Get user group joined information. If user does not belong to any group, it will return warning to give information about it
	 * - Required valid user authentication
	 * 
	 * @param RequestContent { Limit=1, Offset=0 } for pagination.
	 * @param OnSuccess This will be called when the operation is succeeded.
	 * @param OnError This will be called when the operation is fail.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void GetMyJoinedGroupInfo(
			const FAccelByteModelsLimitOffsetRequest& RequestContent,
			const FGetMyJoinedGroupInfoSuccess& OnSuccess,
			const FDErrorHandler& OnError);

	/**
	 * @brief Get My Join Request To The Groups. It will check any join request group for this user
	 * - Required valid user authentication
	 * 
	 * @param RequestContent { Limit=1, Offset=0 } for pagination.
	 * @param OnSuccess This will be called when the operation is succeeded.
	 * - Result is FAccelByteModelsGetGroupMemberListResponse
	 * @param OnError This will be called when the operation is fail.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void GetMyJoinGroupRequest(const FAccelByteModelsLimitOffsetRequest& RequestContent,
		const FGetMyJoinGroupRequestSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	#pragma endregion Group V2 Member (individuals)

	#pragma region Group V2 Roles (Permission)

	/**
	* @brief Assign a role to a group member.
	* - AKA AddMemberRole.
	* - Required Member Role Permission: "GROUP:ROLE [UPDATE]".
	* 
	* @param MemberRoleId of the role you want to assign.
	* @param GroupId of the member.
	* @param RequestContent { UserId } of the user and group you want to assign the role to.
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void AssignV2MemberRole(
		const FString& MemberRoleId,
		const FString& GroupId,
		const FAccelByteModelsUserIdWrapper& RequestContent,
		const FAssignMemberRoleSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	/**
	* @brief Remove a role from a group member.
	* - AKA RevokeMemberRole, RemoveMemberRole.
	* - Required Member Role Permission: "GROUP:ROLE [UPDATE]".
	* 
	* @param MemberRoleId of the role you want to delete.
	* @param GroupId of the member.
	* @param RequestContent { UserId } of the user you want to delete the role from.
	* @param OnSuccess Called upon successful op.
	* @param OnError Called upon failed op.
	*/
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void DeleteV2MemberRole(
		const FString& MemberRoleId,
		const FString& GroupId,
		const FAccelByteModelsUserIdWrapper& RequestContent,
		const FDeleteMemberRoleSuccess& OnSuccess,
		const FDErrorHandler& OnError);
	
	/**
	 * @brief Get list of member roles
	 * @param RequestContent { Limit=1, Offset=0 } for pagination.
	 * @param OnSuccess This will be called when the operation is succeeded.
	 * - Result is FAccelByteModelsGetMemberRolesListResponse
	 * @param OnError This will be called when the operation is fail.
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	void GetAllMemberRoles(
			const FAccelByteModelsLimitOffsetRequest& RequestContent,
			const FGetMemberRolesSuccess& OnSuccess,
			const FDErrorHandler& OnError);

	#pragma endregion Group V2 Roles (Permission)

#pragma endregion Group V2

private:
	AccelByte::FApiClientPtr ApiClientPtr;
};
