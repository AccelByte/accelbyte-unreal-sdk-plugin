// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
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
#include "UserTestAdmin.h"
#include "EcommerceTestAdmin.h"
#include "SubscriptionTestAdmin.h"

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
	UE_LOG(LogAccelByteSubscriptionTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

BEGIN_DEFINE_SPEC(FSubscriptionTestSpec, "AccelByte.Tests.Subscription",  EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	const FString APPID = "PrincessAndromeda";
	FString GAppTypeItemSku;
	FString GSubsTypeItemSku;

	const TArray<FString> GUsersMail = { "SubsUser1SDKTest@example.com", "SubsUser2SDKTest@example.com", "SubsUser3SDKTest@example.com" };
	const FString GUserPassword = "Password+123";
	FString TitleStoreId = "ACCELBYTE STORE";
	FString SubscriptionTestStoreId;
END_DEFINE_SPEC(FSubscriptionTestSpec)

void FSubscriptionTestSpec::Define()
{	
	Describe("A_Setup", [this]()
	{
		It("Should provide test setup config", [this]()
		{
			FString GameNamespace = FRegistry::Settings.Namespace;
			FString PublisherNamespace = FRegistry::Settings.PublisherNamespace;
			FString Language = "en-US";
			FString Region = "US";

			bool clientLoginSuccess = false;
			FRegistry::ServerOauth2.LoginWithClientCredentials(FVoidHandler::CreateLambda([&clientLoginSuccess]()
			{
				clientLoginSuccess = true;
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Client Successfully Login."));
			}), SubscriptionErrorHandler);
			WaitUntil(clientLoginSuccess, "Waiting for client login.");

			AB_TEST_TRUE(clientLoginSuccess);

			bool bHasPublishedStore = false;
			FStoreInfo PublishedStoreInfo;
			AdminGetEcommercePublishedStore(PublisherNamespace, THandler<FStoreInfo>::CreateLambda([&bHasPublishedStore, &PublishedStoreInfo](const FStoreInfo& Result)
			{
				PublishedStoreInfo = Result;
				bHasPublishedStore = true;
			}), SubscriptionErrorHandler);
			WaitUntil(bHasPublishedStore, "Waiting for get published store");

			AB_TEST_TRUE(bHasPublishedStore);
			FString PublishedStoreId = PublishedStoreInfo.storeId;
			
			GAppTypeItemSku = TEXT("APPG0101D");
			FString appTypeItemId;
			bool bDoneGetItemBySku = false;
			AdminGetEcommerceItemBySKU(*GetPublisherNamespace(), PublishedStoreId, GAppTypeItemSku, true,
				THandler<FItemFullInfo>::CreateLambda([&bDoneGetItemBySku, &appTypeItemId](const FItemFullInfo& Result)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Has APP Item type on store"));
				appTypeItemId = Result.itemId;
				bDoneGetItemBySku = true;
			}), FErrorHandler::CreateLambda([&bDoneGetItemBySku](int32 ErrorCode, FString ErrorMessage)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Error Get Item By SKU. Does not have APP Item type on store"));
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
				bDoneGetItemBySku = true;
			}));
			WaitUntil(bDoneGetItemBySku, "Waiting to get item by sku");
			
			AB_TEST_TRUE(bDoneGetItemBySku);

			GSubsTypeItemSku = TEXT("apiseed1635893021d5ec135");
			FString SubsTypeItemId;
			bDoneGetItemBySku = false;
			AdminGetEcommerceItemBySKU(*GetPublisherNamespace(), PublishedStoreId, GSubsTypeItemSku, true,
				THandler<FItemFullInfo>::CreateLambda([&bDoneGetItemBySku, &SubsTypeItemId](const FItemFullInfo& Result)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Has Subscription Item type on store"));
				SubsTypeItemId = Result.itemId;
				bDoneGetItemBySku = true;
			}), FErrorHandler::CreateLambda([&bDoneGetItemBySku](int32 ErrorCode, FString ErrorMessage)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Error Get Item By SKU. Does not have Subscription Item type on store"));
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
				bDoneGetItemBySku = true;
			}));
			WaitUntil(bDoneGetItemBySku, "Waiting to get item by sku");

			AB_TEST_TRUE(bDoneGetItemBySku);

			if (appTypeItemId.IsEmpty() || SubsTypeItemId.IsEmpty())
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Creating item."));

				FStoreCreateRequest TemporaryStore
				{
					TitleStoreId,
					"Create from UE4 SDK Subscription Test",
					{"en"},
					{"US"},
					"en-US",
					"US"
				};

				// Fetch all publisher stores.
				bool bGetAllStoreSuccess = false;
				TArray<FStoreInfo> GetAllResult;
				AdminGetEcommerceStoreAll(PublisherNamespace, THandler<TArray<FStoreInfo>>::CreateLambda([&PublisherNamespace, &bGetAllStoreSuccess, &GetAllResult](const TArray<FStoreInfo>& Result)
				{
					UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Get %s stores Success"), *PublisherNamespace);
					GetAllResult = Result;
					bGetAllStoreSuccess = true;
				}), SubscriptionErrorHandler);
				WaitUntil(bGetAllStoreSuccess, "Waiting for get all stores...");

				AB_TEST_TRUE(bGetAllStoreSuccess);

				int32 DraftStoreNum = 0;
				for (const FStoreInfo& Store : GetAllResult)
				{
					DraftStoreNum += Store.published ? 0 : 1;
				}

				if (DraftStoreNum > 0)
				{
					FStoreInfo* ExistingStore = GetAllResult.FindByPredicate([this](const FStoreInfo& Store)
					{
						return Store.published == false && Store.title == TitleStoreId;
					});
					if (ExistingStore)
					{
						SubscriptionTestStoreId = ExistingStore->storeId;
						UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Found %s draft store for Subscription testing"), *TitleStoreId);
					}
					else
					{
						UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Not found %s draft store for Subscription testing"), *TitleStoreId);
						FStoreInfo* TopDraftStore = GetAllResult.FindByPredicate([](const FStoreInfo& Store)
						{
							return Store.published == false;
						});
						SubscriptionTestStoreId = TopDraftStore->storeId;				
						UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Use %s (%s) as top draf store for testing"), *TopDraftStore->title, *TopDraftStore->storeId);
					}
				}
				else
				{
					bool bDoneCreateTempStore = false;
					FStoreInfo TempStoreInfo;
					AdminCreateEcommerceStore(PublisherNamespace, TemporaryStore, THandler<FStoreInfo>::CreateLambda([&bDoneCreateTempStore, &TempStoreInfo](const FStoreInfo& Result)
					{
						UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Create temp store success"));
						bDoneCreateTempStore = true;
						TempStoreInfo = Result;
					}), SubscriptionErrorHandler);
					WaitUntil(bDoneCreateTempStore, "Waiting to create temp store");
					SubscriptionTestStoreId = TempStoreInfo.storeId;
					AB_TEST_TRUE(bDoneCreateTempStore);

					bool bClonePublishedStore = false;
					AdminCloneEcommerceStore(PublisherNamespace, PublishedStoreId, SubscriptionTestStoreId, FSimpleDelegate::CreateLambda([&bClonePublishedStore]()
					{
						UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Clone published store"));
						bClonePublishedStore = true;
					}), SubscriptionErrorHandler);
					WaitUntil(bClonePublishedStore, "Waiting for clone published store...");
					AB_TEST_TRUE(bClonePublishedStore);
				}

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
					"Princess Andromeda",
					"Safe the the princess to save the world",
					"Princess Andromeda is having great power but she is weak physically. Your task is keeping her safe and give her space to show up her ability"
				};
				TMap<FString, FLocalization> ItemLocalization;
				ItemLocalization.Add(TemporaryStore.defaultLanguage, localization);
				
				// if doesn't have "APP" item on published store
				if (appTypeItemId.IsEmpty())
				{
					FItemCreateRequest AppItemRequest;
					AppItemRequest.itemType = EAccelByteItemType::APP;
					AppItemRequest.name = "Princess Andromeda";
					AppItemRequest.appId = APPID;
					AppItemRequest.appType = EAccelByteAppType::GAME;
					AppItemRequest.SeasonType = EAccelByteSeasonType::PASS;
					AppItemRequest.categoryPath = "/games";
					AppItemRequest.entitlementType = EAccelByteEntitlementType::DURABLE;
					AppItemRequest.sku = GAppTypeItemSku;
					AppItemRequest.status = EAccelByteItemStatus::ACTIVE;
					AppItemRequest.regionData = FRegionDataUS{ {regionData} };
					AppItemRequest.localizations = ItemLocalization;
					AppItemRequest.maxCountPerUser = 1;

					bool bCreateAppItemSuccess = false;
					FItemFullInfo appItemInfo;
					AdminCreateEcommerceItem(PublisherNamespace, SubscriptionTestStoreId, AppItemRequest,
						THandler<FItemFullInfo>::CreateLambda([&bCreateAppItemSuccess, &appItemInfo](const FItemFullInfo& Result) {
							UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Create app item success"));
							bCreateAppItemSuccess = true;
							appItemInfo = Result;
						}), SubscriptionErrorHandler);
					WaitUntil(bCreateAppItemSuccess, "Waiting for app item created...");
					AB_TEST_TRUE(bCreateAppItemSuccess);
					appTypeItemId = appItemInfo.itemId;
				}

				// if doesn't have "SUBSCRIPTION" item on published store
				if (SubsTypeItemId.IsEmpty())
				{
					TArray<FString> EmptyStrings;

					FRecurring SubscriptionRequest;
					SubscriptionRequest.cycle = EAccelByteSubscriptionCycle::YEARLY;
					SubscriptionRequest.graceDays = 1;
					SubscriptionRequest.fixedFreeDays = 0;
					SubscriptionRequest.fixedTrialCycles = 0;

					FItemCreateRequest SubsItemRequest;
					SubsItemRequest.itemType = EAccelByteItemType::SUBSCRIPTION;
					SubsItemRequest.name = "Princess Andromeda Subscription";
					SubsItemRequest.entitlementType = EAccelByteEntitlementType::DURABLE;
					SubsItemRequest.appType = EAccelByteAppType::GAME;
					SubsItemRequest.SeasonType = EAccelByteSeasonType::PASS;
					SubsItemRequest.categoryPath = "/subscription";
					SubsItemRequest.sku = GSubsTypeItemSku;
					SubsItemRequest.status = EAccelByteItemStatus::ACTIVE;
					SubsItemRequest.recurring = SubscriptionRequest;
					SubsItemRequest.maxCountPerUser = 1;
					SubsItemRequest.regionData = FRegionDataUS{ {regionData} };
					SubsItemRequest.localizations = ItemLocalization;

					bool createSubsItemSuccess = false;
					FItemFullInfo appItemInfo;
					AdminCreateEcommerceItem(PublisherNamespace, SubscriptionTestStoreId, SubsItemRequest,
						THandler<FItemFullInfo>::CreateLambda([&createSubsItemSuccess, &appItemInfo](const FItemFullInfo& Result) {
							UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Create subs item success"));
							appItemInfo = Result;
							createSubsItemSuccess = true;
						}), SubscriptionErrorHandler);
					WaitUntil(createSubsItemSuccess, "Waiting for subs item created...");
					AB_TEST_TRUE(createSubsItemSuccess);
					SubsTypeItemId = appItemInfo.itemId;
				}

				// published temp store id to published store
				bool bPublishTempStore = false;
				AdminCloneEcommerceStore(PublisherNamespace, SubscriptionTestStoreId, "", FSimpleDelegate::CreateLambda([&bPublishTempStore]()
				{
					UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Published temp store success"));
					bPublishTempStore = true;
				}), SubscriptionErrorHandler);
				WaitUntil(bPublishTempStore, "Waiting for publish temp store...");
				AB_TEST_TRUE(bPublishTempStore);
			}

			//User Creation (3 users)
			bool bLoginWithDeviceSuccess = false;
			FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success login with device id"));
				bLoginWithDeviceSuccess = true;
			}), SubscriptionErrorHandler);

			WaitUntil(bLoginWithDeviceSuccess, "Waiting for Login...");
			AB_TEST_TRUE(bLoginWithDeviceSuccess);

			bool bDoneSearchUser = false;
			bool bIsUserExist = false;
			for (int i = 0; i < GUsersMail.Num(); i++)
			{
				bDoneSearchUser = false;
				bIsUserExist = false;
				FPagedPublicUsersInfo ReceivedUserData;
				FString DisplayName = FString("SubscriptionUE4" + FString::FromInt(i + 1));
				FRegistry::User.SearchUsers(DisplayName, EAccelByteSearchType::DISPLAYNAME, THandler<FPagedPublicUsersInfo>::CreateLambda([&bDoneSearchUser, &ReceivedUserData](const FPagedPublicUsersInfo& Result)
				{
					bDoneSearchUser = true;
					ReceivedUserData = Result;
					UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Search User"));
					
				}), SubscriptionErrorHandler);
				WaitUntil(bDoneSearchUser, "Waiting for search user");
				AB_TEST_TRUE(bDoneSearchUser);

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
					bool bRegisterSuccess = false;
					bool bDoneRegisterUser = false;
					FRegistry::User.Register(GUsersMail[i], GUserPassword, DisplayName, "MG", Format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccess, &bDoneRegisterUser](const FRegisterResponse& Result)
					{
						UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Register User"));
						bRegisterSuccess = true;
						bDoneRegisterUser = true;
					}), SubscriptionErrorHandler);

					WaitUntil(bDoneRegisterUser, "Waiting for register user");
					AB_TEST_TRUE(bDoneRegisterUser);
					AB_TEST_TRUE(bRegisterSuccess);
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
				}), FCustomErrorHandler::CreateLambda([&](int32 Code, const FString& Message, const FJsonObject& ErrorJson)
				{
					UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Login Failed. Error Code: %d, Message: %s"), Code, *Message);
				}));
				WaitUntil(bLoginSuccess, "Waiting for login");
				AB_TEST_TRUE(bLoginSuccess);

				bool doneGetUserMap = false;
				FUserMapResponse userMap;
				AdminGetUserMap(FRegistry::Credentials.GetUserId(), THandler<FUserMapResponse>::CreateLambda([&doneGetUserMap, &userMap](const FUserMapResponse& Result) 
				{
					UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Get User Map"));
					doneGetUserMap = true;
					userMap = Result;
				}), SubscriptionErrorHandler);
				WaitUntil(doneGetUserMap, "Waiting for get user map");
				AB_TEST_TRUE(doneGetUserMap);

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
			}), FCustomErrorHandler::CreateLambda([&](int32 Code, const FString& Message, const FJsonObject& ErrorJson)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Login Failed. Error Code: %d, Message: %s"), Code, *Message);
			}));
			WaitUntil(bLoginSuccess, "Waiting for login");

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
			WaitUntil(doneGetAppEntitlement, "Waiting for get subs entitlement");
			AB_TEST_TRUE(doneGetAppEntitlement);

			if (!appOwnership.Owned)
			{
				bool grantAppTypeSuccess = false;
				TArray<FAccelByteModelsStackableEntitlementInfo> grantResultInfo;
				AdminGrantEcommerceUserEntitlements(
					PublisherNamespace,
					userPublisherIds[0],
					EntitlementGrants,
					THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>::CreateLambda([&grantAppTypeSuccess, &grantResultInfo](const TArray<FAccelByteModelsStackableEntitlementInfo>& Result)
				{
					UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Grant APP Item to User 1"));
					grantAppTypeSuccess = true;
					grantResultInfo = Result;
				}), SubscriptionErrorHandler);
				WaitUntil(grantAppTypeSuccess, "Waiting for grant app type");
				AB_TEST_TRUE(grantAppTypeSuccess);
			}

			// login into user 2, and check if has subs entitlement or not
			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User"));
			FRegistry::User.ForgetAllCredentials();

			bLoginSuccess = false;
			FRegistry::User.LoginWithUsername(GUsersMail[1], GUserPassword, FVoidHandler::CreateLambda([&bLoginSuccess]()
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Success Login"));
				bLoginSuccess = true;
			}), FCustomErrorHandler::CreateLambda([&](int32 Code, const FString& Message, const FJsonObject& ErrorJson)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Login Failed. Error Code: %d, Message: %s"), Code, *Message);
			}));
			WaitUntil(bLoginSuccess, "Waiting for login");

			bool doneGetUserEntitlement = false;
			FAccelByteModelsEntitlementOwnership ownership;
			FRegistry::Entitlement.GetUserEntitlementOwnershipBySku(
				GSubsTypeItemSku, 
				THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda([&doneGetUserEntitlement, &ownership](const FAccelByteModelsEntitlementOwnership& Result)
				{
					UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("GetUserEntitlementOwnershipBySku Success"));
					ownership = Result;
					doneGetUserEntitlement = true;
				}), FErrorHandler::CreateLambda([&doneGetUserEntitlement](int32 ErrorCode, FString ErrorMessage)
				{
					UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User doesn't have Subscription."));
					UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
					doneGetUserEntitlement = true;
				}));
			WaitUntil(doneGetUserEntitlement, "Waiting for get subs entitlement");
			AB_TEST_TRUE(doneGetUserEntitlement);

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
				AdminGrantSubscriptionFree(userPublisherIds[1], SubsRequest, THandler<FItemFullInfo>::CreateLambda([&bDoneGrantSubs, &subsItemInfo](const FItemFullInfo& Result)
				{
					UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Grant free subs to User 1 success"));
					bDoneGrantSubs = true;
					subsItemInfo = Result;
				}), SubscriptionErrorHandler);
				WaitUntil(bDoneGrantSubs, "Waiting for grant free subs");
				AB_TEST_TRUE(bDoneGrantSubs);
			}

			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User 1"));
			FRegistry::User.ForgetAllCredentials();
			
			return true;
		});
	});

	Describe("CheckUserEligibleOwnedAppId", [this]()
	{
		It("Should check Owned App Id by User", [this]()
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
			}), FCustomErrorHandler::CreateLambda([&](int32 Code, const FString& Message, const FJsonObject& ErrorJson)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Login Failed. Error Code: %d, Message: %s"), Code, *Message);
			}));
			WaitUntil(bLoginFinish, "Waiting user login...");

			bool bCheckEligibleFinish = false;
			bool bCheckEligibleResult = false;
			FRegistry::User.GetUserEligibleToPlay(THandler<bool>::CreateLambda([&bCheckEligibleFinish, &bCheckEligibleResult](bool result) 
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Check Eligibility Finish"));
				bCheckEligibleFinish = true;
				bCheckEligibleResult = result;
			}), SubscriptionErrorHandler);
			WaitUntil(bCheckEligibleFinish, "Waiting checking user eligibility");

			AB_TEST_TRUE(bCheckEligibleResult);
			FRegistry::Settings.AppId = OriAppId;
			
			return true;

		});
	});

	Describe("CheckUserEligibleOwnedSubs", [this]()
	{
		It("Should check Owned Subs Item by User", [this]()
		{
			FString OriAppId = FRegistry::Settings.AppId;
			FRegistry::Settings.AppId = APPID;

			UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Logout User"));
			FRegistry::User.ForgetAllCredentials();

			//Using User 2
			bool bLoginFinish = false;
			FRegistry::User.LoginWithUsername(GUsersMail[1], GUserPassword, FVoidHandler::CreateLambda([&bLoginFinish]()
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("User 2 Login Success"));
				bLoginFinish = true;
			}), FCustomErrorHandler::CreateLambda([&](int32 Code, const FString& Message, const FJsonObject& ErrorJson)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Login Failed. Error Code: %d, Message: %s"), Code, *Message);
			}));
			WaitUntil(bLoginFinish, "Waiting user login 2...");

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
			WaitUntil(bGetSubsOwnership, "Waiting for get subs entitlement for User 2");
			AB_TEST_TRUE(bGetSubsOwnership);
			AB_TEST_TRUE(ownership.Owned);

			FRegistry::Settings.AppId = OriAppId;
			
			return true;

		});
	});

	Describe("CheckUserEligibleOwnedNone", [this]()
	{
		It("Should check Owned Item by User and get none", [this]()
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
			}), FCustomErrorHandler::CreateLambda([&](int32 Code, const FString& Message, const FJsonObject& ErrorJson)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Login Failed. Error Code: %d, Message: %s"), Code, *Message);
			}));
			WaitUntil(bLoginFinish, "Waiting user login...");

			bool bCheckEligibleFinish = false;
			bool bCheckEligibleResult = true;
			FRegistry::User.GetUserEligibleToPlay(THandler<bool>::CreateLambda([&bCheckEligibleFinish, &bCheckEligibleResult](bool result)
			{
				UE_LOG(LogAccelByteSubscriptionTest, Log, TEXT("Check Eligibility Finish"));
				bCheckEligibleFinish = true;
				bCheckEligibleResult = result;
			}), SubscriptionErrorHandler);
			WaitUntil(bCheckEligibleFinish, "Waiting checking user eligibility");

			AB_TEST_FALSE(bCheckEligibleResult);
			FRegistry::Settings.AppId = OriAppId;
			
			return true;

		});
	});
}