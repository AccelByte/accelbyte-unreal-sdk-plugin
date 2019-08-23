// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "TestUtilities.h"
#include "Misc/AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Json.h"
#include "HAL/FileManager.h"

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Settings;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

void Waiting(bool& condition, FString text)
{
    while (!condition)
    {
        FPlatformProcess::Sleep(.5f);
        UE_LOG(LogTemp, Log, TEXT("%s"), *text);
        FTicker::GetCoreTicker().Tick(.5f);
    }
}

FString GetBaseUrl()
{
    const int32 length = 100;
    TCHAR BaseUrl[length];
#if PLATFORM_WINDOWS
    FWindowsPlatformMisc::GetEnvironmentVariable(TEXT("ADMIN_BASE_URL"), BaseUrl, length);
#elif PLATFORM_LINUX
    FLinuxPlatformMisc::GetEnvironmentVariable(TEXT("ADMIN_BASE_URL"), BaseUrl, length);
#elif PLATFORM_MAC
    FApplePlatformMisc::GetEnvironmentVariable(TEXT("ADMIN_BASE_URL"), BaseUrl, length);
#endif
    return FString::Printf(TEXT("%s"), BaseUrl);
}

void UAccelByteBlueprintsTest::SendNotification(FString Message, bool bAsync, const UAccelByteBlueprintsTest::FSendNotificationSuccess& OnSuccess, const UAccelByteBlueprintsTest::FBlueprintErrorHandler& OnError)
{
    UAccelByteBlueprintsTest::SendNotif(FRegistry::Credentials.GetUserId(), Message, bAsync, FVoidHandler::CreateLambda([OnSuccess]()
    {
        OnSuccess.ExecuteIfBound();
    }), FErrorHandler::CreateLambda([OnError](int32 Code, const FString& Message)
    {
        OnError.ExecuteIfBound(Code, Message);
    }));
}

void UAccelByteBlueprintsTest::SendNotif(FString UserId, FString Message, bool bAsync, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
    /*
     If the client is not logged in
    */
    if (FRegistry::Credentials.GetClientAccessToken() == "")
    {
        bool bClientTokenObtained = false;
        FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
        {
            UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: CLIENT is logged in"));
            bClientTokenObtained = true;
        }), OnError);
        FlushHttpRequests();
        Waiting(bClientTokenObtained, "Waiting for Login...");
        check(bClientTokenObtained);
    }


    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/lobby/notification/namespaces/%s/users/%s/freeform"), *BaseUrl, *FRegistry::Settings.Namespace, *UserId);
    FString Verb = TEXT("POST");
    FString ContentType = TEXT("application/json");
    FString Accept = TEXT("application/json");
    FString Content = FString::Printf(TEXT("{\"message\":\"%s\"}"), *Message);
    Url = Url.Replace(TEXT("wss"), TEXT("https")); //change protocol
    Url = Url.Replace(TEXT("lobby/"), TEXT("")); //no /lobby
    if (bAsync)
    {
        Url.Append(TEXT("?async=true"));
    }
    else
    {
        Url.Append(TEXT("?async=false"));
    }

    FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetHeader(TEXT("Authorization"), Authorization);
    Request->SetVerb(Verb);
    Request->SetHeader(TEXT("Content-Type"), ContentType);
    Request->SetHeader(TEXT("Accept"), Accept);
    Request->SetContentAsString(Content);

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FString UAccelByteBlueprintsTest::BytesToFString(TArray<uint8> Input)
{
    uint32 size = Input.Num();
    FString Data = TEXT("");
    Data = BytesToString(Input.GetData(), Input.Num());
    return Data;
}

TArray<uint8> UAccelByteBlueprintsTest::FStringToBytes(FString Input)
{
    uint8 * Output = new uint8[Input.Len()];
    TArray<uint8> Return;
    Return.AddUninitialized(Input.Len());
    StringToBytes(Input, Return.GetData(), Input.Len());
    return Return;
}

