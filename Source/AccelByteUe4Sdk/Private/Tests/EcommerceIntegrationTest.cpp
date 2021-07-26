// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "TestUtilities.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteOrderApi.h"
#include "Api/AccelByteCategoryApi.h"
#include "Api/AccelByteItemApi.h"
#include "Api/AccelByteWalletApi.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Api/AccelByteFulfillmentApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerEcommerceApi.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::Category;
using AccelByte::Api::Item;
using AccelByte::Api::Order;
using AccelByte::Api::Wallet;
using AccelByte::Api::Entitlement;

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
EcommerceExpectedVariable EcommerceTestExpectedVariable{
	"/UE4RootCategory",
	"/UE4RootCategory/UE4ChildCategory",
	"/UE4RootCategory/UE4ChildCategory/UE4GrandChildCategory",
	EcommerceTestCurrencyRequest,
	"", // CampaignCode
	"UE4CampaignTest", // CampaignName
	"UE4ExpiredCampaignTest", // ExpiredCaimpaignName
	"UE4NotStartedCampaignTest", // NotStartedCampaignName
	"UE4RedeemableItem", // RedeemableItemTitle
	"UE4RootItem",
	"UE4ChildItem",
	EcommerceTestArchiveStore,
	EcommerceTestTemporaryStore,
	1000, // LootCoin Quantity
	EcommerceTestEmptyItem, // RedeemableItem Result
	EcommerceTestEmptyItem, // LootItem Result
	EcommerceTestEmptyItem, // LootCoin Result
	EcommerceTestEmptyItem, // Lootbox Result
	EcommerceTestEmptyItem, // Bundle Result
	EcommerceTestEmptyCampaignInfo, // Campaign Result
	EcommerceTestEmptyCampaignInfo, // ExpiredCampaign Result
	EcommerceTestEmptyCampaignInfo, // NotStartedCampaign Result
	EcommerceTestEmptyCodeInfo, // CodeInfo Result
	EcommerceTestEmptyCodeInfo, // ExpiredCodeInfo Result
	EcommerceTestEmptyCodeInfo // NotStartedCodeInfo Result
};

FString EcommerceTestExpectedRootCategoryPath = EcommerceTestExpectedVariable.ExpectedRootCategoryPath;
FString EcommerceTestExpectedChildCategoryPath = EcommerceTestExpectedVariable.ExpectedChildCategoryPath;
FString EcommerceTestExpectedGrandChildCategoryPath = EcommerceTestExpectedVariable.ExpectedGrandChildCategoryPath;
FString EcommerceTestExpectedRootItemTitle = EcommerceTestExpectedVariable.ExpectedRootItemTitle; //"INGAMEITEM", 2 DOGECOIN, 0 Discount
FString EcommerceTestExpectedChildItemTitle = EcommerceTestExpectedVariable.ExpectedChildItemTitle;// Publisher's Currency, 0 USD, free, auto fulfilled, "COINS", "VIRTUAL", "DOGECOIN"
FString EcommerceTestExpectedCurrencyCode = EcommerceTestExpectedVariable.ExpectedCurrency.currencyCode;
FString EcommerceTestExpectedEntitlementId;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestSetup, "AccelByte.Tests.Ecommerce.A.Setup", AutomationFlagMaskEcommerce);
bool FEcommerceTestSetup::RunTest(const FString& Parameters)
{

	bool bSetupSuccess = false;
	SetupEcommerce(EcommerceTestExpectedVariable, FSimpleDelegate::CreateLambda([&]() { bSetupSuccess = true; }), EcommerceTestErrorHandler);
	Waiting(bSetupSuccess, "Waiting for setup ecommerce...");
	AB_TEST_TRUE(bSetupSuccess);

	bool bUserLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginSuccess]()
		{
			bUserLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	Waiting(bUserLoginSuccess, "Waiting for Login...");
	AB_TEST_TRUE(bUserLoginSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestTearDown, "AccelByte.Tests.Ecommerce.Z.Teardown", AutomationFlagMaskEcommerce);
bool FEcommerceTestTearDown::RunTest(const FString& Parameters)
{
	bool bTearDownSuccess = false;
	TearDownEcommerce(EcommerceTestExpectedVariable, FSimpleDelegate::CreateLambda([&]() { bTearDownSuccess = true; }), EcommerceTestErrorHandler);
	Waiting(bTearDownSuccess, "Waiting for teardown...");
	AB_TEST_TRUE(bTearDownSuccess)

#pragma region DeleteUser

		bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), EcommerceTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for user deletion...");

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
	Waiting(bGetCategorySuccess, "Waiting for get category...");

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

	Waiting(bGetRootCategoriesSuccess, "Waiting for get root category...");

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

	Waiting(bGetChildCategoriesSuccess, "Waiting for get child category...");

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

	Waiting(bGetDescendantCategoriesSuccess, "Waiting for get descendant category...");

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

	Waiting(bGetItemByCriteriaSuccess, "Waiting for get items...");

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

	Waiting(bGetItemByCriteriaSuccess, "Waiting for get items...");

#pragma endregion GetItemByCriteria

#pragma region GetItem

	bool bGetItemByIdSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemById"));
	FRegistry::Item.GetItemById(ItemId, Language, Region, THandler<FAccelByteModelsPopulatedItemInfo>::CreateLambda([&](const FAccelByteModelsPopulatedItemInfo& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetItemByIdSuccess = true;
		}), EcommerceTestErrorHandler);

	Waiting(bGetItemByIdSuccess, "Waiting for get items...");

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

	Waiting(bGetItemByCriteriaSuccess, "Waiting for get items...");

#pragma endregion GetItemByCriteria

	AB_TEST_TRUE(bGetItemByCriteriaSuccess);
	AB_TEST_TRUE(bExpectedRootItemFound);
	AB_TEST_NOT_EQUAL(ItemId, FString{});
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestSearchItem, "AccelByte.unTested.Ecommerce.C.SearchItem", AutomationFlagMaskEcommerce);
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

	Waiting(bSearchItemSuccess, "Waiting for search item...");

