// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteOrderApi.h"
#include "Api/AccelByteCategoryApi.h"
#include "Api/AccelByteItemApi.h"
#include "Api/AccelByteWalletApi.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Api/AccelByteFulfillmentApi.h"
#include "Api/AccelByteCurrencyApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerEcommerceApi.h"
#include "Core/AccelByteRegistry.h"
#include "TestUtilities.h"
#include "EcommerceTestAdmin.h"
#include "UserTestAdmin.h"
#include "Api/AccelByteRewardApi.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::Category;
using AccelByte::Api::Item;
using AccelByte::Api::Order;
using AccelByte::Api::Wallet;
using AccelByte::Api::Entitlement;
using AccelByte::Api::Currency;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteEcommerceTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteEcommerceTest);

const int32 AutomationFlagMaskEcommerce = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto EcommerceTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
	{
		UE_LOG(LogAccelByteEcommerceTest, Error, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
	});

FString EcommerceTestSDKCurrencyCode = TEXT("SDKC");
TMap<FString, FString> EcommerceTestLocalizationDescription;
FCurrencyCreateRequest EcommerceTestCurrencyRequest
{
	EcommerceTestSDKCurrencyCode,
	EcommerceTestLocalizationDescription,
	EcommerceTestSDKCurrencyCode,
	EAccelByteItemCurrencyType::VIRTUAL,
	0,
	-1,
	-1,
	-1
};
FStoreCreateRequest EcommerceTestArchiveStore
{
	"UE4-Store-Archive",
	"keep the original store",
	{"en"},
	{"US"},
	"en",
	"US"
};
FStoreCreateRequest EcommerceTestTemporaryStore
{
	"UE4-Store-Temporary",
	"for SDK testing purpose",
	{"en"},
	{"US"},
	"en",
	"US"
};
FItemFullInfo EcommerceTestEmptyItem;
FCampaignInfo EcommerceTestEmptyCampaignInfo;
FCodeInfo EcommerceTestEmptyCodeInfo;
FRewardCreateInfo RewardCreateInfo;
EcommerceExpectedVariable EcommerceTestExpectedVariable{
	"/UE4RootCategory",
	"/UE4RootCategory/UE4ChildCategory",
	"/UE4RootCategory/UE4ChildCategory/UE4GrandChildCategory",
	"/UE4MediaCategory",
	EcommerceTestCurrencyRequest,
	"", // CampaignCode
	"UE4CampaignTest", // CampaignName
	"UE4ExpiredCampaignTest", // ExpiredCaimpaignName
	"UE4NotStartedCampaignTest", // NotStartedCampaignName
	"UE4RedeemableItem", // RedeemableItemTitle
	"UE4MediaItem", // MediaItemTitle
	"UE4RootItem",
	"UE4ChildItem",
	"UE4GrandChildItem",
	"UEPurchasingItem",
	EcommerceTestArchiveStore,
	EcommerceTestTemporaryStore,
	1000, // LootCoin Quantity
	EcommerceTestEmptyItem, // RedeemableItem Result
	EcommerceTestEmptyItem, // MediaItem Result
	EcommerceTestEmptyItem, // LootItem Result
	EcommerceTestEmptyItem, // LootCoin Result
	EcommerceTestEmptyItem, // Lootbox Result
	EcommerceTestEmptyItem, // Bundle Result
	EcommerceTestEmptyCampaignInfo, // Campaign Result
	EcommerceTestEmptyCampaignInfo, // ExpiredCampaign Result
	EcommerceTestEmptyCampaignInfo, // NotStartedCampaign Result
	EcommerceTestEmptyCodeInfo, // CodeInfo Result
	EcommerceTestEmptyCodeInfo, // ExpiredCodeInfo Result
	EcommerceTestEmptyCodeInfo, // NotStartedCodeInfo Result
	RewardCreateInfo // RewardId
};

