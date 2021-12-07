// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Api/AccelByteItemApi.h"
#include "Api/AccelByteOrderApi.h"
#include "Api/AccelByteSeasonPassApi.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteWalletApi.h"
#include "GameServerApi/AccelByteServerEcommerceApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerSeasonPassApi.h"
#include "TestUtilities.h"
#include "EcommerceTestAdmin.h"
#include "SeasonPassTestAdmin.h"
#include "UserTestAdmin.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteSeasonPassTest, Log, All);

DEFINE_LOG_CATEGORY(LogAccelByteSeasonPassTest);

FString SeasonItemCategoryPath = TEXT("/Season");
FStoreInfo SeasonStoreInfo;
FItemFullInfo FreeSeasonPassItem, PremiumSeasonPassItem, TierSeasonPassItem, SeasonCurrencyItemInfo, SeasonRewardItemInfo;
FString CurrencyCode = TEXT("UEC");
const int32 CurrencyRewardQuantity = 5;
FSeasonPassCreateSeasonResponse SeasonResponse;
FSeasonPassCreatePassResponse FreePassResponse, PremiumPassResponse;
FString CurrencyRewardCode = "coins-reward", ItemRewardCode = "premium-reward";
FSeasonPassRewardResponse CurrencyRewardResponse, ItemRewardResponse;
TArray<FAccelByteModelsSeasonPassTierJsonObject> SeasonTiersResponse;
int32 SeasonTierCount = 6, SeasonTierRequiredExp = 100;
TArray<TSharedPtr<FJsonObject>> SeasonTierRewards;
const int32 PremiumPassPrice = 100, SeasonTierPrice = 10;

const int32 AutomationFlagMaskSeasonPass = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter |
	EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const FErrorHandler SeasonPassOnError = FErrorHandler::CreateLambda([](int32 Code, const FString& Message)
{
	UE_LOG(LogAccelByteSeasonPassTest, Error, TEXT("Code : %d, Message : %s"), Code, *Message);
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SeasonPassSetup, "AccelByte.Tests.SeasonPass.A.Setup", AutomationFlagMaskSeasonPass)
bool SeasonPassSetup::RunTest(const FString& Parameters)
{
	FRegistry::Credentials.ForgetAll();
	FRegistry::ServerCredentials.ForgetAll();

	// Check if there any season, delete if any
	bool bQuerySeasonSuccess = false;
	FSeasonPassCreateGetSeasonsPagingResponse SeasonsResponse;
	AdminQuerySeason({EAccelByteSeasonPassStatus::DRAFT, EAccelByteSeasonPassStatus::PUBLISHED},
					THandler<FSeasonPassCreateGetSeasonsPagingResponse>::CreateLambda([&bQuerySeasonSuccess, &SeasonsResponse](const FSeasonPassCreateGetSeasonsPagingResponse& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("query existing season succeed"));
		bQuerySeasonSuccess = true;
		SeasonsResponse = Result;
	}), SeasonPassOnError);
	WaitUntil(bQuerySeasonSuccess, "Waiting to query existing season...");
	AB_TEST_TRUE(bQuerySeasonSuccess);

	for (const FSeasonPassCreateGetSeasonResponse& Season : SeasonsResponse.Data)
	{
		if (Season.Status == EAccelByteSeasonPassStatus::PUBLISHED)
		{
			// Unpublish before delete
			bool bUnpublishSuccess = false;
			AdminForceUnpublishSeason(Season.Id, THandler<FSeasonPassCreateSeasonResponse>::CreateLambda([&bUnpublishSuccess](const FSeasonPassCreateSeasonResponse& Result)
			{
				UE_LOG(LogAccelByteSeasonPassTest, Log,	TEXT("unpublish season succeed"));
				bUnpublishSuccess = true;
			}), SeasonPassOnError);
			WaitUntil(bUnpublishSuccess, "Waiting to unpublish season...");
			AB_TEST_TRUE(bUnpublishSuccess);
		}

		bool bDeleteSeasonSuccess = false;
		AdminDeleteSeason(Season.Id, FSimpleDelegate::CreateLambda([&bDeleteSeasonSuccess]()
		{
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Delete season succeed"));
			bDeleteSeasonSuccess = true;
		}), SeasonPassOnError);
		WaitUntil(bDeleteSeasonSuccess, "Waiting delete season...");
		AB_TEST_TRUE(bDeleteSeasonSuccess);
	}

	/*
	Check the currency that expected for integration test. If it's already created, it doesn't need to be created again.
	If it doesn't exist, then it will be created.
	*/
	bool bCurrencyAlreadyExist = false;
	bool bCurrencyCreated = false;
	bool bCurrencyCheckDone = false;
	AdminGetEcommerceCurrency(CurrencyCode, FSimpleDelegate::CreateLambda([&bCurrencyAlreadyExist, &bCurrencyCreated, &bCurrencyCheckDone]()
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("CURRENCY is created already."));
		bCurrencyAlreadyExist = true;
		bCurrencyCreated = true;
		bCurrencyCheckDone = true;
	}), FErrorHandler::CreateLambda(
	[&bCurrencyAlreadyExist, &bCurrencyCheckDone](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log,TEXT("CURRENCY does not exist. Creating..."));
		bCurrencyAlreadyExist = false;
		bCurrencyCheckDone = true;
	}));
	WaitUntil(bCurrencyCheckDone, "Waiting for currency check...");
	AB_TEST_TRUE(bCurrencyCheckDone);

	if (!bCurrencyAlreadyExist)
	{
		FCurrencyCreateRequest CurrencyRequest{
			CurrencyCode,
			{{"en", CurrencyCode}},
			CurrencyCode,
			EAccelByteItemCurrencyType::VIRTUAL,
			0,
			-1,
			-1,
			-1
		};

		AdminCreateEcommerceCurrency(CurrencyRequest, FSimpleDelegate::CreateLambda([&bCurrencyCreated]()
		{
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("SetupEcommerce: CURRENCY is created"));
			bCurrencyCreated = true;
		}), FErrorHandler::CreateLambda([this, &bCurrencyCreated](int32 Code, FString Message)
		{
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("SetupEcommerce: CURRENCY can not be created."));
			bCurrencyCreated = false;
			SeasonPassOnError.ExecuteIfBound(Code, Message);
		}));
		WaitUntil(bCurrencyCreated, "Waiting for currency created...");
		AB_TEST_TRUE(bCurrencyCreated);
	}

	// delete if any, only 1 allowed
	bool bDeletePublishedStore = false;
	AdminDeleteEcommercePublishedStore(FSimpleDelegate::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Delete published store succeed"));
		bDeletePublishedStore = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("No published store found. Ignoring"));
		bDeletePublishedStore = true; // just ignore
	}));
	WaitUntil(bDeletePublishedStore, "Waiting to delete published store ...");
	AB_TEST_TRUE(bDeletePublishedStore);

	bool bGetAllStoreSuccess = false;
	TArray<FStoreInfo> GetAllResult;
	AdminGetEcommerceStoreAll(
		THandler<TArray<FStoreInfo>>::CreateLambda([&](const TArray<FStoreInfo>& Result)
		{
			GetAllResult = Result;
			bGetAllStoreSuccess = true;
		}), SeasonPassOnError);
	WaitUntil(bGetAllStoreSuccess, "Waiting for get all store...");
	for (int i = 0; i < GetAllResult.Num(); i++)
	{
		bool bDeleteTestingStoreSuccess = false;
		AdminDeleteEcommerceStore(GetAllResult[i].storeId,FSimpleDelegate::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Delete store succeed"));
			bDeleteTestingStoreSuccess = true;
		}), SeasonPassOnError);
		WaitUntil(bDeleteTestingStoreSuccess, "Waiting for testing store deletion...");
		AB_TEST_TRUE(bDeleteTestingStoreSuccess);
	}