#pragma endregion SearchItem

	AB_TEST_TRUE(bSearchItemSuccess);
	AB_TEST_TRUE(bSearchedItemFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestCreateDistributionReceiver, "AccelByte.Tests.Ecommerce.D.CreateDistributionReceiver", AutomationFlagMaskEcommerce);
bool FEcommerceTestCreateDistributionReceiver::RunTest(const FString& Parameters)
{
#pragma region CreateDistributionReceiver
	FString const ExtUserId = "55n8dj2jqgr5s3ryg9cpm4bm7k7vr33t";

	FAccelByteModelsAttributes Attributes;
	Attributes.ServerId = "70391cb5af52427e896e05290bc65832";
	Attributes.ServerName = "testserver";
	Attributes.CharacterId = "32aaf2eabcbb45d096e06be8a4584320";
	Attributes.CharacterName = "newcharacter";

	bool bCreateDistributionSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Create Distribution Receiver"));
	FRegistry::Entitlement.CreateDistributionReceiver(ExtUserId, Attributes, FVoidHandler::CreateLambda([&bCreateDistributionSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bCreateDistributionSuccess = true;
		}), EcommerceTestErrorHandler);

	Waiting(bCreateDistributionSuccess, "Waiting for create distribution receiver...");

#pragma endregion CreateDistributionReceiver
#pragma region GetDistributionReceiver
	bool bGetDistributionSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Get Distribution Receiver"));
	FRegistry::Entitlement.GetDistributionReceiver(FRegistry::Credentials.GetNamespace(), FRegistry::Credentials.GetUserId(), THandler<TArray<FAccelByteModelsDistributionReceiver>>::CreateLambda([&bGetDistributionSuccess](const TArray<FAccelByteModelsDistributionReceiver>& Result)
		{
			for (auto Data : Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("\n\nAttributes:\nServer Id:%s\nServer Name:%s\nCharacter Id:%s\nCharacter Name:%s\nExtUserId:%s\nUser Id:%s\nNamespace:%s\n"),
					*Data.Attributes.ServerId,
					*Data.Attributes.ServerName,
					*Data.Attributes.CharacterId,
					*Data.Attributes.CharacterName,
					*Data.ExtUserId,
					*Data.UserId,
					*Data.Namespace);
			}
			bGetDistributionSuccess = true;
		}), EcommerceTestErrorHandler);

	Waiting(bGetDistributionSuccess, "Waiting for get distribution receiver...");