FString EcommerceTestExpectedRootCategoryPath = EcommerceTestExpectedVariable.ExpectedRootCategoryPath;
FString EcommerceTestExpectedChildCategoryPath = EcommerceTestExpectedVariable.ExpectedChildCategoryPath;
FString EcommerceTestExpectedGrandChildCategoryPath = EcommerceTestExpectedVariable.ExpectedGrandChildCategoryPath;
FString EcommerceTestExpectedMediaCategoryPath = EcommerceTestExpectedVariable.ExpectedMediaCategoryPath;
FString EcommerceTestExpectedRootItemTitle = EcommerceTestExpectedVariable.ExpectedRootItemTitle; //"INGAMEITEM", 2 DOGECOIN, 0 Discount
FString EcommerceTestExpectedChildItemTitle = EcommerceTestExpectedVariable.ExpectedChildItemTitle;// Publisher's Currency, 0 USD, free, auto fulfilled, "COINS", "VIRTUAL", "DOGECOIN"
FString EcommerceTestExpectedMediaItemTitle = EcommerceTestExpectedVariable.mediaItemTitle;// Publisher's Currency, 0 USD, free, auto fulfilled, "COINS", "VIRTUAL", "DOGECOIN"
FString EcommerceTestExpectedCurrencyCode = EcommerceTestExpectedVariable.ExpectedCurrency.currencyCode;
FString EcommerceTestExpectedEntitlementId;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestSetup, "AccelByte.Tests.Ecommerce.A.Setup", AutomationFlagMaskEcommerce);
bool FEcommerceTestSetup::RunTest(const FString& Parameters)
{
	bool bSetupCurrencySuccess = false;
	bool bWaitingTerminated = false;
	SetupEcommerceCurrency(EcommerceTestExpectedVariable, 
		FSimpleDelegate::CreateLambda([&bSetupCurrencySuccess, &bWaitingTerminated]() {
			bSetupCurrencySuccess = true; 
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&bWaitingTerminated](int32 Code, const FString& Message) {
			EcommerceTestErrorHandler.ExecuteIfBound(Code, Message);
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting for Setup Ecommerce CURRENCY...");
	AB_TEST_TRUE(bSetupCurrencySuccess);

	bool bSetupStoreSuccess = false;
	bWaitingTerminated = false;
	SetupEcommerceStore(EcommerceTestExpectedVariable, 
		FSimpleDelegate::CreateLambda([&bSetupStoreSuccess, &bWaitingTerminated]() { 
			bSetupStoreSuccess = true; 
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&bWaitingTerminated](int32 Code, const FString& Message) {
			EcommerceTestErrorHandler.ExecuteIfBound(Code, Message);
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting for Setup Ecommerce STORE...");
	AB_TEST_TRUE(bSetupStoreSuccess);

	bool bSetupCampaignSuccess = false;
	bWaitingTerminated = false;
	SetupEcommerceCampaign(EcommerceTestExpectedVariable,
		FSimpleDelegate::CreateLambda([&bSetupCampaignSuccess, &bWaitingTerminated]() {
			bSetupCampaignSuccess = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&bWaitingTerminated](int32 Code, const FString& Message) {
			EcommerceTestErrorHandler.ExecuteIfBound(Code, Message);
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting for Setup Ecommerce CAMPAIGN...");
	AB_TEST_TRUE(bSetupCampaignSuccess);

	bool bSetupRewardSuccess = false;
	bWaitingTerminated = false;
	SetupEcommerceReward(EcommerceTestExpectedVariable,
		FSimpleDelegate::CreateLambda([&bSetupRewardSuccess, &bWaitingTerminated]()
		{
			bSetupRewardSuccess = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&bWaitingTerminated](int32 Code, const FString& Message)
		{
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting for Setup Reward...");
	AB_TEST_TRUE(bSetupRewardSuccess);

	bool bUserLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginSuccess]()
		{
			bUserLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bUserLoginSuccess, "Waiting for Login...");
	AB_TEST_TRUE(bUserLoginSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestTearDown, "AccelByte.Tests.Ecommerce.Z.Teardown", AutomationFlagMaskEcommerce);
bool FEcommerceTestTearDown::RunTest(const FString& Parameters)
{
	bool bTearDownSuccess = false;
	TearDownEcommerce(EcommerceTestExpectedVariable, FSimpleDelegate::CreateLambda([&]() { bTearDownSuccess = true; }), EcommerceTestErrorHandler);
	WaitUntil(bTearDownSuccess, "Waiting for teardown...");
	AB_TEST_TRUE(bTearDownSuccess)

#pragma region DeleteUser

		bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for user deletion...");

#pragma endregion DeleteUser

	return bDeleteSuccessful;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetCategory, "AccelByte.Tests.Ecommerce.B.GetCategory", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetCategory::RunTest(const FString& Parameters)
{
#pragma region GetCategory

	FString const CategoryPath = EcommerceTestExpectedChildCategoryPath;
	FString const Language = TEXT("en");
	bool bGetCategorySuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetCategory"));
	FRegistry::Category.GetCategory(CategoryPath, Language, THandler<FAccelByteModelsCategoryInfo>::CreateLambda([&](const FAccelByteModelsCategoryInfo& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetCategorySuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bGetCategorySuccess, "Waiting for get category...");

#pragma endregion GetCategory

	AB_TEST_TRUE(bGetCategorySuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetRootCategories, "AccelByte.Tests.Ecommerce.B.GetRootCategories", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetRootCategories::RunTest(const FString& Parameters)
{
#pragma region GetRootCategories

	FString const Language = TEXT("en");
	bool bGetRootCategoriesSuccess = false;
	bool bExpectedRootCategoryFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetRootCategories"));
	FRegistry::Category.GetRootCategories(Language, THandler<TArray<FAccelByteModelsCategoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsCategoryInfo>& Results)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			for (int i = 0; i < Results.Num(); i++)
			{
				if (Results[i].CategoryPath == EcommerceTestExpectedRootCategoryPath)
				{
					bExpectedRootCategoryFound = true;
				}
			}
			bGetRootCategoriesSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetRootCategoriesSuccess, "Waiting for get root category...");

#pragma endregion GetRootCategories

	AB_TEST_TRUE(bGetRootCategoriesSuccess);
	AB_TEST_TRUE(bExpectedRootCategoryFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetChildCategories, "AccelByte.Tests.Ecommerce.B.GetChildCategories", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetChildCategories::RunTest(const FString& Parameters)
{
#pragma region GetChildCategories

	FString const Language = TEXT("en");
	bool bGetChildCategoriesSuccess = false;
	bool bExpectedChildCategoryFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetChildCategory"));
	FRegistry::Category.GetChildCategories(Language, EcommerceTestExpectedRootCategoryPath, THandler<TArray<FAccelByteModelsCategoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsCategoryInfo>& Results)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			for (int i = 0; i < Results.Num(); i++)
			{
				if (Results[i].CategoryPath == EcommerceTestExpectedChildCategoryPath)
				{
					bExpectedChildCategoryFound = true;
				}
			}
			bGetChildCategoriesSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetChildCategoriesSuccess, "Waiting for get child category...");

#pragma endregion GetChildCategories

	AB_TEST_TRUE(bGetChildCategoriesSuccess);
	AB_TEST_TRUE(bExpectedChildCategoryFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetDescendantCategories, "AccelByte.Tests.Ecommerce.B.GetDescendantCategories", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetDescendantCategories::RunTest(const FString& Parameters)
{
#pragma region GetDescendantCategories
	FString const Language = TEXT("en");
	bool bGetDescendantCategoriesSuccess = false;
	bool bExpectedDescendantCategoryFound1 = false;
	bool bExpectedDescendantCategoryFound2 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetDescendantCategories"));
	FRegistry::Category.GetDescendantCategories(Language, EcommerceTestExpectedRootCategoryPath, THandler<TArray<FAccelByteModelsCategoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsCategoryInfo>& Results)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			for (int i = 0; i < Results.Num(); i++)
			{
				if (Results[i].CategoryPath == EcommerceTestExpectedChildCategoryPath)
				{
					bExpectedDescendantCategoryFound1 = true;
				}
				else if (Results[i].CategoryPath == EcommerceTestExpectedGrandChildCategoryPath)
				{
					bExpectedDescendantCategoryFound2 = true;
				}
			}
			bGetDescendantCategoriesSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetDescendantCategoriesSuccess, "Waiting for get descendant category...");

#pragma endregion GetDescendantCategories

	AB_TEST_TRUE(bGetDescendantCategoriesSuccess);
	AB_TEST_TRUE(bExpectedDescendantCategoryFound1);
	AB_TEST_TRUE(bExpectedDescendantCategoryFound2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetItemsByCriteria, "AccelByte.Tests.Ecommerce.C.GetItemsByCriteria", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetItemsByCriteria::RunTest(const FString& Parameters)
{
#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::INGAMEITEM;
	ItemCriteria.CategoryPath = EcommerceTestExpectedRootCategoryPath;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedRootItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			for (int i = 0; i < Result.Data.Num(); i++)
			{
				if (Result.Data[i].Title == EcommerceTestExpectedRootItemTitle)
				{
					bExpectedRootItemFound = true;
				}
			}
			bGetItemByCriteriaSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get items...");

#pragma endregion GetItemByCriteria

	AB_TEST_TRUE(bGetItemByCriteriaSuccess);
	AB_TEST_TRUE(bExpectedRootItemFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetItemById, "AccelByte.Tests.Ecommerce.C.GetItem", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetItemById::RunTest(const FString& Parameters)
{
	FString ItemId = TEXT("");
	const FString Language = TEXT("en");
	const FString Region = TEXT("US");

#pragma region GetItemByCriteria
	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = Language;
	ItemCriteria.Region = Region;
	ItemCriteria.ItemType = EAccelByteItemType::INGAMEITEM;
	ItemCriteria.CategoryPath = EcommerceTestExpectedRootCategoryPath;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedRootItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			for (int i = 0; i < Result.Data.Num(); i++)
			{
				if (Result.Data[i].Title == EcommerceTestExpectedRootItemTitle)
				{
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found expected item"));
					ItemId = Result.Data[i].ItemId;
					bExpectedRootItemFound = true;
				}
			}
			bGetItemByCriteriaSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get items...");

#pragma endregion GetItemByCriteria

#pragma region GetItem

	bool bGetItemByIdSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemById"));
	FRegistry::Item.GetItemById(ItemId, Language, Region, THandler<FAccelByteModelsPopulatedItemInfo>::CreateLambda([&](const FAccelByteModelsPopulatedItemInfo& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetItemByIdSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByIdSuccess, "Waiting for get items...");

#pragma endregion GetItem

	AB_TEST_TRUE(bGetItemByCriteriaSuccess);
	AB_TEST_TRUE(bExpectedRootItemFound);
	AB_TEST_TRUE(bGetItemByIdSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetItemsByCriteriaDiscounted, "AccelByte.Tests.Ecommerce.C.GetItemsByCriteriaDiscounted", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetItemsByCriteriaDiscounted::RunTest(const FString& Parameters)
{
	FString ItemId{};

#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::INGAMEITEM;
	ItemCriteria.CategoryPath = EcommerceTestExpectedRootCategoryPath;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedRootItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			for (int i = 0; i < Result.Data.Num(); i++)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Discount expired time: %s"), *Result.Data[i].RegionData[0].DiscountExpireAt.ToIso8601());
				if (Result.Data[i].RegionData[0].DiscountExpireAt > FDateTime::Now())
				{
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        The item is on discount"));
					ItemId = Result.Data[i].ItemId;
					bExpectedRootItemFound = true;
				}
			}
			bGetItemByCriteriaSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get items...");

#pragma endregion GetItemByCriteria

	AB_TEST_TRUE(bGetItemByCriteriaSuccess);
	AB_TEST_TRUE(bExpectedRootItemFound);
	AB_TEST_NOT_EQUAL(ItemId, FString{});
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestSearchItem, "AccelByte.DisabledTests.Ecommerce.C.SearchItem", AutomationFlagMaskEcommerce);
bool FEcommerceTestSearchItem::RunTest(const FString& Parameters)
{
#pragma region SearchItem

	FString const Language = TEXT("en");
	FString const Region = TEXT("US");
	bool bSearchItemSuccess = false;
	bool bSearchedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("SearchItem"));
	FRegistry::Item.SearchItem(Language, EcommerceTestExpectedChildItemTitle, 0, 20, Region, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			for (int i = 0; i < Result.Data.Num(); i++)
			{
				if (Result.Data[i].Title == EcommerceTestExpectedChildItemTitle)
				{
					bSearchedItemFound = true;
				}
			}
			bSearchItemSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bSearchItemSuccess, "Waiting for search item...");

#pragma endregion SearchItem

	AB_TEST_TRUE(bSearchItemSuccess);
	AB_TEST_TRUE(bSearchedItemFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestCreateNewOrder, "AccelByte.Tests.Ecommerce.D1.CreateNewOrder", AutomationFlagMaskEcommerce);
bool FEcommerceTestCreateNewOrder::RunTest(const FString& Parameters)
{
	FAccelByteModelsItemInfo Item;

#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::COINS;
	ItemCriteria.CategoryPath = EcommerceTestExpectedChildCategoryPath;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria_VirtualCurrency"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    ChildFound: %d"), Result.Data.Num());
			for (int i = 0; i < Result.Data.Num(); i++)
			{
				if (EcommerceTestExpectedChildItemTitle.Contains(Result.Data[i].Title))
				{
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found"));
					Item = Result.Data[i];
					bExpectedItemFound = true;
					bGetItemByCriteriaSuccess = true;
				}
			}
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get item...");

#pragma endregion GetItemByCriteria

#pragma region CreateOrder

	const int32 Quantity = 1;
	FAccelByteModelsOrderCreate OrderCreate;
	OrderCreate.CurrencyCode = Item.RegionData[0].CurrencyCode;
	OrderCreate.DiscountedPrice = Item.RegionData[0].DiscountedPrice * Quantity;
	OrderCreate.Price = Item.RegionData[0].Price * Quantity;
	OrderCreate.Quantity = 1;
	OrderCreate.ReturnUrl = TEXT("https://sdk.example.com");
	OrderCreate.ItemId = Item.ItemId;
	OrderCreate.Region = "US";
	OrderCreate.Language = "en";

	bool bCreateNewOrderSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("CreateNewOrder"));
	FRegistry::Order.CreateNewOrder(OrderCreate, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](FAccelByteModelsOrderInfo Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bCreateNewOrderSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bCreateNewOrderSuccess, "Waiting for new order created...");

#pragma endregion CreateOrder

#pragma region GetItemByCriteria_InGameItem

	ItemCriteria.CategoryPath = EcommerceTestExpectedRootCategoryPath;
	ItemCriteria.ItemType = EAccelByteItemType::INGAMEITEM;
	bool bGetItemByCriteriaSuccess2 = false;
	bool bExpectedItemFound2 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria_InGameItem"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    ChildFound: %d"), Result.Data.Num());
			for (int i = 0; i < Result.Data.Num(); i++)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				if (Result.Data[i].Title == EcommerceTestExpectedRootItemTitle)
				{
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found"));
					Item = Result.Data[i];
					bExpectedItemFound2 = true;
					bGetItemByCriteriaSuccess2 = true;
				}
			}
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess2, "Waiting for get item...");

#pragma endregion GetItemByCriteria_InGameItem

#pragma region CreateOrder_InGameItem

	OrderCreate.CurrencyCode = Item.RegionData[0].CurrencyCode;
	OrderCreate.DiscountedPrice = Item.RegionData[0].DiscountedPrice * Quantity;
	OrderCreate.Price = Item.RegionData[0].Price * Quantity;
	OrderCreate.Quantity = Quantity;
	OrderCreate.ReturnUrl = TEXT("https://sdk.example.com");
	OrderCreate.ItemId = Item.ItemId;
	OrderCreate.Region = "US";
	OrderCreate.Language = "en";

	bool bCreateNewOrderSuccess2 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("CreateNewOrder2"));
	FRegistry::Order.CreateNewOrder(OrderCreate, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](FAccelByteModelsOrderInfo Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bCreateNewOrderSuccess2 = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bCreateNewOrderSuccess2, "Waiting for new order created...");

#pragma endregion CreateOrder_InGameItem

#pragma region GetItemByCriteria_MediaItem

	ItemCriteria.CategoryPath = EcommerceTestExpectedMediaCategoryPath;
	ItemCriteria.ItemType = EAccelByteItemType::MEDIA;
	bool bGetItemByCriteriaSuccess3 = false;
	bool bExpectedItemFound3 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria_MediaItem"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    ChildFound: %d"), Result.Data.Num());
			for (int i = 0; i < Result.Data.Num(); i++)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				if (Result.Data[i].Title == EcommerceTestExpectedMediaItemTitle)
				{
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found"));
					Item = Result.Data[i];
					bExpectedItemFound3 = true;
					bGetItemByCriteriaSuccess3 = true;
				}
			}
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess3, "Waiting for get item...");

#pragma endregion GetItemByCriteria_MediaItem

#pragma region CreateOrder_MediaItem

	OrderCreate.CurrencyCode = Item.RegionData[0].CurrencyCode;
	OrderCreate.DiscountedPrice = Item.RegionData[0].DiscountedPrice * Quantity;
	OrderCreate.Price = Item.RegionData[0].Price * Quantity;
	OrderCreate.Quantity = Quantity;
	OrderCreate.ReturnUrl = TEXT("https://sdk.example.com");
	OrderCreate.ItemId = Item.ItemId;
	OrderCreate.Region = "US";
	OrderCreate.Language = "en";

	bool bCreateNewOrderSuccess3 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("CreateNewOrder3"));
	FRegistry::Order.CreateNewOrder(OrderCreate, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](FAccelByteModelsOrderInfo Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bCreateNewOrderSuccess3 = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bCreateNewOrderSuccess3, "Waiting for new order created...");

	bool bGetEntitlementByItemId = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserEntitlementOwnershipByItemId for Media Item"));
	FAccelByteModelsEntitlementOwnership OwnershipResult;
	FRegistry::Entitlement.GetUserEntitlementOwnershipByItemId(Item.ItemId,
		THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda([&](FAccelByteModelsEntitlementOwnership Result)
			{
				OwnershipResult = Result;
				bGetEntitlementByItemId = true;
			}), EcommerceTestErrorHandler);
	WaitUntil(bGetEntitlementByItemId, "Waiting for Query User's Entitlement...");

	bool MediaItemEntitlementSuccess = false;
	if (OwnershipResult.Owned) {
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		MediaItemEntitlementSuccess = true;
	}

#pragma endregion CreateOrder_MediaItem

#pragma region CreateOrder_EmptyReturnURL

	OrderCreate.CurrencyCode = Item.RegionData[0].CurrencyCode;
	OrderCreate.DiscountedPrice = Item.RegionData[0].DiscountedPrice * Quantity;
	OrderCreate.Price = Item.RegionData[0].Price * Quantity;
	OrderCreate.Quantity = 1;
	OrderCreate.ReturnUrl = TEXT("");
	OrderCreate.ItemId = Item.ItemId;
	OrderCreate.Region = "US";
	OrderCreate.Language = "en";

	bool bCreateNewOrderSuccess4 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("CreateNewOrder"));
	FRegistry::Order.CreateNewOrder(OrderCreate, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](FAccelByteModelsOrderInfo Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bCreateNewOrderSuccess4 = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bCreateNewOrderSuccess4, "Waiting for new order created...");

#pragma endregion CreateOrder

	AB_TEST_TRUE(bGetItemByCriteriaSuccess);
	AB_TEST_TRUE(bExpectedItemFound);
	AB_TEST_TRUE(bCreateNewOrderSuccess);
	AB_TEST_TRUE(bGetItemByCriteriaSuccess2);
	AB_TEST_TRUE(bExpectedItemFound2);
	AB_TEST_TRUE(bCreateNewOrderSuccess2);
	AB_TEST_TRUE(bExpectedItemFound3);
	AB_TEST_TRUE(bCreateNewOrderSuccess3);
	AB_TEST_TRUE(MediaItemEntitlementSuccess);
	AB_TEST_TRUE(bCreateNewOrderSuccess4);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetUserOrder, "AccelByte.Tests.Ecommerce.D2.GetUserOrder", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetUserOrder::RunTest(const FString& Parameters)
{
	FAccelByteModelsItemInfo Item;
	FString OrderNo = TEXT("");

#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::COINS;
	ItemCriteria.CategoryPath = EcommerceTestExpectedChildCategoryPath;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    ChildFound: %d"), Result.Data.Num());
			for (int i = 0; i < Result.Data.Num(); i++)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				if (EcommerceTestExpectedChildItemTitle.Contains(Result.Data[i].Title))
				{
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found"));
					Item = Result.Data[i];
					bExpectedItemFound = true;
					bGetItemByCriteriaSuccess = true;
				}
			}
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get item...");

#pragma endregion GetItemByCriteria

#pragma region CreateOrder

	const int32 Quantity = 1;
	FAccelByteModelsOrderCreate OrderCreate;
	OrderCreate.CurrencyCode = Item.RegionData[0].CurrencyCode;
	OrderCreate.DiscountedPrice = Item.RegionData[0].DiscountedPrice * Quantity;
	OrderCreate.Price = Item.RegionData[0].Price * Quantity;
	OrderCreate.Quantity = 1;
	OrderCreate.ReturnUrl = TEXT("https://sdk.example.com");
	OrderCreate.ItemId = Item.ItemId;
	OrderCreate.Region = "US";
	OrderCreate.Language = "en";

	bool bCreateNewOrderSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("CreateNewOrder"));
	FRegistry::Order.CreateNewOrder(OrderCreate, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](const FAccelByteModelsOrderInfo& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			OrderNo = Result.OrderNo;
			bCreateNewOrderSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bCreateNewOrderSuccess, "Waiting for new order created...");

#pragma endregion CreateOrder

#pragma region GetUserOrder

	bool bGetUserOrderSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserOrder"));
	FRegistry::Order.GetUserOrder(OrderNo, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](const FAccelByteModelsOrderInfo& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetUserOrderSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetUserOrderSuccess, "Waiting for get user order...");

#pragma endregion GetUserOrder

	AB_TEST_TRUE(bGetItemByCriteriaSuccess);
	AB_TEST_TRUE(bExpectedItemFound);
	AB_TEST_TRUE(bCreateNewOrderSuccess);
	AB_TEST_TRUE(bGetUserOrderSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetUserOrderHistory, "AccelByte.Tests.Ecommerce.D3.GetUserOrderHistory", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetUserOrderHistory::RunTest(const FString& Parameters)
{
	FAccelByteModelsItemInfo Item;
	FString OrderNo = TEXT("");

#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::COINS;
	ItemCriteria.CategoryPath = EcommerceTestExpectedChildCategoryPath;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    ChildFound: %d"), Result.Data.Num());
			for (int i = 0; i < Result.Data.Num(); i++)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				if (EcommerceTestExpectedChildItemTitle.Contains(Result.Data[i].Title))
				{
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found"));
					Item = Result.Data[i];
					bExpectedItemFound = true;
					bGetItemByCriteriaSuccess = true;
				}
			}
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get item...");

#pragma endregion GetItemByCriteria

#pragma region CreateOrder

	const int32 Quantity = 1;
	FAccelByteModelsOrderCreate OrderCreate;
	OrderCreate.CurrencyCode = Item.RegionData[0].CurrencyCode;
	OrderCreate.DiscountedPrice = Item.RegionData[0].DiscountedPrice * Quantity;
	OrderCreate.Price = Item.RegionData[0].Price * Quantity;
	OrderCreate.Quantity = 1;
	OrderCreate.ReturnUrl = TEXT("https://sdk.example.com");
	OrderCreate.ItemId = Item.ItemId;
	OrderCreate.Region = "US";
	OrderCreate.Language = "en";

	bool bCreateNewOrderSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("CreateNewOrder"));
	FRegistry::Order.CreateNewOrder(OrderCreate, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](FAccelByteModelsOrderInfo Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			OrderNo = Result.OrderNo;
			bCreateNewOrderSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bCreateNewOrderSuccess, "Waiting for new order created...");

#pragma endregion CreateOrder

#pragma region GetUserOrderHistory

	bool bGetUserOrderHistorySuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserOrder"));
	FRegistry::Order.GetUserOrderHistory(OrderNo, THandler<TArray<FAccelByteModelsOrderHistoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsOrderHistoryInfo>& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetUserOrderHistorySuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetUserOrderHistorySuccess, "Waiting for get user order history...");

#pragma endregion GetUserOrderHistory

	AB_TEST_TRUE(bGetItemByCriteriaSuccess);
	AB_TEST_TRUE(bExpectedItemFound);
	AB_TEST_TRUE(bCreateNewOrderSuccess);
	AB_TEST_TRUE(bGetUserOrderHistorySuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetUserOrders, "AccelByte.Tests.Ecommerce.D4.GetUserOrders", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetUserOrders::RunTest(const FString& Parameters)
{
#pragma region GetUserOrders

	bool bGetUserOrdersSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserOrders"));
	FRegistry::Order.GetUserOrders(0, 20, THandler<FAccelByteModelsPagedOrderInfo>::CreateLambda([&](const FAccelByteModelsPagedOrderInfo& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetUserOrdersSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetUserOrdersSuccess, "Waiting for get user order...");

#pragma endregion GetUserOrders

	AB_TEST_TRUE(bGetUserOrdersSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestCancelOrder, "AccelByte.Tests.Ecommerce.D5.CancelOrder", AutomationFlagMaskEcommerce);
bool FEcommerceTestCancelOrder::RunTest(const FString& Parameters)
{
#pragma region LoginWithDevice

	bool bUserLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
		{
			bUserLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bUserLoginSuccess, "Waiting for login with device id...");
	AB_TEST_TRUE(bUserLoginSuccess);

#pragma endregion LoginWithDevice

#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");

	bool bGetItemByCriteriaSuccess = false;
	FAccelByteModelsItemInfo Item;
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			for (int i = 0; i < Result.Data.Num(); i++)
			{
				if (Result.Data[i].RegionData[0].CurrencyType == EAccelByteItemCurrencyType::REAL)
				{
					if (Result.Data[i].RegionData[0].Price > 0)
					{
						Item = Result.Data[i];
						bGetItemByCriteriaSuccess = true;
						break;
					}
				}
				bGetItemByCriteriaSuccess = false;
			}
		}), EcommerceTestErrorHandler);
	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get item by criteria...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess);

#pragma endregion GetItemByCriteria

#pragma region CreateNewOrder

	int32 Quantity = 1;
	FAccelByteModelsOrderCreate OrderCreate;
	OrderCreate.CurrencyCode = Item.RegionData[0].CurrencyCode;
	OrderCreate.Region = Item.Region;
	OrderCreate.Quantity = Quantity;
	OrderCreate.ItemId = Item.ItemId;
	OrderCreate.Language = Item.Language;
	OrderCreate.ReturnUrl = TEXT("https://sdk.example.com");
	OrderCreate.Price = Item.RegionData[0].Price * Quantity;
	OrderCreate.DiscountedPrice = Item.RegionData[0].DiscountedPrice * Quantity;
	bool bCreateNewOrderSuccess = false;
	FAccelByteModelsOrderInfo Order;
	FRegistry::Order.CreateNewOrder(OrderCreate, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](const FAccelByteModelsOrderInfo& Result)
		{
			Order = Result;
			bCreateNewOrderSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bCreateNewOrderSuccess, "Waiting create new order...");
	AB_TEST_TRUE(bCreateNewOrderSuccess);

#pragma endregion CreateNewOrder

#pragma region CancelOrder

	bool bCancelOrder = false;
	FRegistry::Order.CancelOrder(Order.OrderNo, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](const FAccelByteModelsOrderInfo& Result)
		{
			bCancelOrder = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bCancelOrder, "Waiting cancel order...");
	AB_TEST_TRUE(bCancelOrder);

#pragma endregion CancelOrder

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetWalletInfoByCurrencyCode, "AccelByte.Tests.Ecommerce.E1.GetWalletInfoByCurrencyCode", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetWalletInfoByCurrencyCode::RunTest(const FString& Parameters)
{
	bool bGetWalletSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetWalletInfoByCurrencyCode"));
	FRegistry::Wallet.GetWalletInfoByCurrencyCode(EcommerceTestExpectedCurrencyCode, THandler<FAccelByteModelsWalletInfo>::CreateLambda([&](const FAccelByteModelsWalletInfo& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetWalletSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetWalletSuccess, "Waiting for get wallet...");

	AB_TEST_TRUE(bGetWalletSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestCreditUserWallet, "AccelByte.Tests.Ecommerce.E2.CreditUserWallet", AutomationFlagMaskEcommerce);
bool FEcommerceTestCreditUserWallet::RunTest(const FString& Parameters)
{
#pragma region GetWalletInfo

	bool bGetWalletSuccess = false;
	FAccelByteModelsWalletInfo WalletInfo;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetWalletInfoByCurrencyCode"));
	FRegistry::Wallet.GetWalletInfoByCurrencyCode(EcommerceTestExpectedCurrencyCode, THandler<FAccelByteModelsWalletInfo>::CreateLambda([&WalletInfo, &bGetWalletSuccess](const FAccelByteModelsWalletInfo& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			WalletInfo = Result;
			bGetWalletSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetWalletSuccess, "Waiting for get wallet...");

#pragma endregion GetWalletInfo

#pragma region ClientLogin

	bool bServerLoginWithClientCredentialsDone = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(
		FVoidHandler::CreateLambda([&bServerLoginWithClientCredentialsDone]()
			{
				bServerLoginWithClientCredentialsDone = true;
			}), EcommerceTestErrorHandler);
	WaitUntil(bServerLoginWithClientCredentialsDone, "Server Login With Client Credentials");

#pragma endregion ClientLogin

#pragma region CreditUserWallet

	bool bCreditWalletSuccess = false;
	bool bBalanceIncrease = false;
	FAccelByteModelsCreditUserWalletRequest request;
	request.Amount = 1000;
	request.Source = EAccelByteCreditUserWalletSource::PURCHASE;
	request.Reason = TEXT("GameServer Ecommerce CreditUserWallet test.");
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("CreditUserWallet"));
	FRegistry::ServerEcommerce.CreditUserWallet(FRegistry::Credentials.GetUserId(), EcommerceTestSDKCurrencyCode, request, THandler<FAccelByteModelsWalletInfo>::CreateLambda([&WalletInfo, &bBalanceIncrease, &request, &bCreditWalletSuccess](const FAccelByteModelsWalletInfo& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bBalanceIncrease = ((WalletInfo.Balance + request.Amount) == Result.Balance);
			bCreditWalletSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bCreditWalletSuccess, "Waiting for get wallet...");

#pragma endregion CreditUserWallet

	AB_TEST_TRUE(bGetWalletSuccess);
	AB_TEST_TRUE(bCreditWalletSuccess);
	AB_TEST_TRUE(bBalanceIncrease);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestDebitUserWallet, "AccelByte.Tests.Ecommerce.E3.DebitUserWallet", AutomationFlagMaskEcommerce)
bool FEcommerceTestDebitUserWallet::RunTest(const FString& Parameters)
{
#pragma region GetWalletInfo

	bool bGetWalletSuccess = false;
	FAccelByteModelsWalletInfo WalletInfo;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetWalletInfoByCurrencyCode"));
	FRegistry::Wallet.GetWalletInfoByCurrencyCode(EcommerceTestExpectedCurrencyCode, THandler<FAccelByteModelsWalletInfo>::CreateLambda([&WalletInfo, &bGetWalletSuccess](const FAccelByteModelsWalletInfo& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			WalletInfo = Result;
			bGetWalletSuccess = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetWalletSuccess, "Waiting for get wallet...");
	AB_TEST_TRUE(bGetWalletSuccess)
#pragma endregion GetWalletInfo

#pragma region ServerLogin

		bool bServerLoginWithClientCredentialsDone = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(
		FVoidHandler::CreateLambda([&bServerLoginWithClientCredentialsDone]()
			{
				bServerLoginWithClientCredentialsDone = true;
			}), EcommerceTestErrorHandler);
	WaitUntil(bServerLoginWithClientCredentialsDone, "Server Login With Client Credentials");

	AB_TEST_TRUE(bServerLoginWithClientCredentialsDone)
#pragma endregion ServerLogin

#pragma region CreditUserWallet

		bool bCreditWalletSuccess = false;
	bool bBalanceIncrease = false;
	FAccelByteModelsCreditUserWalletRequest request;
	request.Amount = 1000;
	request.Source = EAccelByteCreditUserWalletSource::PURCHASE;
	request.Reason = TEXT("GameServer Ecommerce CreditUserWallet test.");
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("CreditUserWallet"));
	FRegistry::ServerEcommerce.CreditUserWallet(FRegistry::Credentials.GetUserId(), EcommerceTestSDKCurrencyCode, request, THandler<FAccelByteModelsWalletInfo>::CreateLambda([&WalletInfo, &bBalanceIncrease, &request, &bCreditWalletSuccess](const FAccelByteModelsWalletInfo& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bBalanceIncrease = ((WalletInfo.Balance + request.Amount) == Result.Balance);
			bCreditWalletSuccess = true;
			WalletInfo = Result;
		}), EcommerceTestErrorHandler);
	WaitUntil(bCreditWalletSuccess, "Waiting for get wallet...");
	AB_TEST_TRUE(bCreditWalletSuccess)
#pragma endregion CreditUserWallet

#pragma region DebitUserWallet

		bool bDebitWalletSuccess = false;
	bool bWalletDecreased = false;
	FAccelByteModelsDebitUserWalletRequest DebitRequest;
	DebitRequest.Amount = 50;
	DebitRequest.Reason = TEXT("Integration Test");
	FRegistry::ServerEcommerce.DebitUserWallet(FRegistry::Credentials.GetUserId(), WalletInfo.Id,
		DebitRequest, THandler<FAccelByteModelsWalletInfo>::CreateLambda([&bDebitWalletSuccess, &WalletInfo, &DebitRequest, &bWalletDecreased](const FAccelByteModelsWalletInfo& Result)
			{
				bWalletDecreased = ((WalletInfo.Balance - DebitRequest.Amount) == Result.Balance);
				bDebitWalletSuccess = true;
			}), EcommerceTestErrorHandler);
	WaitUntil(bDebitWalletSuccess, "Waiting for debiting the user wallet");

	AB_TEST_TRUE(bWalletDecreased)
#pragma endregion DebitUserWallet

		return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGrantUserEntitlements, "AccelByte.Tests.Ecommerce.F1.GrantUserEntitlements", AutomationFlagMaskEcommerce);
bool FEcommerceTestGrantUserEntitlements::RunTest(const FString& Parameters)
{
#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::INGAMEITEM;
	bool bGetItemByCriteriaSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FAccelByteModelsItemPagingSlicedResult Items;
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetItemByCriteriaSuccess = true;
			Items = Result;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get items...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess);
#pragma endregion GetItemByCriteria

#pragma region GrantEntitlement
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bClientLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bClientLoginSuccess, "Waiting for Client Login...");

	FAccelByteModelsEntitlementGrant GrantedEntitlement;
	GrantedEntitlement.ItemId = Items.Data[0].ItemId;
	GrantedEntitlement.ItemNamespace = Items.Data[0].Namespace;
	GrantedEntitlement.GrantedCode = "123456789";
	GrantedEntitlement.Quantity = 1;
	GrantedEntitlement.Source = EAccelByteEntitlementSource::ACHIEVEMENT;
	GrantedEntitlement.Region = Items.Data[0].Region;
	GrantedEntitlement.Language = Items.Data[0].Language;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GrantingUserEntitlement"));
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("UserId: %s"), *FRegistry::Credentials.GetUserId());

	bool bGrantEntitlementSuccess = false;
	TArray<FAccelByteModelsStackableEntitlementInfo> GrantedEntitlementResult;
	FRegistry::ServerEcommerce.GrantUserEntitlements(FRegistry::Credentials.GetUserId(), { GrantedEntitlement }, THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>::CreateLambda([&bGrantEntitlementSuccess, &GrantedEntitlementResult](const TArray<FAccelByteModelsStackableEntitlementInfo>& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			GrantedEntitlementResult = Result;
			bGrantEntitlementSuccess = true;
			for (auto Entitlement : Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Entitlement: %s"), *Entitlement.Name);
			}
		}), EcommerceTestErrorHandler);
	WaitUntil(bGrantEntitlementSuccess, "Waiting for entitlement granted...");

	AB_TEST_TRUE(bGrantEntitlementSuccess);
#pragma endregion GrantEntitlement

#pragma region CheckEntitlementGranted
	bool bQueryEntitlementSuccess = false;
	FAccelByteModelsEntitlementPagingSlicedResult UserEntitlement;
	FRegistry::Entitlement.QueryUserEntitlements("", Items.Data[0].ItemId, 0, 20, THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda([&bQueryEntitlementSuccess, &UserEntitlement](const FAccelByteModelsEntitlementPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			UserEntitlement = Result;
			bQueryEntitlementSuccess = true;
		}), EcommerceTestErrorHandler, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);
	WaitUntil(bQueryEntitlementSuccess, "Waiting for Query User's Entitlement...");

	bool bEntitlementGranted = false;
	for (auto Entitlement : UserEntitlement.Data)
	{
		if (Entitlement.ItemId == GrantedEntitlementResult[0].ItemId && Entitlement.GrantedCode == GrantedEntitlementResult[0].GrantedCode)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Granted Entitlement Found!"));
			bEntitlementGranted = true;
			break;
		}
	}
	AB_TEST_TRUE(bEntitlementGranted);
#pragma endregion CheckEntitlementGranted
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGrantUserEntitlementsForMedia, "AccelByte.Tests.Ecommerce.F1.GrantUserEntitlementsForMedia", AutomationFlagMaskEcommerce);
bool FEcommerceTestGrantUserEntitlementsForMedia::RunTest(const FString& Parameters)
{
#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::MEDIA;
	bool bGetItemByCriteriaSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FAccelByteModelsItemPagingSlicedResult Items;
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetItemByCriteriaSuccess = true;
			Items = Result;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get items...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess);
#pragma endregion GetItemByCriteria

#pragma region GrantEntitlement
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bClientLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bClientLoginSuccess, "Waiting for Client Login...");

	FAccelByteModelsEntitlementGrant GrantedEntitlement;
	GrantedEntitlement.ItemId = Items.Data[0].ItemId;
	GrantedEntitlement.ItemNamespace = Items.Data[0].Namespace;
	GrantedEntitlement.GrantedCode = "123456789";
	GrantedEntitlement.Quantity = 1;
	GrantedEntitlement.Source = EAccelByteEntitlementSource::ACHIEVEMENT;
	GrantedEntitlement.Region = Items.Data[0].Region;
	GrantedEntitlement.Language = Items.Data[0].Language;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GrantingUserEntitlement"));
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("UserId: %s"), *FRegistry::Credentials.GetUserId());

	bool bGrantEntitlementSuccess = false;
	TArray<FAccelByteModelsStackableEntitlementInfo> GrantedEntitlementResult;
	FRegistry::ServerEcommerce.GrantUserEntitlements(FRegistry::Credentials.GetUserId(), { GrantedEntitlement }, THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>::CreateLambda([&bGrantEntitlementSuccess, &GrantedEntitlementResult](const TArray<FAccelByteModelsStackableEntitlementInfo>& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			GrantedEntitlementResult = Result;
			bGrantEntitlementSuccess = true;
			for (auto Entitlement : Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Entitlement: %s"), *Entitlement.Name);
			}
		}), EcommerceTestErrorHandler);
	WaitUntil(bGrantEntitlementSuccess, "Waiting for entitlement granted...");

	AB_TEST_TRUE(bGrantEntitlementSuccess);
#pragma endregion GrantEntitlement

#pragma region CheckEntitlementGranted

	bool bGetEntitlementByItemId = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserEntitlementOwnershipByItemId for Media Item"));
	FAccelByteModelsEntitlementOwnership OwnershipResult;
	FRegistry::Entitlement.GetUserEntitlementOwnershipByItemId(Items.Data[0].ItemId,
		THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda([&](FAccelByteModelsEntitlementOwnership Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				bGetEntitlementByItemId = true;
				OwnershipResult = Result;
			}), EcommerceTestErrorHandler);
	WaitUntil(bGetEntitlementByItemId, "Waiting for Query User's Entitlement...");

	bool MediaItemEntitlementSuccess = false;
	if (OwnershipResult.Owned) {
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		MediaItemEntitlementSuccess = true;
	}

	AB_TEST_TRUE(MediaItemEntitlementSuccess);

#pragma endregion CheckEntitlementGranted
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGrantUserEntitlementsInvalidItemId, "AccelByte.Tests.Ecommerce.F2.GrantUserEntitlementsInvalidItemId", AutomationFlagMaskEcommerce);
bool FEcommerceTestGrantUserEntitlementsInvalidItemId::RunTest(const FString& Parameters)
{

#pragma region GrantEntitlement
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bClientLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bClientLoginSuccess, "Waiting for Client Login...");

	FAccelByteModelsEntitlementGrant GrantedEntitlement;
	GrantedEntitlement.ItemId = "Invalid";
	GrantedEntitlement.ItemNamespace = FRegistry::Settings.Namespace;
	GrantedEntitlement.GrantedCode = "123456789";
	GrantedEntitlement.Quantity = 1;
	GrantedEntitlement.Source = EAccelByteEntitlementSource::ACHIEVEMENT;
	GrantedEntitlement.Region = "US";
	GrantedEntitlement.Language = "en";
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GrantingUserEntitlement"));
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("UserId: %s"), *FRegistry::Credentials.GetUserId());

	bool bGrantEntitlementSuccess = false;
	bool bGrantEntitlementDone = false;
	FRegistry::ServerEcommerce.GrantUserEntitlements(FRegistry::Credentials.GetUserId(), { GrantedEntitlement }, THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>::CreateLambda([&bGrantEntitlementSuccess, &bGrantEntitlementDone](const TArray<FAccelByteModelsStackableEntitlementInfo>& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGrantEntitlementSuccess = true;
			bGrantEntitlementDone = true;
			for (auto Entitlement : Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Entitlement: %s"), *Entitlement.Name);
			}
		}), FErrorHandler::CreateLambda([&bGrantEntitlementDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				if (static_cast<ErrorCodes>(ErrorCode) != ErrorCodes::ItemNotFoundException)
				{
					UE_LOG(LogAccelByteEcommerceTest, Error, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				}
				else
				{
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
					bGrantEntitlementDone = true;
				}
			}));
	WaitUntil(bGrantEntitlementDone, "Waiting for entitlement granted...");

	AB_TEST_TRUE(bGrantEntitlementDone);
	AB_TEST_FALSE(bGrantEntitlementSuccess);
#pragma endregion GrantEntitlement

#pragma region CheckEntitlementGranted
	bool bQueryEntitlementSuccess = false;
	FAccelByteModelsEntitlementPagingSlicedResult UserEntitlement;
	FRegistry::Entitlement.QueryUserEntitlements("", "Invalid", 0, 20, THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda([&bQueryEntitlementSuccess, &UserEntitlement](const FAccelByteModelsEntitlementPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			UserEntitlement = Result;
			bQueryEntitlementSuccess = true;
		}), EcommerceTestErrorHandler, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);
	WaitUntil(bQueryEntitlementSuccess, "Waiting for Query User's Entitlement...");

	bool bEntitlementGranted = false;
	for (auto Entitlement : UserEntitlement.Data)
	{
		if (Entitlement.ItemId == "Invalid" && Entitlement.GrantedCode == "123456789")
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Granted Entitlement Found!"));
			bEntitlementGranted = true;
			break;
		}
	}
	AB_TEST_FALSE(bEntitlementGranted);
#pragma endregion CheckEntitlementGranted
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestQueryUserEntitlements, "AccelByte.Tests.Ecommerce.F3.QueryUserEntitlements", AutomationFlagMaskEcommerce);
bool FEcommerceTestQueryUserEntitlements::RunTest(const FString& Parameters)
{
	bool bQueryEntitlementSuccess = false;
	bool bQueryResultTrue = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("QueryUserEntitlementSuccess"));
	FRegistry::Entitlement.QueryUserEntitlements("", "", 0, 20,
		THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsEntitlementPagingSlicedResult& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				bQueryEntitlementSuccess = true;
				bQueryResultTrue = (Result.Data.Num() > 0);
				EcommerceTestExpectedEntitlementId = Result.Data[0].Id;
			}), EcommerceTestErrorHandler, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);

	WaitUntil(bQueryEntitlementSuccess, "Waiting for get user entitlement...");

	AB_TEST_TRUE(bQueryEntitlementSuccess);
	AB_TEST_TRUE(bQueryResultTrue);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestConsumeUserEntitlement, "AccelByte.Tests.Ecommerce.F4.ConsumeUserEntitlement", AutomationFlagMaskEcommerce);
bool FEcommerceTestConsumeUserEntitlement::RunTest(const FString& Parameters)
{
#pragma region FirstConsumption

	bool bConsumeEntitlement1Success = false;
	bool bConsumeResult1True = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("FirstConsumeUserEntitlement"));
	FRegistry::Entitlement.ConsumeUserEntitlement(EcommerceTestExpectedEntitlementId, 1,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&bConsumeResult1True, &bConsumeEntitlement1Success](const FAccelByteModelsEntitlementInfo& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				bConsumeEntitlement1Success = true;
				bConsumeResult1True = (Result.Status == EAccelByteEntitlementStatus::CONSUMED);
			}), EcommerceTestErrorHandler);
	WaitUntil(bConsumeEntitlement1Success, "Waiting for consume user entitlement...");

#pragma endregion FirstConsumption

#pragma region SecondConsumption

	bool bConsumeEntitlement2Success = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("SecondConsumeUserEntitlement"));
	FRegistry::Entitlement.ConsumeUserEntitlement(EcommerceTestExpectedEntitlementId, 1,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&bConsumeEntitlement2Success](const FAccelByteModelsEntitlementInfo& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				bConsumeEntitlement2Success = true;
			}), FErrorHandler::CreateLambda([&bConsumeEntitlement2Success](int32 ErrorCode, const FString& ErrorMessage)
				{
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
					bConsumeEntitlement2Success = true;
				}));
	WaitUntil(bConsumeEntitlement2Success, "Waiting for consume user entitlement...");

#pragma endregion SecondConsumption

	AB_TEST_TRUE(bConsumeEntitlement1Success);
	AB_TEST_TRUE(bConsumeResult1True);
	AB_TEST_TRUE(bConsumeEntitlement2Success);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetUserEntitlementById, "AccelByte.Tests.Ecommerce.F5.GetUserEntitlementById", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetUserEntitlementById::RunTest(const FString& Parameters)
{
	bool bGetEntitlementSuccess = false;
	bool bGetResultTrue = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserEntitlementById"));
	FRegistry::Entitlement.GetUserEntitlementById(EcommerceTestExpectedEntitlementId,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&](const FAccelByteModelsEntitlementInfo& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				bGetEntitlementSuccess = true;
				bGetResultTrue = (Result.Id == EcommerceTestExpectedEntitlementId);
			}), EcommerceTestErrorHandler);

	WaitUntil(bGetEntitlementSuccess, "Waiting for get user entitlement...");

	AB_TEST_TRUE(bGetEntitlementSuccess);
	AB_TEST_TRUE(bGetResultTrue);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetUserEntitlementByIdInvalidId, "AccelByte.Tests.Ecommerce.F6.GetUserEntitlementByIdInvalidId", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetUserEntitlementByIdInvalidId::RunTest(const FString& Parameters)
{
	bool bGetEntitlementDone = false;
	bool bGetEntitlementError = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserEntitlementByIdInvalidId"));
	FRegistry::Entitlement.GetUserEntitlementById("440e447619a44658a95fd6607204c478",
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&](const FAccelByteModelsEntitlementInfo& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				bGetEntitlementDone = true;
			}), FErrorHandler::CreateLambda([&bGetEntitlementError, &bGetEntitlementDone](int32 ErrorCode, const FString& ErrorMessage)
				{
					bGetEntitlementDone = true;
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
					if (static_cast<ErrorCodes>(ErrorCode) == ErrorCodes::EntitlementIdNotFoundException)
					{
						bGetEntitlementError = true;
					}
				}));

	WaitUntil(bGetEntitlementDone, "Waiting for get user entitlement...");

	AB_TEST_TRUE(bGetEntitlementError);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestServerGetUserEntitlementById, "AccelByte.Tests.Ecommerce.F7.ServerGetUserEntitlementById", AutomationFlagMaskEcommerce);
