// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "EcommerceTestAdmin.h"

void SetupEcommerce(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	/*
	 Check the currency that expected for integration test. If it's already created, it doesn't need to be created again.
	 If it doesn't exist, then it will be created.
	*/
	bool bCurrencyAlreadyExist = false;
	bool bCurrencyCreated = false;
	bool bCurrencyCheckDone = false;
	AdminGetEcommerceCurrency(Variables.ExpectedCurrency.currencyCode, FSimpleDelegate::CreateLambda([&bCurrencyAlreadyExist, &bCurrencyCreated, &bCurrencyCheckDone]()
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CURRENCY is created already."));
			bCurrencyAlreadyExist = true;
			bCurrencyCreated = true;
			bCurrencyCheckDone = true;
		}), FErrorHandler::CreateLambda([&bCurrencyAlreadyExist, &bCurrencyCheckDone](int32 Code, FString Message)
			{
				UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CURRENCY does not exist. Creating..."));
				bCurrencyAlreadyExist = false;
				bCurrencyCheckDone = true;
			}));
	Waiting(bCurrencyCheckDone, "Waiting for currency check...");

	if (!bCurrencyAlreadyExist)
	{
		AdminCreateEcommerceCurrency(Variables.ExpectedCurrency, FSimpleDelegate::CreateLambda([&bCurrencyCreated]()
			{
				UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CURRENCY is created"));
				bCurrencyCreated = true;
			}), FErrorHandler::CreateLambda([&OnError, &bCurrencyCreated](int32 Code, FString Message)
				{
					UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CURRENCY can not be created."));
					bCurrencyCreated = false;
					OnError.ExecuteIfBound(Code, Message);
				}));
		Waiting(bCurrencyCreated, "Waiting for currency created...");
	}

#if 1 
	// 2021-7-1 Backend change: only 1 draft store is allowed now

	// Delete published store (if any)
	bool bPublishedStoreDeleted = false;
	AdminDeleteEcommercePublishedStore(
		FSimpleDelegate::CreateLambda([&]()
			{
				bPublishedStoreDeleted = true;
			}),
		FErrorHandler::CreateLambda([&](int32 Code, FString Message)
			{
				bPublishedStoreDeleted = true;  // Ignore if there is no published store to delete
			}));
	Waiting(bPublishedStoreDeleted, "Waiting for published store deletion...");

	// Delete ALL draft stores
	bool bGetAllStoreSuccess = false;
	TArray<FStoreInfo> GetAllResult;
	AdminGetEcommerceStoreAll(
		THandler<TArray<FStoreInfo>>::CreateLambda([&](const TArray<FStoreInfo>& Result)
			{
				GetAllResult = Result;
				bGetAllStoreSuccess = true;
			}), OnError);
	Waiting(bGetAllStoreSuccess, "Waiting for get all store...");
	for (int i = 0; i < GetAllResult.Num(); i++)
	{
		bool bDeleteTestingStoreSuccess = false;
		AdminDeleteEcommerceStore(GetAllResult[i].storeId,
			FSimpleDelegate::CreateLambda([&]()
				{
					bDeleteTestingStoreSuccess = true;
				}), OnError);
		Waiting(bDeleteTestingStoreSuccess, "Waiting for testing store deletion...");
	}