#pragma endregion GetDistributionReceiver
#pragma region DeleteDistributionReceiver
	bool bDeleteDistributionSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Delete Distribution Receiver"));
	FRegistry::Entitlement.DeleteDistributionReceiver(ExtUserId, FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDistributionSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bDeleteDistributionSuccess = true;
		}), EcommerceTestErrorHandler);

	Waiting(bDeleteDistributionSuccess, "Waiting for delete distribution receiver...");
#pragma endregion DeleteDistributionReceiver

	AB_TEST_TRUE(bCreateDistributionSuccess);
	AB_TEST_TRUE(bGetDistributionSuccess);
	AB_TEST_TRUE(bDeleteDistributionSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FEcommerceTestUpdateDistributionReceiver, "AccelByte.Tests.Ecommerce.D.UpdateDistributionReceiver", AutomationFlagMaskEcommerce);
bool FEcommerceTestUpdateDistributionReceiver::RunTest(const FString& Parameters)
{
#pragma region CreateDistributionReceiver
	FString const ExtUserId = "55n8dj2jqgr5s3ryg9cpm4bm7k7vr33t";

	FAccelByteModelsAttributes Attributes;
	Attributes.ServerId = "70391cb5af52427e896e05290bc65832";
	Attributes.ServerName = "testserver";
	Attributes.CharacterId = "32aaf2eabcbb45d096e06be8a4584320";
	Attributes.CharacterName = "newcharacter";

	bool bCreateDistributionSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Create Distribution Receiver"));
	FRegistry::Entitlement.CreateDistributionReceiver(ExtUserId, Attributes, FVoidHandler::CreateLambda([&bCreateDistributionSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bCreateDistributionSuccess = true;
		}), EcommerceTestErrorHandler);

	Waiting(bCreateDistributionSuccess, "Waiting for create distribution receiver...");

#pragma endregion CreateDistributionReceiver
#pragma region GetDistributionReceiver
	bool bGetDistributionSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Get Distribution Receiver"));
	FRegistry::Entitlement.GetDistributionReceiver(FRegistry::Credentials.GetNamespace(), FRegistry::Credentials.GetUserId(), THandler<TArray<FAccelByteModelsDistributionReceiver>>::CreateLambda([&bGetDistributionSuccess](const TArray<FAccelByteModelsDistributionReceiver>& Result)
		{
			for (auto Data : Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("\n\nAttributes:\nServer Id:%s\nServer Name:%s\nCharacter Id:%s\nCharacter Name:%s\nExtUserId:%s\nUser Id:%s\nNamespace:%s\n"),
					*Data.Attributes.ServerId,
					*Data.Attributes.ServerName,
					*Data.Attributes.CharacterId,
					*Data.Attributes.CharacterName,
					*Data.ExtUserId,
					*Data.UserId,
					*Data.Namespace);
			}
			bGetDistributionSuccess = true;
		}), EcommerceTestErrorHandler);

	Waiting(bGetDistributionSuccess, "Waiting for get distribution receiver...");
#pragma endregion GetDistributionReceiver
#pragma region UpdateDistributionReceiver
	Attributes.ServerId = "70391cb5af52427e896e05290bc65831";
	Attributes.ServerName = "updatedtestserver";
	Attributes.CharacterId = "32aaf2eabcbb45d096e06be8a4584321";
	Attributes.CharacterName = "updatednewcharacter";

	bool bUpdateDistributionSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Update Distribution Receiver"));
	FRegistry::Entitlement.UpdateDistributionReceiver(ExtUserId, Attributes, FVoidHandler::CreateLambda([&bUpdateDistributionSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bUpdateDistributionSuccess = true;
		}), EcommerceTestErrorHandler);

	Waiting(bUpdateDistributionSuccess, "Waiting for update distribution receiver...");

#pragma endregion UpdateDistributionReceiver
#pragma region GetDistributionReceiver
	bGetDistributionSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Get Distribution Receiver"));
	FRegistry::Entitlement.GetDistributionReceiver(FRegistry::Credentials.GetNamespace(), FRegistry::Credentials.GetUserId(), THandler<TArray<FAccelByteModelsDistributionReceiver>>::CreateLambda([&bGetDistributionSuccess](const TArray<FAccelByteModelsDistributionReceiver>& Result)
		{
			for (auto Data : Result)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("\n\nAttributes:\nServer Id:%s\nServer Name:%s\nCharacter Id:%s\nCharacter Name:%s\nExtUserId:%s\nUser Id:%s\nNamespace:%s\n"),
					*Data.Attributes.ServerId,
					*Data.Attributes.ServerName,
					*Data.Attributes.CharacterId,
					*Data.Attributes.CharacterName,
					*Data.ExtUserId,
					*Data.UserId,
					*Data.Namespace);
			}
			bGetDistributionSuccess = true;
		}), EcommerceTestErrorHandler);

	Waiting(bGetDistributionSuccess, "Waiting for get distribution receiver...");
