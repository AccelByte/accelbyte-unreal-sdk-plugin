// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteError.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Models/AccelByteStatisticModels.h"
#include "Models/AccelByteAchievementModels.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestUtilities.generated.h"

using AccelByte::FErrorHandler;
using AccelByte::THandler;

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsTest : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE(FSendNotificationSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Test ")
		static void SendNotification(FString Message, bool bAsync, const FSendNotificationSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	static void SendNotif(FString UserId, FString Message, bool bAsync, const AccelByte::FVoidHandler & OnSuccess, const FErrorHandler & OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Test ")
		static FString BytesToFString(TArray<uint8> Input);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Test ")
		static TArray<uint8> FStringToBytes(FString Input);
};

void DeleteUserById(
	const FString& UserId,
	const FSimpleDelegate& OnSuccess,
	const FErrorHandler& OnError);

void DeleteUserProfile(
	const FString& UserId,
	const FString& Namespace,
	const FSimpleDelegate& OnSuccess,
	const FErrorHandler& OnError);

void FlushHttpRequests();

void Waiting(bool& bcondition, FString Message);

void WaitUntil(TFunction<bool()> Condition, double TimeoutSeconds = 5.0, FString Message = "");

FString GetPublisherNamespace();

inline static bool LatenciesPredicate(const TPair<FString, float>& left, const TPair<FString, float>& right)
{
	return left.Value < right.Value;
}

#pragma region ECOMMERCE_MODEL
USTRUCT(BlueprintType)
struct FCurrencyCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
		FString currencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
		TMap<FString, FString> localizationDescriptions; // "Language" : "Description"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
		FString currencySymbol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
		EAccelByteItemCurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
		int decimals;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
		int maxAmountPerTransaction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
		int maxTransactionAmountPerDay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Currency")
		int maxBalanceAmount;
};

USTRUCT(BlueprintType)
struct FStoreCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		FString title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		TArray<FString> supportedLanguages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		TArray<FString> supportedRegions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		FString defaultLanguage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		FString defaultRegion;
};

USTRUCT(BlueprintType)
struct FStoreInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		FString storeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		FString title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		bool published;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		TArray<FString> supportedLanguages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		TArray<FString> supportedRegions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		FString defaultRegion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		FString defaultLanguage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		FString createdAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Store")
		FString updatedAt;
};

USTRUCT(BlueprintType)
struct FCategoryInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
		FString parentCategoryPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
		FString categoryPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
		FString createdAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
		FString updatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
		TMap<FString, FString> localizationDisplayNames; // "Language" : "Description"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
		bool root;
};

USTRUCT(BlueprintType)
struct FCategoryCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
		FString categoryPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Category")
		TMap<FString, FString> localizationDisplayNames; // "Language" : "Description"
};

USTRUCT(BlueprintType)
struct FLocalExt
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		TArray<FString> properties;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		TArray<FString> functions;
};

USTRUCT(BlueprintType)
struct FLocalization
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString longDescription;
};

USTRUCT(BlueprintType)
struct FCreateRegionDataItem
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
		EAccelByteItemCurrencyType CurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		FString CurrencyNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		FString PurchaseAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		FString ExpireAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		FString DiscountPurchaseAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | Item | Models | ItemInfo | RegionData")
		FString DiscountExpireAt;
};

USTRUCT(BlueprintType)
struct FRegionDataUS
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		TArray<FCreateRegionDataItem> US;
};

USTRUCT(BlueprintType)
struct FRecurring
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		EAccelByteSubscriptionCycle cycle = EAccelByteSubscriptionCycle::WEEKLY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		int fixedFreeDays = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		int fixedTrialCycles = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		int graceDays = 1; // min val = 1
};

USTRUCT(BlueprintType)
struct FItemCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		EAccelByteItemType itemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		EAccelByteEntitlementType entitlementType; // "Language" : "Description"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		int useCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		bool stackable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString appId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		EAccelByteAppType appType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString baseAppId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString targetCurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString targetNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString categoryPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		TMap<FString, FLocalization> localizations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		EAccelByteItemStatus status;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString sku;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		TArray<FAccelByteModelsItemImage> images;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString thumbnailUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FRegionDataUS regionData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		TArray<FString> itemIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		TArray<FString> tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		int maxCountPerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		int maxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString boothName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		int displayOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString clazz;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FRecurring recurring;
};