#pragma region SETUP_DRAFT_STORE
	/*
	Create a new store, for testing purpose, temporary store.
	*/
	FStoreCreateRequest StoreRequest{
		TEXT("Integration Test UE4 Season Pass"),
		TEXT("for SDK testing purpose"),
		{"en"},
		{"US"},
		TEXT("en"),
		TEXT("US")
	};
	bool bTemporaryStoreCreated = false;
	AdminCreateEcommerceStore(StoreRequest, THandler<FStoreInfo>::CreateLambda([this, &bTemporaryStoreCreated](const FStoreInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("season pass store is created"));
		SeasonStoreInfo = Result;
		bTemporaryStoreCreated = true;
	}), SeasonPassOnError);
	WaitUntil(bTemporaryStoreCreated, "Waiting for store created...");
	AB_TEST_TRUE(bTemporaryStoreCreated);

	FCategoryCreateRequest CategoryRequest;
	CategoryRequest.categoryPath = SeasonItemCategoryPath;
	CategoryRequest.localizationDisplayNames = {{"en", SeasonItemCategoryPath}};
	bool bCreateCategorySuccess = false;
	AdminCreateEcommerceCategory(SeasonStoreInfo.storeId, CategoryRequest,
		THandler<FCategoryInfo>::CreateLambda([&bCreateCategorySuccess](const FCategoryInfo& Result) {
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Create category succeed"));
			bCreateCategorySuccess = true;
		}), SeasonPassOnError);
	WaitUntil(bCreateCategorySuccess, "Waiting to create category...");
	AB_TEST_TRUE(bCreateCategorySuccess);

	FLocalization itemLocalization
	{
		"season item",
		"season item",
		"season item",
	};
	TMap<FString, FLocalization> itemLocalizationMap;
	itemLocalizationMap.Add("en", itemLocalization);
	FCreateRegionDataItem FreeRegionDataItem{
		0,
		0,
		0,
		0,
		CurrencyCode,
		EAccelByteItemCurrencyType::VIRTUAL,
		FRegistry::Settings.Namespace,
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601(),
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601()
	};
	FItemCreateRequest FreeSeasonPassItemRequest{
		EAccelByteItemType::SEASON,
		EAccelByteSeasonType::PASS,
		TEXT("Free Season Pass UE Integration"),
		EAccelByteEntitlementType::CONSUMABLE,
		1,
		false,
		"",
		EAccelByteAppType::GAME,
		"",
		CurrencyCode,
		FRegistry::Settings.Namespace,
		SeasonItemCategoryPath,
		itemLocalizationMap,
		EAccelByteItemStatus::ACTIVE,
		"FreeSeasonPassSku",
		{FAccelByteModelsItemImage{"image", "itemImage", 32, 32, "http://example.com", "http://example.com"}},
		"",
		FRegionDataUS{{FreeRegionDataItem}},
		{},
		{},
		-1,
		1,
		"",
		0,
		"classless",
		FRecurring{}
	};
	bool bCreateFreeSeasonSuccess = false;
	AdminCreateEcommerceItem(SeasonStoreInfo.storeId, FreeSeasonPassItemRequest,
		THandler<FItemFullInfo>::CreateLambda([this, &bCreateFreeSeasonSuccess](const FItemFullInfo& Result) {
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Create Free season pass item succeed"));
			bCreateFreeSeasonSuccess = true;
			FreeSeasonPassItem = Result;
		}), SeasonPassOnError);
	WaitUntil(bCreateFreeSeasonSuccess, "Waiting to create free season pass item...");
	AB_TEST_TRUE(bCreateFreeSeasonSuccess);

	FCreateRegionDataItem PremiumRegionDataItem{
		PremiumPassPrice, 0, 0, 0, CurrencyCode, EAccelByteItemCurrencyType::VIRTUAL, FRegistry::Settings.Namespace,
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601(),
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601()
	};
	FItemCreateRequest PremiumSeasonPassItemRequest{
		EAccelByteItemType::SEASON,
		EAccelByteSeasonType::PASS,
		TEXT("Premium Season Pass UE Integration"),
		EAccelByteEntitlementType::CONSUMABLE,
		1,
		false,
		"",
		EAccelByteAppType::GAME,
		"",
		CurrencyCode,
		FRegistry::Settings.Namespace,
		SeasonItemCategoryPath,
		itemLocalizationMap,
		EAccelByteItemStatus::ACTIVE,
		"PremiumSeasonPassSku",
		{FAccelByteModelsItemImage{"image", "itemImage", 32, 32, "http://example.com", "http://example.com"}},
		"",
		FRegionDataUS{{PremiumRegionDataItem}},
		{},
		{},
		-1,
		1,
		"",
		0,
		"classless",
		FRecurring{}
	};
	bool bCreatePremiumSeasonSuccess = false;
	AdminCreateEcommerceItem(SeasonStoreInfo.storeId, PremiumSeasonPassItemRequest,
		THandler<FItemFullInfo>::CreateLambda([this, &bCreatePremiumSeasonSuccess](const FItemFullInfo& Result) {
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Create Premium Season Pass item succeed"));
			bCreatePremiumSeasonSuccess = true;
			PremiumSeasonPassItem = Result;
		}), SeasonPassOnError);
	WaitUntil(bCreatePremiumSeasonSuccess, "Waiting to create premium season pass item...");
	AB_TEST_TRUE(bCreatePremiumSeasonSuccess);

	FCreateRegionDataItem TierRegionDataItem{
		SeasonTierPrice, 0, 0, 0, CurrencyCode, EAccelByteItemCurrencyType::VIRTUAL, FRegistry::Settings.Namespace,
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601(),
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601()
	};
	FItemCreateRequest TierSeasonPassItemRequest{
		EAccelByteItemType::SEASON,
		EAccelByteSeasonType::TIER,
		TEXT("Tier Season Pass UE Integration"),
		EAccelByteEntitlementType::CONSUMABLE,
		1,
		false,
		"",
		EAccelByteAppType::GAME,
		"",
		CurrencyCode,
		FRegistry::Settings.Namespace,
		SeasonItemCategoryPath,
		itemLocalizationMap,
		EAccelByteItemStatus::ACTIVE,
		"TierSeasonPassSku",
		{FAccelByteModelsItemImage{"image", "itemImage", 32, 32, "http://example.com", "http://example.com"}},
		"",
		FRegionDataUS{{TierRegionDataItem}},
		{},
		{},
		-1,
		1,
		"",
		0,
		"classless",
		FRecurring{}
	};
	bool bCreateTierSeasonSuccess = false;
	AdminCreateEcommerceItem(SeasonStoreInfo.storeId, TierSeasonPassItemRequest,
		THandler<FItemFullInfo>::CreateLambda([this, &bCreateTierSeasonSuccess](const FItemFullInfo& Result) {
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Create Tier item succeed"));
			bCreateTierSeasonSuccess = true;
			TierSeasonPassItem = Result;
		}), SeasonPassOnError);
	WaitUntil(bCreateTierSeasonSuccess, "Waiting to create tier season pass item...");
	AB_TEST_TRUE(bCreateTierSeasonSuccess);

	FCreateRegionDataItem CurrencyItemRegionDataItem{
		0, 0, 0, 0, CurrencyCode, EAccelByteItemCurrencyType::VIRTUAL, FRegistry::Settings.Namespace,
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601(),
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601()
	};
	FItemCreateRequest CurrencyItemRequest{
		EAccelByteItemType::COINS,
		EAccelByteSeasonType::PASS,
		TEXT("Currency Coin Pass UE Integration"),
		EAccelByteEntitlementType::CONSUMABLE,
		1,
		true,
		"",
		EAccelByteAppType::GAME,
		"",
		CurrencyCode,
		FRegistry::Settings.Namespace,
		SeasonItemCategoryPath,
		itemLocalizationMap,
		EAccelByteItemStatus::ACTIVE,
		"CoinSeasonPassSku",
		{FAccelByteModelsItemImage{"image", "itemImage", 32, 32, "http://example.com", "http://example.com"}},
		"",
		FRegionDataUS{{TierRegionDataItem}},
		{},
		{},
		-1,
		-1,
		"",
		0,
		"classless",
		FRecurring{}
	};
	bool bCreateCurrencyItemSuccess = false;
	AdminCreateEcommerceItem(SeasonStoreInfo.storeId, CurrencyItemRequest, 
		THandler<FItemFullInfo>::CreateLambda([this, &bCreateCurrencyItemSuccess](const FItemFullInfo& Result) {
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Create Currency item succeed"));
			bCreateCurrencyItemSuccess = true;
			SeasonCurrencyItemInfo = Result;
		}), SeasonPassOnError);
	WaitUntil(bCreateCurrencyItemSuccess, "Waiting to create currency item...");
	AB_TEST_TRUE(bCreateCurrencyItemSuccess);

	FCreateRegionDataItem ItemRewardRegionDataItem{
		0, 0, 0, 0, CurrencyCode, EAccelByteItemCurrencyType::VIRTUAL, FRegistry::Settings.Namespace,
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601(),
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601()
	};
	FItemCreateRequest ItemRewardRequest{
		EAccelByteItemType::INGAMEITEM,
		EAccelByteSeasonType::PASS,
		TEXT("Item Reward UE Integration"),
		EAccelByteEntitlementType::CONSUMABLE,
		1,
		true,
		"",
		EAccelByteAppType::GAME,
		"",
		CurrencyCode,
		FRegistry::Settings.Namespace,
		SeasonItemCategoryPath,
		itemLocalizationMap,
		EAccelByteItemStatus::ACTIVE,
		"ItemRewardSeasonPassSku",
		{FAccelByteModelsItemImage{"image", "itemImage", 32, 32, "http://example.com", "http://example.com"}},
		"",
		FRegionDataUS{{TierRegionDataItem}},
		{},
		{},
		-1,
		-1,
		"",
		0,
		"classless",
		FRecurring{}
	};
	bool bCreateItemRewardSuccess = false;
	AdminCreateEcommerceItem(SeasonStoreInfo.storeId, ItemRewardRequest,
		THandler<FItemFullInfo>::CreateLambda([this, &bCreateItemRewardSuccess](const FItemFullInfo& Result) {
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Create Reward Item succeed"));
			bCreateItemRewardSuccess = true;
			SeasonRewardItemInfo = Result;
		}), SeasonPassOnError);
	WaitUntil(bCreateItemRewardSuccess, "Waiting to create item Reward...");
	AB_TEST_TRUE(bCreateItemRewardSuccess);
