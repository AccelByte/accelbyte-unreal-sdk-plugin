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

struct FItemInfoJustice : public FJsonSerializable
{
	FString				Title;
	FString				Description;
	FString				LongDescription;
	TArray<FImage>		Images;
	FImage				ThumbnailImage;
	FString				ItemID;
	FString				AppId;
	FString				AppType;
	FString				SKU;
	FString				Namespace;
	FString				EntitlementName;
	FString				EntitlementType;
	int32				UseCount;
	FString				CategoryPath;
	FString				Status;
	FString				ItemType;
	FString				CreatedAt;
	FString				UpdatedAt;
	FString				TargetCurrencyCode;
	FString				TargetNamespace;
	TArray<FRegionData>	RegionDatas;
	TArray<FString>		ItemIds;
	TArray<FString>		Tags;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("title", Title);
		JSON_SERIALIZE("description", Description);
		JSON_SERIALIZE("longDescription", LongDescription);
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("images", Images, FImage);
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("thumbnailImage", ThumbnailImage);
		JSON_SERIALIZE("itemId", ItemID);
		JSON_SERIALIZE("appId", AppId);
		JSON_SERIALIZE("appType", AppType);
		JSON_SERIALIZE("sku", SKU);
		JSON_SERIALIZE("namespace", Namespace);
		JSON_SERIALIZE("entitlementName", EntitlementName);
		JSON_SERIALIZE("entitlementType", EntitlementType);
		JSON_SERIALIZE("useCount", UseCount);
		JSON_SERIALIZE("categoryPath", CategoryPath);
		JSON_SERIALIZE("status", Status);
		JSON_SERIALIZE("itemType", ItemType);
		JSON_SERIALIZE("createdAt", CreatedAt);
		JSON_SERIALIZE("updatedAt", UpdatedAt);
		JSON_SERIALIZE("targetCurrencyCode", TargetCurrencyCode);
		JSON_SERIALIZE("targetNamespace", TargetNamespace);
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("regionData", RegionDatas, FRegionData);		
		JSON_SERIALIZE_ARRAY("itemIds", ItemIds);
		JSON_SERIALIZE_ARRAY("tags", Tags);
	END_JSON_SERIALIZER
};

UCLASS()
class UItemInfo : public UObject, public FItemInfoJustice, public FBaseModelJustice<UItemInfo, FItemInfoJustice>
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
};