// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HAL/FileManager.h"
#include "TestUtilities.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteOrderApi.h"
#include "Api/AccelByteCategoryApi.h"
#include "Api/AccelByteItemApi.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteWalletApi.h"
#include "Api/AccelByteEntitlementApi.h"
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
DECLARE_DELEGATE(FEcommerceUserInitializationSuccess);
DECLARE_DELEGATE(FEcommerceUserInitializationFail);

DECLARE_DELEGATE(FDeleteUserByIdSuccess);
static void DeleteUserByIdLobby(const FString& UserID, const FDeleteUserByIdSuccess& OnSuccess, const FErrorHandler& OnError);
void FlushHttpRequests();//defined in TestUtilities.cpp

const int32 AutomationFlagMaskEcommerce = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

FString SDKCurrencyCode = TEXT("SDKC");
TMap<FString, FString> LocalizationDescription;
FCurrencyCreateRequest CurrencyRequest
{
	SDKCurrencyCode,
	LocalizationDescription,
	SDKCurrencyCode,
	EAccelByteItemCurrencyType::VIRTUAL,
	0,
	-1,
	-1,
	-1
};
FStoreCreateRequest ArchiveStore
{
	"UE4-Store-Archive",
	"keep the original store",
	{"en"},
	{"US"},
	"en",
	"US"
};
FStoreCreateRequest TemporaryStore
{
	"UE4-Store-Temporary",
	"for SDK testing purpose",
	{"en"},
	{"US"},
	"en",
	"US"
};
const EcommerceExpectedVariable ExpectedVariable{
	"/UE4RootCategory",
	"/UE4RootCategory/UE4ChildCategory",
	"/UE4RootCategory/UE4ChildCategory/UE4GrandChildCategory",
	CurrencyRequest,
	"UE4RootItem",
	"UE4ChildItem",
	ArchiveStore,
	TemporaryStore,
};

FString ExpectedRootCategoryPath = ExpectedVariable.ExpectedRootCategoryPath;
FString ExpectedChildCategoryPath = ExpectedVariable.ExpectedChildCategoryPath;
FString ExpectedGrandChildCategoryPath = ExpectedVariable.ExpectedGrandChildCategoryPath;
FString ExpectedRootItemTitle = ExpectedVariable.ExpectedRootItemTitle; //"INGAMEITEM", 2 DOGECOIN, 0 Discount
FString ExpectedChildItemTitle = ExpectedVariable.ExpectedChildItemTitle;// Publisher's Currency, 0 USD, free, auto fulfilled, "COINS", "VIRTUAL", "DOGECOIN"
FString ExpectedCurrencyCode = ExpectedVariable.ExpectedCurrency.currencyCode;
FString ExpectedEntitlementId;

const auto EcommerceErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
{
	UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetCategorySuccess, "AccelByte.Tests.Ecommerce.B.GetCategory", AutomationFlagMaskEcommerce);
bool EcommerceGetCategorySuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();

#pragma region GetCategory

	FString CategoryPath = ExpectedChildCategoryPath;
	FString Language = TEXT("en");
	bool bGetCategorySuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetCategory"));
	FRegistry::Category.GetCategory(CategoryPath, Language, THandler<FAccelByteModelsCategoryInfo>::CreateLambda([&](const FAccelByteModelsCategoryInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetCategorySuccess = true;
	}), EcommerceErrorHandler);
	FlushHttpRequests();
	Waiting(bGetCategorySuccess,"Waiting for get category...");

#pragma endregion GetCategory

	check(bGetCategorySuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetRootCategoriesSuccess, "AccelByte.Tests.Ecommerce.B.GetRootCategories", AutomationFlagMaskEcommerce);
bool EcommerceGetRootCategoriesSuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();

#pragma region GetRootCategories

	FString Language = TEXT("en");
	bool bGetRootCategoriesSuccess = false;
	bool bExpectedRootCategoryFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetRootCategories"));
	FRegistry::Category.GetRootCategories(Language, THandler<TArray<FAccelByteModelsCategoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsCategoryInfo>& Results)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		for (int i = 0; i < Results.Num(); i++)
		{
			if (Results[i].CategoryPath == ExpectedRootCategoryPath)
			{
				bExpectedRootCategoryFound = true;
			}
		}
		bGetRootCategoriesSuccess = true;
	}), EcommerceErrorHandler);
	
	FlushHttpRequests();
	Waiting(bGetRootCategoriesSuccess,"Waiting for get root category...");

