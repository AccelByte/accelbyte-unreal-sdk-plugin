// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeImage.generated.h"

struct FImage : public FJsonSerializable
{
	int32	Height;
	int32	Width;
	FString ImageURL;
	FString SmallImageURL;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("height", Height);
		JSON_SERIALIZE("width", Width);
		JSON_SERIALIZE("imageUrl", ImageURL);
		JSON_SERIALIZE("smallImageUrl", SmallImageURL);
	END_JSON_SERIALIZER
};

UCLASS()
class UJusticeImage : public UObject, public FImage
{
	GENERATED_BODY()		
};