#else
	/*
	 Get the published store, obtain the language and region to create another draft store as an archive.
	 IF there's published store:
	 * Create a new store
	 * Clone the published to the store
	 IF there's no published store: just go on.
	*/
	bool bTheresPublishedStore = false;
	bool bPublishedStoreCheck = false;
	FStoreInfo PublishedStoreInfo;
	Ecommerce_PublishedStore_Get(THandler<FStoreInfo>::CreateLambda([&Variables, &bTheresPublishedStore, &PublishedStoreInfo, &bPublishedStoreCheck](const FStoreInfo& Result)
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: PUBLISHED_STORE is found"));
			Variables.ExpectedStoreArchive.defaultLanguage = Result.defaultLanguage;
			Variables.ExpectedStoreArchive.defaultRegion = Result.defaultRegion;
			Variables.ExpectedStoreArchive.supportedLanguages = Result.supportedLanguages;
			Variables.ExpectedStoreArchive.supportedRegions = Result.supportedRegions;
			PublishedStoreInfo = Result;
			bTheresPublishedStore = true;
			bPublishedStoreCheck = true;
		}), FErrorHandler::CreateLambda([&OnError, &bPublishedStoreCheck](int32 Code, FString Message)
			{
				UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: PUBLISHED_STORE is not found: %d | %s"), Code, *Message);
				bPublishedStoreCheck = true;
			}));
	Waiting(bPublishedStoreCheck, "Waiting for ");

	if (bTheresPublishedStore)
	{
		bool bArchiveStoreCreated = false;
		FStoreInfo CreatedArchiveStoreInfo;
		Ecommerce_Store_Create(Variables.ExpectedStoreArchive, THandler<FStoreInfo>::CreateLambda([&bArchiveStoreCreated, &CreatedArchiveStoreInfo](const FStoreInfo& Result)
			{
				UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce:     ARCHIVE_STORE for PUBLISHED_STORE is created"));
				CreatedArchiveStoreInfo = Result;
				bArchiveStoreCreated = true;
			}), OnError);
		Waiting(bArchiveStoreCreated, "Waiting for archive store created...");
		bool bArchiveCloned = false;
		Ecommerce_Store_Clone(PublishedStoreInfo.storeId, CreatedArchiveStoreInfo.storeId, FSimpleDelegate::CreateLambda([&bArchiveCloned]()
			{
				UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce:     PUBLISHED_STORE is cloned to ARCHIVE_STORE"));
				bArchiveCloned = true;
			}), OnError);
		Waiting(bArchiveCloned, "Waiting for archive store cloned...");
		check(bArchiveStoreCreated);
		check(bArchiveCloned);

		/*
		 Try to delete the published store, if failed no problem.
		*/
		bool bDeleteStoreDone = false;
		Ecommerce_PublishedStore_Delete(FSimpleDelegate::CreateLambda([&bDeleteStoreDone]()
			{
				bDeleteStoreDone = true;
				UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce:     PUBLISHED_STORE is deleted"));
			}), FErrorHandler::CreateLambda([&bDeleteStoreDone](int32, FString)
				{
					bDeleteStoreDone = true;
				}));

		WaitUntil([&bDeleteStoreDone]() { return bDeleteStoreDone; }, 5.0, "Waiting for archive store cloned...");
	}