bool FEcommerceTestServerGetUserEntitlementById::RunTest(const FString& Parameters)
{
	bool bLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bLoginSuccess, "Waiting for Login Creds...");

	bool bGetEntitlementSuccess = false;
	bool bGetResultTrue = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("ServerGetUserEntitlementById"));
	FRegistry::ServerEcommerce.GetUserEntitlementById(EcommerceTestExpectedEntitlementId,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&](const FAccelByteModelsEntitlementInfo& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				bGetEntitlementSuccess = true;
				bGetResultTrue = (Result.Id == EcommerceTestExpectedEntitlementId);
			}), EcommerceTestErrorHandler);
	WaitUntil(bGetEntitlementSuccess, "Waiting for get user entitlement...");

	AB_TEST_TRUE(bGetEntitlementSuccess);
	AB_TEST_TRUE(bGetResultTrue);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestServerGetUserEntitlementbyIdUserId, "AccelByte.Tests.Ecommerce.F7.ServerGetUserEntitlementbyIdUserId", AutomationFlagMaskEcommerce);
bool FEcommerceTestServerGetUserEntitlementbyIdUserId::RunTest(const FString& Parameters)
{
	bool bLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bLoginSuccess, "Waiting for Login Creds...");

	bool bGetEntitlementSuccess = false;
	bool bGetResultTrue = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("ServerGetUserEntitlementbyIdUserId"));
	FRegistry::ServerEcommerce.GetUserEntitlementById(FRegistry::Credentials.GetUserId(), EcommerceTestExpectedEntitlementId,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&](const FAccelByteModelsEntitlementInfo& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				bGetEntitlementSuccess = true;
				bGetResultTrue = (Result.Id == EcommerceTestExpectedEntitlementId);
			}), EcommerceTestErrorHandler);
	WaitUntil(bGetEntitlementSuccess, "Waiting for get user entitlement...");

	AB_TEST_TRUE(bGetEntitlementSuccess);
	AB_TEST_TRUE(bGetResultTrue);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestServerQueryUserEntitlements, "AccelByte.Tests.Ecommerce.F7.ServerQueryUserEntitlements", AutomationFlagMaskEcommerce);