#pragma endregion SETUP_DRAFT_STORE

#pragma region SETUP_SEASONPASS
	// Setup the season pass
	FAccelByteSeasonPassExcessStrategy ExcessStrategy{
		EAccelByteSeasonPassStrategyMethod::NONE, CurrencyCode, 1
	};
	FLocalization SeasonLocalization
	{
		"Season pass",
		"Level up your season pass to earn the rewards",
		"Level up your season pass to earn the rewards",
	};
	TMap<FString, FLocalization> SeasonLocalizationMap;
	SeasonLocalizationMap.Add("en", SeasonLocalization);
	FSeasonPassCreateRequest SeasonRequest{
		"Season Pass UE", FDateTime::MinValue().ToIso8601(), FDateTime::MaxValue().ToIso8601(), "en",
		SeasonTierRequiredExp, SeasonStoreInfo.storeId, TierSeasonPassItem.itemId, false, ExcessStrategy,
		SeasonLocalizationMap,
		{FAccelByteModelsItemImage{"image", "seasonImage", 32, 32, "http://example.com", "http://example.com"}}
	};
	bool bCreateSeasonSuccess = false;
	AdminCreateSeason(SeasonRequest, THandler<FSeasonPassCreateSeasonResponse>::CreateLambda([this, &bCreateSeasonSuccess](const FSeasonPassCreateSeasonResponse& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Create Season succeed"));
		bCreateSeasonSuccess = true;
		SeasonResponse = Result;
	}), SeasonPassOnError);
	WaitUntil(bCreateSeasonSuccess, "Waiting to create season");
	AB_TEST_TRUE(bCreateSeasonSuccess);

	FLocalization FreePassLocalization
	{
		"Free pass",
		"Level up your free pass to earn the rewards for free",
		"Level up your free pass to earn the rewards for free",
	};
	TMap<FString, FLocalization> FreePassLocalizationMap;
	FreePassLocalizationMap.Add("en", FreePassLocalization);
	FSeasonPassCreatePassRequest FreePassRequest{
		"free-pass-ue", 0, true, FreeSeasonPassItem.itemId, FreePassLocalizationMap,
		{FAccelByteModelsItemImage{"image", "seasonImage", 32, 32, "http://example.com", "http://example.com"}}
	};
	bool bCreateFreePassSuccess = false;
	AdminCreateSeasonPass(SeasonResponse.Id, FreePassRequest, THandler<FSeasonPassCreatePassResponse>::CreateLambda([this, &bCreateFreePassSuccess](const FSeasonPassCreatePassResponse& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Create Pass succeed"));
		FreePassResponse = Result;
		bCreateFreePassSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bCreateFreePassSuccess, "Waiting to create season pass free pass...");
	AB_TEST_TRUE(bCreateFreePassSuccess);

	FLocalization PremiumPassLocalization
	{
		"Premium pass",
		"Level up your premium pass to earn the rewards for only 100 Coins",
		"Level up your premium pass to earn the rewards for only 100 Coins",
	};
	TMap<FString, FLocalization> PremiumPassLocalizationMap;
	PremiumPassLocalizationMap.Add("en", PremiumPassLocalization);
	FSeasonPassCreatePassRequest PremiumPassRequest{
		"premium-pass-ue", 1, false, PremiumSeasonPassItem.itemId, PremiumPassLocalizationMap,
		{FAccelByteModelsItemImage{"image", "seasonImage", 32, 32, "http://example.com", "http://example.com"}}
	};
	bool bCreatePremiumPassSuccess = false;
	AdminCreateSeasonPass(SeasonResponse.Id, PremiumPassRequest, THandler<FSeasonPassCreatePassResponse>::CreateLambda([this, &bCreatePremiumPassSuccess](const FSeasonPassCreatePassResponse& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Create Premium Pass succeed"));
		PremiumPassResponse = Result;
		bCreatePremiumPassSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bCreatePremiumPassSuccess, "Waiting to create season pass Premium pass...");
	AB_TEST_TRUE(bCreatePremiumPassSuccess);

	FSeasonPassCreateReward CoinsReward{
		CurrencyRewardCode, "ITEM", SeasonCurrencyItemInfo.itemId, CurrencyRewardQuantity,
		{FAccelByteModelsItemImage{"image", "seasonImage", 32, 32, "http://example.com", "http://example.com"}}
	};
	bool bCreateFreeRewardSuccess = false;
	AdminCreateSeasonReward(SeasonResponse.Id, CoinsReward, THandler<FSeasonPassRewardResponse>::CreateLambda([this, &bCreateFreeRewardSuccess](const FSeasonPassRewardResponse& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Create Coin Reward succeed"));
		bCreateFreeRewardSuccess = true;
		CurrencyRewardResponse = Result;
	}), SeasonPassOnError);
	WaitUntil(bCreateFreeRewardSuccess, "Waiting to create season pass coins reward to free pass...");
	AB_TEST_TRUE(bCreateFreeRewardSuccess);

	FSeasonPassCreateReward PremiumReward{
		ItemRewardCode, "ITEM", SeasonRewardItemInfo.itemId, 1,
		{FAccelByteModelsItemImage{"image", "seasonImage", 32, 32, "http://example.com", "http://example.com"}}
	};

	bool bCreatePremiumRewardSuccess = false;
	AdminCreateSeasonReward(SeasonResponse.Id, PremiumReward, THandler<FSeasonPassRewardResponse>::CreateLambda([this, &bCreatePremiumRewardSuccess](const FSeasonPassRewardResponse& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Create Premium Reward succeed"));
		bCreatePremiumRewardSuccess = true;
		ItemRewardResponse = Result;
	}), SeasonPassOnError);
	WaitUntil(bCreatePremiumRewardSuccess, "Waiting to create season pass item reward...");
	AB_TEST_TRUE(bCreatePremiumRewardSuccess);

	// make sure SeasonTierCount is not less than 4! If less, it will not coverage all the test
	if (SeasonTierCount < 4)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Warning,
				TEXT("SeasonTierCount can't less than 4, automatically set it to 4 now."));
		SeasonTierCount = 4;
	}

	SeasonTierRewards.Empty();
	SeasonTiersResponse.Empty();
	const TArray<TSharedPtr<FJsonValue>> PremiumRewardCodes{
		MakeShareable(new FJsonValueString(CurrencyRewardResponse.Code)),
		MakeShareable(new FJsonValueString(ItemRewardResponse.Code))
	};
	const TArray<TSharedPtr<FJsonValue>> FreeRewardCodes{
		MakeShareable(new FJsonValueString(CurrencyRewardResponse.Code))
	};
	FJsonObject RewardValue;
	RewardValue.SetArrayField(PremiumPassResponse.Code, PremiumRewardCodes);
	RewardValue.SetArrayField(FreePassResponse.Code, FreeRewardCodes);

	FJsonObjectWrapper RewardWrapper;
	RewardWrapper.JsonObject = MakeShared<FJsonObject>(RewardValue);

	SeasonTierRewards.Add(RewardWrapper.JsonObject);

	FSeasonPassTierRequest Tier{
		SeasonTierRequiredExp, RewardWrapper
	};
	FSeasonPassCreateTierRequest TierRequest{
		0, 6, Tier
	};
	bool bCreateTierSuccess = false;
	AdminCreateSeasonTier(SeasonResponse.Id, TierRequest,THandler<TArray<FAccelByteModelsSeasonPassTierJsonObject>>::CreateLambda(
	[this, &bCreateTierSuccess](const TArray<FAccelByteModelsSeasonPassTierJsonObject>& Result)
	{
		bCreateTierSuccess = true;
		SeasonTiersResponse.Append(Result);
	}), SeasonPassOnError);
	WaitUntil(bCreateTierSuccess, "Waiting to create tier...");
	AB_TEST_TRUE(bCreateTierSuccess);

#pragma endregion SETUP_SEASONPASS

	// Publish the store and Season pass

	/*
	Publish this store, CreatedTemporaryStoreInfo.
	*/
	bool bPublishStoreSuccess = false;
	AdminCloneEcommerceStore(SeasonStoreInfo.storeId, "", FSimpleDelegate::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Season Pass STORE is published"));
		bPublishStoreSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bPublishStoreSuccess, "Waiting for publish Season Pass store...");
	AB_TEST_TRUE(bPublishStoreSuccess);

	bool bPublishSeasonPassSuccess = false;
	AdminPublishSeason(SeasonResponse.Id, THandler<FSeasonPassCreateSeasonResponse>::CreateLambda([&](const FSeasonPassCreateSeasonResponse& Result)
	{
	UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Season is published"));
	bPublishSeasonPassSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bPublishSeasonPassSuccess, "Waiting to publish season pass");
	AB_TEST_TRUE(bPublishSeasonPassSuccess);

	bool bLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("user login succeed"));
		bLoginSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bLoginSuccess, "Waiting to logon...");
	AB_TEST_TRUE(bLoginSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SeasonPassTeardown, "AccelByte.Tests.SeasonPass.Z.Teardown", AutomationFlagMaskSeasonPass)
bool SeasonPassTeardown::RunTest(const FString& Parameters)
{
	bool bQuerySeasonSuccess = false;
	FSeasonPassCreateGetSeasonsPagingResponse SeasonsResponse;
	AdminQuerySeason({EAccelByteSeasonPassStatus::DRAFT, EAccelByteSeasonPassStatus::PUBLISHED},THandler<FSeasonPassCreateGetSeasonsPagingResponse>::CreateLambda([&bQuerySeasonSuccess, &SeasonsResponse](
	const FSeasonPassCreateGetSeasonsPagingResponse& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("query existing season succeed"));
		bQuerySeasonSuccess = true;
		SeasonsResponse = Result;
	}), SeasonPassOnError);
	WaitUntil(bQuerySeasonSuccess, "Waiting to query existing season...");
	AB_TEST_TRUE(bQuerySeasonSuccess);

	for (const FSeasonPassCreateGetSeasonResponse& Season : SeasonsResponse.Data)
	{
		if (Season.Status == EAccelByteSeasonPassStatus::PUBLISHED)
		{
			// Unpublish before delete
			bool bUnpublishSuccess = false;
			AdminForceUnpublishSeason(Season.Id, THandler<FSeasonPassCreateSeasonResponse>::CreateLambda([&bUnpublishSuccess](const FSeasonPassCreateSeasonResponse& Result)
			{
				UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("unpublish season succeed"));
				bUnpublishSuccess = true;
			}), SeasonPassOnError);
			WaitUntil(bUnpublishSuccess, "Waiting to unpublish season...");
			AB_TEST_TRUE(bUnpublishSuccess);
		}

		bool bDeleteSeasonSuccess = false;
		AdminDeleteSeason(Season.Id, FSimpleDelegate::CreateLambda([&bDeleteSeasonSuccess]()
		{
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Delete season succeed"));
			bDeleteSeasonSuccess = true;
		}), SeasonPassOnError);
		WaitUntil(bDeleteSeasonSuccess, "Waiting delete season...");
		AB_TEST_TRUE(bDeleteSeasonSuccess);
	}

	// Delete testing currency
	bool bCurrencyDeleted = false;
	AdminDeleteEcommerceCurrency(CurrencyCode, FSimpleDelegate::CreateLambda([&bCurrencyDeleted]()
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("CURRENCY is deleted"));
		bCurrencyDeleted = true;
	}), SeasonPassOnError);
	WaitUntil(bCurrencyDeleted, "Waiting for currency deletion...");
	AB_TEST_TRUE(bCurrencyDeleted);

	// Delete published testing store
	bool bPublishedStoreDeleted = false;
	AdminDeleteEcommercePublishedStore(FSimpleDelegate::CreateLambda([&bPublishedStoreDeleted]()
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Season pass store is deleted"));
		bPublishedStoreDeleted = true;
	}), nullptr);
	WaitUntil(bPublishedStoreDeleted, "Waiting for Season pass store deletion...");
	AB_TEST_TRUE(bPublishedStoreDeleted);

	bool bGetAllStoreSuccess = false;
	TArray<FStoreInfo> GetAllResult;
	AdminGetEcommerceStoreAll(
		THandler<TArray<FStoreInfo>>::CreateLambda([&](const TArray<FStoreInfo>& Result)
		{
			GetAllResult = Result;
			bGetAllStoreSuccess = true;
		}), SeasonPassOnError);
	WaitUntil(bGetAllStoreSuccess, "Waiting for get all store...");
	for (int i = 0; i < GetAllResult.Num(); i++)
	{
		bool bDeleteTestingStoreSuccess = false;
		AdminDeleteEcommerceStore(GetAllResult[i].storeId,FSimpleDelegate::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Delete store succeed"));
			bDeleteTestingStoreSuccess = true;
		}), SeasonPassOnError);
		WaitUntil(bDeleteTestingStoreSuccess, "Waiting for testing store deletion...");
		AB_TEST_TRUE(bDeleteTestingStoreSuccess);
	}

	bool bDeleteUserSuccess = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bDeleteUserSuccess]()
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Delete user succeed"));
		bDeleteUserSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bDeleteUserSuccess, "Waiting to delete user...");
	AB_TEST_TRUE(bDeleteUserSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SeasonGetCurrentSeason, "AccelByte.Tests.SeasonPass.B.GetCurrentSeason", AutomationFlagMaskSeasonPass)
