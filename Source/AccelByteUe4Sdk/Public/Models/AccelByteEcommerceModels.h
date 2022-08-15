// Copyright (c) 2019 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"
#include "Models/AccelByteGeneralModels.h"
#include "JsonObjectWrapper.h"
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
	SUBSCRIPTION,
	SEASON,
	MEDIA,
	OPTIONBOX
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
	CODE,
	MEDIA
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
enum class EAccelByteItemSource : uint8
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
enum class EAccelByteEntitlementSource : uint8
{
	NONE = 0,
	PURCHASE,
	IAP,
	PROMOTION,
	ACHIEVEMENT,
	REFERRAL_BONUS,
	REDEEM_CODE,
	REWARD,
	GIFT,
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

UENUM(BlueprintType)
enum class EAccelByteSeasonType : uint8
{
	PASS = 0,
	TIER
};

UENUM(BlueprintType)
enum class EAccelByteCurrencyType : uint8
{
	NONE = 0,
	REAL,
	VIRTUAL
};

UENUM(BlueprintType)
enum class EAccelByteRewardListSortBy : uint8
{
	NONE = 0,
	NAMESPACE,
	NAMESPACE_ASC,
	NAMESPACE_DESC,
	REWARDCODE,
	REWARDCODE_ASC,
	REWARDCODE_DESC
};

UENUM(BlueprintType)
enum class EAccelByteWalletAction :uint8
{
	NONE = 0,
	CREDIT,
	PAYMENT,
	DEBIT
};

UENUM(BlueprintType)
enum class EAccelByteWalletTable :uint8
{
	Playstation = 0,
	Xbox,
	Steam,
	Epic,
	Stadia,
	IOS,
	GooglePlay,
	Twitch,
	System,
	Nintendo,
	Other
};

UENUM(BlueprintType)
enum class EAccelByteItemListSortBy : uint8
{
	NONE = 0,
	NAME,
	NAME_DESC,
	CREATEDAT,
	CREATEDAT_ASC,
	CREATEDAT_DESC,
	UPDATEDAT,
	UPDATEDAT_ASC,
	UPDATEDAT_DESC,
	DISPLAYORDER,
	DISPLAYORDER_ASC,
	DISPLAYORDER_DESC
};

UENUM(BlueprintType)
enum class EAccelByteCycle : uint8
{
	WEEKLY,
	MONTHLY,
	QUARTERLY,
	YEARLY 
};

UENUM(BlueprintType)
enum class EAccelBytePredicateType : uint8
{
	EntitlementPredicate,
	SeasonPassPredicate,
	SeasonTierPredicate
};

#pragma endregion EnumField

#pragma region ItemModelsField

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemRegionDataItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	int32 Price{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	int32 DiscountPercentage{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	int32 DiscountAmount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	int32 DiscountedPrice{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	FString CurrencyCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	EAccelByteItemCurrencyType CurrencyType{EAccelByteItemCurrencyType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	FString CurrencyNamespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	int TrialPrice{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	FDateTime PurchaseAt{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	FDateTime ExpireAt{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	FDateTime DiscountPurchaseAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	FDateTime DiscountExpireAt{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemImage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
	FString As{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
	FString Caption{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
	int32 Height{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
	int32 Width{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
	FString ImageUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
	FString SmallImageUrl{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemRecurring
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
	EAccelByteCycle Cycle{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
	int FixedFreeDays{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
	int FixedTrialCycles{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | Image")
	int GraceDays{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelItemPredicate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	EAccelBytePredicateType PredicateType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Comparison{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString AnyOf{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FString> Values{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Value{};
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelItemConditionGroup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FAccelByteModelItemPredicate> Predicate{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Operator{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelItemPurchaseCondition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FAccelByteModelItemConditionGroup> FAccelByteModelItemConditionGroups{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelItemBoxItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString ItemSku{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	int32 Count{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelItemOptionBoxConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FAccelByteModelItemBoxItem> BoxItems{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Title{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Description{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString LongDescription{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString AppId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString AppType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	EAccelByteSeasonType SeasonType{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString BaseAppId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Sku{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Name{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	EAccelByteEntitlementType EntitlementType{EAccelByteEntitlementType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	int32 UseCount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	bool Stackable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString CategoryPath{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	EAccelByteItemStatus Status{EAccelByteItemStatus::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	bool Listable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	bool Purchasable{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	EAccelByteItemType ItemType{EAccelByteItemType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString TargetNamespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString TargetCurrencyCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString TargetItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FAccelByteModelsItemImage> Images{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString ThumbnailUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FAccelByteModelsItemRegionDataItem> RegionData{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FAccelByteModelsItemRecurring Recurring{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FString> ItemIds{}; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FString> BoundItemIds{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FString> Tags{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FString> Features{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	int32 MaxCountPerUser{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	int32 MaxCount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Clazz{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString BoothName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	int32 DisplayOrder{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FJsonObjectWrapper Ext{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Region{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Language{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FDateTime UpdatedAt{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FAccelByteModelItemPurchaseCondition PurchaseCondition{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FAccelByteModelItemOptionBoxConfig OptionBoxConfig{};	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FJsonObjectWrapper LocalExt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TMap<FString, int32> ItemQty{};

};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPopulatedItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Title{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString LongDescription{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString AppId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString AppType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString BaseAppId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Sku{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Name{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	EAccelByteEntitlementType EntitlementType{EAccelByteEntitlementType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	int32 UseCount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	bool Stackable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString CategoryPath{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	EAccelByteItemStatus Status{EAccelByteItemStatus::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	EAccelByteItemType ItemType{EAccelByteItemType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString TargetNamespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString TargetCurrencyCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString TargetItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FAccelByteModelsItemImage> Images{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString ThumbnailUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FAccelByteModelsItemRegionDataItem> RegionData{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FString> ItemIds{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FString> Tags{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FString> Features{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	int32 MaxCountPerUser{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	int32 MaxCount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Clazz{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString BoothName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	int32 DisplayOrder{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FJsonObjectWrapper Ext{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Region{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Language{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FAccelByteModelsItemInfo> Items{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FJsonObjectWrapper LocalExt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TMap<FString, int32> ItemQty{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemPagingSlicedResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemPagingSlicedResult")
	TArray<FAccelByteModelsItemInfo> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemPagingSlicedResult")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemCriteria
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
	FString StoreId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
	EAccelByteItemType ItemType{EAccelByteItemType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
	EAccelByteAppType AppType{EAccelByteAppType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
	FString Language{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
	FString CategoryPath{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
	FString BaseAppId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
	TArray<FString> Tags{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemCriteria")
	TArray<FString> Features{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
	FString SortBy{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBulkGetItemsBySkus
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemInfo")
	FString Sku{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemInfo")
	FString ItemId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemDynamicData
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemDynamicData")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemDynamicData")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemDynamicData")
	int32 AvailableCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemDynamicData")
	int32 UserAvailableCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemDynamicData")
	int32 UserPurchaseLimit{};
};

#pragma endregion ItemModelsField

#pragma region EntitlementModelsField

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementItemSnapshot
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString AppId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	EAccelByteAppType AppType{EAccelByteAppType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString BaseAppId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString Sku{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	bool Listable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	EAccelByteEntitlementType EntitlementType{EAccelByteEntitlementType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	int32 UseCount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	bool Stackable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	bool Purchasable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	EAccelByteItemType ItemType{EAccelByteItemType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString ThumbnailUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString TargetNamespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString TargetCurrencyCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString TargetItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString Title{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FAccelByteModelsItemRecurring Recurring{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FAccelByteModelsItemRegionDataItem RegionDataItem{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	TArray<FString> ItemIds{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FJsonObjectWrapper ItemQty{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	TArray<FString> Features{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	int32 MaxCountPerUser{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	int32 MaxCount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString BoothName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FString Language{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo | ItemSnapshot ")
	FAccelByteModelItemOptionBoxConfig OptionBoxConfig{};	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	EAccelByteEntitlementClass Clazz{EAccelByteEntitlementClass::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	EAccelByteEntitlementType Type{EAccelByteEntitlementType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	EAccelByteEntitlementStatus Status{EAccelByteEntitlementStatus::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FString AppId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	EAccelByteAppType AppType{EAccelByteAppType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FString Sku{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FString GrantedCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FString ItemNamespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	TArray<FString> Features{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	int32 UseCount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	EAccelByteEntitlementSource Source{EAccelByteEntitlementSource::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FAccelByteModelsEntitlementItemSnapshot ItemSnapshot{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FString StartDate{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FString EndDate{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	bool Stackable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FDateTime GrantedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FAccelByteModelItemOptionBoxConfig OptionBoxConfig{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementPagingSlicedResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult ")
	TArray<FAccelByteModelsEntitlementInfo> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult ")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsConsumeUserEntitlementRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | ConsumeUserEntitlementRequest")
	int32 UseCount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | ConsumeUserEntitlementRequest")
	TArray<FString> Options{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementOwnershipItemIds
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | OwnershipByItemIds")
	bool Owned{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | OwnershipByItemIds")
	FString ItemId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementOwnership
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementOwnership")
	bool Owned{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementOwnership")
	FDateTime EndDate{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOwnershipToken
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementOwnershipToken")
	FString OwnershipToken{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementOwnershipDetail
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementOwnership")
	FString AppId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementOwnership")
	FString AppType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementOwnership")
	FString Sku{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementOwnership")
	FString ItemId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementOwnership")
	FString ItemNamespace{};	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementOwnershipDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementOwnership")
	TArray<FAccelByteModelsEntitlementOwnershipDetail> Entitlements{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementGrant
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
	FString GrantedCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
	FString ItemNamespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
	int32 Quantity{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
	EAccelByteEntitlementSource Source{EAccelByteEntitlementSource::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | EntitlementGrant")
	FString Language{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsStackableEntitlementInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	EAccelByteEntitlementClass Clazz{EAccelByteEntitlementClass::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	EAccelByteEntitlementType Type{EAccelByteEntitlementType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	EAccelByteEntitlementStatus Status{EAccelByteEntitlementStatus::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	FString AppId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	EAccelByteAppType AppType{EAccelByteAppType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	FString Sku{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	FString GrantedCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	FString ItemNamespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	int32 UseCount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	int32 Quantity{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	EAccelByteEntitlementSource Source{EAccelByteEntitlementSource::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	int32 DistributedQuantity{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	FString TargetNamespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | StackableEntitlementInfo")
	FAccelByteModelsEntitlementItemSnapshot ItemSnapshot{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
	FString StartDate{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
	FString EndDate{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
	bool Stackable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
	FDateTime GrantedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
	int32 StackedUseCount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | StackableEntitlementInfo ")
	int32 StackedQuantity{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | Attributes")
	FString ServerId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | Attributes")
	FString ServerName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | Attributes")
	FString CharacterId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | Attributes")
	FString CharacterName{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDistributionAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | DistributionAttributes")
	FAccelByteModelsAttributes Attributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDistributionReceiver
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | DistributionReceiver")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | DistributionReceiver")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | DistributionReceiver")
	FString ExtUserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | DistributionReceiver")
	FAccelByteModelsAttributes Attributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBulkRevokeEntitlements
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | BulkRevokeEntitlements")
	int32 Affected{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlatformSyncMobileApple
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileApple")
	FString ProductId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileApple")
	FString TransactionId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileApple")
	FString ReceiptData{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileApple")
	bool ExcludeOldTransactions{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileApple")
	FString Region{}; //optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileApple")
	FString Language{}; //optional
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlatformSyncMobileGoogle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileGoogle")
	FString OrderId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileGoogle")
	FString PackageName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileGoogle")
	FString ProductId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileGoogle")
	int64 PurchaseTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileGoogle")
	FString PurchaseToken{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileGoogle")
	bool AutoAck{}; //should be true for sync DURABLE item entitlement

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileGoogle")
	FString Region{}; //optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileGoogle")
	FString Language{}; //optional
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlatformSyncMobileGoogleResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlatformSyncMobileGoogle")
	bool NeedConsume{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsXBoxDLCSync
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | XboxDLCSync")
	FString XstsToken{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayStationDLCSync
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationDLCSync")
	int32 ServiceLabel{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsTwitchDropEntitlement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Platform")
	FString GameId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Platform")
	FString Region{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Platform")
	FString Language{};
};

#pragma endregion EntitlementModelsField

#pragma region OrderModelsField
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderInfoPaymentUrl
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
	FString PaymentProvider{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
	FString PaymentUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
	FString PaymentToken{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
	FString ReturnUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
	FString PaymentType{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderCurrencySummary
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
	FString CurrencyCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
	FString CurrencySymbol{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
	EAccelByteItemCurrencyType CurrencyType{EAccelByteItemCurrencyType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
	int32 Decimals{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderInfoTransaction
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	FString TransactionId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	int32 Amount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	int32 Vat{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	int32 SalesTax{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	FAccelByteModelsOrderCurrencySummary Currency{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	FString Type{}; //['CHARGE', 'REFUND'],

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	FString Status{}; // ['INIT', 'FINISHED', 'FAILED'],

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	FString Provider{}; //['XSOLLA', 'WALLET'],

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	int32 PaymentProviderFee{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	FString PaymentMethod{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	int32 PaymentMethodFee{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	FString MerchantId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	FString ExternalTransactionId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	FString ExternalStatusCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	FString ExternalMessage{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	FString TransactionStartTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
	FString TransactionEndTime{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString OrderNo{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString PaymentOrderNo{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	bool Sandbox{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 Quantity{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 Price{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 DiscountedPrice{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 Vat{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 SalesTax{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 PaymentProviderFee{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 PaymentMethodFee{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FAccelByteModelsOrderCurrencySummary Currency{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString PaymentStationUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FAccelByteModelsEntitlementItemSnapshot ItemSnapshot{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString Language{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	EAccelByteOrderStatus Status{EAccelByteOrderStatus::NONE}; // ['INIT', 'CHARGED', 'CHARGEBACK', 'CHARGEBACK_REVERSED', 'FULFILLED', 'FULFILL_FAILED', 'REFUNDING', 'REFUNDED', 'REFUND_FAILED', 'CLOSED', 'DELETED']

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString StatusReason{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FDateTime CreatedTime{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FDateTime ChargedTime{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FDateTime FulfilledTime{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FDateTime RefundedTime{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FDateTime ChargebackTime{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FDateTime ChargebackReversedTime{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FDateTime UpdatedAt{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPagedOrderInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	TArray<FAccelByteModelsOrderInfo> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderHistoryInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
	FString OrderNo{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
	FString Operator{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
	FString Action{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
	FString Reason{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
	FDateTime UpdatedAt{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderCreate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
	int32 Quantity{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
	int32 Price{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
	int32 DiscountedPrice{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
	FString CurrencyCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
	FString Language{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
	FString ReturnUrl{};
};

#pragma endregion OrderModelsField

#pragma region CategoryModelsField

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCategoryInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
	FString ParentCategoryPath{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
	FString CategoryPath{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
	FString DisplayName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | CategoryInfo")
	bool Root{};
};

#pragma	endregion CategoryModelsField

#pragma region WalletModelsField

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsTimeLimitedBalance
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | TimeLimitedBalance")
	int32 Balance{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | TimeLimitedBalance")
	FString BalanceSource{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | TimeLimitedBalance")
	FDateTime ExpiredAt{0};
	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsWalletInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	FString CurrencyCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	FString CurrencySymbol{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	int32 Balance{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	FString BalanceOrigin{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	TArray<FAccelByteModelsTimeLimitedBalance> TimeLimitedBalances{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	EAccelByteItemStatus Status{EAccelByteItemStatus::NONE};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCreditUserWalletRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | CreditUserWalletRequest")
	int32 Amount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | CreditUserWalletRequest")
	EAccelByteCreditUserWalletSource Source{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | CreditUserWalletRequest")
	FString Reason{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | CreditUserWalletRequest")
	EAccelByteWalletTable Origin{EAccelByteWalletTable::System};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDebitUserWalletRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | CreditUserWalletRequest")
	int32 Amount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | CreditUserWalletRequest")
	FString Reason{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsWalletTransactionInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionInfo")
		FString WalletId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionInfo")
		int32 Amount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionInfo")
		FString Reason{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionInfo")
		FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionInfo")
		FString UserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionInfo")
		FString Operator{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionInfo")
		EAccelByteWalletAction WalletAction{EAccelByteWalletAction::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionInfo")
		FString CurrencyCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionInfo")
		FString BalanceSource{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionInfo")
		FString CreatedAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionInfo")
		FString UpdatedAt{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsWalletTransactionPaging
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionPaging")
		TArray<FAccelByteModelsWalletTransactionInfo> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionPaging")
		FAccelByteModelsPaging Paging;
};

#pragma endregion WalletModelsField

#pragma region FulfillmentModelsField

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsFulfillmentRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentRequest")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentRequest")
	int32 Quantity{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentRequest")
	FString OrderNo{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentRequest")
	EAccelByteItemSource Source{EAccelByteItemSource::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentRequest")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentRequest")
	FString Language{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsFulFillCodeRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulFillCodeRequest")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulFillCodeRequest")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulFillCodeRequest")
	FString Language{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementSummary
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	EAccelByteEntitlementClass Clazz{EAccelByteEntitlementClass::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	EAccelByteEntitlementType Type{EAccelByteEntitlementType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	bool Stackable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	int32 StackedUseCount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	int32 StackedQuantity{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	FString CreatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	FString GrantedCode{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCreditSummary
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | CreditSummary")
	FString WalletId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | CreditSummary")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | CreditSummary")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | CreditSummary")
	int32 Amount{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsFulfillmentResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentResult")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentResult")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentResult")
	TArray<FAccelByteModelsEntitlementSummary> EntitlementSummaries{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentResult")
	TArray<FAccelByteModelsCreditSummary> CreditSummaries{};
};

#pragma endregion FulfillmentModelsField

#pragma region CurrencyModelsField

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCurrencyList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Currency | Models | CurrencyList")
	FString CurrencyCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Currency | Models | CurrencyList")
	TMap<FString, FString> LocalizationDescriptions{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Currency | Models | CurrencyList")
	FString CurrencySymbol{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Currency | Models | CurrencyList")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Currency | Models | CurrencyList")
	EAccelByteCurrencyType CurrencyType{EAccelByteCurrencyType::NONE}; // ['REAL', 'VIRTUAL'],

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Currency | Models | CurrencyList")
	int32 Decimals{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Currency | Models | CurrencyList")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Currency | Models | CurrencyList")
	FDateTime UpdateAt{0};
}; 

#pragma endregion CurrencyModelsField

#pragma region RewardModelsField

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRewardItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardItem")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardItem")
	int32 Quantity{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRewardCondition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardCondition")
	FString ConditionName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardCondition")
	FString Condition{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardCondition")
	FString EventName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardCOndition")
	TArray<FAccelByteModelsRewardItem> RewardItems{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRewardInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardInfo")
	FString RewardId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardInfo")
	FString RewardCode{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardInfo")
	FString Description{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardInfo")
	FString EventTopic{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardInfo")
	TArray<FAccelByteModelsRewardCondition> RewardConditions{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardInfo")
	int32 MaxAwarded{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardInfo")
	int32 MaxAwardedPerUser{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardInfo")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardInfo")
	FDateTime UpdatedAt{0};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsQueryRewardPaging
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | QueryRewardPaging")
	FString Previous{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | QueryRewardPaging")
	FString Next{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsQueryReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | QueryReward")
	TArray<FAccelByteModelsRewardInfo> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | QueryReward")
	FAccelByteModelsQueryRewardPaging Paging{};
};

#pragma endregion RewardModelsField

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlatformPredicateValidateResults
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Platform")
	FString PredicateName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Platform")
	FString Validated{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Platform")
	TArray<FString> Matched{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Platform")
	TArray<FString> Unmatched{}; 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlatformValidateDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Platform")
	TArray<FAccelByteModelsPlatformPredicateValidateResults> PredicateValidateResults{}; 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlatformValidateUserItemPurchaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Platform")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Platform")
	FString Sku{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Platform")
	bool Purchasable{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Platform")
	TArray<FAccelByteModelsPlatformValidateDetails> ValidateDetails{};  
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlatformStore
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	FString StoreId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	FString Title{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	bool Published{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	TArray<FString> SupportedLanguages{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	TArray<FString> SupportedRegions{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	FString DefaultRegion{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	FString DefaultLanguage{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	FDateTime PublishedTime{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	FDateTime CreatedAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	FDateTime UpdatedAt{};
};