USTRUCT(BlueprintType)
struct FItemFullInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString appId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		EAccelByteAppType appType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString sku;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		EAccelByteEntitlementType entitlementType; // "Language" : "Description"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		int useCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString categoryPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		TMap<FString, FLocalization> localizations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		EAccelByteItemStatus status;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		EAccelByteItemType itemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString targetCurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString targetNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FAccelByteModelsItemRegionDataItem regionData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		TArray<FString> itemIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		TArray<FString> tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		int maxCountPerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		int maxCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString clazz;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString ext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		TArray<FString> codeFiles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString createdAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | Item")
		FString updatedAt;
};

USTRUCT(BlueprintType)
struct FRedeemableItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | RedeemableItem")
		FString itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | RedeemableItem")
		FString itemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | RedeemableItem")
		int32 quantity;
};

USTRUCT(BlueprintType)
struct FCampaignCreateModel
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
		TArray<FString> tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
		FString status; // ACTIVE/ INACTIVE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
		int32 maxRedeemCountPerCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
		int32 maxRedeemCountPerCodePerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
		int32 maxRedeemCountPerCampaignPerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
		int32 maxSaleCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
		FString redeemStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
		FString redeemEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
		FString redeemType; // ITEM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCreateModel")
		TArray<FRedeemableItem> items;
};

USTRUCT(BlueprintType)
struct FCampaignUpdateModel
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
		TArray<FString> tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
		FString status; // ACTIVE/ INACTIVE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
		int32 maxRedeemCountPerCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
		int32 maxRedeemCountPerCodePerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
		int32 maxRedeemCountPerCampaignPerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
		int32 maxSaleCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
		FString redeemStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
		FString redeemEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
		FString redeemType; // ITEM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignUpdateModel")
		TArray<FRedeemableItem> items;
};

USTRUCT(BlueprintType)
struct FCampaignInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		FString id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		FString type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		TArray<FString> tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		FString status; // ACTIVE/ INACTIVE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		FDateTime redeemStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		FDateTime redeemEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		int32 maxRedeemCountPerCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		int32 maxRedeemCountPerCodePerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		int32 maxRedeemCountPerCampaignPerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		int32 maxSaleCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		FString redeemType; // ITEM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		TArray<FRedeemableItem> items;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		FString boothName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		FDateTime createdAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignInfo")
		FDateTime updatedAt;
};

USTRUCT(BlueprintType)
struct FCampaignPagingSlicedResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignPaging")
		TArray<FCampaignInfo> data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignPaging")
		FAccelByteModelsPaging paging;
};

USTRUCT(BlueprintType)
struct FCampaignCodeCreateModel
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCodeCreateModel")
		int32 quantity;
};

USTRUCT(BlueprintType)
struct FCampaignCodeCreateResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CampaignCodeCreateResult")
		int32 numCreated;
};

USTRUCT(BlueprintType)
struct FCodeInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		FString id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		FString type; // REDEMPTION
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		FString campaignId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		FString value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		int32 maxRedeemCountPerCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		int32 maxRedeemCountPerCodePerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		int32 maxRedeemCountPerCampaignPerUser;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		int32 remainder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		int32 redeemedCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		FString status; // ACTIVE / INACTIVE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		FDateTime redeemStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		FDateTime redeemEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		FString redeemType; // ITEM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		TArray<FRedeemableItem> items;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		int32 batchNo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		FString acquireOrderNo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		FString acquireUserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		FDateTime createdAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfo")
		FDateTime updatedAt;
};

USTRUCT(BlueprintType)
struct FCodeInfoPagingSlicedResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfoPaging")
		TArray<FCodeInfo> data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ecommerce | CodeInfoPaging")
		FAccelByteModelsPaging paging;
};
#pragma endregion ECOMMERCE_MODEL

