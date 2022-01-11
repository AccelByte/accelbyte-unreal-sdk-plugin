// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "Models/AccelByteGroupModels.h"
#include "Core/AccelByteMultiRegistry.h"

using namespace AccelByte;
using namespace AccelByte::Api;
using namespace AccelByte::GameServerApi;

DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateGroupSuccess, const FAccelByteModelsGroupInformation&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSearchGroupsSuccess, const FAccelByteModelsGetGroupListResponse&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FGetGroupSuccess, const FAccelByteModelsGroupInformation&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGroupSuccess, const FAccelByteModelsGroupInformation&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGroupCustomAttributesSuccess, const FAccelByteModelsGroupInformation&, Response);
DECLARE_DYNAMIC_DELEGATE(FDeleteGroupSuccess);
DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGroupCustomRuleSuccess, const FAccelByteModelsGroupInformation&, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGroupPredefinedRuleSuccess, const FAccelByteModelsGroupInformation&, Response);
DECLARE_DYNAMIC_DELEGATE(FDeleteGroupPredefinedRuleSuccess);


/** @brief Provide APIs to access Group [Management] service.
 * - While authed, Namespace is automatically passed to all Requests.
 */
UCLASS(Blueprintable, BlueprintType)
class UABGroup final : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(const FApiClientPtr NewApiClientPtr);

	
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
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group")
	void GetGroupList(
		const FAccelByteModelsGetGroupListRequest& RequestContent,
		const FSearchGroupsSuccess& OnSuccess,
		const FDErrorHandler& OnError) const;
	
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
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group")
	void CreateGroup(
		const FAccelByteModelsCreateGroupRequest& RequestContent,
		const FCreateGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError) const;

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
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group")
	void GetGroup(
		const FString& GroupId,
		const FGetGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError) const;

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
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group")
	void UpdateGroup(
		const FString& GroupId,
		const bool bCompletelyReplace,
		const FAccelByteModelsGroupUpdatable RequestContent,
		const FUpdateGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError) const;

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
	DECLARE_DYNAMIC_DELEGATE(FDeleteGroupSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group")
	void DeleteGroup(
		const FString& GroupId,
		const FDeleteGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError) const;

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
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group")
	void UpdateGroupCustomAttributes(
		const FString& GroupId,
		const FAccelByteModelsUpdateGroupCustomAttributesRequest& RequestContent,
		const FUpdateGroupCustomAttributesSuccess& OnSuccess,
		const FDErrorHandler& OnError) const;

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
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group")
	void UpdateGroupCustomRule(
		const FString& GroupId,
		const FAccelByteModelsUpdateCustomRulesRequest& RequestContent,
		const FUpdateGroupCustomRuleSuccess& OnSuccess,
		const FDErrorHandler& OnError) const;

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
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group")
	void UpdateGroupPredefinedRule(
		const FString& GroupId,
		const EAccelByteAllowedAction& AllowedAction,
		const FAccelByteModelsUpdateGroupPredefinedRuleRequest& RequestContent,
		const FUpdateGroupPredefinedRuleSuccess& OnSuccess,
		const FDErrorHandler& OnError) const;

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
	DECLARE_DYNAMIC_DELEGATE(FDeleteGroupPredefinedRuleSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group")
	void DeleteGroupPredefinedRule(
		const FString& GroupId,
		const EAccelByteAllowedAction& AllowedAction,
		const FDeleteGroupPredefinedRuleSuccess& OnSuccess,
		const FDErrorHandler& OnError) const;
	#pragma endregion /Group (multi-member actions)

	
private:
	FApiClientPtr ApiClientPtr;
};
