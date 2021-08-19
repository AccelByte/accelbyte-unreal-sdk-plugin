// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Models/AccelByteStatisticModels.h"
#include "Models/AccelByteAchievementModels.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestUtilities.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteTest, Log, All);

#define AB_TEST_TRUE(Expr) UTEST_TRUE(TEXT(#Expr), Expr)
#define AB_TEST_FALSE(Expr) UTEST_FALSE(TEXT(#Expr), Expr)
#define AB_TEST_VALID(Ptr) UTEST_VALID(TEXT(#Ptr), Ptr)
#define AB_TEST_INVALID(Ptr) UTEST_INVALID(TEXT(#Ptr), Ptr)
#define AB_TEST_NOT_NULL(Ptr) UTEST_NOT_NULL(TEXT(#Ptr), Ptr)
#define AB_TEST_NULL(Ptr) UTEST_NULL(TEXT(#Ptr), Ptr)
#define AB_TEST_EQUAL(Actual, Expected) UTEST_EQUAL(TEXT(#Actual), Actual, Expected)
#define AB_TEST_NOT_EQUAL(Actual, Expected) UTEST_NOT_EQUAL(TEXT(#Actual " and " #Expected), Actual, Expected)
#define AB_TEST_SAME(Actual, Expected) UTEST_SAME(TEXT(#Actual " and " #Expected)), Actual, Expected)
#define AB_TEST_NOT_SAME(Actual, Expected) UTEST_NOT_SAME(TEXT(#Actual " and " #Expected)), Actual, Expected)

#define AB_HTTP_REQUEST(Name, URL, Verb, Authorization, ContentType, Content) \
	FHttpRequestPtr Name = FHttpModule::Get().CreateRequest(); \
	Name->SetURL(URL); \
	Name->SetHeader(TEXT("Authorization"), Authorization); \
	Name->SetVerb(Verb); \
	Name->SetHeader(TEXT("Content-Type"), ContentType); \
	Name->SetHeader(TEXT("Accept"), TEXT("application/json")); \
	Name->SetContentAsString(Content);
#define AB_HTTP_GET(Name, URL, Authorization) \
	AB_HTTP_REQUEST(Name, URL, TEXT("GET"), Authorization,  TEXT("application/json"), TEXT(""))
#define AB_HTTP_DELETE(Name, URL, Authorization) \
	AB_HTTP_REQUEST(Name, URL, TEXT("DELETE"), Authorization, TEXT("application/json"), TEXT(""))
#define AB_HTTP_POST(Name, URL, Authorization, Content) \
	AB_HTTP_REQUEST(Name, URL, TEXT("POST"), Authorization, TEXT("application/json"), Content)
#define AB_HTTP_POST_FORM(Name, URL, Authorization, Content) \
	AB_HTTP_REQUEST(Name, URL, TEXT("POST"), Authorization, TEXT("application/x-www-form-urlencoded"), Content)
#define AB_HTTP_PUT(Name, URL, Authorization, Content) \
	AB_HTTP_REQUEST(Name, URL, TEXT("PUT"), Authorization, TEXT("application/json"), Content)
#define AB_HTTP_PATCH(Name, URL, Authorization, Content) \
	AB_HTTP_REQUEST(Name, URL, TEXT("PATCH"), Authorization, TEXT("application/json"), Content)


using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::FVoidHandler;

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

void WaitUntil(const bool& bCondition, const FString Message = "", const double TimeoutSeconds = 60.0);

void WaitUntil(const TFunction<bool()> Condition, const FString Message = "", const double TimeoutSeconds = 60.0);

void DelaySeconds(double Seconds, FString Message = "");

FString GetPublisherNamespace();

FString GetSteamUserId();

FString GetSteamAppId();

FString GetSteamTicket();

bool CheckSteamTicket();

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
		EAccelByteSeasonType SeasonType;
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
		float GoalValue;
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
		float GoalValue;
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

struct FTestUser
{
	FString FirstName = TEXT("John");
	FString LastName = TEXT("Doe");
	FString DateOfBirth = TEXT("2000-01-01");
	FString Country = TEXT("US");
	FString Language = TEXT("en");
	FString Timezone = TEXT("Etc/UTC");
	FString DisplayName;
	FString Email;
	FString Password = TEXT("Password123!");
	FString AvatarSmallUrl = TEXT("http://example.com/avatar/small.jpg");
	FString AvatarUrl = TEXT("http://example.com/avatar/normal.jpg");
	FString AvatarLargeUrl = TEXT("http://example.com/avatar/large.jpg");

	FTestUser(const FString& TestUID, const int32 UserIndex = 0) :
		DisplayName(FString::Printf(TEXT("%s%s-%s-%02d"), *FirstName, *LastName, *TestUID, UserIndex).ToLower()),
		Email(FString::Printf(TEXT("%s_%s_%s_%02d@example.com"), TEXT("justice-ue4-sdk"), TEXT("test"), *TestUID, UserIndex).ToLower())
	{}
};

FString GetAdminBaseUrl();
FString GetAdminUserAccessToken();

bool SetupTestUsers(const FString& InTestUID, const int32 InNumOfUsers, TArray<TSharedPtr<FTestUser>>& OutUsers);
bool SetupTestUsers(const FString& InTestUID, const int32 InNumOfUsers, TArray<TSharedPtr<FTestUser>>& OutUsers, TArray<TSharedPtr<Credentials>>& OutCredentials);
bool CheckTestUsers(const TArray<TSharedPtr<FTestUser>>& InUsers, const TArray<TSharedPtr<Credentials>>& InCredentials);
bool TearDownTestUsers(TArray<TSharedPtr<Credentials>>& InCredentials);

void User_Get_User_Mapping(const FString& userId, const THandler<FUserMapResponse>& OnSuccess, const FErrorHandler& OnError);
void User_Get_Verification_Code(const FString& userId, const THandler<FVerificationCode>& OnSuccess, const FErrorHandler& OnError);
void User_Get_By_Email_Address(const FString& EmailAddress, const THandler<FUserResponse>& OnSuccess, const FErrorHandler& OnError);
void User_Delete_By_Email_Address(const FString& EmailAddress, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void User_Get_MyData_Direct(const FString& JsonWebToken, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);