#pragma endregion GetRootCategories

	check(bGetRootCategoriesSuccess);
	check(bExpectedRootCategoryFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetChildCategoriesSuccess, "AccelByte.Tests.Ecommerce.B.GetChildCategories", AutomationFlagMaskEcommerce);
bool EcommerceGetChildCategoriesSuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();

#pragma region GetChildCategories

	FString Language = TEXT("en");
	bool bGetChildCategoriesSuccess = false;
	bool bExpectedChildCategoryFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetChildCategory"));
	FRegistry::Category.GetChildCategories(Language, ExpectedRootCategoryPath, THandler<TArray<FAccelByteModelsCategoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsCategoryInfo>& Results)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		for (int i = 0; i < Results.Num(); i++)
		{
			if (Results[i].CategoryPath == ExpectedChildCategoryPath)
			{
				bExpectedChildCategoryFound = true;
			}
		}
		bGetChildCategoriesSuccess = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetChildCategoriesSuccess,"Waiting for get child category...");

#pragma endregion GetChildCategories

	check(bGetChildCategoriesSuccess);
	check(bExpectedChildCategoryFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetDescendantCategoriesSuccess, "AccelByte.Tests.Ecommerce.B.GetDescendantCategories", AutomationFlagMaskEcommerce);
bool EcommerceGetDescendantCategoriesSuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();

#pragma region GetDescendantCategories
	FString Language = TEXT("en");
	bool bGetDescendantCategoriesSuccess = false;
	bool bExpectedDescendantCategoryFound1 = false;
	bool bExpectedDescendantCategoryFound2 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetDescendantCategories"));
	FRegistry::Category.GetDescendantCategories(Language, ExpectedRootCategoryPath, THandler<TArray<FAccelByteModelsCategoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsCategoryInfo>& Results)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		for (int i = 0; i < Results.Num(); i++)
		{
			if (Results[i].CategoryPath == ExpectedChildCategoryPath)
			{
				bExpectedDescendantCategoryFound1 = true;
			}
			else if (Results[i].CategoryPath == ExpectedGrandChildCategoryPath)
			{
				bExpectedDescendantCategoryFound2 = true;
			}
		}
		bGetDescendantCategoriesSuccess = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetDescendantCategoriesSuccess,"Waiting for get descendant category...");

#pragma endregion GetDescendantCategories

	check(bGetDescendantCategoriesSuccess);
	check(bExpectedDescendantCategoryFound1);
	check(bExpectedDescendantCategoryFound2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetItemsByCriteriaSuccess, "AccelByte.Tests.Ecommerce.C.GetItemsByCriteria", AutomationFlagMaskEcommerce);
bool EcommerceGetItemsByCriteriaSuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();

#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::INGAMEITEM;
	ItemCriteria.CategoryPath = ExpectedRootCategoryPath;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedRootItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		for (int i = 0; i < Result.Data.Num(); i++)
		{
			if (Result.Data[i].Title == ExpectedRootItemTitle)
			{
				bExpectedRootItemFound = true;
			}
		}
		bGetItemByCriteriaSuccess = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetItemByCriteriaSuccess,"Waiting for get items...");

#pragma endregion GetItemByCriteria

	check(bGetItemByCriteriaSuccess);
	check(bExpectedRootItemFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetItemSuccess, "AccelByte.Tests.Ecommerce.C.GetItem", AutomationFlagMaskEcommerce);
bool EcommerceGetItemSuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();
	FString ItemId = TEXT("");
	const FString Language = TEXT("en");
	const FString Region = TEXT("US");

#pragma region GetItemByCriteria
	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = Language;
	ItemCriteria.Region = Region;
	ItemCriteria.ItemType = EAccelByteItemType::INGAMEITEM;
	ItemCriteria.CategoryPath = ExpectedRootCategoryPath;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedRootItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		for (int i = 0; i < Result.Data.Num(); i++)
		{
			if (Result.Data[i].Title == ExpectedRootItemTitle)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found expected item"));
				ItemId = Result.Data[i].ItemId;
				bExpectedRootItemFound = true;
			}
		}
		bGetItemByCriteriaSuccess = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetItemByCriteriaSuccess,"Waiting for get items...");

#pragma endregion GetItemByCriteria

