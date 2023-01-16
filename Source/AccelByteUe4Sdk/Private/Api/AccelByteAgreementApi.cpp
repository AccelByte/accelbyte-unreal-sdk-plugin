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

void Agreement::GetLegalPolicies(EAccelByteAgreementPolicyType const& AgreementPolicyType
	, bool DefaultOnEmpty
	, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	GetLegalPolicies(*CredentialsRef.GetNamespace(), AgreementPolicyType, DefaultOnEmpty, OnSuccess, OnError);
}

void Agreement::GetLegalPolicies(FString const& Namespace
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

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Agreement::GetLegalPolicies(EAccelByteAgreementPolicyType const& AgreementPolicyType
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
		, *CredentialsRef.GetNamespace() 
		, *AgreementPolicyTypeString
		, *TagsString
		, DefaultOnEmpty ? TEXT("true") : TEXT("false"));

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Agreement::GetLegalPoliciesByCountry(FString const& CountryCode
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

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Agreement::GetLegalPoliciesByCountry(FString const& CountryCode
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

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Agreement::BulkAcceptPolicyVersions(TArray<FAccelByteModelsAcceptAgreementRequest> const& AgreementRequests
	, THandler<FAccelByteModelsAcceptAgreementResponse> const& OnSuccess
	, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/agreements/policies")
		, *SettingsRef.AgreementServerUrl);

	FString Content;
	FAccelByteUtilities::TArrayUStructToJsonString(AgreementRequests, Content);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void Agreement::AcceptPolicyVersion(FString const& LocalizedPolicyVersionId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/agreements/localized-policy-versions/%s")
		, *SettingsRef.AgreementServerUrl
		, *LocalizedPolicyVersionId);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

void Agreement::QueryLegalEligibilities(FString const& Namespace
	, THandler<TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/eligibilities/namespaces/%s")
		, *SettingsRef.AgreementServerUrl
		, *Namespace);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Agreement::GetLegalDocument(FString const& Url
	, THandler<FString> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("*/*")}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
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
