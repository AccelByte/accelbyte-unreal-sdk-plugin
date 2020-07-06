// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteGeneralModels.h"
#include "AccelByteAgreementModels.generated.h"

UENUM(BlueprintType)
enum class EAccelByteAgreementPolicyType : uint8
{
	EMPTY,
	LEGAL_DOCUMENT_TYPE,
	MARKETING_PREFERENCE_TYPE
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAcceptAgreementRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementRequest")
		FString LocalizedPolicyVersionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementRequest")
		FString PolicyVersionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementRequest")
		FString PolicyId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementRequest")
		bool IsAccepted;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAcceptAgreementResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementResponse")
		bool Proceed;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLocalizedPolicyVersionObject
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
		FDateTime CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
		FDateTime UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
		FString LocaleCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
		FString ContentType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
		FString AttachmentLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
		FString AttachmentVersionIdentifier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
		FString Status;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
		FDateTime PublishedDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | LocalizedPolicyVersionObject")
		bool IsDefaultSelection;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPolicyVersionWithLocalizedVersionObject
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
		FDateTime CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
		FDateTime UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
		FString DisplayVersion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
		FString Status;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
		FDateTime PublishedDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
		TArray<FAccelByteModelsLocalizedPolicyVersionObject> LocalizedPolicyVersions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
		bool IsCommitted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
		bool IsCrucial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PolicyVersionWithLocalizedVersionObject")
		bool IsInEffect;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicPolicy
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		FDateTime createdAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		FDateTime updatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		FString ReadableId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		FString PolicyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		FString PolicyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		FString CountryCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		FString CountryGroupCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		TArray<FString> BaseUrls;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		bool ShouldNotifyOnUpdate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		TArray<FAccelByteModelsPolicyVersionWithLocalizedVersionObject> PolicyVersions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		bool IsMandatory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		bool IsDefaultOpted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | PublicPolicy")
		bool IsDefaultSelection;
};