// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "AccelByteOrderApi.h"
#include "AccelByteCategoryApi.h"
#include "AccelByteItemApi.h"
#include "AccelByteUserManagementApi.h"
#include "AccelByteWalletApi.h"
#include "AccelByteEntitlementApi.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"
#include "FileManager.h"
#include "AccelByteUserAuthenticationApi.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::UserAuthentication;
using AccelByte::Api::UserManagement;
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

const int32 AutomationFlagMaskEcommerce = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
FString EcommerceUserEmail;
FString EcommerceUserPassword;
FString EcommerceUserDisplayName;
FString ExpectedRootCategoryPath;
FString ExpectedChildCategoryPath;
FString ExpectedGrandChildCategoryPath;
FString ExpectedRootItemTitle; //"INGAMEITEM", 2 DOGECOIN, 0 Discount
FString ExpectedChildItemTitle;// Publisher's Currency, 0 USD, free, auto fulfilled, "COINS", "VIRTUAL", "DOGECOIN"
FString ExpectedCurrencyCode;

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
	Category::GetCategory(CategoryPath, Language, Category::FGetCategorySuccess::CreateLambda([&](FAccelByteModelsFullCategoryInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetCategorySuccess = true;
	}), EcommerceErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

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
	Category::GetRootCategories(Language, Category::FGetRootCategoriesSuccess::CreateLambda([&](TArray<FAccelByteModelsFullCategoryInfo> Results)
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
	FHttpModule::Get().GetHttpManager().Flush(false);

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
	Category::GetChildCategories(Language, ExpectedRootCategoryPath, Category::FGetChildCategoriesSuccess::CreateLambda([&](TArray<FAccelByteModelsFullCategoryInfo> Results)
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
	FHttpModule::Get().GetHttpManager().Flush(false);

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
	Category::GetDescendantCategories(Language, ExpectedRootCategoryPath, Category::FGetChildCategoriesSuccess::CreateLambda([&](TArray<FAccelByteModelsFullCategoryInfo> Results)
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
	FHttpModule::Get().GetHttpManager().Flush(false);

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
	Item::GetItemsByCriteria(Language, Region, ExpectedRootCategoryPath, Type, Status, 0, 20, Item::FGetItemsByCriteriaSuccess::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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
	FHttpModule::Get().GetHttpManager().Flush(false);

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
	Item::GetItemsByCriteria(Language, Region, ExpectedRootCategoryPath, Type, Status, 0, 20, Item::FGetItemsByCriteriaSuccess::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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
	FHttpModule::Get().GetHttpManager().Flush(false);

#pragma endregion GetItemByCriteria

#pragma region GetItem

	bool bGetItemByIdSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemById"));
	Item::GetItemById(ItemId, Language, Region, Item::FGetItemByIdSuccess::CreateLambda([&](const FAccelByteModelsItemInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetItemByIdSuccess = true;
	}), EcommerceErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

#pragma endregion GetItem

	check(bGetItemByCriteriaSuccess);
	check(bExpectedRootItemFound);
	check(bGetItemByIdSuccess);
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
	Item::GetItemsByCriteria(Language, Region, ExpectedChildCategoryPath, TypeCoin, Status, 0, 20, Item::FGetItemsByCriteriaSuccess::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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
	FHttpModule::Get().GetHttpManager().Flush(false);

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
	Order::CreateNewOrder(OrderCreate, Order::FCreateNewOrderSuccess::CreateLambda([&](FAccelByteModelsOrderInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bCreateNewOrderSuccess = true;
	}), EcommerceErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

#pragma endregion CreateOrder

#pragma region GetItemByCriteria_InGameItem

	EAccelByteItemType TypeInGameItem = EAccelByteItemType::INGAMEITEM;
	bool bGetItemByCriteriaSuccess2 = false;
	bool bExpectedItemFound2 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria_InGameItem"));
	Item::GetItemsByCriteria(Language, Region, ExpectedRootCategoryPath, TypeInGameItem, Status, 0, 20, Item::FGetItemsByCriteriaSuccess::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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
	FHttpModule::Get().GetHttpManager().Flush(false);

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
	Order::CreateNewOrder(OrderCreate, Order::FCreateNewOrderSuccess::CreateLambda([&](FAccelByteModelsOrderInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bCreateNewOrderSuccess2 = true;
	}), EcommerceErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

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
	Item::GetItemsByCriteria(Language, Region, ExpectedChildCategoryPath, Type, Status, 0, 20, Item::FGetItemsByCriteriaSuccess::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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
	FHttpModule::Get().GetHttpManager().Flush(false);

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
	Order::CreateNewOrder(OrderCreate, Order::FCreateNewOrderSuccess::CreateLambda([&](FAccelByteModelsOrderInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		OrderNo = Result.OrderNo;
		bCreateNewOrderSuccess = true;
	}), EcommerceErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

#pragma endregion CreateOrder

#pragma region GetUserOrder

	bool bGetUserOrderSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserOrder"));
	Order::GetUserOrder(OrderNo, Order::FGetUserOrderSuccess::CreateLambda([&](FAccelByteModelsOrderInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetUserOrderSuccess = true;
	}), EcommerceErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

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
	Item::GetItemsByCriteria(Language, Region, ExpectedChildCategoryPath, Type, Status, 0, 20, Item::FGetItemsByCriteriaSuccess::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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
	FHttpModule::Get().GetHttpManager().Flush(false);

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
	Order::CreateNewOrder(OrderCreate, Order::FCreateNewOrderSuccess::CreateLambda([&](FAccelByteModelsOrderInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		OrderNo = Result.OrderNo;
		bCreateNewOrderSuccess = true;
	}), EcommerceErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

#pragma endregion CreateOrder

#pragma region GetUserOrderHistory

	bool bGetUserOrderHistorySuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserOrder"));
	Order::GetUserOrderHistory(OrderNo, Order::FGetUserOrderHistorySuccess::CreateLambda([&](TArray<FAccelByteModelsOrderHistoryInfo> Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetUserOrderHistorySuccess = true;
	}), EcommerceErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

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
	Order::GetUserOrders(0, 20, Order::FGetUserOrdersSuccess::CreateLambda([&](FAccelByteModelsOrderInfoPaging Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetUserOrdersSuccess = true;
	}), EcommerceErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

#pragma region GetUserOrders

	check(bGetUserOrdersSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetWalletInfoByCurrencyCode, "AccelByte.Tests.Ecommerce.E.GetWalletInfoByCurrencyCode", AutomationFlagMaskEcommerce);
bool EcommerceGetWalletInfoByCurrencyCode::RunTest(const FString& Parameters)
{
	bool bGetWalletSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetWalletInfoByCurrencyCode"));
	Wallet::GetWalletInfoByCurrencyCode(ExpectedCurrencyCode, Wallet::FGetWalletByCurrencyCodeSuccess::CreateLambda([&](FAccelByteModelsWalletInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetWalletSuccess = true;
	}), EcommerceErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

	check(bGetWalletSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceQueryUserEntitlement, "AccelByte.Tests.Ecommerce.F.QueryUserEntitlementSuccess", AutomationFlagMaskEcommerce);
bool EcommerceQueryUserEntitlement::RunTest(const FString& Parameters)
{
	bool bQueryEntitlementSuccess = false;
	bool bQueryResultTrue = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetWalletInfoByCurrencyCode"));

	Entitlement::QueryUserEntitlement("", "", 0, 20,
 Entitlement::FQueryUserEntitlementSuccess::CreateLambda([&](FAccelByteModelsEntitlementPagingSlicedResult Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bQueryEntitlementSuccess = true;
		bQueryResultTrue = (Result.Data.Num() > 0);
	}), EcommerceErrorHandler, EAccelByteEntitlementClass::NONE, EAccelByteAppType::NONE);
	FHttpModule::Get().GetHttpManager().Flush(false);

	check(bQueryEntitlementSuccess);
	check(bQueryResultTrue);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceSetup, "AccelByte.Tests.Ecommerce.A.Setup", AutomationFlagMaskEcommerce);
bool EcommerceSetup::RunTest(const FString& Parameters)
{
	FString TestVariableFileContent = TEXT("");
	FString CurrentDirectory = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectDir());
	CurrentDirectory.Append(TEXT("TestUtilities/EcommerceVariables.txt"));
	CurrentDirectory.Replace(TEXT("/"), TEXT("\\"));
	FFileHelper::LoadFileToString(TestVariableFileContent, *CurrentDirectory);
	TArray<FString> TestVariables;
	TestVariableFileContent.ParseIntoArray(TestVariables, TEXT("___"), false);
	ExpectedRootCategoryPath = TestVariables[0];
	ExpectedChildCategoryPath = TestVariables[1];
	ExpectedGrandChildCategoryPath = TestVariables[2];
	EcommerceUserEmail = TestVariables[3];
	EcommerceUserPassword = TestVariables[4];
	ExpectedCurrencyCode = TestVariables[5];
	ExpectedRootItemTitle = TestVariables[6];
	ExpectedChildItemTitle = TestVariables[7];

	double LastTime = 0;
	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("ClientLogin"));
	UserAuthentication::LoginWithClientCredentials(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
	}), EcommerceErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("LoginWithDeviceAccount"));
	UserAuthentication::LoginWithDeviceId(UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("   Success"));
		bLoginSuccessful = true;
	}), EcommerceErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

	if (bLoginSuccessful)
	{
		// call E-CommerceTest.exe -setup
		return true;
	}
	return false;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceTearDown, "AccelByte.Tests.Ecommerce.Z.Teardown", AutomationFlagMaskEcommerce);
bool EcommerceTearDown::RunTest(const FString& Parameters)
{

	// call E-CommerceTest.exe -tearDown

	float LastTime = 0.0f;

#pragma region DeleteUser

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("DeleteUserById"));
	DeleteUserByIdLobby(Credentials::Get().GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), EcommerceErrorHandler);
	FHttpModule::Get().GetHttpManager().Flush(false);

#pragma endregion DeleteUser

	return bDeleteSuccessful;
}


void DeleteUserByIdLobby(const FString& UserId, const FDeleteUserByIdSuccess& OnSuccess, const FErrorHandler& OnError)
{
	using AccelByte::Settings;
	UserAuthentication::LoginWithClientCredentials(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([OnSuccess, OnError, UserId]()
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/platforms/justice/%s"), *Settings::IamServerUrl, *Settings::Namespace, *UserId, *Settings::PublisherNamespace);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Content;
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);
		Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
		{
			int32 Code = 0;
			FString Message;
			if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				TSharedPtr<FJsonObject> JsonParsed;
				TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Response->GetContentAsString());
				if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
				{
					FString RealUserId = JsonParsed->GetStringField("UserId");

					FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetClientAccessToken());
					FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s"), *Settings::IamServerUrl, *Settings::PublisherNamespace, *RealUserId);
					FString Verb = TEXT("DELETE");
					FString ContentType = TEXT("application/json");
					FString Accept = TEXT("application/json");
					FString Content;
					FHttpRequestPtr Request2 = FHttpModule::Get().CreateRequest();
					Request2->SetURL(Url);
					Request2->SetHeader(TEXT("Authorization"), Authorization);
					Request2->SetVerb(Verb);
					Request2->SetHeader(TEXT("Content-Type"), ContentType);
					Request2->SetHeader(TEXT("Accept"), Accept);
					Request2->SetContentAsString(Content);
					Request2->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
					{
						int32 Code;
						FString Message;
						if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
						{
							OnSuccess.ExecuteIfBound();
						}
						else
						{
							HandleHttpError(Request, Response, Code, Message);
							OnError.ExecuteIfBound(Code, Message);
						}
					});
					Request2->ProcessRequest();
				}
				else
				{
					HandleHttpError(Request, Response, Code, Message);
					OnError.ExecuteIfBound(Code, Message);
				}
			}
			else
			{
				HandleHttpError(Request, Response, Code, Message);
				OnError.ExecuteIfBound(Code, Message);
			}
		});
		Request->ProcessRequest();
	}), FErrorHandler::CreateLambda([OnError](int32 Code, FString Message)
	{
		OnError.ExecuteIfBound(Code, Message);
	}));
}