bool SeasonGetCurrentSeason::RunTest(const FString& Parameters)
{
	bool bGetSeason = false;
	FAccelByteModelsSeasonInfo GetSeason;
	FRegistry::SeasonPass.GetCurrentSeason("en", THandler<FAccelByteModelsSeasonInfo>::CreateLambda([&](const FAccelByteModelsSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get season succeed"));
		bGetSeason = true;
		GetSeason = Result;
	}), SeasonPassOnError);
	WaitUntil(bGetSeason, "Waiting to get season...");
	AB_TEST_TRUE(bGetSeason);
	AB_TEST_EQUAL(GetSeason.Id, SeasonResponse.Id);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SeasonGetCurrentUserSeason, "AccelByte.Tests.SeasonPass.B.GetCurrentUserSeason", AutomationFlagMaskSeasonPass)
bool SeasonGetCurrentUserSeason::RunTest(const FString& Parameters)
{
	bool bGetSeason = false;
	FAccelByteModelsSeasonInfo GetSeason;
	FRegistry::SeasonPass.GetCurrentSeason("en", THandler<FAccelByteModelsSeasonInfo>::CreateLambda([&](const FAccelByteModelsSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get season succeed"));
		bGetSeason = true;
		GetSeason = Result;
	}), SeasonPassOnError);
	WaitUntil(bGetSeason, "Waiting to get season...");
	AB_TEST_TRUE(bGetSeason);
	AB_TEST_EQUAL(GetSeason.Id, SeasonResponse.Id);

	bool bGetUserSeasonInfo = false;
	FAccelByteModelsUserSeasonInfo GetUserSeason;
	FRegistry::SeasonPass.GetCurrentUserSeason(THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda([&](const FAccelByteModelsUserSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get user season succeed"));
		GetUserSeason = Result;
		bGetUserSeasonInfo = true;
	}), SeasonPassOnError);
	WaitUntil(bGetUserSeasonInfo, "Waiting to get user season data info...");
	AB_TEST_TRUE(bGetUserSeasonInfo);
	AB_TEST_EQUAL(GetUserSeason.SeasonId, GetSeason.Id);
	AB_TEST_EQUAL(GetUserSeason.RequiredExp, SeasonTierRequiredExp);

	bGetUserSeasonInfo = false;
	FRegistry::SeasonPass.GetUserSeason(GetSeason.Id, THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda([&](const FAccelByteModelsUserSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get user season succeed"));
		GetUserSeason = Result;
		bGetUserSeasonInfo = true;
	}), SeasonPassOnError);
	WaitUntil(bGetUserSeasonInfo, "Waiting to get user season data info...");
	AB_TEST_TRUE(bGetUserSeasonInfo);
	AB_TEST_EQUAL(GetUserSeason.SeasonId, GetSeason.Id);
	AB_TEST_EQUAL(GetUserSeason.RequiredExp, SeasonTierRequiredExp);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SeasonGetInvalidUserSeason, "AccelByte.Tests.SeasonPass.B.GetInvalidUserSeason", AutomationFlagMaskSeasonPass)
