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

void Reporting::SubmitReport(const FAccelByteModelsReportingSubmitData ReportData
	, const THandler<FAccelByteModelsReportingSubmitResponse>& OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if(ReportData.ObjectId.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("ObjectID is Empty, You Should Fill it with UUID v4 without hyphen format"));
		return;
	}

	if(ReportData.ObjectId.Contains("-"))
	{
		OnError.ExecuteIfBound(404, TEXT("ObjectId doesn't follow the UUID V4 without hyphen format, You Should Fill it with UUID v4 without hyphen format"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/reports")
		, *SettingsRef.ReportingServerUrl
		, *CredentialsRef.GetNamespace());

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, ReportData, OnSuccess, OnError);
}

void Reporting::SubmitChatReport(const FAccelByteModelsReportingSubmitDataChat& ReportData
	, const THandler<FAccelByteModelsReportingSubmitResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	if(ReportData.ChatId.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Chat report failed! ChatId cannot be empty."));
		return;
	}
	if(ReportData.UserId.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Chat report failed! UserId cannot be empty."));
		return;
	}
	if(ReportData.ChatTopicId.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Chat report failed! ChatTopicId cannot be empty."));
		return;
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
	, *CredentialsRef.GetNamespace());

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

void Reporting::GetReasons(const FString& ReasonGroup
	, int32 const& Offset
	, int32 const& Limit
	, const THandler<FAccelByteModelsReasonsResponse>& OnSuccess
	, const FErrorHandler & OnError
	, FString const& Title)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/reasons")
		, *SettingsRef.ReportingServerUrl
		, *CredentialsRef.GetNamespace());

	const TMap<FString, FString> QueryParams = {
		{ TEXT("group"), ReasonGroup },
		{ TEXT("offset"), FString::Printf(TEXT("%d"), Offset) },
		{ TEXT("limit"), Limit > 0 ? FString::Printf(TEXT("%d"), Limit) : TEXT("") },
		{ TEXT("title"), Title },
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

void Reporting::GetReasonGroups(int32 const& Offset
	, int32 const& Limit
	, const THandler<FAccelByteModelsReasonGroupsResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/reasonGroups")
		, *SettingsRef.ReportingServerUrl
		, *CredentialsRef.GetNamespace());

	const TMap<FString, FString> QueryParams = {
		{ TEXT("offset"),FString::Printf(TEXT("%d"), Offset) },
		{ TEXT("limit"), Limit > 0 ? FString::Printf(TEXT("%d"), Limit) : TEXT("") },
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
