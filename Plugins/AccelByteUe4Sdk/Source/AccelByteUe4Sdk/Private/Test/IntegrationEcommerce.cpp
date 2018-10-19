// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Test/IntegrationEcommerce.h"
#include "Test/IntegrationTest.h"
#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "AccelByteOrderApi.h"
#include "AccelByteCategoryApi.h"
#include "AccelByteItemApi.h"
#include "AccelByteUserManagementApi.h"
#include "AccelByteWalletApi.h"
#include "AccelByteSettings.h"
#include "AccelByteHttpRetrySystem.h"
#include "AccelByteCredentials.h"
#include "FileManager.h"
#include "AccelByteUserAuthenticationApi.h"

DEFINE_LOG_CATEGORY(LogAccelByteEcommerceTest);

const int32 AutomationFlagMaskEcommerce = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
const FString EcommerceUserEmail = TEXT("EcommerceUser@example.com");
const FString EcommerceUserPassword = TEXT("password");
const FString EcommerceUserDisplayName = TEXT("eCommUser");
const FString ExpectedRootCategoryPath = TEXT("/currencySDK");
const FString ExpectedChildCategoryPath = TEXT("/currencySDK/DogeCoin");
const FString ExpectedGrandChildCategoryPath = TEXT("/currencySDK/DogeCoin/Secret");
const FString ExpectedRootItemTitle = TEXT("Weapon");// "INGAMEITEM", 2 DOGECOIN, 0 Discount
const FString ExpectedChildItemTitle = TEXT("5 DogeCoinSDK");// Publisher's Currency, 0 USD, free, auto fulfilled, "COINS", "VIRTUAL", "DOGECOIN"
const FString ExpectedCurrencyCode = TEXT("DOGECOIN");

using namespace AccelByte;
using namespace AccelByte::Api;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetCategorySuccess, "LogAccelByteEcommerceTest.B.GetCategory.Success", AutomationFlagMaskEcommerce);
bool EcommerceGetCategorySuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();

