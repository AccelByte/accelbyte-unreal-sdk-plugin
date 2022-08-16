// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteGroupApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
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
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
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


void Group::CreateGroup(
	const FAccelByteModelsCreateGroupRequest& RequestContent,
	const THandler<FAccelByteModelsGroupInformation>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups"),
		*SettingsRef.GroupServerUrl);

	HttpClient.ApiRequest("POST", Url, {}, RequestContent, OnSuccess, OnError);
}

void Group::GetGroupList(
	const FAccelByteModelsGetGroupListRequest& RequestContent,
	const THandler<FAccelByteModelsGetGroupListResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups"),
		*SettingsRef.GroupServerUrl);

	// Convert request info to a JSON obj, so we can null out empty "" strings.
	FString ContentBody = TEXT("");
	const TSharedPtr<FJsonObject> JsonObj = FJsonObjectConverter::UStructToJsonObject(RequestContent);
	FAccelByteUtilities::RemoveEmptyStrings(JsonObj);
	
	const TMap<FString, FString> QueryParams
	{
		{ "groupName", RequestContent.GroupName },
		{ "groupRegion", RequestContent.GroupRegion },
		{ "limit", FString::FromInt(RequestContent.Limit) },
		{ "offset", FString::FromInt(RequestContent.Offset) }
	};

	HttpClient.ApiRequest("GET", Url, QueryParams, FString(), OnSuccess, OnError);
}

void Group::GetGroup(
	const FString& GroupId,
	const THandler<FAccelByteModelsGroupInformation>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s"),
		*SettingsRef.GroupServerUrl,
		*FPlatformHttp::UrlEncode(GroupId));
	
	HttpClient.ApiRequest("GET", Url, {}, FString(), OnSuccess, OnError);
}

void Group::UpdateGroup(
	const FString& GroupId,
	const bool bCompletelyReplace,
	const FAccelByteModelsGroupUpdatable& RequestContent,
	const THandler<FAccelByteModelsGroupInformation>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s"),
		*SettingsRef.GroupServerUrl,
		*GroupId);

	// Convert request info to a JSON obj, so we can null out empty "" strings.
	FString ContentBody = TEXT("");
	const TSharedPtr<FJsonObject> JsonObj = FJsonObjectConverter::UStructToJsonObject(RequestContent);
	FAccelByteUtilities::RemoveEmptyStrings(JsonObj);

	// Serialize request info to ContentBody, so we can attach to an HTTP Request
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ContentBody);
	FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);

	const FString Verb = bCompletelyReplace ? "PUT" : "PATCH";
	HttpClient.ApiRequest(Verb, Url, {}, ContentBody, OnSuccess, OnError);
}

void Group::UpdateGroupCustomAttributes(
	const FString& GroupId,
	const FAccelByteModelsUpdateGroupCustomAttributesRequest& RequestContent,
	const THandler<FAccelByteModelsGroupInformation>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/attributes/custom"),
		*SettingsRef.GroupServerUrl,
		*GroupId);

	HttpClient.ApiRequest("PUT", Url, {}, RequestContent, OnSuccess, OnError);
}
	
void Group::DeleteGroup(
	const FString& GroupId,
	const FVoidHandler& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s"),
		*SettingsRef.GroupServerUrl,
		*GroupId);

	HttpClient.ApiRequest("DELETE", Url, {}, FString(), OnSuccess, OnError);
}

void Group::UpdateGroupCustomRule(
	const FString& GroupId,
	const FAccelByteModelsUpdateCustomRulesRequest& RequestContent,
	const THandler<FAccelByteModelsGroupInformation>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/rules/custom"),
		*SettingsRef.GroupServerUrl,
		*GroupId);

	HttpClient.ApiRequest("PUT", Url, {}, RequestContent, OnSuccess, OnError);
}
	
void Group::UpdateGroupPredefinedRule(
	const FString& GroupId,
	const EAccelByteAllowedAction& AllowedAction,
	const FAccelByteModelsUpdateGroupPredefinedRuleRequest& RequestContent,
	const THandler<FAccelByteModelsGroupInformation>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString AllowedActionStr = ConvertGroupAllowedActionToString(AllowedAction);
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/rules/defined/%s"),
		*SettingsRef.GroupServerUrl,
		*GroupId,
		*AllowedActionStr);

	HttpClient.ApiRequest("PUT", Url, {}, RequestContent, OnSuccess, OnError);
}
	
void Group::DeleteGroupPredefinedRule(
	const FString& GroupId,
	const EAccelByteAllowedAction& AllowedAction,
	const FVoidHandler& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString AllowedActionStr = ConvertGroupAllowedActionToString(AllowedAction);
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/rules/defined/%s"),
		*SettingsRef.GroupServerUrl,
		*GroupId,
		*AllowedActionStr);

	HttpClient.ApiRequest("DELETE", Url, {}, FString(), OnSuccess, OnError);
}
	
void Group::GetUserGroupInfoByUserId(
	const FString& UserId,
	const THandler<FAccelByteModelsGetUserGroupInfoResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/users/%s"),
		*SettingsRef.GroupServerUrl,
		*UserId);

	HttpClient.ApiRequest("GET", Url, {}, FString(), OnSuccess, OnError);
}
	
void Group::InviteUserToGroup(
	const FString UserId,
	const THandler<FAccelByteModelsMemberRequestGroupResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/users/%s/invite"),
		*SettingsRef.GroupServerUrl,
		*UserId);

	HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}
	
