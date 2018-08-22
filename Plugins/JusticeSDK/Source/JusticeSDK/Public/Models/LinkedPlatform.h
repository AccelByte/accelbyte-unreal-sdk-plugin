// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeBaseModel.h"
#include "LinkedPlatform.generated.h"

struct FLinkedPlatform : public FJsonSerializable
{
	FString PlatformID;
	FString PlatformUserID;
	FString Namespace;
	FString UserID;

    FLinkedPlatform() {}
    FLinkedPlatform(FString PlatformID) 
    {
        this->PlatformID = PlatformID;
    }

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("PlatformId", PlatformID);
		JSON_SERIALIZE("PlatformUserId", PlatformUserID);
		JSON_SERIALIZE("Namespace", Namespace);
		JSON_SERIALIZE("UserId", UserID);
	END_JSON_SERIALIZER
};

UCLASS()
class ULinkedPlatform : public UObject, public FLinkedPlatform, public FBaseModelJustice<ULinkedPlatform, FLinkedPlatform>
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "LinkedPlatform")
		FString GetPlatformId() { return PlatformID; };

	UFUNCTION(BlueprintPure, Category = "LinkedPlatform")
		FString GetPlatformUserId() { return PlatformUserID; };
};