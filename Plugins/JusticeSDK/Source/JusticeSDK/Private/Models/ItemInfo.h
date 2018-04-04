// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeImage.h"
#include "Price.h"
#include "ItemInfo.generated.h"


class ItemInfo : public FJsonSerializable
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
	Price PriceInfo;
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
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("thumbnailImage", ThumbnailImage);
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("images", Images, JusticeImage);		
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("price", PriceInfo);

		JSON_SERIALIZE("status", Status);
		JSON_SERIALIZE("itemType", ItemType);
		JSON_SERIALIZE("createdAt", CreatedAt);
		JSON_SERIALIZE("updatedAt", UpdatedAt);
		JSON_SERIALIZE("favoritCnt", FavoritCnt);
	END_JSON_SERIALIZER
};


UCLASS()
class UItemInfo : public UObject, public ItemInfo
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "ItemInfo")
		FString GetItemId() { return ItemId; };

	UFUNCTION(BlueprintPure, Category = "ItemInfo")
		FString GetTitle() { return Title; };

	UFUNCTION(BlueprintPure, Category = "ItemInfo")
		FString GetDetail() { return Details; };

	UFUNCTION(BlueprintPure, Category = "ItemInfo")
		UPrice* GetPrice() 
		{
			UPrice* result = NewObject<UPrice>(); 
			result->FromPrice(PriceInfo);
			return result;			
		};


public:
		void FromItemInfo(ItemInfo item);
		//void operator=(const ItemInfo item);
		
};
