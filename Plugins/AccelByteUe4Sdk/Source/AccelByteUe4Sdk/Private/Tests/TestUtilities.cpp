// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "TestUtilities.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteEnvironment.h"
#include "Api/AccelByteOauth2Api.h"
#include "Misc/Base64.h"

DEFINE_LOG_CATEGORY(LogAccelByteTest);

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Settings;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

TArray<FString> GetDisabledTestList()
{
	TArray<FString> DisabledTest;
	const FString DisabledTestString = Environment::GetEnvironmentVariable(TEXT("AB_UE4_SDK_DISABLED_TESTS"), 1000);
	DisabledTestString.ParseIntoArray(DisabledTest, TEXT(";"), true);
	DisabledTest.RemoveAll([](const FString& Test) -> bool
		{
			return Test.IsEmpty();
		});
	return DisabledTest;
}

bool IsAccelByteTestEnabled(const FString& TestName)
{
	static TArray<FString> DisabledTests = GetDisabledTestList();
	for (const FString& DisabledTest : DisabledTests)
	{
		if (DisabledTest.StartsWith("\"") && DisabledTest.EndsWith("\""))
		{
			return !DisabledTest.Equals(TestName);
		}
		if (DisabledTest.StartsWith("*") && TestName.EndsWith(DisabledTest))
		{
			return false;
		}
		if (TestName.StartsWith(DisabledTest))
		{
			return false;
		}
	}
	return true;
}

bool AccelByteSkipTest(const FString& TestName)
{
	if (!IsAccelByteTestEnabled(TestName))
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("=== Skipping test `%s`"), *TestName);
		return true;
	}

	return false;
}

void Waiting(bool& bCondition, FString Message)
{
	const double StartTime = FPlatformTime::Seconds();
	//Timeout value is already too long to wait for automation tests
	const double TimeoutSeconds = StartTime + 300;
	double LastTickTime = StartTime;

	while (!bCondition && (FPlatformTime::Seconds() < TimeoutSeconds))
	{
		FPlatformProcess::Sleep(.2f);
		UE_LOG(LogAccelByteTest, Log, TEXT("%s"), *Message);
		LastTickTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(.2f);
		FTicker::GetCoreTicker().Tick(.2f);
	}
}

void WaitUntil(TFunction<bool()> Condition, double TimeoutSeconds, const FString Message)
{
	const double StartTime = FPlatformTime::Seconds();
	TimeoutSeconds = StartTime + TimeoutSeconds;
	double LastTickTime = StartTime;

	while (Condition && !Condition() && (FPlatformTime::Seconds() < TimeoutSeconds))
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("%s. Elapsed: %f"), *Message, FPlatformTime::Seconds() - StartTime);
		FTicker::GetCoreTicker().Tick(FPlatformTime::Seconds() - LastTickTime);
		FHttpModule::Get().GetHttpManager().Tick(FPlatformTime::Seconds() - LastTickTime);
		LastTickTime = FPlatformTime::Seconds();
		FPlatformProcess::Sleep(.2f);
	}
}

FString GetRequiredEnvironmentVariable(const FString Name, const int32 ValueLength = 100)
{
	FString Value = Environment::GetEnvironmentVariable(Name, ValueLength + 1);
	if (Value.IsEmpty())
	{
		UE_LOG(LogAccelByteTest, Fatal, TEXT("Missing required environment variable: %s"), *Name);
	}
	if (Value.Len() > ValueLength)
	{
		UE_LOG(LogAccelByteTest, Fatal, TEXT("Environment variable value is too long: %s"), *Name);
	}
	return FString::Printf(TEXT("%s"), *Value);
}

FString GetAdminBaseUrl()
{
	return GetRequiredEnvironmentVariable(TEXT("ADMIN_BASE_URL"));
}

FString GetPublisherNamespace()
{
	return GetRequiredEnvironmentVariable(TEXT("PUBLISHER_NAMESPACE"));
}

FString GetSteamUserId()
{
	return GetRequiredEnvironmentVariable(TEXT("STEAM_USER_ID"));
}

FString AdminAccessTokenCache;

FString GetAdminAccessToken()
{
	if (!AdminAccessTokenCache.IsEmpty())
	{
		return AdminAccessTokenCache;
	}

	FString ClientId = GetRequiredEnvironmentVariable(TEXT("ADMIN_CLIENT_ID"));
	FString ClientSecret = GetRequiredEnvironmentVariable(TEXT("ADMIN_CLIENT_SECRET"));

	FOauth2Token ClientLogin;
	bool ClientLoginSuccess = false;
	Api::Oauth2::GetTokenWithClientCredentials(FString::Printf(TEXT("%s"), *ClientId), FString::Printf(TEXT("%s"), *ClientSecret), THandler<FOauth2Token>::CreateLambda([&ClientLogin, &ClientLoginSuccess](const FOauth2Token& Result)
		{
			ClientLogin = Result;
			ClientLoginSuccess = true;
			UE_LOG(LogAccelByteTest, Log, TEXT("Login with Client Success..!"));
		}), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteTest, Fatal, TEXT("ERROR: %i - %s"), ErrorCode, *ErrorMessage);
			}));
	Waiting(ClientLoginSuccess, "Login with Client...");

	AdminAccessTokenCache = ClientLogin.Access_token;

	return ClientLogin.Access_token;
}