USTRUCT(BlueprintType)
struct FStatCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
		float defaultValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
		bool incrementOnly;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
		float maximum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
		float minimum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
		bool setAsGlobal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
		EAccelByteStatisticSetBy setBy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
		FString statCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
		TArray<FString> tags;
};

USTRUCT(BlueprintType)
struct FAllianceRule
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | AllianceRule")
	int max_number;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | AllianceRule")
	int min_number;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | AllianceRule")
	int player_max_number;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | AllianceRule")
	int player_min_number;
};

USTRUCT(BlueprintType)
struct FFlexingRule
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FFlexingRule")
	FString attribute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FFlexingRule")
	FString criteria;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FFlexingRule")
	int duration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FFlexingRule")
	int reference;
};

USTRUCT(BlueprintType)
struct FMatchingRule
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FMatchingRule")
	FString attribute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FMatchingRule")
	FString criteria;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | FMatchingRule")
	int reference;
};

USTRUCT(BlueprintType)
struct FMatchmakingRuleSet
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | RuleSet")
	FAllianceRule alliance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | RuleSet")
	TArray<FFlexingRule> flexing_rule;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | RuleSet")
	TArray<FMatchingRule> matching_rule;
};

USTRUCT(BlueprintType)
struct FMatchmakingCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
	FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
	int find_match_timeout_seconds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
	FString game_mode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
	FMatchmakingRuleSet rule_set;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
	bool joinable;
};

USTRUCT(BlueprintType)
struct FUserMapResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserMapResponse")
		FString userId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserMapResponse")
		FString Namespace;
};

USTRUCT(BlueprintType)
struct FVerificationCode
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | VerificationCode")
		FString accountRegistration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | VerificationCode")
		FString accountUpgrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | VerificationCode")
		FString passwordReset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | VerificationCode")
		FString updateEmail;
};

USTRUCT(BlueprintType)
struct FUserResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString Country;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString DateOfBirth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		bool DeletionStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString EmailAddress; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		bool EmailVerified;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		bool Enabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		TArray<FString> Roles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString UserId;
};

USTRUCT(BlueprintType)
struct FUserSearchResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserSearchResponse")
		TArray<FUserResponse> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserSearchResponse")
		FAccelByteModelsPaging Paging;
};

struct EcommerceExpectedVariable
{
	FString ExpectedRootCategoryPath;
	FString ExpectedChildCategoryPath;
	FString ExpectedGrandChildCategoryPath;
	FCurrencyCreateRequest ExpectedCurrency;
	FString campaignCode;
	FString campaignName;
	FString expiredCampaignName;
	FString notStartedCampaignName;
	FString redeemableItemTitle;
	FString ExpectedRootItemTitle;
	FString ExpectedChildItemTitle;
	FStoreCreateRequest ExpectedStoreArchive;
	FStoreCreateRequest ExpectedStoreTemporary;
	int32 LootCoinQuantity;
	FItemFullInfo redeemableItem;
	FItemFullInfo LootItem;
	FItemFullInfo LootCoin;
	FItemFullInfo LootResult;
	FItemFullInfo BundleResult;
	FCampaignInfo campaignResult;
	FCampaignInfo expiredCampaignResult;
	FCampaignInfo notStartedCampaignResult;
	FCodeInfo codeInfo;
	FCodeInfo expiredCodeInfo;
	FCodeInfo notStartedCodeInfo;
};

#pragma region LEADERBOARD_MODEL
USTRUCT(BlueprintType)
struct FLeaderboardDailyConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | DailyConfig")
		// FORMAT = hours:minutes EXAMPLE = ["01:30", "10:30", "15:30", "23:15" DEFAULT = "00:00"
		FString resetTime = "00:00";
};

USTRUCT(BlueprintType)
struct FLeaderboardMonthlyConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | MonthlyConfig")
		// RANGE = 1-31 DEFAULT = 1
		int resetDate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | MonthlyConfig")
		// FORMAT = hours:minutes EXAMPLE = ["01:30", "10:30", "15:30", "23:15" DEFAULT = "00:00
		FString resetTime = "00:00";
};