bool FEcommerceTestServerQueryUserEntitlements::RunTest(const FString& Parameters)
{
	bool bLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bLoginSuccess, "Waiting for Login Creds...");

	bool bGetEntitlementSuccess = false;
	bool bGetResultTrue = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("ServerQueryUserEntitlements"));
	FRegistry::ServerEcommerce.QueryUserEntitlements(FRegistry::Credentials.GetUserId(), true, "",
		{ EcommerceTestExpectedEntitlementId }, 0, 20, THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda(
			[&bGetEntitlementSuccess, &bGetResultTrue](const FAccelByteModelsEntitlementPagingSlicedResult& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				bGetEntitlementSuccess = true;
				bGetResultTrue = (Result.Data[0].Id == EcommerceTestExpectedEntitlementId);
			}), EcommerceTestErrorHandler, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);
	WaitUntil(bGetEntitlementSuccess, "Waiting for get user entitlement...");

	AB_TEST_TRUE(bGetEntitlementSuccess);
	AB_TEST_TRUE(bGetResultTrue);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestServerGetUserEntitlementByIdInvalidId, "AccelByte.Tests.Ecommerce.F8.ServerGetUserEntitlementByIdInvalidId", AutomationFlagMaskEcommerce);
bool FEcommerceTestServerGetUserEntitlementByIdInvalidId::RunTest(const FString& Parameters)
{
	bool bLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bLoginSuccess, "Waiting for Login Creds...");

	bool bGetEntitlementDone = false;
	bool bGetEntitlementError = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserEntitlementSuccess"));
	FRegistry::ServerEcommerce.GetUserEntitlementById("340a5ae79bcf4871926d7874c6f2fa54",
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&](const FAccelByteModelsEntitlementInfo& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				bGetEntitlementDone = true;
			}), FErrorHandler::CreateLambda([&bGetEntitlementError, &bGetEntitlementDone](int32 ErrorCode, const FString& ErrorMessage)
				{
					bGetEntitlementDone = true;
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
					if (static_cast<ErrorCodes>(ErrorCode) == ErrorCodes::EntitlementIdNotFoundException)
					{
						bGetEntitlementError = true;
					}
				}));

	WaitUntil(bGetEntitlementDone, "Waiting for get user entitlement...");

	AB_TEST_TRUE(bGetEntitlementError);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGrantUserEntitlementsMany, "AccelByte.Tests.Ecommerce.F9.GrantUserEntitlementsMany", AutomationFlagMaskEcommerce);
