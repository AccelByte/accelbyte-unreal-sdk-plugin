// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteCredentials.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "TestUtilities.h"

using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteSubscriptionTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteSubscriptionTest);

int32 const AutomationFlagMaskSubscription = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
void FlushHttpRequests();

auto const SubscriptionErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteSubscriptionTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

static FString APPID = "sdkTest0001";
FString GAppTypeItemSku;
FString GSubsTypeItemSku;

TArray<FString> GUsersMail;
static FString GUserPassword = "Password+123";

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SubscriptionSetup, "AccelByte.Tests.Subscription.A.SubscriptionSetup", AutomationFlagMaskSubscription);
bool SubscriptionSetup::RunTest(const FString& Parameters) 
{
	FString GameNamespace = FRegistry::Settings.Namespace;
	FString PublisherNamespace = FRegistry::Settings.PublisherNamespace;
	FString Language = "en-US";
	FString Region = "US";

	GUsersMail.Add(TEXT("Subs1SDKTest@example.com"));
	GUsersMail.Add(TEXT("Subs2SDKTest@example.com"));
	GUsersMail.Add(TEXT("Subs3SDKTest@example.com"));

	bool clientLoginSuccess = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&clientLoginSuccess]()
	{
		clientLoginSuccess = true;
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Client Successfully Login."));
	}), SubscriptionErrorHandler);
	Waiting(clientLoginSuccess, "Waiting for client login.");
	check(clientLoginSuccess);

	bool bHasPublishedStore = false;
	FStoreInfo PublishedStoreInfo;
	Ecommerce_PublishedStore_Get(PublisherNamespace, THandler<FStoreInfo>::CreateLambda([&bHasPublishedStore, &PublishedStoreInfo](const FStoreInfo& Result)
	{
		PublishedStoreInfo = Result;
		bHasPublishedStore = true;
	}), SubscriptionErrorHandler);
	Waiting(bHasPublishedStore, "Waiting for get published store");
	check(bHasPublishedStore);
	FString PublishedStoreId = PublishedStoreInfo.storeId;
	
	GAppTypeItemSku = "sdktestSkuApp001";
	FString appTypeItemId;
	FItemFullInfo appTypeInfo;
	bool bHasAppTypeItem = false;
	bool bDoneGetItemBySku = false;
	Ecommerce_GetItem_BySKU(*GetPublisherNamespace(), PublishedStoreId, GAppTypeItemSku, true, THandler<FItemFullInfo>::CreateLambda([&bDoneGetItemBySku, &bHasAppTypeItem, &appTypeInfo](const FItemFullInfo& Result)
	{
		bDoneGetItemBySku = true;
		bHasAppTypeItem = true;
		appTypeInfo = Result;
	}), FErrorHandler::CreateLambda([&bDoneGetItemBySku](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Error Get Item By SKU"));
		bDoneGetItemBySku = true;
	}));
	Waiting(bDoneGetItemBySku, "Waiting to get item by sku");
	check(bDoneGetItemBySku);
	if (bHasAppTypeItem) 
	{
		appTypeItemId = appTypeInfo.itemId;
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Has APP Item type on store"));
	}

	GSubsTypeItemSku = "sdktestSkuSubs001";
	FString SubsTypeItemId;
	FItemFullInfo SubsTypeInfo;
	bool bHasSubsTypeItem = false;
	bDoneGetItemBySku = false;
	Ecommerce_GetItem_BySKU(*GetPublisherNamespace(), PublishedStoreId, GSubsTypeItemSku, true, THandler<FItemFullInfo>::CreateLambda([&bDoneGetItemBySku, &bHasSubsTypeItem, &SubsTypeInfo](const FItemFullInfo& Result)
	{
		bDoneGetItemBySku = true;
		bHasSubsTypeItem = true;
		SubsTypeInfo = Result;
	}), FErrorHandler::CreateLambda([&bDoneGetItemBySku](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Error Get Item By SKU"));
		bDoneGetItemBySku = true;
	}));
	Waiting(bDoneGetItemBySku, "Waiting to get item by sku");
	check(bDoneGetItemBySku);
	if (bHasSubsTypeItem)
	{
		SubsTypeItemId = SubsTypeInfo.itemId;
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Has Subscription Item type on store"));
	}

	bool bNeedToCreateItem = !bHasAppTypeItem || !bHasSubsTypeItem;
	if (bNeedToCreateItem) 
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

		bool bDoneCreateTempStore = false;
		FStoreInfo TempStoreInfo;
		FString TempStoreId;
		Ecommerce_Store_Create(PublisherNamespace, TemporaryStore, THandler<FStoreInfo>::CreateLambda([&bDoneCreateTempStore, &TempStoreInfo](const FStoreInfo& Result)
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Create temp store success"));
			bDoneCreateTempStore = true;
			TempStoreInfo = Result;
		}), SubscriptionErrorHandler);
		Waiting(bDoneCreateTempStore, "Waiting to create temp store");
		TempStoreId = TempStoreInfo.storeId;
		check(bDoneCreateTempStore);

		bool bClonePublishedStore = false;
		Ecommerce_Store_Clone(PublisherNamespace, PublishedStoreId, TempStoreId, FSimpleDelegate::CreateLambda([&bClonePublishedStore]()
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Clone published store"));
			bClonePublishedStore = true;
		}), SubscriptionErrorHandler);
		Waiting(bClonePublishedStore, "Waiting for clone published store...");
		check(bClonePublishedStore);

		FCreateRegionDataItem regionData
		{
			0,
			0,
			0,
			0,
			"USD",
			EAccelByteItemCurrencyType::REAL,
			PublisherNamespace,
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
		TMap<FString, FLocalization> ItemLocalization;
		ItemLocalization.Add(TemporaryStore.defaultLanguage, localization);

		FString AppId = "ue4SdkTest001";
		// if doesn't have "APP" item on published store
		if (!bHasAppTypeItem) 
		{
			FItemCreateRequest AppItemRequest;
			AppItemRequest.itemType = EAccelByteItemType::APP;
			AppItemRequest.name = "SDK UE4 App Item";
			AppItemRequest.appId = APPID;
			AppItemRequest.appType = EAccelByteAppType::GAME;
			AppItemRequest.categoryPath = "/games";
			AppItemRequest.entitlementType = EAccelByteEntitlementType::DURABLE;
			AppItemRequest.sku = GAppTypeItemSku;
			AppItemRequest.status = EAccelByteItemStatus::ACTIVE;
			AppItemRequest.regionData = FRegionDataUS{ {regionData} };
			AppItemRequest.localizations = ItemLocalization;
			AppItemRequest.maxCountPerUser = 1;

			bool bCreateAppItemSuccess = false;
			FItemFullInfo appItemInfo;
			Ecommerce_Item_Create(PublisherNamespace, AppItemRequest, TempStoreId, THandler<FItemFullInfo>::CreateLambda([&bCreateAppItemSuccess, &appItemInfo](const FItemFullInfo& Result)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Create app item success"));
				bCreateAppItemSuccess = true;
				appItemInfo = Result;
			}), SubscriptionErrorHandler);
			Waiting(bCreateAppItemSuccess, "Waiting for app item created...");
			check(bCreateAppItemSuccess);
			appTypeItemId = appItemInfo.itemId;
		}

		// if doesn't have "SUBSCRIPTION" item on published store
		if (!bHasSubsTypeItem)
		{
			TArray<FString> EmptyStrings;

			FRecurring SubscriptionRequest;
			SubscriptionRequest.cycle = EAccelByteSubscriptionCycle::YEARLY;
			SubscriptionRequest.graceDays = 1;
			SubscriptionRequest.fixedFreeDays = 0;
			SubscriptionRequest.fixedTrialCycles = 0;

			FItemCreateRequest SubsItemRequest;
			SubsItemRequest.itemType = EAccelByteItemType::SUBSCRIPTION;
			SubsItemRequest.name = "SDK UE4 Test Yearly subs";
			SubsItemRequest.entitlementType = EAccelByteEntitlementType::DURABLE;
			SubsItemRequest.appType = EAccelByteAppType::GAME;
			SubsItemRequest.categoryPath = "/subscription";
			SubsItemRequest.sku = GSubsTypeItemSku;
			SubsItemRequest.status = EAccelByteItemStatus::ACTIVE;
			SubsItemRequest.recurring = SubscriptionRequest;
			SubsItemRequest.maxCountPerUser = 1;
			SubsItemRequest.regionData = FRegionDataUS{ {regionData} };
			SubsItemRequest.localizations = ItemLocalization;

			bool createSubsItemSuccess = false;
			FItemFullInfo appItemInfo;
			Ecommerce_Item_Create(PublisherNamespace, SubsItemRequest, TempStoreId, THandler<FItemFullInfo>::CreateLambda([&createSubsItemSuccess, &appItemInfo](const FItemFullInfo& Result)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Create subs item success"));
				createSubsItemSuccess = true;
				appItemInfo = Result;
			}), SubscriptionErrorHandler);
			Waiting(createSubsItemSuccess, "Waiting for subs item created...");
			check(createSubsItemSuccess);
			SubsTypeItemId = appItemInfo.itemId;
		}

		// published temp store id to published store
		bool bPublishTempStore = false;
		Ecommerce_Store_Clone(PublisherNamespace, TempStoreId, "", FSimpleDelegate::CreateLambda([&bPublishTempStore]()
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Published temp store success"));
			bPublishTempStore = true;
		}), SubscriptionErrorHandler);
		Waiting(bPublishTempStore, "Waiting for publish temp store...");
		check(bPublishTempStore);
	}

	//User Creation (3 users)

	bool bLoginWithDeviceSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success login with device id"));
		bLoginWithDeviceSuccess = true;
	}), SubscriptionErrorHandler);

	Waiting(bLoginWithDeviceSuccess, "Waiting for Login...");
	check(bLoginWithDeviceSuccess);

	bool bDoneSearchUser = false;
	bool bIsUserExist = false;
	for (int i = 0; i < GUsersMail.Num(); i++)
	{
		bDoneSearchUser = false;
		bIsUserExist = false;
		FPagedPublicUsersInfo ReceivedUserData;
		FRegistry::User.SearchUsers(GUsersMail[i], EAccelByteSearchType::EMAILADDRESS, THandler<FPagedPublicUsersInfo>::CreateLambda([&bDoneSearchUser, &ReceivedUserData](const FPagedPublicUsersInfo& Result)
		{
			bDoneSearchUser = true;
			ReceivedUserData = Result;
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Search User"));
			
		}), SubscriptionErrorHandler);
		Waiting(bDoneSearchUser, "Waiting for search user");
		check(bDoneSearchUser);

		if (ReceivedUserData.Data.Num() > 0)
		{
			bIsUserExist = true;
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User is exist"));
		}

		if (!bIsUserExist) 
		{
			//create new user
			FString EmailAddress = GUsersMail[i];
			FDateTime const DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 22));
			FString const Format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
			FString Username = FString("Subs UE4 " + FString::FromInt(i+1));
			bool bRegisterSuccess = false;
			bool bDoneRegisterUser = false;
			FRegistry::User.Register(GUsersMail[i], GUserPassword, Username, "MG", Format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccess, &bDoneRegisterUser](const FRegisterResponse& Result)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Register User"));
				bRegisterSuccess = true;
				bDoneRegisterUser = true;
			}), SubscriptionErrorHandler);

			Waiting(bDoneRegisterUser, "Waiting for register user");
			check(bDoneRegisterUser);
			check(bRegisterSuccess);
		}
	}

	UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout device id"));
	FRegistry::User.ForgetAllCredentials();

	TArray<FString> userPublisherIds;
	for (int i = 0; i < GUsersMail.Num(); i++) 
	{
		bool bLoginSuccess = false;;
		FRegistry::User.LoginWithUsername(GUsersMail[i], GUserPassword, FVoidHandler::CreateLambda([&bLoginSuccess]()
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Login"));
			bLoginSuccess = true;
		}), SubscriptionErrorHandler);
		Waiting(bLoginSuccess, "Waiting for login");
		check(bLoginSuccess);

		bool doneGetUserMap = false;
		FUserMapResponse userMap;
		User_Get_User_Mapping(FRegistry::Credentials.GetUserId(), THandler<FUserMapResponse>::CreateLambda([&doneGetUserMap, &userMap](const FUserMapResponse& Result) 
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Get User Map"));
			doneGetUserMap = true;
			userMap = Result;
		}), SubscriptionErrorHandler);
		Waiting(doneGetUserMap, "Waiting for get user map");
		check(doneGetUserMap);

		userPublisherIds.Add(userMap.userId);

		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User"));
		FRegistry::User.ForgetAllCredentials();
	}

	//grant APP type into User 1
	FAccelByteModelsEntitlementGrant GrantRequest;
	GrantRequest.ItemNamespace = PublisherNamespace;
	GrantRequest.ItemId = appTypeItemId;
	GrantRequest.GrantedCode = "02501205";
	GrantRequest.Quantity = 1;
	GrantRequest.Source = EAccelByteEntitlementSource::REDEEM_CODE;
	GrantRequest.Region = "US";
	GrantRequest.Language = "en-US";

	TArray<FAccelByteModelsEntitlementGrant> EntitlementGrants;
	EntitlementGrants.Add(GrantRequest);

	// login into user 1, and check if has app entitlement or not
	bool bLoginSuccess = false;;
	FRegistry::User.LoginWithUsername(GUsersMail[0], GUserPassword, FVoidHandler::CreateLambda([&bLoginSuccess]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Login"));
		bLoginSuccess = true;
	}), SubscriptionErrorHandler);
	Waiting(bLoginSuccess, "Waiting for login");

	bool doneGetAppEntitlement = false;
	FAccelByteModelsEntitlementOwnership appOwnership;
	FRegistry::Entitlement.GetUserEntitlementOwnershipBySku(
		GAppTypeItemSku,
		THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda([&doneGetAppEntitlement, &appOwnership](const FAccelByteModelsEntitlementOwnership& Result)
	{
		doneGetAppEntitlement = true;
		appOwnership = Result;
	}), FErrorHandler::CreateLambda([&doneGetAppEntitlement](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User doesn't have APP."));
		doneGetAppEntitlement = true;
	}));
	Waiting(doneGetAppEntitlement, "Waiting for get subs entitlement");
	check(doneGetAppEntitlement);

	if (!appOwnership.Owned)
	{
		bool grantAppTypeSuccess = false;
		TArray<FAccelByteModelsStackableEntitlementInfo> grantResultInfo;
		Ecommerce_GrantUserEntitlements(
			PublisherNamespace,
			userPublisherIds[0],
			EntitlementGrants,
			THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>::CreateLambda([&grantAppTypeSuccess, &grantResultInfo](const TArray<FAccelByteModelsStackableEntitlementInfo>& Result)
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Grant APP Item to User 1"));
			grantAppTypeSuccess = true;
			grantResultInfo = Result;
		}), SubscriptionErrorHandler);
		Waiting(grantAppTypeSuccess, "Waiting for grant app type");
		check(grantAppTypeSuccess);
	}

	// login into user 2, and check if has subs entitlement or not
	UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User"));
	FRegistry::User.ForgetAllCredentials();
	bLoginSuccess = false;
	FRegistry::User.LoginWithUsername(GUsersMail[1], GUserPassword, FVoidHandler::CreateLambda([&bLoginSuccess]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Login"));
		bLoginSuccess = true;
	}), SubscriptionErrorHandler);
	Waiting(bLoginSuccess, "Waiting for login");

	bool doneGetUserEntitlement = false;
	FAccelByteModelsEntitlementOwnership ownership;
	FRegistry::Entitlement.GetUserEntitlementOwnershipBySku(
		GSubsTypeItemSku, 
		THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda([&doneGetUserEntitlement, &ownership](const FAccelByteModelsEntitlementOwnership& Result)
		{
			doneGetUserEntitlement = true;
			ownership = Result;
		}), FErrorHandler::CreateLambda([&doneGetUserEntitlement](int32 ErrorCode, FString ErrorMessage)
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User doesn't have Subscription."));
			doneGetUserEntitlement = true;
		}));
	Waiting(doneGetUserEntitlement, "Waiting for get subs entitlement");
	check(doneGetUserEntitlement);

	FFreeSubscriptionRequest SubsRequest;
	SubsRequest.ItemId = SubsTypeItemId;
	SubsRequest.GrantDays = 365;
	SubsRequest.Source = "ue4 sdk test";
	SubsRequest.Reason = "testing purpose";
	SubsRequest.Region = "US";
	SubsRequest.Language = "en-US";

	if (!ownership.Owned) 
	{
		bool bDoneGrantSubs = false;
		FItemFullInfo subsItemInfo;
		Subscription_GrantFreeSubs(userPublisherIds[1], SubsRequest, THandler<FItemFullInfo>::CreateLambda([&bDoneGrantSubs, &subsItemInfo](const FItemFullInfo& Result)
		{
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Grant free subs to User 1 success"));
			bDoneGrantSubs = true;
			subsItemInfo = Result;
		}), SubscriptionErrorHandler);
		Waiting(bDoneGrantSubs, "Waiting for grant free subs");
		check(bDoneGrantSubs);
	}

	UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User 1"));
	FRegistry::User.ForgetAllCredentials();

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CheckUserEligibleOwnedAppId, "AccelByte.Tests.Subscription.B.CheckUserEligibleOwnedAppId", AutomationFlagMaskSubscription);
bool CheckUserEligibleOwnedAppId::RunTest(const FString& Parameters)
{
	FString OriAppId = FRegistry::Settings.AppId;
	FRegistry::Settings.AppId = APPID;

	UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User"));
	FRegistry::User.ForgetAllCredentials();

	//Using User 1
	bool bLoginFinish = false;
	FRegistry::User.LoginWithUsername(GUsersMail[0], GUserPassword, FVoidHandler::CreateLambda([&bLoginFinish]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User Login Success"));
		bLoginFinish = true;
	}), SubscriptionErrorHandler);
	Waiting(bLoginFinish, "Waiting user login...");

	bool bCheckEligibleFinish = false;
	bool bCheckEligibleResult = false;
	FRegistry::User.GetUserEligibleToPlay(THandler<bool>::CreateLambda([&bCheckEligibleFinish, &bCheckEligibleResult](bool result) 
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Check Eligibility Finish"));
		bCheckEligibleFinish = true;
		bCheckEligibleResult = result;
	}), SubscriptionErrorHandler);
	Waiting(bCheckEligibleFinish, "Waiting checking user eligibility");

	AB_TEST_TRUE(bCheckEligibleResult);
	FRegistry::Settings.AppId = OriAppId;

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CheckUserEligibleOwnedSubs, "AccelByte.Tests.Subscription.C.CheckUserEligibleOwnedSubs", AutomationFlagMaskSubscription);
bool CheckUserEligibleOwnedSubs::RunTest(const FString& Parameters)
{
	FString OriAppId = FRegistry::Settings.AppId;
	FRegistry::Settings.AppId = APPID;

	UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User"));
	FRegistry::User.ForgetAllCredentials();

	//Using User 2
	bool bLoginFinish = false;
	FRegistry::User.LoginWithUsername(GUsersMail[1], GUserPassword, FVoidHandler::CreateLambda([&bLoginFinish]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User Login Success"));
		bLoginFinish = true;
	}), SubscriptionErrorHandler);
	Waiting(bLoginFinish, "Waiting user login...");

	bool bGetSubsOwnership = false;
	FAccelByteModelsEntitlementOwnership ownership;
	FRegistry::Entitlement.GetUserEntitlementOwnershipBySku(
		GSubsTypeItemSku,
		THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda([&bGetSubsOwnership, &ownership](const FAccelByteModelsEntitlementOwnership& Result)
	{
		bGetSubsOwnership = true;
		ownership = Result;
	}), FErrorHandler::CreateLambda([&bGetSubsOwnership](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User doesn't have Subscription."));
		bGetSubsOwnership = true;
	}));
	Waiting(bGetSubsOwnership, "Waiting for get subs entitlement");
	AB_TEST_TRUE(bGetSubsOwnership);
	AB_TEST_TRUE(ownership.Owned);

	FRegistry::Settings.AppId = OriAppId;

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(CheckUserEligibleOwnedNone, "AccelByte.Tests.Subscription.E.CheckUserEligibleOwnedNone", AutomationFlagMaskSubscription);
bool CheckUserEligibleOwnedNone::RunTest(const FString& Parameters)
{
	FString OriAppId = FRegistry::Settings.AppId;
	FRegistry::Settings.AppId = APPID;

	UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User"));
	FRegistry::User.ForgetAllCredentials();

	//Using User 3
	bool bLoginFinish = false;
	FRegistry::User.LoginWithUsername(GUsersMail[2], GUserPassword, FVoidHandler::CreateLambda([&bLoginFinish]()
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User Login Success"));
		bLoginFinish = true;
	}), SubscriptionErrorHandler);
	Waiting(bLoginFinish, "Waiting user login...");

	bool bCheckEligibleFinish = false;
	bool bCheckEligibleResult = true;
	FRegistry::User.GetUserEligibleToPlay(THandler<bool>::CreateLambda([&bCheckEligibleFinish, &bCheckEligibleResult](bool result)
	{
		UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Check Eligibility Finish"));
		bCheckEligibleFinish = true;
		bCheckEligibleResult = result;
	}), SubscriptionErrorHandler);
	Waiting(bCheckEligibleFinish, "Waiting checking user eligibility");

	AB_TEST_FALSE(bCheckEligibleResult);
	FRegistry::Settings.AppId = OriAppId;
	return true;
}