#endif


	/*
	 Create a new store, for testing purpose, temporary store.
	*/
	FStoreInfo CreatedTemporaryStoreInfo;
	bool bTemporaryStoreCreated = false;
	AdminCreateEcommerceStore(Variables.ExpectedStoreTemporary, THandler<FStoreInfo>::CreateLambda([&CreatedTemporaryStoreInfo, &bTemporaryStoreCreated](const FStoreInfo& Result)
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: TESTING_STORE is created"));
			CreatedTemporaryStoreInfo = Result;
			bTemporaryStoreCreated = true;
		}), OnError);
	Waiting(bTemporaryStoreCreated, "Waiting for store created...");
	check(bTemporaryStoreCreated);

	/*
	 Create root CATEGORY
	*/
	TMap<FString, FString> Localization;
	Localization.Add("en", "UE4's ecommerce root category");
	FCategoryCreateRequest CategoryRequest{ Variables.ExpectedRootCategoryPath, Localization };
	bool bCreateRootCategorySuccess = false;
	AdminCreateEcommerceCategory(CategoryRequest, CreatedTemporaryStoreInfo.storeId, THandler<FCategoryInfo>::CreateLambda([&bCreateRootCategorySuccess](const FCategoryInfo& Result)
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CATEGORY root is created"));
			bCreateRootCategorySuccess = true;
		}), OnError);
	Waiting(bCreateRootCategorySuccess, "Waiting for root category created...");
	check(bCreateRootCategorySuccess);

	/*
	 Create child CATEGORY
	*/
	Localization.Reset();
	Localization.Add("en", "UE4's ecommerce child category");
	CategoryRequest = FCategoryCreateRequest{ Variables.ExpectedChildCategoryPath, Localization };
	bool bCreateChildCategorySuccess = false;
	AdminCreateEcommerceCategory(CategoryRequest, CreatedTemporaryStoreInfo.storeId, THandler<FCategoryInfo>::CreateLambda([&bCreateChildCategorySuccess](const FCategoryInfo& Result)
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CATEGORY child is created"));
			bCreateChildCategorySuccess = true;
		}), OnError);
	Waiting(bCreateChildCategorySuccess, "Waiting for child category created...");
	check(bCreateChildCategorySuccess);

	/*
	 Create grand child CATEGORY
	*/
	Localization.Reset();
	Localization.Add("en", "UE4's ecommerce grand child category");
	CategoryRequest = FCategoryCreateRequest{ Variables.ExpectedGrandChildCategoryPath, Localization };
	bool bCreateGrandChildCategorySuccess = false;
	AdminCreateEcommerceCategory(CategoryRequest, CreatedTemporaryStoreInfo.storeId, THandler<FCategoryInfo>::CreateLambda([&bCreateGrandChildCategorySuccess](const FCategoryInfo& Result)
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: CATEGORY grandchild is created"));
			bCreateGrandChildCategorySuccess = true;
		}), OnError);
	Waiting(bCreateGrandChildCategorySuccess, "Waiting for grand child category created...");
	check(bCreateGrandChildCategorySuccess);

	TArray<FString> emptyStrings;
	/*
	 Create root ITEM
	*/
	FLocalization rootLocalization
	{
		Variables.ExpectedRootItemTitle,
		"Root item",
		"Root item, virtual currency, not free",
	};
	TMap<FString, FLocalization> rootLocalizationMap;
	rootLocalizationMap.Add("en", rootLocalization);
	FCreateRegionDataItem rootRegionData
	{
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
	FItemCreateRequest RootItemRequest
	{
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
		rootLocalizationMap,
		EAccelByteItemStatus::ACTIVE,
		"sku",
		{ FAccelByteModelsItemImage{"image", "itemImage" ,32, 32, "http://example.com", "http://example.com"} },
		"",
		FRegionDataUS{{rootRegionData}},
		emptyStrings,
		emptyStrings,
		-1,
		-1,
		"",
		0,
		"",
		FRecurring { }
	};
	bool bRootItemCreated = false;
	AdminCreateEcommerceItem(RootItemRequest, CreatedTemporaryStoreInfo.storeId, THandler<FItemFullInfo>::CreateLambda([&bRootItemCreated](const FItemFullInfo& Result)
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: ITEM root is created "));
			bRootItemCreated = true;
		}), OnError);
	Waiting(bRootItemCreated, "Waiting for root item created...");
	check(bRootItemCreated);

	/*
	 Create child ITEM
	*/
	FLocalization childLocalization
	{
		Variables.ExpectedChildItemTitle,
		"Child item",
		"Child item, real currency, free, USD"
	};
	TMap<FString, FLocalization> childLocalizationMap;
	childLocalizationMap.Add("en", childLocalization);
	FCreateRegionDataItem childRegionData
	{
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
	FItemCreateRequest ChildItemRequest
	{
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
		childLocalizationMap,
		EAccelByteItemStatus::ACTIVE,
		"UE4childSku",
		{ FAccelByteModelsItemImage{"image", "itemImage" ,32, 32, "http://example.com", "http://example.com"} },
		"",
		FRegionDataUS{{childRegionData}},
		emptyStrings,
		emptyStrings,
		-1,
		-1,
		"",
		0,
		"classless",
		FRecurring { }
	};
	bool bChildItemCreated = false;
	AdminCreateEcommerceItem(ChildItemRequest, CreatedTemporaryStoreInfo.storeId, THandler<FItemFullInfo>::CreateLambda([&bChildItemCreated](const FItemFullInfo& Result)
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: ITEM child is created"));
			bChildItemCreated = true;
		}), OnError);
	Waiting(bChildItemCreated, "Waiting for child item created...");
	check(bChildItemCreated);

	/*
	 Create grandchild ITEM
	*/
	FLocalization grandchildLocalization
	{
		"UE4_GrandChildItem",
		"Grandchild item",
		"Grandchild item, real currency, free, USD"
	};
	TMap<FString, FLocalization> grandchildLocalizationMap;
	grandchildLocalizationMap.Add("en", grandchildLocalization);
	FCreateRegionDataItem grandchildRegionData
	{
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
	FItemCreateRequest grandChildItemRequest
	{
		EAccelByteItemType::COINS,
		EAccelByteSeasonType::PASS, // set as default
		Variables.ExpectedChildItemTitle,
		EAccelByteEntitlementType::CONSUMABLE,
		10,
		0,
		"",
		EAccelByteAppType::GAME,
		"",
		Variables.ExpectedCurrency.currencyCode,
		FRegistry::Settings.Namespace,
		Variables.ExpectedGrandChildCategoryPath,
		grandchildLocalizationMap,
		EAccelByteItemStatus::ACTIVE,
		"sku",
		{ FAccelByteModelsItemImage{"image", "itemImage" ,32, 32, "http://example.com", "http://example.com"} },
		"",
		FRegionDataUS{{grandchildRegionData}},
		emptyStrings,
		emptyStrings,
		-1,
		-1,
		"",
		0,
		"classless",
		FRecurring { }
	};
	bool bGrandchildItemCreated = false;
	AdminCreateEcommerceItem(grandChildItemRequest, CreatedTemporaryStoreInfo.storeId, THandler<FItemFullInfo>::CreateLambda([&bGrandchildItemCreated](const FItemFullInfo& Result)
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: ITEM grandchild is created"));
			bGrandchildItemCreated = true;
		}), OnError);
	Waiting(bGrandchildItemCreated, "Waiting for grand child item created...");
	check(bGrandchildItemCreated);

