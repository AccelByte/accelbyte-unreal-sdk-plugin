// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteEntitlementApi.h"
#include "AccelByteError.h"
#include "AccelByteRegistry.h"
#include "AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "EngineMinimal.h"

namespace AccelByte
{
namespace Api
{

void Entitlement::QueryUserEntitlement(const FString & EntitlementName, const FString & ItemId, int32 Page, int32 Size, const THandler<FAccelByteModelsEntitlementPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError, EAccelByteEntitlementClass EntitlementClass = EAccelByteEntitlementClass::NONE, EAccelByteAppType AppType = EAccelByteAppType::NONE )
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements"), *FRegistry::Settings.PlatformServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId());
	
	FString Query = TEXT("");
	if (!EntitlementName.IsEmpty())
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("entitlementName=%s"), *EntitlementName));
	}
	if (!ItemId.IsEmpty())
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("itemId=%s"), *ItemId));
	}
	if (Page>=0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("page=%d"), Page));
	}
	if (Size>=0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("size=%d"), Size));
	}
	if (EntitlementClass != EAccelByteEntitlementClass::NONE)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("entitlementClazz=%s"), *FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccelByteEntitlementClass"), true)->GetNameStringByValue((int32)EntitlementClass)));
	}
	if (AppType != EAccelByteAppType::NONE)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("appType=%s"), *FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccelByteAppType"), true)->GetNameStringByValue((int32)AppType)));
	}

	Url.Append(Query.IsEmpty() ? TEXT("") : FString::Printf(TEXT("?%s"),*Query));
	
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
} // Namespace Api
}
