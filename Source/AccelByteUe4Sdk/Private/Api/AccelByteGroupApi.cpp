// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteGroupApi.h"
#include "Core/AccelByteError.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace Api
{

Group::Group(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{}

Group::~Group()
{}

#pragma region Utils
FString Group::ConvertGroupAllowedActionToString(const EAccelByteAllowedAction& AllowedAction)
{
	switch (AllowedAction)
	{
		case EAccelByteAllowedAction::createGroup:
			return TEXT("createGroup");
		case EAccelByteAllowedAction::joinGroup:
			return TEXT("joinGroup");
		case EAccelByteAllowedAction::inviteUser:
			return TEXT("inviteUser");
		case EAccelByteAllowedAction::kickUser:
			return TEXT("kickUser");
		case EAccelByteAllowedAction::leaveGroup:
			return TEXT("leaveGroup");
		case EAccelByteAllowedAction::responseJoinRequest:
			return TEXT("responseJoinRequest");
			
		default:
			return TEXT("None");
	}
}
#pragma endregion /Utils


FAccelByteTaskWPtr Group::CreateGroup(FAccelByteModelsCreateGroupRequest const& RequestContent
	, THandler<FAccelByteModelsGroupInformation> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups")
		, *SettingsRef.GroupServerUrl);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestContent, OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::GetGroupList(FAccelByteModelsGetGroupListRequest const& RequestContent
	, THandler<FAccelByteModelsGetGroupListResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups")
		, *SettingsRef.GroupServerUrl);

	// Convert request info to a JSON obj, so we can null out empty "" strings.
	FString ContentBody = TEXT("");
	const TSharedPtr<FJsonObject> JsonObj = FJsonObjectConverter::UStructToJsonObject(RequestContent);
	FAccelByteUtilities::RemoveEmptyStrings(JsonObj);
	
	const TMultiMap<FString, FString> QueryParams
	{
		{ "groupName", RequestContent.GroupName },
		{ "groupRegion", RequestContent.GroupRegion },
		{ "limit", FString::FromInt(RequestContent.Limit) },
		{ "offset", FString::FromInt(RequestContent.Offset) }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::GetGroup(FString const& GroupId
	, THandler<FAccelByteModelsGroupInformation> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s")
		, *SettingsRef.GroupServerUrl
		, *FPlatformHttp::UrlEncode(GroupId));
	
	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::UpdateGroup(FString const& GroupId
	, bool bCompletelyReplace
	, FAccelByteModelsGroupUpdatable const& RequestContent
	, THandler<FAccelByteModelsGroupInformation> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	// Convert request info to a JSON obj, so we can null out empty "" strings.
	FString ContentBody = TEXT("");
	const TSharedPtr<FJsonObject> JsonObj = FJsonObjectConverter::UStructToJsonObject(RequestContent);
	FAccelByteUtilities::RemoveEmptyStrings(JsonObj);

	// Serialize request info to ContentBody, so we can attach to an HTTP Request
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ContentBody);
	FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);

	const FString Verb = bCompletelyReplace ? "PUT" : "PATCH";
	return HttpClient.ApiRequest(Verb, Url, {}, ContentBody, OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::UpdateGroupCustomAttributes(FString const& GroupId
	, FAccelByteModelsUpdateGroupCustomAttributesRequest const& RequestContent
	, THandler<FAccelByteModelsGroupInformation> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/attributes/custom")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, RequestContent, OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::DeleteGroup(FString const& GroupId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::UpdateGroupCustomRule(FString const& GroupId
	, FAccelByteModelsUpdateCustomRulesRequest const& RequestContent
	, THandler<FAccelByteModelsGroupInformation> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/rules/custom")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, RequestContent, OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::UpdateGroupPredefinedRule(FString const& GroupId
	, EAccelByteAllowedAction AllowedAction
	, FAccelByteModelsUpdateGroupPredefinedRuleRequest const& RequestContent
	, THandler<FAccelByteModelsGroupInformation> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString AllowedActionStr = ConvertGroupAllowedActionToString(AllowedAction);
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/rules/defined/%s")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(GroupId)
		, *AllowedActionStr);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, RequestContent, OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::DeleteGroupPredefinedRule(FString const& GroupId
	, EAccelByteAllowedAction AllowedAction
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString AllowedActionStr = ConvertGroupAllowedActionToString(AllowedAction);
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/rules/defined/%s")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(GroupId)
		, *AllowedActionStr);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::GetUserGroupInfoByUserId(FString const& UserId
	, THandler<FAccelByteModelsGetUserGroupInfoResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/users/%s")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::InviteUserToGroup(FString const& UserId
	, THandler<FAccelByteModelsMemberRequestGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/users/%s/invite")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::AcceptGroupJoinRequest(FString const& UserId
	, THandler<FAccelByteModelsMemberRequestGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/users/%s/join/accept")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::RejectGroupJoinRequest(FString const& UserId
	, THandler<FAccelByteModelsMemberRequestGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/users/%s/join/reject")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::KickGroupMember(FString const& UserId
	, THandler<FAccelByteModelsKickGroupMemberResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/users/%s/kick")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::GetMemberRoles(FAccelByteModelsLimitOffsetRequest const& RequestContent
	, THandler<FAccelByteModelsGetMemberRolesListResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/roles")
		, *SettingsRef.GroupServerUrl);

	const TMultiMap<FString, FString> QueryParams
	{
		{ "limit", FString::FromInt(RequestContent.Limit) },
		{ "offset", FString::FromInt(RequestContent.Offset) },
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::AssignMemberRole(FString const& MemberRoleId
	, FAccelByteModelsUserIdWrapper const& RequestContent
	, THandler<FAccelByteModelsGetUserGroupInfoResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/roles/%s/members")
		, *SettingsRef.GroupServerUrl
		, *MemberRoleId);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, RequestContent, OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::DeleteMemberRole(FString const& MemberRoleId
	, FAccelByteModelsUserIdWrapper const& RequestContent
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/roles/%s/members")
		, *SettingsRef.GroupServerUrl
		, *MemberRoleId);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, RequestContent, OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::GetGroupJoinRequests(FString const& GroupId
	, FAccelByteModelsLimitOffsetRequest const& RequestContent
	, THandler<FAccelByteModelsGetMemberRequestsListResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/join/request")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	const TMultiMap<FString, FString> QueryParams
    {
    	{ "limit", FString::FromInt(RequestContent.Limit) },
    	{ "offset", FString::FromInt(RequestContent.Offset) },
    };

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::GetGroupInvitationRequests(FAccelByteModelsLimitOffsetRequest const& RequestContent
	, THandler<FAccelByteModelsGetMemberRequestsListResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/users/me/invite/request")
		, *SettingsRef.GroupServerUrl);

	const TMultiMap<FString, FString> QueryParams
	{
    	{ "limit", FString::FromInt(RequestContent.Limit) },
		{ "offset", FString::FromInt(RequestContent.Offset) },
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::AcceptGroupInvitation(FString const& GroupId
	, THandler<FAccelByteModelsMemberRequestGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/invite/accept")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::RejectGroupInvitation(FString const& GroupId
	, THandler<FAccelByteModelsMemberRequestGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/invite/reject")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::JoinGroup(FString const& GroupId
	, THandler<FAccelByteModelsJoinGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/join")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::CancelJoinGroupRequest(FString const& GroupId 
	, const THandler<FAccelByteModelsMemberRequestGroupResponse>& OnSuccess 
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/join/cancel")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::GetGroupMembersListByGroupId(FString const& GroupId
	, FAccelByteModelsGetGroupMembersListByGroupIdRequest const& RequestContent
	, THandler<FAccelByteModelsGetGroupMemberListResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/members")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	FString Order = (RequestContent.SortBy == EAccelByteGroupListSortBy::DESCENDING) ? TEXT("desc") : TEXT("asc");
	const TMultiMap<FString, FString> QueryParams
	{
		{ "limit", FString::FromInt(RequestContent.Limit) },
		{ "offset", FString::FromInt(RequestContent.Offset) },
		{ "order",  Order}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::LeaveGroup(THandler<FAccelByteModelsMemberRequestGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/leave")
		, *SettingsRef.GroupServerUrl);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::CreateV2Group(FAccelByteModelsCreateGroupRequest const& RequestContent
	, THandler<FAccelByteModelsGroupInformation> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	return HttpClient.ApiRequest("POST", Url, {}, RequestContent, OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::AcceptV2GroupInvitation(FString const& GroupId
	, THandler<FAccelByteModelsMemberRequestGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups/%s/invite/accept")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::RejectV2GroupInvitation(FString const& GroupId
	, THandler<FAccelByteModelsMemberRequestGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups/%s/invite/reject")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::JoinV2Group(FString const& GroupId
	, THandler<FAccelByteModelsJoinGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups/%s/join")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::LeaveV2Group(FString const& GroupId
	, THandler<FAccelByteModelsMemberRequestGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups/%s/leave")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::InviteUserToV2Group(FString const& UserId
	, FString const& GroupId
	, THandler<FAccelByteModelsMemberRequestGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/groups/%s/invite")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::AcceptV2GroupJoinRequest(FString const& UserId
	, FString const& GroupId
	, THandler<FAccelByteModelsMemberRequestGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/groups/%s/join/accept")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Group::RejectV2GroupJoinRequest(FString const& UserId
	, FString const& GroupId
	, THandler<FAccelByteModelsMemberRequestGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/groups/%s/join/reject")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::KickV2GroupMember(FString const& UserId
	, FString const& GroupId
	, THandler<FAccelByteModelsKickGroupMemberResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/groups/%s/kick")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::AssignV2MemberRole(FString const& MemberRoleId
	, FString const& GroupId
	, FAccelByteModelsUserIdWrapper const& RequestContent
	, THandler<FAccelByteModelsGetUserGroupInfoResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/roles/%s/groups/%s/members")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(MemberRoleId)
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("POST", Url, {}, RequestContent, OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::DeleteV2MemberRole(FString const& MemberRoleId
	, FString const& GroupId
	, FAccelByteModelsUserIdWrapper const& RequestContent
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/roles/%s/groups/%s/members")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(MemberRoleId)
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("DELETE", Url, {}, RequestContent, OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::GetGroupsByGroupIds(TArray<FString> const& GroupIds
	, THandler<FAccelByteModelsGetGroupListResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups/bulk")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	auto Request = FAccelByteModelsGetGroupsByGroupIdsRequest{GroupIds};
	FString Content = FString("");
	FJsonObjectConverter::UStructToJsonObjectString(Request, Content);

	return HttpClient.ApiRequest("POST",Url, {},Content,OnSuccess,OnError);
}

FAccelByteTaskWPtr Group::UpdateV2Group(FString const& GroupId
	, FAccelByteModelsUpdateGroupRequest const& RequestContent
	, THandler<FAccelByteModelsGroupInformation> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups/%s")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("PATCH",Url, {}, RequestContent, OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::DeleteV2Group(FString const& GroupId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups/%s")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("DELETE",Url,{},OnSuccess,OnError);
}

FAccelByteTaskWPtr Group::UpdateV2GroupCustomAttributes(FString const& GroupId
	, FAccelByteModelsUpdateGroupCustomAttributesRequest const& RequestContent
	, THandler<FAccelByteModelsGroupInformation> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups/%s/attributes/custom")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("PUT", Url, {}, RequestContent,OnSuccess,OnError);
}

FAccelByteTaskWPtr Group::UpdateV2GroupCustomRule(FString const& GroupId
	, FAccelByteModelsUpdateCustomRulesRequest const& RequestContent
	, THandler<FAccelByteModelsGroupInformation> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups/%s/rules/custom")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("PUT", Url, {}, RequestContent,OnSuccess,OnError);
}

FAccelByteTaskWPtr Group::UpdateV2GroupPredefinedRule(FString const& GroupId
	, EAccelByteAllowedAction AllowedAction
	, FAccelByteModelsUpdateGroupPredefinedRuleRequest const& RequestContent
	, THandler<FAccelByteModelsGroupInformation> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString AllowedActionStr = ConvertGroupAllowedActionToString(AllowedAction);
	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups/%s/rules/defined/%s")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(GroupId)
		, *FGenericPlatformHttp::UrlEncode(AllowedActionStr));

	return HttpClient.ApiRequest("PUT",Url,{},RequestContent,OnSuccess,OnError);
}

FAccelByteTaskWPtr Group::DeleteV2GroupPredefinedRule(FString const& GroupId
	, EAccelByteAllowedAction AllowedAction
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString AllowedActionStr = ConvertGroupAllowedActionToString(AllowedAction);
	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups/%s/rules/defined/%s")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(GroupId)
		, *FGenericPlatformHttp::UrlEncode(AllowedActionStr));

	return HttpClient.ApiRequest("DELETE",Url, {},OnSuccess,OnError);
}

FAccelByteTaskWPtr Group::GetUserGroupStatusInfo(FString const& UserId
	, FString const& GroupId
	, THandler<FAccelByteModelsGetUserGroupInfoResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/groups/%s/status")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("GET",Url, {},OnSuccess,OnError);
}

FAccelByteTaskWPtr Group::GetMyJoinedGroupInfo(FAccelByteModelsLimitOffsetRequest const& RequestContent
	, THandler<FAccelByteModelsGetGroupMemberListResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/me/groups")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));
	const TMultiMap<FString, FString> QueryParams
	{
			{ "limit", FString::FromInt(RequestContent.Limit) },
			{ "offset", FString::FromInt(RequestContent.Offset) }
	};

	return HttpClient.ApiRequest("GET", Url,QueryParams,OnSuccess,OnError);
}

FAccelByteTaskWPtr Group::GetMyJoinGroupRequest(FAccelByteModelsLimitOffsetRequest const& RequestContent
	, THandler<FAccelByteModelsGetMemberRequestsListResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/me/join/request")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	const TMultiMap<FString,FString> QueryParams
	{
		{"limit", FString::FromInt(RequestContent.Limit)},
		{"offset", FString::FromInt(RequestContent.Offset)}
	};

	return HttpClient.ApiRequest("GET", Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::GetGroupInviteRequestList(FString const& GroupId
	, FAccelByteModelsLimitOffsetRequest const& RequestContent
	, THandler<FAccelByteModelsGetMemberRequestsListResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups/%s/invite/request")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	const TMultiMap<FString,FString> QueryParams
	{
		{"limit", FString::FromInt(RequestContent.Limit)},
		{"offset", FString::FromInt(RequestContent.Offset)}
	};

	return HttpClient.ApiRequest("GET", Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::GetGroupJoinRequestList(FString const& GroupId
	, FAccelByteModelsLimitOffsetRequest const& RequestContent
	, THandler<FAccelByteModelsGetMemberRequestsListResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/groups/%s/join/request")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	const TMultiMap<FString,FString> QueryParams
	{
		{"limit", FString::FromInt(RequestContent.Limit)},
		{"offset", FString::FromInt(RequestContent.Offset)}
	};

	return HttpClient.ApiRequest("GET", Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::CancelGroupMemberInvitation(FString const& UserId
	, FString const& GroupId
	, THandler<FAccelByteModelsMemberRequestGroupResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/groups/%s/invite/cancel")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(GroupId));

	return HttpClient.ApiRequest("POST", Url, {}, OnSuccess, OnError);
}

FAccelByteTaskWPtr Group::GetAllMemberRoles(FAccelByteModelsLimitOffsetRequest const& RequestContent
	, THandler<FAccelByteModelsGetMemberRolesListResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/roles")
		, *SettingsRef.GroupServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	const TMultiMap<FString,FString> QueryParams
	{
		{"limit", FString::FromInt(RequestContent.Limit)},
		{"offset", FString::FromInt(RequestContent.Offset)}
	};

	return HttpClient.ApiRequest("GET", Url, QueryParams, OnSuccess, OnError);
}
	
} // Namespace Api
} // Namespace AccelByte