void DeleteUserById(const FString& UserId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
    using AccelByte::Settings;
    FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([OnSuccess, OnError, UserId]()
    {
        UE_LOG(LogTemp, Log, TEXT("-----------------USER ID: %s---------------------"), *UserId);
        FString BaseUrl = GetBaseUrl();
        FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
        FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/justice/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *UserId, *FRegistry::Settings.PublisherNamespace);
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
        Request->OnProcessRequestComplete().BindLambda([BaseUrl, OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
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

                    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
                    FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s"), *BaseUrl, *FRegistry::Settings.PublisherNamespace, *RealUserId);
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

void FlushHttpRequests()
{
    double LastTickTime = FPlatformTime::Seconds();

    while (FRegistry::HttpRetryScheduler.PollRetry(FPlatformTime::Seconds(), FRegistry::Credentials))
    {
        FHttpModule::Get().GetHttpManager().Tick(FPlatformTime::Seconds() - LastTickTime);
        LastTickTime = FPlatformTime::Seconds();
        FPlatformProcess::Sleep(0.5);
    };

    FHttpModule::Get().GetHttpManager().Flush(false);
}

void SetupEcommerce(EcommerceExpectedVariable Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
    /*
     If the client is not logged in
    */
    if (FRegistry::Credentials.GetClientAccessToken() == "")
    {
        bool bClientTokenObtained = false;
        FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
        {
            UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: CLIENT is logged in"));
            bClientTokenObtained = true;
        }), OnError);
        FlushHttpRequests();
        Waiting(bClientTokenObtained, "Waiting for Login...");
        check(bClientTokenObtained);
    }

    /*
     Check the currency that expected for integration test. If it's already created, it doesn't need to be created again.
     If it doesn't exist, then it will be created.
    */
    bool bCurrencyAlreadyExist = false;
    bool bCurrencyCreated = false;
    bool bCurrencyCheckDone = false;
    Ecommerce_Currency_Get(Variables.ExpectedCurrency.currencyCode, FSimpleDelegate::CreateLambda([&bCurrencyAlreadyExist, &bCurrencyCreated, &bCurrencyCheckDone]()
    {
        UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: CURRENCY is created already."));
        bCurrencyAlreadyExist = true;
        bCurrencyCreated = true;
        bCurrencyCheckDone = true;
    }), FErrorHandler::CreateLambda([&bCurrencyAlreadyExist, &bCurrencyCheckDone](int32 Code, FString Message)
    {
        UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: CURRENCY does not exist. Creating..."));
        bCurrencyAlreadyExist = false;
        bCurrencyCheckDone = true;
    }));
    FlushHttpRequests();
    Waiting(bCurrencyCheckDone, "Waiting for currency check...");

    if (!bCurrencyAlreadyExist)
    {
        Ecommerce_Currency_Create(Variables.ExpectedCurrency, FSimpleDelegate::CreateLambda([&bCurrencyCreated]()
        {
            UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: CURRENCY is created"));
            bCurrencyCreated = true;
        }), FErrorHandler::CreateLambda([&OnError, &bCurrencyCreated](int32 Code, FString Message)
        {
            UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: CURRENCY can not be created."));
            bCurrencyCreated = false;
            OnError.ExecuteIfBound(Code, Message);
        }));
        FlushHttpRequests();
        Waiting(bCurrencyCreated, "Waiting for currency created...");
    }

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
        UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: PUBLISHED_STORE is found"));
        Variables.ExpectedStoreArchive.defaultLanguage = Result.defaultLanguage;
        Variables.ExpectedStoreArchive.defaultRegion = Result.defaultRegion;
        Variables.ExpectedStoreArchive.supportedLanguages = Result.supportedLanguages;
        Variables.ExpectedStoreArchive.supportedRegions = Result.supportedRegions;
        PublishedStoreInfo = Result;
        bTheresPublishedStore = true;
        bPublishedStoreCheck = true;
    }), FErrorHandler::CreateLambda([&OnError, &bPublishedStoreCheck](int32 Code, FString Message)
    {
        UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: PUBLISHED_STORE is not found: %d | %s"), Code, *Message);
        bPublishedStoreCheck = true;
    }));
    FlushHttpRequests();
    Waiting(bPublishedStoreCheck, "Waiting for ");

    if (bTheresPublishedStore)
    {
        bool bArchiveStoreCreated = false;
        FStoreInfo CreatedArchiveStoreInfo;
        Ecommerce_Store_Create(Variables.ExpectedStoreArchive, THandler<FStoreInfo>::CreateLambda([&bArchiveStoreCreated, &CreatedArchiveStoreInfo](const FStoreInfo& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("SetupEcommerce:     ARCHIVE_STORE for PUBLISHED_STORE is created"));
            CreatedArchiveStoreInfo = Result;
            bArchiveStoreCreated = true;
        }), OnError);
        FlushHttpRequests();
        Waiting(bArchiveStoreCreated, "Waiting for archive store created...");
        bool bArchiveCloned = false;
        Ecommerce_Store_Clone(PublishedStoreInfo.storeId, CreatedArchiveStoreInfo.storeId, FSimpleDelegate::CreateLambda([&bArchiveCloned]()
        {
            UE_LOG(LogTemp, Log, TEXT("SetupEcommerce:     PUBLISHED_STORE is cloned to ARCHIVE_STORE"));
            bArchiveCloned = true;
        }), OnError);
        FlushHttpRequests();
        Waiting(bArchiveCloned, "Waiting for archive store cloned...");
        check(bArchiveStoreCreated);
        check(bArchiveCloned);

        /*
         Try to delete the published store, if failed no problem.
        */
        Ecommerce_PublishedStore_Delete(FSimpleDelegate::CreateLambda([]()
        {
            UE_LOG(LogTemp, Log, TEXT("SetupEcommerce:     PUBLISHED_STORE is deleted"));
        }), nullptr);
        FlushHttpRequests();
    }


    /*
     Create a new store, for testing purpose, temporary store.
    */
    FStoreInfo CreatedTemporaryStoreInfo;
    bool bTemporaryStoreCreated = false;
    Ecommerce_Store_Create(Variables.ExpectedStoreTemporary, THandler<FStoreInfo>::CreateLambda([&CreatedTemporaryStoreInfo, &bTemporaryStoreCreated](const FStoreInfo& Result)
    {
        UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: TESTING_STORE is created"));
        CreatedTemporaryStoreInfo = Result;
        bTemporaryStoreCreated = true;
    }), OnError);
    FlushHttpRequests();
    Waiting(bTemporaryStoreCreated, "Waiting for store created...");
    check(bTemporaryStoreCreated);

    /*
     Create root CATEGORY
    */
    TMap<FString, FString> Localization;
    Localization.Add("en", "UE4's ecommerce root category");
    FCategoryCreateRequest CategoryRequest{ Variables.ExpectedRootCategoryPath, Localization };
    bool bCreateRootCategorySuccess = false;
    Ecommerce_Category_Create(CategoryRequest, CreatedTemporaryStoreInfo.storeId, THandler<FCategoryInfo>::CreateLambda([&bCreateRootCategorySuccess](const FCategoryInfo& Result)
    {
        UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: CATEGORY root is created"));
        bCreateRootCategorySuccess = true;
    }), OnError);
    FlushHttpRequests();
    Waiting(bCreateRootCategorySuccess, "Waiting for root category created...");
    check(bCreateRootCategorySuccess);

    /*
     Create child CATEGORY
    */
    Localization.Reset();
    Localization.Add("en", "UE4's ecommerce child category");
    CategoryRequest = FCategoryCreateRequest{ Variables.ExpectedChildCategoryPath, Localization };
    bool bCreateChildCategorySuccess = false;
    Ecommerce_Category_Create(CategoryRequest, CreatedTemporaryStoreInfo.storeId, THandler<FCategoryInfo>::CreateLambda([&bCreateChildCategorySuccess](const FCategoryInfo& Result)
    {
        UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: CATEGORY child is created"));
        bCreateChildCategorySuccess = true;
    }), OnError);
    FlushHttpRequests();
    Waiting(bCreateChildCategorySuccess, "Waiting for child category created...");
    check(bCreateChildCategorySuccess);

    /*
     Create grand child CATEGORY
    */
    Localization.Reset();
    Localization.Add("en", "UE4's ecommerce grand child category");
    CategoryRequest = FCategoryCreateRequest{ Variables.ExpectedGrandChildCategoryPath, Localization };
    bool bCreateGrandChildCategorySuccess = false;
    Ecommerce_Category_Create(CategoryRequest, CreatedTemporaryStoreInfo.storeId, THandler<FCategoryInfo>::CreateLambda([&bCreateGrandChildCategorySuccess](const FCategoryInfo& Result)
    {
        UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: CATEGORY grandchild is created"));
        bCreateGrandChildCategorySuccess = true;
    }), OnError);
    FlushHttpRequests();
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
        { FAccelByteModelsItemInfoImage{32, 32, "http://example.com", "http://example.com", "itemImage"} },
        FAccelByteModelsItemInfoImage{32, 32, "http://example.com", "http://example.com", "thumbnailImage"}
    };
    TMap<FString, FLocalization> rootLocalizationMap;
    rootLocalizationMap.Add("en", rootLocalization);
    FAccelByteModelsItemInfoRegionData rootRegionData
    {
        1,
        0,
        0,
        1,
        Variables.ExpectedCurrency.currencyCode,
        "VIRTUAL",
        FRegistry::Settings.Namespace,
        FDateTime::MinValue().ToIso8601(),
        FDateTime::MaxValue().ToIso8601(),
        FDateTime::MinValue().ToIso8601(),
        FDateTime::MaxValue().ToIso8601()
    };
    FItemCreateRequest RootItemRequest
    {
        EItemType::INGAMEITEM,
        Variables.ExpectedRootItemTitle,
        EEntitlementType::DURABLE,
        0,
        "",
        "",//appType
        Variables.ExpectedCurrency.currencyCode,
        FRegistry::Settings.Namespace,
        Variables.ExpectedRootCategoryPath,
        rootLocalizationMap,
        EStatus::ACTIVE,
        "sku",
        FRegionDataUS{{rootRegionData}},
        emptyStrings,
        emptyStrings,
        -1,
        -1,
        "",
        ""
    };
    bool bRootItemCreated = false;
    Ecommerce_Item_Create(RootItemRequest, CreatedTemporaryStoreInfo.storeId, THandler<FItemFullInfo>::CreateLambda([&bRootItemCreated](const FItemFullInfo& Result)
    {
        UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: ITEM root is created "));
        bRootItemCreated = true;
    }), OnError);
    FlushHttpRequests();
    Waiting(bRootItemCreated, "Waiting for root item created...");
    check(bRootItemCreated);

    /*
     Create child ITEM
    */
    FLocalization childLocalization
    {
        Variables.ExpectedChildItemTitle,
        "Child item",
        "Child item, real currency, free, USD",
        { FAccelByteModelsItemInfoImage{32, 32, "http://example.com", "http://example.com", "itemImage"} },
        FAccelByteModelsItemInfoImage{32, 32, "http://example.com", "http://example.com", "thumbnailImage"}
    };
    TMap<FString, FLocalization> childLocalizationMap;
    childLocalizationMap.Add("en", childLocalization);
    FAccelByteModelsItemInfoRegionData childRegionData
    {
        0,
        0,
        0,
        0,
        "USD",
        "REAL",
        FRegistry::Settings.Namespace,
        FDateTime::MinValue().ToIso8601(),
        FDateTime::MaxValue().ToIso8601(),
        FDateTime::MinValue().ToIso8601(),
        FDateTime::MaxValue().ToIso8601()
    };
    FItemCreateRequest ChildItemRequest
    {
        EItemType::COINS,
        Variables.ExpectedChildItemTitle,
        EEntitlementType::CONSUMABLE,
        20,
        "",
        "",
        Variables.ExpectedCurrency.currencyCode,
        "",
        Variables.ExpectedChildCategoryPath,
        childLocalizationMap,
        EStatus::ACTIVE,
        "UE4childSku",
        FRegionDataUS{{childRegionData}},
        emptyStrings,
        emptyStrings,
        -1,
        -1,
        "classless",
        ""
    };
    bool bChildItemCreated = false;
    Ecommerce_Item_Create(ChildItemRequest, CreatedTemporaryStoreInfo.storeId, THandler<FItemFullInfo>::CreateLambda([&bChildItemCreated](const FItemFullInfo& Result)
    {
        UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: ITEM child is created"));
        bChildItemCreated = true;
    }), OnError);
    FlushHttpRequests();
    Waiting(bChildItemCreated, "Waiting for child item created...");
    check(bChildItemCreated);

    /*
     Create grandchild ITEM
    */
    FLocalization grandchildLocalization
    {
        "UE4_GrandChildItem",
        "Grandchild item",
        "Grandchild item, real currency, free, USD",
        { FAccelByteModelsItemInfoImage{32, 32, "http://example.com", "http://example.com", "itemImage"} },
        FAccelByteModelsItemInfoImage{32, 32, "http://example.com", "http://example.com", "thumbnailImage"}
    };
    TMap<FString, FLocalization> grandchildLocalizationMap;
    grandchildLocalizationMap.Add("en", grandchildLocalization);
    FAccelByteModelsItemInfoRegionData grandchildRegionData
    {
        0,
        0,
        0,
        0,
        "USD",
        "REAL",
        FRegistry::Settings.Namespace,
        FDateTime::MinValue().ToIso8601(),
        FDateTime::MaxValue().ToIso8601(),
        FDateTime::MinValue().ToIso8601(),
        FDateTime::MaxValue().ToIso8601()
    };
    FItemCreateRequest grandChildItemRequest
    {
        EItemType::COINS,
        Variables.ExpectedChildItemTitle,
        EEntitlementType::CONSUMABLE,
        10,
        "",
        "",
        Variables.ExpectedCurrency.currencyCode,
        FRegistry::Settings.Namespace,
        Variables.ExpectedGrandChildCategoryPath,
        grandchildLocalizationMap,
        EStatus::ACTIVE,
        "sku",
        FRegionDataUS{{grandchildRegionData}},
        emptyStrings,
        emptyStrings,
        -1,
        -1,
        "classless",
        ""
    };
    bool bGrandchildItemCreated = false;
    Ecommerce_Item_Create(grandChildItemRequest, CreatedTemporaryStoreInfo.storeId, THandler<FItemFullInfo>::CreateLambda([&bGrandchildItemCreated](const FItemFullInfo& Result)
    {
        UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: ITEM grandchild is created"));
        bGrandchildItemCreated = true;
    }), OnError);
    FlushHttpRequests();
    Waiting(bGrandchildItemCreated, "Waiting for grand child item created...");
    check(bGrandchildItemCreated);

    /*
     Publish this store, CreatedTemporaryStoreInfo.
    */
    bool bPublishTemporaryStoreSuccess = false;
    Ecommerce_Store_Clone(CreatedTemporaryStoreInfo.storeId, "", FSimpleDelegate::CreateLambda([&]()
    {
        UE_LOG(LogTemp, Log, TEXT("SetupEcommerce: TESTING_STORE is published"));
        bPublishTemporaryStoreSuccess = true;
    }), OnError);
    FlushHttpRequests();
    Waiting(bPublishTemporaryStoreSuccess, "Waiting for publish temp store...");
    check(bPublishTemporaryStoreSuccess);

    OnSuccess.ExecuteIfBound();
}