#pragma region GetItem

	bool bGetItemByIdSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemById"));
	FRegistry::Item.GetItemById(ItemId, Language, Region, THandler<FAccelByteModelsPopulatedItemInfo>::CreateLambda([&](const FAccelByteModelsPopulatedItemInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetItemByIdSuccess = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetItemByIdSuccess,"Waiting for get items...");

#pragma endregion GetItem

	check(bGetItemByCriteriaSuccess);
	check(bExpectedRootItemFound);
	check(bGetItemByIdSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetDiscountedItemSuccess, "AccelByte.Tests.Ecommerce.C.GetDiscountedItem", AutomationFlagMaskEcommerce);
bool EcommerceGetDiscountedItemSuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();
	FString ItemId = TEXT("");
	const FString Language = TEXT("en");
	const FString Region = TEXT("US");

#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = Language;
	ItemCriteria.Region = Region;
	ItemCriteria.ItemType = EAccelByteItemType::INGAMEITEM;
	ItemCriteria.CategoryPath = ExpectedRootCategoryPath;
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
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetItemByCriteriaSuccess, "Waiting for get items...");

#pragma endregion GetItemByCriteria

	check(bGetItemByCriteriaSuccess);
	check(bExpectedRootItemFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceSearchItemSuccess, "AccelByte.unTested.Ecommerce.C.SearchItem", AutomationFlagMaskEcommerce);
bool EcommerceSearchItemSuccess::RunTest(const FString& Parameters)
{
#pragma region SearchItem

	FString Language = TEXT("en");
	FString Region = TEXT("US");
	bool bSearchItemSuccess = false;
	bool bSearchedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("SearchItem"));
	FRegistry::Item.SearchItem(Language, ExpectedChildItemTitle, 0, 20, Region, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		for (int i = 0; i < Result.Data.Num(); i++)
		{
			if (Result.Data[i].Title == ExpectedChildItemTitle)
			{
				bSearchedItemFound = true;
			}
		}
		bSearchItemSuccess = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bSearchItemSuccess,"Waiting for search item...");

#pragma endregion SearchItem

	check(bSearchItemSuccess);
	check(bSearchedItemFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceCreateOrderSuccess, "AccelByte.Tests.Ecommerce.D1.CreateOrder", AutomationFlagMaskEcommerce);
bool EcommerceCreateOrderSuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();
	FAccelByteModelsItemInfo Item;

#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::COINS;
	ItemCriteria.CategoryPath = ExpectedChildCategoryPath;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria_VirtualCurrency"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    ChildFound: %d"), Result.Data.Num());
		for (int i = 0; i < Result.Data.Num(); i++)
		{
			if (ExpectedChildItemTitle.Contains(Result.Data[i].Title))
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found"));
				Item = Result.Data[i];
				bExpectedItemFound = true;
				bGetItemByCriteriaSuccess = true;
			}
		}
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetItemByCriteriaSuccess,"Waiting for get item...");

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
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bCreateNewOrderSuccess,"Waiting for new order created...");

#pragma endregion CreateOrder

#pragma region GetItemByCriteria_InGameItem

	ItemCriteria.CategoryPath = ExpectedRootCategoryPath;
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
			if (Result.Data[i].Title == ExpectedRootItemTitle)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found"));
				Item = Result.Data[i];
				bExpectedItemFound2 = true;
				bGetItemByCriteriaSuccess2 = true;
			}
		}
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetItemByCriteriaSuccess2,"Waiting for get item...");

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
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bCreateNewOrderSuccess2,"Waiting for new order created...");

#pragma endregion CreateOrder_InGameItem

	check(bGetItemByCriteriaSuccess);
	check(bExpectedItemFound);
	check(bCreateNewOrderSuccess);
	check(bGetItemByCriteriaSuccess2);
	check(bExpectedItemFound2);
	check(bCreateNewOrderSuccess2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetUserOrder, "AccelByte.Tests.Ecommerce.D2.GetUserOrder", AutomationFlagMaskEcommerce);
bool EcommerceGetUserOrder::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();
	FAccelByteModelsItemInfo Item;
	FString OrderNo = TEXT("");

