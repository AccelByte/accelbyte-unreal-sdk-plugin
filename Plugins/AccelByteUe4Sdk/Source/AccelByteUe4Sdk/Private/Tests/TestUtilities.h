// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
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

void Waiting(bool& condition, FString Message);

void WaitUntil(TFunction<bool()> Condition, double TimeoutSeconds = 5.0, FString Message = "");

inline static bool LatenciesPredicate(const TPair<FString, float>& left, const TPair<FString, float>& right)
{
	return left.Value < right.Value;
}

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
	FString ExpectedRootItemTitle;
	FString ExpectedChildItemTitle;
	FStoreCreateRequest ExpectedStoreArchive;
	FStoreCreateRequest ExpectedStoreTemporary;
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
void Matchmaking_Create_Matchmaking_Channel(const FString& channel, FAllianceRule AllianceRule, const FSimpleDelegate & OnSuccess, const FErrorHandler& OnError);
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

void DSM_Delete_Server(const FString& podName, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

void Agreement_Create_Base_Policy(const FAgreementBasePolicyCreate& CreateRequest, const THandler<FAgreementBasePolicy>& OnSuccess, const FErrorHandler& OnError);
void Agreement_Create_Policy_Version(const FString& PolicyId, const FAgreementPolicyVersionCreate& CreateRequest, const THandler<FAgreementPolicyVersion>& OnSuccess, const FErrorHandler& OnError);
void Agreement_Create_Localized_Policy(const FString& PolicyVersionId, const FAgreementLocalizedPolicyCreate& CreateRequest, const THandler<FAgreementLocalizedPolicy>& OnSuccess, const FErrorHandler& OnError);
void Agreement_Publish_Policy_Version(const FString& PolicyVersionId, bool ShouldNotify, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void Agreement_Get_Base_Policies(const THandler<TArray<FAgreementBasePolicy>>& OnSuccess, const FErrorHandler& OnError);
void Agreement_Get_Country_Base_Policy(const FString& PolicyId, const FString& CountryCode, const THandler<FAgreementCountryPolicy>& OnSuccess, const FErrorHandler& OnError);
void Agreement_Get_Policy_Types(const THandler<TArray<FAgreementPolicyTypeObject>>& OnSuccess, const FErrorHandler& OnError);
void Agreement_Get_Localized_Policies(const FString& PolicyVersionId, const THandler<TArray<FAgreementLocalizedPolicy>>& OnSuccess, const FErrorHandler& OnError);