bool FEcommerceTestGrantUserEntitlementsMany::RunTest(const FString& Parameters)
{
	bool bUserLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginSuccess]()
		{
			bUserLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bUserLoginSuccess, "Waiting for Login...");
	AB_TEST_TRUE(bUserLoginSuccess);

#pragma region GetItemByCriteria
	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::INGAMEITEM;
	bool bGetItemByCriteriaSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FAccelByteModelsItemPagingSlicedResult Items;
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetItemByCriteriaSuccess = true;
			Items = Result;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get items...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess);
#pragma endregion GetItemByCriteria

#pragma region GrantEntitlement
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bClientLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bClientLoginSuccess, "Waiting for Client Login...");

	TArray<FAccelByteModelsEntitlementGrant> GrantedEntitlements;
	TArray<FString> ItemIdsGranted;

	FAccelByteModelsEntitlementGrant GrantedEntitlement;
	GrantedEntitlement.ItemId = Items.Data[0].ItemId;
	GrantedEntitlement.ItemNamespace = Items.Data[0].Namespace;
	GrantedEntitlement.GrantedCode = "123456789";
	GrantedEntitlement.Quantity = 1;
	GrantedEntitlement.Source = EAccelByteEntitlementSource::ACHIEVEMENT;
	GrantedEntitlement.Region = Items.Data[0].Region;
	GrantedEntitlement.Language = Items.Data[0].Language;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GrantingUserEntitlement"));
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("UserId: %s"), *FRegistry::Credentials.GetUserId());

	GrantedEntitlements.Add(GrantedEntitlement);
	ItemIdsGranted.Add(Items.Data[0].ItemId);

	GrantedEntitlement.ItemId = Items.Data[1].ItemId;
	GrantedEntitlement.ItemNamespace = Items.Data[1].Namespace;
	GrantedEntitlement.GrantedCode = "123456789";
	GrantedEntitlement.Quantity = 1;
	GrantedEntitlement.Source = EAccelByteEntitlementSource::ACHIEVEMENT;
	GrantedEntitlement.Region = Items.Data[1].Region;
	GrantedEntitlement.Language = Items.Data[1].Language;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GrantingUserEntitlement"));
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("UserId: %s"), *FRegistry::Credentials.GetUserId());

	GrantedEntitlements.Add(GrantedEntitlement);
	ItemIdsGranted.Add(Items.Data[1].ItemId);

	bool bGrantEntitlementSuccess = false;
	TArray<FAccelByteModelsStackableEntitlementInfo> GrantedEntitlementResult;
	FRegistry::ServerEcommerce.GrantUserEntitlements(FRegistry::Credentials.GetUserId(), GrantedEntitlements, THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>::CreateLambda([&bGrantEntitlementSuccess, &GrantedEntitlementResult](const TArray<FAccelByteModelsStackableEntitlementInfo>& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			GrantedEntitlementResult = Result;
			bGrantEntitlementSuccess = true;
			for (auto Entitlement : Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Entitlement: %s"), *Entitlement.Name);
			}
		}), EcommerceTestErrorHandler);
	WaitUntil(bGrantEntitlementSuccess, "Waiting for entitlement granted...");

	AB_TEST_TRUE(bGrantEntitlementSuccess);
#pragma endregion GrantEntitlement

#pragma region CheckEntitlementGranted
	bool bQueryEntitlementSuccess = false;
	FAccelByteModelsEntitlementPagingSlicedResult UserEntitlement;
	FRegistry::Entitlement.QueryUserEntitlements("", ItemIdsGranted, 0, 20, THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda([&bQueryEntitlementSuccess, &UserEntitlement](const FAccelByteModelsEntitlementPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			UserEntitlement = Result;
			bQueryEntitlementSuccess = true;
		}), EcommerceTestErrorHandler, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);
	WaitUntil(bQueryEntitlementSuccess, "Waiting for Query User's Entitlement...");

	bool bEntitlementGranted = true;
	for (auto Grant : GrantedEntitlements)
	{
		bool bIsFound = false;
		for (auto Entitlement : UserEntitlement.Data)
		{
			if (Entitlement.ItemId == Grant.ItemId && Entitlement.GrantedCode == Grant.GrantedCode)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Granted Entitlement Found!"));
				bIsFound = true;
				break;
			}
		}
		if (!bIsFound)
		{
			bEntitlementGranted = false;
			break;
		}
	}
	AB_TEST_TRUE(bEntitlementGranted);
#pragma endregion CheckEntitlementGranted

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestServerConsumeUserEntitlement, "AccelByte.Tests.Ecommerce.G.ServerConsumeUserEntitlement", AutomationFlagMaskEcommerce)
bool FEcommerceTestServerConsumeUserEntitlement::RunTest(const FString& Parameters)
{

	bool bUserLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginSuccess]()
		{
			bUserLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bUserLoginSuccess, "Waiting for Login...");
	AB_TEST_TRUE(bUserLoginSuccess);

#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::INGAMEITEM;
	bool bGetItemByCriteriaSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FAccelByteModelsItemPagingSlicedResult Items;
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetItemByCriteriaSuccess = true;
			Items = Result;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get items...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess);

	FAccelByteModelsItemCriteria ItemCriteria2;
	ItemCriteria2.Language = TEXT("en");
	ItemCriteria2.Region = TEXT("US");
	ItemCriteria2.ItemType = EAccelByteItemType::COINS;
	bool bGetItemByCriteriaSuccess2 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FAccelByteModelsItemPagingSlicedResult Items2;
	FRegistry::Item.GetItemsByCriteria(ItemCriteria2, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetItemByCriteriaSuccess2 = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess2, "Waiting for get items...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess2);
#pragma endregion GetItemByCriteria

#pragma region GrantEntitlement	
	FAccelByteModelsEntitlementGrant GrantedEntitlement;
	GrantedEntitlement.ItemId = Items.Data[0].ItemId;
	GrantedEntitlement.ItemNamespace = Items.Data[0].Namespace;
	GrantedEntitlement.GrantedCode = "123456789";
	GrantedEntitlement.Quantity = 1;
	GrantedEntitlement.Source = EAccelByteEntitlementSource::ACHIEVEMENT;
	GrantedEntitlement.Region = Items.Data[0].Region;
	GrantedEntitlement.Language = Items.Data[0].Language;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GrantingUserEntitlement"));
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("UserId: %s"), *FRegistry::Credentials.GetUserId());

	bool bGrantEntitlementSuccess = false;
	TArray<FAccelByteModelsStackableEntitlementInfo> GrantedEntitlementResult;
	FRegistry::ServerEcommerce.GrantUserEntitlements(FRegistry::Credentials.GetUserId(), { GrantedEntitlement }, THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>::CreateLambda([&bGrantEntitlementSuccess, &GrantedEntitlementResult](const TArray<FAccelByteModelsStackableEntitlementInfo>& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			GrantedEntitlementResult = Result;
			bGrantEntitlementSuccess = true;
			for (auto Entitlement : Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Entitlement: %s"), *Entitlement.Name);
			}
		}), EcommerceTestErrorHandler);
	WaitUntil(bGrantEntitlementSuccess, "Waiting for entitlement granted...");

	AB_TEST_TRUE(bGrantEntitlementSuccess);
#pragma endregion GrantEntitlement

#pragma region ConsumeEntitlement
	bool bIsConsumed = false;
	const int32 LastEntitlementCount = GrantedEntitlementResult[0].UseCount;
	FRegistry::ServerEcommerce.ConsumeUserEntitlement(FRegistry::Credentials.GetUserId(), GrantedEntitlementResult[0].Id, 1,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&bIsConsumed, &LastEntitlementCount](const FAccelByteModelsEntitlementInfo& Result)
			{
				bIsConsumed = Result.UseCount == LastEntitlementCount - 1;
			}), EcommerceTestErrorHandler);
	WaitUntil(bIsConsumed, "Waiting to be consumed");

	AB_TEST_TRUE(bIsConsumed);
#pragma endregion ConsumeEntitlement
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestDisableEnableRevokeUserEntitlement, "AccelByte.Tests.Ecommerce.H.DisableEnableRevokeUserEntitlement", AutomationFlagMaskEcommerce)
bool FEcommerceTestDisableEnableRevokeUserEntitlement::RunTest(const FString& Parameters)
{
	bool bUserLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginSuccess]()
		{
			bUserLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bUserLoginSuccess, "Waiting for Login...");
	AB_TEST_TRUE(bUserLoginSuccess);

#pragma region GetItemByCriteria
	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::INGAMEITEM;
	bool bGetItemByCriteriaSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FAccelByteModelsItemPagingSlicedResult Items;
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetItemByCriteriaSuccess = true;
			Items = Result;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get items...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess);

	FAccelByteModelsItemCriteria ItemCriteria2;
	ItemCriteria2.Language = TEXT("en");
	ItemCriteria2.Region = TEXT("US");
	ItemCriteria2.ItemType = EAccelByteItemType::COINS;
	bool bGetItemByCriteriaSuccess2 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FAccelByteModelsItemPagingSlicedResult Items2;
	FRegistry::Item.GetItemsByCriteria(ItemCriteria2, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetItemByCriteriaSuccess2 = true;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess2, "Waiting for get items...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess2);
#pragma endregion GetItemByCriteria

#pragma region GrantEntitlement
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bClientLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bClientLoginSuccess, "Waiting for Client Login...");

	FAccelByteModelsEntitlementGrant GrantedEntitlement;
	GrantedEntitlement.ItemId = Items.Data[0].ItemId;
	GrantedEntitlement.ItemNamespace = Items.Data[0].Namespace;
	GrantedEntitlement.GrantedCode = "123456789";
	GrantedEntitlement.Quantity = 1;
	GrantedEntitlement.Source = EAccelByteEntitlementSource::ACHIEVEMENT;
	GrantedEntitlement.Region = Items.Data[0].Region;
	GrantedEntitlement.Language = Items.Data[0].Language;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GrantingUserEntitlement"));
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("UserId: %s"), *FRegistry::Credentials.GetUserId());

	bool bGrantEntitlementSuccess = false;
	TArray<FAccelByteModelsStackableEntitlementInfo> GrantedEntitlementResult;
	FRegistry::ServerEcommerce.GrantUserEntitlements(FRegistry::Credentials.GetUserId(), { GrantedEntitlement }, THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>::CreateLambda([&bGrantEntitlementSuccess, &GrantedEntitlementResult](const TArray<FAccelByteModelsStackableEntitlementInfo>& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			GrantedEntitlementResult = Result;
			bGrantEntitlementSuccess = true;
			for (auto Entitlement : Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Entitlement: %s"), *Entitlement.Name);
			}
		}), EcommerceTestErrorHandler);
	WaitUntil(bGrantEntitlementSuccess, "Waiting for entitlement granted...");

	AB_TEST_TRUE(bGrantEntitlementSuccess);
#pragma endregion GrantEntitlement

#pragma region EnableEntitlement
	bool bIsEnable = false;
	FRegistry::ServerEcommerce.EnableUserEntitlement(FRegistry::Credentials.GetUserId(), GrantedEntitlementResult[0].Id,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&bIsEnable](const FAccelByteModelsEntitlementInfo& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Enable the Entitlement Success"));
				bIsEnable = Result.Status == EAccelByteEntitlementStatus::ACTIVE;
			}), EcommerceTestErrorHandler);
	WaitUntil(bIsEnable, "Waiting to enable the entitlement");
	AB_TEST_TRUE(bIsEnable);
#pragma endregion EnableEntitlement

#pragma region DisableEntitlement
	bool bIsDisabled = false;
	FRegistry::ServerEcommerce.DisableUserEntitlement(FRegistry::Credentials.GetUserId(), GrantedEntitlementResult[0].Id,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&bIsDisabled](const FAccelByteModelsEntitlementInfo& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Disable the Entitlement Success"));
				bIsDisabled = Result.Status == EAccelByteEntitlementStatus::INACTIVE;
			}), EcommerceTestErrorHandler);
	WaitUntil(bIsDisabled, "Waiting to disable the entitlement");
	AB_TEST_TRUE(bIsDisabled);
#pragma endregion DisableEntitlement