#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::COINS;
	ItemCriteria.CategoryPath = ExpectedChildCategoryPath;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    ChildFound: %d"), Result.Data.Num());
		for (int i = 0; i < Result.Data.Num(); i++)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			if (ExpectedChildItemTitle.Contains(Result.Data[i].Title))
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found"));
				Item = Result.Data[i];
				bExpectedItemFound = true;
				bGetItemByCriteriaSuccess = true;
			}
		}
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetItemByCriteriaSuccess,"Waiting for get item...");

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
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bCreateNewOrderSuccess,"Waiting for new order created...");

#pragma endregion CreateOrder

#pragma region GetUserOrder

	bool bGetUserOrderSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserOrder"));
	FRegistry::Order.GetUserOrder(OrderNo, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](const FAccelByteModelsOrderInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetUserOrderSuccess = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetUserOrderSuccess,"Waiting for get user order...");

#pragma endregion GetUserOrder

	check(bGetItemByCriteriaSuccess);
	check(bExpectedItemFound);
	check(bCreateNewOrderSuccess);
	check(bGetUserOrderSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetUserOrderHistory, "AccelByte.Tests.Ecommerce.D3.GetUserOrderHistory", AutomationFlagMaskEcommerce);
bool EcommerceGetUserOrderHistory::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();
	FAccelByteModelsItemInfo Item;
	FString OrderNo = TEXT("");

#pragma region GetItemByCriteria

	FAccelByteModelsItemCriteria ItemCriteria;
	ItemCriteria.Language = TEXT("en");
	ItemCriteria.Region = TEXT("US");
	ItemCriteria.ItemType = EAccelByteItemType::COINS;
	ItemCriteria.CategoryPath = ExpectedChildCategoryPath;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FRegistry::Item.GetItemsByCriteria(ItemCriteria, 0 ,20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    ChildFound: %d"), Result.Data.Num());
		for (int i = 0; i < Result.Data.Num(); i++)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			if (ExpectedChildItemTitle.Contains(Result.Data[i].Title))
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found"));
				Item = Result.Data[i];
				bExpectedItemFound = true;
				bGetItemByCriteriaSuccess = true;
			}
		}
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetItemByCriteriaSuccess,"Waiting for get item...");

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
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bCreateNewOrderSuccess,"Waiting for new order created...");

#pragma endregion CreateOrder

#pragma region GetUserOrderHistory

	bool bGetUserOrderHistorySuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserOrder"));
	FRegistry::Order.GetUserOrderHistory(OrderNo, THandler<TArray<FAccelByteModelsOrderHistoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsOrderHistoryInfo>& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetUserOrderHistorySuccess = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetUserOrderHistorySuccess,"Waiting for get user order history...");

#pragma endregion GetUserOrderHistory

	check(bGetItemByCriteriaSuccess);
	check(bExpectedItemFound);
	check(bCreateNewOrderSuccess);
	check(bGetUserOrderHistorySuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetUserOrders, "AccelByte.Tests.Ecommerce.D4.GetUserOrders", AutomationFlagMaskEcommerce);
bool EcommerceGetUserOrders::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();

