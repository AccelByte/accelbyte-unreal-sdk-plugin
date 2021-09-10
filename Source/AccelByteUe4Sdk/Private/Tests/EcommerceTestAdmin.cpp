// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "EcommerceTestAdmin.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "TestUtilities.h"

#pragma region SetupEcommerceCurrency

void SetupEcommerceCurrency(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	/*
	 * Check the currency that expected for integration test. If it's already created, it doesn't need to be created again.
	 * If it doesn't exist, then it will be created.
	 */
	bool bCurrencyAlreadyExist = false;
	bool bCurrencyCreated = false;
	bool bCurrencyCheckDone = false;
	AdminGetEcommerceCurrency(Variables.ExpectedCurrency.currencyCode,
		FSimpleDelegate::CreateLambda([&bCurrencyAlreadyExist, &bCurrencyCreated, &bCurrencyCheckDone]() {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CURRENCY is created already"));
			bCurrencyAlreadyExist = true;
			bCurrencyCreated = true;
			bCurrencyCheckDone = true;
		}),
		FErrorHandler::CreateLambda([&bCurrencyAlreadyExist, &bCurrencyCheckDone](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CURRENCY does not exist. Creating..."));
			bCurrencyAlreadyExist = false;
			bCurrencyCheckDone = true;
		}));
	WaitUntil(bCurrencyCheckDone, "Waiting for CURRENCY check...");

	if (!bCurrencyAlreadyExist) {
		bool bWaitingTerminated = false;
		AdminCreateEcommerceCurrency(Variables.ExpectedCurrency,
			FSimpleDelegate::CreateLambda([&bCurrencyCreated, &bWaitingTerminated]() {
				UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CURRENCY is created"));
				bCurrencyCreated = true;
				bWaitingTerminated = true;
			}),
			FErrorHandler::CreateLambda([&OnError, &bCurrencyCreated, &bWaitingTerminated](int32 Code, const FString& Message) {
				UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CURRENCY can not be created"));
				bCurrencyCreated = false;
				OnError.ExecuteIfBound(Code, Message);
				bWaitingTerminated = true;
			}));
		WaitUntil(bWaitingTerminated, "Waiting for CURRENCY creation...");
	}

	if (bCurrencyCreated) {
		OnSuccess.ExecuteIfBound();
	}
}

#pragma endregion SetupEcommerceCurrency

#pragma region SetupEcommerceStore

