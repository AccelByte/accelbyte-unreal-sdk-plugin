// Copyright (c) 2019 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"
#include "Models/AccelByteGeneralModels.h" 
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
	OPTIONBOX,
	EXTENSION,
	LOOTBOX
};

UENUM(BlueprintType)
enum class EAccelByteItemStatus : uint8
{
	NONE = 0,
	ACTIVE,
	INACTIVE
};

UENUM(BlueprintType)
enum class EAccelByteWalletStatus : uint8
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
	SUBSCRIPTION, 
	MEDIA,
	OPTIONBOX,
	LOOTBOX
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
	REWARD,
	GIFT,
	DLC,
	SELL_BACK,
	CONSUME_ENTITLEMENT,
	ORDER_REVOCATION,
	PAYMENT,
	EXPIRATION,
	IAP_CHARGEBACK_REVERSED,
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
enum class EAccelByteDebitUserWalletSource : uint8
{
	OTHER = 0,
	ORDER_REVOCATION,
	DLC_REVOCATION,
	IAP_REVOCATION,
	PAYMENT,
	EXPIRATION,
	TRADE,
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
enum class EAccelBytePredicateType : uint8
{
	EntitlementPredicate,
	SeasonPassPredicate,
	SeasonTierPredicate,
	StatisticCodePredicate
};

UENUM(BlueprintType)
enum class EAccelByteLootBoxRewardType : uint8
{
	PROBABILITY_GROUP,
	REWARD_GROUP,
	REWARD
};

UENUM(BlueprintType)
enum class EAccelByteSubscriptionSummaryStatus : uint8
{
	NONE = 0,
	INIT,
	ACTIVE,
	CANCELLED,
	EXPIRED 
};

UENUM(BlueprintType)
enum class EAccelByteSubscriptionSummarySubscribedBy : uint8
{
	NONE = 0,
	USER,
	PLATFORM 
};

UENUM(BlueprintType)
enum class EAccelByteEntitlementItemIdentityType : uint8
{
	NONE = 0,
	ITEM_ID,
	ITEM_SKU 
};
UENUM(BlueprintType)
enum class EAccelByteEntitlementIAPOrderStatus : uint8
{
	NONE = 0,
	VERIFIED,
	FULFILLED,
	FAILED
};

UENUM(BlueprintType)
enum class EAccelBytePlatformRewardType : uint8
{
	NONE = 0,
	ITEM,
	CURRENCY  
};

UENUM(BlueprintType)
enum class EAccelBytePlatformRewardSource : uint8
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
	DLC,
	OTHER,
	SELL_BACK,
	CONSUME_ENTITLEMENT,
	ORDER_REVOCATION,
	DLC_REVOCATION,
	PAYMENT,
	EXPIRATION
};

UENUM(BlueprintType)
enum class EAccelBytePlatformRewardOrigin : uint8
{
	NONE = 0,
	Playstation,
	Xbox,
	Steam,
	Epic,
	IOS,
	GooglePlay,
	Twitch,
	Nintendo,
	Oculus,
	System,
	Other
};

UENUM(BlueprintType)
enum class EAccelBytePlatformBalanceOrigin : uint8
{
	NONE = 0,
	Playstation,
	Xbox,
	Steam,
	Epic,
	IOS,
	GooglePlay,
	Twitch,
	Nintendo,
	Oculus,
	System,
	Other
};

UENUM(BlueprintType)
enum class EAccelByteWalletPlatform : uint8
{
	NONE = 0,
	Playstation,
	Xbox,
	Steam,
	Epic,
	IOS,
	GooglePlay,
	Nintendo,
	Oculus,
	Other
};

UENUM(BlueprintType)
enum class EAccelBytePaymentProvider : uint8
{
	NONE = 0,
	WALLET,
	XSOLLA,
	ADYEN,
	STRIPE,
	CHECKOUT,
	ALIPAY,
	WXPAY,
	PAYPAL
};

UENUM(BlueprintType)
enum class EAccelByteUserEntitlementAction : uint8
{
	NONE = 0,
	GRANT,
	UPDATE,
	DECREMENT,
	REVOKE,
	DISABLE,
	ENABLE,
	SELL_BACK
};