#pragma region GetUserOrders

	bool bGetUserOrdersSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserOrders"));
	FRegistry::Order.GetUserOrders(0, 20, THandler<FAccelByteModelsPagedOrderInfo>::CreateLambda([&](const FAccelByteModelsPagedOrderInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetUserOrdersSuccess = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetUserOrdersSuccess,"Waiting for get user order...");

#pragma endregion GetUserOrders

	check(bGetUserOrdersSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetWalletInfoByCurrencyCode, "AccelByte.Tests.Ecommerce.E1.GetWalletInfoByCurrencyCode", AutomationFlagMaskEcommerce);
bool EcommerceGetWalletInfoByCurrencyCode::RunTest(const FString& Parameters)
{
	bool bGetWalletSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetWalletInfoByCurrencyCode"));
	FRegistry::Wallet.GetWalletInfoByCurrencyCode(ExpectedCurrencyCode, THandler<FAccelByteModelsWalletInfo>::CreateLambda([&](const FAccelByteModelsWalletInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetWalletSuccess = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetWalletSuccess,"Waiting for get wallet...");

	check(bGetWalletSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceCreditUserWallet, "AccelByte.Tests.Ecommerce.E2.CreditUserWallet", AutomationFlagMaskEcommerce);
bool EcommerceCreditUserWallet::RunTest(const FString& Parameters)
{
#pragma region GetWalletInfo

	bool bGetWalletSuccess = false;
	FAccelByteModelsWalletInfo WalletInfo;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetWalletInfoByCurrencyCode"));
	FRegistry::Wallet.GetWalletInfoByCurrencyCode(ExpectedCurrencyCode, THandler<FAccelByteModelsWalletInfo>::CreateLambda([&WalletInfo, &bGetWalletSuccess](const FAccelByteModelsWalletInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		WalletInfo = Result;
		bGetWalletSuccess = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetWalletSuccess, "Waiting for get wallet...");

#pragma endregion GetWalletInfo

#pragma region ClientLogin

	bool bServerLoginWithClientCredentialsDone = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(
		FVoidHandler::CreateLambda([&bServerLoginWithClientCredentialsDone]()
	{
		bServerLoginWithClientCredentialsDone = true;
	}), EcommerceErrorHandler);
	FlushHttpRequests();
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
	FRegistry::ServerEcommerce.CreditUserWallet(FRegistry::Credentials.GetUserId(), SDKCurrencyCode, request, THandler<FAccelByteModelsWalletInfo>::CreateLambda([&WalletInfo, &bBalanceIncrease, &request, &bCreditWalletSuccess](const FAccelByteModelsWalletInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bBalanceIncrease = ((WalletInfo.Balance + request.Amount) == Result.Balance);
		bCreditWalletSuccess = true;
	}), EcommerceErrorHandler);
	FlushHttpRequests();
	Waiting(bCreditWalletSuccess, "Waiting for get wallet...");

#pragma endregion CreditUserWallet

	check(bGetWalletSuccess);
	check(bCreditWalletSuccess);
	check(bBalanceIncrease);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceEntitlementGrant, "AccelByte.Tests.Ecommerce.F1.EntitlementGrantSuccess", AutomationFlagMaskEcommerce);
bool EcommerceEntitlementGrant::RunTest(const FString& Parameters)
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
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetItemByCriteriaSuccess, "Waiting for get items...");
	check(bGetItemByCriteriaSuccess);
#pragma endregion GetItemByCriteria

#pragma region GrantEntitlement
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
		bClientLoginSuccess = true;
	}), EcommerceErrorHandler);
	FlushHttpRequests();
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
	}), EcommerceErrorHandler);
	FlushHttpRequests();
	Waiting(bGrantEntitlementSuccess, "Waiting for entitlement granted...");

	check(bGrantEntitlementSuccess);
#pragma endregion GrantEntitlement

#pragma region CheckEntitlementGranted
	bool bQueryEntitlementSuccess = false;
	FAccelByteModelsEntitlementPagingSlicedResult UserEntitlement;
	FRegistry::Entitlement.QueryUserEntitlements("", Items.Data[0].ItemId, 0, 20, THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda([&bQueryEntitlementSuccess, &UserEntitlement](const FAccelByteModelsEntitlementPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		UserEntitlement = Result;
		bQueryEntitlementSuccess = true;
	}), EcommerceErrorHandler, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);
	FlushHttpRequests();
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
	check(bEntitlementGranted);
#pragma endregion CheckEntitlementGranted
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceEntitlementGrantInvalid, "AccelByte.Tests.Ecommerce.F2.EntitlementGrantInvalidFailed", AutomationFlagMaskEcommerce);
bool EcommerceEntitlementGrantInvalid::RunTest(const FString& Parameters)
{

#pragma region GrantEntitlement
	bool bClientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
		bClientLoginSuccess = true;
	}), EcommerceErrorHandler);
	FlushHttpRequests();
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
		if ((ErrorCodes)ErrorCode != ErrorCodes::ItemNotFoundException)
		{
			UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
		}
		else
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
			bGrantEntitlementDone = true;
		}
	}));
	FlushHttpRequests();
	Waiting(bGrantEntitlementDone, "Waiting for entitlement granted...");

	check(bGrantEntitlementDone);
	check(!bGrantEntitlementSuccess);
#pragma endregion GrantEntitlement

#pragma region CheckEntitlementGranted
	bool bQueryEntitlementSuccess = false;
	FAccelByteModelsEntitlementPagingSlicedResult UserEntitlement;
	FRegistry::Entitlement.QueryUserEntitlements("", "Invalid", 0, 20, THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda([&bQueryEntitlementSuccess, &UserEntitlement](const FAccelByteModelsEntitlementPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		UserEntitlement = Result;
		bQueryEntitlementSuccess = true;
	}), EcommerceErrorHandler, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);
	FlushHttpRequests();
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
	check(!bEntitlementGranted);