#pragma region RevokeEntitlement
	bool bIsRevoked = false;
	FRegistry::ServerEcommerce.RevokeUserEntitlement(FRegistry::Credentials.GetUserId(), GrantedEntitlementResult[0].Id,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&bIsRevoked](const FAccelByteModelsEntitlementInfo& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Revoke the Entitlement Success"));
				bIsRevoked = Result.Status == EAccelByteEntitlementStatus::REVOKED;
			}), EcommerceTestErrorHandler);
	WaitUntil(bIsRevoked, "Waiting to revoke the entitlement");
	AB_TEST_TRUE(bIsRevoked);
#pragma endregion RevokeEntitlement

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestDisableEnableRevokeUserEntitlementInvalidEntitlementId, "AccelByte.Tests.Ecommerce.H.DisableEnableRevokeUserEntitlementInvalidEntitlementId", AutomationFlagMaskEcommerce)
bool FEcommerceTestDisableEnableRevokeUserEntitlementInvalidEntitlementId::RunTest(const FString& Parameters)
{
	bool bUserLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginSuccess]()
		{
			bUserLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bUserLoginSuccess, "Waiting for Login...");
	AB_TEST_TRUE(bUserLoginSuccess);

	const FString InvalidId = TEXT("Invalid");
#pragma region DisableEntitlement
	bool bIsDisabledFailed = false;
	FRegistry::ServerEcommerce.DisableUserEntitlement(FRegistry::Credentials.GetUserId(), InvalidId,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([](const FAccelByteModelsEntitlementInfo& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Disable the Entitlement Success"));
			}), FErrorHandler::CreateLambda([&bIsDisabledFailed](int32 Code, const FString& Message) {
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Failed Disable the Entitlement (EXPECTED)"));
				bIsDisabledFailed = true;
				})
				);
	WaitUntil(bIsDisabledFailed, "Waiting to disable the entitlement");
	AB_TEST_TRUE(bIsDisabledFailed);
#pragma endregion DisableEntitlement

#pragma region EnableEntitlement
	bool bIsEnableFailed = false;
	FRegistry::ServerEcommerce.EnableUserEntitlement(FRegistry::Credentials.GetUserId(), InvalidId,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([](const FAccelByteModelsEntitlementInfo& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Enable the Entitlement Success"));
			}), FErrorHandler::CreateLambda([&bIsEnableFailed](int32 Code, const FString& Message) {
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Failed Disable the Entitlement (EXPECTED)"));
				bIsEnableFailed = true;
				}));
	WaitUntil(bIsEnableFailed, "Waiting to enable the entitlement");
	AB_TEST_TRUE(bIsEnableFailed);
#pragma endregion EnableEntitlement

#pragma region RevokeEntitlement
	bool bIsRevokedFailed = false;
	FRegistry::ServerEcommerce.RevokeUserEntitlement(FRegistry::Credentials.GetUserId(), InvalidId,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([](const FAccelByteModelsEntitlementInfo& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Revoke the Entitlement Success"));
			}), FErrorHandler::CreateLambda([&bIsRevokedFailed](int32 Code, const FString& Message) {
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Failed Disable the Entitlement (EXPECTED)"));
				bIsRevokedFailed = true;
				}));
	WaitUntil(bIsRevokedFailed, "Waiting to revoke the entitlement");
	AB_TEST_TRUE(bIsRevokedFailed);
#pragma endregion RevokeEntitlement

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestRevokeUserEntitlements, "AccelByte.Tests.Ecommerce.I.RevokeUserEntitlements", AutomationFlagMaskEcommerce);
bool FEcommerceTestRevokeUserEntitlements::RunTest(const FString& Parameters)
{
	bool bUserLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginSuccess]()
		{
			bUserLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bUserLoginSuccess, "Waiting for Login...");
	AB_TEST_TRUE(bUserLoginSuccess);

#pragma region GetItemByCriteria
	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::INGAMEITEM;
	bool bGetItemByCriteriaSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FAccelByteModelsItemPagingSlicedResult Items;
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetItemByCriteriaSuccess = true;
			Items = Result;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get items...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess);

	FAccelByteModelsItemCriteria ItemCriteria2;
	ItemCriteria2.Language = TEXT("en");
	ItemCriteria2.Region = TEXT("US");
	ItemCriteria2.ItemType = EAccelByteItemType::COINS;
	bool bGetItemByCriteriaSuccess2 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FAccelByteModelsItemPagingSlicedResult Items2;
	FRegistry::Item.GetItemsByCriteria(ItemCriteria2, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetItemByCriteriaSuccess2 = true;
			Items2 = Result;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess2, "Waiting for get items...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess2);
#pragma endregion GetItemByCriteria

#pragma region GrantEntitlement
	TArray<FAccelByteModelsEntitlementGrant> GrantedEntitlements;

	FAccelByteModelsEntitlementGrant GrantedEntitlement;
	GrantedEntitlement.ItemId = Items.Data[0].ItemId;
	GrantedEntitlement.ItemNamespace = Items.Data[0].Namespace;
	GrantedEntitlement.GrantedCode = "123456789";
	GrantedEntitlement.Quantity = 1;
	GrantedEntitlement.Source = EAccelByteEntitlementSource::ACHIEVEMENT;
	GrantedEntitlement.Region = Items.Data[0].Region;
	GrantedEntitlement.Language = Items.Data[0].Language;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GrantingUserEntitlement"));
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("UserId: %s"), *FRegistry::Credentials.GetUserId());

	GrantedEntitlements.Add(GrantedEntitlement);

	GrantedEntitlement.ItemId = Items2.Data[0].ItemId;
	GrantedEntitlement.ItemNamespace = Items2.Data[0].Namespace;
	GrantedEntitlement.GrantedCode = "123456789";
	GrantedEntitlement.Quantity = 1;
	GrantedEntitlement.Source = EAccelByteEntitlementSource::ACHIEVEMENT;
	GrantedEntitlement.Region = Items2.Data[0].Region;
	GrantedEntitlement.Language = Items2.Data[0].Language;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GrantingUserEntitlement"));
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("UserId: %s"), *FRegistry::Credentials.GetUserId());

	GrantedEntitlements.Add(GrantedEntitlement);

	bool bGrantEntitlementSuccess = false;
	TArray<FString> GrantedEntitlementResultIds;
	FRegistry::ServerEcommerce.GrantUserEntitlements(FRegistry::Credentials.GetUserId(), GrantedEntitlements,
		THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>::CreateLambda([&bGrantEntitlementSuccess, &GrantedEntitlementResultIds](const TArray<FAccelByteModelsStackableEntitlementInfo>& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				for (auto Res : Result)
				{
					GrantedEntitlementResultIds.Add(Res.Id);
				}
				bGrantEntitlementSuccess = true;
				for (auto Entitlement : Result)
				{
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Entitlement: %s"), *Entitlement.Name);
				}
			}), EcommerceTestErrorHandler);
	WaitUntil(bGrantEntitlementSuccess, "Waiting for entitlement granted...");

	AB_TEST_TRUE(bGrantEntitlementSuccess);
#pragma endregion GrantEntitlement

#pragma region RevokeEntitlements
	bool bIsRevokeSuccess = false;
	FRegistry::ServerEcommerce.RevokeUserEntitlements(FRegistry::Credentials.GetUserId(), GrantedEntitlementResultIds,
		THandler<FAccelByteModelsBulkRevokeEntitlements>::CreateLambda([&bIsRevokeSuccess](const FAccelByteModelsBulkRevokeEntitlements Response)
			{
				bIsRevokeSuccess = true;
			}), EcommerceTestErrorHandler);
	WaitUntil(bIsRevokeSuccess, "Waiting for revoking the entitlements");

	AB_TEST_TRUE(bIsRevokeSuccess);

#pragma endregion RevokeEntitlements

#pragma region CheckEntitlementRevoked
	bool bServerQueryEntitlementSuccess = false;
	bool bServerEntitlementRevoked = true;
	FRegistry::ServerEcommerce.QueryUserEntitlements(FRegistry::Credentials.GetUserId(), true, "",
		GrantedEntitlementResultIds, 0, 20, THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda(
			[&bServerQueryEntitlementSuccess, &bServerEntitlementRevoked, &GrantedEntitlementResultIds](const FAccelByteModelsEntitlementPagingSlicedResult& Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
				bServerQueryEntitlementSuccess = true;
				for (const FString& ExpectedRevokedId : GrantedEntitlementResultIds)
				{
					for (auto ResData : Result.Data)
					{
						if (ResData.Id == ExpectedRevokedId)
						{
							if (ResData.Status != EAccelByteEntitlementStatus::REVOKED)
							{
								bServerEntitlementRevoked = false;
								break;
							}
						}
					}
				}
			}), EcommerceTestErrorHandler, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);
	WaitUntil(bServerQueryEntitlementSuccess, "Waiting for Query User's Entitlement...");
	AB_TEST_TRUE(bServerQueryEntitlementSuccess);
	AB_TEST_TRUE(bServerEntitlementRevoked);
#pragma endregion CheckEntitlementRevoked

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECommerceTestFulfillmentRedeemCode, "AccelByte.Tests.Ecommerce.Fulfillment.1.FulfillmentRedeemCode", AutomationFlagMaskEcommerce);
bool FECommerceTestFulfillmentRedeemCode::RunTest(const FString& Parameters)
{
	FAccelByteModelsFulfillmentResult fulfillmentResult;
	bool bRedeemCodeSuccess = false;
	FRegistry::Fulfillment.RedeemCode(
		EcommerceTestExpectedVariable.codeInfo.value,
		"US",
		"en",
		THandler<FAccelByteModelsFulfillmentResult>::CreateLambda([&bRedeemCodeSuccess, &fulfillmentResult](const FAccelByteModelsFulfillmentResult& Result)
			{
				fulfillmentResult = Result;
				bRedeemCodeSuccess = true;
			}), EcommerceTestErrorHandler);
	WaitUntil(bRedeemCodeSuccess, "Waiting for redeem code...");

	AB_TEST_EQUAL(fulfillmentResult.EntitlementSummaries[0].ItemId, EcommerceTestExpectedVariable.redeemableItem.itemId);

	FAccelByteModelsEntitlementInfo getUserEntitlementByIdResult;
	bool bGetEntitlementSuccess = false;
	FRegistry::Entitlement.GetUserEntitlementById(fulfillmentResult.EntitlementSummaries[0].Id, THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&bGetEntitlementSuccess, &getUserEntitlementByIdResult](const FAccelByteModelsEntitlementInfo& Result)
		{
			getUserEntitlementByIdResult = Result;
			bGetEntitlementSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bGetEntitlementSuccess, "Waiting for get entitlement...");

	AB_TEST_EQUAL(getUserEntitlementByIdResult.Id, fulfillmentResult.EntitlementSummaries[0].Id);
	AB_TEST_EQUAL(getUserEntitlementByIdResult.ItemId, EcommerceTestExpectedVariable.redeemableItem.itemId);
	AB_TEST_EQUAL(getUserEntitlementByIdResult.Source, EAccelByteEntitlementSource::REDEEM_CODE);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECommerceTestFulfillmentRedeemCodeExceeded, "AccelByte.Tests.Ecommerce.Fulfillment.2.FulfillmentRedeemCodeExceeded", AutomationFlagMaskEcommerce);
bool FECommerceTestFulfillmentRedeemCodeExceeded::RunTest(const FString& Parameters)
{
	FAccelByteModelsFulfillmentResult fulfillmentResult;
	bool bRedeemCodeSuccess = false;
	bool bRedeemCodeDone = false;
	int32 ErrorCode;
	FRegistry::Fulfillment.RedeemCode(
		EcommerceTestExpectedVariable.codeInfo.value,
		"",
		"",
		THandler<FAccelByteModelsFulfillmentResult>::CreateLambda([&bRedeemCodeSuccess, &bRedeemCodeDone, &fulfillmentResult](const FAccelByteModelsFulfillmentResult& Result)
			{
				fulfillmentResult = Result;
				bRedeemCodeSuccess = true;
				bRedeemCodeDone = true;
			}),
		FErrorHandler::CreateLambda([&bRedeemCodeDone, &ErrorCode](int32 Code, const FString Message)
			{
				ErrorCode = Code;
				bRedeemCodeDone = true;
			}));
	WaitUntil(bRedeemCodeDone, "Waiting for redeem code...");

	AB_TEST_FALSE(bRedeemCodeSuccess);
	AB_TEST_TRUE(bRedeemCodeDone);
	AB_TEST_EQUAL(ErrorCode, static_cast<int32>(ErrorCodes::ExceedMaxRedeemCountPerUserException));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECommerceTestFulfillmentRedeemCodeInactived, "AccelByte.Tests.Ecommerce.Fulfillment.3.FulfillmentRedeemCodeInactived", AutomationFlagMaskEcommerce);
