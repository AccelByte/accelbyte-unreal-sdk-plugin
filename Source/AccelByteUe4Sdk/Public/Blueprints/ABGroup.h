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

UCLASS(Blueprintable, BlueprintType)
class UABGroup final : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(const FApiClientPtr NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Group")
	void CreateGroup(
		const FAccelByteModelsCreateGroupRequest& RequestContent,
		const FCreateGroupSuccess& OnSuccess,
		const FDErrorHandler& OnError);

private:
	FApiClientPtr ApiClientPtr;
};