#pragma endregion GetDistributionReceiver
#pragma region DeleteDistributionReceiver
	bool bDeleteDistributionSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Delete Distribution Receiver"));
	FRegistry::Entitlement.DeleteDistributionReceiver(ExtUserId, FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDistributionSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bDeleteDistributionSuccess = true;
		}), EcommerceTestErrorHandler);

	Waiting(bDeleteDistributionSuccess, "Waiting for delete distribution receiver...");
#pragma endregion DeleteDistributionReceiver

	AB_TEST_TRUE(bCreateDistributionSuccess);
	AB_TEST_TRUE(bGetDistributionSuccess);
	AB_TEST_TRUE(bUpdateDistributionSuccess);
	AB_TEST_TRUE(bDeleteDistributionSuccess);

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

	Waiting(bGetItemByCriteriaSuccess, "Waiting for get item...");

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

	Waiting(bCreateNewOrderSuccess, "Waiting for new order created...");

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

	Waiting(bGetItemByCriteriaSuccess2, "Waiting for get item...");

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

	Waiting(bCreateNewOrderSuccess2, "Waiting for new order created...");

#pragma endregion CreateOrder_InGameItem

	AB_TEST_TRUE(bGetItemByCriteriaSuccess);
	AB_TEST_TRUE(bExpectedItemFound);
	AB_TEST_TRUE(bCreateNewOrderSuccess);
	AB_TEST_TRUE(bGetItemByCriteriaSuccess2);
	AB_TEST_TRUE(bExpectedItemFound2);
	AB_TEST_TRUE(bCreateNewOrderSuccess2);
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

	Waiting(bGetItemByCriteriaSuccess, "Waiting for get item...");

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

	Waiting(bCreateNewOrderSuccess, "Waiting for new order created...");

#pragma endregion CreateOrder

#pragma region GetUserOrder

	bool bGetUserOrderSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserOrder"));
	FRegistry::Order.GetUserOrder(OrderNo, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](const FAccelByteModelsOrderInfo& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetUserOrderSuccess = true;
		}), EcommerceTestErrorHandler);

	Waiting(bGetUserOrderSuccess, "Waiting for get user order...");

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

	Waiting(bGetItemByCriteriaSuccess, "Waiting for get item...");

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

	Waiting(bCreateNewOrderSuccess, "Waiting for new order created...");

#pragma endregion CreateOrder

#pragma region GetUserOrderHistory

	bool bGetUserOrderHistorySuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserOrder"));
	FRegistry::Order.GetUserOrderHistory(OrderNo, THandler<TArray<FAccelByteModelsOrderHistoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsOrderHistoryInfo>& Result)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			bGetUserOrderHistorySuccess = true;
		}), EcommerceTestErrorHandler);

	Waiting(bGetUserOrderHistorySuccess, "Waiting for get user order history...");

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

	Waiting(bGetUserOrdersSuccess, "Waiting for get user order...");

#pragma endregion GetUserOrders

	AB_TEST_TRUE(bGetUserOrdersSuccess);
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

	Waiting(bGetWalletSuccess, "Waiting for get wallet...");

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

	Waiting(bGetWalletSuccess, "Waiting for get wallet...");

#pragma endregion GetWalletInfo

#pragma region ClientLogin

	bool bServerLoginWithClientCredentialsDone = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(
		FVoidHandler::CreateLambda([&bServerLoginWithClientCredentialsDone]()
			{
				bServerLoginWithClientCredentialsDone = true;
			}), EcommerceTestErrorHandler);
	Waiting(bServerLoginWithClientCredentialsDone, "Server Login With Client Credentials");

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
	Waiting(bCreditWalletSuccess, "Waiting for get wallet...");

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

	Waiting(bGetWalletSuccess, "Waiting for get wallet...");
	AB_TEST_TRUE(bGetWalletSuccess)
#pragma endregion GetWalletInfo

