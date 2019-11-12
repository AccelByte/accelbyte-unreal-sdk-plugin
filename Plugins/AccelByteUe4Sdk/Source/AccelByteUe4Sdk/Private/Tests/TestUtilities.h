// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteError.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Models/AccelByteStatisticModels.h"
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

void Waiting(bool& condition, FString text);

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
struct FMatchmakingRuleSet
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | RuleSet")
		int alliance_number;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | RuleSet")
		bool symmetric_match;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | RuleSet")
		int symmetric_party_number;
};

USTRUCT(BlueprintType)
struct FMatchmakingCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
		FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
		FString game_mode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Matchmaking | ChannelCreate")
		FMatchmakingRuleSet rule_set;
};

USTRUCT(BlueprintType)
struct FUserMapResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserMapResponse")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserMapResponse")
		FString userId;
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
};

struct EcommerceExpectedVariable
{
	FString ExpectedRootCategoryPath;
	FString ExpectedChildCategoryPath;
	FString ExpectedGrandChildCategoryPath;
	FCurrencyCreateRequest ExpectedCurrency;
	FString ExpectedRootItemTitle;
	FString ExpectedChildItemTitle;
	FStoreCreateRequest ExpectedStoreArchive;
	FStoreCreateRequest ExpectedStoreTemporary;
};

void SetupEcommerce(EcommerceExpectedVariable Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void TearDownEcommerce(EcommerceExpectedVariable Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Currency_Create(FCurrencyCreateRequest Currency, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Currency_Get(FString CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Currency_Delete(FString CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_PublishedStore_Get(const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_PublishedStore_Delete(const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Store_Create(FStoreCreateRequest Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Get_All(const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Delete(FString StoreId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Ecommerce_Store_Clone(FString Source, FString Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Category_Create(FCategoryCreateRequest Category, FString StoreId, const THandler<FCategoryInfo>& OnSuccess, const FErrorHandler& OnError);

void Ecommerce_Item_Create(FItemCreateRequest Item, FString StoreId, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);

void Matchmaking_Create_Matchmaking_Channel(const FString& channel, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Matchmaking_Delete_Matchmaking_Channel(const FString& channel, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void Statistic_Get_Stat_By_StatCode(FString statCode, const THandler<FAccelByteModelsStatInfo>& OnSuccess, const FErrorHandler& OnError);
void Statistic_Create_Stat(FStatCreateRequest body, const THandler<FAccelByteModelsStatInfo>& OnSuccess, const FErrorHandler& OnError);
void Statistic_Bulk_Create_User_StatItem(FString userId, TArray<FString> statCode, const THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>& OnSuccess, const FErrorHandler& OnError);

void User_Get_User_Mapping(const FString& userId, const THandler<FUserMapResponse>& OnSuccess, const FErrorHandler& OnError);
void User_Get_Verification_Code(const FString& userId, const THandler<FVerificationCode>& OnSuccess, const FErrorHandler& OnError);
