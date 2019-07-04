// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "AccelByteOrderApi.h"
#include "AccelByteCategoryApi.h"
#include "AccelByteItemApi.h"
#include "AccelByteUserApi.h"
#include "AccelByteWalletApi.h"
#include "AccelByteEntitlementApi.h"
#include "AccelByteRegistry.h"
#include "FileManager.h"
#include "AccelByteUserApi.h"
#include "TestUtilities.h"

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

TMap<FString, FString> LocalizationDescription;
FCurrencyCreateRequest CurrencyRequest
{
	TEXT("UE4_SdkCoin"),
	LocalizationDescription,
	TEXT("UE4SDKC"),
	ECurrencyType::VIRTUAL,
	0,
	-1,
	-1,
	-1
};
FStoreCreateRequest ArchiveStore
{
	"UE4_Store_Archive",
	"keep the original store",
	{"en"},
	{"US"},
	"en",
	"US"
};
FStoreCreateRequest TemporaryStore
{
	"UE4_Store_Temporary",
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
	TemporaryStore
};

FString ExpectedRootCategoryPath = ExpectedVariable.ExpectedRootCategoryPath;
FString ExpectedChildCategoryPath = ExpectedVariable.ExpectedChildCategoryPath;
FString ExpectedGrandChildCategoryPath = ExpectedVariable.ExpectedGrandChildCategoryPath;
FString ExpectedRootItemTitle = ExpectedVariable.ExpectedRootItemTitle; //"INGAMEITEM", 2 DOGECOIN, 0 Discount
FString ExpectedChildItemTitle = ExpectedVariable.ExpectedChildItemTitle;// Publisher's Currency, 0 USD, free, auto fulfilled, "COINS", "VIRTUAL", "DOGECOIN"
FString ExpectedCurrencyCode = ExpectedVariable.ExpectedCurrency.currencyCode;

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
	FRegistry::Category.GetCategory(CategoryPath, Language, THandler<FAccelByteModelsFullCategoryInfo>::CreateLambda([&](const FAccelByteModelsFullCategoryInfo& Result)
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
	FRegistry::Category.GetRootCategories(Language, THandler<TArray<FAccelByteModelsFullCategoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsFullCategoryInfo>& Results)
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
	FRegistry::Category.GetChildCategories(Language, ExpectedRootCategoryPath, THandler<TArray<FAccelByteModelsFullCategoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsFullCategoryInfo>& Results)
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
	FRegistry::Category.GetDescendantCategories(Language, ExpectedRootCategoryPath, THandler<TArray<FAccelByteModelsFullCategoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsFullCategoryInfo>& Results)
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

	FString Language = TEXT("en");
	FString Region = TEXT("US");
	EAccelByteItemStatus Status = EAccelByteItemStatus::ACTIVE;
	EAccelByteItemType Type = EAccelByteItemType::INGAMEITEM;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedRootItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FRegistry::Item.GetItemsByCriteria(Language, Region, ExpectedRootCategoryPath, Type, Status, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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

#pragma region GetItemByCriteria

	FString Language = TEXT("en");
	FString Region = TEXT("US");
	EAccelByteItemStatus Status = EAccelByteItemStatus::ACTIVE;
	EAccelByteItemType Type = EAccelByteItemType::INGAMEITEM;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedRootItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FRegistry::Item.GetItemsByCriteria(Language, Region, ExpectedRootCategoryPath, Type, Status, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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
	FRegistry::Item.GetItemById(ItemId, Language, Region, THandler<FAccelByteModelsItemInfo>::CreateLambda([&](const FAccelByteModelsItemInfo& Result)
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceSearchItemSuccess, "AccelByte.Tests.Ecommerce.C.SearchItem", AutomationFlagMaskEcommerce);
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

	FString Language = TEXT("en");
	FString Region = TEXT("US");
	EAccelByteItemStatus Status = EAccelByteItemStatus::ACTIVE;
	EAccelByteItemType TypeCoin = EAccelByteItemType::COINS;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria_VirtualCurrency"));
	FRegistry::Item.GetItemsByCriteria(Language, Region, ExpectedChildCategoryPath, TypeCoin, Status, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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

	EAccelByteItemType TypeInGameItem = EAccelByteItemType::INGAMEITEM;
	bool bGetItemByCriteriaSuccess2 = false;
	bool bExpectedItemFound2 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria_InGameItem"));
	FRegistry::Item.GetItemsByCriteria(Language, Region, ExpectedRootCategoryPath, TypeInGameItem, Status, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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

	FString Language = TEXT("en");
	FString Region = TEXT("US");
	EAccelByteItemStatus Status = EAccelByteItemStatus::ACTIVE;
	EAccelByteItemType Type = EAccelByteItemType::COINS;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FRegistry::Item.GetItemsByCriteria(Language, Region, ExpectedChildCategoryPath, Type, Status, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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

	FString Language = TEXT("en");
	FString Region = TEXT("US");
	EAccelByteItemStatus Status = EAccelByteItemStatus::ACTIVE;
	EAccelByteItemType Type = EAccelByteItemType::COINS;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	FRegistry::Item.GetItemsByCriteria(Language, Region, ExpectedChildCategoryPath, Type, Status, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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
	FRegistry::Order.GetUserOrders(0, 20, THandler<FAccelByteModelsOrderInfoPaging>::CreateLambda([&](const FAccelByteModelsOrderInfoPaging& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetUserOrdersSuccess = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bGetUserOrdersSuccess,"Waiting for get user order...");

#pragma region GetUserOrders

	check(bGetUserOrdersSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetWalletInfoByCurrencyCode, "AccelByte.Tests.Ecommerce.E.GetWalletInfoByCurrencyCode", AutomationFlagMaskEcommerce);
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceQueryUserEntitlement, "AccelByte.Tests.Ecommerce.F.QueryUserEntitlementSuccess", AutomationFlagMaskEcommerce);
bool EcommerceQueryUserEntitlement::RunTest(const FString& Parameters)
{
	bool bQueryEntitlementSuccess = false;
	bool bQueryResultTrue = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetWalletInfoByCurrencyCode"));

	FRegistry::Entitlement.QueryUserEntitlement("", "", 0, 20,
	THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsEntitlementPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bQueryEntitlementSuccess = true;
		bQueryResultTrue = (Result.Data.Num() > 0);
	}), EcommerceErrorHandler, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);

	FlushHttpRequests();
	Waiting(bQueryEntitlementSuccess,"Waiting for get user entitlement...");

	check(bQueryEntitlementSuccess);
	check(bQueryResultTrue);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceSetup, "AccelByte.Tests.Ecommerce.A.Setup", AutomationFlagMaskEcommerce);
bool EcommerceSetup::RunTest(const FString& Parameters)
{
	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("ClientLogin"));
	FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
	}), EcommerceErrorHandler);

	FlushHttpRequests();
	Waiting(bClientTokenObtained,"Waiting for Login...");

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