#pragma region ServerLogin

		bool bServerLoginWithClientCredentialsDone = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(
		FVoidHandler::CreateLambda([&bServerLoginWithClientCredentialsDone]()
			{
				bServerLoginWithClientCredentialsDone = true;
			}), EcommerceTestErrorHandler);
	Waiting(bServerLoginWithClientCredentialsDone, "Server Login With Client Credentials");

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
	Waiting(bCreditWalletSuccess, "Waiting for get wallet...");
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
	Waiting(bDebitWalletSuccess, "Waiting for debiting the user wallet");

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

	Waiting(bGetItemByCriteriaSuccess, "Waiting for get items...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess);
#pragma endregion GetItemByCriteria

#pragma region GrantEntitlement
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bClientLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	Waiting(bClientLoginSuccess, "Waiting for Client Login...");

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
	Waiting(bGrantEntitlementSuccess, "Waiting for entitlement granted...");

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
	Waiting(bQueryEntitlementSuccess, "Waiting for Query User's Entitlement...");

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
	Waiting(bClientLoginSuccess, "Waiting for Client Login...");

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
					UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				}
				else
				{
					UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
					bGrantEntitlementDone = true;
				}
			}));
	Waiting(bGrantEntitlementDone, "Waiting for entitlement granted...");

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
	Waiting(bQueryEntitlementSuccess, "Waiting for Query User's Entitlement...");

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

	Waiting(bQueryEntitlementSuccess, "Waiting for get user entitlement...");

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
	Waiting(bConsumeEntitlement1Success, "Waiting for consume user entitlement...");

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
	Waiting(bConsumeEntitlement2Success, "Waiting for consume user entitlement...");

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

	Waiting(bGetEntitlementSuccess, "Waiting for get user entitlement...");

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

	Waiting(bGetEntitlementDone, "Waiting for get user entitlement...");

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
	Waiting(bLoginSuccess, "Waiting for Login Creds...");

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
	Waiting(bGetEntitlementSuccess, "Waiting for get user entitlement...");

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
	Waiting(bLoginSuccess, "Waiting for Login Creds...");

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
	Waiting(bGetEntitlementSuccess, "Waiting for get user entitlement...");

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
	Waiting(bLoginSuccess, "Waiting for Login Creds...");

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
	Waiting(bGetEntitlementSuccess, "Waiting for get user entitlement...");

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
	Waiting(bLoginSuccess, "Waiting for Login Creds...");

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

	Waiting(bGetEntitlementDone, "Waiting for get user entitlement...");

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
	Waiting(bUserLoginSuccess, "Waiting for Login...");
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

	Waiting(bGetItemByCriteriaSuccess, "Waiting for get items...");
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

	Waiting(bGetItemByCriteriaSuccess2, "Waiting for get items...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess2);
#pragma endregion GetItemByCriteria

#pragma region GrantEntitlement
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bClientLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	Waiting(bClientLoginSuccess, "Waiting for Client Login...");

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
	ItemIdsGranted.Add(Items2.Data[0].ItemId);

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
	Waiting(bGrantEntitlementSuccess, "Waiting for entitlement granted...");

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
	Waiting(bQueryEntitlementSuccess, "Waiting for Query User's Entitlement...");

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
	Waiting(bUserLoginSuccess, "Waiting for Login...");
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

	Waiting(bGetItemByCriteriaSuccess, "Waiting for get items...");
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

	Waiting(bGetItemByCriteriaSuccess2, "Waiting for get items...");
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
	Waiting(bGrantEntitlementSuccess, "Waiting for entitlement granted...");

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
	Waiting(bIsConsumed, "Waiting to be consumed");

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
	Waiting(bUserLoginSuccess, "Waiting for Login...");
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

	Waiting(bGetItemByCriteriaSuccess, "Waiting for get items...");
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

	Waiting(bGetItemByCriteriaSuccess2, "Waiting for get items...");
	AB_TEST_TRUE(bGetItemByCriteriaSuccess2);
#pragma endregion GetItemByCriteria

#pragma region GrantEntitlement
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
			bClientLoginSuccess = true;
		}), EcommerceTestErrorHandler);
	Waiting(bClientLoginSuccess, "Waiting for Client Login...");

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
	Waiting(bGrantEntitlementSuccess, "Waiting for entitlement granted...");

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
	Waiting(bIsEnable, "Waiting to enable the entitlement");
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
	Waiting(bIsDisabled, "Waiting to disable the entitlement");
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
	Waiting(bIsRevoked, "Waiting to revoke the entitlement");
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
	Waiting(bUserLoginSuccess, "Waiting for Login...");
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
	Waiting(bIsDisabledFailed, "Waiting to disable the entitlement");
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
	Waiting(bIsEnableFailed, "Waiting to enable the entitlement");
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
	Waiting(bIsRevokedFailed, "Waiting to revoke the entitlement");
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
	Waiting(bUserLoginSuccess, "Waiting for Login...");
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

	Waiting(bGetItemByCriteriaSuccess, "Waiting for get items...");
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

	Waiting(bGetItemByCriteriaSuccess2, "Waiting for get items...");
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
	Waiting(bGrantEntitlementSuccess, "Waiting for entitlement granted...");

	AB_TEST_TRUE(bGrantEntitlementSuccess);