#pragma region CreateRedeemableInGameItem

	FLocalization redeemableItemLocalization
	{
		Variables.redeemableItemTitle,
		"Redeemable item",
		"Redeemable item, virtual currency, free"
	};
	TMap<FString, FLocalization> redeemableItemLocalizations;
	redeemableItemLocalizations.Add("en", redeemableItemLocalization);

	FCreateRegionDataItem redeemableItemRegionData
	{
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
	FItemCreateRequest redeemableItemRequest
	{
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
		redeemableItemLocalizations,
		EAccelByteItemStatus::ACTIVE,
		"skuReedemableItem",
		{ FAccelByteModelsItemImage{"image", "itemImage" ,32, 32, "http://example.com", "http://example.com"} },
		"",
		FRegionDataUS{{redeemableItemRegionData}},
		emptyStrings,
		emptyStrings,
		-1,
		-1,
		"",
		0,
		"classless"
	};
	bool bRedeemableItemCreated = false;
	AdminCreateEcommerceItem(redeemableItemRequest, CreatedTemporaryStoreInfo.storeId, THandler<FItemFullInfo>::CreateLambda([&bRedeemableItemCreated, &Variables](const FItemFullInfo& Result)
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: ITEM redeemable is created"));
			Variables.redeemableItem = Result;
			bRedeemableItemCreated = true;
		}), OnError);
	Waiting(bRedeemableItemCreated, "Waiting for child item created...");
	check(bRedeemableItemCreated);
#pragma endregion

	/*
	 Publish this store, CreatedTemporaryStoreInfo.
	*/
	bool bPublishTemporaryStoreSuccess = false;
	AdminCloneEcommerceStore(CreatedTemporaryStoreInfo.storeId, "", FSimpleDelegate::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: TESTING_STORE is published"));
			bPublishTemporaryStoreSuccess = true;
		}), OnError);
	Waiting(bPublishTemporaryStoreSuccess, "Waiting for publish temp store...");
	check(bPublishTemporaryStoreSuccess);


