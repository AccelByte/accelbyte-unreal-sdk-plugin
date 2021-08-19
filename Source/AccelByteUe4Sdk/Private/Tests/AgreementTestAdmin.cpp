// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AgreementTestAdmin.h"
#include "Core/AccelByteHttpRetryScheduler.h"

void AdminCreateAgreementBasePolicy(const FAgreementBasePolicyCreate& CreateRequest, const THandler<FAgreementBasePolicy>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/base-policies"), *GetAdminBaseUrl());
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(CreateRequest, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCreateAgreementPolicyVersion(const FString& PolId, const FAgreementPolicyVersionCreate& CreateRequest, const THandler<FAgreementPolicyVersion>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/policies/%s/versions"), *GetAdminBaseUrl(), *PolId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(CreateRequest, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCreateAgreementLocalizedPolicy(const FString& PolicyVersionId, const FAgreementLocalizedPolicyCreate& CreateRequest, const THandler<FAgreementLocalizedPolicy>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/localized-policy-versions/versions/%s"), *GetAdminBaseUrl(), *PolicyVersionId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(CreateRequest, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminPublishAgreementPolicyVersion(const FString& PolicyVersionId, bool ShouldNotify, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/policies/versions/%s/latest?shouldNotify=%s"), *GetAdminBaseUrl(), *PolicyVersionId, ShouldNotify ? TEXT("true") : TEXT("false"));
	AB_HTTP_PATCH(Request, Url, Authorization, TEXT(""));
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetAgreementBasePolicies(const THandler<TArray<FAgreementBasePolicy>>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/base-policies"), *GetAdminBaseUrl());
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetAgreementCountryBasePolicy(const FString& BasePolicyId, const FString& CountryCode, const THandler<FAgreementCountryPolicy>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/base-policies/%s/countries/%s"), *GetAdminBaseUrl(), *BasePolicyId, *CountryCode);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetAgreementPolicyTypes(const THandler<TArray<FAgreementPolicyTypeObject>>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/policy-types?limit=100"), *GetAdminBaseUrl());
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetAgreementLocalizedPolicies(const FString& PolicyVersionId, const THandler<TArray<FAgreementLocalizedPolicy>>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/localized-policy-versions/versions/%s"), *GetAdminBaseUrl(), *PolicyVersionId);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