FString AdminUserTokenCache;

FString GetAdminUserAccessToken()
{
	if (!AdminUserTokenCache.IsEmpty())
	{
		return AdminUserTokenCache;
	}

	FString ClientId = GetRequiredEnvironmentVariable(TEXT("ADMIN_CLIENT_ID"));
	FString ClientSecret = GetRequiredEnvironmentVariable(TEXT("ADMIN_CLIENT_SECRET"));
	FString UserName = GetRequiredEnvironmentVariable(TEXT("ADMIN_USER_NAME"));
	FString UserPass = GetRequiredEnvironmentVariable(TEXT("ADMIN_USER_PASS"));

	FString BaseUrl = GetAdminBaseUrl();

	FString Authorization = TEXT("Basic " + FBase64::Encode(FString::Printf(TEXT("%s:%s"), *ClientId, *ClientSecret)));
	FString Url = FString::Printf(TEXT("%s/iam/oauth/token"), *BaseUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("grant_type=password&username=%s&password=%s"), *FGenericPlatformHttp::UrlEncode(UserName), *FGenericPlatformHttp::UrlEncode(UserPass));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	bool bGetTokenSuccess = false;
	FOauth2Token TokenResult;
	THandler<FOauth2Token> OnSuccess;
	OnSuccess.BindLambda([&TokenResult, &bGetTokenSuccess](const FOauth2Token& Result)
		{
			TokenResult = Result;
			bGetTokenSuccess = true;
		});
	FErrorHandler OnError;
	OnError.BindLambda([](int32 ErrorCode, const FString& ErrorMessage)
		{
			UE_LOG(LogAccelByteTest, Fatal, TEXT("Error Code: %d, Message: %s"), ErrorCode, *ErrorMessage);
		});
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	Waiting(bGetTokenSuccess, "Waiting for get token...");

	AdminUserTokenCache = TokenResult.Access_token;

	return TokenResult.Access_token;
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
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
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
	uint8* Output = new uint8[Input.Len()];
	TArray<uint8> Return;
	Return.AddUninitialized(Input.Len());
	StringToBytes(Input, Return.GetData(), Input.Len());
	return Return;
}

bool SetupTestUsers(const FString& InTestUID, const int32 InNumOfUsers, TArray<TSharedPtr<FTestUser>>& OutUsers, TArray<TSharedPtr<Credentials>>& OutCredentials)
{
	const auto Register = [](const FTestUser& InUser)
	{
		bool bIsDone = false;
		bool bIsOk = false;
		UE_LOG(LogAccelByteTest, Log, TEXT("%s: %s %s"), TEXT("SetupTestUsers"), TEXT("Registering"), *InUser.Email);
		FRegistry::User.Register(InUser.Email, InUser.Password, InUser.DisplayName, InUser.Country, InUser.DateOfBirth,
			THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
				{
					bIsOk = true;
					bIsDone = true;
				}),
			FErrorHandler::CreateLambda([&](int32 Code, FString Message)
				{
					if ((ErrorCodes)Code == ErrorCodes::UserEmailAlreadyUsedException)
					{
						bIsOk = true;
					}
					bIsDone = true;
				}));
		Waiting(bIsDone, TEXT("Waiting ..."));
		return bIsOk;
	};

	const auto Login = [](const FTestUser& InUser, Credentials& OutCredentials)
	{
		bool bIsDone = false;
		bool bIsOk = false;
		UE_LOG(LogAccelByteTest, Log, TEXT("%s: %s"), TEXT("Logging in"), *InUser.Email);
		Api::User UserApi(OutCredentials, FRegistry::Settings);
		UserApi.LoginWithUsername(InUser.Email, InUser.Password,
			FVoidHandler::CreateLambda([&]()
				{
					bIsOk = true;
					bIsDone = true;
				}),
			FErrorHandler::CreateLambda([&](int32 ErrorCode, const FString& ErrorMessage)
				{
					bIsDone = true;
				}));
		Waiting(bIsDone, TEXT("Waiting ..."));
		return bIsOk;
	};

	OutUsers.Empty();
	OutCredentials.Empty();

	for (int i = 0; i < InNumOfUsers; i++)
	{
		OutUsers.Add(MakeShared<FTestUser>(InTestUID, i));
		if (!Register(*OutUsers[i]))
		{
			return false;
		}
	}

	for (int i = 0; i < InNumOfUsers; i++)
	{
		OutCredentials.Add(MakeShared<Credentials>());
		if (!Login(*OutUsers[i], *OutCredentials[i]))
		{
			return false;
		}
	}

	return true;
}