bool SeasonGetInvalidUserSeason::RunTest(const FString& Parameters)
{
	bool bGetUserSeasonInfo = false;
	bool bGetUserSeasonInfoFailed = false;
	FRegistry::SeasonPass.GetUserSeason("INVALID_ID", THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda([&](const FAccelByteModelsUserSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get user season succeed"));
		bGetUserSeasonInfo = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log,	TEXT("Get user season failed [EXPECTED]"));
		bGetUserSeasonInfo = true;
		bGetUserSeasonInfoFailed = true;
	}));
	WaitUntil(bGetUserSeasonInfo, "Waiting to get invalid user season data info...");
	AB_TEST_TRUE(bGetUserSeasonInfoFailed);

	return true;
}

// Test C is about enroll pass
IMPLEMENT_SIMPLE_AUTOMATION_TEST(SeasonEnrollPremiumPass, "AccelByte.Tests.SeasonPass.C.EnrollPremiumPass",	AutomationFlagMaskSeasonPass)
bool SeasonEnrollPremiumPass::RunTest(const FString& Parameters)
{
	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::SEASON;
	ItemCriteria.CategoryPath = SeasonItemCategoryPath;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	FAccelByteModelsItemInfo PremiumPassItemInfo;
	UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get Season Pass Item"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20,THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		for (int i = 0; i < Result.Data.Num(); i++)
		{
			if (Result.Data[i].Name.Equals(PremiumSeasonPassItem.name))
			{
				UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("        Found"));
				PremiumPassItemInfo = Result.Data[i];
				bExpectedItemFound = true;
				bGetItemByCriteriaSuccess = true;
			}
		}
	}), SeasonPassOnError);
	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get item...");
	AB_TEST_TRUE(bExpectedItemFound);

	bool bLoginClientSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Login with server creds success"));
		bLoginClientSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bLoginClientSuccess, "Waiting to login with server creds...");
	AB_TEST_TRUE(bLoginClientSuccess);

	// Grant currency
	FAccelByteModelsCreditUserWalletRequest CreditRequest{
		PremiumPassPrice, EAccelByteCreditUserWalletSource::OTHER, "UE Integration Test"
	};
	bool bCreditUserWalletSuccess = false;
	FRegistry::ServerEcommerce.CreditUserWallet(FRegistry::Credentials.GetUserId(), CurrencyCode, CreditRequest,THandler<FAccelByteModelsWalletInfo>::CreateLambda(	[&](const FAccelByteModelsWalletInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("credit wallet succeed"));
		bCreditUserWalletSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bCreditUserWalletSuccess, "Waiting to credit user wallet...");
	AB_TEST_TRUE(bCreditUserWalletSuccess);

	const int32 Quantity = 1;
	FAccelByteModelsOrderCreate OrderCreate;
	OrderCreate.CurrencyCode = PremiumPassItemInfo.RegionData[0].CurrencyCode;
	OrderCreate.DiscountedPrice = PremiumPassItemInfo.RegionData[0].DiscountedPrice * Quantity;
	OrderCreate.Price = PremiumPassItemInfo.RegionData[0].Price * Quantity;
	OrderCreate.Quantity = Quantity;
	OrderCreate.ReturnUrl = TEXT("https://sdk.example.com");
	OrderCreate.ItemId = PremiumPassItemInfo.ItemId;
	OrderCreate.Region = "US";
	OrderCreate.Language = "en";

	FString OrderString;
	FJsonObjectConverter::UStructToJsonObjectString(OrderCreate, OrderString);
	UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("%s"), *OrderString);

	bool bCreateNewOrderSuccess = false;
	UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("CreateNewOrder"));
	FRegistry::Order.CreateNewOrder(OrderCreate, THandler<FAccelByteModelsOrderInfo>::CreateLambda(
	[&](FAccelByteModelsOrderInfo Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("    Success"));
		bCreateNewOrderSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bCreateNewOrderSuccess, "Waiting for new order created...");

	bool bGetUserSeasonInfo = false;
	FAccelByteModelsUserSeasonInfo GetUserSeason;
	FRegistry::SeasonPass.GetUserSeason(SeasonResponse.Id, THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda(
	[&](const FAccelByteModelsUserSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get user season succeed"));
		GetUserSeason = Result;
		bGetUserSeasonInfo = true;
	}), SeasonPassOnError);
	WaitUntil(bGetUserSeasonInfo, "Waiting to get user season data info...");
	AB_TEST_TRUE(bGetUserSeasonInfo);
	AB_TEST_EQUAL(GetUserSeason.SeasonId, SeasonResponse.Id);
	AB_TEST_EQUAL(GetUserSeason.RequiredExp, SeasonTierRequiredExp);
	AB_TEST_TRUE(GetUserSeason.EnrolledPasses.Contains(PremiumPassResponse.Code));

	return true;
}

// Test D is about tier level up
IMPLEMENT_SIMPLE_AUTOMATION_TEST(SeasonGainExp, "AccelByte.Tests.SeasonPass.D.GainExp", AutomationFlagMaskSeasonPass)
bool SeasonGainExp::RunTest(const FString& Parameters)
{
	bool bGetSeason = false;
	FAccelByteModelsSeasonInfo GetSeason;
	FRegistry::SeasonPass.GetCurrentSeason("en", THandler<FAccelByteModelsSeasonInfo>::CreateLambda(
	[&](const FAccelByteModelsSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get season succeed"));
		bGetSeason = true;
		GetSeason = Result;
	}), SeasonPassOnError);
	WaitUntil(bGetSeason, "Waiting to get season...");
	AB_TEST_TRUE(bGetSeason);
	AB_TEST_EQUAL(GetSeason.Id, SeasonResponse.Id);

	bool bGetUserSeasonInfo = false;
	FAccelByteModelsUserSeasonInfo GetUserSeason;
	FRegistry::SeasonPass.GetUserSeason(GetSeason.Id, THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda(
	[&](const FAccelByteModelsUserSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get user season succeed"));
		GetUserSeason = Result;
		bGetUserSeasonInfo = true;
	}), SeasonPassOnError);
	WaitUntil(bGetUserSeasonInfo, "Waiting to get user season data info...");
	AB_TEST_TRUE(bGetUserSeasonInfo);
	AB_TEST_EQUAL(GetUserSeason.SeasonId, GetSeason.Id);
	AB_TEST_EQUAL(GetUserSeason.RequiredExp, SeasonTierRequiredExp);

	const int32 LastExp = GetUserSeason.CurrentExp;

	bool bLoginClientSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Login with server creds success"));
		bLoginClientSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bLoginClientSuccess, "Waiting to login with server creds...");
	AB_TEST_TRUE(bLoginClientSuccess);

	const int32 ExpectedExp = FMath::RoundToInt(LastExp + (SeasonTierRequiredExp * 0.5)) % SeasonTierRequiredExp;
	// Gain exp by one level
	// Using the ServerApi
	bool bGrantExpSuccess = false;
	FAccelByteModelsUserSeasonInfoWithoutReward UserSeasonAfterGrantedExp;
	FRegistry::ServerSeasonPass.GrantExpToUser(FRegistry::Credentials.GetUserId(), SeasonTierRequiredExp * 0.5,THandler<FAccelByteModelsUserSeasonInfoWithoutReward>::CreateLambda(
	[&](const FAccelByteModelsUserSeasonInfoWithoutReward& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Grant exp to user"));
		bGrantExpSuccess = true;
		UserSeasonAfterGrantedExp = Result;
	}), SeasonPassOnError);
	WaitUntil(bGrantExpSuccess, "Waiting to grant exp using server api...");
	AB_TEST_TRUE(bGrantExpSuccess);
	AB_TEST_EQUAL(ExpectedExp, UserSeasonAfterGrantedExp.CurrentExp);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SeasonGainExpLevelUp, "AccelByte.Tests.SeasonPass.D.GainExpLevelUp", AutomationFlagMaskSeasonPass)
