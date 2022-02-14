// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
// DEPRECATED: In favor of Blueprints/ABGroup.h

#pragma once

#include "CoreMinimal.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "Models/AccelByteGroupModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "AccelByteGroupBlueprints.generated.h"

using namespace AccelByte;
using namespace AccelByte::Api;
using namespace AccelByte::GameServerApi;

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsGroup : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateGroupSuccess, const FAccelByteModelsGroupInformation&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	static void CreateGroup(
		const FAccelByteModelsCreateGroupRequest& RequestContent,
		const FCreateGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);
	

	DECLARE_DYNAMIC_DELEGATE_OneParam(FSearchGroupsSuccess, const FAccelByteModelsGetGroupListResponse&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	static void GetGroupList(
		const FAccelByteModelsGetGroupListRequest& RequestContent,
		const FSearchGroupsSuccess& OnSuccess,
		const FDErrorHandler& OnError);
	

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetGroupSuccess, const FAccelByteModelsGroupInformation&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	static void GetGroup(
		const FString& GroupId,
		const FGetGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);
	

	DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGroupSuccess, const FAccelByteModelsGroupInformation&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	static void UpdateGroup(
		const FString& GroupId,
		const bool bCompletelyReplace,
		const FAccelByteModelsGroupUpdatable RequestContent,
		const FUpdateGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGroupCustomAttributesSuccess, const FAccelByteModelsGroupInformation&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	static void UpdateGroupCustomAttributes(
		const FString& GroupId,
		const FAccelByteModelsUpdateGroupCustomAttributesRequest& RequestContent,
		const FUpdateGroupCustomAttributesSuccess& OnSuccess,
		const FDErrorHandler& OnError);


	DECLARE_DYNAMIC_DELEGATE(FDeleteGroupSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	static void DeleteGroup(
		const FString& GroupId,
		const FDeleteGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	
	DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGroupCustomRuleSuccess, const FAccelByteModelsGroupInformation&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	static void UpdateGroupCustomRule(
		const FString& GroupId,
		const FAccelByteModelsUpdateCustomRulesRequest& RequestContent,
		const FUpdateGroupCustomRuleSuccess& OnSuccess,
		const FDErrorHandler& OnError);


	DECLARE_DYNAMIC_DELEGATE_OneParam(FUpdateGroupPredefinedRuleSuccess, const FAccelByteModelsGroupInformation&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	static void UpdateGroupPredefinedRule(
		const FString& GroupId,
		const EAccelByteAllowedAction& AllowedAction,
		const FAccelByteModelsUpdateGroupPredefinedRuleRequest& RequestContent,
		const FUpdateGroupPredefinedRuleSuccess& OnSuccess,
		const FDErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE(FDeleteGroupPredefinedRuleSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group | Api")
	static void DeleteGroupPredefinedRule(
		const FString& GroupId,
		const EAccelByteAllowedAction& AllowedAction,
		const FDeleteGroupPredefinedRuleSuccess& OnSuccess,
		const FDErrorHandler& OnError);
};