#pragma region CreateCampaign
	bool bGetCampaignSuccess = false;
	FCampaignPagingSlicedResult GetCampaignResult;
	AdminGetEcommerceCampaignByName(
		Variables.campaignName,
		THandler<FCampaignPagingSlicedResult>::CreateLambda([&bGetCampaignSuccess, &GetCampaignResult](const FCampaignPagingSlicedResult& Result)
			{
				GetCampaignResult = Result;
				bGetCampaignSuccess = true;
			}), OnError);
	Waiting(bGetCampaignSuccess, "Waiting for get Campaign Info...");

	check(bGetCampaignSuccess);

	FRedeemableItem redeemableItem = FRedeemableItem
	{
		Variables.redeemableItem.itemId,
		Variables.redeemableItemTitle,
		1
	};

	if (GetCampaignResult.data.Num() == 0)
	{
		FCampaignCreateModel campaignCreate
		{
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
			TArray<FRedeemableItem>{ redeemableItem }
		};

		bool bCreateCampaignSuccess = false;
		AdminCreateEcommerceCampaign(
			campaignCreate,
			THandler<FCampaignInfo>::CreateLambda([&bCreateCampaignSuccess, &Variables](const FCampaignInfo& Result)
				{
					Variables.campaignResult = Result;
					bCreateCampaignSuccess = true;
				}), OnError);
		Waiting(bCreateCampaignSuccess, "Waiting for Campaign Created...");

		check(bCreateCampaignSuccess);
	}
	else
	{
		FCampaignUpdateModel campaignUpdate
		{
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
			TArray<FRedeemableItem>{ redeemableItem }
		};

		bool bUpdateCampaignSuccess = false;
		AdminUpdateEcommerceCampaign(
			GetCampaignResult.data[0].id,
			campaignUpdate,
			THandler<FCampaignInfo>::CreateLambda([&bUpdateCampaignSuccess, &Variables](const FCampaignInfo& Result)
				{
					Variables.campaignResult = Result;
					bUpdateCampaignSuccess = true;
				}), OnError);
		Waiting(bUpdateCampaignSuccess, "Waiting for Campaign Update...");

		check(bUpdateCampaignSuccess);
	}

	bGetCampaignSuccess = false;
	AdminGetEcommerceCampaignByName(
		Variables.expiredCampaignName,
		THandler<FCampaignPagingSlicedResult>::CreateLambda([&bGetCampaignSuccess, &GetCampaignResult](const FCampaignPagingSlicedResult& Result)
			{
				GetCampaignResult = Result;
				bGetCampaignSuccess = true;
			}), OnError);
	Waiting(bGetCampaignSuccess, "Waiting for get Campaign Info...");

	check(bGetCampaignSuccess);

	if (GetCampaignResult.data.Num() == 0)
	{
		FCampaignCreateModel campaignCreate
		{
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

		bool bCreateExpiredCampaignSuccess = false;
		AdminCreateEcommerceCampaign(
			campaignCreate,
			THandler<FCampaignInfo>::CreateLambda([&bCreateExpiredCampaignSuccess, &Variables](const FCampaignInfo& Result)
				{
					Variables.expiredCampaignResult = Result;
					bCreateExpiredCampaignSuccess = true;
				}), OnError);
		Waiting(bCreateExpiredCampaignSuccess, "Waiting for Campaign Created...");

		check(bCreateExpiredCampaignSuccess);
	}
	else
	{
		Variables.expiredCampaignResult = GetCampaignResult.data[0];
	}

	bool bGetNotStartedCampaignSuccess = false;
	AdminGetEcommerceCampaignByName(
		Variables.notStartedCampaignName,
		THandler<FCampaignPagingSlicedResult>::CreateLambda([&bGetNotStartedCampaignSuccess, &GetCampaignResult](const FCampaignPagingSlicedResult& Result)
			{
				GetCampaignResult = Result;
				bGetNotStartedCampaignSuccess = true;
			}), OnError);
	Waiting(bGetNotStartedCampaignSuccess, "Waiting for get Campaign Info...");

	check(bGetNotStartedCampaignSuccess);

	if (GetCampaignResult.data.Num() == 0)
	{
		FCampaignCreateModel campaignCreate
		{
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
		bool bCreateNotStartedCampaignSuccess = false;
		AdminCreateEcommerceCampaign(
			campaignCreate,
			THandler<FCampaignInfo>::CreateLambda([&bCreateNotStartedCampaignSuccess, &Variables](const FCampaignInfo& Result)
				{
					Variables.notStartedCampaignResult = Result;
					bCreateNotStartedCampaignSuccess = true;
				}), OnError);
		Waiting(bCreateNotStartedCampaignSuccess, "Waiting for Campaign Created...");

		check(bCreateNotStartedCampaignSuccess);
	}
	else
	{
		Variables.notStartedCampaignResult = GetCampaignResult.data[0];
	}
#pragma endregion CreateCampaign

#pragma region CreateCampaignCode

	FCampaignCodeCreateModel campaignCodesCreate = FCampaignCodeCreateModel{ 1 };
	FCampaignCodeCreateResult createCampaignCodesResult;
	bool bCreateCampaignCodesSuccess = false;
	AdminCreateEcommerceCampaignCodes(
		Variables.campaignResult.id,
		campaignCodesCreate,
		THandler<FCampaignCodeCreateResult>::CreateLambda([&bCreateCampaignCodesSuccess, &createCampaignCodesResult](const FCampaignCodeCreateResult& Result)
			{
				createCampaignCodesResult = Result;
				bCreateCampaignCodesSuccess = true;
			}), OnError);
	Waiting(bCreateCampaignCodesSuccess, "Waiting fore create Campaign Code...");

	check(bCreateCampaignCodesSuccess);

	FCodeInfoPagingSlicedResult getCampaignCodesResult;
	bool bGetCampaignCodeSuccess = false;
	AdminGetEcommerceCampaignCodes(
		Variables.campaignResult.id,
		THandler<FCodeInfoPagingSlicedResult>::CreateLambda([&bGetCampaignCodeSuccess, &getCampaignCodesResult](const FCodeInfoPagingSlicedResult& Result)
			{
				getCampaignCodesResult = Result;
				bGetCampaignCodeSuccess = true;
			}), OnError);
	Waiting(bGetCampaignCodeSuccess, "Waiting for Get Campaign Code...");

	check(bGetCampaignCodeSuccess);

	for (auto codeInfo : getCampaignCodesResult.data)
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

	check(!Variables.codeInfo.id.IsEmpty());

	bool bGetExpiredCampaignCodeSuccess = false;
	AdminGetEcommerceCampaignCodes(
		Variables.expiredCampaignResult.id,
		THandler<FCodeInfoPagingSlicedResult>::CreateLambda([&bGetExpiredCampaignCodeSuccess, &getCampaignCodesResult](const FCodeInfoPagingSlicedResult& Result)
			{
				getCampaignCodesResult = Result;
				bGetExpiredCampaignCodeSuccess = true;
			}), OnError);
	Waiting(bGetExpiredCampaignCodeSuccess, "Waiting for Get Campaign Code...");

	check(bGetExpiredCampaignCodeSuccess);

	if (getCampaignCodesResult.data.Num() == 0)
	{
		bool bCreateExpiredCampaignCodesSuccess = false;
		AdminCreateEcommerceCampaignCodes(
			Variables.expiredCampaignResult.id,
			campaignCodesCreate,
			THandler<FCampaignCodeCreateResult>::CreateLambda([&bCreateExpiredCampaignCodesSuccess, &createCampaignCodesResult](const FCampaignCodeCreateResult& Result)
				{
					createCampaignCodesResult = Result;
					bCreateExpiredCampaignCodesSuccess = true;
				}), OnError);
		Waiting(bCreateExpiredCampaignCodesSuccess, "Waiting fore create Campaign Code...");

		check(bCreateExpiredCampaignCodesSuccess);

		bGetExpiredCampaignCodeSuccess = false;
		AdminGetEcommerceCampaignCodes(
			Variables.expiredCampaignResult.id,
			THandler<FCodeInfoPagingSlicedResult>::CreateLambda([&bGetExpiredCampaignCodeSuccess, &getCampaignCodesResult, &Variables](const FCodeInfoPagingSlicedResult& Result)
				{
					getCampaignCodesResult = Result;
					Variables.expiredCodeInfo = getCampaignCodesResult.data[0];
					bGetExpiredCampaignCodeSuccess = true;
				}), OnError);
		Waiting(bGetExpiredCampaignCodeSuccess, "Waiting for Get Campaign Code...");

		check(bGetExpiredCampaignCodeSuccess);
	}
	else
	{
		Variables.expiredCodeInfo = getCampaignCodesResult.data[0];
	}

	bool bGetNotStartedCampaignCodeSuccess = false;
	AdminGetEcommerceCampaignCodes(
		Variables.notStartedCampaignResult.id,
		THandler<FCodeInfoPagingSlicedResult>::CreateLambda([&bGetNotStartedCampaignCodeSuccess, &getCampaignCodesResult](const FCodeInfoPagingSlicedResult& Result)
			{
				getCampaignCodesResult = Result;
				bGetNotStartedCampaignCodeSuccess = true;
			}), OnError);
	Waiting(bGetNotStartedCampaignCodeSuccess, "Waiting for Get Campaign Code...");

	check(bGetExpiredCampaignCodeSuccess);

	if (getCampaignCodesResult.data.Num() == 0)
	{
		bool bCreateNotStartedCampaignCodesSuccess = false;
		AdminCreateEcommerceCampaignCodes(
			Variables.notStartedCampaignResult.id,
			campaignCodesCreate,
			THandler<FCampaignCodeCreateResult>::CreateLambda([&bCreateNotStartedCampaignCodesSuccess, &createCampaignCodesResult](const FCampaignCodeCreateResult& Result)
				{
					createCampaignCodesResult = Result;
					bCreateNotStartedCampaignCodesSuccess = true;
				}), OnError);
		Waiting(bCreateNotStartedCampaignCodesSuccess, "Waiting fore create Campaign Code...");

		check(bCreateNotStartedCampaignCodesSuccess);

		bGetNotStartedCampaignCodeSuccess = false;
		AdminGetEcommerceCampaignCodes(
			Variables.notStartedCampaignResult.id,
			THandler<FCodeInfoPagingSlicedResult>::CreateLambda([&bGetNotStartedCampaignCodeSuccess, &getCampaignCodesResult, &Variables](const FCodeInfoPagingSlicedResult& Result)
				{
					getCampaignCodesResult = Result;
					Variables.notStartedCodeInfo = getCampaignCodesResult.data[0];
					bGetNotStartedCampaignCodeSuccess = true;
				}), OnError);
		Waiting(bGetNotStartedCampaignCodeSuccess, "Waiting for Get Campaign Code...");

		check(bGetNotStartedCampaignCodeSuccess);
	}
	else
	{
		Variables.notStartedCodeInfo = getCampaignCodesResult.data[0];
	}
#pragma endregion CreateCampaignCode
	OnSuccess.ExecuteIfBound();
}

void TearDownEcommerce(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	// Delete testing currency
	bool bCurrencyDeleted = false;
	AdminDeleteEcommerceCurrency(Variables.ExpectedCurrency.currencyCode, FSimpleDelegate::CreateLambda([&bCurrencyDeleted]()
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: CURRENCY is deleted"));
			bCurrencyDeleted = true;
		}), OnError);
	Waiting(bCurrencyDeleted, "Waiting for currency deletion...");
	check(bCurrencyDeleted);

	// Delete published testing store
	bool bPublishedStoreDeleted = false;
	AdminDeleteEcommercePublishedStore(FSimpleDelegate::CreateLambda([&bPublishedStoreDeleted]()
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: TESTING_STORE (PUBLISHED_STORE) is deleted"));
			bPublishedStoreDeleted = true;
		}), nullptr);
	Waiting(bPublishedStoreDeleted, "Waiting for published store deletion...");
	check(bPublishedStoreDeleted);

	// Fetch all store
	bool bGetAllStoreSuccess = false;
	TArray<FStoreInfo> GetAllResult;
	AdminGetEcommerceStoreAll(THandler<TArray<FStoreInfo>>::CreateLambda([&bGetAllStoreSuccess, &GetAllResult](const TArray<FStoreInfo>& Result)
		{
			GetAllResult = Result;
			bGetAllStoreSuccess = true;
		}), OnError);
	Waiting(bGetAllStoreSuccess, "Waiting for get all store...");
	for (int i = 0; i < GetAllResult.Num(); i++)
	{
		// PUBLISH then DELETE the Archive
		if (GetAllResult[i].title == Variables.ExpectedStoreArchive.title)
		{
			bool bRestorePublishedStore = false;
			UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: ARCHIVE_STORE is found"));
			AdminCloneEcommerceStore(GetAllResult[i].storeId, "", FSimpleDelegate::CreateLambda([&bRestorePublishedStore]()
				{
					UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce:     ARCHIVE_STORE is published / restored again"));
					bRestorePublishedStore = true;
				}), nullptr);
			Waiting(bRestorePublishedStore, "Waiting for published store restored...");
			bool bArchiveStoreDeleteSuccess = false;
			AdminDeleteEcommerceStore(GetAllResult[i].storeId, FSimpleDelegate::CreateLambda([&bArchiveStoreDeleteSuccess]()
				{
					UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce:     ARCHIVE_STORE is deleted"));
					bArchiveStoreDeleteSuccess = true;
				}), nullptr);
			Waiting(bArchiveStoreDeleteSuccess, "Waiting for archive store deletion...");
		}
		else // DELETE all testing store
			if (GetAllResult[i].title == Variables.ExpectedStoreTemporary.title)
			{
				bool bDeleteTestingStoreSuccess = false;
				UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: TESTING_STORE is found"));
				AdminDeleteEcommerceStore(GetAllResult[i].storeId, FSimpleDelegate::CreateLambda([&bDeleteTestingStoreSuccess]()
					{
						UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce:     TESTING_STORE is deleted"));
						bDeleteTestingStoreSuccess = true;
					}), nullptr);
				Waiting(bDeleteTestingStoreSuccess, "Waiting for testing store deletion...");
			}
	}

	OnSuccess.ExecuteIfBound();
}