bool DeleteEcommerceStore(const FSimpleDelegate&, const FErrorHandler& OnError)
{
	// Delete published store (if any)
	bool bWaitingTerminated = false;
	bool bStoreDeleted = false;
	AdminDeleteEcommercePublishedStore(
		FSimpleDelegate::CreateLambda([&bStoreDeleted, &bWaitingTerminated]() {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: Published STORE is deleted"));
			bStoreDeleted = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&bStoreDeleted, &bWaitingTerminated](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: no Published STORE"));
			bStoreDeleted = true;  // Ignore if there is no published store to delete
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting for Published STORE deletion...");

	// Delete ALL draft stores
	bool bGetAllStoreSuccess = false;
	bWaitingTerminated = false;
	TArray<FStoreInfo> GetAllResult;
	AdminGetEcommerceStoreAll(
		THandler<TArray<FStoreInfo>>::CreateLambda([&bGetAllStoreSuccess, &bWaitingTerminated, &GetAllResult](const TArray<FStoreInfo>& Result) {
			GetAllResult = Result;
			bGetAllStoreSuccess = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&OnError, &bWaitingTerminated](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: STORE data can not be retrieved"));
			OnError.ExecuteIfBound(Code, Message);
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting to retrieve all STORE data...");

	for (int index = 0; index < GetAllResult.Num(); index++) {
		bool bDeleteTestingStoreSuccess = false;
		bWaitingTerminated = false;
		FString StoreId = GetAllResult[index].storeId;
		AdminDeleteEcommerceStore(StoreId,
			FSimpleDelegate::CreateLambda([&bDeleteTestingStoreSuccess, &bWaitingTerminated, &StoreId]() {
				UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: STORE %s is deleted"), *StoreId);
				bDeleteTestingStoreSuccess = true;
				bWaitingTerminated = true;
			}),
			FErrorHandler::CreateLambda([&OnError, &bWaitingTerminated, &StoreId](int32 Code, const FString& Message) {
				UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: STORE %s can not be deleted"), *StoreId);
				OnError.ExecuteIfBound(Code, Message);
				bWaitingTerminated = true;
			}));
		WaitUntil(bWaitingTerminated, "Waiting for STORE deletion...");
		bStoreDeleted &= bDeleteTestingStoreSuccess;
	}

	return bStoreDeleted;
}

bool CreateEcommerceStore(const FStoreCreateRequest& StoreRequest, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, FString& StoreId)
{
	// 2021-7-1 Backend change: only 1 draft store is allowed now
	bool bResult = DeleteEcommerceStore(OnSuccess, OnError);
	
	if (bResult) {
		/*
		 * Create a new store, for testing purpose, temporary store.
		 */
		bool bStoreCreated = false;
		bool bWaitingTerminated = false;
		AdminCreateEcommerceStore(StoreRequest,
			THandler<FStoreInfo>::CreateLambda([&StoreId, &bStoreCreated, &bWaitingTerminated](const FStoreInfo& Result) {
				UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: STORE is created"));
				StoreId = Result.storeId;
				bStoreCreated = true;
				bWaitingTerminated = true;
			}),
			FErrorHandler::CreateLambda([&OnError, &bWaitingTerminated](int32 Code, const FString& Message) {
				UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: STORE can not be created"));
				OnError.ExecuteIfBound(Code, Message);
				bWaitingTerminated = true;
			}));
		WaitUntil(bWaitingTerminated, "Waiting for STORE creation...");
		bResult &= bStoreCreated;
	}

	return bResult;
}

bool CreateEcommerceCategory(const FString& StoreId, const FString& CategoryPath, const FString& LanguageCode, const FString& Description, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	TMap<FString, FString> Localization;
	Localization.Add(LanguageCode, Description);
	FCategoryCreateRequest CategoryRequest{ CategoryPath, Localization };
	bool bCreateCategorySuccess = false;
	bool bWaitingTerminated = false;
	AdminCreateEcommerceCategory(StoreId, CategoryRequest,
		THandler<FCategoryInfo>::CreateLambda([&bCreateCategorySuccess, &bWaitingTerminated, &CategoryPath](const FCategoryInfo& Result) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CATEGORY %s is created"), *CategoryPath);
			bCreateCategorySuccess = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&OnError, &bWaitingTerminated, &CategoryPath](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CATEGORY %s can not be created"), *CategoryPath);
			OnError.ExecuteIfBound(Code, Message);
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting for CATEGORY creation...");

	return bCreateCategorySuccess;
}

bool CreateEcommerceItem(const FString& StoreId, const FItemCreateRequest& ItemRequest, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, FItemFullInfo& ItemResult)
{
	bool bItemCreated = false;
	bool bWaitingTerminated = false;
	FString ItemName = ItemRequest.name;
	AdminCreateEcommerceItem(StoreId, ItemRequest,
		THandler<FItemFullInfo>::CreateLambda([&bItemCreated, &bWaitingTerminated, &ItemName, &ItemResult](const FItemFullInfo& Result) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: ITEM %s is created"), *ItemName);
			ItemResult = Result;
			bItemCreated = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&bWaitingTerminated, &OnError, &ItemName](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: ITEM %s can not be created"), *ItemName);
			OnError.ExecuteIfBound(Code, Message);
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting for ITEM creation...");

	return bItemCreated;
}

void SetupEcommerceStore(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString NewStoreId;
	if (!CreateEcommerceStore(Variables.ExpectedStoreTemporary, OnSuccess, OnError, NewStoreId)) {
		return;
	}

#pragma region SetupEcommerceCategory
	/*
	 * Create root CATEGORY
	 */
	if (!CreateEcommerceCategory(NewStoreId, Variables.ExpectedRootCategoryPath, TEXT("en"), TEXT("UE4 Ecommerce Root Category"), OnSuccess, OnError)) {
		return;
	}

	/*
	 * Create child CATEGORY
	 */
	if (!CreateEcommerceCategory(NewStoreId, Variables.ExpectedChildCategoryPath, TEXT("en"), TEXT("UE4 Ecommerce Child Category"), OnSuccess, OnError)) {
		return;
	}

	/*
	 * Create grand child CATEGORY
	 */
	if (!CreateEcommerceCategory(NewStoreId, Variables.ExpectedGrandChildCategoryPath, TEXT("en"), TEXT("UE4 Ecommerce Grand Child Category"), OnSuccess, OnError)) {
		return;
	}

	/*
	 * Create grand child CATEGORY
	 */
	if (!CreateEcommerceCategory(NewStoreId, Variables.ExpectedMediaCategoryPath, TEXT("en"), TEXT("UE4 Ecommerce Media Category"), OnSuccess, OnError)) {
		return;
	}
#pragma endregion

#pragma region SetupEcommerceItem
	TArray<FString> EmptyStrings;
	FItemFullInfo ItemResult;
	/*
	 Create root ITEM
	*/
	FLocalization RootLocalization {
		Variables.ExpectedRootItemTitle,
		"Root item",
		"Root item, virtual currency, not free",
	};
	TMap<FString, FLocalization> RootLocalizationMap;
	RootLocalizationMap.Add("en", RootLocalization);
	FCreateRegionDataItem RootRegionData {
		1,
		0,
		0,
		1,
		Variables.ExpectedCurrency.currencyCode,
		EAccelByteItemCurrencyType::VIRTUAL,
		FRegistry::Settings.Namespace,
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601(),
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601()
	};
	FItemCreateRequest RootItemRequest {
		EAccelByteItemType::INGAMEITEM,
		EAccelByteSeasonType::PASS, // set as default
		Variables.ExpectedRootItemTitle,
		EAccelByteEntitlementType::CONSUMABLE,
		1,
		0,
		"",
		EAccelByteAppType::GAME,//appType
		"",
		Variables.ExpectedCurrency.currencyCode,
		FRegistry::Settings.Namespace,
		Variables.ExpectedRootCategoryPath,
		RootLocalizationMap,
		EAccelByteItemStatus::ACTIVE,
		"UE4rootSku",
		{ FAccelByteModelsItemImage{"image", "itemImage" ,32, 32, "http://example.com", "http://example.com"} },
		"",
		FRegionDataUS{{RootRegionData}},
		EmptyStrings,
		EmptyStrings,
		-1,
		-1,
		"",
		0,
		"",
		FRecurring { }
	};

	if (!CreateEcommerceItem(NewStoreId, RootItemRequest, OnSuccess, OnError, ItemResult)) {
		return;
	}

	/*
	 Create child ITEM
	*/
	FLocalization ChildLocalization {
		Variables.ExpectedChildItemTitle,
		"Child item",
		"Child item, real currency, free, USD"
	};
	TMap<FString, FLocalization> ChildLocalizationMap;
	ChildLocalizationMap.Add("en", ChildLocalization);
	FCreateRegionDataItem ChildRegionData {
		0,
		0,
		0,
		0,
		"USD",
		EAccelByteItemCurrencyType::REAL,
		FRegistry::Settings.Namespace,
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601(),
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601()
	};
	FItemCreateRequest ChildItemRequest {
		EAccelByteItemType::COINS,
		EAccelByteSeasonType::PASS, // set as default
		Variables.ExpectedChildItemTitle,
		EAccelByteEntitlementType::CONSUMABLE,
		20,
		0,
		"",
		EAccelByteAppType::GAME,
		"",
		Variables.ExpectedCurrency.currencyCode,
		FRegistry::Settings.Namespace,
		Variables.ExpectedChildCategoryPath,
		ChildLocalizationMap,
		EAccelByteItemStatus::ACTIVE,
		"UE4childSku",
		{ FAccelByteModelsItemImage{"image", "itemImage" ,32, 32, "http://example.com", "http://example.com"} },
		"",
		FRegionDataUS{{ChildRegionData}},
		EmptyStrings,
		EmptyStrings,
		-1,
		-1,
		"",
		0,
		"classless",
		FRecurring { }
	};

	if (!CreateEcommerceItem(NewStoreId, ChildItemRequest, OnSuccess, OnError, ItemResult)) {
		return;
	}

	/*
	 Create grandchild ITEM
	*/
	FLocalization GrandChildLocalization {
		"UE4_GrandChildItem",
		"Grandchild item",
		"Grandchild item, real currency, free, USD"
	};
	TMap<FString, FLocalization> GrandChildLocalizationMap;
	GrandChildLocalizationMap.Add("en", GrandChildLocalization);
	FCreateRegionDataItem GrandChildRegionData {
		0,
		0,
		0,
		0,
		"USD",
		EAccelByteItemCurrencyType::REAL,
		FRegistry::Settings.Namespace,
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601(),
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601()
	};
	FItemCreateRequest GrandChildItemRequest {
		EAccelByteItemType::COINS,
		EAccelByteSeasonType::PASS, // set as default
		Variables.ExpectedGrandChildItemTitle,
		EAccelByteEntitlementType::CONSUMABLE,
		10,
		0,
		"",
		EAccelByteAppType::GAME,
		"",
		Variables.ExpectedCurrency.currencyCode,
		FRegistry::Settings.Namespace,
		Variables.ExpectedGrandChildCategoryPath,
		GrandChildLocalizationMap,
		EAccelByteItemStatus::ACTIVE,
		"UE4grandchildSku",
		{ FAccelByteModelsItemImage{"image", "itemImage" ,32, 32, "http://example.com", "http://example.com"} },
		"",
		FRegionDataUS{{GrandChildRegionData}},
		EmptyStrings,
		EmptyStrings,
		-1,
		-1,
		"",
		0,
		"classless",
		FRecurring { }
	};

	if (!CreateEcommerceItem(NewStoreId, GrandChildItemRequest, OnSuccess, OnError, ItemResult)) {
		return;
	}
	/*
	Create purchasing ITEM
	*/
	FLocalization PurchaseitemLocalization
	{
		"UE4_PurchasingItem",
		"Purchasing item",
		"Purchasing item, real currency, not free, USD"
	};
	TMap<FString, FLocalization> PurchaseItemLocalizationMap;
	PurchaseItemLocalizationMap.Add("en", PurchaseitemLocalization);
	FCreateRegionDataItem PurchaseItemRegionData
	{
		1, //not free so it will not auto fulfill.
		0,
		0,
		0,
		"USD",
		EAccelByteItemCurrencyType::REAL,
		FRegistry::Settings.Namespace,
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601(),
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601()
	};
	FItemCreateRequest PurchaseItemRequest
	{
		EAccelByteItemType::COINS,
		EAccelByteSeasonType::PASS, // set as default
		Variables.ExpectedPurchasingItemTitle,
		EAccelByteEntitlementType::CONSUMABLE,
		10,
		0,
		"",
		EAccelByteAppType::GAME,
		"",
		Variables.ExpectedCurrency.currencyCode,
		FRegistry::Settings.Namespace,
		Variables.ExpectedRootCategoryPath,
		PurchaseItemLocalizationMap,
		EAccelByteItemStatus::ACTIVE,
		"UE4purchaseitemSku",
		{ FAccelByteModelsItemImage{"image", "itemImage" ,32, 32, "http://example.com", "http://example.com"} },
		"",
		FRegionDataUS{{PurchaseItemRegionData}},
		EmptyStrings,
		EmptyStrings,
		-1,
		-1,
		"",
		0,
		"classless",
		FRecurring { }
	};
	if (!CreateEcommerceItem(NewStoreId, PurchaseItemRequest, OnSuccess, OnError, ItemResult)) {
		return;
	}

#pragma endregion

#pragma region SetupEcommerceRedeemableInGameItem
	FLocalization RedeemableItemLocalization {
		Variables.redeemableItemTitle,
		"Redeemable item",
		"Redeemable item, virtual currency, free"
	};
	TMap<FString, FLocalization> RedeemableItemLocalizations;
	RedeemableItemLocalizations.Add("en", RedeemableItemLocalization);

	FCreateRegionDataItem RedeemableItemRegionData {
		0,
		0,
		0,
		0,
		"SDKC",
		EAccelByteItemCurrencyType::VIRTUAL,
		FRegistry::Settings.Namespace,
		FDateTime::UtcNow().ToIso8601(),
		FDateTime::MaxValue().ToIso8601(),
		FDateTime::UtcNow().ToIso8601(),
		FDateTime::MaxValue().ToIso8601()
	};
	FItemCreateRequest RedeemableItemRequest {
		EAccelByteItemType::INGAMEITEM,
		EAccelByteSeasonType::PASS, // set as default
		Variables.redeemableItemTitle,
		EAccelByteEntitlementType::CONSUMABLE,
		1,
		false,
		"",
		EAccelByteAppType::GAME,
		"",
		Variables.ExpectedCurrency.currencyCode,
		FRegistry::Settings.Namespace,
		Variables.ExpectedRootCategoryPath,
		RedeemableItemLocalizations,
		EAccelByteItemStatus::ACTIVE,
		"UE4reedemableSku",
		{ FAccelByteModelsItemImage{"image", "itemImage" ,32, 32, "http://example.com", "http://example.com"} },
		"",
		FRegionDataUS{{RedeemableItemRegionData}},
		EmptyStrings,
		EmptyStrings,
		-1,
		-1,
		"",
		0,
		"classless"
	};

	if (!CreateEcommerceItem(NewStoreId, RedeemableItemRequest, OnSuccess, OnError, Variables.redeemableItem)) {
		return;
	}
#pragma endregion

#pragma region SetupEcommerceMediaItem
	FLocalization MediaItemLocalization{
		Variables.mediaItemTitle,
		"Media item",
		"Purchasing item, real currency, not free, USD"
	};
	TMap<FString, FLocalization> MediaItemLocalizations;
	MediaItemLocalizations.Add("en", MediaItemLocalization);
	FCreateRegionDataItem MediaItemRegionData
	{
		1, //not free so it will not auto fulfill.
		0,
		0,
		0,
		"USD",
		EAccelByteItemCurrencyType::REAL,
		FRegistry::Settings.Namespace,
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601(),
		FDateTime::MinValue().ToIso8601(),
		FDateTime::MaxValue().ToIso8601()
	};
	FItemCreateRequest MediaItemRequest{
		EAccelByteItemType::MEDIA,
		EAccelByteSeasonType::PASS, // set as default
		Variables.mediaItemTitle,
		EAccelByteEntitlementType::CONSUMABLE,
		1,
		false,
		"",
		EAccelByteAppType::GAME,
		"",
		Variables.ExpectedCurrency.currencyCode,
		FRegistry::Settings.Namespace,
		Variables.ExpectedMediaCategoryPath,
		MediaItemLocalizations,
		EAccelByteItemStatus::ACTIVE,
		"UE4mediaSku",
		{ FAccelByteModelsItemImage{"image", "itemImage" ,32, 32, "http://example.com", "http://example.com"} },
		"",
		FRegionDataUS{{MediaItemRegionData}},
		EmptyStrings,
		EmptyStrings,
		-1,
		-1,
		"",
		0,
		"classless"
	};

	if (!CreateEcommerceItem(NewStoreId, MediaItemRequest, OnSuccess, OnError, Variables.mediaItem)) {
		return;
	}
#pragma endregion

	/*
	 * Publish this store, CreatedTemporaryStoreInfo.
	 */
	bool bPublishTemporaryStoreSuccess = false;
	bool bWaitingTerminated = false;
	AdminCloneEcommerceStore(NewStoreId, "",
		FSimpleDelegate::CreateLambda([&bPublishTemporaryStoreSuccess, &bWaitingTerminated, &NewStoreId]() {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: STORE %s is published"), *NewStoreId);
			bPublishTemporaryStoreSuccess = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&OnError, &bWaitingTerminated, &NewStoreId](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: STORE %s can not be published"), *NewStoreId);
			OnError.ExecuteIfBound(Code, Message);
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting for publishing STORE...");

	if (!bPublishTemporaryStoreSuccess) {
		return;
	}

	OnSuccess.ExecuteIfBound();
}

#pragma endregion

#pragma region SetupEcommerceCampaign

bool GetEcommerceCampaignByName(const FString& Name, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, FCampaignPagingSlicedResult& Campaigns)
{
	bool bGetCampaignSuccess = false;
	bool bWaitingTerminated = false;
	AdminGetEcommerceCampaignByName(Name,
		THandler<FCampaignPagingSlicedResult>::CreateLambda([&bGetCampaignSuccess, &bWaitingTerminated, &Campaigns, &Name](const FCampaignPagingSlicedResult& Result) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CAMPAIGN %s is retrieved"), *Name);
			Campaigns = Result;
			bGetCampaignSuccess = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&OnError, &bWaitingTerminated, &Name](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CAMPAIGN %s can not be retrieved"), *Name);
			OnError.ExecuteIfBound(Code, Message);
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting to retrieve CAMPAIGN Info...");

	return bGetCampaignSuccess;
}

bool CreateEcommerceCampaign(const FCampaignCreateModel& CampaignCreateRequest, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, FCampaignInfo& CampaignInfo)
{
	bool bCreateCampaignSuccess = false;
	bool bWaitingTerminated = false;
	FString CampaignName = CampaignCreateRequest.name;
	AdminCreateEcommerceCampaign(CampaignCreateRequest,
		THandler<FCampaignInfo>::CreateLambda([&bCreateCampaignSuccess, &bWaitingTerminated, &CampaignInfo, &CampaignName](const FCampaignInfo& Result) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CAMPAIGN %s is created"), *CampaignName);
			CampaignInfo = Result;
			bCreateCampaignSuccess = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&OnError, &bWaitingTerminated, &CampaignName](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CAMPAIGN %s can not be created"), *CampaignName);
			OnError.ExecuteIfBound(Code, Message);
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting for CAMPAIGN Created...");

	return bCreateCampaignSuccess;
}

bool UpdateEcommerceCampaign(const FString CampaignId, const FCampaignUpdateModel& CampaignUpdateRequest, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, FCampaignInfo& CampaignInfo)
{
	bool bUpdateCampaignSuccess = false;
	bool bWaitingTerminated = false;
	FString CampaignName = CampaignUpdateRequest.name;
	AdminUpdateEcommerceCampaign(CampaignId, CampaignUpdateRequest,
		THandler<FCampaignInfo>::CreateLambda([&bUpdateCampaignSuccess, &bWaitingTerminated, &CampaignInfo, &CampaignName](const FCampaignInfo& Result) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CAMPAIGN %s is updated"), *CampaignName);
			CampaignInfo = Result;
			bUpdateCampaignSuccess = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&OnError, &bWaitingTerminated, &CampaignName](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CAMPAIGN %s can not be updated"), *CampaignName);
			OnError.ExecuteIfBound(Code, Message);
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting for Campaign Update...");

	return bUpdateCampaignSuccess;
}

bool GetEcommerceCampaignCode(const FString CampaignId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, FCodeInfoPagingSlicedResult& CampaignCodesResult)
{
	bool bGetCampaignCodesSuccess = false;
	bool bWaitingTerminated = false;
	AdminGetEcommerceCampaignCodes(CampaignId,
		THandler<FCodeInfoPagingSlicedResult>::CreateLambda([&bGetCampaignCodesSuccess, &bWaitingTerminated, &CampaignCodesResult, &CampaignId](const FCodeInfoPagingSlicedResult& Result) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CAMPAIGN CODES for CAMPAIGN ID %s is retrieved"), *CampaignId);
			CampaignCodesResult = Result;
			bGetCampaignCodesSuccess = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&OnError, &bWaitingTerminated, &CampaignId](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CAMPAIGN CODES for CAMPAIGN ID %s can not be retrieved"), *CampaignId);
			OnError.ExecuteIfBound(Code, Message);
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting to retrieve CAMPAIGN CODES...");

	return bGetCampaignCodesSuccess;
}

bool CreateEcommerceCampaignCode(const FString CampaignId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, FCampaignCodeCreateResult& CampaignCodeResult)
{
	bool bCreateCampaignCodeSuccess = false;
	bool bWaitingTerminated = false;
	FCampaignCodeCreateModel CampaignCodeCreate = FCampaignCodeCreateModel{ 1 };
	AdminCreateEcommerceCampaignCodes(CampaignId, CampaignCodeCreate,
		THandler<FCampaignCodeCreateResult>::CreateLambda([&bCreateCampaignCodeSuccess, &bWaitingTerminated, &CampaignCodeResult, &CampaignId](const FCampaignCodeCreateResult& Result) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CAMPAIGN CODE for CAMPAIGN ID %s is created"), *CampaignId);
			CampaignCodeResult = Result;
			bCreateCampaignCodeSuccess = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&OnError, &bWaitingTerminated, &CampaignId](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CAMPAIGN CODE for CAMPAIGN ID %s can not be created"), *CampaignId);
			OnError.ExecuteIfBound(Code, Message);
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting for CAMPAIGN CODE creation...");

	return bCreateCampaignCodeSuccess;
}

void SetupEcommerceCampaign(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
#pragma region CreateCampaign
	FCampaignPagingSlicedResult GetCampaignResult;

	/*
	 * Setup standard campaign
	 */
	if (!GetEcommerceCampaignByName(Variables.campaignName, OnSuccess, OnError, GetCampaignResult)) {
		return;
	}

	FRedeemableItem RedeemableItem = FRedeemableItem {
		Variables.redeemableItem.itemId,
		Variables.redeemableItemTitle,
		1
	};

	if (GetCampaignResult.data.Num() == 0) {
		FCampaignCreateModel CampaignCreate {
			Variables.campaignName,
			"UE4 Campaign Test",
			TArray<FString>(),
			"ACTIVE",
			-1,
			1,
			-1,
			-1,
			FDateTime::UtcNow().ToIso8601(),
			FDateTime::MaxValue().ToIso8601(),
			"ITEM",
			TArray<FRedeemableItem>{ RedeemableItem }
		};

		if (!CreateEcommerceCampaign(CampaignCreate, OnSuccess, OnError, Variables.campaignResult)) {
			return;
		}
	} else {
		FCampaignUpdateModel CampaignUpdate {
			Variables.campaignName,
			"UE4 Campaign Test",
			TArray<FString>(),
			"ACTIVE",
			-1,
			1,
			-1,
			-1,
			FDateTime::UtcNow().ToIso8601(),
			FDateTime::MaxValue().ToIso8601(),
			"ITEM",
			TArray<FRedeemableItem>{ RedeemableItem }
		};

		if (!UpdateEcommerceCampaign(GetCampaignResult.data[0].id, CampaignUpdate, OnSuccess, OnError, Variables.campaignResult)) {
			return;
		}
	}

	/*
	 * Setup expired campaign
	 */
	if (!GetEcommerceCampaignByName(Variables.expiredCampaignName, OnSuccess, OnError, GetCampaignResult)) {
		return;
	}

	if (GetCampaignResult.data.Num() == 0) {
		FCampaignCreateModel CampaignCreate {
			Variables.expiredCampaignName,
			"UE4 Expired Campaign Test",
			TArray<FString>(),
			"ACTIVE",
			-1,
			-1,
			-1,
			-1,
			FDateTime::MinValue().ToIso8601(),
			FDateTime::MinValue().ToIso8601(),
			"ITEM",
			TArray<FRedeemableItem>()
		};

		if (!CreateEcommerceCampaign(CampaignCreate, OnSuccess, OnError, Variables.expiredCampaignResult)) {
			return;
		}
	} else {
		Variables.expiredCampaignResult = GetCampaignResult.data[0];
	}

	/*
	 * Setup not started campaign
	 */
	if (!GetEcommerceCampaignByName(Variables.notStartedCampaignName, OnSuccess, OnError, GetCampaignResult)) {
		return;
	}

	if (GetCampaignResult.data.Num() == 0) {
		FCampaignCreateModel CampaignCreate {
			Variables.notStartedCampaignName,
			"UE4 Not Started Campaign Test",
			TArray<FString>(),
			"ACTIVE",
			-1,
			-1,
			-1,
			-1,
			FDateTime::MaxValue().ToIso8601(),
			FDateTime::MaxValue().ToIso8601(),
			"ITEM",
			TArray<FRedeemableItem>()
		};

		if (!CreateEcommerceCampaign(CampaignCreate, OnSuccess, OnError, Variables.notStartedCampaignResult)) {
			return;
		}
	} else {
		Variables.notStartedCampaignResult = GetCampaignResult.data[0];
	}
#pragma endregion

#pragma region CreateCampaignCode
	FCampaignCodeCreateResult CreateCampaignCodeResult;
	if (!CreateEcommerceCampaignCode(Variables.campaignResult.id, OnSuccess, OnError, CreateCampaignCodeResult)) {
		return;
	}

	FCodeInfoPagingSlicedResult GetCampaignCodesResult;

	/*
	 * Process standard campaign code
	 */
	if (!GetEcommerceCampaignCode(Variables.campaignResult.id, OnSuccess, OnError, GetCampaignCodesResult)) {
		return;
	}

	for (auto codeInfo : GetCampaignCodesResult.data)
	{
		bool bCodeFound = false;
		for (auto codeItem : codeInfo.items)
		{
			if (codeItem.itemId == Variables.redeemableItem.itemId)
			{
				Variables.codeInfo = codeInfo;
				bCodeFound = true;
				break;
			}
		}
		if (bCodeFound) break;
	}

	if (Variables.codeInfo.id.IsEmpty()) {
		UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: couldn't find CAMPAIGN CODE for CAMPAIGN ID %s"), *Variables.campaignResult.id);
		int32 ErrorCode = static_cast<int32>(AccelByte::ErrorCodes::UnknownError);
		OnError.ExecuteIfBound(ErrorCode, AccelByte::ErrorMessages::Default.at(ErrorCode));
		return;
	}

	/*
	 * Process expired campaign code
	 */
	if (!GetEcommerceCampaignCode(Variables.expiredCampaignResult.id, OnSuccess, OnError, GetCampaignCodesResult)) {
		return;
	}

	if (GetCampaignCodesResult.data.Num() == 0) {
		if (!CreateEcommerceCampaignCode(Variables.expiredCampaignResult.id, OnSuccess, OnError, CreateCampaignCodeResult)) {
			return;
		}

		if (!GetEcommerceCampaignCode(Variables.expiredCampaignResult.id, OnSuccess, OnError, GetCampaignCodesResult)) {
			return;
		}
	}
		
	Variables.expiredCodeInfo = GetCampaignCodesResult.data[0];

	/*
	 * Process not started campaign code
	 */
	if (!GetEcommerceCampaignCode(Variables.notStartedCampaignResult.id, OnSuccess, OnError, GetCampaignCodesResult)) {
		return;
	}

	if (GetCampaignCodesResult.data.Num() == 0) {
		if (!CreateEcommerceCampaignCode(Variables.notStartedCampaignResult.id, OnSuccess, OnError, CreateCampaignCodeResult)) {
			return;
		}

		if (!GetEcommerceCampaignCode(Variables.notStartedCampaignResult.id, OnSuccess, OnError, GetCampaignCodesResult)) {
			return;
		}
	}

	Variables.notStartedCodeInfo = GetCampaignCodesResult.data[0];
#pragma endregion

	OnSuccess.ExecuteIfBound();
}

#pragma endregion

void TearDownEcommerce(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	// Delete testing currency
	bool bCurrencyDeleted = false;
	bool bWaitingTerminated = false;
	FString CurrencyCode = Variables.ExpectedCurrency.currencyCode;
	AdminDeleteEcommerceCurrency(CurrencyCode,
		FSimpleDelegate::CreateLambda([&bCurrencyDeleted, &bWaitingTerminated, &CurrencyCode]() {
			UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: CURRENCY %s is deleted"), *CurrencyCode);
			bCurrencyDeleted = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&OnError, &bWaitingTerminated, &CurrencyCode](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: CURRENCY %s can not be deleted"), *CurrencyCode);
			OnError.ExecuteIfBound(Code, Message);
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting for CURRENCY deletion...");

	if (!bCurrencyDeleted) {
		return;
	}

	// Delete published testing store
	bool bPublishedStoreDeleted = false;
	bWaitingTerminated = false;
	AdminDeleteEcommercePublishedStore(FSimpleDelegate::CreateLambda([&bPublishedStoreDeleted, &bWaitingTerminated]() {
			UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: Published STORE is deleted"));
			bPublishedStoreDeleted = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&bWaitingTerminated](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: Published STORE %s can not be deleted"));
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting for Published STORE deletion...");

	if (!bPublishedStoreDeleted) {
		return;
	}

	// Fetch all store
	bool bGetAllStoreSuccess = false;
	bWaitingTerminated = false;
	TArray<FStoreInfo> GetAllResult;
	AdminGetEcommerceStoreAll(THandler<TArray<FStoreInfo>>::CreateLambda([&bGetAllStoreSuccess, &bWaitingTerminated, &GetAllResult](const TArray<FStoreInfo>& Result) {
			GetAllResult = Result;
			bGetAllStoreSuccess = true;
			bWaitingTerminated = true;
		}),
		FErrorHandler::CreateLambda([&bWaitingTerminated](int32 Code, const FString& Message) {
			UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: Published STORE %s can not be deleted"));
			bWaitingTerminated = true;
		}));
	WaitUntil(bWaitingTerminated, "Waiting to retrieve all STORE data...");

	for (int index = 0; index < GetAllResult.Num(); index++) {
		bWaitingTerminated = false;
		FString StoreId = GetAllResult[index].storeId;
		// PUBLISH then DELETE the Archive
		if (GetAllResult[index].title == Variables.ExpectedStoreArchive.title) {
			UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: Archive STORE is found"));
			bool bRestorePublishedStore = false;
			AdminCloneEcommerceStore(StoreId, "", 
				FSimpleDelegate::CreateLambda([&bRestorePublishedStore, &bWaitingTerminated, &StoreId]() {
					UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: Archive STORE %s is published / restored again"), *StoreId);
					bRestorePublishedStore = true;
					bWaitingTerminated = true;
				}),
				FErrorHandler::CreateLambda([&bWaitingTerminated, &StoreId](int32 Code, const FString& Message) {
					UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: Archive STORE %s can not be published / restored again"), *StoreId);
					bWaitingTerminated = true;
				}));
			WaitUntil(bWaitingTerminated, "Waiting for Published STORE restored...");

			bool bArchiveStoreDeleteSuccess = false;
			bWaitingTerminated = false;
			AdminDeleteEcommerceStore(StoreId, 
				FSimpleDelegate::CreateLambda([&bArchiveStoreDeleteSuccess, &bWaitingTerminated, &StoreId]() {
					UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: Archive STORE %s is deleted"), *StoreId);
					bArchiveStoreDeleteSuccess = true;
					bWaitingTerminated = true;
				}),
				FErrorHandler::CreateLambda([&bWaitingTerminated, &StoreId](int32 Code, const FString& Message) {
					UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: Archive STORE %s can not be deleted"), *StoreId);
					bWaitingTerminated = true;
				}));
			WaitUntil(bWaitingTerminated, "Waiting for Archive STORE deletion...");
		} else {
			// DELETE all testing store
			if (GetAllResult[index].title == Variables.ExpectedStoreTemporary.title) {
				UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: STORE is found"));
				bool bDeleteTestingStoreSuccess = false;
				bWaitingTerminated = false;
				AdminDeleteEcommerceStore(StoreId, 
					FSimpleDelegate::CreateLambda([&bDeleteTestingStoreSuccess, &bWaitingTerminated, &StoreId]() {
						UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: STORE %s is deleted"), *StoreId);
						bDeleteTestingStoreSuccess = true;
						bWaitingTerminated = true;
					}),
					FErrorHandler::CreateLambda([&bWaitingTerminated, &StoreId](int32 Code, const FString& Message) {
						UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: STORE %s can not be deleted"), *StoreId);
						bWaitingTerminated = true;
					}));
				WaitUntil(bWaitingTerminated, "Waiting for STORE deletion...");
			}
		}
	}

	OnSuccess.ExecuteIfBound();
}

void AdminCreateEcommerceCurrency(const FCurrencyCreateRequest& Currency, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/currencies"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Currency, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetEcommerceCurrency(const FString& CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/currencies/%s/summary"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *CurrencyCode);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminDeleteEcommerceCurrency(const FString& CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/currencies/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *CurrencyCode);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetEcommercePublishedStore(const FString& Namespace, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores/published"), *GetAdminBaseUrl(), *Namespace);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetEcommercePublishedStore(const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Namespace = *FRegistry::Settings.Namespace;
	AdminGetEcommercePublishedStore(Namespace, OnSuccess, OnError);
}

void AdminDeleteEcommercePublishedStore(const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores/published"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCreateEcommerceStore(const FString& Namespace, const FStoreCreateRequest& Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores"), *GetAdminBaseUrl(), *Namespace);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Store, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCreateEcommerceStore(const FStoreCreateRequest& Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Namespace = *FRegistry::Settings.Namespace;
	AdminCreateEcommerceStore(Namespace, Store, OnSuccess, OnError);
}

void AdminGetEcommerceStoreAll(const FString& Namespace, const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores"), *GetAdminBaseUrl(), *Namespace);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetEcommerceStoreAll(const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError)
{
	AdminGetEcommerceStoreAll(FRegistry::Settings.Namespace, OnSuccess, OnError);
}

void AdminDeleteEcommerceStore(const FString& StoreId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *StoreId);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCloneEcommerceStore(const FString& Namespace, const FString& Source, const FString& Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores/%s/clone%s"), *GetAdminBaseUrl(), *Namespace, *Source, *((Target != "") ? "?targetStoreId=" + Target : ""));
	FString Content;
	AB_HTTP_PUT(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCloneEcommerceStore(const FString& Source, const FString& Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Namespace = *FRegistry::Settings.Namespace;
	AdminCloneEcommerceStore(Namespace, Source, Target, OnSuccess, OnError);
}

void AdminCreateEcommerceCategory(const FString& StoreId, const FCategoryCreateRequest& Category, const THandler<FCategoryInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/categories?storeId=%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *StoreId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Category, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCreateEcommerceItem(const FString& StoreId, const FItemCreateRequest& Item, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Namespace = *FRegistry::Settings.Namespace;
	AdminCreateEcommerceItem(Namespace, StoreId, Item, OnSuccess, OnError);
}

void AdminCreateEcommerceItem(const FString& Namespace, const FString& StoreId, const FItemCreateRequest& Item, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/items?storeId=%s"), *GetAdminBaseUrl(), *Namespace, *StoreId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Item, Content);
	// Fix "US" property name because of the JsonObjectConverter's camelCase. 
	Content = Content.Replace(TEXT("\"uS\""), TEXT("\"US\""), ESearchCase::CaseSensitive);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetEcommerceItemBySKU(const FString& Namespace, const FString& StoreId, const FString& SKU, bool ActiveOnly, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/items/bySku?storeId=%s&sku=%s&activeOnly=%s"), *GetAdminBaseUrl(), *Namespace, *StoreId, *SKU, ActiveOnly ? TEXT("true") : TEXT("false"));
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGrantEcommerceUserEntitlements(const FString& Namespace, const FString& UserId, const TArray<FAccelByteModelsEntitlementGrant>& EntitlementGrant, const THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/users/%s/entitlements"), *GetAdminBaseUrl(), *Namespace, *UserId);
	FString Contents = "[";
	for (int i = 0; i < EntitlementGrant.Num(); i++)
	{
		FString Content;
		FJsonObjectConverter::UStructToJsonObjectString(EntitlementGrant[i], Content);
		Contents += Content;
		if (i < EntitlementGrant.Num() - 1)
		{
			Contents += ",";
		}
		else
		{
			Contents += "]";
		}
	}
	AB_HTTP_POST(Request, Url, Authorization, Contents);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCreateEcommerceCampaign(const FCampaignCreateModel& Body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/campaigns"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Body, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminUpdateEcommerceCampaign(const FString& CampaignId, const FCampaignUpdateModel& Body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/campaigns/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *CampaignId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Body, Content);
	AB_HTTP_PUT(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCreateEcommerceCampaignCodes(const FString& CampaignId, const FCampaignCodeCreateModel& Body, const THandler<FCampaignCodeCreateResult>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/codes/campaigns/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *CampaignId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Body, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetEcommerceCampaignByName(const FString& Name, const THandler<FCampaignPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/campaigns"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	FString Query = FString::Printf(TEXT("?name=%s"), *Name);
	Url.Append(Name.IsEmpty() ? TEXT("") : FString::Printf(TEXT("%s"), *Query));
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetEcommerceCampaignCodes(const FString& CampaignId, const THandler<FCodeInfoPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/codes/campaigns/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *CampaignId);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminDisableEcommerceCampaignCode(const FString& CampaignCode, const THandler<FCodeInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/codes/%s/disable"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *CampaignCode);
	FString Content;
	AB_HTTP_PUT(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