bool SeasonGainExpLevelUp::RunTest(const FString& Parameters)
{
	bool bGetSeason = false;
	FAccelByteModelsSeasonInfo GetSeason;
	FRegistry::SeasonPass.GetCurrentSeason("en", THandler<FAccelByteModelsSeasonInfo>::CreateLambda(
	[&](const FAccelByteModelsSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get season succeed"));
		bGetSeason = true;
		GetSeason = Result;
	}), SeasonPassOnError);
	WaitUntil(bGetSeason, "Waiting to get season...");
	AB_TEST_TRUE(bGetSeason);
	AB_TEST_EQUAL(GetSeason.Id, SeasonResponse.Id);

	bool bGetUserSeasonInfo = false;
	FAccelByteModelsUserSeasonInfo GetUserSeason;
	FRegistry::SeasonPass.GetUserSeason(GetSeason.Id, THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda(
	[&](const FAccelByteModelsUserSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get user season succeed"));
		GetUserSeason = Result;
		bGetUserSeasonInfo = true;
	}), SeasonPassOnError);
	WaitUntil(bGetUserSeasonInfo, "Waiting to get user season data info...");
	AB_TEST_TRUE(bGetUserSeasonInfo);
	AB_TEST_EQUAL(GetUserSeason.SeasonId, GetSeason.Id);
	AB_TEST_EQUAL(GetUserSeason.RequiredExp, SeasonTierRequiredExp);

	const int32 LastExp = GetUserSeason.CurrentExp;

	bool bLoginClientSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Login with server creds success"));
		bLoginClientSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bLoginClientSuccess, "Waiting to login with server creds...");
	AB_TEST_TRUE(bLoginClientSuccess);

	const int32 ExpectedExp = (LastExp + SeasonTierRequiredExp) % SeasonTierRequiredExp;
	// Gain exp by one level
	// Using the ServerApi
	bool bGrantExpSuccess = false;
	FAccelByteModelsUserSeasonInfoWithoutReward UserSeasonAfterGrantedExp;
	FRegistry::ServerSeasonPass.GrantExpToUser(FRegistry::Credentials.GetUserId(), SeasonTierRequiredExp,THandler<FAccelByteModelsUserSeasonInfoWithoutReward>::CreateLambda(
	[&](const FAccelByteModelsUserSeasonInfoWithoutReward& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log,	TEXT("Grant exp to user"));
		bGrantExpSuccess = true;
		UserSeasonAfterGrantedExp = Result;
	}), SeasonPassOnError);
	WaitUntil(bGrantExpSuccess, "Waiting to grant exp using server api...");
	AB_TEST_TRUE(bGrantExpSuccess);
	AB_TEST_EQUAL(ExpectedExp, UserSeasonAfterGrantedExp.CurrentExp);

	// Get current user progression
	bool bGetUserProgressionSuccess = false;
	FAccelByteModelsUserSeasonInfoWithoutReward CurrentUserProgression;
	FRegistry::ServerSeasonPass.GetCurrentUserSeasonProgression(FRegistry::Credentials.GetUserId(), THandler<FAccelByteModelsUserSeasonInfoWithoutReward>::CreateLambda(
		[&bGetUserProgressionSuccess, &CurrentUserProgression](const FAccelByteModelsUserSeasonInfoWithoutReward& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log,	TEXT("Get user progression success"));
		bGetUserProgressionSuccess = true;
		CurrentUserProgression = Result;
	}), SeasonPassOnError);
	WaitUntil(bGetUserProgressionSuccess, "waiting to get current user progression");
	AB_TEST_TRUE(bGetUserProgressionSuccess);
	AB_TEST_EQUAL(ExpectedExp, CurrentUserProgression.CurrentExp);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SeasonBuyTiers, "AccelByte.Tests.SeasonPass.D.BuyTier", AutomationFlagMaskSeasonPass)
bool SeasonBuyTiers::RunTest(const FString& Parameters)
{
	bool bGetUserSeasonInfo = false;
	FAccelByteModelsUserSeasonInfo GetUserSeason;
	FRegistry::SeasonPass.GetUserSeason(SeasonResponse.Id, THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda(
	[&](const FAccelByteModelsUserSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get user season succeed"));
		GetUserSeason = Result;
		bGetUserSeasonInfo = true;
	}), SeasonPassOnError);
	WaitUntil(bGetUserSeasonInfo, "Waiting to get user season data info...");
	AB_TEST_TRUE(bGetUserSeasonInfo);
	AB_TEST_EQUAL(GetUserSeason.SeasonId, SeasonResponse.Id);
	AB_TEST_EQUAL(GetUserSeason.RequiredExp, SeasonTierRequiredExp);

	bool bLoginClientSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Login with server creds success"));
		bLoginClientSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bLoginClientSuccess, "Waiting to login with server creds...");
	AB_TEST_TRUE(bLoginClientSuccess);

	// Grant currency
	FAccelByteModelsCreditUserWalletRequest CreditRequest{
		SeasonTierPrice, EAccelByteCreditUserWalletSource::OTHER, "UE Integration Test"
	};
	bool bCreditUserWalletSuccess = false;
	FRegistry::ServerEcommerce.CreditUserWallet(FRegistry::Credentials.GetUserId(), CurrencyCode, CreditRequest,THandler<FAccelByteModelsWalletInfo>::CreateLambda(
	[&](const FAccelByteModelsWalletInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("credit wallet succeed"));
		bCreditUserWalletSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bCreditUserWalletSuccess, "Waiting to credit user wallet...");

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::SEASON;
	ItemCriteria.CategoryPath = SeasonItemCategoryPath;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	FAccelByteModelsItemInfo TierItemInfo;
	UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get Season Pass Item"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20,THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda(
	[&](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		for (int i = 0; i < Result.Data.Num(); i++)
		{
			if (Result.Data[i].Name.Equals(TierSeasonPassItem.name))
			{
				UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("        Found"));
				TierItemInfo = Result.Data[i];
				bExpectedItemFound = true;
				bGetItemByCriteriaSuccess = true;
			}
		}
	}), SeasonPassOnError);
	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get item...");
	AB_TEST_TRUE(bExpectedItemFound);

	const int32 Quantity = 1;
	FAccelByteModelsOrderCreate OrderCreate;
	OrderCreate.CurrencyCode = TierItemInfo.RegionData[0].CurrencyCode;
	OrderCreate.DiscountedPrice = TierItemInfo.RegionData[0].DiscountedPrice * Quantity;
	OrderCreate.Price = TierItemInfo.RegionData[0].Price * Quantity;
	OrderCreate.Quantity = Quantity;
	OrderCreate.ReturnUrl = TEXT("https://sdk.example.com");
	OrderCreate.ItemId = TierItemInfo.ItemId;
	OrderCreate.Region = "US";
	OrderCreate.Language = "en";

	FString OrderString;
	FJsonObjectConverter::UStructToJsonObjectString(OrderCreate, OrderString);
	UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("%s"), *OrderString);

	bool bCreateNewOrderSuccess = false;
	UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("CreateNewOrder"));
	FRegistry::Order.CreateNewOrder(OrderCreate, THandler<FAccelByteModelsOrderInfo>::CreateLambda(
	[&](FAccelByteModelsOrderInfo Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("    Success"));
		bCreateNewOrderSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bCreateNewOrderSuccess, "Waiting for new order created...");


	bGetUserSeasonInfo = false;
	FAccelByteModelsUserSeasonInfo UserSeasonAfterOrderTier;
	FRegistry::SeasonPass.GetUserSeason(SeasonResponse.Id, THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda(
	[&](const FAccelByteModelsUserSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get user season succeed"));
		UserSeasonAfterOrderTier = Result;
		bGetUserSeasonInfo = true;
	}), SeasonPassOnError);
	WaitUntil(bGetUserSeasonInfo, "Waiting to get user season data info...");
	AB_TEST_TRUE(bGetUserSeasonInfo);
	AB_TEST_EQUAL(GetUserSeason.SeasonId, UserSeasonAfterOrderTier.SeasonId);
	AB_TEST_EQUAL(UserSeasonAfterOrderTier.RequiredExp, SeasonTierRequiredExp);
	AB_TEST_EQUAL(UserSeasonAfterOrderTier.CurrentTierIndex, GetUserSeason.CurrentTierIndex + Quantity);

	return true;
}

