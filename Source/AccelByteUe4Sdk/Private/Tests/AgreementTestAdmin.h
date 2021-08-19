// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"

void AdminCreateAgreementBasePolicy(const FAgreementBasePolicyCreate& CreateRequest, const THandler<FAgreementBasePolicy>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateAgreementPolicyVersion(const FString& PolicyId, const FAgreementPolicyVersionCreate& CreateRequest, const THandler<FAgreementPolicyVersion>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateAgreementLocalizedPolicy(const FString& PolicyVersionId, const FAgreementLocalizedPolicyCreate& CreateRequest, const THandler<FAgreementLocalizedPolicy>& OnSuccess, const FErrorHandler& OnError);
void AdminPublishAgreementPolicyVersion(const FString& PolicyVersionId, bool ShouldNotify, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminGetAgreementBasePolicies(const THandler<TArray<FAgreementBasePolicy>>& OnSuccess, const FErrorHandler& OnError);
void AdminGetAgreementCountryBasePolicy(const FString& PolicyId, const FString& CountryCode, const THandler<FAgreementCountryPolicy>& OnSuccess, const FErrorHandler& OnError);
void AdminGetAgreementPolicyTypes(const THandler<TArray<FAgreementPolicyTypeObject>>& OnSuccess, const FErrorHandler& OnError);
void AdminGetAgreementLocalizedPolicies(const FString& PolicyVersionId, const THandler<TArray<FAgreementLocalizedPolicy>>& OnSuccess, const FErrorHandler& OnError);
