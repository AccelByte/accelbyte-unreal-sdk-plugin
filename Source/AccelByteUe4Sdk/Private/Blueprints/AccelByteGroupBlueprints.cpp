// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
// DEPRECATED: In favor of Blueprints/ABGroup.cpp

#include "Blueprints/AccelByteGroupBlueprints.h"
#include "Api/AccelByteGroupApi.h"
#include "Core/AccelByteRegistry.h"

using Api::Group;
using AccelByte::FErrorHandler;
using AccelByte::THandler;

void UAccelByteBlueprintsGroup::CreateGroup(
	const FAccelByteModelsCreateGroupRequest& RequestContent,
	const FCreateGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	FRegistry::Group.CreateGroup(
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

void UAccelByteBlueprintsGroup::GetGroupList(
	const FAccelByteModelsGetGroupListRequest& RequestContent,
	const FSearchGroupsSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	FRegistry::Group.GetGroupList(
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

void UAccelByteBlueprintsGroup::GetGroup(
	const FString& GroupId,
	const FGetGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	FRegistry::Group.GetGroup(
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

void UAccelByteBlueprintsGroup::UpdateGroup(
	const FString& GroupId,
	const bool bCompletelyReplace,
	const FAccelByteModelsGroupUpdatable RequestContent,
	const FUpdateGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	FRegistry::Group.UpdateGroup(
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

void UAccelByteBlueprintsGroup:: UpdateGroupCustomAttributes(
	const FString& GroupId,
	const FAccelByteModelsUpdateGroupCustomAttributesRequest& RequestContent,
	const FUpdateGroupCustomAttributesSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	FRegistry::Group.UpdateGroupCustomAttributes(
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

void UAccelByteBlueprintsGroup::DeleteGroup(
	const FString& GroupId,
	const FDeleteGroupSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	FRegistry::Group.DeleteGroup(
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

void UAccelByteBlueprintsGroup::UpdateGroupCustomRule(
	const FString& GroupId,
	const FAccelByteModelsUpdateCustomRulesRequest& RequestContent,
	const FUpdateGroupCustomRuleSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	FRegistry::Group.UpdateGroupCustomRule(
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

void UAccelByteBlueprintsGroup::UpdateGroupPredefinedRule(
	const FString& GroupId,
	const EAccelByteAllowedAction& AllowedAction,
	const FAccelByteModelsUpdateGroupPredefinedRuleRequest& RequestContent,
	const FUpdateGroupPredefinedRuleSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	FRegistry::Group.UpdateGroupPredefinedRule(
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

void UAccelByteBlueprintsGroup::DeleteGroupPredefinedRule(
	const FString& GroupId,
	const EAccelByteAllowedAction& AllowedAction,
	const FDeleteGroupPredefinedRuleSuccess& OnSuccess,
	const FDErrorHandler& OnError)
{
	FRegistry::Group.DeleteGroupPredefinedRule(
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
