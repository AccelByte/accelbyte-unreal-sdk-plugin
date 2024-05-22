// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteReportingApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
Reporting::Reporting(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Reporting::~Reporting()
{}

FAccelByteTaskWPtr Reporting::SubmitReport(FAccelByteModelsReportingSubmitData const& ReportData
	, THandler<FAccelByteModelsReportingSubmitResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ReportData.ObjectId.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("ObjectID is Empty, You Should Fill it with UUID v4 without hyphen format"));
		return nullptr;
	}

	if (ReportData.ObjectId.Contains("-"))
	{
		OnError.ExecuteIfBound(404, TEXT("ObjectId doesn't follow the UUID V4 without hyphen format, You Should Fill it with UUID v4 without hyphen format"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/reports")
		, *SettingsRef.ReportingServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, ReportData, OnSuccess, OnError);
}

FAccelByteTaskWPtr Reporting::SubmitChatReport(FAccelByteModelsReportingSubmitDataChat const& ReportData
	, THandler<FAccelByteModelsReportingSubmitResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	if (ReportData.ChatId.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Chat report failed! ChatId cannot be empty."));
		return nullptr;
	}
	if (ReportData.UserId.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Chat report failed! UserId cannot be empty."));
		return nullptr;
	}
	if (ReportData.ChatTopicId.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Chat report failed! ChatTopicId cannot be empty."));
		return nullptr;
	}
	
	FAccelByteModelsReportingSubmitDataChatRequest Request;
	Request.Category = EAccelByteReportingCategory::CHAT;
	Request.Comment = ReportData.Comment;
	Request.Reason = ReportData.Reason;
	Request.ObjectId = ReportData.ChatId;
	Request.ObjectType = "chat";
	Request.UserID = ReportData.UserId;

	FAccelByteModelsReportingAdditionalInfoChat Additions;
	Additions.TopicId = ReportData.ChatTopicId;
	Additions.ChatCreatedAt = ReportData.ChatCreatedAt.ToIso8601();

	Request.AdditionalInfo = Additions;

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/reports")
		, *SettingsRef.ReportingServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr Reporting::GetReasons(FString const& ReasonGroup
	, int32 Offset
	, int32 Limit
	, THandler<FAccelByteModelsReasonsResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& Title)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/reasons")
		, *SettingsRef.ReportingServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("group"), ReasonGroup },
		{ TEXT("offset"), FString::Printf(TEXT("%d"), Offset) },
		{ TEXT("limit"), Limit > 0 ? FString::Printf(TEXT("%d"), Limit) : TEXT("") },
		{ TEXT("title"), Title },
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Reporting::GetReasonGroups(int32 Offset
	, int32 Limit
	, THandler<FAccelByteModelsReasonGroupsResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/reasonGroups")
		, *SettingsRef.ReportingServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("offset"),FString::FromInt(Offset) },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