bool FECommerceTestFulfillmentRedeemCodeInactived::RunTest(const FString& Parameters)
{
	FCampaignUpdateModel const CampaignUpdate
	{
		EcommerceTestExpectedVariable.campaignName,
		"UE4 Campaign Test",
		TArray<FString>(),
		"INACTIVE",
		-1,
		1,
		-1,
		-1,
		FDateTime::UtcNow().ToIso8601(),
		FDateTime::MaxValue().ToIso8601(),
		"ITEM",
		TArray<FRedeemableItem>()
	};
	FCampaignInfo updateCampaignResult;
	bool bUpdateCampaignSuccess = false;
	AdminUpdateEcommerceCampaign(
		EcommerceTestExpectedVariable.campaignResult.id,
		CampaignUpdate,
		THandler<FCampaignInfo>::CreateLambda([&bUpdateCampaignSuccess, &updateCampaignResult](const FCampaignInfo& Result)
			{
				updateCampaignResult = Result;
				bUpdateCampaignSuccess = true;
			}), EcommerceTestErrorHandler);
	WaitUntil(bUpdateCampaignSuccess, "Waiting for update campaign...");

	AB_TEST_TRUE(bUpdateCampaignSuccess);

	FAccelByteModelsFulfillmentResult fulfillmentResult;
	bool bRedeemCodeSuccess = false;
	bool bRedeemCodeDone = false;
	int32 ErrorCode;
	FRegistry::Fulfillment.RedeemCode(
		EcommerceTestExpectedVariable.codeInfo.value,
		"",
		"",
		THandler<FAccelByteModelsFulfillmentResult>::CreateLambda([&bRedeemCodeSuccess, &bRedeemCodeDone, &fulfillmentResult](const FAccelByteModelsFulfillmentResult& Result)
			{
				fulfillmentResult = Result;
				bRedeemCodeSuccess = true;
				bRedeemCodeDone = true;
			}),
		FErrorHandler::CreateLambda([&bRedeemCodeDone, &ErrorCode](int32 Code, const FString Message)
			{
				ErrorCode = Code;
				bRedeemCodeDone = true;
			}));
	WaitUntil(bRedeemCodeDone, "Waiting for redeem code...");

	AB_TEST_FALSE(bRedeemCodeSuccess);
	AB_TEST_TRUE(bRedeemCodeDone);
	AB_TEST_EQUAL(ErrorCode, static_cast<int32>(ErrorCodes::CampaignIsInactiveException));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECommerceTestFulfillmentRedeemCodeDisabled, "AccelByte.Tests.Ecommerce.Fulfillment.4.FulfillmentRedeemCodeDisabled", AutomationFlagMaskEcommerce);
bool FECommerceTestFulfillmentRedeemCodeDisabled::RunTest(const FString& Parameters)
{
	FCodeInfo disableCodeResult;
	bool bDisableCodeSuccess = false;
	AdminDisableEcommerceCampaignCode(
		EcommerceTestExpectedVariable.codeInfo.value,
		THandler<FCodeInfo>::CreateLambda([&bDisableCodeSuccess, &disableCodeResult](const FCodeInfo& Result)
			{
				disableCodeResult = Result;
				bDisableCodeSuccess = true;
			}), EcommerceTestErrorHandler);
	WaitUntil(bDisableCodeSuccess, "Waiting for disabling code...");

	AB_TEST_TRUE(bDisableCodeSuccess);

	FAccelByteModelsFulfillmentResult fulfillmentResult;
	bool bRedeemCodeSuccess = false;
	bool bRedeemCodeDone = false;
	int32 ErrorCode;
	FRegistry::Fulfillment.RedeemCode(
		EcommerceTestExpectedVariable.codeInfo.value,
		"",
		"",
		THandler<FAccelByteModelsFulfillmentResult>::CreateLambda([&bRedeemCodeSuccess, &bRedeemCodeDone, &fulfillmentResult](const FAccelByteModelsFulfillmentResult& Result)
			{
				fulfillmentResult = Result;
				bRedeemCodeSuccess = true;
				bRedeemCodeDone = true;
			}),
		FErrorHandler::CreateLambda([&bRedeemCodeDone, &ErrorCode](int32 Code, const FString Message)
			{
				ErrorCode = Code;
				bRedeemCodeDone = true;
			}));
	WaitUntil(bRedeemCodeDone, "Waiting for redeem code...");

	AB_TEST_FALSE(bRedeemCodeSuccess);
	AB_TEST_TRUE(bRedeemCodeDone);
	AB_TEST_EQUAL(ErrorCode, static_cast<int32>(ErrorCodes::CodeIsInactiveException));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECommerceTestFulfillmentRedeemCodeInvalid, "AccelByte.Tests.Ecommerce.Fulfillment.1.FulfillmentRedeemCodeInvalid", AutomationFlagMaskEcommerce);
bool FECommerceTestFulfillmentRedeemCodeInvalid::RunTest(const FString& Parameters)
{
	FAccelByteModelsFulfillmentResult fulfillmentResult;
	bool bRedeemCodeSuccess = false;
	bool bRedeemCodeDone = false;
	int32 ErrorCode;
	FRegistry::Fulfillment.RedeemCode(
		"InvalidCode",
		"US",
		"en",
		THandler<FAccelByteModelsFulfillmentResult>::CreateLambda([&bRedeemCodeSuccess, &bRedeemCodeDone, &fulfillmentResult](const FAccelByteModelsFulfillmentResult& Result)
			{
				fulfillmentResult = Result;
				bRedeemCodeSuccess = true;
				bRedeemCodeDone = true;
			}),
		FErrorHandler::CreateLambda([&bRedeemCodeDone, &ErrorCode](int32 Code, const FString Message)
			{
				ErrorCode = Code;
				bRedeemCodeDone = true;
			}));
	WaitUntil(bRedeemCodeDone, "Waiting for redeem code...");

	AB_TEST_FALSE(bRedeemCodeSuccess);
	AB_TEST_TRUE(bRedeemCodeDone);
	AB_TEST_EQUAL(ErrorCode, static_cast<int32>(ErrorCodes::CodeNotFoundException));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECommerceTestFulfillmentRedeemCodeNotStarted, "AccelByte.Tests.Ecommerce.Fulfillment.1.FulfillmentRedeemCodeNotStarted", AutomationFlagMaskEcommerce);
bool FECommerceTestFulfillmentRedeemCodeNotStarted::RunTest(const FString& Parameters)
{
	FAccelByteModelsFulfillmentResult fulfillmentResult;
	bool bRedeemCodeSuccess = false;
	bool bRedeemCodeDone = false;
	int32 ErrorCode;
	FRegistry::Fulfillment.RedeemCode(
		EcommerceTestExpectedVariable.notStartedCodeInfo.value,
		"US",
		"en",
		THandler<FAccelByteModelsFulfillmentResult>::CreateLambda([&bRedeemCodeSuccess, &bRedeemCodeDone, &fulfillmentResult](const FAccelByteModelsFulfillmentResult& Result)
			{
				fulfillmentResult = Result;
				bRedeemCodeSuccess = true;
				bRedeemCodeDone = true;
			}),
		FErrorHandler::CreateLambda([&bRedeemCodeDone, &ErrorCode](int32 Code, const FString Message)
			{
				ErrorCode = Code;
				bRedeemCodeDone = true;
			}));
	WaitUntil(bRedeemCodeDone, "Waiting for redeem code...");

	AB_TEST_FALSE(bRedeemCodeSuccess);
	AB_TEST_TRUE(bRedeemCodeDone);
	AB_TEST_EQUAL(ErrorCode, static_cast<int32>(ErrorCodes::CodeRedeemptionNotStartedException));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECommerceTestFulfillmentRedeemCodeAlreadyEnded, "AccelByte.Tests.Ecommerce.Fulfillment.1.FulfillmentRedeemCodeAlreadyEnded", AutomationFlagMaskEcommerce);
bool FECommerceTestFulfillmentRedeemCodeAlreadyEnded::RunTest(const FString& Parameters)
{
	FAccelByteModelsFulfillmentResult fulfillmentResult;
	bool bRedeemCodeSuccess = false;
	bool bRedeemCodeDone = false;
	int32 ErrorCode;
	FRegistry::Fulfillment.RedeemCode(
		EcommerceTestExpectedVariable.expiredCodeInfo.value,
		"US",
		"en",
		THandler<FAccelByteModelsFulfillmentResult>::CreateLambda([&bRedeemCodeSuccess, &bRedeemCodeDone, &fulfillmentResult](const FAccelByteModelsFulfillmentResult& Result)
			{
				fulfillmentResult = Result;
				bRedeemCodeSuccess = true;
				bRedeemCodeDone = true;
			}),
		FErrorHandler::CreateLambda([&bRedeemCodeDone, &ErrorCode](int32 Code, const FString Message)
			{
				ErrorCode = Code;
				bRedeemCodeDone = true;
			}));
	WaitUntil(bRedeemCodeDone, "Waiting for redeem code...");

	AB_TEST_FALSE(bRedeemCodeSuccess);
	AB_TEST_TRUE(bRedeemCodeDone);
	AB_TEST_EQUAL(ErrorCode, static_cast<int32>(ErrorCodes::CodeRedeemptionAlreadyEndedException));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECommerceTestServerEntitlementFulfillItem, "AccelByte.Tests.Ecommerce.Fulfillment.5.ServerEntitlementFulfillItemSuccess", AutomationFlagMaskEcommerce);
bool FECommerceTestServerEntitlementFulfillItem::RunTest(const FString& Parameters)
{
#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::INGAMEITEM;
	bool bGetItemByCriteriaSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FAccelByteModelsItemPagingSlicedResult Items;
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetItemByCriteriaSuccess = true;
			Items = Result;
		}), EcommerceTestErrorHandler);

	WaitUntil(bGetItemByCriteriaSuccess, "Waiting for get items...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess);
#pragma endregion GetItemByCriteria

#pragma region FulfillEntitlement
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bClientLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	FlushHttpRequests();
	WaitUntil(bClientLoginSuccess, "Waiting for Client Login...");

	FAccelByteModelsFulfillmentRequest FulfillEntitlement;
	FulfillEntitlement.ItemId = Items.Data[0].ItemId;
	FulfillEntitlement.OrderNo = "123456789";
	FulfillEntitlement.Quantity = 1;
	FulfillEntitlement.Source = EAccelByteItemSource::ACHIEVEMENT;
	FulfillEntitlement.Region = Items.Data[0].Region;
	FulfillEntitlement.Language = Items.Data[0].Language;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GrantingUserEntitlement"));
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("UserId: %s"), *FRegistry::Credentials.GetUserId());

	bool bFulfillEntitlementSuccess = false;
	FAccelByteModelsFulfillmentResult FulfillEntitlementResult;
	FRegistry::ServerEcommerce.FulfillUserItem(FRegistry::Credentials.GetUserId(), FulfillEntitlement, THandler<FAccelByteModelsFulfillmentResult>::CreateLambda([&bFulfillEntitlementSuccess, &FulfillEntitlementResult](const FAccelByteModelsFulfillmentResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			FulfillEntitlementResult = Result;
			bFulfillEntitlementSuccess = true;
			for (auto Entitlement : Result.EntitlementSummaries)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Entitlement: %s"), *Entitlement.Id);
			}
		}), EcommerceTestErrorHandler);
	FlushHttpRequests();
	WaitUntil(bFulfillEntitlementSuccess, "Waiting for entitlement granted...");

	AB_TEST_TRUE(bFulfillEntitlementSuccess);
#pragma endregion FulfillEntitlement

#pragma region CheckEntitlementFulfilled
	bool bQueryEntitlementSuccess = false;
	FAccelByteModelsEntitlementPagingSlicedResult UserEntitlement;
	FRegistry::Entitlement.QueryUserEntitlements("", EcommerceTestExpectedVariable.LootItem.itemId, 0, 20, THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda([&bQueryEntitlementSuccess, &UserEntitlement](const FAccelByteModelsEntitlementPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			UserEntitlement = Result;
			bQueryEntitlementSuccess = true;
		}), EcommerceTestErrorHandler, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);
	FlushHttpRequests();
	WaitUntil(bQueryEntitlementSuccess, "Waiting for Query User's Entitlement...");

	bool bEntitlementFulfilled = false;
	for (auto Entitlement : UserEntitlement.Data)
	{
		if (Entitlement.Id == FulfillEntitlementResult.EntitlementSummaries[0].Id)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Granted Entitlement Found!"));
			bEntitlementFulfilled = true;
			break;
		}
	}
	AB_TEST_TRUE(bEntitlementFulfilled);
#pragma endregion CheckEntitlementFulfilled
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECommerceTestServerEntitlementFulfillInvalid, "AccelByte.IntegrationTests.Ecommerce.Fulfillment.5.ServerEntitlementFulfillInvalidFailed", AutomationFlagMaskEcommerce);
bool FECommerceTestServerEntitlementFulfillInvalid::RunTest(const FString& Parameters)
{

#pragma region FulfillEntitlement
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bClientLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	FlushHttpRequests();
	WaitUntil(bClientLoginSuccess, "Waiting for Client Login...");

	FAccelByteModelsFulfillmentRequest FulfilledEntitlement;
	FulfilledEntitlement.ItemId = "ffffffffffff4fff8fffffffffffffff"; // Invalid item id
	FulfilledEntitlement.OrderNo = "123456789";
	FulfilledEntitlement.Quantity = 1;
	FulfilledEntitlement.Source = EAccelByteItemSource::ACHIEVEMENT;
	FulfilledEntitlement.Region = "US";
	FulfilledEntitlement.Language = "en";
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("FulfillingUserEntitlement"));
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("UserId: %s"), *FRegistry::Credentials.GetUserId());

	bool bFulfillEntitlementSuccess = false;
	bool bFulfillEntitlementDone = false;
	FRegistry::ServerEcommerce.FulfillUserItem(FRegistry::Credentials.GetUserId(), FulfilledEntitlement, THandler<FAccelByteModelsFulfillmentResult>::CreateLambda([&bFulfillEntitlementSuccess, &bFulfillEntitlementDone](const FAccelByteModelsFulfillmentResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bFulfillEntitlementSuccess = true;
			bFulfillEntitlementDone = true;
			for (auto Entitlement : Result.EntitlementSummaries)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Entitlement: %s"), *Entitlement.Id);
			}
		}), FErrorHandler::CreateLambda([&bFulfillEntitlementDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				if ((ErrorCodes)ErrorCode != ErrorCodes::ItemNotFoundException)
				{
					UE_LOG(LogAccelByteEcommerceTest, Error, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				}
				else
				{
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
					bFulfillEntitlementDone = true;
				}
			}));
	FlushHttpRequests();
	WaitUntil(bFulfillEntitlementDone, "Waiting for entitlement Fulfilled...");

	AB_TEST_TRUE(bFulfillEntitlementDone);
	AB_TEST_FALSE(bFulfillEntitlementSuccess);
#pragma endregion FulfillEntitlement

