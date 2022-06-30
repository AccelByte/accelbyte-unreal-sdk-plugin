// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonUtilities.h"
#include "Models/AccelByteGeneralModels.h"
#include "AccelByteGroupModels.generated.h"

#pragma region Shared Models

UENUM(BlueprintType)
enum class EAccelByteGroupListSortBy : uint8
{
	/** @brief Default == ascending. Can change to "desc". Anything != "desc" will remain default */
	ASCENDING = 0,
	DESCENDING
};

/** @brief For CreateGroup request: Only joinGroup || inviteUser are accepted. */
UENUM(BlueprintType)
enum class EAccelByteAllowedAction : uint8
{
	None = 0, // TODO: is this intentionally PascalCase instead of camelCase?
	createGroup,
	joinGroup,
	inviteUser,
	kickUser,
	leaveGroup,
	responseJoinRequest,
};

UENUM(BlueprintType)
enum class EAccelByteRuleCriteria : uint8
{
	MINIMUM,
	MAXIMUM,
	EQUAL,
};

UENUM(BlueprintType)
enum class EAccelByteGroupType : uint8
{
	NONE = 0,
	OPEN,
	PUBLIC,
	PRIVATE,
};

UENUM(BlueprintType)
enum class EAccelByteGroupRequestType : uint8
{
	NONE = 0,
	INVITE,
	JOIN,
	JOINED
};

/**
 * @brief a TArray element from FAccelByteModelsGetMemberRequestsListResponse.Data[].
 * Shared with:
 * - GetGroupJoinRequest
 * - GetGroupInvitationRequest
 */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMemberRequestResponse
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsMemberRequestResponse")
	FString GroupId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsMemberRequestResponse")
	FString UserId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsMemberRequestResponse")
	EAccelByteGroupRequestType RequestType{};
};