#pragma region GetCategory

	FString CategoryPath = ExpectedChildCategoryPath;
	FString Language = TEXT("en");
	bool bHasDone = false;
	bool bGetCategorySuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetCategory"));
	Category::GetCategory(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), CategoryPath, Language, Category::FGetCategorySuccess::CreateLambda([&](FAccelByteModelsFullCategoryInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetCategorySuccess = true;
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetCategorySuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion GetCategory

	check(bGetCategorySuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetRootCategoriesSuccess, "LogAccelByteEcommerceTest.B.GetRootCategories.Success", AutomationFlagMaskEcommerce);
bool EcommerceGetRootCategoriesSuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();

#pragma region GetRootCategories

	FString Language = TEXT("en");
	bool bHasDone = false;
	bool bGetRootCategoriesSuccess = false;
	bool bExpectedRootCategoryFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetRootCategories"));
	Category::GetRootCategories(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, Category::FGetRootCategoriesSuccess::CreateLambda([&](TArray<FAccelByteModelsFullCategoryInfo> Results)
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
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetRootCategoriesSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion GetRootCategories

	check(bGetRootCategoriesSuccess);
	check(bExpectedRootCategoryFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetChildCategoriesSuccess, "LogAccelByteEcommerceTest.B.GetChildCategories.Success", AutomationFlagMaskEcommerce);
bool EcommerceGetChildCategoriesSuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();

#pragma region GetChildCategories

	FString Language = TEXT("en");
	bool bHasDone = false;
	bool bGetChildCategoriesSuccess = false;
	bool bExpectedChildCategoryFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetChildCategory"));
	Category::GetChildCategories(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, ExpectedRootCategoryPath, Category::FGetChildCategoriesSuccess::CreateLambda([&](TArray<FAccelByteModelsFullCategoryInfo> Results)
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
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetChildCategoriesSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion GetChildCategories

	check(bGetChildCategoriesSuccess);
	check(bExpectedChildCategoryFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetDescendantCategoriesSuccess, "LogAccelByteEcommerceTest.B.GetDescendantCategories.Success", AutomationFlagMaskEcommerce);
bool EcommerceGetDescendantCategoriesSuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();

#pragma region GetDescendantCategories

	FString Language = TEXT("en");
	bool bHasDone = false;
	bool bGetDescendantCategoriesSuccess = false;
	bool bExpectedDescendantCategoryFound1 = false;
	bool bExpectedDescendantCategoryFound2 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetDescendantCategories"));
	Category::GetDescendantCategories(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, ExpectedRootCategoryPath, Category::FGetChildCategoriesSuccess::CreateLambda([&](TArray<FAccelByteModelsFullCategoryInfo> Results)
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
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetDescendantCategoriesSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion GetDescendantCategories

	check(bGetDescendantCategoriesSuccess);
	check(bExpectedDescendantCategoryFound1);
	check(bExpectedDescendantCategoryFound2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetItemsByCriteriaSuccess, "LogAccelByteEcommerceTest.C.GetItemsByCriteria.Success", AutomationFlagMaskEcommerce);
bool EcommerceGetItemsByCriteriaSuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();

#pragma region GetItemByCriteria

	FString Language = TEXT("en");
	FString Region = TEXT("US");
	FString Status = TEXT("ACTIVE");
	FString Type = TEXT("INGAMEITEM");
	bool bHasDone = false;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedRootItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	Item::GetItemsByCriteria(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, Region, ExpectedRootCategoryPath, Type, Status, 0, 20, Item::FGetItemsByCriteriaSuccess::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetItemByCriteriaSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion GetItemByCriteria

	check(bGetItemByCriteriaSuccess);
	check(bExpectedRootItemFound);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetItemSuccess, "LogAccelByteEcommerceTest.C.GetItem.Success", AutomationFlagMaskEcommerce);
bool EcommerceGetItemSuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();
	FString ItemId = TEXT("");

#pragma region GetItemByCriteria

	FString Language = TEXT("en");
	FString Region = TEXT("US");
	FString Status = TEXT("ACTIVE");
	FString Type = TEXT("INGAMEITEM");
	bool bHasDone = false;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedRootItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	Item::GetItemsByCriteria(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, Region, ExpectedRootCategoryPath, Type, Status, 0, 20, Item::FGetItemsByCriteriaSuccess::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetItemByCriteriaSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion GetItemByCriteria

#pragma region GetItem

	bHasDone = false;
	bool bGetItemByIdSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemById"));
	Item::GetItemById(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), ItemId, Language, Region, Item::FGetItemByIdSuccess::CreateLambda([&](const FAccelByteModelsItemInfo& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetItemByIdSuccess = true;
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetItemByIdSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion GetItem

	check(bGetItemByCriteriaSuccess);
	check(bExpectedRootItemFound);
	check(bGetItemByIdSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceCreateOrderSuccess, "LogAccelByteEcommerceTest.D1.CreateOrder.Success", AutomationFlagMaskEcommerce);
bool EcommerceCreateOrderSuccess::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();
	FAccelByteModelsItemInfo Item;

#pragma region GetItemByCriteria

	FString Language = TEXT("en");
	FString Region = TEXT("US");
	FString Status = TEXT("ACTIVE");
	FString TypeCoin = TEXT("COINS");
	bool bHasDone = false;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria_VirtualCurrency"));
	Item::GetItemsByCriteria(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, Region, ExpectedChildCategoryPath, TypeCoin, Status, 0, 20, Item::FGetItemsByCriteriaSuccess::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    ChildFound: %d"), Result.Data.Num());
		for (int i = 0; i < Result.Data.Num(); i++)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			if (Result.Data[i].Title == ExpectedChildItemTitle)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found"));
				Item = Result.Data[i];
				bExpectedItemFound = true;
				bGetItemByCriteriaSuccess = true;
			}
		}
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetItemByCriteriaSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

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

	bHasDone = false;
	bool bCreateNewOrderSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("CreateNewOrder"));
	Order::CreateNewOrder(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderCreate, Order::FCreateNewOrderSuccess::CreateLambda([&](FAccelByteModelsOrderInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bHasDone = true;
		bCreateNewOrderSuccess = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bCreateNewOrderSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion CreateOrder

#pragma region GetItemByCriteria_InGameItem

	FString TypeInGameItem = TEXT("INGAMEITEM");
	bHasDone = false;
	bool bGetItemByCriteriaSuccess2 = false;
	bool bExpectedItemFound2 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria_InGameItem"));
	Item::GetItemsByCriteria(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, Region, ExpectedRootCategoryPath, TypeInGameItem, Status, 0, 20, Item::FGetItemsByCriteriaSuccess::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetItemByCriteriaSuccess2 = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion GetItemByCriteria_InGameItem

#pragma region CreateOrder_InGameItem

	OrderCreate.CurrencyCode = Item.RegionData[0].CurrencyCode;
	OrderCreate.DiscountedPrice = Item.RegionData[0].DiscountedPrice * Quantity;
	OrderCreate.Price = Item.RegionData[0].Price * Quantity;
	OrderCreate.Quantity = Quantity;
	OrderCreate.ReturnUrl = TEXT("https://sdk.example.com");
	OrderCreate.ItemId = Item.ItemId;

	bHasDone = false;
	bool bCreateNewOrderSuccess2 = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("CreateNewOrder2"));
	Order::CreateNewOrder(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderCreate, Order::FCreateNewOrderSuccess::CreateLambda([&](FAccelByteModelsOrderInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bHasDone = true;
		bCreateNewOrderSuccess2 = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bCreateNewOrderSuccess2 = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion CreateOrder_InGameItem

	check(bGetItemByCriteriaSuccess);
	check(bExpectedItemFound);
	check(bCreateNewOrderSuccess);
	check(bGetItemByCriteriaSuccess2);
	check(bExpectedItemFound2);
	check(bCreateNewOrderSuccess2);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetUserOrder, "LogAccelByteEcommerceTest.D2.GetUserOrder.Success", AutomationFlagMaskEcommerce);
bool EcommerceGetUserOrder::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();
	FAccelByteModelsItemInfo Item;
	FString OrderNo = TEXT("");

#pragma region GetItemByCriteria

	FString Language = TEXT("en");
	FString Region = TEXT("US");
	FString Status = TEXT("ACTIVE");
	FString Type = TEXT("COINS");
	bool bHasDone = false;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	Item::GetItemsByCriteria(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, Region, ExpectedChildCategoryPath, Type, Status, 0, 20, Item::FGetItemsByCriteriaSuccess::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    ChildFound: %d"), Result.Data.Num());
		for (int i = 0; i < Result.Data.Num(); i++)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			if (Result.Data[i].Title == ExpectedChildItemTitle)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found"));
				Item = Result.Data[i];
				bExpectedItemFound = true;
				bGetItemByCriteriaSuccess = true;
			}
		}
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetItemByCriteriaSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

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

	bHasDone = false;
	bool bCreateNewOrderSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("CreateNewOrder"));
	Order::CreateNewOrder(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderCreate, Order::FCreateNewOrderSuccess::CreateLambda([&](FAccelByteModelsOrderInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		OrderNo = Result.OrderNo;
		bHasDone = true;
		bCreateNewOrderSuccess = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bCreateNewOrderSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion CreateOrder

#pragma region GetUserOrder

	bHasDone = false;
	bool bGetUserOrderSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserOrder"));
	Order::GetUserOrder(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderNo, Order::FGetUserOrderSuccess::CreateLambda([&](FAccelByteModelsOrderInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetUserOrderSuccess = true;
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetUserOrderSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion GetUserOrder

	check(bGetItemByCriteriaSuccess);
	check(bExpectedItemFound);
	check(bCreateNewOrderSuccess);
	check(bGetUserOrderSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetUserOrderHistory, "LogAccelByteEcommerceTest.D3.GetUserOrderHistory.Success", AutomationFlagMaskEcommerce);
bool EcommerceGetUserOrderHistory::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();
	FAccelByteModelsItemInfo Item;
	FString OrderNo = TEXT("");

#pragma region GetItemByCriteria

	FString Language = TEXT("en");
	FString Region = TEXT("US");
	FString Status = TEXT("ACTIVE");
	FString Type = TEXT("COINS");
	bool bHasDone = false;
	bool bGetItemByCriteriaSuccess = false;
	bool bExpectedItemFound = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetItemsByCriteria"));
	Item::GetItemsByCriteria(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, Region, ExpectedChildCategoryPath, Type, Status, 0, 20, Item::FGetItemsByCriteriaSuccess::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    ChildFound: %d"), Result.Data.Num());
		for (int i = 0; i < Result.Data.Num(); i++)
		{
			UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
			if (Result.Data[i].Title == ExpectedChildItemTitle)
			{
				UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found"));
				Item = Result.Data[i];
				bExpectedItemFound = true;
				bGetItemByCriteriaSuccess = true;
			}
		}
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetItemByCriteriaSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

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

	bHasDone = false;
	bool bCreateNewOrderSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("CreateNewOrder"));
	Order::CreateNewOrder(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderCreate, Order::FCreateNewOrderSuccess::CreateLambda([&](FAccelByteModelsOrderInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		OrderNo = Result.OrderNo;
		bHasDone = true;
		bCreateNewOrderSuccess = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bCreateNewOrderSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion CreateOrder

#pragma region GetUserOrderHistory

	bHasDone = false;
	bool bGetUserOrderHistorySuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserOrder"));
	Order::GetUserOrderHistory(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OrderNo, Order::FGetUserOrderHistorySuccess::CreateLambda([&](TArray<FAccelByteModelsOrderHistoryInfo> Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetUserOrderHistorySuccess = true;
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetUserOrderHistorySuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion GetUserOrderHistory

	check(bGetItemByCriteriaSuccess);
	check(bExpectedItemFound);
	check(bCreateNewOrderSuccess);
	check(bGetUserOrderHistorySuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetUserOrders, "LogAccelByteEcommerceTest.D4.GetUserOrders.Success", AutomationFlagMaskEcommerce);
bool EcommerceGetUserOrders::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();

#pragma region GetUserOrders
	bool bHasDone = false;
	bool bGetUserOrdersSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetUserOrders"));
	Order::GetUserOrders(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), 0, 20, Order::FGetUserOrdersSuccess::CreateLambda([&](FAccelByteModelsOrderInfoPaging Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetUserOrdersSuccess = true;
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetUserOrdersSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma region GetUserOrders

	check(bGetUserOrdersSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceGetWalletInfoByCurrencyCode, "LogAccelByteEcommerceTest.E.GetWalletInfoByCurrencyCode", AutomationFlagMaskEcommerce);
bool EcommerceGetWalletInfoByCurrencyCode::RunTest(const FString& Parameters)
{
	float LastTime = FPlatformTime::Seconds();
	bool bHasDone = false;
	bool bGetWalletSuccess = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("GetWalletInfoByCurrencyCode"));
	Wallet::GetWalletInfoByCurrencyCode(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), ExpectedCurrencyCode, Wallet::FGetWalletByCurrencyCodeSuccess::CreateLambda([&](FAccelByteModelsWalletInfo Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bGetWalletSuccess = true;
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bGetWalletSuccess = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	check(bGetWalletSuccess);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceSetup, "LogAccelByteEcommerceTest.A.Setup.Success", AutomationFlagMaskEcommerce);
bool EcommerceSetup::RunTest(const FString& Parameters)
{
	double LastTime = 0;
	bool bHasDone = false;
	bool bClientTokenObtained = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("ClientLogin"));
	UserAuthentication::LoginWithClientCredentialsEasy(UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bClientTokenObtained = true;
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&bHasDone, &bClientTokenObtained](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bClientTokenObtained = false;
		bHasDone = true;
	}));

	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bRegisterSuccessful = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("CreateEmailAccountEasy"));
	UserManagement::CreateUserAccountEasy(EcommerceUserEmail, EcommerceUserPassword, EcommerceUserDisplayName, UserManagement::FCreateUserAccountSuccess::CreateLambda([&bHasDone, &bRegisterSuccessful](FAccelByteModelsUserCreateResponse Result)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bRegisterSuccessful = true;
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&bHasDone, &bRegisterSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bRegisterSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	bHasDone = false;
	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("LoginWithUsernameAndPassword"));
	UserAuthentication::LoginWithUsernameAndPasswordEasy(EcommerceUserEmail, EcommerceUserPassword, UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([&bHasDone, &bLoginSuccessful]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("   Success"));
		bLoginSuccessful = true;
		bHasDone = true;
	}), FErrorDelegate::CreateLambda([&bHasDone, &bLoginSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bLoginSuccessful = false;
		bHasDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bHasDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

	if (bLoginSuccessful)
	{
		// call E-CommerceTest.exe -setup
		return true;
	}
	return false;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EcommerceTearDown, "LogAccelByteEcommerceTest.Z.Teardown.Success", AutomationFlagMaskEcommerce);
bool EcommerceTearDown::RunTest(const FString& Parameters)
{

	// call E-CommerceTest.exe -tearDown

	float LastTime = 0.0f;

#pragma region DeleteUser

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("DeleteUser"));
	FIntegrationTestModule::DeleteUser(Credentials::Get().GetUserId(), FDeleteUserByIdSuccess::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), FErrorDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccessful](int32 Code, FString Message)
	{
		UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Fail\n%d\n%s\n"), Code, *Message);
		bDeleteSuccessful = false;
		bDeleteDone = true;
	}));

	LastTime = FPlatformTime::Seconds();
	while (!bDeleteDone)
	{
		const double AppTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(AppTime - LastTime);
		LastTime = AppTime;
		FPlatformProcess::Sleep(0.5f);
	}

#pragma endregion DeleteUser

	return bDeleteSuccessful;
}
