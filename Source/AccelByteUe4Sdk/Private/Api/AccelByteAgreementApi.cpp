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
	: HttpRef{InHttpRef}
	, CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
{}

Agreement::~Agreement()
{}

void Agreement::GetLegalPolicies(EAccelByteAgreementPolicyType const& AgreementPolicyType, bool DefaultOnEmpty, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess, FErrorHandler const& OnError)
{
	GetLegalPolicies(*SettingsRef.Namespace, AgreementPolicyType, DefaultOnEmpty, OnSuccess, OnError);
}

void Agreement::GetLegalPolicies(FString const& Namespace, EAccelByteAgreementPolicyType const& AgreementPolicyType, bool DefaultOnEmpty, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString AgreementPolicyTypeString = "";
	switch (AgreementPolicyType)
	{
	case EAccelByteAgreementPolicyType::EMPTY:
		AgreementPolicyTypeString = "";
		break;
	case EAccelByteAgreementPolicyType::LEGAL_DOCUMENT_TYPE:
		AgreementPolicyTypeString = "LEGAL_DOCUMENT_TYPE";
		break;
	case EAccelByteAgreementPolicyType::MARKETING_PREFERENCE_TYPE:
		AgreementPolicyTypeString = "MARKETING_PREFERENCE_TYPE";
		break;
	default:
		break;
	}

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/policies/namespaces/%s?policyType=%s&defaultOnEmpty=%s"),
		*SettingsRef.AgreementServerUrl,
		*Namespace,
		*AgreementPolicyTypeString,
		DefaultOnEmpty ? TEXT("true") : TEXT("false"));
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::GetLegalPolicies(EAccelByteAgreementPolicyType const& AgreementPolicyType, TArray<FString> const& Tags, bool DefaultOnEmpty, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	FString AgreementPolicyTypeString = "";
	switch (AgreementPolicyType)
	{
	case EAccelByteAgreementPolicyType::EMPTY:
		AgreementPolicyTypeString = "";
		break;
	case EAccelByteAgreementPolicyType::LEGAL_DOCUMENT_TYPE:
		AgreementPolicyTypeString = "LEGAL_DOCUMENT_TYPE";
		break;
	case EAccelByteAgreementPolicyType::MARKETING_PREFERENCE_TYPE:
		AgreementPolicyTypeString = "MARKETING_PREFERENCE_TYPE";
		break;
	default:
		break;
	}

	FString TagsString = "";
	for (int i = 0 ; i < Tags.Num() ; i++)
	{
		TagsString.Append((i == 0) ? TEXT(""): TEXT(",")).Append(Tags[i]);
	}

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/policies/namespaces/%s?policyType=%s&tags=%s&defaultOnEmpty=%s"), 
		*SettingsRef.AgreementServerUrl, 
		*SettingsRef.Namespace, 
		*AgreementPolicyTypeString,
		*TagsString,
		DefaultOnEmpty ? TEXT("true") : TEXT("false"));
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::GetLegalPoliciesByCountry(FString const& CountryCode, EAccelByteAgreementPolicyType const& AgreementPolicyType,  bool DefaultOnEmpty, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	FString AgreementPolicyTypeString = "";
	switch (AgreementPolicyType)
	{
	case EAccelByteAgreementPolicyType::EMPTY:
		AgreementPolicyTypeString = "";
		break;
	case EAccelByteAgreementPolicyType::LEGAL_DOCUMENT_TYPE:
		AgreementPolicyTypeString = "LEGAL_DOCUMENT_TYPE";
		break;
	case EAccelByteAgreementPolicyType::MARKETING_PREFERENCE_TYPE:
		AgreementPolicyTypeString = "MARKETING_PREFERENCE_TYPE";
		break;
	default:
		break;
	}

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/policies/countries/%s?policyType=%s&defaultOnEmpty=%s"), 
		*SettingsRef.AgreementServerUrl, 
		*CountryCode,
		*AgreementPolicyTypeString,
		DefaultOnEmpty ? TEXT("true") : TEXT("false"));
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::GetLegalPoliciesByCountry(FString const& CountryCode, EAccelByteAgreementPolicyType const& AgreementPolicyType, TArray<FString> const& Tags,  bool DefaultOnEmpty, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	FString AgreementPolicyTypeString = "";
	switch (AgreementPolicyType)
	{
	case EAccelByteAgreementPolicyType::EMPTY:
		AgreementPolicyTypeString = "";
		break;
	case EAccelByteAgreementPolicyType::LEGAL_DOCUMENT_TYPE:
		AgreementPolicyTypeString = "LEGAL_DOCUMENT_TYPE";
		break;
	case EAccelByteAgreementPolicyType::MARKETING_PREFERENCE_TYPE:
		AgreementPolicyTypeString = "MARKETING_PREFERENCE_TYPE";
		break;
	default:
		break;
	}

	FString TagsString = "";
	for (int i = 0 ; i < Tags.Num() ; i++)
	{
		TagsString.Append((i == 0) ? TEXT(""): TEXT(",")).Append(Tags[i]);
	}

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/policies/countries/%s?policyType=%s&tags=%s&defaultOnEmpty=%s"), 
		*SettingsRef.AgreementServerUrl, 
		*CountryCode,
		*AgreementPolicyTypeString,
		*TagsString,
		DefaultOnEmpty ? TEXT("true") : TEXT("false"));
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::BulkAcceptPolicyVersions(TArray<FAccelByteModelsAcceptAgreementRequest> const& AgreementRequests, THandler<FAccelByteModelsAcceptAgreementResponse> const& OnSuccess, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/agreements/policies"), *SettingsRef.AgreementServerUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FAccelByteUtilities::TArrayUStructToJsonString(AgreementRequests, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::AcceptPolicyVersion(FString const& LocalizedPolicyVersionId, FVoidHandler const& OnSuccess, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/agreements/localized-policy-versions/%s"), *SettingsRef.AgreementServerUrl, *LocalizedPolicyVersionId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::QueryLegalEligibilities(FString const& Namespace, THandler<TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/eligibilities/namespaces/%s"), *SettingsRef.AgreementServerUrl, *Namespace);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");


	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::GetLegalDocument(FString const& Url, THandler<FString> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("Accept"), "*/*");
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte
