// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteModelsItem.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemInfoRegionData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		int32 Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		int32 DiscountPercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		int32 DiscountAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		int32 DiscountedPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		FString CurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		FString CurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		FString CurrencyNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		FString PurchaseAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		FString ExpireAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		int32 TotalNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		int32 TotalNumPerAccount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		FString DiscountPurchaseAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		FString DiscountExpireAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		int32 DiscountTotalNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | RegionData")
		int32 DiscountTotalNumPerAccount;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemInfoImage
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | Image")
		int32 Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | Image")
		int32 Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | Image")
		FString ImageUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo | Image")
		FString SmallImageUrl;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString LongDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		TArray<FAccelByteModelsItemInfoImage> Images;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FAccelByteModelsItemInfoImage ThumbnailImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString AppId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString AppType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString Sku;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString EntitlementName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString EntitlementType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		int32 UseCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString CategoryPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString Status;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FDateTime CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FDateTime UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString TargetCurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		FString TargetNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		TArray<FAccelByteModelsItemInfoRegionData> RegionData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		TArray<FString> ItemIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemInfo")
		TArray<FString> Tags;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemPagingSlicedResultPaging
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemPagingSlicedResult | Paging")
		FString Previous;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemPagingSlicedResult | Paging")
		FString Next;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemPagingSlicedResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemPagingSlicedResult")
		TArray<FAccelByteModelsItemInfo> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Item | ItemPagingSlicedResult")
		FAccelByteModelsItemPagingSlicedResultPaging Paging;
};