#pragma endregion GrantEntitlement

#pragma region RevokeEntitlements
	bool bIsRevokeSuccess = false;
	FRegistry::ServerEcommerce.RevokeUserEntitlements(FRegistry::Credentials.GetUserId(), GrantedEntitlementResultIds,
		THandler<FAccelByteModelsBulkRevokeEntitlements>::CreateLambda([&bIsRevokeSuccess](const FAccelByteModelsBulkRevokeEntitlements Response)
			{
				bIsRevokeSuccess = true;
			}), EcommerceTestErrorHandler);
	Waiting(bIsRevokeSuccess, "Waiting for revoking the entitlements");

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
	Waiting(bServerQueryEntitlementSuccess, "Waiting for Query User's Entitlement...");
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
	Waiting(bRedeemCodeSuccess, "Waiting for redeem code...");

	AB_TEST_EQUAL(fulfillmentResult.EntitlementSummaries[0].ItemId, EcommerceTestExpectedVariable.redeemableItem.itemId);

	FAccelByteModelsEntitlementInfo getUserEntitlementByIdResult;
	bool bGetEntitlementSuccess = false;
	FRegistry::Entitlement.GetUserEntitlementById(fulfillmentResult.EntitlementSummaries[0].Id, THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&bGetEntitlementSuccess, &getUserEntitlementByIdResult](const FAccelByteModelsEntitlementInfo& Result)
		{
			getUserEntitlementByIdResult = Result;
			bGetEntitlementSuccess = true;
		}), EcommerceTestErrorHandler);
	Waiting(bGetEntitlementSuccess, "Waiting for get entitlement...");

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
	Waiting(bRedeemCodeDone, "Waiting for redeem code...");

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
	Ecommerce_Campaign_Update(
		EcommerceTestExpectedVariable.campaignResult.id,
		CampaignUpdate,
		THandler<FCampaignInfo>::CreateLambda([&bUpdateCampaignSuccess, &updateCampaignResult](const FCampaignInfo& Result)
			{
				updateCampaignResult = Result;
				bUpdateCampaignSuccess = true;
			}), EcommerceTestErrorHandler);
	Waiting(bUpdateCampaignSuccess, "Waiting for update campaign...");

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
	Waiting(bRedeemCodeDone, "Waiting for redeem code...");

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
	Ecommerce_CampaignCode_Disable(
		EcommerceTestExpectedVariable.codeInfo.value,
		THandler<FCodeInfo>::CreateLambda([&bDisableCodeSuccess, &disableCodeResult](const FCodeInfo& Result)
			{
				disableCodeResult = Result;
				bDisableCodeSuccess = true;
			}), EcommerceTestErrorHandler);
	Waiting(bDisableCodeSuccess, "Waiting for disabling code...");

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
	Waiting(bRedeemCodeDone, "Waiting for redeem code...");

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
	Waiting(bRedeemCodeDone, "Waiting for redeem code...");

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
	Waiting(bRedeemCodeDone, "Waiting for redeem code...");

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
	Waiting(bRedeemCodeDone, "Waiting for redeem code...");

	AB_TEST_FALSE(bRedeemCodeSuccess);
	AB_TEST_TRUE(bRedeemCodeDone);
	AB_TEST_EQUAL(ErrorCode, static_cast<int32>(ErrorCodes::CodeRedeemptionAlreadyEndedException));
	return true;
}