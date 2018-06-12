// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeImage.generated.h"

class JusticeImage : public FJsonSerializable
{
public:
	int32 Height;
	int32 Width;
	FString ImageUrl;
	FString SmallImageUrl;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("height", Height);
		JSON_SERIALIZE("width", Width);
		JSON_SERIALIZE("imageUrl", ImageUrl);
		JSON_SERIALIZE("smallImageUrl", SmallImageUrl);
	END_JSON_SERIALIZER
};


UCLASS()
class UJusticeImage : public UObject, public JusticeImage
{
	GENERATED_BODY()		
};