USTRUCT(BlueprintType)
struct FLeaderboardWeeklyConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | WeeklyConfig")
		// RANGE = 0-6 DEFAULT = 0 (sunday)
		int resetDay = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | WeeklyConfig")
		// FORMAT = hours:minutes EXAMPLE = ["01:30", "10:30", "15:30", "23:15" DEFAULT = "00:00
		FString resetTime = "00:00";
};

USTRUCT(BlueprintType)
struct FLeaderboardConfigRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FLeaderboardDailyConfig daily; // OPTIONAL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FLeaderboardMonthlyConfig monthly; // OPTIONAL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FLeaderboardWeeklyConfig weekly; // OPTIONAL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		bool descending = true; // OPTIONAL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FString iconURL = ""; // OPTIONAL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FString leaderboardCode; // lowercase and maximum length is 32 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FString name; // Max 128 characters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		int seasonPeriod = 32; // OPTIONAL. Must be greater than 31 days. Will trigger seasonal leaderboard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FString startTime; // RFC3339 e.g. "2020-10-02T15:00:00.05Z"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
	FString statCode; // Statistic's service
};
using FLeaderboardConfigResponse = FLeaderboardConfigRequest;
#pragma endregion LEADERBOARD_MODEL

#pragma region AGREEMENT_MODEL
USTRUCT(BlueprintType)
struct FAgreementBasePolicyCreate
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
		FString TypeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
		FString BasePolicyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
		TArray<FString> AffectedCountries;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
		TArray<FString> AffectedClientIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
		TArray<FString> Tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicyCreate")
		bool IsMandatory;
};
USTRUCT(BlueprintType)
struct FAgreementPolicyTypeObject
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyTypeObject")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyTypeObject")
		FString PolicyTypeName;
	// contains more fields but unused for test
};
USTRUCT(BlueprintType)
struct FAgreementPolicyObject
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyObject")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyObject")
		FString CountryCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyObject")
		FString PolicyName;
	// contains more fields but unused for test
};
USTRUCT(BlueprintType)
struct FAgreementBasePolicy
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicy")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicy")
		FString BasePolicyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicy")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | BasePolicy")
		TArray<FAgreementPolicyObject> Policies;
	// contains more fields but unused for test
};
USTRUCT(BlueprintType)
struct FAgreementPolicyVersionCreate
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
		FString DisplayVersion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
		bool IsCrucial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
		bool IsCommitted;
};
USTRUCT(BlueprintType)
struct FAgreementPolicyVersion
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
		FString DisplayVersion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
		FString BasePolicyId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
		bool IsCrucial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | PolicyVersion")
		bool IsInEffect;
	// contains more fields but unused for test
};
USTRUCT(BlueprintType)
struct FAgreementCountryPolicy
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | CountryPolicy")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | CountryPolicy")
		FString CountryCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | CountryPolicy")
		FString PolicyName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | CountryPolicy")
		bool IsCrucial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | CountryPolicy")
		TArray<FAgreementPolicyVersion> PolicyVersions;
	// contains more fields but unused for test
};
USTRUCT(BlueprintType)
struct FAgreementLocalizedPolicyCreate
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicyCreate")
		FString LocaleCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicyCreate")
		FString ContentType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicyCreate")
		FString Description;
};
USTRUCT(BlueprintType)
struct FAgreementLocalizedPolicy
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicy")
		FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicy")
		FString LocaleCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicy")
		FString ContentType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicy")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Agreement | LocalizedPolicy")
		FString AttachmentLocation;
};

#pragma endregion AGREEMENT_MODEL

USTRUCT(BlueprintType)
struct FAchievementRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		FString AchievementCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		FString DefaultLanguage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		TMap<FString, FString> Name; // "Language" : "Name"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		TMap<FString, FString> Description; // "Language" : "Description"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		TArray<FAccelByteModelsAchievementIcon> LockedIcons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		TArray<FAccelByteModelsAchievementIcon> UnlockedIcons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		bool Hidden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		TArray<FString> Tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		bool Incremental;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		int32 GoalValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		FString StatCode;
};