// Test E is about rewards
IMPLEMENT_SIMPLE_AUTOMATION_TEST(SeasonClaimTierReward, "AccelByte.Tests.SeasonPass.E.ClaimTierRewards", AutomationFlagMaskSeasonPass)
bool SeasonClaimTierReward::RunTest(const FString& Parameters)
{
	// Get current progress
	bool bGetUserSeasonInfo = false;
	FAccelByteModelsUserSeasonInfo GetUserSeason;
	FRegistry::SeasonPass.GetUserSeason(SeasonResponse.Id, THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda(
	[&](const FAccelByteModelsUserSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get user season succeed"));
		GetUserSeason = Result;
		bGetUserSeasonInfo = true;
	}), SeasonPassOnError);
	WaitUntil(bGetUserSeasonInfo, "Waiting to get user season data info...");
	AB_TEST_TRUE(bGetUserSeasonInfo);
	AB_TEST_EQUAL(GetUserSeason.SeasonId, SeasonResponse.Id);

	while (GetUserSeason.ToClaimRewards.Num() == 0)
	{
		// grant exp to levelup
		bool bLoginClientSuccess = false;
		FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Login with server creds success"));
			bLoginClientSuccess = true;
		}), SeasonPassOnError);
		WaitUntil(bLoginClientSuccess, "Waiting to login with server creds...");
		AB_TEST_TRUE(bLoginClientSuccess);

		// Gain exp by one level
		// Using the ServerApi
		bool bGrantExpSuccess = false;
		FRegistry::ServerSeasonPass.GrantExpToUser(FRegistry::Credentials.GetUserId(), SeasonTierRequiredExp,THandler<FAccelByteModelsUserSeasonInfoWithoutReward>::CreateLambda(
		[&](const FAccelByteModelsUserSeasonInfoWithoutReward& Result)
		{
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Grant exp to user"));
			bGrantExpSuccess = true;
		}), SeasonPassOnError);
		WaitUntil(bGrantExpSuccess, "Waiting to grant exp using server api...");
		AB_TEST_TRUE(bGrantExpSuccess);

		bGetUserSeasonInfo = false;
		FRegistry::SeasonPass.GetUserSeason(SeasonResponse.Id, THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda(
		[&](const FAccelByteModelsUserSeasonInfo& Result)
		{
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get user season succeed"));
			GetUserSeason = Result;
			bGetUserSeasonInfo = true;
		}), SeasonPassOnError);
		WaitUntil(bGetUserSeasonInfo, "Waiting to get user season data info...");
		AB_TEST_TRUE(bGetUserSeasonInfo);
		AB_TEST_EQUAL(GetUserSeason.SeasonId, SeasonResponse.Id);
	}

	for (const TTuple<int, TMap<FString, TArray<FString>>>& ClaimReward : GetUserSeason.ToClaimRewards)
	{
		for (const TTuple<FString, TArray<FString>>& PassReward : ClaimReward.Value)
		{
			for (const FString& RewardCode : PassReward.Value)
			{
				FAccelByteModelsWalletInfo WalletInfo;
				bool bGetWalletInfoSuccess = false;
				if (RewardCode.Equals(CurrencyRewardCode))
				{
					FRegistry::Wallet.GetWalletInfoByCurrencyCode(CurrencyCode, THandler<FAccelByteModelsWalletInfo>::CreateLambda(
					[&bGetWalletInfoSuccess, &WalletInfo](
					const FAccelByteModelsWalletInfo& Result)
					{
						UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Wallet Balance : %d"), Result.Balance);
						WalletInfo = Result;
						bGetWalletInfoSuccess = true;
					}), SeasonPassOnError);
					WaitUntil(bGetWalletInfoSuccess, "Waiting to get wallet info");
				}

				FAccelByteModelsSeasonClaimRewardRequest ClaimRequest{
					PassReward.Key, ClaimReward.Key, RewardCode
				};
				bool bClaimRewardSuccess = false;
				FRegistry::SeasonPass.ClaimRewards(ClaimRequest, THandler<FAccelByteModelsSeasonClaimRewardResponse>::CreateLambda(
				[&bClaimRewardSuccess, RewardCode](const FAccelByteModelsSeasonClaimRewardResponse& Result)
				{
					UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Claim Reward success : %s"), *RewardCode);
					bClaimRewardSuccess = true;
				}), SeasonPassOnError);
				WaitUntil(bClaimRewardSuccess,FString::Printf(TEXT("Waiting to claim reward. Tier : %d, Pass : %s, RewardCode : %s"), ClaimReward.Key, *PassReward.Key, *RewardCode));
				AB_TEST_TRUE(bClaimRewardSuccess);

				if (RewardCode.Equals(CurrencyRewardCode))
				{
					bGetWalletInfoSuccess = false;
					int32 LastBalance = 0;
					FRegistry::Wallet.GetWalletInfoByCurrencyCode(CurrencyCode,THandler<FAccelByteModelsWalletInfo>::CreateLambda(
					[&bGetWalletInfoSuccess, &LastBalance](const FAccelByteModelsWalletInfo& Result)
					{
						UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("After claiming reward, Wallet Balance : %d"), Result.Balance);
						LastBalance = Result.Balance;
						bGetWalletInfoSuccess = true;
					}), SeasonPassOnError);
					WaitUntil(bGetWalletInfoSuccess, "Waiting to get wallet info");
					AB_TEST_TRUE(LastBalance > WalletInfo.Balance);
				}
				else
				{
					bool bIsFound = false;
					bool bGetEntitlementSuccess = false;
					FAccelByteModelsEntitlementInfo EntitlementInfo;
					// after claiming rewards, user should update inventory automatically (end user)
					FRegistry::Entitlement.QueryUserEntitlements("", ItemRewardResponse.ItemId, 0, 20, THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda(
					[&](const FAccelByteModelsEntitlementPagingSlicedResult&Result)
					{
						bGetEntitlementSuccess = true;
						for (const FAccelByteModelsEntitlementInfo& Ent : Result.Data)
						{
							if (Ent.ItemId.Equals(
								ItemRewardResponse.ItemId))
							{
								UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Claimed Reward in the entitlement!"));
								bIsFound = true;
								EntitlementInfo = Ent;
								break;
							}
						}
					}), SeasonPassOnError, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);
					WaitUntil(bGetEntitlementSuccess, "Waiting to get entitlement");

					if (bIsFound)
					{
						bool bConsumeEntitlementSuccess = false;
						FRegistry::Entitlement.ConsumeUserEntitlement(EntitlementInfo.Id, 1, THandler<FAccelByteModelsEntitlementInfo>::CreateLambda(
						[&bConsumeEntitlementSuccess](const FAccelByteModelsEntitlementInfo& Result)
						{
							UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Consume right away!"));
							bConsumeEntitlementSuccess = true;
						}), SeasonPassOnError);
						WaitUntil(bConsumeEntitlementSuccess, "Waiting to consume the entitlement");
					}
				}
			}
		}
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SeasonBulkClaimTierReward, "AccelByte.Tests.SeasonPass.E.BulkClaimTierRewards", AutomationFlagMaskSeasonPass)
bool SeasonBulkClaimTierReward::RunTest(const FString& Parameters)
{
	int32 LastBalanceCount = 0;
	bool bGetWalletInfoSuccess = false;
	FRegistry::Wallet.GetWalletInfoByCurrencyCode(CurrencyCode, THandler<FAccelByteModelsWalletInfo>::CreateLambda(
	[&bGetWalletInfoSuccess, &LastBalanceCount](
	const FAccelByteModelsWalletInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Wallet Balance : %d"), Result.Balance);
		LastBalanceCount = Result.Balance;
		bGetWalletInfoSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bGetWalletInfoSuccess, "Waiting to get wallet info");
	
	bool bGetEntitlementSuccess = false;
	int32 EntitlementUseCount = 0;
	// after claiming rewards, user should update inventory automatically (end user)
	FRegistry::Entitlement.QueryUserEntitlements("", ItemRewardResponse.ItemId, 0, 20, THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda(
	[&](const FAccelByteModelsEntitlementPagingSlicedResult&Result)
	{
		bGetEntitlementSuccess = true;
		for (const FAccelByteModelsEntitlementInfo& Ent : Result.Data)
		{
			if (Ent.ItemId.Equals(ItemRewardResponse.ItemId))
			{
				UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Claimed Reward in the entitlement!"));
				EntitlementUseCount = Ent.UseCount;
				break;
			}
		}
	}), SeasonPassOnError, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);
	WaitUntil(bGetEntitlementSuccess, "Waiting to get entitlement");
	
	// Get current progress
	bool bGetUserSeasonInfo = false;
	FAccelByteModelsUserSeasonInfo GetUserSeason;
	FRegistry::SeasonPass.GetUserSeason(SeasonResponse.Id, THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda(
	[&](const FAccelByteModelsUserSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get user season succeed"));
		GetUserSeason = Result;
		bGetUserSeasonInfo = true;
	}), SeasonPassOnError);
	WaitUntil(bGetUserSeasonInfo, "Waiting to get user season data info...");
	AB_TEST_TRUE(bGetUserSeasonInfo);
	AB_TEST_EQUAL(GetUserSeason.SeasonId, SeasonResponse.Id);

	// loop until test have claim rewards more than 1
	while (GetUserSeason.ToClaimRewards.Num() < 2)
	{
		// prevent infinite loop
		AB_TEST_TRUE(GetUserSeason.CurrentTierIndex < GetUserSeason.LastTierIndex);
		// grant exp to levelup
		bool bLoginClientSuccess = false;
		FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Login with server creds success"));
			bLoginClientSuccess = true;
		}), SeasonPassOnError);
		WaitUntil(bLoginClientSuccess, "Waiting to login with server creds...");
		AB_TEST_TRUE(bLoginClientSuccess);

		const int32 LevelUp = 2 - GetUserSeason.ToClaimRewards.Num();
		// Gain exp by one level
		// Using the ServerApi
		bool bGrantExpSuccess = false;
		FRegistry::ServerSeasonPass.GrantExpToUser(FRegistry::Credentials.GetUserId(), SeasonTierRequiredExp * LevelUp,THandler<FAccelByteModelsUserSeasonInfoWithoutReward>::CreateLambda(
		[&](const FAccelByteModelsUserSeasonInfoWithoutReward& Result)
		{
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Grant exp to user"));
			bGrantExpSuccess = true;
		}), SeasonPassOnError);
		WaitUntil(bGrantExpSuccess, "Waiting to grant exp using server api...");
		AB_TEST_TRUE(bGrantExpSuccess);

		bGetUserSeasonInfo = false;
		FRegistry::SeasonPass.GetCurrentUserSeason(THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda(
		[&](const FAccelByteModelsUserSeasonInfo& Result)
		{
			UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get user season succeed"));
			GetUserSeason = Result;
			bGetUserSeasonInfo = true;
		}), SeasonPassOnError);
		WaitUntil(bGetUserSeasonInfo, "Waiting to get user season data info...");
		AB_TEST_TRUE(bGetUserSeasonInfo);
		AB_TEST_EQUAL(GetUserSeason.SeasonId, SeasonResponse.Id);
	}

	int32 CurrencyRewardCount = 0;
	int32 ItemRewardCount = 0;
	// Store how many reward to make a validation in the end
	for (const TTuple<int, TMap<FString, TArray<FString>>>& ClaimReward : GetUserSeason.ToClaimRewards)
	{
		for (const TTuple<FString, TArray<FString>>& PassReward : ClaimReward.Value)
		{
			for (const FString& RewardCode : PassReward.Value)
			{
				if(RewardCode.Equals(CurrencyRewardCode))
					CurrencyRewardCount++;
				else if(RewardCode.Equals(ItemRewardCode))
					ItemRewardCount++;
			}
		}
	}
	int32 ExpectedCurrencyBalance = LastBalanceCount + (CurrencyRewardCount * CurrencyRewardQuantity);
	int32 ExpectedItemUseCount = EntitlementUseCount + ItemRewardCount;
	
	bool bClaimRewardSuccess = false;
	FRegistry::SeasonPass.BulkClaimRewards(THandler<FAccelByteModelsSeasonClaimRewardResponse>::CreateLambda(
	[&bClaimRewardSuccess](const FAccelByteModelsSeasonClaimRewardResponse& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Bulk Claim Reward success"));
		bClaimRewardSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bClaimRewardSuccess, FString::Printf(TEXT("Waiting to bulk claim reward.")));
	AB_TEST_TRUE(bClaimRewardSuccess);

	bGetUserSeasonInfo = false;
	FRegistry::SeasonPass.GetCurrentUserSeason(THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda(
	[&](const FAccelByteModelsUserSeasonInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Get user season succeed"));
		GetUserSeason = Result;
		bGetUserSeasonInfo = true;
	}), SeasonPassOnError);
	WaitUntil(bGetUserSeasonInfo, "Waiting to get user season data info...");
	AB_TEST_TRUE(bGetUserSeasonInfo);
	AB_TEST_EQUAL(GetUserSeason.SeasonId, SeasonResponse.Id);
	AB_TEST_EQUAL(0, GetUserSeason.ToClaimRewards.Num()); // must be 0 after bulk claim

	bGetWalletInfoSuccess = false;
	FRegistry::Wallet.GetWalletInfoByCurrencyCode(CurrencyCode, THandler<FAccelByteModelsWalletInfo>::CreateLambda(
	[&bGetWalletInfoSuccess, &LastBalanceCount](
	const FAccelByteModelsWalletInfo& Result)
	{
		UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Wallet Balance : %d"), Result.Balance);
		LastBalanceCount = Result.Balance;
		bGetWalletInfoSuccess = true;
	}), SeasonPassOnError);
	WaitUntil(bGetWalletInfoSuccess, "Waiting to get wallet info");
	AB_TEST_EQUAL(LastBalanceCount, ExpectedCurrencyBalance);
	
	bGetEntitlementSuccess = false;
	FAccelByteModelsEntitlementInfo EntitlementInfo;
	// after claiming rewards, user should update inventory automatically (end user)
	FRegistry::Entitlement.QueryUserEntitlements("", ItemRewardResponse.ItemId, 0, 20, THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda(
	[&](const FAccelByteModelsEntitlementPagingSlicedResult&Result)
	{
		bGetEntitlementSuccess = true;
		for (const FAccelByteModelsEntitlementInfo& Ent : Result.Data)
		{
			if (Ent.ItemId.Equals(ItemRewardResponse.ItemId))
			{
				UE_LOG(LogAccelByteSeasonPassTest, Log, TEXT("Claimed Reward in the entitlement!"));
				EntitlementInfo = Ent;
				break;
			}
		}
	}), SeasonPassOnError, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);
	WaitUntil(bGetEntitlementSuccess, "Waiting to get entitlement");
	AB_TEST_EQUAL(EntitlementInfo.UseCount, ExpectedItemUseCount);
	
	return true;
}