#pragma region CheckEntitlementFulfilled
	bool bQueryEntitlementSuccess = false;
	FAccelByteModelsEntitlementPagingSlicedResult UserEntitlement;
	FRegistry::Entitlement.QueryUserEntitlements("", "Invalid", 0, 20, THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda([&bQueryEntitlementSuccess, &UserEntitlement](const FAccelByteModelsEntitlementPagingSlicedResult& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			UserEntitlement = Result;
			bQueryEntitlementSuccess = true;
		}), EcommerceTestErrorHandler, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);
	FlushHttpRequests();
	WaitUntil(bQueryEntitlementSuccess, "Waiting for Query User's Entitlement...");

	bool bEntitlementFulfilled = false;
	for (auto Entitlement : UserEntitlement.Data)
	{
		if (Entitlement.ItemId == "Invalid")
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Fulfilled Entitlement Found!"));
			bEntitlementFulfilled = true;
			break;
		}
	}
	AB_TEST_FALSE(bEntitlementFulfilled);
#pragma endregion CheckEntitlementFulfilled
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECommerceTestGetCurrencyList, "AccelByte.Tests.Ecommerce.Currency.1.GetCurrencyList", AutomationFlagMaskEcommerce);
bool FECommerceTestGetCurrencyList::RunTest(const FString& Parameters)
{
	FAccelByteModelsCurrencyList currencyListResult;
	bool bGetCurrencyListSuccess = false;
	bool bGetCurrencyListDone = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bGetCurrencyListSuccess]()
		{
			bGetCurrencyListSuccess = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bGetCurrencyListSuccess, "Waiting for login...");
	AB_TEST_TRUE(bGetCurrencyListSuccess);

	FString Namespace = TEXT("accelbyte");
	FRegistry::Currency.GetCurrencyList(Namespace, THandler<TArray<FAccelByteModelsCurrencyList>>::CreateLambda([&bGetCurrencyListDone](const TArray<FAccelByteModelsCurrencyList>& Result)
		{
			bGetCurrencyListDone = true;
		}), EcommerceTestErrorHandler);
	WaitUntil(bGetCurrencyListDone, "Waiting for getting currency list...");
	AB_TEST_TRUE(bGetCurrencyListDone);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FECommerceTestGetRewardByRewardCode, "AccelByte.Tests.Ecommerce.Reward.1.GetRewardByRewardCode", AutomationFlagMaskEcommerce);
bool FECommerceTestGetRewardByRewardCode::RunTest(const FString& Parameters)
{
	bool bLoginWithDeviceId = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginWithDeviceId]()
	{
		bLoginWithDeviceId = true;
	}), EcommerceTestErrorHandler);
	WaitUntil(bLoginWithDeviceId, "Waiting for login...");
	AB_TEST_TRUE(bLoginWithDeviceId);
	
	bool bGetRewardByRewardCodeDone = false;
	FRegistry::Reward.GetRewardByRewardCode(*EcommerceTestExpectedVariable.RewardCreateInfo.RewardCode, THandler<FAccelByteModelsRewardInfo>::CreateLambda([&bGetRewardByRewardCodeDone](const FAccelByteModelsRewardInfo& Result)
	{
		bGetRewardByRewardCodeDone = true;
	}), EcommerceTestErrorHandler);
	WaitUntil(bGetRewardByRewardCodeDone, "Waiting for get reward by reward code...");
	AB_TEST_TRUE(bGetRewardByRewardCodeDone);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestGetRewardByRewardId, "AccelByte.Tests.Ecommerce.Reward.2.GetRewardByRewardId", AutomationFlagMaskEcommerce);
bool FEcommerceTestGetRewardByRewardId::RunTest(const FString& Parameters)
{
	bool bLoginWithDeviceId = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginWithDeviceId]()
	{
		bLoginWithDeviceId = true;
	}), EcommerceTestErrorHandler);
	WaitUntil(bLoginWithDeviceId, "Waiting for login...");
	AB_TEST_TRUE(bLoginWithDeviceId);

	bool bGetRewardByRewardIdDone = false;
	FRegistry::Reward.GetRewardByRewardId(*EcommerceTestExpectedVariable.RewardCreateInfo.RewardId, THandler<FAccelByteModelsRewardInfo>::CreateLambda([&bGetRewardByRewardIdDone](const FAccelByteModelsRewardInfo& Result)
	{
		bGetRewardByRewardIdDone = true;
	}), EcommerceTestErrorHandler);
	WaitUntil(bGetRewardByRewardIdDone, "Waiting for get reward by reward id...");
	AB_TEST_TRUE(bGetRewardByRewardIdDone);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestQueryRewards, "AccelByte.Tests.Ecommerce.Reward.3.QueryRewards", AutomationFlagMaskEcommerce);
bool FEcommerceTestQueryRewards::RunTest(const FString& Parameters)
{
	bool bLoginWithDeviceId = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginWithDeviceId]()
	{
		bLoginWithDeviceId = true;
	}), EcommerceTestErrorHandler);
	WaitUntil(bLoginWithDeviceId, "Waiting for login...");
	AB_TEST_TRUE(bLoginWithDeviceId);

	FString EventTopic;
	EventTopic = "statistic";
	bool bQueryRewardsStatisticTopicDone = false;
	FRegistry::Reward.QueryRewards(EventTopic, 0, 99, EAccelByteRewardListSortBy::NONE, THandler<FAccelByteModelsQueryReward>::CreateLambda([&bQueryRewardsStatisticTopicDone](const FAccelByteModelsQueryReward& Result)
	{
		bQueryRewardsStatisticTopicDone = true;
	}), EcommerceTestErrorHandler);
	WaitUntil(bQueryRewardsStatisticTopicDone, "Waiting for query statistic topic rewards...");
	AB_TEST_TRUE(bQueryRewardsStatisticTopicDone);

	EventTopic = "achievement";
	bool bQueryRewardsAchievementTopicDone = false;
	FRegistry::Reward.QueryRewards(EventTopic, 0, 99, EAccelByteRewardListSortBy::NONE, THandler<FAccelByteModelsQueryReward>::CreateLambda([&bQueryRewardsAchievementTopicDone](const FAccelByteModelsQueryReward& Result)
	{
		bQueryRewardsAchievementTopicDone = true;
	}), EcommerceTestErrorHandler);
	WaitUntil(bQueryRewardsAchievementTopicDone, "Waiting for query achievement topic rewards");
	AB_TEST_TRUE(bQueryRewardsAchievementTopicDone);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestSyncMobilePlatformFailed, "AccelByte.Tests.Ecommerce.J.SyncMobilePlatformFailed", AutomationFlagMaskEcommerce);
bool FEcommerceTestSyncMobilePlatformFailed::RunTest(const FString& Parameters)
{
	bool bMobileIAPConfigAlreadyExist = false;
	bool bMobileIAPCheckDone = false;
	FGoogleIAPConfig GoogleConfig;
	AdminGetGoogleIAPConfig(THandler<FGoogleIAPConfig>::CreateLambda([&bMobileIAPConfigAlreadyExist, &bMobileIAPCheckDone, &GoogleConfig](const FGoogleIAPConfig& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Apple IAP config already exits."));
		bMobileIAPConfigAlreadyExist = true;
		bMobileIAPCheckDone = true;
		GoogleConfig = Result;
		if (GoogleConfig.ApplicationName.IsEmpty()) UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Google IAP not configured properly."));
	}), FErrorHandler::CreateLambda(
	[&bMobileIAPConfigAlreadyExist, &bMobileIAPCheckDone](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log,TEXT("Google IAP config not exist. Create config from AP first!"));
		bMobileIAPConfigAlreadyExist = false;
		bMobileIAPCheckDone = true;
	}));
	WaitUntil(bMobileIAPCheckDone, "Waiting for IAP config check...");
	
	AB_TEST_TRUE(bMobileIAPCheckDone);

	//This negative test only run to check the endpoint and it's process
	//because all fields in request body come from Apple Apps or Google Play (mobile)

	bool bSyncDone = false;
	FAccelByteModelsPlatformSyncMobileGoogle SyncReqGoogle;
	SyncReqGoogle.OrderId = "test-OrderId";
	SyncReqGoogle.PackageName = "test-packageName";
	SyncReqGoogle.ProductId = "testProductId";
	SyncReqGoogle.PurchaseTime = 0;
	SyncReqGoogle.PurchaseToken = "test.PurchaseToken";
	SyncReqGoogle.Region = "ID";
	SyncReqGoogle.Language = "en";
	
	if (!GoogleConfig.ApplicationName.IsEmpty())
	{
		FRegistry::Entitlement.SyncMobilePlatformPurchaseGoogle(SyncReqGoogle, FVoidHandler::CreateLambda([&bSyncDone]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bSyncDone = true;
		}), FErrorHandler::CreateLambda([&bSyncDone](int32 ErrorCode, const FString& ErrorMessage)
		{
			bSyncDone = true;
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
		}));
		WaitUntil(bSyncDone, "Waiting for sync...");

		AB_TEST_TRUE(bSyncDone);
	}

	bMobileIAPConfigAlreadyExist = false;
	bMobileIAPCheckDone = false;
	FAppleIAPConfig AppleConfig;
	AdminGetAppleIAPConfig(THandler<FAppleIAPConfig>::CreateLambda([&bMobileIAPConfigAlreadyExist, &bMobileIAPCheckDone, &AppleConfig](const FAppleIAPConfig& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Apple IAP config already exits."));
		bMobileIAPConfigAlreadyExist = true;
		bMobileIAPCheckDone = true;
		AppleConfig = Result;
		if (AppleConfig.BundleId.IsEmpty()) UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Apple IAP not configured properly."));
	}), FErrorHandler::CreateLambda(
	[&bMobileIAPConfigAlreadyExist, &bMobileIAPCheckDone](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log,TEXT("Apple IAP config not exist. Create config from AP first!"));
		bMobileIAPConfigAlreadyExist = false;
		bMobileIAPCheckDone = true;
	}));
	WaitUntil(bMobileIAPCheckDone, "Waiting for IAP config check...");
	
	AB_TEST_TRUE(bMobileIAPCheckDone);
	
	bSyncDone = false;
	FAccelByteModelsPlatformSyncMobileApple SyncReqApple;
	SyncReqApple.ProductId = "testProductIdInvalid";
	SyncReqApple.TransactionId = "testTransactionIdInvalid";
	SyncReqApple.ReceiptData = "testReceiptDataInvalid";
	SyncReqApple.ExcludeOldTransactions = true;
	SyncReqApple.Region = "ID";
	SyncReqApple.Language = "en";

	if (!AppleConfig.BundleId.IsEmpty())
	{
		FRegistry::Entitlement.SyncMobilePlatformPurchaseApple(SyncReqApple, FVoidHandler::CreateLambda([&bSyncDone]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bSyncDone = true;
		}), FErrorHandler::CreateLambda([&bSyncDone](int32 ErrorCode, const FString& ErrorMessage)
		{
			bSyncDone = true;
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
		}));
		WaitUntil(bSyncDone, "Waiting for sync...");

		AB_TEST_TRUE(bSyncDone);
	}
	
	return true;
}

#if 0
/**
 * This test is disabled since we can not run it on jenkins.
 * But, prefer to let this test here to make it as an example to use Sync DLC items
 **/
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestSyncDLCItemFailed, "AccelByte.Tests.Ecommerce.J.SyncDLCItemFailed", AutomationFlagMaskEcommerce);
bool FEcommerceTestSyncDLCItemFailed::RunTest(const FString& Parameters)
{
	//This negative test only run to check the endpoint and it's process
	//because XstsToken come XBox one manager sample apps
	
	FRegistry::User.ForgetAllCredentials();
	
	FAccelByteModelsXBoxDLCSync XBoxDLCSyncToken;
	XBoxDLCSyncToken.XstsToken = "XBox Xsts token from XBox One Manager sample apps";
	
	bool bXBoxLoginSuccessful = false;
	bool bXBoxLoginDone = false;
	FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Live, XBoxDLCSyncToken.XstsToken, FVoidHandler::CreateLambda([&bXBoxLoginSuccessful, &bXBoxLoginDone]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("LoginWithXBoxAccount Success"));
		bXBoxLoginSuccessful = true;
		bXBoxLoginDone = true;
	}), FErrorHandler::CreateLambda([&bXBoxLoginDone](int32 ErrorCode, const FString& ErrorMessage)
	{
		UE_LOG(LogAccelByteEcommerceTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
		bXBoxLoginDone = true;
	}));
	WaitUntil(bXBoxLoginDone, "Waiting for Login...");

	AB_TEST_TRUE(bXBoxLoginDone);

	bool bGetDataSuccessful = false;
	FAccountUserData GetDataResult;
	FRegistry::User.GetData(
	THandler<FAccountUserData>::CreateLambda([&GetDataResult, &bGetDataSuccessful](const FAccountUserData& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("   Success"));
		bGetDataSuccessful = true;
		GetDataResult = Result;
	}),
	FErrorHandler::CreateLambda([&bGetDataSuccessful](int32 ErrorCode, const FString& ErrorMessage)
	{
		UE_LOG(LogAccelByteEcommerceTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
	}));

	WaitUntil(bGetDataSuccessful, "Waiting for Get Data...");
	AB_TEST_TRUE(bGetDataSuccessful);
	
	if (GetDataResult.Username.IsEmpty())
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("User login with headless 3rd party account"));
		XBoxDLCSyncToken.XstsToken = "";
	}
	
	bool bSyncDone = false;
	FRegistry::Entitlement.SyncXBoxDLC(XBoxDLCSyncToken, FVoidHandler::CreateLambda([&bSyncDone]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bSyncDone = true;
	}), FErrorHandler::CreateLambda([&bSyncDone](int32 ErrorCode, const FString& ErrorMessage)
	{
		bSyncDone = true;
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
	
	}));
	WaitUntil(bSyncDone, "Waiting for sync...");

	AB_TEST_TRUE(bSyncDone);
	
	return true;
}
#endif