void TearDownEcommerce(EcommerceExpectedVariable Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
    // If the client is not logged in
    if (FRegistry::Credentials.GetClientAccessToken() == "")
    {
        bool bClientTokenObtained = false;
        FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&]()
        {
            UE_LOG(LogTemp, Log, TEXT("TeardownEcommerce: CLIENT is logged in"));
            bClientTokenObtained = true;
        }), OnError);
        FlushHttpRequests();
        Waiting(bClientTokenObtained, "Waiting for Login...");
        check(bClientTokenObtained);
    }

    // Delete testing currency
    bool bCurrencyDeleted = false;
    Ecommerce_Currency_Delete(Variables.ExpectedCurrency.currencyCode, FSimpleDelegate::CreateLambda([&bCurrencyDeleted]()
    {
        UE_LOG(LogTemp, Log, TEXT("TeardownEcommerce: CURRENCY is deleted"));
        bCurrencyDeleted = true;
    }), OnError);
    FlushHttpRequests();
    Waiting(bCurrencyDeleted, "Waiting for currency deletion...");
    check(bCurrencyDeleted);

    // Delete published testing store
    bool bPublishedStoreDeleted = false;
    Ecommerce_PublishedStore_Delete(FSimpleDelegate::CreateLambda([&bPublishedStoreDeleted]()
    {
        UE_LOG(LogTemp, Log, TEXT("TeardownEcommerce: TESTING_STORE (PUBLISHED_STORE) is deleted"));
        bPublishedStoreDeleted = true;
    }), nullptr);
    FlushHttpRequests();
    Waiting(bPublishedStoreDeleted, "Waiting for published store deletion...");
    check(bPublishedStoreDeleted);

    // Fetch all store
    bool bGetAllStoreSuccess = false;
    TArray<FStoreInfo> GetAllResult;
    Ecommerce_Store_Get_All(THandler<TArray<FStoreInfo>>::CreateLambda([&bGetAllStoreSuccess, &GetAllResult](const TArray<FStoreInfo>& Result)
    {
        GetAllResult = Result;
        bGetAllStoreSuccess = true;
    }), OnError);
    FlushHttpRequests();
    Waiting(bGetAllStoreSuccess, "Waiting for get all store...");
    for (int i = 0; i < GetAllResult.Num(); i++)
    {
        // PUBLISH then DELETE the Archive
        if (GetAllResult[i].title == Variables.ExpectedStoreArchive.title)
        {
            bool bRestorePublishedStore = false;
            UE_LOG(LogTemp, Log, TEXT("TeardownEcommerce: ARCHIVE_STORE is found"));
            Ecommerce_Store_Clone(GetAllResult[i].storeId, "", FSimpleDelegate::CreateLambda([&bRestorePublishedStore]()
            {
                UE_LOG(LogTemp, Log, TEXT("TeardownEcommerce:     ARCHIVE_STORE is published / restored again"));
                bRestorePublishedStore = true;
            }), nullptr);
            FlushHttpRequests();
            Waiting(bRestorePublishedStore, "Waiting for published store restored...");
            bool bArchiveStoreDeleteSuccess = false;
            Ecommerce_Store_Delete(GetAllResult[i].storeId, FSimpleDelegate::CreateLambda([&bArchiveStoreDeleteSuccess]()
            {
                UE_LOG(LogTemp, Log, TEXT("TeardownEcommerce:     ARCHIVE_STORE is deleted"));
                bArchiveStoreDeleteSuccess = true;
            }), nullptr);
            FlushHttpRequests();
            Waiting(bArchiveStoreDeleteSuccess, "Waiting for archive store deletion...");
        }
        else // DELETE all testing store
            if (GetAllResult[i].title == Variables.ExpectedStoreTemporary.title)
            {
                bool bDeleteTestingStoreSuccess = false;
                UE_LOG(LogTemp, Log, TEXT("TeardownEcommerce: TESTING_STORE is found"));
                Ecommerce_Store_Delete(GetAllResult[i].storeId, FSimpleDelegate::CreateLambda([&bDeleteTestingStoreSuccess]()
                {
                    UE_LOG(LogTemp, Log, TEXT("TeardownEcommerce:     TESTING_STORE is deleted"));
                    bDeleteTestingStoreSuccess = true;
                }), nullptr);
                FlushHttpRequests();
                Waiting(bDeleteTestingStoreSuccess, "Waiting for testing store deletion...");
            }
    }

    OnSuccess.ExecuteIfBound();
}