#pragma endregion CheckEntitlementGranted
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceQueryUserEntitlement, "AccelByte.Tests.Ecommerce.F3.QueryUserEntitlementSuccess", AutomationFlagMaskEcommerce);
bool EcommerceQueryUserEntitlement::RunTest(const FString& Parameters)
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
		ExpectedEntitlementId = Result.Data[0].Id;
	}), EcommerceErrorHandler, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);

	FlushHttpRequests();
	Waiting(bQueryEntitlementSuccess, "Waiting for get user entitlement...");

	check(bQueryEntitlementSuccess);
	check(bQueryResultTrue);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceConsumeUserEntitlement, "AccelByte.Tests.Ecommerce.F4.ConsumeUserEntitlement", AutomationFlagMaskEcommerce);
bool EcommerceConsumeUserEntitlement::RunTest(const FString& Parameters)
{
#pragma region FirstConsumption

	bool bConsumeEntitlement1Success = false;
	bool bConsumeResult1True = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("FirstConsumeUserEntitlement"));
	FRegistry::Entitlement.ConsumeUserEntitlement(ExpectedEntitlementId, 1,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&bConsumeResult1True, &bConsumeEntitlement1Success](const FAccelByteModelsEntitlementInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bConsumeEntitlement1Success = true;
		bConsumeResult1True = (Result.Status == EAccelByteEntitlementStatus::CONSUMED);
	}), EcommerceErrorHandler);
	FlushHttpRequests();
	Waiting(bConsumeEntitlement1Success, "Waiting for consume user entitlement...");

#pragma endregion FirstConsumption

#pragma region SecondConsumption

	bool bConsumeEntitlement2Success = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("SecondConsumeUserEntitlement"));
	FRegistry::Entitlement.ConsumeUserEntitlement(ExpectedEntitlementId, 1,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&bConsumeEntitlement2Success](const FAccelByteModelsEntitlementInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bConsumeEntitlement2Success = true;
	}), FErrorHandler::CreateLambda([&bConsumeEntitlement2Success](int32 ErrorCode, const FString& ErrorMessage)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
		bConsumeEntitlement2Success = true;
	}));
	FlushHttpRequests();
	Waiting(bConsumeEntitlement1Success, "Waiting for consume user entitlement...");

#pragma endregion SecondConsumption

	check(bConsumeEntitlement1Success);
	check(bConsumeResult1True);
	check(bConsumeEntitlement2Success);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetUserEntitlementByEntitlementId, "AccelByte.Tests.Ecommerce.F5.GetUserEntitlementSuccess", AutomationFlagMaskEcommerce);
bool EcommerceGetUserEntitlementByEntitlementId::RunTest(const FString& Parameters)
{
	bool bGetEntitlementSuccess = false;
	bool bGetResultTrue = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserEntitlementSuccess"));
	FAccelByteModelsEntitlementInfo GetResult;
	FRegistry::Entitlement.GetUserEntitlementById(ExpectedEntitlementId,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&](const FAccelByteModelsEntitlementInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetEntitlementSuccess = true;
		bGetResultTrue = (Result.Id == ExpectedEntitlementId);
		GetResult = Result;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetEntitlementSuccess, "Waiting for get user entitlement...");

	check(bGetEntitlementSuccess);
	check(bGetResultTrue);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetUserEntitlementByEntitlementIdInvalid, "AccelByte.Tests.Ecommerce.F6.GetUserEntitlementInvalid", AutomationFlagMaskEcommerce);
bool EcommerceGetUserEntitlementByEntitlementIdInvalid::RunTest(const FString& Parameters)
{
	bool bGetEntitlementDone = false;
	bool bGetEntitlementError = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserEntitlementSuccess"));
	FRegistry::Entitlement.GetUserEntitlementById("Invalid",
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&](const FAccelByteModelsEntitlementInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetEntitlementDone = true;
	}), FErrorHandler::CreateLambda([&bGetEntitlementError, &bGetEntitlementDone](int32 ErrorCode, const FString& ErrorMessage)
	{
		bGetEntitlementDone = true;
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
		if ((ErrorCodes)ErrorCode == ErrorCodes::EntitlementIdNotFoundException)
		{
			bGetEntitlementError = true;
		}
	}));

	FlushHttpRequests();
	Waiting(bGetEntitlementDone, "Waiting for get user entitlement...");

	check(bGetEntitlementError);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceServerGetEntitlementByEntitlementId, "AccelByte.Tests.Ecommerce.F7.ServerGetEntitlementSuccess", AutomationFlagMaskEcommerce);