USTRUCT(BlueprintType)
struct FAchievementResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		FString AchievementCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		FString DefaultLanguage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		TMap<FString, FString> Name; // "Language" : "Name"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		TMap<FString, FString> Description; // "Language" : "Description"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		TArray<FAccelByteModelsAchievementIcon> LockedIcons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		TArray<FAccelByteModelsAchievementIcon> UnlockedIcons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		bool Hidden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		int32 ListOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		TArray<FString> Tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		bool Incremental;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		int32 GoalValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		FString StatCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		FString UpdatedAt;
};

USTRUCT(BlueprintType)
struct FFreeSubscriptionRequest 
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Subscriptions | SubscriptionRequest")
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Subscriptions | SubscriptionRequest")
		int32 GrantDays;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Subscriptions | SubscriptionRequest")
		FString Source;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Subscriptions | SubscriptionRequest")
		FString Reason;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Subscriptions | SubscriptionRequest")
		FString Region;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Subscriptions | SubscriptionRequest")
		FString Language;
};

USTRUCT(BlueprintType)
struct FPodConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Cpu_limit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Mem_limit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Params;
};

USTRUCT(BlueprintType)
struct FDeploymentConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Buffer_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Configuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Game_version;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Max_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Min_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TArray<FString> Regions;
};

USTRUCT(BlueprintType)
struct FDeploymentWithOverride
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	bool Allow_version_override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Buffer_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Configuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Game_version;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Max_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Min_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, FDeploymentConfig> Overrides;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TArray<FString> Regions;
};

USTRUCT(BlueprintType)
struct FDsmConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	bool Allow_version_override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Buffer_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Claim_timeout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, FPodConfig> Configurations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Cpu_limit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Mem_limit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Params;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Creation_timeout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Default_version;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, FDeploymentWithOverride> Deployments;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Heartbeat_timeout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, FString> Image_version_mapping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Max_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Min_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, FDeploymentConfig> Overrides;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Port;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, int32> Ports;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Protocol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TArray<FString> Providers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Session_timeout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Unreachable_timeout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, int32> Version_image_size_mapping;
};

TArray<FString> GetDisabledTestList();
bool IsAccelByteTestEnabled(const FString& TestName);
bool AccelByteSkipTest(const FString& TestName);

#define AB_TEST_SKIP_WHEN_DISABLED() if (AccelByteSkipTest(GetBeautifiedTestName())) return true

