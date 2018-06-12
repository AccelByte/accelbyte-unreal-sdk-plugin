// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeBaseModel.h"
#include "LinkedPlatform.generated.h"

class LinkedPlatform : public FJsonSerializable
{
public:
	FString PlatformID;
	FString PlatformUserId;
	FString Namespace;
	FString UserID;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("PlatformId", PlatformID);
		JSON_SERIALIZE("PlatformUserId", PlatformUserId);
		JSON_SERIALIZE("Namespace", Namespace);
		JSON_SERIALIZE("UserId", UserID);
	END_JSON_SERIALIZER
};

UCLASS()
class ULinkedPlatform : public UObject, public LinkedPlatform, public JusticeBaseModel<ULinkedPlatform, LinkedPlatform>
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "LinkedPlatform")
		FString GetPlatformId() { return PlatformID; };
};