bool CheckTestUsers(const TArray<TSharedPtr<FTestUser>>& InUsers, const TArray<TSharedPtr<Credentials>>& InCredentials)
{
	bool bIsOk = true;

	if (InUsers.Num() == 0)
	{
		bIsOk = false;
	}

	if (InUsers.Num() != InCredentials.Num())
	{
		bIsOk = false;
	}

	for (const TSharedPtr <Credentials> Credentials : InCredentials)
	{
		if (Credentials->GetUserId().IsEmpty())
		{
			bIsOk = false;
		}
	}

	if (!bIsOk)
	{
		UE_LOG(LogAccelByteTest, Error, TEXT("%s: %s"), TEXT("CheckTestUsers"), TEXT("Test users are not ready. Maybe SetupTestUsers has not been invoked?"));
	}

	return bIsOk;
}

bool TearDownTestUsers(TArray<TSharedPtr<Credentials>>& InCredentials)
{
	const auto Delete = [](const Credentials& InCredentials)
	{
		bool bIsDone = false;
		bool bIsOk = false;
		UE_LOG(LogAccelByteTest, Log, TEXT("%s: %s %s"), TEXT("TearDownTestUsers"), TEXT("Deleting"), *InCredentials.GetUserId());
		DeleteUserById(InCredentials.GetUserId(),
			FSimpleDelegate::CreateLambda([&]()
				{
					bIsOk = true;
					bIsDone = true;
				}),
			FErrorHandler::CreateLambda([&](int32 ErrorCode, const FString& ErrorMessage)
				{
					bIsDone = true;
				}));
		Waiting(bIsDone, TEXT("Waiting ..."));
		return bIsOk;
	};

	for (int i = 0; i < InCredentials.Num(); i++)
	{
		if (!Delete(*InCredentials[i]))
		{
			return false;
		}
		InCredentials[i]->ForgetAll();
	}

	return true;
}