void Group::AcceptGroupJoinRequest(
	const FString UserId,
	const THandler<FAccelByteModelsMemberRequestGroupResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/users/%s/join/accept"),
		*SettingsRef.GroupServerUrl,
		*UserId);

	HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}
	
void Group::RejectGroupJoinRequest(
	const FString UserId,
	const THandler<FAccelByteModelsMemberRequestGroupResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/users/%s/join/reject"),
		*SettingsRef.GroupServerUrl,
		*UserId);

	HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}
	
void Group::KickGroupMember(
	const FString UserId,
	const THandler<FAccelByteModelsKickGroupMemberResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/users/%s/kick"),
		*SettingsRef.GroupServerUrl,
		*UserId);

	HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}
	
void Group::GetMemberRoles(
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const THandler<FAccelByteModelsGetMemberRolesListResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/roles"),
		*SettingsRef.GroupServerUrl);

	const TMap<FString, FString> QueryParams
	{
		{ "limit", FString::FromInt(RequestContent.Limit) },
		{ "offset", FString::FromInt(RequestContent.Offset) },
	};

	HttpClient.ApiRequest("GET", Url, QueryParams, FString(), OnSuccess, OnError);
}
	
void Group::AssignMemberRole(
	const FString& MemberRoleId,
	const FAccelByteModelsUserIdWrapper& RequestContent,
	const THandler<FAccelByteModelsGetUserGroupInfoResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/roles/%s/members"),
		*SettingsRef.GroupServerUrl,
		*MemberRoleId);

	HttpClient.ApiRequest("POST", Url, {}, RequestContent, OnSuccess, OnError);
}
	
void Group::DeleteMemberRole(
	const FString& MemberRoleId,
	const FAccelByteModelsUserIdWrapper& RequestContent,
	const FVoidHandler& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/roles/%s/members"),
		*SettingsRef.GroupServerUrl,
		*MemberRoleId);

	HttpClient.ApiRequest("DELETE", Url, {}, RequestContent, OnSuccess, OnError);
}
	
void Group::GetGroupJoinRequests(
	const FString& GroupId,
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const THandler<FAccelByteModelsGetMemberRequestsListResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/join/request"),
		*SettingsRef.GroupServerUrl,
		*GroupId);

	const TMap<FString, FString> QueryParams
    {
    	{ "limit", FString::FromInt(RequestContent.Limit) },
    	{ "offset", FString::FromInt(RequestContent.Offset) },
    };

	HttpClient.ApiRequest("GET", Url, QueryParams, FString(), OnSuccess, OnError);
}
	
void Group::GetGroupInvitationRequests(
	const FAccelByteModelsLimitOffsetRequest& RequestContent,
	const THandler<FAccelByteModelsGetMemberRequestsListResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/users/me/invite/request"),
		*SettingsRef.GroupServerUrl);

	const TMap<FString, FString> QueryParams
	{
    		{ "limit", FString::FromInt(RequestContent.Limit) },
			{ "offset", FString::FromInt(RequestContent.Offset) },
		};

	HttpClient.ApiRequest("GET", Url, QueryParams, FString(), OnSuccess, OnError);
}

void Group::AcceptGroupInvitation(
	const FString& GroupId,
	const THandler<FAccelByteModelsMemberRequestGroupResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/invite/accept"),
		*SettingsRef.GroupServerUrl,
		*GroupId);

	HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}
	
void Group::RejectGroupInvitation(
	const FString& GroupId,
	const THandler<FAccelByteModelsMemberRequestGroupResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/invite/reject"),
		*SettingsRef.GroupServerUrl,
		*GroupId);

	HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}
	
void Group::JoinGroup(
	const FString& GroupId,
	const THandler<FAccelByteModelsJoinGroupResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/join"),
		*SettingsRef.GroupServerUrl,
		*GroupId);

	HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}
	
void Group::CancelJoinGroupRequest(
	const FString& GroupId, 
	const THandler<FAccelByteModelsMemberRequestGroupResponse>& OnSuccess, 
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/join/cancel"),
		*SettingsRef.GroupServerUrl,
		*GroupId);

	HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}

void Group::GetGroupMembersListByGroupId(
	const FString& GroupId,
	const FAccelByteModelsGetGroupMembersListByGroupIdRequest& RequestContent,
	const THandler<FAccelByteModelsGetGroupMemberListResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/groups/%s/members"),
		*SettingsRef.GroupServerUrl,
		*GroupId);

	FString Order = (RequestContent.SortBy == EAccelByteGroupListSortBy::DESCENDING) ? TEXT("desc") : TEXT("asc");
	const TMap<FString, FString> QueryParams
	{
		{ "limit", FString::FromInt(RequestContent.Limit) },
		{ "offset", FString::FromInt(RequestContent.Offset) },
		{ "order",  Order}
	};

	HttpClient.ApiRequest("GET", Url, QueryParams, FString(), OnSuccess, OnError);
}

void Group::LeaveGroup(
	const THandler<FAccelByteModelsMemberRequestGroupResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/leave"),
		*SettingsRef.GroupServerUrl);

	HttpClient.ApiRequest("POST", Url, {}, FString(), OnSuccess, OnError);
}
	
} // Namespace Api
} // Namespace AccelByte
