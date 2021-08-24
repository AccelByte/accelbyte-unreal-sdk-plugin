// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Models/AccelByteStatisticModels.h"
#include "Models/AccelByteAchievementModels.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TestUtilities.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteTest, Log, All);

#define AB_TEST_TRUE(Expr) UTEST_TRUE(TEXT(#Expr), Expr)
#define AB_TEST_FALSE(Expr) UTEST_FALSE(TEXT(#Expr), Expr)
#define AB_TEST_VALID(Ptr) UTEST_VALID(TEXT(#Ptr), Ptr)
#define AB_TEST_INVALID(Ptr) UTEST_INVALID(TEXT(#Ptr), Ptr)
#define AB_TEST_NOT_NULL(Ptr) UTEST_NOT_NULL(TEXT(#Ptr), Ptr)
#define AB_TEST_NULL(Ptr) UTEST_NULL(TEXT(#Ptr), Ptr)
#define AB_TEST_EQUAL(Actual, Expected) UTEST_EQUAL(TEXT(#Actual), Actual, Expected)
#define AB_TEST_NOT_EQUAL(Actual, Expected) UTEST_NOT_EQUAL(TEXT(#Actual " and " #Expected), Actual, Expected)
#define AB_TEST_SAME(Actual, Expected) UTEST_SAME(TEXT(#Actual " and " #Expected)), Actual, Expected)
#define AB_TEST_NOT_SAME(Actual, Expected) UTEST_NOT_SAME(TEXT(#Actual " and " #Expected)), Actual, Expected)

#define AB_HTTP_REQUEST(Name, URL, Verb, Authorization, ContentType, Content) \
	FHttpRequestPtr Name = FHttpModule::Get().CreateRequest(); \
	Name->SetURL(URL); \
	Name->SetHeader(TEXT("Authorization"), Authorization); \
	Name->SetVerb(Verb); \
	Name->SetHeader(TEXT("Content-Type"), ContentType); \
	Name->SetHeader(TEXT("Accept"), TEXT("application/json")); \
	Name->SetContentAsString(Content);
#define AB_HTTP_GET(Name, URL, Authorization) \
	AB_HTTP_REQUEST(Name, URL, TEXT("GET"), Authorization,  TEXT("application/json"), TEXT(""))
#define AB_HTTP_DELETE(Name, URL, Authorization) \
	AB_HTTP_REQUEST(Name, URL, TEXT("DELETE"), Authorization, TEXT("application/json"), TEXT(""))
#define AB_HTTP_POST(Name, URL, Authorization, Content) \
	AB_HTTP_REQUEST(Name, URL, TEXT("POST"), Authorization, TEXT("application/json"), Content)
#define AB_HTTP_POST_FORM(Name, URL, Authorization, Content) \
	AB_HTTP_REQUEST(Name, URL, TEXT("POST"), Authorization, TEXT("application/x-www-form-urlencoded"), Content)
#define AB_HTTP_PUT(Name, URL, Authorization, Content) \
	AB_HTTP_REQUEST(Name, URL, TEXT("PUT"), Authorization, TEXT("application/json"), Content)
#define AB_HTTP_PATCH(Name, URL, Authorization, Content) \
	AB_HTTP_REQUEST(Name, URL, TEXT("PATCH"), Authorization, TEXT("application/json"), Content)


using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::FVoidHandler;

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsTest : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE(FSendNotificationSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Test ")
		static void SendNotification(FString Message, bool bAsync, const FSendNotificationSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	static void SendNotif(FString UserId, FString Message, bool bAsync, const AccelByte::FVoidHandler & OnSuccess, const FErrorHandler & OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Test ")
		static FString BytesToFString(TArray<uint8> Input);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Test ")
		static TArray<uint8> FStringToBytes(FString Input);
};

void FlushHttpRequests();

void WaitUntil(const bool& bCondition, const FString Message = "", const double TimeoutSeconds = 60.0);

void WaitUntil(const TFunction<bool()> Condition, const FString Message = "", const double TimeoutSeconds = 60.0);

void DelaySeconds(double Seconds, FString Message = "");

FString GetPublisherNamespace();

FString GetSteamUserId();

FString GetSteamAppId();

FString GetSteamTicket();

bool CheckSteamTicket();

FString GetAdminBaseUrl();
FString GetAdminUserAccessToken();