void DeleteUserById(const FString& UserId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	bool bGetUserMapSuccess = false;
	FUserMapResponse userMap;
	User_Get_User_Mapping(UserId, THandler<FUserMapResponse>::CreateLambda([&userMap, &bGetUserMapSuccess](const FUserMapResponse& Result)
		{
			userMap = Result;
			bGetUserMapSuccess = true;
		}), OnError);

	Waiting(bGetUserMapSuccess, "Wait for getting user map data...");

	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s"), *BaseUrl, *userMap.Namespace, *userMap.userId);
	FString Verb = TEXT("DELETE");
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

void DeleteUserProfile(const FString& Namespace, const FString& UserId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/basic/v1/admin/namespaces/%s/users/%s/profiles"), *BaseUrl, *Namespace, *UserId);
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

void FlushHttpRequests()
{
	double LastTickTime = FPlatformTime::Seconds();

	while (FRegistry::HttpRetryScheduler.PollRetry(FPlatformTime::Seconds()))
	{
		FHttpModule::Get().GetHttpManager().Tick(FPlatformTime::Seconds() - LastTickTime);
		LastTickTime = FPlatformTime::Seconds();
		FPlatformProcess::Sleep(0.5);
	};

	FHttpModule::Get().GetHttpManager().Flush(false);
}

void SetupEcommerce(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	/*
	 Check the currency that expected for integration test. If it's already created, it doesn't need to be created again.
	 If it doesn't exist, then it will be created.
	*/
	bool bCurrencyAlreadyExist = false;
	bool bCurrencyCreated = false;
	bool bCurrencyCheckDone = false;
	Ecommerce_Currency_Get(Variables.ExpectedCurrency.currencyCode, FSimpleDelegate::CreateLambda([&bCurrencyAlreadyExist, &bCurrencyCreated, &bCurrencyCheckDone]()
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
		Ecommerce_Currency_Create(Variables.ExpectedCurrency, FSimpleDelegate::CreateLambda([&bCurrencyCreated]()
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


	/*
	 Create a new store, for testing purpose, temporary store.
	*/
	FStoreInfo CreatedTemporaryStoreInfo;
	bool bTemporaryStoreCreated = false;
	Ecommerce_Store_Create(Variables.ExpectedStoreTemporary, THandler<FStoreInfo>::CreateLambda([&CreatedTemporaryStoreInfo, &bTemporaryStoreCreated](const FStoreInfo& Result)
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
	Ecommerce_Category_Create(CategoryRequest, CreatedTemporaryStoreInfo.storeId, THandler<FCategoryInfo>::CreateLambda([&bCreateRootCategorySuccess](const FCategoryInfo& Result)
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
	Ecommerce_Category_Create(CategoryRequest, CreatedTemporaryStoreInfo.storeId, THandler<FCategoryInfo>::CreateLambda([&bCreateChildCategorySuccess](const FCategoryInfo& Result)
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
	Ecommerce_Category_Create(CategoryRequest, CreatedTemporaryStoreInfo.storeId, THandler<FCategoryInfo>::CreateLambda([&bCreateGrandChildCategorySuccess](const FCategoryInfo& Result)
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
	Ecommerce_Item_Create(RootItemRequest, CreatedTemporaryStoreInfo.storeId, THandler<FItemFullInfo>::CreateLambda([&bRootItemCreated](const FItemFullInfo& Result)
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
	Ecommerce_Item_Create(ChildItemRequest, CreatedTemporaryStoreInfo.storeId, THandler<FItemFullInfo>::CreateLambda([&bChildItemCreated](const FItemFullInfo& Result)
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
	Ecommerce_Item_Create(grandChildItemRequest, CreatedTemporaryStoreInfo.storeId, THandler<FItemFullInfo>::CreateLambda([&bGrandchildItemCreated](const FItemFullInfo& Result)
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
	Ecommerce_Item_Create(redeemableItemRequest, CreatedTemporaryStoreInfo.storeId, THandler<FItemFullInfo>::CreateLambda([&bRedeemableItemCreated, &Variables](const FItemFullInfo& Result)
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: ITEM redeemable is created"));
			Variables.redeemableItem = Result;
			bRedeemableItemCreated = true;
		}), OnError);
	FlushHttpRequests();
	Waiting(bRedeemableItemCreated, "Waiting for child item created...");
	check(bRedeemableItemCreated);
#pragma endregion

	/*
	 Publish this store, CreatedTemporaryStoreInfo.
	*/
	bool bPublishTemporaryStoreSuccess = false;
	Ecommerce_Store_Clone(CreatedTemporaryStoreInfo.storeId, "", FSimpleDelegate::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("SetupEcommerce: TESTING_STORE is published"));
			bPublishTemporaryStoreSuccess = true;
		}), OnError);
	Waiting(bPublishTemporaryStoreSuccess, "Waiting for publish temp store...");
	check(bPublishTemporaryStoreSuccess);


#pragma region CreateCampaign
	bool bGetCampaignSuccess = false;
	FCampaignPagingSlicedResult GetCampaignResult;
	Ecommerce_Campaign_Get_ByName(
		Variables.campaignName,
		THandler<FCampaignPagingSlicedResult>::CreateLambda([&bGetCampaignSuccess, &GetCampaignResult](const FCampaignPagingSlicedResult& Result)
			{
				GetCampaignResult = Result;
				bGetCampaignSuccess = true;
			}), OnError);
	FlushHttpRequests();
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
		Ecommerce_Campaign_Create(
			campaignCreate,
			THandler<FCampaignInfo>::CreateLambda([&bCreateCampaignSuccess, &Variables](const FCampaignInfo& Result)
				{
					Variables.campaignResult = Result;
					bCreateCampaignSuccess = true;
				}), OnError);
		FlushHttpRequests();
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
		Ecommerce_Campaign_Update(
			GetCampaignResult.data[0].id,
			campaignUpdate,
			THandler<FCampaignInfo>::CreateLambda([&bUpdateCampaignSuccess, &Variables](const FCampaignInfo& Result)
				{
					Variables.campaignResult = Result;
					bUpdateCampaignSuccess = true;
				}), OnError);
		FlushHttpRequests();
		Waiting(bUpdateCampaignSuccess, "Waiting for Campaign Update...");

		check(bUpdateCampaignSuccess);
	}

	bGetCampaignSuccess = false;
	Ecommerce_Campaign_Get_ByName(
		Variables.expiredCampaignName,
		THandler<FCampaignPagingSlicedResult>::CreateLambda([&bGetCampaignSuccess, &GetCampaignResult](const FCampaignPagingSlicedResult& Result)
			{
				GetCampaignResult = Result;
				bGetCampaignSuccess = true;
			}), OnError);
	FlushHttpRequests();
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
		Ecommerce_Campaign_Create(
			campaignCreate,
			THandler<FCampaignInfo>::CreateLambda([&bCreateExpiredCampaignSuccess, &Variables](const FCampaignInfo& Result)
				{
					Variables.expiredCampaignResult = Result;
					bCreateExpiredCampaignSuccess = true;
				}), OnError);
		FlushHttpRequests();
		Waiting(bCreateExpiredCampaignSuccess, "Waiting for Campaign Created...");

		check(bCreateExpiredCampaignSuccess);
	}
	else
	{
		Variables.expiredCampaignResult = GetCampaignResult.data[0];
	}

	bool bGetNotStartedCampaignSuccess = false;
	Ecommerce_Campaign_Get_ByName(
		Variables.notStartedCampaignName,
		THandler<FCampaignPagingSlicedResult>::CreateLambda([&bGetNotStartedCampaignSuccess, &GetCampaignResult](const FCampaignPagingSlicedResult& Result)
			{
				GetCampaignResult = Result;
				bGetNotStartedCampaignSuccess = true;
			}), OnError);
	FlushHttpRequests();
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
		Ecommerce_Campaign_Create(
			campaignCreate,
			THandler<FCampaignInfo>::CreateLambda([&bCreateNotStartedCampaignSuccess, &Variables](const FCampaignInfo& Result)
				{
					Variables.notStartedCampaignResult = Result;
					bCreateNotStartedCampaignSuccess = true;
				}), OnError);
		FlushHttpRequests();
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
	Ecommerce_CampaignCodes_Create(
		Variables.campaignResult.id,
		campaignCodesCreate,
		THandler<FCampaignCodeCreateResult>::CreateLambda([&bCreateCampaignCodesSuccess, &createCampaignCodesResult](const FCampaignCodeCreateResult& Result)
			{
				createCampaignCodesResult = Result;
				bCreateCampaignCodesSuccess = true;
			}), OnError);
	FlushHttpRequests();
	Waiting(bCreateCampaignCodesSuccess, "Waiting fore create Campaign Code...");

	check(bCreateCampaignCodesSuccess);

	FCodeInfoPagingSlicedResult getCampaignCodesResult;
	bool bGetCampaignCodeSuccess = false;
	Ecommerce_CampaignCodes_Get(
		Variables.campaignResult.id,
		THandler<FCodeInfoPagingSlicedResult>::CreateLambda([&bGetCampaignCodeSuccess, &getCampaignCodesResult](const FCodeInfoPagingSlicedResult& Result)
			{
				getCampaignCodesResult = Result;
				bGetCampaignCodeSuccess = true;
			}), OnError);
	FlushHttpRequests();
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
	Ecommerce_CampaignCodes_Get(
		Variables.expiredCampaignResult.id,
		THandler<FCodeInfoPagingSlicedResult>::CreateLambda([&bGetExpiredCampaignCodeSuccess, &getCampaignCodesResult](const FCodeInfoPagingSlicedResult& Result)
			{
				getCampaignCodesResult = Result;
				bGetExpiredCampaignCodeSuccess = true;
			}), OnError);
	FlushHttpRequests();
	Waiting(bGetExpiredCampaignCodeSuccess, "Waiting for Get Campaign Code...");

	check(bGetExpiredCampaignCodeSuccess);

	if (getCampaignCodesResult.data.Num() == 0)
	{
		bool bCreateExpiredCampaignCodesSuccess = false;
		Ecommerce_CampaignCodes_Create(
			Variables.expiredCampaignResult.id,
			campaignCodesCreate,
			THandler<FCampaignCodeCreateResult>::CreateLambda([&bCreateExpiredCampaignCodesSuccess, &createCampaignCodesResult](const FCampaignCodeCreateResult& Result)
				{
					createCampaignCodesResult = Result;
					bCreateExpiredCampaignCodesSuccess = true;
				}), OnError);
		FlushHttpRequests();
		Waiting(bCreateExpiredCampaignCodesSuccess, "Waiting fore create Campaign Code...");

		check(bCreateExpiredCampaignCodesSuccess);

		bGetExpiredCampaignCodeSuccess = false;
		Ecommerce_CampaignCodes_Get(
			Variables.expiredCampaignResult.id,
			THandler<FCodeInfoPagingSlicedResult>::CreateLambda([&bGetExpiredCampaignCodeSuccess, &getCampaignCodesResult, &Variables](const FCodeInfoPagingSlicedResult& Result)
				{
					getCampaignCodesResult = Result;
					Variables.expiredCodeInfo = getCampaignCodesResult.data[0];
					bGetExpiredCampaignCodeSuccess = true;
				}), OnError);
		FlushHttpRequests();
		Waiting(bGetExpiredCampaignCodeSuccess, "Waiting for Get Campaign Code...");

		check(bGetExpiredCampaignCodeSuccess);
	}
	else
	{
		Variables.expiredCodeInfo = getCampaignCodesResult.data[0];
	}

	bool bGetNotStartedCampaignCodeSuccess = false;
	Ecommerce_CampaignCodes_Get(
		Variables.notStartedCampaignResult.id,
		THandler<FCodeInfoPagingSlicedResult>::CreateLambda([&bGetNotStartedCampaignCodeSuccess, &getCampaignCodesResult](const FCodeInfoPagingSlicedResult& Result)
			{
				getCampaignCodesResult = Result;
				bGetNotStartedCampaignCodeSuccess = true;
			}), OnError);
	FlushHttpRequests();
	Waiting(bGetNotStartedCampaignCodeSuccess, "Waiting for Get Campaign Code...");

	check(bGetExpiredCampaignCodeSuccess);

	if (getCampaignCodesResult.data.Num() == 0)
	{
		bool bCreateNotStartedCampaignCodesSuccess = false;
		Ecommerce_CampaignCodes_Create(
			Variables.notStartedCampaignResult.id,
			campaignCodesCreate,
			THandler<FCampaignCodeCreateResult>::CreateLambda([&bCreateNotStartedCampaignCodesSuccess, &createCampaignCodesResult](const FCampaignCodeCreateResult& Result)
				{
					createCampaignCodesResult = Result;
					bCreateNotStartedCampaignCodesSuccess = true;
				}), OnError);
		FlushHttpRequests();
		Waiting(bCreateNotStartedCampaignCodesSuccess, "Waiting fore create Campaign Code...");

		check(bCreateNotStartedCampaignCodesSuccess);

		bGetNotStartedCampaignCodeSuccess = false;
		Ecommerce_CampaignCodes_Get(
			Variables.notStartedCampaignResult.id,
			THandler<FCodeInfoPagingSlicedResult>::CreateLambda([&bGetNotStartedCampaignCodeSuccess, &getCampaignCodesResult, &Variables](const FCodeInfoPagingSlicedResult& Result)
				{
					getCampaignCodesResult = Result;
					Variables.notStartedCodeInfo = getCampaignCodesResult.data[0];
					bGetNotStartedCampaignCodeSuccess = true;
				}), OnError);
		FlushHttpRequests();
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
	Ecommerce_Currency_Delete(Variables.ExpectedCurrency.currencyCode, FSimpleDelegate::CreateLambda([&bCurrencyDeleted]()
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: CURRENCY is deleted"));
			bCurrencyDeleted = true;
		}), OnError);
	Waiting(bCurrencyDeleted, "Waiting for currency deletion...");
	check(bCurrencyDeleted);

	// Delete published testing store
	bool bPublishedStoreDeleted = false;
	Ecommerce_PublishedStore_Delete(FSimpleDelegate::CreateLambda([&bPublishedStoreDeleted]()
		{
			UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: TESTING_STORE (PUBLISHED_STORE) is deleted"));
			bPublishedStoreDeleted = true;
		}), nullptr);
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
	Waiting(bGetAllStoreSuccess, "Waiting for get all store...");
	for (int i = 0; i < GetAllResult.Num(); i++)
	{
		// PUBLISH then DELETE the Archive
		if (GetAllResult[i].title == Variables.ExpectedStoreArchive.title)
		{
			bool bRestorePublishedStore = false;
			UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce: ARCHIVE_STORE is found"));
			Ecommerce_Store_Clone(GetAllResult[i].storeId, "", FSimpleDelegate::CreateLambda([&bRestorePublishedStore]()
				{
					UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce:     ARCHIVE_STORE is published / restored again"));
					bRestorePublishedStore = true;
				}), nullptr);
			Waiting(bRestorePublishedStore, "Waiting for published store restored...");
			bool bArchiveStoreDeleteSuccess = false;
			Ecommerce_Store_Delete(GetAllResult[i].storeId, FSimpleDelegate::CreateLambda([&bArchiveStoreDeleteSuccess]()
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
				Ecommerce_Store_Delete(GetAllResult[i].storeId, FSimpleDelegate::CreateLambda([&bDeleteTestingStoreSuccess]()
					{
						UE_LOG(LogAccelByteTest, Log, TEXT("TeardownEcommerce:     TESTING_STORE is deleted"));
						bDeleteTestingStoreSuccess = true;
					}), nullptr);
				Waiting(bDeleteTestingStoreSuccess, "Waiting for testing store deletion...");
			}
	}

	OnSuccess.ExecuteIfBound();
}

void Ecommerce_Currency_Create(FCurrencyCreateRequest Currency, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
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
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
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
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
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

void Ecommerce_PublishedStore_Get(const FString& Namespace, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores/published"), *BaseUrl, *Namespace);
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

void Ecommerce_PublishedStore_Get(const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Namespace = *FRegistry::Settings.Namespace;
	Ecommerce_PublishedStore_Get(Namespace, OnSuccess, OnError);
}

void Ecommerce_PublishedStore_Delete(const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
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

void Ecommerce_Store_Create(const FString& Namespace, FStoreCreateRequest Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores"), *BaseUrl, *Namespace);
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

void Ecommerce_Store_Create(FStoreCreateRequest Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Namespace = *FRegistry::Settings.Namespace;
	Ecommerce_Store_Create(Namespace, Store, OnSuccess, OnError);
}

void Ecommerce_Store_Get_All(const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
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
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
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

void Ecommerce_Store_Clone(const FString& Namespace, FString Source, FString Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/stores/%s/clone%s"), *BaseUrl, *Namespace, *Source, *((Target != "") ? "?targetStoreId=" + Target : ""));
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

void Ecommerce_Store_Clone(FString Source, FString Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Namespace = *FRegistry::Settings.Namespace;
	Ecommerce_Store_Clone(Namespace, Source, Target, OnSuccess, OnError);
}

void Ecommerce_Category_Create(FCategoryCreateRequest Category, FString StoreId, const THandler<FCategoryInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
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
	FString Namespace = *FRegistry::Settings.Namespace;
	Ecommerce_Item_Create(Namespace, Item, StoreId, OnSuccess, OnError);
}

void Ecommerce_Item_Create(const FString& Namespace, FItemCreateRequest Item, FString StoreId, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/items?storeId=%s"), *BaseUrl, *Namespace, *StoreId);
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

void Ecommerce_GetItem_BySKU(const FString& Namespace, const FString& StoreId, const FString& SKU, const bool& ActiveOnly, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/items/bySku?storeId=%s&sku=%s&activeOnly=%s"), *BaseUrl, *Namespace, *StoreId, *SKU, ActiveOnly ? TEXT("true") : TEXT("false"));
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

void Ecommerce_GrantUserEntitlements(const FString& Namespace, const FString& UserId, const TArray<FAccelByteModelsEntitlementGrant>& EntitlementGrant, const THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/users/%s/entitlements"), *BaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
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

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Contents);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Ecommerce_Campaign_Create(FCampaignCreateModel body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/campaigns"), *BaseUrl, *FRegistry::Settings.Namespace);
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

void Ecommerce_Campaign_Update(FString campaignId, FCampaignUpdateModel body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/campaigns/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *campaignId);
	FString Verb = TEXT("PUT");
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

void Ecommerce_CampaignCodes_Create(FString campaignId, FCampaignCodeCreateModel body, const THandler<FCampaignCodeCreateResult>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/codes/campaigns/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *campaignId);
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

void Ecommerce_Campaign_Get_ByName(FString name, const THandler<FCampaignPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/campaigns"), *BaseUrl, *FRegistry::Settings.Namespace);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Query = FString::Printf(TEXT("?name=%s"), *name);
	FString Content;
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

	Url.Append(name.IsEmpty() ? TEXT("") : FString::Printf(TEXT("%s"), *Query));

	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Ecommerce_CampaignCodes_Get(FString campaignId, const THandler<FCodeInfoPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/codes/campaigns/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *campaignId);
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

void Ecommerce_CampaignCode_Disable(FString code, const THandler<FCodeInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/codes/%s/disable"), *BaseUrl, *FRegistry::Settings.Namespace, *code);
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

void Matchmaking_Create_Matchmaking_Channel(const FString& channel, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable)
{
	FAllianceRule AllianceRule;
	AllianceRule.min_number = 2;
	AllianceRule.max_number = 2;
	AllianceRule.player_min_number = 1;
	AllianceRule.player_max_number = 1;

	Matchmaking_Create_Matchmaking_Channel(channel, AllianceRule, OnSuccess, OnError, joinable);
}

void Matchmaking_Create_Matchmaking_Channel(const FString& channel, FAllianceRule AllianceRule, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable)
{
	Matchmaking_Create_Matchmaking_Channel(channel, AllianceRule, TArray<FMatchingRule>(), OnSuccess, OnError, joinable);
}

void Matchmaking_Create_Matchmaking_Channel(const FString& channel, FAllianceRule AllianceRule, TArray<FMatchingRule> MatchingRules, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError, bool joinable)
{
	FMatchmakingCreateRequest RequestBody;
	RequestBody.description = channel;
	RequestBody.find_match_timeout_seconds = 60;
	RequestBody.game_mode = channel;

	RequestBody.rule_set.alliance = AllianceRule;
	RequestBody.rule_set.matching_rule = MatchingRules;
	RequestBody.joinable = joinable;

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(RequestBody, Content);
	UE_LOG(LogAccelByteTest, Log, TEXT("JSON Content: %s"), *Content);
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/matchmaking/namespaces/%s/channels"), *BaseUrl, *FRegistry::Settings.Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Matchmaking_Delete_Matchmaking_Channel(const FString& channel, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/matchmaking/namespaces/%s/channels/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *channel);
	FString Verb = TEXT("DELETE");
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

void Statistic_Get_Stat_By_StatCode(FString statCode, const THandler<FAccelByteModelsStatInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/social/v1/admin/namespaces/%s/stats/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *statCode);
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
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/social/v1/admin/namespaces/%s/stats"), *BaseUrl, *FRegistry::Settings.Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(body, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Statistic_Delete_Stat(const FString& statCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/social/v1/admin/namespaces/%s/stats/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *statCode);
	FString Verb = TEXT("DELETE");
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

void Statistic_Delete_StatItem(const FString& userId, const FString& statCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/social/v1/admin/namespaces/%s/users/%s/stats/%s/statitems"), *BaseUrl, *FRegistry::Settings.Namespace, *userId, *statCode);
	FString Verb = TEXT("DELETE");
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

void Leaderboard_Create_Leaderboard(const FLeaderboardConfigRequest& request, const THandler<FLeaderboardConfigRequest>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/leaderboard/v1/admin/namespaces/%s/leaderboards"), *BaseUrl, *FRegistry::Settings.Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(request, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Leaderboard_GetAll_Leaderboard(const FString& leaderboardCode, const THandler<FLeaderboardConfigResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/leaderboard/v1/admin/namespaces/%s/leaderboards"), *BaseUrl, *FRegistry::Settings.Namespace, *leaderboardCode);
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

void Leaderboard_Delete_Leaderboard(const FString& leaderboardCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/leaderboard/v1/admin/namespaces/%s/leaderboards/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *leaderboardCode);
	FString Verb = TEXT("DELETE");
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

void User_Get_User_Mapping(const FString& userId, const THandler<FUserMapResponse>& OnSuccess, const FErrorHandler& OnError)
{
	check(!userId.IsEmpty());
	UE_LOG(LogAccelByteTest, Log, TEXT("-----------------USER ID: %s---------------------"), *userId);
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/justice/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *userId, *GetPublisherNamespace());
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

void User_Get_Verification_Code(const FString& userId, const THandler<FVerificationCode>& OnSuccess, const FErrorHandler& OnError)
{
	bool bGetUserMapSuccess = false;
	FUserMapResponse userMap;
	User_Get_User_Mapping(userId, THandler<FUserMapResponse>::CreateLambda([&userMap, &bGetUserMapSuccess](const FUserMapResponse& Result)
		{
			userMap = Result;
			bGetUserMapSuccess = true;
		}), OnError);
	Waiting(bGetUserMapSuccess, "Wait for getting user map data...");

	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/v3/admin/namespaces/%s/users/%s/codes"), *BaseUrl, *userMap.Namespace, *userMap.userId);
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

void User_Get_By_Email_Address(const FString& EmailAddress, const THandler<FUserResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/v3/admin/namespaces/%s/users/search?query=%s&limit=2"), *BaseUrl, *FRegistry::Settings.Namespace, *FGenericPlatformHttp::UrlEncode(EmailAddress));
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(THandler<FUserSearchResponse>::CreateLambda([OnSuccess, OnError](FUserSearchResponse Users) {
		if (Users.Data.Num() == 1)
		{
			OnSuccess.ExecuteIfBound(Users.Data[0]);
		}
		else
		{
			OnError.ExecuteIfBound((int32)ErrorCodes::StatusNotFound, TEXT("User not found"));
		}
		}), OnError), FPlatformTime::Seconds());
}

void User_Delete_By_Email_Address(const FString& EmailAddress, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	bool bGetUserSuccess = false;
	bool bGetUserDone = false;
	FUserResponse userData;
	User_Get_By_Email_Address(EmailAddress, THandler<FUserResponse>::CreateLambda([&userData, &bGetUserSuccess, &bGetUserDone](const FUserResponse& Result)
		{
			userData = Result;
			bGetUserDone = true;
			bGetUserSuccess = true;
		}), FErrorHandler::CreateLambda([OnError, &bGetUserDone](int32 ErrorCode, const FString& Message) {
			bGetUserDone = true;
			OnError.Execute(ErrorCode, Message);
			}));
	Waiting(bGetUserDone, "Wait for getting user map data...");

	if (bGetUserSuccess)
	{
		DeleteUserById(userData.UserId, OnSuccess, OnError);
	}
}

void User_Get_MyData_Direct(const FString& JsonWebToken, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *JsonWebToken);
	FString Url = FString::Printf(TEXT("%s/iam/v3/public/users/me"), *FRegistry::Settings.BaseUrl);
	FString Verb = TEXT("GET");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void DSM_Delete_Server(const FString& podName, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/dsmcontroller/admin/namespaces/%s/servers/local/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *podName);
	FString Verb = TEXT("DELETE");
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

void Agreement_Create_Base_Policy(const FAgreementBasePolicyCreate& CreateRequest, const THandler<FAgreementBasePolicy>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/base-policies"), *BaseUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(CreateRequest, Content);

	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement_Create_Policy_Version(const FString& PolicyId, const FAgreementPolicyVersionCreate& CreateRequest, const THandler<FAgreementPolicyVersion>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/policies/%s/versions"), *BaseUrl, *PolicyId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(CreateRequest, Content);

	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement_Create_Localized_Policy(const FString& PolicyVersionId, const FAgreementLocalizedPolicyCreate& CreateRequest, const THandler<FAgreementLocalizedPolicy>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/localized-policy-versions/versions/%s"), *BaseUrl, *PolicyVersionId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(CreateRequest, Content);

	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Agreement_Publish_Policy_Version(const FString& PolicyVersionId, bool ShouldNotify, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/policies/versions/%s/latest?shouldNotify=%s"), *BaseUrl, *PolicyVersionId, ShouldNotify ? TEXT("true") : TEXT("false"));
	FString Verb = TEXT("PATCH");
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

void Agreement_Get_Base_Policies(const THandler<TArray<FAgreementBasePolicy>>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/base-policies"), *BaseUrl);
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

void Agreement_Get_Country_Base_Policy(const FString& BasePolicyId, const FString& CountryCode, const THandler<FAgreementCountryPolicy>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/base-policies/%s/countries/%s"), *BaseUrl, *BasePolicyId, *CountryCode);
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

void Agreement_Get_Policy_Types(const THandler<TArray<FAgreementPolicyTypeObject>>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/policy-types?limit=100"), *BaseUrl);
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

void Agreement_Get_Localized_Policies(const FString& PolicyVersionId, const THandler<TArray<FAgreementLocalizedPolicy>>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/agreement/admin/localized-policy-versions/versions/%s"), *BaseUrl, *PolicyVersionId);
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

void Achievement_Create(const FAchievementRequest& AchievementRequest, const THandler<FAchievementResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/achievement/v1/admin/namespaces/%s/achievements"), *BaseUrl, *FRegistry::Settings.Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(AchievementRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Achievement_Delete(const FString& AchievementCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/achievement/v1/admin/namespaces/%s/achievements/%s"), *BaseUrl, *FRegistry::Settings.Namespace, *AchievementCode);
	FString Verb = TEXT("DELETE");
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

void Subscription_GrantFreeSubs(const FString& UserId, const FFreeSubscriptionRequest& BodyRequest, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/platform/admin/namespaces/%s/users/%s/subscriptions/platformSubscribe"), *BaseUrl, *FRegistry::Settings.PublisherNamespace, *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(BodyRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void DSM_Get_Config(const THandler<FDsmConfig>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/dsmcontroller/admin/namespaces/%s/configs"), *BaseUrl, *FRegistry::Settings.Namespace);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void DSM_Set_Config(const FDsmConfig& Body, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/dsmcontroller/admin/configs"), *BaseUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(Body, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