UENUM(BlueprintType)
enum class EAccelByteUserEntitlementOrigin : uint8
{
	NONE = 0,
	Playstation,
	Xbox,
	Steam,
	Epic,
	IOS,
	GooglePlay,
	Twitch,
	Nintendo,
	Oculus,
	System,
	Other
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
	int32 AnyOf{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FString> Values{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Value{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Code{};
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelItemConditionGroup
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FAccelByteModelItemPredicate> Predicates{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Operator{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelItemPurchaseCondition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FAccelByteModelItemConditionGroup> ConditionGroups{};
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
	FString ItemType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	int32 Duration{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FDateTime EndDate{0};

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
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemLootBoxItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemLootBoxItem")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemLootBoxItem")
	FString ItemSku{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemLootBoxItem")
	FString ItemType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemLootBoxItem")
	int32 Duration{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemLootBoxItem")
	FDateTime EndDate{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemLootBoxItem")
	int32 Count{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemReward
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemReward")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemReward")
	EAccelByteLootBoxRewardType Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemReward")
	TArray<FAccelByteModelsItemLootBoxItem> LootBoxItems{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemReward")
	int32 Weight{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemReward")
	int32 Odds{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemLootBoxConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | LootBoxConfig")
	int32 RewardCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | LootBoxConfig")
	TArray<FAccelByteModelsItemReward> Rewards{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemSaleConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | SaleConfig")
	FString CurrencyCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | SaleConfig")
	int32 Price{};
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
	bool SectionExclusive{};
	
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
	TMap<FString, int32> ItemQty{};

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
	bool Fresh{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo", Meta=(ScriptName="sellable(Deprecated)"))
	bool bSellable{}; // obsolete, please use Sellable instead  

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	bool Sellable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FAccelByteModelsItemSaleConfig SaleConfig{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FJsonObjectWrapper LocalExt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FAccelByteModelsItemLootBoxConfig LootBoxConfig{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	bool Flexible{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPopulatedItemInfo : public FAccelByteModelsItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | FAccelByteModelsPopulatedItemInfo")
	TArray<FAccelByteModelsItemInfo> Items{};
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

	/** @brief Default == false. Can change to "true". Anything != "true" will remain default */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemCriteria")
	bool IncludeSubCategoryItem{};

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
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementSyncBase
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementSyncBase ")
		FString ProductId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementSyncBase ")
		int32 Price{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementSyncBase ")
		FString CurrencyCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementSyncBase ")
		FString XstsToken{}; // Used for Xbox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementSyncBase ")
		int32 ServiceLabel = 0; // Used for PS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementSyncBase ")
		FString Region{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementSyncBase ")
		FString Language{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementSyncBase ")
		FString EpicGamesJwtToken{};//Used for EpicGames
};

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
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | Reward ")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | Reward ")
	FString ItemSku{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | Reward ")
	int32 Count{};
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FString RequestId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	bool Replayed{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	TArray<FAccelByteModelsEntitlementReward> Rewards{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | EntitlementPagingSlicedResult | EntitlementInfo ")
	FAccelByteModelsItemLootBoxConfig LootBoxConfig{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBaseUserEntitlementHistory
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | BaseUserEntitlementHistory ")
	FString EntitlementId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | BaseUserEntitlementHistory ")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | BaseUserEntitlementHistory ")
	EAccelByteUserEntitlementAction Action{ EAccelByteUserEntitlementAction::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | BaseUserEntitlementHistory ")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | BaseUserEntitlementHistory ")
	int32 UseCount{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | BaseUserEntitlementHistory ")
	int32 UseCountChange{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | BaseUserEntitlementHistory ")
	FString Reason{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | BaseUserEntitlementHistory ")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | BaseUserEntitlementHistory ")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | BaseUserEntitlementHistory ")
	EAccelByteUserEntitlementOrigin Origin{ EAccelByteUserEntitlementOrigin::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | BaseUserEntitlementHistory ")
	EAccelByteEntitlementClass Clazz{ EAccelByteEntitlementClass::NONE };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | BaseUserEntitlementHistory ")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | BaseUserEntitlementHistory ")
	FString Sku{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserEntitlementHistoryPagingResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | UserEntitlementHistoryPagingResult ")
	TArray<FAccelByteModelsBaseUserEntitlementHistory> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | UserEntitlementHistoryPagingResult ")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserEntitlementHistory : public FAccelByteModelsBaseUserEntitlementHistory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | UserEntitlementHistory ")
	FString Operator{};
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | ConsumeUserEntitlementRequest")
	FString RequestId{};
	
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
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayStationIAPSync
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationIAPSync")
	FString ProductId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationIAPSync")
	int32 Price{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationIAPSync")
	FString CurrencyCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationIAPSync")
	int32 ServiceLabel{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMultipleServicePSNDLCSync
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationMultipleServiceDLCSync")
	TArray<int32> ServiceLabel{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMultipleServicePSNIAPSync
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationMultipleServiceIAPSync")
	FString ProductId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationMultipleServiceIAPSync")
	int32 Price{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationMultipleServiceIAPSync")
	FString CurrencyCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationMultipleServiceIAPSync")
	TArray<int32> ServiceLabel{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMultipleServicePlayStationInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationMultipleServiceIAPSync")
	FString TransactionId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationMultipleServiceIAPSync")
	FString PsnItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationMultipleServiceIAPSync")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationMultipleServiceIAPSync")
	FString Sku{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | PlayStationMultipleServiceIAPSync")
	EAccelByteEntitlementIAPOrderStatus IAPOrderStatus{EAccelByteEntitlementIAPOrderStatus::NONE};
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
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderBundleItemInfo
{
	GENERATED_BODY() 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderBundleItemInfo")
	int32 Price{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderBundleItemInfo")
	int32 DiscountedPrice{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderBundleItemInfo")
	FString ItemName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderBundleItemInfo")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderBundleItemInfo")
	FString ItemSku{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderBundleItemInfo")
	int32 Quantity{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderBundleItemInfo")
	bool Purchased{};
};

UENUM(BlueprintType)
enum class EAccelByteDeductionType : uint8
{
	NONE = 0,
	DISCOUNT_CODE
};

UENUM(BlueprintType)
enum class EDiscountType : uint8
{
	NONE = 0,
	AMOUNT,
	PERCENTAGE
};

UENUM(BlueprintType)
enum class ERestrictType : uint8
{
	NONE = 0,
	ITEMS_AND_CATEGORIES
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FDiscountItemModel
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountItem")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountItem")
	FString ItemName{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FDiscountCategoryModel
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountCategory")
	FString CategoryPath{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountCategory")
	bool IncludeSubCategories{false};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDiscountConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountConfig")
	FString CurrencyNamespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountConfig")
	FString CurrencyCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountConfig")
	EDiscountType DiscountType{EDiscountType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountConfig")
	int32 DiscountPercentage{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountConfig")
	int32 DiscountAmount{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountConfig")
	bool Stackable{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountConfig")
	ERestrictType RestrictType{ERestrictType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountConfig")
	TArray<FDiscountItemModel> Items{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountConfig")
	TArray<FDiscountCategoryModel> Categories{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDiscountCodeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountCodeDeductionDetail")
	FString Code{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountCodeDeductionDetail")
	FString CampaignName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountCodeDeductionDetail")
	FString CampaignId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountCodeDeductionDetail")
	int32 Deduction{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountCodeDeductionDetail")
	FAccelByteModelsDiscountConfig DiscountConfig{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDiscountCodeDeductionDetail
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountCodeDeductionDetail")
	int32 TotalDeduction{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountCodeDeductionDetail")
	int32 TotalPercentageDeduction{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountCodeDeductionDetail")
	int32 TotalAmountDeduction{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountCodeDeductionDetail")
	TArray<FAccelByteModelsDiscountCodeInfo> DiscountPercentageCodes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DiscountCodeDeductionDetail")
	TArray<FAccelByteModelsDiscountCodeInfo> DiscountAmountCodes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDeductionDetail
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DeductionDetail")
	EAccelByteDeductionType	DeductionType{EAccelByteDeductionType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | DeductionDetail")
	FAccelByteModelsDiscountCodeDeductionDetail DiscountCodeDeductionDetail{};
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
	EAccelBytePaymentProvider PaymentProvider{EAccelBytePaymentProvider::NONE};

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
	FDateTime ExpireTime{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 PaymentRemainSeconds{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FJsonObjectWrapper Ext{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 TotalTax{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 TotalPrice{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 SubtotalPrice{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	TArray<FAccelByteModelsOrderBundleItemInfo> OrderBundleItemInfos{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 Deduction{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	TArray<FAccelByteModelsDeductionDetail> DeductionDetails{};
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
	FString SectionId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
	TArray<FString> DiscountCodes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderSummary
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderSummary")
	FAccelByteModelsOrderCurrencySummary Currency{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Order | Models | OrderSummary")
	FJsonObjectWrapper Ext{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderSummary")
	bool Free{};
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
	int64 Balance{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | TimeLimitedBalance")
	FString BalanceSource{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | TimeLimitedBalance")
	FDateTime ExpiredAt{0}; //WARNING: this field is obsolete and will be deprecated at November 2024

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | TimeLimitedBalance")
	FDateTime ExpireAt{0};

	// Avoid breaking change by duplicate the value to the old member name
	void DuplicateExpiredAtValue()
	{
		ExpiredAt = ExpireAt;
	}
	
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
	int64 Balance{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	FString BalanceOrigin{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	TArray<FAccelByteModelsTimeLimitedBalance> TimeLimitedBalances{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	FDateTime UpdatedAt{0};
 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	int64 TotalPermanentBalance{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	int64 TotalTimeLimitedBalance{};
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletInfo")
	EAccelByteItemStatus Status{ EAccelByteItemStatus::NONE };

	void DuplicateTimeLimitedBalancesExpirationInfo()
	{
		for (int i = 0; i < TimeLimitedBalances.Num(); i++)
		{
			TimeLimitedBalances[i].DuplicateExpiredAtValue();
		}
	}
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlatformWallet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | PlatformWallet")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | PlatformWallet")
	FString Namespace{}; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | PlatformWallet")
	FString UserId{}; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | PlatformWallet")
	FString CurrencyCode{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | PlatformWallet")
	FString CurrencySymbol{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | PlatformWallet")
	int64 Balance{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | PlatformWallet")
	TArray<FAccelByteModelsWalletInfo> WalletInfos{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | PlatformWallet")
	EAccelByteWalletStatus WalletStatus{EAccelByteWalletStatus::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | PlatformWallet")
	EAccelByteWalletStatus Status{EAccelByteWalletStatus::NONE};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsWalletInfoResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletResponse")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletResponse")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletResponse")
	FString CurrencyCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletResponse")
	FString CurrencySymbol{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletResponse")
	int64 Balance{}; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletResponse")
	TArray<FAccelByteModelsWalletInfo> WalletInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletResponse")
	EAccelByteItemStatus WalletStatus{EAccelByteItemStatus::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletResponse")
	EAccelByteItemStatus Status{ EAccelByteItemStatus::NONE };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletResponse")
	FString Id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCreditUserWalletRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | CreditUserWalletRequest")
	int64 Amount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | CreditUserWalletRequest")
	EAccelByteCreditUserWalletSource Source{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | CreditUserWalletRequest")
	FString Reason{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | CreditUserWalletRequest")
	EAccelByteWalletTable Origin{EAccelByteWalletTable::System};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsWalletCreditResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletCreditResponse")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletCreditResponse")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletCreditResponse")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletCreditResponse")
	FString CurrencySymbol {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletCreditResponse")
	FString CurrencyCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletCreditResponse")
	int64 Balance{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletCreditResponse")
	FString BalanceOrigin {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletCreditResponse")
	TArray<FAccelByteModelsTimeLimitedBalance> timeLimitedBalances {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletCreditResponse")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletCreditResponse")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletCreditResponse")
	int64 TotalPermanentBalance{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletCreditResponse")
	int64 TotalTimeLimitedBalance {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletCreditResponse")
	EAccelByteItemStatus Status{ EAccelByteItemStatus::NONE };
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDebitUserWalletRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | DebitUserWalletRequest")
	int64 Amount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | DebitUserWalletRequest")
	FString Reason{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDebitUserWalletRequestV2 : public FAccelByteModelsDebitUserWalletRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | DebitUserWalletRequest")
	EAccelByteDebitUserWalletSource BalanceSource { EAccelByteDebitUserWalletSource::OTHER };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | DebitUserWalletRequest")
	FJsonObjectWrapper Metadata{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | DebitUserWalletRequest")
	EAccelBytePlatformBalanceOrigin BalanceOrigin { EAccelBytePlatformBalanceOrigin::Other };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | DebitUserWalletRequest")
	bool AllowOverdraft{false};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaymentUserWalletRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | PaymentUserWalletRequest")
	int64 Amount{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | PaymentUserWalletRequest")
	EAccelByteWalletPlatform WalletPlatform { EAccelByteWalletPlatform::NONE };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | PaymentUserWalletRequest")
	FJsonObjectWrapper Metadata{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsWalletTransactionInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionInfo")
		FString WalletId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Wallet | Models | WalletTransactionInfo")
		int64 Amount{};
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentRequest")
	FString StoreId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentRequest")
	FString ItemSku{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentRequest")
	FAccelByteModelsOrderSummary Order{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentRequest")
	FDateTime StartDate{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentRequest")
	FDateTime EndDate{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentRequest")
	int32 Duration{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Fulfillment | Models | FulfillmentRequest")
	EAccelBytePlatformRewardOrigin Origin{EAccelBytePlatformRewardOrigin::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Fulfillment | Models | FulfillmentRequest")
	FJsonObjectWrapper Metadata{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Fulfillment | Models | FulfillmentRequest")
	FJsonObjectWrapper OverrideBundleItemQty{};
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
	FString Name{};

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
	FString StoreId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	int32 StackedQuantity{}; // deprecated 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	FString CreatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	FString GrantedCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	FDateTime StartDate{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | EntitlementSummary")
	FDateTime EndDate{0};
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | CreditSummary")
	FString CurrencyCode{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSubscriptionSummary
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | SubscriptionSummary")
	FString Id{}; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | SubscriptionSummary")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | SubscriptionSummary")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | SubscriptionSummary")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | SubscriptionSummary")
	FString Sku{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | SubscriptionSummary")
	EAccelByteSubscriptionSummaryStatus Status{EAccelByteSubscriptionSummaryStatus::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | SubscriptionSummary")
	FDateTime CurrentPeriodStart {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | SubscriptionSummary")
	FDateTime CurrentPeriodEnd {0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | SubscriptionSummary")
	EAccelByteSubscriptionSummarySubscribedBy SubscribedBy{EAccelByteSubscriptionSummarySubscribedBy::NONE};
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Fulfillment | Models | FulfillmentResult")
	TArray<FAccelByteModelsSubscriptionSummary> SubscriptionSummaries{};
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardItem")
	int32 Duration{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardItem")
	FDateTime EndDate{0};
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardInfo")
	FString UserIdExpression{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Reward | Models | RewardInfo")
	FString NamespaceExpression{};
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
	FDateTime PublishedTime{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	FDateTime CreatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	FDateTime UpdatedAt{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemCriteriaV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	FString StoreId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	FString CategoryPath{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	bool IncludeSubCategoryItem = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	EAccelByteItemType ItemType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	EAccelByteAppType AppType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	FString BaseAppId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	TArray<FString> Features{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	bool ActiveOnly = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	FString Region{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	FDateTime AvailableDate{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	FString TargetNamespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	int32 Offset = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	int32 Limit = 20;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	TArray<EAccelByteItemListSortBy> SortBy{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemCriteriaV3
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	FString StoreId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	FString CategoryPath{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	bool IncludeSubCategoryItem = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	EAccelByteItemType ItemType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	EAccelByteAppType AppType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	FString BaseAppId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	TArray<FString> Tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	TArray<FString> Features{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	EAccelByteItemStatus ItemStatus{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	FString Region{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	FDateTime AvailableDate{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	FString TargetNamespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	FString ItemName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	bool bSectionExclusive{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	int32 Offset{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	int32 Limit{20};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | ItemRequest")
	TArray<EAccelByteItemListSortBy> SortBy{}; 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemInventoryConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	int32 SlotUsed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FJsonObjectWrapper ServerCustomAttributes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FJsonObjectWrapper CustomAttributes{};
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemInfoV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString AppId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString AppType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	EAccelByteSeasonType SeasonType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString BaseAppId {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString Sku{};
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
	TArray<FAccelByteModelsItemImage> Images{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString ThumbnailUrl{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FJsonObjectWrapper Localizations{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	EAccelByteItemStatus Status{EAccelByteItemStatus::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	bool Listable{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	bool Purchasable{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	bool bSectionExclusive{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	bool Flexible{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	EAccelByteItemType ItemType{EAccelByteItemType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString TargetNamespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString TargetCurrencyCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString TargetItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FJsonObjectWrapper RegionData{}; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FAccelByteModelsItemRecurring Recurring{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TArray<FString> ItemIds{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FJsonObjectWrapper ItemQty{}; 
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
	FJsonObjectWrapper Ext{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FString BoothName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	int32 DisplayOrder{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FDateTime CreatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FDateTime UpdatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FAccelByteModelItemPurchaseCondition PurchaseCondition{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FAccelByteModelItemOptionBoxConfig OptionBoxConfig{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FAccelByteModelsItemLootBoxConfig LootBoxConfig{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo", Meta=(ScriptName= "sellable(Deprecated)"))
	bool bSellable{}; // obsolete, please use Sellable instead  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	bool Sellable{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FAccelByteModelsItemSaleConfig SaleConfig{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	FAccelByteModelsItemInventoryConfig InventoryConfig{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemPagingSlicedResultV2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemPagingSlicedResult")
	TArray<FAccelByteModelsItemInfoV2> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Item | Models | ItemPagingSlicedResult")
	FAccelByteModelsPaging Paging {};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSectionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	FString Title{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	FString Description{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	FString LongDescription{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	FString SectionId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	FString ViewId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	bool Active{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	FDateTime StartDate{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	FDateTime EndDate{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	TArray<FAccelByteModelsItemInfo> CurrentRotationItems{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	FDateTime CurrentRotationExpireAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	int32 DisplayOrder{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
    FJsonObjectWrapper Ext{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | SectionInfo")
	FJsonObjectWrapper LocalExt{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsViewInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | ViewInfo")
	FString Title{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | ViewInfo")
	FString Description{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | ViewInfo")
	FString LongDescription{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | ViewInfo")
	FString ViewId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | ViewInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | ViewInfo")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | ViewInfo")
	int32 DisplayOrder{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | ViewInfo")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | ViewInfo")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Section | Models | ViewInfo")
	FJsonObjectWrapper LocalExt{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSellUserEntitlementRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | SellUserEntitlementRequest")
	int32 UseCount{}; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Entitlements | Models | SellUserEntitlementRequest")
	FString RequestId{};
	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSellItemEntitlementInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | SellItemEntitlementInfo ")
	FString RequestId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | SellItemEntitlementInfo ")
	bool bReplayed{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | SellItemEntitlementInfo ")
	TArray<FAccelByteModelsCreditSummary> CreditSummaries{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | SellItemEntitlementInfo ")
	FAccelByteModelsEntitlementInfo EntitlementInfo{}; 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSyncOculusConsumableEntitlementInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | SyncOculusConsumableEntitlementInfo ")
	FString TransactionId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | SyncOculusConsumableEntitlementInfo ")
	FString OculusItemSku{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | SyncOculusConsumableEntitlementInfo ")
	EAccelByteEntitlementItemIdentityType ItemIdentityType{EAccelByteEntitlementItemIdentityType::NONE};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | SyncOculusConsumableEntitlementInfo ")
	FString ItemIdentity{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Entitlement | Models | SyncOculusConsumableEntitlementInfo ")
	EAccelByteEntitlementIAPOrderStatus IAPOrderStatus{EAccelByteEntitlementIAPOrderStatus::NONE};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlatformRewardItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PlatformRewardItem")
	FString ItemType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PlatformRewardItem")
	FString ItemId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PlatformRewardItem")
	FString ItemSku{}; 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlatformRewardCurrency
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PlatformRewardCurrency")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PlatformRewardCurrency")
	FString CurrencyCode{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlatformReward
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PlatformReward")
	EAccelBytePlatformRewardType Type{EAccelBytePlatformRewardType::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PlatformReward")
	FAccelByteModelsPlatformRewardItem Item{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PlatformReward")
	FAccelByteModelsPlatformRewardCurrency Currency{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PlatformReward")
	int32 Quantity{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsFulfillRewards
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | FulfillRewards")
	TArray<FAccelByteModelsPlatformReward> Rewards{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | FulfillRewards")
	EAccelBytePlatformRewardSource Source{EAccelBytePlatformRewardSource::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | FulfillRewards")
	EAccelBytePlatformRewardOrigin Origin{EAccelBytePlatformRewardOrigin::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | FulfillRewards")
	FJsonObjectWrapper Metadata{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPriceDetail
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PriceDetail")
	int32 Price{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PriceDetail")
	int32 DiscountedPrice{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PriceDetail")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PriceDetail")
	FString ItemSku{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PriceDetail")
	int32 Quantity{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PriceDetail")
	bool Owned{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PriceDetail")
	FString ItemName{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEstimatedPrice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | EstimatedPrice")
	FString CurrencyCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | EstimatedPrice")
	FString CurrencyNamespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | EstimatedPrice")
	int32 Price{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | EstimatedPrice")
	int32 DiscountedPrice{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | EstimatedPrice")
	TArray<FAccelByteModelsPriceDetail> PriceDetails{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEstimatedPrices
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	FString Region{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | Store")
	TArray<FAccelByteModelsEstimatedPrice> EstimatedPrices{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserOrdersRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserOrderRequest")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserOrderRequest")
	EAccelByteOrderStatus Status{ EAccelByteOrderStatus::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserOrderRequest")
	int32 Offset{0};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserOrderRequest")
	int32 Limit{20};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | UserOrderRequest")
	bool Discounted{false};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserPreviewOrderRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PreviewOrderRequest")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PreviewOrderRequest")
	int32 Quantity{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PreviewOrderRequest")
	int32 Price{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PreviewOrderRequest")
	int32 DiscountedPrice{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PreviewOrderRequest")
	FString CurrencyCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PreviewOrderRequest")
	TArray<FString> DiscountCodes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserPreviewOrderResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PreviewOrderResponse")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PreviewOrderResponse")
	int32 Quantity{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PreviewOrderResponse")
	int32 Price{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PreviewOrderResponse")
	int32 DiscountedPrice{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PreviewOrderResponse")
	int32 Deduction{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PreviewOrderResponse")
	TArray<FAccelByteModelsDeductionDetail> DeductionDetails{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Achievements | Models | PreviewOrderResponse")
	int32 FinalPrice{0};
};

// if updated, need to also update EAccelBytePlatformMappingInternal also
UENUM(BlueprintType)
enum class EAccelBytePlatformMapping : uint8
{
	NONE = 0,
	APPLE,
	GOOGLE,
	PLAYSTATION,
	STEAM,
	XBOX,
	EPIC_GAMES,
	OCULUS,
	TWITCH,
	STADIA,
};

// if updated, need to also update EAccelBytePlatformMapping
UENUM(BlueprintType)
enum class EAccelBytePlatformMappingInternal : uint8
{
	NONE = 0,
	APPLE,
	GOOGLE,
	PLAYSTATION,
	STEAM,
	XBOX,
	EPICGAMES,
	OCULUS,
	TWITCH,
	STADIA,
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemMapping
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | IAP | Models | ItemMapping")
	EAccelBytePlatformMapping Platform{EAccelBytePlatformMapping::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | IAP | Models | ItemMapping")
	FString ItemIdentityType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | IAP | Models | ItemMapping")
	FString ItemIdentity{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | IAP | Models | ItemMapping")
	FString PlatformProductId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemMappingsResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | IAP | Models | ItemMappingsResponse")
	TArray<FAccelByteModelsItemMapping> Data;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemMappingInternal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | IAP | Models | ItemMapping")
	EAccelBytePlatformMappingInternal Platform {EAccelBytePlatformMappingInternal::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | IAP | Models | ItemMapping")
	FString ItemIdentityType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | IAP | Models | ItemMapping")
	FString ItemIdentity{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | IAP | Models | ItemMapping")
	FString PlatformProductId{};
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemMappingsResponseInternal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | IAP | Models | ItemMappingsResponse")
	TArray<FAccelByteModelsItemMappingInternal> Data;
};
