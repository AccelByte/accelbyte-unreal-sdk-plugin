// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteAgreementApi.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace Api
{

Agreement::Agreement(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Setting) : Credentials(Credentials), Settings(Setting)
{}

Agreement::~Agreement()
{}

void Agreement::GetLegalPolicies(const EAccelByteAgreementPolicyType& AgreementPolicyType, bool DefaultOnEmpty, const THandler<TArray<FAccelByteModelsPublicPolicy>>& OnSuccess, const FErrorHandler& OnError)
{
	GetLegalPolicies(*Settings.Namespace, AgreementPolicyType, DefaultOnEmpty, OnSuccess, OnError);
}

void Agreement::GetLegalPolicies(const FString& Namespace, const EAccelByteAgreementPolicyType& AgreementPolicyType, bool DefaultOnEmpty, const THandler<TArray<FAccelByteModelsPublicPolicy>>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

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

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/policies/namespaces/%s?policyType=%s&defaultOnEmpty=%s"),
		*Settings.AgreementServerUrl,
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::GetLegalPolicies(const EAccelByteAgreementPolicyType& AgreementPolicyType, const TArray<FString>& Tags, bool DefaultOnEmpty, const THandler<TArray<FAccelByteModelsPublicPolicy>>& OnSuccess, const FErrorHandler& OnError) 
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

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

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/policies/namespaces/%s?policyType=%s&tags=%s&defaultOnEmpty=%s"), 
		*Settings.AgreementServerUrl, 
		*Settings.Namespace, 
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::GetLegalPoliciesByCountry(const FString& CountryCode, const EAccelByteAgreementPolicyType& AgreementPolicyType,  bool DefaultOnEmpty, const THandler<TArray<FAccelByteModelsPublicPolicy>>& OnSuccess, const FErrorHandler& OnError) 
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

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

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/policies/countries/%s?policyType=%s&defaultOnEmpty=%s"), 
		*Settings.AgreementServerUrl, 
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::GetLegalPoliciesByCountry(const FString& CountryCode, const EAccelByteAgreementPolicyType& AgreementPolicyType, const TArray<FString>& Tags,  bool DefaultOnEmpty, const THandler<TArray<FAccelByteModelsPublicPolicy>>& OnSuccess, const FErrorHandler& OnError) 
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

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

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/policies/countries/%s?policyType=%s&tags=%s&defaultOnEmpty=%s"), 
		*Settings.AgreementServerUrl, 
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::BulkAcceptPolicyVersions(const TArray<FAccelByteModelsAcceptAgreementRequest>& AgreementRequests, const THandler<FAccelByteModelsAcceptAgreementResponse>& OnSuccess, const FErrorHandler& OnError) 
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/agreements/policies"), *Settings.AgreementServerUrl);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::BulkAcceptPolicyVersionsIndirect(const TArray<FAccelByteModelsAcceptAgreementRequest>& AgreementRequests, const THandler<FAccelByteModelsAcceptAgreementResponse>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/agreements/policies/users/me"), *Settings.AgreementServerUrl);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::AcceptPolicyVersion(const FString& LocalizedPolicyVersionId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError) 
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/agreements/localized-policy-versions/%s"), *Settings.AgreementServerUrl, *LocalizedPolicyVersionId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::QueryLegalEligibilities(const FString& Namespace, const THandler<TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/eligibilities/namespaces/%s"), *Settings.AgreementServerUrl, *Namespace);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");


	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement::GetLegalDocument(const FString & Url, const THandler<FString>& OnSuccess, const FErrorHandler & OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("Accept"), "*/*");
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte
