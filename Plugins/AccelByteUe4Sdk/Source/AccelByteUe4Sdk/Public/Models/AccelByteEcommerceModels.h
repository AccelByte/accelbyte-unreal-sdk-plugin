// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteGeneralModels.h"
#include "AccelByteEcommerceModels.generated.h"

#pragma region EnumField
UENUM(BlueprintType)
enum class EAccelByteItemType : uint8
{
	NONE = 0,
	APP,
	COINS,
	INGAMEITEM,
	BUNDLE,
	CODE,
	SUBSCRIPTION
};

UENUM(BlueprintType)
enum class EAccelByteItemStatus : uint8
{
	NONE = 0,
	ACTIVE,
	INACTIVE
};

UENUM(BlueprintType)
enum class EAccelByteItemCurrencyType : uint8
{
	NONE = 0,
	REAL,
	VIRTUAL
};

UENUM(BlueprintType)
enum class EAccelByteEntitlementClass : uint8
{
	NONE = 0,
	APP,
	ENTITLEMENT,
	DISTRIBUTION,
	CODE
};

UENUM(BlueprintType)
enum class EAccelByteEntitlementType : uint8
{
	NONE = 0,
	DURABLE,
	CONSUMABLE
};

UENUM(BlueprintType)
enum class EAccelByteEntitlementStatus : uint8
{
	NONE = 0,
	ACTIVE,
	INACTIVE,
	CONSUMED,
	DISTRIBUTED,
	REVOKED
};

UENUM(BlueprintType)
enum class EAccelByteAppType : uint8
{
	NONE = 0,
	GAME,
	SOFTWARE,
	DLC,
	DEMO
};

UENUM(BlueprintType)
enum class EAccelByteOrderStatus : uint8
{
	NONE = 0,
	INIT,
	CHARGED,
	CHARGEBACK,
	CHARGEBACK_REVERSED,
	FULFILLED,
	FULFILL_FAILED,
	REFUNDING,
	REFUNDED,
	REFUND_FAILED,
	CLOSED,
	DELETED
};

UENUM(BlueprintType)
enum class EAccelByteEntitlementSource : uint8
{
	NONE = 0,
	PURCHASE,
	IAP,
	PROMOTION,
	ACHIEVEMENT,
	REFERRAL_BONUS,
	REDEEM_CODE,
	OTHER
};

UENUM(BlueprintType)
enum class EAccelByteCreditUserWalletSource : uint8
{
	PURCHASE = 0,
	IAP,
	PROMOTION,
	ACHIEVEMENT,
	REFERRAL_BONUS,
	REDEEM_CODE,
	REFUND,
	OTHER
};

UENUM(BlueprintType) 
enum class EAccelByteSubscriptionCycle : uint8 
{
	WEEKLY = 0,
	MONTHLY,
	QUARTERLY,
	YEARLY
};

UENUM(BlueprintType)
enum class EAccelBytePlatformSync : uint8
{
	STEAM = 0,
	XBOX_LIVE,
	PLAYSTATION,
	STADIA,
	EPIC_GAMES,
	OTHER
};

#pragma endregion EnumField

