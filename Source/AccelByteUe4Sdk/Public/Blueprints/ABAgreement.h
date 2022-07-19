// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Models/AccelByteAgreementModels.h"
#include "Core/AccelByteError.h"
#include "ABAgreement.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

USTRUCT(BlueprintType)
struct FArrayModelsPublicPolicyResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | Public Policy")
	TArray<FAccelByteModelsPublicPolicy> Content;
};

USTRUCT(BlueprintType)
struct FArrayModelsRetrieveUserEligibilitiesResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | Eligibility")
	TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse> Content;
};

USTRUCT(BlueprintType)
struct FArrayModelsAcceptAgreementRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | Accept Agreement")
	TArray<FAccelByteModelsAcceptAgreementRequest> Content;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayModelsPublicPolicyResponse, FArrayModelsPublicPolicyResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayModelsRetrieveUserEligibilitiesResponse, FArrayModelsRetrieveUserEligibilitiesResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsAcceptAgreementResponse, FAccelByteModelsAcceptAgreementResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDString, FString, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class  UABAgreement : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Agreement | Api")
	void GetLegalPolicies(
		EAccelByteAgreementPolicyType const& AgreementPolicyType,
		bool DefaultOnEmpty,
		FDArrayModelsPublicPolicyResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Agreement | Api")
	void GetLegalPoliciesByNamespace(
		FString const& Namesapce,
		EAccelByteAgreementPolicyType const& AgreementPolicyType,
		bool DefaultOnEmpty,
		FDArrayModelsPublicPolicyResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Agreement | Api")
	void GetLegalPoliciesByTags(
		EAccelByteAgreementPolicyType const& AgreementPolicyType,
		TArray<FString> const& tags,
		bool DefaultOnEmpty,
		FDArrayModelsPublicPolicyResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Agreement | Api")
	void GetLegalPoliciesByCountry(
		FString const& CountryCode,
		EAccelByteAgreementPolicyType const& AgreementPolicyType,
		bool DefaultOnEmpty,
		FDArrayModelsPublicPolicyResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Agreement | Api")
	void GetLegalPoliciesByCountryAndTags(
		FString const& CountryCode,
		EAccelByteAgreementPolicyType const& AgreementPolicyType,
		TArray<FString> const& Tags,
		bool DefaultOnEmpty,
		FDArrayModelsPublicPolicyResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Agreement | Api")
	void BulkAcceptPolicyVersions(
		FArrayModelsAcceptAgreementRequest const& AgreementRequests,
		FDModelsAcceptAgreementResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Agreement | Api")
	void AcceptPolicyVersion(
		FString const& LocalizedPolicyVersionId,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Agreement | Api")
	void QueryLegalEligibilities(
		FString const& Namespace,
		FDArrayModelsRetrieveUserEligibilitiesResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AAccelByte | Agreement | Api")
	void GetLegalDocument(
		FString const& Url,
		FDString const& OnSuccess,
		FDErrorHandler const& OnError);

private:
	FApiClientPtr ApiClientPtr;
};