/**
 * @brief Shared request for:
 * - AssignRoleToMemberRequest
 * - DeleteRoleRequest
 */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserIdWrapper
{
	GENERATED_BODY()
	
	/** @brief The UserId you want to assign the role to. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsUserIdWrapper")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRuleInformation
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsRuleInformation")
	FString RuleAttribute{};
	
	/** @brief The value will be in enum of EQUAL, MINIMUM, MAXIMUM	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsRuleInformation")
	EAccelByteRuleCriteria RuleCriteria{EAccelByteRuleCriteria::MINIMUM};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsRuleInformation")
	float RuleValue{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRules
{
	GENERATED_BODY()
	
	/** @brief For CreateGroup request: Only joinGroup || inviteUser are accepted. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsRules")
	EAccelByteAllowedAction AllowedAction{EAccelByteAllowedAction::None};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsRules")
	TArray<FAccelByteModelsRuleInformation> RuleDetail{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupRules
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupRules")
	FJsonObjectWrapper GroupCustomRule{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupRules")
	TArray<FAccelByteModelsRules> GroupPredefinedRules{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupMember
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupMember")
	FString UserId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupMember")
	TArray<FString> MemberRoleId{};
};

/**
 * @brief Shared response for:
 * - GetUserGroupInfoByUserId
 * - AssignRoleToMember
 */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetUserGroupInfoResponse
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetUserGroupInfoResponse")
	FString GroupId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetUserGroupInfoResponse")
	TArray<FString> MemberRoleId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetUserGroupInfoResponse")
	FString Status{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetUserGroupInfoResponse")
	FString UserId{};
};

/**
 * @brief Shared request for:
 * - GetMemberRolesListRequest
 * - GetGroupRequestListRequest
 * - GetGroupMembersListByGroupIdRequest
 */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLimitOffsetRequest
{
	GENERATED_BODY()
	
	/** @brief Default == 1	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsLimitOffsetRequest")
	int32 Limit{1};

	/** @brief Default == 0 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsLimitOffsetRequest")
	int32 Offset{0};
};

/**
 * @brief Shared GroupResponseV1 for:
 * - CreateGroup,
 * - GetGroupList (partial),
 * - GetGroup
 */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupInformation
{	
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupInformation")
	FString ConfigurationCode{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupInformation")
	FJsonObjectWrapper CustomAttributes{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupInformation")
	FString GroupDescription{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupInformation")
	FString GroupIcon{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupInformation")
	FString GroupId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupInformation")
	int GroupMaxMember{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupInformation")
	TArray<FAccelByteModelsGroupMember> GroupMembers{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupInformation")
	FString GroupName{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupInformation")
	FString GroupRegion{};
	
	/**
	 * @brief Rules for specific group.
	 * - Consists of GroupCustomRule for creating arbitrary rules.
	 * - Consists of GroupPredefinedRules that has similar usage with config, only works in a specific group
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupInformation")
	FAccelByteModelsGroupRules GroupRules{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupInformation")
	EAccelByteGroupType GroupType{EAccelByteGroupType::NONE};
};

/**
 * @brief Shared model for updating a group (AKA 'UpdateCustomAttributesRequest') for:
 * - CreateGroup 
 * - UpdateGroup
 * 
 * - Any of these fields can be updated after created.
 * - Do not use this directly to update, or you may inadvertently clear fields (such as GroupName).
 */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupUpdatable
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupUpdatable")
	FString GroupName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupUpdatable")
	FString GroupRegion{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupUpdatable")
	EAccelByteGroupType GroupType{EAccelByteGroupType::NONE};

	/** @brief Optional */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupUpdatable")
	FJsonObjectWrapper CustomAttributes{};
	
	/** @brief Optional */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupUpdatable")
	FString GroupDescription{};
	
	/** @brief Optional */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGroupUpdatable")
	FString GroupIcon{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserGroupInformationResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsUserGroupInformationResponse")
	FString GroupId {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsUserGroupInformationResponse")
	FString UserId {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsUserGroupInformationResponse")
	FString Status {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsUserGroupInformationResponse")
	TArray<FString> MemberRoleId{};
};

/**
 * @brief Shared Model for group member requests for:
 * - GetGroupMembersList
 * - AcceptGroupInvitation
 * - RejectGroupInvitation
 * - AcceptGroupJoinRequest
 * - CancelGroupJoinRequest
 * - LeaveGroup
 * - InviteGroup
 */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMemberRequestGroupResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsMemberRequestGroupResponse")
	FString GroupId {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsMemberRequestGroupResponse")
	FString UserId {};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsJoinGroupResponse : public FAccelByteModelsMemberRequestGroupResponse
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsMemberRequestGroupResponse")
	FString Status{};
};

#pragma endregion /Shared Models


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsKickGroupMemberResponse
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsKickGroupMemberResponse")
	FString GroupId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsKickGroupMemberResponse")
	FString KickedUserId{};
};

/** @brief Request data includes UpdateGroupRequest */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCreateGroupRequest : public FAccelByteModelsGroupUpdatable
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsCreateGroupRequest")
	FString ConfigurationCode{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsCreateGroupRequest")
	int32 GroupMaxMember{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsCreateGroupRequest")
	FAccelByteModelsGroupRules GroupRules{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetGroupListRequest : public FAccelByteModelsLimitOffsetRequest
{
	GENERATED_BODY()
	
	/** @brief Default == "" (empty) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetGroupListRequest")
	FString GroupName{};

	/** @brief Default == "" (empty) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetGroupListRequest")
	FString GroupRegion{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetGroupMembersListByGroupIdRequest : public FAccelByteModelsLimitOffsetRequest
{
	GENERATED_BODY()

	/** @brief Default == ascending. Can change to "desc". Anything != "desc" will remain default */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetGroupMembersListByGroupIdRequest")
	EAccelByteGroupListSortBy SortBy {};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetGroupMemberListResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetGroupMemberListResponse")
	TArray<FAccelByteModelsUserGroupInformationResponse> data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetGroupMemberListResponse")
	FAccelByteModelsPaging Paging {};
};

/** @brief AKA "SearchGroupListResponse"{}; GroupInformation + Paging */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetGroupListResponse
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetGroupListResponse")
	TArray<FAccelByteModelsGroupInformation> data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetGroupListResponse")
	FAccelByteModelsPaging Paging{};
};

/** @brief Member Role Permission*/
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMemberRolePermission
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsMemberRolePermission")
	int Action = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsMemberRolePermission")
	FString ResourceName{};
};

/** @brief Member Role*/
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMemberRole
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsMemberRole")
	FString MemberRoleId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsMemberRole")
	FString MemberRoleName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsMemberRole")
	TArray<FAccelByteModelsMemberRolePermission> MemberRolePermissions{};
};

/** @brief Paginated */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetMemberRolesListResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetMemberRolesListResponse")
	FAccelByteModelsPaging Paging {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetMemberRolesListResponse")
	TArray<FAccelByteModelsMemberRole> Data{};
};

/**
 * @brief Accepts any arbitrary values
 * - Useful for: UpdateGroupCustomRule 
 */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateGroupCustomAttributesRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsUpdateGroupCustomAttributesRequest")
	FJsonObjectWrapper CustomAttributes{};
};

/** @brief For creating arbitrary values. */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateCustomRulesRequest
{
	GENERATED_BODY()
	
	/** @brief Completely arbitrary */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsUpdateCustomRulesRequest")
	FJsonObjectWrapper GroupCustomRule{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateGroupPredefinedRuleRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsUpdateGroupPredefinedRuleRequest")
	TArray<FAccelByteModelsRuleInformation> RuleDetail{};
};

/** @brief Contains paginated list of group member requests. */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetMemberRequestsListResponse
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetMemberRequestsListResponse")
	TArray<FAccelByteModelsMemberRequestResponse> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Group | Models | FAccelByteModelsGetMemberRequestsListResponse")
	FAccelByteModelsPaging Paging{};
};

/** @brief For HTTP requests that want empty data, but still want a USTRUCT */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEmptyRequest { GENERATED_BODY() };
