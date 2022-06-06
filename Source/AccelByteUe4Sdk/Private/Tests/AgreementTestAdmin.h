// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"
#include "AgreementTestAdmin.generated.h"

#pragma region AGREEMENT_MODEL

USTRUCT(BlueprintType)
struct FAgreementBasePolicyCreate
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
	FString TypeId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
	FString BasePolicyName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
	TArray<FString> AffectedCountries{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
	TArray<FString> AffectedClientIds{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
	bool IsMandatory{};
};
USTRUCT(BlueprintType)
struct FAgreementPolicyTypeObject
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyTypeObject")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyTypeObject")
	FString PolicyTypeName{};
	// contains more fields but unused for test
};
USTRUCT(BlueprintType)
struct FAgreementPolicyObject
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyObject")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyObject")
	FString CountryCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyObject")
	FString PolicyName{};
	// contains more fields but unused for test
};
USTRUCT(BlueprintType)
struct FAgreementBasePolicy
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicy")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicy")
	FString BasePolicyName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicy")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicy")
	TArray<FAgreementPolicyObject> Policies{};
	// contains more fields but unused for test
};
USTRUCT(BlueprintType)
struct FAgreementPolicyVersionCreate
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
	FString DisplayVersion{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
	bool IsCrucial{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
	bool IsCommitted{};
};
USTRUCT(BlueprintType)
struct FAgreementPolicyVersion
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
	FString DisplayVersion{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
	FString BasePolicyId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
	bool IsCrucial{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
	bool IsInEffect{};
	// contains more fields but unused for test
};
USTRUCT(BlueprintType)
struct FAgreementCountryPolicy
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | CountryPolicy")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | CountryPolicy")
	FString CountryCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | CountryPolicy")
	FString PolicyName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | CountryPolicy")
	bool IsCrucial{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | CountryPolicy")
	TArray<FAgreementPolicyVersion> PolicyVersions{};
	// contains more fields but unused for test
};
USTRUCT(BlueprintType)
struct FAgreementLocalizedPolicyCreate
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicyCreate")
	FString LocaleCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicyCreate")
	FString ContentType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicyCreate")
	FString Description{};
};
USTRUCT(BlueprintType)
struct FAgreementLocalizedPolicy
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicy")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicy")
	FString LocaleCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicy")
	FString ContentType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicy")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicy")
	FString AttachmentLocation{};
};

#pragma endregion AGREEMENT_MODEL

void AdminCreateAgreementBasePolicy(const FAgreementBasePolicyCreate& CreateRequest, const THandler<FAgreementBasePolicy>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateAgreementPolicyVersion(const FString& PolicyId, const FAgreementPolicyVersionCreate& CreateRequest, const THandler<FAgreementPolicyVersion>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateAgreementLocalizedPolicy(const FString& PolicyVersionId, const FAgreementLocalizedPolicyCreate& CreateRequest, const THandler<FAgreementLocalizedPolicy>& OnSuccess, const FErrorHandler& OnError);
void AdminPublishAgreementPolicyVersion(const FString& PolicyVersionId, bool ShouldNotify, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminGetAgreementBasePolicies(const THandler<TArray<FAgreementBasePolicy>>& OnSuccess, const FErrorHandler& OnError);
void AdminGetAgreementCountryBasePolicy(const FString& PolicyId, const FString& CountryCode, const THandler<FAgreementCountryPolicy>& OnSuccess, const FErrorHandler& OnError);
void AdminGetAgreementPolicyTypes(const THandler<TArray<FAgreementPolicyTypeObject>>& OnSuccess, const FErrorHandler& OnError);
void AdminGetAgreementLocalizedPolicies(const FString& PolicyVersionId, const THandler<TArray<FAgreementLocalizedPolicy>>& OnSuccess, const FErrorHandler& OnError);
