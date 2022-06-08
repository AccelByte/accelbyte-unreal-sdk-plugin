// Copyright (c) 2022 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Models/AccelByteAgreementModels.h"
#include "Core/AccelByteError.h"
#include "AgreementTestAdmin.h"
#include "AgreementTestAdminBP.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

USTRUCT(BlueprintType)
struct FArrayAgreementBasePolicy
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	TArray<FAgreementBasePolicy> Content{};
};

USTRUCT(BlueprintType)
struct FArrayAgreementPolicyTypeObject
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	TArray<FAgreementPolicyTypeObject> Content{};
};

USTRUCT(BlueprintType)
struct FArrayAgreementLocalizedPolicy
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	TArray<FAgreementLocalizedPolicy> Content{};
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAgreementBasePolicyResponse, FAgreementBasePolicy, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDAgreementPolicyVersionResponse, FAgreementPolicyVersion, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDAgreementLocalizedPolicyResponse, FAgreementLocalizedPolicy, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayAgreementBasePolicyResponse, FArrayAgreementBasePolicy, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDAgreementCountryPolicyResponse, FAgreementCountryPolicy, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayAgreementPolicyTypeObjectResponse, FArrayAgreementPolicyTypeObject, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayAgreementLocalizedPolicyResponse, FArrayAgreementLocalizedPolicy, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class  UAgreementTestAdmin : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "AccelByte | AgreementAdmin")
	void AdminCreateAgreementBasePolicyBP(
		FAgreementBasePolicyCreate const& CreateRequest,
		FDAgreementBasePolicyResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | AgreementAdmin")
	void AdminCreateAgreementPolicyVersionBP(
		FString const& PolicyId, 
		FAgreementPolicyVersionCreate const& CreateRequest, 
		FDAgreementPolicyVersionResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | AgreementAdmin")
	void AdminCreateAgreementLocalizedPolicyBP(
		FString const& PolicyVersionId, 
		FAgreementLocalizedPolicyCreate const& CreateRequest, 
		FDAgreementLocalizedPolicyResponse const& OnSuccess, 
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | AgreementAdmin")
	void AdminPublishAgreementPolicyVersionBP(
		FString const& PolicyVersionId, 
		bool ShouldNotify, 
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | AgreementAdmin")
	void AdminGetAgreementBasePoliciesBP(
		FDArrayAgreementBasePolicyResponse const& OnSuccess,
		FDErrorHandler const& OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | AgreementAdmin")
	void AdminGetAgreementCountryBasePolicyBP(
		FString const& PolicyId, 
		FString const& CountryCode, 
		FDAgreementCountryPolicyResponse const& OnSuccess,
		FDErrorHandler const& OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | AgreementAdmin")
	void AdminGetAgreementPolicyTypesBP(
		FDArrayAgreementPolicyTypeObjectResponse const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | AgreementAdmin")
	void AdminGetAgreementLocalizedPoliciesBP(
		FString const& PolicyVersionId, 
		FDArrayAgreementLocalizedPolicyResponse const& OnSuccess,
		FDErrorHandler const& OnError);

};
