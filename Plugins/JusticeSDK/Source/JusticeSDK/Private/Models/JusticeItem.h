// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeImage.h"
#include "Price.h"
#include "JusticeItem.generated.h"


class JusticeItem : public FJsonSerializable
{

public:

	FString ItemId;
	FString AppId;
	FString Namespace;
	FString EntitlementName;
	FString EntitlementType;
	int UseCount;
	FString CategoryPath;
	FString Title;
	FString Details;
	JusticeImage ThumbnailImage;
	TArray<JusticeImage> Images;
	Price Price;
	FString Status;
	FString ItemType;
	FString CreatedAt;
	FString UpdatedAt;
	int FavoritCnt;


	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("itemId", ItemId);
		JSON_SERIALIZE("appId", AppId);
		JSON_SERIALIZE("namespace", Namespace);
		JSON_SERIALIZE("entitlementName", EntitlementName);
		JSON_SERIALIZE("entitlementType", EntitlementType);
		JSON_SERIALIZE("useCount", UseCount);
		JSON_SERIALIZE("categoryPath", CategoryPath);
		JSON_SERIALIZE("title", Title);
		JSON_SERIALIZE("details", Details);		
		JSON_SERIALIZE_SERIALIZABLE("thumbnailImage", thumbnailImage, JusticeImage);
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("images", images, JusticeImage);
		JSON_SERIALIZE_SERIALIZABLE("price", price, Price);
		JSON_SERIALIZE("status", Status);
		JSON_SERIALIZE("itemType", ItemType);
		JSON_SERIALIZE("createdAt", CreatedAt);
		JSON_SERIALIZE("updatedAt", UpdatedAt);
		JSON_SERIALIZE("favoritCnt", FavoritCnt);
	END_JSON_SERIALIZER

};


UCLASS()
class UJusticeItem : public UObject, public JusticeItem
{
	GENERATED_BODY()
		
};