void SetupEcommerce(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void TearDownEcommerce(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Currency_Create(FCurrencyCreateRequest Currency, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Currency_Get(FString CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Currency_Delete(FString CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_PublishedStore_Get(const FString& Namespace, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_PublishedStore_Get(const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_PublishedStore_Delete(const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Store_Create(FStoreCreateRequest Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Create(const FString& Namespace, FStoreCreateRequest Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Get_All(const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Delete(FString StoreId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Clone(FString Source, FString Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Clone(const FString& Namespace, FString Source, FString Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Category_Create(FCategoryCreateRequest Category, FString StoreId, const THandler<FCategoryInfo>& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Item_Create(FItemCreateRequest Item, FString StoreId, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Item_Create(const FString& Namespace, FItemCreateRequest Item, FString StoreId, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_GetItem_BySKU(const FString& Namespace, const FString& StoreId, const FString& SKU, const bool& ActiveOnly, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_GrantUserEntitlements(const FString& Namespace, const FString& UserId, const TArray<FAccelByteModelsEntitlementGrant>& EntitlementGrant, const THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Campaign_Create(FCampaignCreateModel body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Campaign_Update(FString campaignId, FCampaignUpdateModel body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_CampaignCodes_Create(FString campaignId, FCampaignCodeCreateModel body, const THandler<FCampaignCodeCreateResult>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Campaign_Get_ByName(FString name, const THandler<FCampaignPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_CampaignCodes_Get(FString campaignId, const THandler<FCodeInfoPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_CampaignCode_Disable(FString code, const THandler<FCodeInfo>& OnSuccess, const FErrorHandler& OnError);

void Matchmaking_Create_Matchmaking_Channel(const FString& channel, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable = false);
void Matchmaking_Create_Matchmaking_Channel(const FString& channel, FAllianceRule AllianceRule, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable = false);
void Matchmaking_Create_Matchmaking_Channel(const FString& channel, FAllianceRule AllianceRule, TArray<FMatchingRule> MatchingRules, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable = false);
void Matchmaking_Delete_Matchmaking_Channel(const FString& channel, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Statistic_Get_Stat_By_StatCode(FString statCode, const THandler<FAccelByteModelsStatInfo>& OnSuccess, const FErrorHandler& OnError);
void Statistic_Create_Stat(FStatCreateRequest body, const THandler<FAccelByteModelsStatInfo>& OnSuccess, const FErrorHandler& OnError);
void Statistic_Delete_Stat(const FString& statCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Statistic_Delete_StatItem(const FString& userId, const FString& statCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Leaderboard_Create_Leaderboard(const FLeaderboardConfigRequest& request, const THandler<FLeaderboardConfigRequest>& OnSuccess, const FErrorHandler& OnError);
void Leaderboard_GetAll_Leaderboard(const FString& leaderboardCode, const THandler<FLeaderboardConfigResponse>& OnSuccess, const FErrorHandler& OnError);
void Leaderboard_Delete_Leaderboard(const FString& leaderboardCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void User_Get_User_Mapping(const FString& userId, const THandler<FUserMapResponse>& OnSuccess, const FErrorHandler& OnError);
void User_Get_Verification_Code(const FString& userId, const THandler<FVerificationCode>& OnSuccess, const FErrorHandler& OnError);
void User_Get_By_Email_Address(const FString& EmailAddress, const THandler<FUserResponse>& OnSuccess, const FErrorHandler& OnError);
void User_Delete_By_Email_Address(const FString& EmailAddress, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void User_Get_MyData_Direct(const FString& JsonWebToken, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);

void DSM_Delete_Server(const FString& podName, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
void DSM_Get_Config(const THandler<FDsmConfig> & OnSuccess, const FErrorHandler & OnError);
void DSM_Set_Config(const FDsmConfig& Body, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

void Agreement_Create_Base_Policy(const FAgreementBasePolicyCreate& CreateRequest, const THandler<FAgreementBasePolicy>& OnSuccess, const FErrorHandler& OnError);
void Agreement_Create_Policy_Version(const FString& PolicyId, const FAgreementPolicyVersionCreate& CreateRequest, const THandler<FAgreementPolicyVersion>& OnSuccess, const FErrorHandler& OnError);
void Agreement_Create_Localized_Policy(const FString& PolicyVersionId, const FAgreementLocalizedPolicyCreate& CreateRequest, const THandler<FAgreementLocalizedPolicy>& OnSuccess, const FErrorHandler& OnError);
void Agreement_Publish_Policy_Version(const FString& PolicyVersionId, bool ShouldNotify, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Agreement_Get_Base_Policies(const THandler<TArray<FAgreementBasePolicy>>& OnSuccess, const FErrorHandler& OnError);
void Agreement_Get_Country_Base_Policy(const FString& PolicyId, const FString& CountryCode, const THandler<FAgreementCountryPolicy>& OnSuccess, const FErrorHandler& OnError);
void Agreement_Get_Policy_Types(const THandler<TArray<FAgreementPolicyTypeObject>>& OnSuccess, const FErrorHandler& OnError);
void Agreement_Get_Localized_Policies(const FString& PolicyVersionId, const THandler<TArray<FAgreementLocalizedPolicy>>& OnSuccess, const FErrorHandler& OnError);

void Achievement_Create(const FAchievementRequest& AchievementRequest, const THandler<FAchievementResponse>& OnSuccess, const FErrorHandler& OnError);
void Achievement_Delete(const FString& AchievementCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Subscription_GrantFreeSubs(const FString& UserId, const FFreeSubscriptionRequest& BodyRequest, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