void AdminCreateEcommerceCurrency(FCurrencyCreateRequest Currency, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/currencies"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Currency, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetEcommerceCurrency(FString CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/currencies/%s/summary"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *CurrencyCode);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminDeleteEcommerceCurrency(FString CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
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

void AdminCreateEcommerceStore(const FString& Namespace, FStoreCreateRequest Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores"), *GetAdminBaseUrl(), *Namespace);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Store, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCreateEcommerceStore(FStoreCreateRequest Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError)
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

void AdminDeleteEcommerceStore(FString StoreId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *StoreId);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCloneEcommerceStore(const FString& Namespace, FString Source, FString Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores/%s/clone%s"), *GetAdminBaseUrl(), *Namespace, *Source, *((Target != "") ? "?targetStoreId=" + Target : ""));
	FString Content;
	AB_HTTP_PUT(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCloneEcommerceStore(FString Source, FString Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Namespace = *FRegistry::Settings.Namespace;
	AdminCloneEcommerceStore(Namespace, Source, Target, OnSuccess, OnError);
}

void AdminCreateEcommerceCategory(FCategoryCreateRequest Category, FString StoreId, const THandler<FCategoryInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/categories?storeId=%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *StoreId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Category, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCreateEcommerceItem(FItemCreateRequest Item, FString StoreId, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Namespace = *FRegistry::Settings.Namespace;
	AdminCreateEcommerceItem(Namespace, Item, StoreId, OnSuccess, OnError);
}

void AdminCreateEcommerceItem(const FString& Namespace, FItemCreateRequest Item, FString StoreId, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError)
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

void AdminGetEcommerceItemBySKU(const FString& Namespace, const FString& StoreId, const FString& SKU, const bool& ActiveOnly, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError)
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

void AdminCreateEcommerceCampaign(FCampaignCreateModel body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/campaigns"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(body, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminUpdateEcommerceCampaign(FString campaignId, FCampaignUpdateModel body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/campaigns/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *campaignId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(body, Content);
	AB_HTTP_PUT(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminCreateEcommerceCampaignCodes(FString campaignId, FCampaignCodeCreateModel body, const THandler<FCampaignCodeCreateResult>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/codes/campaigns/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *campaignId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(body, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetEcommerceCampaignByName(FString name, const THandler<FCampaignPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/campaigns"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	FString Query = FString::Printf(TEXT("?name=%s"), *name);
	Url.Append(name.IsEmpty() ? TEXT("") : FString::Printf(TEXT("%s"), *Query));
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetEcommerceCampaignCodes(FString campaignId, const THandler<FCodeInfoPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/codes/campaigns/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *campaignId);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminDisableEcommerceCampaignCode(FString code, const THandler<FCodeInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/codes/%s/disable"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *code);
	FString Content;
	AB_HTTP_PUT(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