void Ecommerce_Currency_Create(FCurrencyCreateRequest Currency, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/currencies"), *BaseUrl, *FRegistry::Settings.Namespace);
    FString Verb = TEXT("POST");
    FString ContentType = TEXT("application/json");
    FString Accept = TEXT("application/json");
    FString Content;
    FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
    FJsonObjectConverter::UStructToJsonObjectString(Currency, Content);

    Request->SetURL(Url);
    Request->SetHeader(TEXT("Authorization"), Authorization);
    Request->SetVerb(Verb);
    Request->SetHeader(TEXT("Content-Type"), ContentType);
    Request->SetHeader(TEXT("Accept"), Accept);
    Request->SetContentAsString(Content);

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Ecommerce_Currency_Get(FString CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/currencies/%s/summary"), *BaseUrl, *FRegistry::Settings.Namespace, *CurrencyCode);
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

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Ecommerce_Currency_Delete(FString CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/currencies/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *CurrencyCode);
    FString Verb = TEXT("DELETE");
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

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Ecommerce_PublishedStore_Get(const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores/published"), *BaseUrl, *FRegistry::Settings.Namespace);
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

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Ecommerce_PublishedStore_Delete(const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores/published"), *BaseUrl, *FRegistry::Settings.Namespace);
    FString Verb = TEXT("DELETE");
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

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Ecommerce_Store_Create(FStoreCreateRequest Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores"), *BaseUrl, *FRegistry::Settings.Namespace);
    FString Verb = TEXT("POST");
    FString ContentType = TEXT("application/json");
    FString Accept = TEXT("application/json");
    FString Content;
    FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
    FJsonObjectConverter::UStructToJsonObjectString(Store, Content);

    Request->SetURL(Url);
    Request->SetHeader(TEXT("Authorization"), Authorization);
    Request->SetVerb(Verb);
    Request->SetHeader(TEXT("Content-Type"), ContentType);
    Request->SetHeader(TEXT("Accept"), Accept);
    Request->SetContentAsString(Content);

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Ecommerce_Store_Get_All(const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores"), *BaseUrl, *FRegistry::Settings.Namespace);
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

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Ecommerce_Store_Delete(FString StoreId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *StoreId);
    FString Verb = TEXT("DELETE");
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

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Ecommerce_Store_Clone(FString Source, FString Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores/%s/clone%s"), *BaseUrl, *FRegistry::Settings.Namespace, *Source, *((Target != "") ? "?targetStoreId=" + Target : ""));
    FString Verb = TEXT("PUT");
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

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Ecommerce_Category_Create(FCategoryCreateRequest Category, FString StoreId, const THandler<FCategoryInfo>& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/categories?storeId=%s"), *BaseUrl, *FRegistry::Settings.Namespace, *StoreId);
    FString Verb = TEXT("POST");
    FString ContentType = TEXT("application/json");
    FString Accept = TEXT("application/json");
    FString Content;
    FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
    FJsonObjectConverter::UStructToJsonObjectString(Category, Content);
    Request->SetURL(Url);
    Request->SetHeader(TEXT("Authorization"), Authorization);
    Request->SetVerb(Verb);
    Request->SetHeader(TEXT("Content-Type"), ContentType);
    Request->SetHeader(TEXT("Accept"), Accept);
    Request->SetContentAsString(Content);

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Ecommerce_Item_Create(FItemCreateRequest Item, FString StoreId, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/items?storeId=%s"), *BaseUrl, *FRegistry::Settings.Namespace, *StoreId);
    FString Verb = TEXT("POST");
    FString ContentType = TEXT("application/json");
    FString Accept = TEXT("application/json");
    FString Content;
    FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
    FJsonObjectConverter::UStructToJsonObjectString(Item, Content);
    Content = Content.Replace(TEXT("uS"), TEXT("US"), ESearchCase::CaseSensitive);
    Request->SetURL(Url);
    Request->SetHeader(TEXT("Authorization"), Authorization);
    Request->SetVerb(Verb);
    Request->SetHeader(TEXT("Content-Type"), ContentType);
    Request->SetHeader(TEXT("Accept"), Accept);
    Request->SetContentAsString(Content);

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Statistic_Get_Stat_By_StatCode(FString statCode, const THandler<FAccelByteModelsStatInfo>& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/statistic/admin/namespaces/%s/stats/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *statCode);
    FString Verb = TEXT("GET");
    FString ContentType = TEXT("application/json");
    FString Accept = TEXT("application/json");
    FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetHeader(TEXT("Authorization"), Authorization);
    Request->SetVerb(Verb);
    Request->SetHeader(TEXT("Content-Type"), ContentType);
    Request->SetHeader(TEXT("Accept"), Accept);

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Statistic_Create_Stat(FStatCreateRequest body, const THandler<FAccelByteModelsStatInfo>& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/statistic/admin/namespaces/%s/stats"), *BaseUrl, *FRegistry::Settings.Namespace);
    FString Verb = TEXT("POST");
    FString ContentType = TEXT("application/json");
    FString Accept = TEXT("application/json");
    FString Content;
    FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
    FJsonObjectConverter::UStructToJsonObjectString(body, Content);
    Request->SetURL(Url);
    Request->SetHeader(TEXT("Authorization"), Authorization);
    Request->SetVerb(Verb);
    Request->SetHeader(TEXT("Content-Type"), ContentType);
    Request->SetHeader(TEXT("Accept"), Accept);
    Request->SetContentAsString(Content);

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Statistic_Bulk_Create_StatItem(FString userId, FString profileId, TArray<FString> statCode, const THandler<TArray<FAccelByteModelsBulkStatItemIncResult>>& OnSuccess, const FErrorHandler& OnError)
{
    FString BaseUrl = GetBaseUrl();
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
    FString Url = FString::Printf(TEXT("%s/statistic/admin/namespaces/%s/users/%s/profiles/%s/statitems/bulk/create"), *BaseUrl, *FRegistry::Settings.Namespace, *userId, *profileId);
    FString Verb = TEXT("POST");
    FString ContentType = TEXT("application/json");
    FString Accept = TEXT("application/json");
    FString Content = "[";
    for (int i = 0; i < statCode.Num(); i++)
    {
        Content += FString::Printf(TEXT("{\"statCode\":\"%s\"}"), *statCode[i]);
        if (i < statCode.Num() - 1)
        {
            Content += ",";
        }
        else
        {
            Content += "]";
        }
    }
    FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetHeader(TEXT("Authorization"), Authorization);
    Request->SetVerb(Verb);
    Request->SetHeader(TEXT("Content-Type"), ContentType);
    Request->SetHeader(TEXT("Accept"), Accept);
    Request->SetContentAsString(Content);

    FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}