#pragma region ItemModelsField
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemRegionDataItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		int32 Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		int32 DiscountPercentage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		int32 DiscountAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		int32 DiscountedPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		FString CurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		EAccelByteItemCurrencyType CurrencyType{EAccelByteItemCurrencyType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		FString CurrencyNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		FDateTime PurchaseAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		FDateTime ExpireAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		FDateTime DiscountPurchaseAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		FDateTime DiscountExpireAt;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemImage
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
		FString As;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
		FString Caption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
		int32 Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
		int32 Width;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
		FString ImageUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
		FString SmallImageUrl;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString LongDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString AppId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString AppType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString BaseAppId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Sku;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		EAccelByteEntitlementType EntitlementType{EAccelByteEntitlementType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		int32 UseCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		bool Stackable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString CategoryPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		EAccelByteItemStatus Status{EAccelByteItemStatus::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		EAccelByteItemType ItemType{EAccelByteItemType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString TargetNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString TargetCurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString TargetItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		TArray<FAccelByteModelsItemImage> Images;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString ThumbnailUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		TArray<FAccelByteModelsItemRegionDataItem> RegionData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		TArray<FString> ItemIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		TArray<FString> Tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		TArray<FString> Features;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		int32 MaxCountPerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		int32 MaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Clazz;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString BoothName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		int32 DisplayOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Ext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Region;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Language;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FDateTime CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FDateTime UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString LocalExt;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPopulatedItemInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString LongDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString AppId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString AppType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString BaseAppId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Sku;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		EAccelByteEntitlementType EntitlementType{EAccelByteEntitlementType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		int32 UseCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		bool Stackable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString CategoryPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		EAccelByteItemStatus Status{EAccelByteItemStatus::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		EAccelByteItemType ItemType{EAccelByteItemType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString TargetNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString TargetCurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString TargetItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		TArray<FAccelByteModelsItemImage> Images;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString ThumbnailUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		TArray<FAccelByteModelsItemRegionDataItem> RegionData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		TArray<FString> ItemIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		TArray<FString> Tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		TArray<FString> Features;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		int32 MaxCountPerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		int32 MaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Clazz;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString BoothName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		int32 DisplayOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Ext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Region;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString Language;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FDateTime CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FDateTime UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		TArray<FAccelByteModelsItemInfo> Items;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		FString LocalExt;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemPagingSlicedResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemPagingSlicedResult")
		TArray<FAccelByteModelsItemInfo> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemPagingSlicedResult")
		FAccelByteModelsPaging Paging;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemCriteria
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
		EAccelByteItemType ItemType{EAccelByteItemType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
		EAccelByteAppType AppType{EAccelByteAppType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
		FString Region;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
		FString Language;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
		FString CategoryPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
		FString BaseAppId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
		TArray<FString> Tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
		TArray<FString> Features;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
		FString SortBy;
};

#pragma endregion ItemModelsField

#pragma region EntitlementModelsField

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementItemSnapshot
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString AppId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		EAccelByteAppType AppType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString BaseAppId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString Sku;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		EAccelByteEntitlementType EntitlementType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		int32 UseCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		bool Stackable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		EAccelByteItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString ThumbnailUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString TargetNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString TargetCurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString TargetItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FAccelByteModelsItemRegionDataItem RegionDataItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		TArray<FString> ItemIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		int32 MaxCountPerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		int32 MaxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString BoothName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString Region;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
		FString Language;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		EAccelByteEntitlementClass Clazz;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		EAccelByteEntitlementType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		EAccelByteEntitlementStatus Status;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString AppId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		EAccelByteAppType AppType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString Sku;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString GrantedCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString ItemNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		int32 UseCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		int32 Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		EAccelByteEntitlementSource Source;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		int32 DistributedQuantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString TargetNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FAccelByteModelsEntitlementItemSnapshot ItemSnapshot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString StartDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FString EndDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FDateTime GrantedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FDateTime CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
		FDateTime UpdatedAt;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementPagingSlicedResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult ")
		TArray<FAccelByteModelsEntitlementInfo> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult ")
		FAccelByteModelsPaging Paging;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsConsumeUserEntitlementRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | ConsumeUserEntitlementRequest")
		int32 UseCount;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementOwnership
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | EntitlementOwnership")
		bool Owned;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementGrant
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
		FString GrantedCode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
		FString ItemNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
		int32 Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
		EAccelByteEntitlementSource Source;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
		FString Region;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
		FString Language;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsStackableEntitlementInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		EAccelByteEntitlementClass Clazz;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		EAccelByteEntitlementType Type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		EAccelByteEntitlementStatus Status;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		FString AppId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		EAccelByteAppType AppType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		FString Sku;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		FString GrantedCode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		FString ItemNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		int32 UseCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		int32 Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		EAccelByteEntitlementSource Source;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		int32 DistributedQuantity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		FString TargetNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
		FAccelByteModelsEntitlementItemSnapshot ItemSnapshot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
		FString StartDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
		FString EndDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
		bool Stackable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
		FDateTime GrantedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
		FDateTime CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
		FDateTime UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
		int32 StackedUseCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
		int32 StackedQuantity;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAttributes
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | Attributes")
		FString ServerId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | Attributes")
		FString ServerName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | Attributes")
		FString CharacterId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | Attributes")
		FString CharacterName;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDistributionAttributes
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | DistributionAttributes")
		FAccelByteModelsAttributes Attributes;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDistributionReceiver
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | DistributionReceiver")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | DistributionReceiver")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | DistributionReceiver")
		FString ExtUserId;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Entitlements | Models | DistributionReceiver")
		FAccelByteModelsAttributes Attributes;
};

#pragma endregion EntitlementModelsField

#pragma region OrderModelsField
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderInfoPaymentUrl
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
		FString PaymentProvider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
		FString PaymentUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
		FString PaymentToken;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
		FString ReturnUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
		FString PaymentType;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderCurrencySummary
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
		FString CurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
		FString CurrencySymbol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
		EAccelByteItemCurrencyType CurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
		int32 Decimals;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderInfoTransaction
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString TransactionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		int32 Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		int32 Vat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		int32 SalesTax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FAccelByteModelsOrderCurrencySummary Currency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString Type; //['CHARGE', 'REFUND'],
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString Status; // ['INIT', 'FINISHED', 'FAILED'],
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString Provider; //['XSOLLA', 'WALLET'],
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		int32 PaymentProviderFee;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString PaymentMethod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		int32 PaymentMethodFee;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString MerchantId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString ExternalTransactionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString ExternalStatusCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString ExternalMessage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString TransactionStartTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString TransactionEndTime;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FString OrderNo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FString PaymentOrderNo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		bool Sandbox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		int32 Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		int32 Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		int32 DiscountedPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		int32 Vat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		int32 SalesTax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		int32 PaymentProviderFee;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		int32 PaymentMethodFee;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FAccelByteModelsOrderCurrencySummary Currency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FString PaymentStationUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FAccelByteModelsEntitlementItemSnapshot ItemSnapshot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FString Region;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FString Language;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		EAccelByteOrderStatus Status; // ['INIT', 'CHARGED', 'CHARGEBACK', 'CHARGEBACK_REVERSED', 'FULFILLED', 'FULFILL_FAILED', 'REFUNDING', 'REFUNDED', 'REFUND_FAILED', 'CLOSED', 'DELETED']
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FString StatusReason;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FDateTime CreatedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FDateTime ChargedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FDateTime FulfilledTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FDateTime RefundedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FDateTime ChargebackTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FDateTime ChargebackReversedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FDateTime CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FDateTime UpdatedAt;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPagedOrderInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		TArray<FAccelByteModelsOrderInfo> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FAccelByteModelsPaging Paging;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderHistoryInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FString OrderNo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FString Operator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FString Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FString Reason;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FDateTime CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FDateTime UpdatedAt;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderCreate
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		int32 Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		int32 Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		int32 DiscountedPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		FString CurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		FString Region;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		FString Language;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		FString ReturnUrl;
};

#pragma endregion OrderModelsField

#pragma region CategoryModelsField

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCategoryInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
		FString ParentCategoryPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
		FString CategoryPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
		FDateTime CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
		FDateTime UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
		bool Root;
};

#pragma	endregion CategoryModelsField

#pragma region WalletModelsField

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsWalletInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FString CurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FString CurrencySymbol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		int32 Balance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FDateTime CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		FDateTime UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
		EAccelByteItemStatus Status;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCreditUserWalletRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | CreditUserWalletRequest")
		int32 Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | CreditUserWalletRequest")
		EAccelByteCreditUserWalletSource Source;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | CreditUserWalletRequest")
		FString Reason;
};

#pragma endregion WalletModelsField
