// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
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
	: HttpRef{InHttpRef}
	, CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
{}

Reporting::~Reporting()
{}

void Reporting::SubmitReport(const FAccelByteModelsReportingSubmitData ReportData, const THandler<FAccelByteModelsReportingSubmitResponse>& OnSuccess, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/reports"), *SettingsRef.ReportingServerUrl, *CredentialsRef.GetNamespace());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FJsonObjectConverter::UStructToJsonObjectString(ReportData, Content);
	
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
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Reporting::GetReasons(const FString & ReasonGroup, int32 const& Offset, int32 const& Limit, const THandler<FAccelByteModelsReasonsResponse>& OnSuccess, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/reasons"), *SettingsRef.ReportingServerUrl, *SettingsRef.Namespace);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FString Query = TEXT("");
	if (!ReasonGroup.IsEmpty())
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("group=%s"), *ReasonGroup));
	}
	if (Offset > 0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("offset=%d"), Offset));
	}
	if (Limit > 0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("limit=%d"), Limit));
	}

	Url.Append(Query.IsEmpty() ? TEXT("") : FString::Printf(TEXT("?%s"), *Query));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Reporting::GetReasonGroups(int32 const& Offset, int32 const& Limit, const THandler<FAccelByteModelsReasonGroupsResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/reasonGroups"), *SettingsRef.ReportingServerUrl, *SettingsRef.Namespace);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FString Query = TEXT("");
	if (Offset > 0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("offset=%d"), Offset));
	}
	if (Limit > 0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("limit=%d"), Limit));
	}

	Url.Append(Query.IsEmpty() ? TEXT("") : FString::Printf(TEXT("?%s"), *Query));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte
