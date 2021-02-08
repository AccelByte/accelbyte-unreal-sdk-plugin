// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteCredentials.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerEcommerceApi.h"
#include "TestUtilities.h"
#include "HAL/FileManager.h"

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteSubscriptionTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteSubscriptionTest);

const int32 AutomationFlagMaskSubscription = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
void FlushHttpRequests();

const auto SubscriptionErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteSubscriptionTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

static FString APPID = "sdkTest0001";
FString appTypeItemSku;
FString subsTypeItemSku;

TArray<FString> usersMail;
static FString userPassword = "Password+123";

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SubscriptionSetup, "AccelByte.Tests.Subscription.A.SubscriptionSetup", AutomationFlagMaskSubscription);
bool SubscriptionSetup::RunTest(const FString& Parameters) 
{
	AB_TEST_SKIP_WHEN_DISABLED();

	FString gameNamespace = FRegistry::Settings.Namespace;
	FString publisherNamespace = FRegistry::Settings.PublisherNamespace;
	FString language = "en-US";
	FString region = "US";

	usersMail.Add(TEXT("Subs1SDKTest@example.com"));
	usersMail.Add(TEXT("Subs2SDKTest@example.com"));
	usersMail.Add(TEXT("Subs3SDKTest@example.com"));

	bool clientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&clientLoginSuccess]()
	{
		clientLoginSuccess = true;
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Client Successfully Login."));
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(clientLoginSuccess, "Waiting for client login.");
	check(clientLoginSuccess);

	bool hasPublishedStore = false;
	FStoreInfo PublishedStoreInfo;
	Ecommerce_PublishedStore_Get(publisherNamespace, THandler<FStoreInfo>::CreateLambda([&hasPublishedStore, &PublishedStoreInfo](const FStoreInfo& Result)
	{
		PublishedStoreInfo = Result;
		hasPublishedStore = true;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(hasPublishedStore, "Waiting for get published store");
	check(hasPublishedStore);
	FString publishedStoreId = PublishedStoreInfo.storeId;
	
	appTypeItemSku = "sdktestSkuApp001";
	FString appTypeItemId;
	FItemFullInfo appTypeInfo;
	bool hasAppTypeItem = false;
	bool doneGetItemBySKU = false;
	Ecommerce_GetItem_BySKU(*GetPublisherNamespace(), publishedStoreId, appTypeItemSku, true, THandler<FItemFullInfo>::CreateLambda([&doneGetItemBySKU, &hasAppTypeItem, &appTypeInfo](const FItemFullInfo& Result)
	{
		doneGetItemBySKU = true;
		hasAppTypeItem = true;
		appTypeInfo = Result;
	}), FErrorHandler::CreateLambda([&doneGetItemBySKU](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Error Get Item By SKU"));
		doneGetItemBySKU = true;
	}));
	FlushHttpRequests();
	Waiting(doneGetItemBySKU, "Waiting to get item by sku");
	check(doneGetItemBySKU);
	if (hasAppTypeItem) 
	{
		appTypeItemId = appTypeInfo.itemId;
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Has APP Item type on store"));
	}

	subsTypeItemSku = "sdktestSkuSubs001";
	FString subsTypeItemId;
	FItemFullInfo subsTypeInfo;
	bool hasSubsTypeItem = false;
	doneGetItemBySKU = false;
	Ecommerce_GetItem_BySKU(*GetPublisherNamespace(), publishedStoreId, subsTypeItemSku, true, THandler<FItemFullInfo>::CreateLambda([&doneGetItemBySKU, &hasSubsTypeItem, &subsTypeInfo](const FItemFullInfo& Result)
	{
		doneGetItemBySKU = true;
		hasSubsTypeItem = true;
		subsTypeInfo = Result;
	}), FErrorHandler::CreateLambda([&doneGetItemBySKU](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Error Get Item By SKU"));
		doneGetItemBySKU = true;
	}));
	FlushHttpRequests();
	Waiting(doneGetItemBySKU, "Waiting to get item by sku");
	check(doneGetItemBySKU);
	if (hasSubsTypeItem)
	{
		subsTypeItemId = subsTypeInfo.itemId;
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Has Subscription Item type on store"));
	}

	bool needToCreateItem = !hasAppTypeItem || !hasSubsTypeItem;
	if (needToCreateItem) 
	{
		FStoreCreateRequest TemporaryStore
		{
			"UE4 Store Test",
			"for UE4 SDK testing purpose",
			{"en"},
			{"US"},
			"en-US",
			"US"
		};

		bool doneCreateTempStore = false;
		FStoreInfo tempStoreInfo;
		FString tempStoreId;
		Ecommerce_Store_Create(publisherNamespace, TemporaryStore, THandler<FStoreInfo>::CreateLambda([&doneCreateTempStore, &tempStoreInfo](const FStoreInfo& Result)
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Create temp store success"));
			doneCreateTempStore = true;
			tempStoreInfo = Result;
		}), SubscriptionErrorHandler);
		FlushHttpRequests();
		Waiting(doneCreateTempStore, "Waiting to create temp store");
		tempStoreId = tempStoreInfo.storeId;
		check(doneCreateTempStore);

		bool clonePublishedStore = false;
		Ecommerce_Store_Clone(publisherNamespace, publishedStoreId, tempStoreId, FSimpleDelegate::CreateLambda([&clonePublishedStore]()
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Clone published store"));
			clonePublishedStore = true;
		}), SubscriptionErrorHandler);
		FlushHttpRequests();
		Waiting(clonePublishedStore, "Waiting for clone published store...");
		check(clonePublishedStore);

		FCreateRegionDataItem regionData
		{
			0,
			0,
			0,
			0,
			"USD",
			EAccelByteItemCurrencyType::REAL,
			publisherNamespace,
			FDateTime::MinValue().ToIso8601(),
			FDateTime::MaxValue().ToIso8601(),
			FDateTime::MinValue().ToIso8601(),
			FDateTime::MaxValue().ToIso8601()
		};

		FLocalization localization
		{
			"UE4 SDK Test Item",
			"SDK Test Item",
			"SDK Item, real currency, USD."
		};
		TMap<FString, FLocalization> itemLocalization;
		itemLocalization.Add(TemporaryStore.defaultLanguage, localization);

		FString appId = "ue4SdkTest001";
		// if doesn't have "APP" item on published store
		if (!hasAppTypeItem) 
		{
			FItemCreateRequest appItemRequest;
			appItemRequest.itemType = EAccelByteItemType::APP;
			appItemRequest.name = "SDK UE4 App Item";
			appItemRequest.appId = APPID;
			appItemRequest.appType = EAccelByteAppType::GAME;
			appItemRequest.categoryPath = "/game";
			appItemRequest.entitlementType = EAccelByteEntitlementType::DURABLE;
			appItemRequest.sku = appTypeItemSku;
			appItemRequest.status = EAccelByteItemStatus::ACTIVE;
			appItemRequest.regionData = FRegionDataUS{ {regionData} };
			appItemRequest.localizations = itemLocalization;
			appItemRequest.maxCountPerUser = 1;

			bool createAppItemSuccess = false;
			FItemFullInfo appItemInfo;
			Ecommerce_Item_Create(publisherNamespace, appItemRequest, tempStoreId, THandler<FItemFullInfo>::CreateLambda([&createAppItemSuccess, &appItemInfo](const FItemFullInfo& Result)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Create app item success"));
				createAppItemSuccess = true;
				appItemInfo = Result;
			}), SubscriptionErrorHandler);
			FlushHttpRequests();
			Waiting(createAppItemSuccess, "Waiting for app item created...");
			check(createAppItemSuccess);
			appTypeItemId = appItemInfo.itemId;
		}

		// if doesn't have "SUBSCRIPTION" item on published store
		if (!hasSubsTypeItem)
		{
			TArray<FString> emptyStrings;

			FRecurring subscriptionRequest;
			subscriptionRequest.cycle = EAccelByteSubscriptionCycle::YEARLY;
			subscriptionRequest.graceDays = 1;
			subscriptionRequest.fixedFreeDays = 0;
			subscriptionRequest.fixedTrialCycles = 0;

			FItemCreateRequest subsItemRequest;
			subsItemRequest.itemType = EAccelByteItemType::SUBSCRIPTION;
			subsItemRequest.name = "SDK UE4 Test Yearly subs";
			subsItemRequest.entitlementType = EAccelByteEntitlementType::DURABLE;
			subsItemRequest.appType = EAccelByteAppType::GAME;
			subsItemRequest.categoryPath = "/subscription";
			subsItemRequest.sku = subsTypeItemSku;
			subsItemRequest.status = EAccelByteItemStatus::ACTIVE;
			subsItemRequest.recurring = subscriptionRequest;
			subsItemRequest.maxCountPerUser = 1;
			subsItemRequest.regionData = FRegionDataUS{ {regionData} };
			subsItemRequest.localizations = itemLocalization;

			bool createSubsItemSuccess = false;
			FItemFullInfo appItemInfo;
			Ecommerce_Item_Create(publisherNamespace, subsItemRequest, tempStoreId, THandler<FItemFullInfo>::CreateLambda([&createSubsItemSuccess, &appItemInfo](const FItemFullInfo& Result)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Create subs item success"));
				createSubsItemSuccess = true;
				appItemInfo = Result;
			}), SubscriptionErrorHandler);
			FlushHttpRequests();
			Waiting(createSubsItemSuccess, "Waiting for subs item created...");
			check(createSubsItemSuccess);
			subsTypeItemId = appItemInfo.itemId;
		}

		// published temp store id to published store
		bool publishTempStore = false;
		Ecommerce_Store_Clone(publisherNamespace, tempStoreId, "", FSimpleDelegate::CreateLambda([&publishTempStore]()
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Published temp store success"));
			publishTempStore = true;
		}), SubscriptionErrorHandler);
		FlushHttpRequests();
		Waiting(publishTempStore, "Waiting for publish temp store...");
		check(publishTempStore);
	}

	//User Creation (3 users)

	bool loginWithDeviceSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success login with device id"));
		loginWithDeviceSuccess = true;
	}), SubscriptionErrorHandler);

	FlushHttpRequests();
	Waiting(loginWithDeviceSuccess, "Waiting for Login...");
	check(loginWithDeviceSuccess);

	bool doneSearchUser = false;
	bool isUserExist = false;
	for (int i = 0; i < usersMail.Num(); i++)
	{
		doneSearchUser = false;
		isUserExist = false;
		FPagedPublicUsersInfo ReceivedUserData;
		FRegistry::User.SearchUsers(usersMail[i], EAccelByteSearchType::EMAILADDRESS, THandler<FPagedPublicUsersInfo>::CreateLambda([&doneSearchUser, &ReceivedUserData](const FPagedPublicUsersInfo& Result)
		{
			doneSearchUser = true;
			ReceivedUserData = Result;
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Search User"));
			
		}), SubscriptionErrorHandler);
		FlushHttpRequests();
		Waiting(doneSearchUser, "Waiting for search user");
		check(doneSearchUser);

		if (ReceivedUserData.Data.Num() > 0)
		{
			isUserExist = true;
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User is exist"));
		}

		if (!isUserExist) 
		{
			//create new user
			FString emailAddress = usersMail[i];
			const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 22));
			const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
			FString username = FString("Subs UE4 " + FString::FromInt(i+1));
			bool registerSuccess = false;
			bool doneRegisterUser = false;
			FRegistry::User.Register(usersMail[i], userPassword, username, "MG", format, THandler<FRegisterResponse>::CreateLambda([&registerSuccess, &doneRegisterUser](const FRegisterResponse& Result)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Register User"));
				registerSuccess = true;
				doneRegisterUser = true;
			}), SubscriptionErrorHandler);

			FlushHttpRequests();
			Waiting(doneRegisterUser, "Waiting for register user");
			check(doneRegisterUser);
			check(registerSuccess);
		}
	}

	UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout device id"));
	FRegistry::User.ForgetAllCredentials();

	TArray<FString> userPublisherIds;
	for (int i = 0; i < usersMail.Num(); i++) 
	{
		bool loginSuccess = false;;
		FRegistry::User.LoginWithUsername(usersMail[i], userPassword, FVoidHandler::CreateLambda([&loginSuccess]()
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Login"));
			loginSuccess = true;
		}), SubscriptionErrorHandler);
		FlushHttpRequests();
		Waiting(loginSuccess, "Waiting for login");
		check(loginSuccess);

		bool doneGetUserMap = false;
		FUserMapResponse userMap;
		User_Get_User_Mapping(FRegistry::Credentials.GetUserId(), THandler<FUserMapResponse>::CreateLambda([&doneGetUserMap, &userMap](const FUserMapResponse& Result) 
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Get User Map"));
			doneGetUserMap = true;
			userMap = Result;
		}), SubscriptionErrorHandler);
		FlushHttpRequests();
		Waiting(doneGetUserMap, "Waiting for get user map");
		check(doneGetUserMap);

		userPublisherIds.Add(userMap.userId);

		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User"));
		FRegistry::User.ForgetAllCredentials();
	}

	//grant APP type into User 1
	FAccelByteModelsEntitlementGrant grantRequest;
	grantRequest.ItemNamespace = publisherNamespace;
	grantRequest.ItemId = appTypeItemId;
	grantRequest.GrantedCode = "02501205";
	grantRequest.Quantity = 1;
	grantRequest.Source = EAccelByteEntitlementSource::REDEEM_CODE;
	grantRequest.Region = "US";
	grantRequest.Language = "en-US";

	TArray<FAccelByteModelsEntitlementGrant> grantEnitlementBody;
	grantEnitlementBody.Add(grantRequest);

	// login into user 1, and check if has app entitlement or not
	bool loginSuccess = false;;
	FRegistry::User.LoginWithUsername(usersMail[0], userPassword, FVoidHandler::CreateLambda([&loginSuccess]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Login"));
		loginSuccess = true;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(loginSuccess, "Waiting for login");

	bool doneGetAppEntitlement = false;
	FAccelByteModelsEntitlementOwnership appOwnership;
	FRegistry::Entitlement.GetUserEntitlementOwnershipBySku(
		appTypeItemSku,
		THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda([&doneGetAppEntitlement, &appOwnership](const FAccelByteModelsEntitlementOwnership& Result)
	{
		doneGetAppEntitlement = true;
		appOwnership = Result;
	}), FErrorHandler::CreateLambda([&doneGetAppEntitlement](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User doesn't have APP."));
		doneGetAppEntitlement = true;
	}));
	FlushHttpRequests();
	Waiting(doneGetAppEntitlement, "Waiting for get subs entitlement");
	check(doneGetAppEntitlement);

	if (!appOwnership.Owned)
	{
		bool grantAppTypeSuccess = false;
		TArray<FAccelByteModelsStackableEntitlementInfo> grantResultInfo;
		Ecommerce_GrantUserEntitlements(
			publisherNamespace,
			userPublisherIds[0],
			grantEnitlementBody,
			THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>::CreateLambda([&grantAppTypeSuccess, &grantResultInfo](const TArray<FAccelByteModelsStackableEntitlementInfo>& Result)
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Grant APP Item to User 1"));
			grantAppTypeSuccess = true;
			grantResultInfo = Result;
		}), SubscriptionErrorHandler);
		FlushHttpRequests();
		Waiting(grantAppTypeSuccess, "Waiting for grant app type");
		check(grantAppTypeSuccess);
	}

	// login into user 2, and check if has subs entitlement or not
	UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User"));
	FRegistry::User.ForgetAllCredentials();
	loginSuccess = false;
	FRegistry::User.LoginWithUsername(usersMail[1], userPassword, FVoidHandler::CreateLambda([&loginSuccess]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Login"));
		loginSuccess = true;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(loginSuccess, "Waiting for login");

	bool doneGetUserEntitlement = false;
	FAccelByteModelsEntitlementOwnership ownership;
	FRegistry::Entitlement.GetUserEntitlementOwnershipBySku(
		subsTypeItemSku, 
		THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda([&doneGetUserEntitlement, &ownership](const FAccelByteModelsEntitlementOwnership& Result)
		{
			doneGetUserEntitlement = true;
			ownership = Result;
		}), FErrorHandler::CreateLambda([&doneGetUserEntitlement](int32 ErrorCode, FString ErrorMessage)
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User doesn't have Subscription."));
			doneGetUserEntitlement = true;
		}));
	FlushHttpRequests();
	Waiting(doneGetUserEntitlement, "Waiting for get subs entitlement");
	check(doneGetUserEntitlement);

	FFreeSubscriptionRequest subsRequest;
	subsRequest.ItemId = subsTypeItemId;
	subsRequest.GrantDays = 365;
	subsRequest.Source = "ue4 sdk test";
	subsRequest.Reason = "testing purpose";
	subsRequest.Region = "US";
	subsRequest.Language = "en-US";

	if (!ownership.Owned) 
	{
		bool doneGrantSubs = false;
		FItemFullInfo subsItemInfo;
		Subscription_GrantFreeSubs(userPublisherIds[1], subsRequest, THandler<FItemFullInfo>::CreateLambda([&doneGrantSubs, &subsItemInfo](const FItemFullInfo& Result)
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Grant free subs to User 1 success"));
			doneGrantSubs = true;
			subsItemInfo = Result;
		}), SubscriptionErrorHandler);
		FlushHttpRequests();
		Waiting(doneGrantSubs, "Waiting for grant free subs");
		check(doneGrantSubs);
	}

	UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User 1"));
	FRegistry::User.ForgetAllCredentials();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CheckUserEligibleOwnedAppId, "AccelByte.Tests.Subscription.B.CheckUserEligibleOwnedAppId", AutomationFlagMaskSubscription);
bool CheckUserEligibleOwnedAppId::RunTest(const FString& Parameters)
{
	AB_TEST_SKIP_WHEN_DISABLED();

	FString oriAppId = FRegistry::Settings.AppId;
	FRegistry::Settings.AppId = APPID;

	UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User"));
	FRegistry::User.ForgetAllCredentials();

	//Using User 1
	bool bLoginFinish = false;
	FRegistry::User.LoginWithUsername(usersMail[0], userPassword, FVoidHandler::CreateLambda([&bLoginFinish]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User Login Success"));
		bLoginFinish = true;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(bLoginFinish, "Waiting user login...");

	bool bCheckEligibleFinish = false;
	bool bCheckEligibleResult = false;
	FRegistry::User.GetUserEligibleToPlay(THandler<bool>::CreateLambda([&bCheckEligibleFinish, &bCheckEligibleResult](bool result) 
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Check Eligibility Finish"));
		bCheckEligibleFinish = true;
		bCheckEligibleResult = result;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(bCheckEligibleFinish, "Waiting checking user eligibility");

	check(bCheckEligibleResult);
	FRegistry::Settings.AppId = oriAppId;

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CheckUserEligibleOwnedSubs, "AccelByte.Tests.Subscription.C.CheckUserEligibleOwnedSubs", AutomationFlagMaskSubscription);
bool CheckUserEligibleOwnedSubs::RunTest(const FString& Parameters)
{
	AB_TEST_SKIP_WHEN_DISABLED();

	FString oriAppId = FRegistry::Settings.AppId;
	FRegistry::Settings.AppId = APPID;

	UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User"));
	FRegistry::User.ForgetAllCredentials();

	//Using User 2
	bool bLoginFinish = false;
	FRegistry::User.LoginWithUsername(usersMail[1], userPassword, FVoidHandler::CreateLambda([&bLoginFinish]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User Login Success"));
		bLoginFinish = true;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(bLoginFinish, "Waiting user login...");

	bool doneGetSubsOwnership = false;
	FAccelByteModelsEntitlementOwnership ownership;
	FRegistry::Entitlement.GetUserEntitlementOwnershipBySku(
		subsTypeItemSku,
		THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda([&doneGetSubsOwnership, &ownership](const FAccelByteModelsEntitlementOwnership& Result)
	{
		doneGetSubsOwnership = true;
		ownership = Result;
	}), FErrorHandler::CreateLambda([&doneGetSubsOwnership](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User doesn't have Subscription."));
		doneGetSubsOwnership = true;
	}));
	FlushHttpRequests();
	Waiting(doneGetSubsOwnership, "Waiting for get subs entitlement");
	check(doneGetSubsOwnership);
	check(ownership.Owned);

	FRegistry::Settings.AppId = oriAppId;

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CheckUserEligibleOwnedNone, "AccelByte.Tests.Subscription.E.CheckUserEligibleOwnedNone", AutomationFlagMaskSubscription);
bool CheckUserEligibleOwnedNone::RunTest(const FString& Parameters)
{
	AB_TEST_SKIP_WHEN_DISABLED();

	FString oriAppId = FRegistry::Settings.AppId;
	FRegistry::Settings.AppId = APPID;

	UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User"));
	FRegistry::User.ForgetAllCredentials();

	//Using User 3
	bool bLoginFinish = false;
	FRegistry::User.LoginWithUsername(usersMail[2], userPassword, FVoidHandler::CreateLambda([&bLoginFinish]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User Login Success"));
		bLoginFinish = true;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(bLoginFinish, "Waiting user login...");

	bool bCheckEligibleFinish = false;
	bool bCheckEligibleResult = true;
	FRegistry::User.GetUserEligibleToPlay(THandler<bool>::CreateLambda([&bCheckEligibleFinish, &bCheckEligibleResult](bool result)
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Check Eligibility Finish"));
		bCheckEligibleFinish = true;
		bCheckEligibleResult = result;
	}), SubscriptionErrorHandler);
	FlushHttpRequests();
	Waiting(bCheckEligibleFinish, "Waiting checking user eligibility");

	check(!bCheckEligibleResult);
	FRegistry::Settings.AppId = oriAppId;
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SubscriptionTeardown, "AccelByte.Tests.Subscription.Z.SubscriptionTeardown", AutomationFlagMaskSubscription);
bool SubscriptionTeardown::RunTest(const FString& Parameters)
{
	AB_TEST_SKIP_WHEN_DISABLED();

	return true;
}