bool EcommerceServerGetEntitlementByEntitlementId::RunTest(const FString& Parameters)
{
	bool bLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bLoginSuccess]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
		bLoginSuccess = true;
	}), EcommerceErrorHandler);
	FlushHttpRequests();
	Waiting(bLoginSuccess, "Waiting for Login Creds...");

	bool bGetEntitlementSuccess = false;
	bool bGetResultTrue = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("ServerGetEntitlementSuccess"));
	FAccelByteModelsEntitlementInfo GetResult;
	FRegistry::ServerEcommerce.GetUserEntitlementById(ExpectedEntitlementId,
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&](const FAccelByteModelsEntitlementInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetEntitlementSuccess = true;
		bGetResultTrue = (Result.Id == ExpectedEntitlementId);
		GetResult = Result;
	}), EcommerceErrorHandler);
	FlushHttpRequests();
	Waiting(bGetEntitlementSuccess, "Waiting for get user entitlement...");

	check(bGetEntitlementSuccess);
	check(bGetResultTrue);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceServerGetEntitlementByEntitlementIdInvalid, "AccelByte.Tests.Ecommerce.F8.ServerGetEntitlementInvalid", AutomationFlagMaskEcommerce);
bool EcommerceServerGetEntitlementByEntitlementIdInvalid::RunTest(const FString& Parameters)
{
	bool bLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bLoginSuccess]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Login Success"));
		bLoginSuccess = true;
	}), EcommerceErrorHandler);
	FlushHttpRequests();
	Waiting(bLoginSuccess, "Waiting for Login Creds...");

	bool bGetEntitlementDone = false;
	bool bGetEntitlementError = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserEntitlementSuccess"));
	FRegistry::ServerEcommerce.GetUserEntitlementById("Invalid",
		THandler<FAccelByteModelsEntitlementInfo>::CreateLambda([&](const FAccelByteModelsEntitlementInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetEntitlementDone = true;
	}), FErrorHandler::CreateLambda([&bGetEntitlementError, &bGetEntitlementDone](int32 ErrorCode, const FString& ErrorMessage)
	{
		bGetEntitlementDone = true;
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
		if ((ErrorCodes)ErrorCode == ErrorCodes::EntitlementIdNotFoundException)
		{
			bGetEntitlementError = true;
		}
	}));

	FlushHttpRequests();
	Waiting(bGetEntitlementDone, "Waiting for get user entitlement...");

	check(bGetEntitlementError);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceSetup, "AccelByte.Tests.Ecommerce.A.Setup", AutomationFlagMaskEcommerce);
bool EcommerceSetup::RunTest(const FString& Parameters)
{

	bool bSetupSuccess = false;
	SetupEcommerce(ExpectedVariable, FSimpleDelegate::CreateLambda([&]() { bSetupSuccess = true; }), EcommerceErrorHandler);
	FlushHttpRequests();
	Waiting(bSetupSuccess,"Waiting for setup ecommerce...");
	check(bSetupSuccess);

	bool bUserLoginSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoginSuccess]()
	{ 
		bUserLoginSuccess = true;
	}), EcommerceErrorHandler);
	FlushHttpRequests();
	Waiting(bUserLoginSuccess,"Waiting for Login...");
	check(bUserLoginSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceTearDown, "AccelByte.Tests.Ecommerce.Z.Teardown", AutomationFlagMaskEcommerce);
bool EcommerceTearDown::RunTest(const FString& Parameters)
{
	bool bTearDownSuccess = false;
	TearDownEcommerce(ExpectedVariable, FSimpleDelegate::CreateLambda([&]() { bTearDownSuccess = true; }), EcommerceErrorHandler);
	FlushHttpRequests();
	Waiting(bTearDownSuccess,"Waiting for teardown...");
	check(bTearDownSuccess)

	float LastTime = 0.0f;

#pragma region DeleteUser

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bDeleteDone,"Waiting for user deletion...");

#pragma endregion DeleteUser

	return bDeleteSuccessful;
}
