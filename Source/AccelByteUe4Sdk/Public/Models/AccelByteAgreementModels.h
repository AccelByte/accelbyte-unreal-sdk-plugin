// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteAgreementModels.generated.h"

/** @brief Enumeration for Agreement policy */
UENUM(BlueprintType)
enum class EAccelByteAgreementPolicyType : uint8
{
	EMPTY,
	LEGAL_DOCUMENT_TYPE,
	MARKETING_PREFERENCE_TYPE
};

/** @brief Data Model for Accepting request of specific Agreement */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAcceptAgreementRequest
{
	GENERATED_BODY()

	/** @brief Unique Id for localized policy document */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementRequest")
	FString LocalizedPolicyVersionId{};

	/** @brief Unique Id for policy document */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementRequest")
	FString PolicyVersionId{};

	/** @brief Unique Id for policy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementRequest")
	FString PolicyId{};

	/** @brief Flag that the Agreement is accepted */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementRequest")
	bool IsAccepted{};
};

/** @brief Data Model for Accept Agreement Response */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAcceptAgreementResponse
{
	GENERATED_BODY()
	/** @brief Flag to indicate player can proceed to the game */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementResponse")
	bool Proceed{};
};

/** @brief Data Model for Localized Policy agreement */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLocalizedPolicyVersionObject
{
	GENERATED_BODY()

	/** @brief Unique Id for localized policy document */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
	FString Id{};

	/** @brief Document creation time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
	FDateTime CreatedAt{0};

	/** @brief Document updated time */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
	FDateTime UpdatedAt{0};

	/** @brief Locale code */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
	FString LocaleCode{};

	/** @brief Content type of the legal document */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
	FString ContentType{};

	/** @brief Url to download legal document's content */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
	FString AttachmentLocation{};

	/** @brief Attachment version identifier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
	FString AttachmentVersionIdentifier{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
	FString Status{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
	FDateTime PublishedDate{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
	bool IsDefaultSelection{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPolicyVersionWithLocalizedVersionObject
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
	FString DisplayVersion{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
	FString Status{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
	FDateTime PublishedDate{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
	TArray<FAccelByteModelsLocalizedPolicyVersionObject> LocalizedPolicyVersions{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
	bool IsCommitted{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
	bool IsCrucial{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
	bool IsInEffect{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicPolicy
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	FDateTime createdAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	FDateTime updatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	FString ReadableId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	FString PolicyName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	FString PolicyType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	FString CountryCode{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	FString CountryGroupCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	TArray<FString> BaseUrls{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	bool ShouldNotifyOnUpdate{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	TArray<FAccelByteModelsPolicyVersionWithLocalizedVersionObject> PolicyVersions{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	bool IsMandatory{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	bool IsDefaultOpted{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
	bool IsDefaultSelection{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRetrieveUserEligibilitiesResponse 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | RetrieveUserEligibilitiesResponse")
	FString ReadableId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | RetrieveUserEligibilitiesResponse")
	FString PolicyName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | RetrieveUserEligibilitiesResponse")
	FString PolicyType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | RetrieveUserEligibilitiesResponse")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | RetrieveUserEligibilitiesResponse")
	FString CountryCode{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | RetrieveUserEligibilitiesResponse")
	FString CountryGrupCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | RetrieveUserEligibilitiesResponse")
	TArray<FString> BaseUrls{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | RetrieveUserEligibilitiesResponse")
	TArray<FAccelByteModelsPolicyVersionWithLocalizedVersionObject> PolicyVersions{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | RetrieveUserEligibilitiesResponse")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | RetrieveUserEligibilitiesResponse")
	FString PolicyId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | RetrieveUserEligibilitiesResponse")
	bool IsMandatory{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | RetrieveUserEligibilitiesResponse")
	bool IsAccepted{};
};