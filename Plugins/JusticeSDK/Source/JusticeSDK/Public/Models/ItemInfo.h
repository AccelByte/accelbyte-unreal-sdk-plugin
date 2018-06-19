// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeImage.h"
#include "Price.h"
#include "RegionData.h"
#include "JusticeBaseModel.h"
#include "ItemInfo.generated.h"

class ItemInfo : public FJsonSerializable
{
public:
	FString Title;
	FString Description;
	FString LongDescription;
	TArray<JusticeImage> Images;
	JusticeImage ThumbnailImage;
	FString ItemID;
	FString AppId;
	FString Namespace;
	FString StoreId;
	FString EntitlementName;
	FString EntitlementType;
	int32 UseCount;
	FString CategoryPath;
	FString Status;
	FString ItemType;
	FString CreatedAt;
	FString UpdatedAt;
	TArray<RegionData> RegionDatas;
	TArray<FString> ItemIds;
	TArray<FString> Tags;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("title", Title);
		JSON_SERIALIZE("description", Description);
		JSON_SERIALIZE("longDescription", LongDescription);
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("images", Images, JusticeImage);
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("thumbnailImage", ThumbnailImage);
		JSON_SERIALIZE("itemId", ItemID);
		JSON_SERIALIZE("appId", AppId);
		JSON_SERIALIZE("namespace", Namespace);
		JSON_SERIALIZE("storeId", StoreId);
		JSON_SERIALIZE("entitlementName", EntitlementName);
		JSON_SERIALIZE("entitlementType", EntitlementType);
		JSON_SERIALIZE("useCount", UseCount);
		JSON_SERIALIZE("categoryPath", CategoryPath);
		JSON_SERIALIZE("status", Status);
		JSON_SERIALIZE("itemType", ItemType);
		JSON_SERIALIZE("createdAt", CreatedAt);
		JSON_SERIALIZE("updatedAt", UpdatedAt);
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("regionData", RegionDatas, RegionData);		
		JSON_SERIALIZE_ARRAY("itemIds", ItemIds);
		JSON_SERIALIZE_ARRAY("tags", Tags);
	END_JSON_SERIALIZER
};

UCLASS()
class UItemInfo : public UObject, public ItemInfo, public JusticeBaseModel<UItemInfo, ItemInfo>
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "ItemInfo")
		FString GetItemId() { return ItemID; };

	UFUNCTION(BlueprintPure, Category = "ItemInfo")
		FString GetTitle() { return Title; };

	UFUNCTION(BlueprintPure, Category = "ItemInfo")
		FString GetDescription() { return Description; };

	UFUNCTION(BlueprintPure, Category = "ItemInfo")
		FString GetCurrencyCode() { return RegionDatas[0].CurrencyCode; };

	UFUNCTION(BlueprintPure, Category = "ItemInfo")
		int32 GetPrice() { return RegionDatas[0].Price; };

	UFUNCTION(BlueprintPure, Category = "ItemInfo")
		int32 GetDiscountedPrice() { return RegionDatas[0].DiscountedPrice; };

	UFUNCTION(BlueprintPure, Category = "ItemInfo")
		FString GetStoreID() { return StoreId; };
};