// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteAgreementApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace Api
{
Agreement::Agreement(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Agreement::~Agreement()
{}

FAccelByteTaskWPtr Agreement::GetLegalPolicies(EAccelByteAgreementPolicyType const& AgreementPolicyType
	, bool DefaultOnEmpty
	, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	return GetLegalPolicies(*CredentialsRef->GetNamespace(), AgreementPolicyType, DefaultOnEmpty, OnSuccess, OnError);
}

FAccelByteTaskWPtr Agreement::GetLegalPolicies(FString const& Namespace
	, EAccelByteAgreementPolicyType const& AgreementPolicyType
	, bool DefaultOnEmpty
	, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString AgreementPolicyTypeString = ConvertAgreementPolicyType(AgreementPolicyType);

	const FString Url = FString::Printf(TEXT("%s/public/policies/namespaces/%s?policyType=%s&defaultOnEmpty=%s")
		, *SettingsRef.AgreementServerUrl
		, *Namespace
		, *AgreementPolicyTypeString
		, DefaultOnEmpty ? TEXT("true") : TEXT("false"));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Agreement::GetLegalPolicies(EAccelByteAgreementPolicyType const& AgreementPolicyType
	, TArray<FString> const& Tags
	, bool DefaultOnEmpty
	, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess
	, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	const FString AgreementPolicyTypeString = ConvertAgreementPolicyType(AgreementPolicyType);

	const FString TagsString = FAccelByteUtilities::CreateQueryParamValueUrlEncodedFromArray(Tags);

	const FString Url = FString::Printf(TEXT("%s/public/policies/namespaces/%s?policyType=%s&tags=%s&defaultOnEmpty=%s")
		, *SettingsRef.AgreementServerUrl 
		, *CredentialsRef->GetNamespace() 
		, *AgreementPolicyTypeString
		, *TagsString
		, DefaultOnEmpty ? TEXT("true") : TEXT("false"));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Agreement::GetLegalPoliciesByCountry(FString const& CountryCode
	, EAccelByteAgreementPolicyType const& AgreementPolicyType
	, bool DefaultOnEmpty
	, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess
	, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	const FString AgreementPolicyTypeString = ConvertAgreementPolicyType(AgreementPolicyType);

	const FString Url = FString::Printf(TEXT("%s/public/policies/countries/%s?policyType=%s&defaultOnEmpty=%s")
		, *SettingsRef.AgreementServerUrl 
		, *CountryCode
		, *AgreementPolicyTypeString
		, DefaultOnEmpty ? TEXT("true") : TEXT("false"));

	return HttpClient.Request(TEXT("GET"), Url, OnSuccess, OnError);
}

FAccelByteTaskWPtr Agreement::GetLegalPoliciesByCountry(FString const& CountryCode
	, EAccelByteAgreementPolicyType const& AgreementPolicyType
	, TArray<FString> const& Tags
	, bool DefaultOnEmpty
	, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess
	, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	const FString AgreementPolicyTypeString = ConvertAgreementPolicyType(AgreementPolicyType);

	const FString TagsString = FAccelByteUtilities::CreateQueryParamValueUrlEncodedFromArray(Tags);

	const FString Url = FString::Printf(TEXT("%s/public/policies/countries/%s?policyType=%s&tags=%s&defaultOnEmpty=%s")
		, *SettingsRef.AgreementServerUrl 
		, *CountryCode
		, *AgreementPolicyTypeString
		, *TagsString
		, DefaultOnEmpty ? TEXT("true") : TEXT("false"));

	return HttpClient.Request(TEXT("GET"), Url, OnSuccess, OnError);
}

FAccelByteTaskWPtr Agreement::BulkAcceptPolicyVersions(TArray<FAccelByteModelsAcceptAgreementRequest> const& AgreementRequests
	, THandler<FAccelByteModelsAcceptAgreementResponse> const& OnSuccess
	, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/agreements/policies")
		, *SettingsRef.AgreementServerUrl);

	FString Content;
	FAccelByteUtilities::TArrayUStructToJsonString(AgreementRequests, Content);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Agreement::AcceptPolicyVersion(FString const& LocalizedPolicyVersionId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/agreements/localized-policy-versions/%s")
		, *SettingsRef.AgreementServerUrl
		, *LocalizedPolicyVersionId);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Agreement::ChangePolicyPreferences(TArray<FAccelByteModelsChangeAgreementRequest> const& ChangeAgreementRequests
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChangeAgreementRequests.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Cannot change preference for current consent. Request is empty."));
		return nullptr;
	}

	// Validate each request to make sure they are not failing later.
	for (const FAccelByteModelsAcceptAgreementRequest& ChangeAgreement : ChangeAgreementRequests)
	{
		if (ChangeAgreement.PolicyVersionId.IsEmpty())
		{
			OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Cannot change preference for current consent. Policy version id is empty."));
			return nullptr;
		}
		if (ChangeAgreement.LocalizedPolicyVersionId.IsEmpty())
		{
			OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Cannot change preference for current consent. Localized policy id is empty."));
			return nullptr;
		}
	}

	const FString Url = FString::Printf(TEXT("%s/public/agreements/localized-policy-versions/preferences")
		, *SettingsRef.AgreementServerUrl);

	FString Content;
	FAccelByteUtilities::TArrayUStructToJsonString(ChangeAgreementRequests, Content);

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Agreement::QueryLegalEligibilities(FString const& Namespace
	, THandler<TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/eligibilities/namespaces/%s")
		, *SettingsRef.AgreementServerUrl
		, *Namespace);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Agreement::GetLegalDocument(FString const& Url
	, THandler<FString> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("*/*")}
	};

	return HttpClient.Request(TEXT("GET"), Url, OnSuccess, OnError);
}

FString Agreement::ConvertAgreementPolicyType(EAccelByteAgreementPolicyType const& AgreementPolicyType)
{
	switch (AgreementPolicyType)
	{
	case EAccelByteAgreementPolicyType::EMPTY:
		return  TEXT("");
	case EAccelByteAgreementPolicyType::LEGAL_DOCUMENT_TYPE:
		return TEXT("LEGAL_DOCUMENT_TYPE");
	case EAccelByteAgreementPolicyType::MARKETING_PREFERENCE_TYPE:
		return TEXT("MARKETING_PREFERENCE_TYPE");
	default:
		return TEXT("");
	}
}

} // Namespace Api
} // Namespace AccelByte
