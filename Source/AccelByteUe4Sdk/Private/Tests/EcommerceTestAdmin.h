// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"
#include "Models/AccelByteEcommerceModels.h"
#include "EcommerceTestAdmin.generated.h"

#pragma region ECOMMERCE_MODEL

USTRUCT(BlueprintType)
struct FCurrencyCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
	FString currencyCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
	TMap<FString, FString> localizationDescriptions{}; // "Language" : "Description"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
	FString currencySymbol{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
	EAccelByteItemCurrencyType currencyType{EAccelByteItemCurrencyType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
	int decimals{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
	int maxAmountPerTransaction{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
	int maxTransactionAmountPerDay{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
	int maxBalanceAmount{};
};

USTRUCT(BlueprintType)
struct FStoreCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	FString title{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	FString description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	TArray<FString> supportedLanguages{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	TArray<FString> supportedRegions{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	FString defaultLanguage{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	FString defaultRegion{};
};

USTRUCT(BlueprintType)
struct FStoreInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	FString storeId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	FString title{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	FString description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	bool published{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	TArray<FString> supportedLanguages{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	TArray<FString> supportedRegions{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	FString defaultRegion{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	FString defaultLanguage{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	FString createdAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
	FString updatedAt{};
};

USTRUCT(BlueprintType)
struct FCategoryInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
	FString parentCategoryPath{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
	FString categoryPath{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
	FString createdAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
	FString updatedAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
	TMap<FString, FString> localizationDisplayNames{}; // "Language" : "Description"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
	bool root{};
};

USTRUCT(BlueprintType)
struct FCategoryCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
	FString categoryPath{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
	TMap<FString, FString> localizationDisplayNames{}; // "Language" : "Description"
};

USTRUCT(BlueprintType)
struct FLocalExt
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	TArray<FString> properties{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	TArray<FString> functions{};
};

USTRUCT(BlueprintType)
struct FLocalization
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString title{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString longDescription{};
};

USTRUCT(BlueprintType)
struct FCreateRegionDataItem
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
	FString PurchaseAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	FString ExpireAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	FString DiscountPurchaseAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
	FString DiscountExpireAt{};
};

USTRUCT(BlueprintType)
struct FRegionDataUS
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	TArray<FCreateRegionDataItem> US{};
};

USTRUCT(BlueprintType)
struct FRecurring
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	EAccelByteSubscriptionCycle cycle{EAccelByteSubscriptionCycle::WEEKLY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	int fixedFreeDays{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	int fixedTrialCycles{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	int graceDays{1}; // min val = 1
};

USTRUCT(BlueprintType)
struct FItemCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	EAccelByteItemType itemType{EAccelByteItemType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	EAccelByteSeasonType SeasonType{EAccelByteSeasonType::PASS};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	EAccelByteEntitlementType entitlementType{EAccelByteEntitlementType::NONE}; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	int useCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	bool stackable{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString appId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	EAccelByteAppType appType{EAccelByteAppType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString baseAppId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString targetCurrencyCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString targetNamespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString categoryPath{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	TMap<FString, FLocalization> localizations{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	EAccelByteItemStatus status{EAccelByteItemStatus::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString sku{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	TArray<FAccelByteModelsItemImage> images{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString thumbnailUrl{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FRegionDataUS regionData{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	TArray<FString> itemIds{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	TArray<FString> tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	int maxCountPerUser{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	int maxCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString boothName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	int displayOrder{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString clazz{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FRecurring recurring{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TMap<FString, int32> ItemQty{};
};

USTRUCT(BlueprintType)
struct FItemFullInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString itemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString appId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	EAccelByteAppType appType{EAccelByteAppType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString sku{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	EAccelByteEntitlementType entitlementType{EAccelByteEntitlementType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	int useCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString categoryPath{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	TMap<FString, FLocalization> localizations{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	EAccelByteItemStatus status{EAccelByteItemStatus::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	EAccelByteItemType itemType{EAccelByteItemType::NONE};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString targetCurrencyCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString targetNamespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FAccelByteModelsItemRegionDataItem regionData{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	TArray<FString> itemIds{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	TArray<FString> tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	int maxCountPerUser{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	int maxCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString clazz{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString ext{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	TArray<FString> codeFiles{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString createdAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
	FString updatedAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo")
	TMap<FString, int32> ItemQty{};
};

USTRUCT(BlueprintType)
struct FRedeemableItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | RedeemableItem")
	FString itemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | RedeemableItem")
	FString itemName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | RedeemableItem")
	int32 quantity{};
};

USTRUCT(BlueprintType)
struct FCampaignCreateModel
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
	FString name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
	FString description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
	TArray<FString> tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
	FString status{}; // ACTIVE/ INACTIVE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
	int32 maxRedeemCountPerCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
	int32 maxRedeemCountPerCodePerUser{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
	int32 maxRedeemCountPerCampaignPerUser{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
	int32 maxSaleCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
	FString redeemStart{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
	FString redeemEnd{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
	FString redeemType{}; // ITEM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
	TArray<FRedeemableItem> items{};
};

USTRUCT(BlueprintType)
struct FCampaignUpdateModel
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
	FString name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
	FString description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
	TArray<FString> tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
	FString status{}; // ACTIVE/ INACTIVE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
	int32 maxRedeemCountPerCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
	int32 maxRedeemCountPerCodePerUser{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
	int32 maxRedeemCountPerCampaignPerUser{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
	int32 maxSaleCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
	FString redeemStart{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
	FString redeemEnd{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
	FString redeemType{}; // ITEM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
	TArray<FRedeemableItem> items{};
};

USTRUCT(BlueprintType)
struct FCampaignInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	FString id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	FString type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	FString name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	FString description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	TArray<FString> tags{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	FString status{}; // ACTIVE/ INACTIVE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	FDateTime redeemStart{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	FDateTime redeemEnd{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	int32 maxRedeemCountPerCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	int32 maxRedeemCountPerCodePerUser{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	int32 maxRedeemCountPerCampaignPerUser{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	int32 maxSaleCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	FString redeemType{}; // ITEM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	TArray<FRedeemableItem> items{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	FString boothName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	FDateTime createdAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
	FDateTime updatedAt{};
};

USTRUCT(BlueprintType)
struct FCampaignPagingSlicedResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignPaging")
	TArray<FCampaignInfo> data{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignPaging")
	FAccelByteModelsPaging paging{};
};

USTRUCT(BlueprintType)
struct FCampaignCodeCreateModel
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCodeCreateModel")
	int32 quantity{};
};

USTRUCT(BlueprintType)
struct FCampaignCodeCreateResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCodeCreateResult")
	int32 numCreated{};
};

USTRUCT(BlueprintType)
struct FCodeInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	FString id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	FString type{}; // REDEMPTION
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	FString campaignId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	FString value{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	int32 maxRedeemCountPerCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	int32 maxRedeemCountPerCodePerUser{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	int32 maxRedeemCountPerCampaignPerUser{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	int32 remainder{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	int32 redeemedCount{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	FString status{}; // ACTIVE / INACTIVE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	FDateTime redeemStart{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	FDateTime redeemEnd{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	FString redeemType{}; // ITEM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	TArray<FRedeemableItem> items{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	int32 batchNo{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	FString acquireOrderNo{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	FString acquireUserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	FDateTime createdAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
	FDateTime updatedAt{};
};

USTRUCT(BlueprintType)
struct FCodeInfoPagingSlicedResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfoPaging")
	TArray<FCodeInfo> data{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfoPaging")
	FAccelByteModelsPaging paging{};
};

USTRUCT(BlueprintType)
struct FRewardItemRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	int32 Quantity{};
};

USTRUCT(BlueprintType)
struct FRewardConditionRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString ConditionName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString Condition{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString EventName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	TArray<FRewardItemRequest> RewardItems{};
};

USTRUCT(BlueprintType)
struct FRewardCreateRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString RewardCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString EventTopic{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	TArray<FRewardConditionRequest> RewardConditions{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	int32 MaxAwarded{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	int32 MaxAwardedPerUser{};
};

USTRUCT(BlueprintType)
struct FRewardCreateInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString RewardId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString RewardCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString EventTopic{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	TArray<FRewardConditionRequest> RewardConditions{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	int32 MaxAwarded{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	int32 MaxAwardedPerUser{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FDateTime CreatedAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FDateTime UpdatedAt{};
};

USTRUCT(BlueprintType)
struct FQueryRewardDataInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString RewardId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString RewardCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString EventTopic{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	TArray<FRewardConditionRequest> RewardConditions{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	int32 MaxAwarded{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	int32 MaxAwardedPerUser{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FDateTime CreatedAt{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FDateTime UpdatedAt{};
};

USTRUCT(BlueprintType)
struct FQueryRewardPagingInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString Previous{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FString Next{};
};

USTRUCT(BlueprintType)
struct FQueryRewardInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	TArray<FQueryRewardDataInfo> Data{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Reward")
	FQueryRewardPagingInfo Paging{};
};

USTRUCT(BlueprintType)
struct FAppleIAPConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | IAP")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | IAP")
	FString BundleId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | IAP")
	FString Password{};
};

USTRUCT(BlueprintType)
struct FGoogleIAPConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | IAP")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | IAP")
	FString ApplicationName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | IAP")
	FString ServiceAccountId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | IAP")
	FString P12FileName{};
};

USTRUCT(BlueprintType)
struct FFulfillmentRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Fulfillment")
	FString ItemId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Fulfillment")
	int32 Quantity {0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Fulfillment")
	FString EndDate{}; // in ISO8601 format
};

struct EcommerceExpectedVariable
{
	FString ExpectedRootCategoryPath{};
	FString ExpectedChildCategoryPath{};
	FString ExpectedGrandChildCategoryPath{};
	FString ExpectedMediaCategoryPath{};
	FCurrencyCreateRequest ExpectedCurrency{};
	FString campaignCode{};
	FString campaignName{};
	FString expiredCampaignName{};
	FString notStartedCampaignName{};
	FString redeemableItemTitle{};
	FString mediaItemTitle{};
	FString ExpectedRootItemTitle{};
	FString ExpectedChildItemTitle{};
	FString ExpectedGrandChildItemTitle{};
	FString ExpectedPurchasingItemTitle{};
	FString ExpectedRootItemAppId{};
	FString ExpectedChildItemAppId{};
	FString ExpectedGrandChildItemAppId{};
	FString ExpectedRootItemSku{};
	FString ExpectedChildItemSku{};
	FString ExpectedGrandChildItemSku{};
	FStoreCreateRequest ExpectedStoreArchive{};
	FStoreCreateRequest ExpectedStoreTemporary{};
	int32 LootCoinQuantity{};
	FItemFullInfo redeemableItem{};
	FItemFullInfo mediaItem{};
	FItemFullInfo LootItem{};
	FItemFullInfo LootCoin{};
	FItemFullInfo LootResult{};
	FItemFullInfo BundleResult{};
	FCampaignInfo campaignResult{};
	FCampaignInfo expiredCampaignResult{};
	FCampaignInfo notStartedCampaignResult{};
	FCodeInfo codeInfo{};
	FCodeInfo expiredCodeInfo{};
	FCodeInfo notStartedCodeInfo{};
	FRewardCreateInfo RewardCreateInfo{};
};

#pragma endregion ECOMMERCE_MODEL

void SetupEcommerceCurrency(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void SetupEcommerceStore(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void SetupEcommerceCampaign(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void SetupEcommerceReward(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void TearDownEcommerce(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceCurrency(const FCurrencyCreateRequest& Currency, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceCurrency(const FString& CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteEcommerceCurrency(const FString& CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void AdminGetEcommercePublishedStore(const FString& Namespace, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommercePublishedStore(const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteEcommercePublishedStore(const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceStore(const FStoreCreateRequest& Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateEcommerceStore(const FString& Namespace, const FStoreCreateRequest& Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceStoreAll(const FString& Namespace, const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceStoreAll(const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteEcommerceStore(const FString& StoreId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminCloneEcommerceStore(const FString& Source, const FString& Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminCloneEcommerceStore(const FString& Namespace, const FString& Source, const FString& Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceCategory(const FString& StoreId, const FCategoryCreateRequest& Category, const THandler<FCategoryInfo>& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceItem(const FString& StoreId, const FItemCreateRequest& Item, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateEcommerceItem(const FString& Namespace, const FString& StoreId, const FItemCreateRequest& Item, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceItemBySKU(const FString& Namespace, const FString& StoreId, const FString& SKU, bool ActiveOnly, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGrantEcommerceUserEntitlements(const FString& Namespace, const FString& UserId, const TArray<FAccelByteModelsEntitlementGrant>& EntitlementGrant, const THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceCampaign(const FCampaignCreateModel& Body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminUpdateEcommerceCampaign(const FString& CampaignId, const FCampaignUpdateModel& Body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceCampaignByName(const FString& Name, const THandler<FCampaignPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateEcommerceCampaignCodes(const FString& CampaignId, const FCampaignCodeCreateModel& Body, const THandler<FCampaignCodeCreateResult>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceCampaignCodes(const FString& CampaignId, const THandler<FCodeInfoPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);
void AdminDisableEcommerceCampaignCode(const FString& CampaignCode, const THandler<FCodeInfo>& OnSuccess, const FErrorHandler& OnError);

void AdminCreateReward(const FRewardCreateRequest& Body, const THandler<FRewardCreateInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminQueryReward(const THandler<FQueryRewardInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteReward(const FString& RewardId, const THandler<FRewardCreateInfo>& OnSuccess, const FErrorHandler& OnError);

void AdminGetAppleIAPConfig(const FString& Namespace, const THandler<FAppleIAPConfig>& OnSuccess, const FErrorHandler& OnError);
void AdminGetAppleIAPConfig(const THandler<FAppleIAPConfig>& OnSuccess, const FErrorHandler& OnError);
void AdminGetGoogleIAPConfig(const FString& Namespace, const THandler<FGoogleIAPConfig>& OnSuccess, const FErrorHandler& OnError);
void AdminGetGoogleIAPConfig(const THandler<FGoogleIAPConfig>& OnSuccess, const FErrorHandler& OnError);

void AdminFulfillItem(const FString& Namespace, const FString& UserId, FFulfillmentRequest FulfillmentRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);