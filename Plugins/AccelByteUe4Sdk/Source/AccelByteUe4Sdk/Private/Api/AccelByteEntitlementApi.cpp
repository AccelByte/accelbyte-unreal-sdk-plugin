// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteEntitlementApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "EngineMinimal.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
Entitlement::Entitlement(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Setting) : Credentials(Credentials), Settings(Setting){}

Entitlement::~Entitlement(){}

void Entitlement::QueryUserEntitlements(const FString& EntitlementName, const FString& ItemId, const int32& Offset, const int32& Limit, const THandler<FAccelByteModelsEntitlementPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError, EAccelByteEntitlementClass EntitlementClass = EAccelByteEntitlementClass::NONE, EAccelByteAppType AppType = EAccelByteAppType::NONE )
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements"), *Settings.PlatformServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId());
	
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
	if (Offset>=0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("offset=%d"), Offset));
	}
	if (Limit>=0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("limit=%d"), Limit));
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
	
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
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

void Entitlement::QueryUserEntitlements(const FString& EntitlementName, const TArray<FString>& ItemIds,
	const int32& Offset, const int32& Limit, const THandler<FAccelByteModelsEntitlementPagingSlicedResult>& OnSuccess,
	const FErrorHandler& OnError, EAccelByteEntitlementClass EntitlementClass, EAccelByteAppType AppType)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements"), *Settings.PlatformServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId());
	
	FString Query = TEXT("");
	if (!EntitlementName.IsEmpty())
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("entitlementName=%s"), *EntitlementName));
	}
	for (const FString& ItemId : ItemIds)
	{
		if (!ItemId.IsEmpty())
		{
			Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
			Query.Append(FString::Printf(TEXT("itemId=%s"), *ItemId));
		}
	}
	if (Offset>=0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("offset=%d"), Offset));
	}
	if (Limit>=0)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("limit=%d"), Limit));
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
	
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
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

void Entitlement::GetUserEntitlementById(const FString& Entitlementid, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/%s"), *Settings.PlatformServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *Entitlementid);

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

void Entitlement::GetUserEntitlementOwnershipByAppId(const FString& AppId, const THandler<FAccelByteModelsEntitlementOwnership>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/entitlements/ownership/byAppId?appId=%s"), *Settings.PlatformServerUrl, *Settings.PublisherNamespace, *AppId);

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

void Entitlement::GetUserEntitlementOwnershipBySku(const FString& Sku, const THandler<FAccelByteModelsEntitlementOwnership>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/entitlements/ownership/bySku?sku=%s"), *Settings.PlatformServerUrl, *Settings.PublisherNamespace, *Sku);

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

void Entitlement::GetUserEntitlementOwnershipAny(const TArray<FString> ItemIds, const TArray<FString> AppIds, const TArray<FString> Skus,
	const THandler<FAccelByteModelsEntitlementOwnership> OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	if (ItemIds.Num() < 1
		&& AppIds.Num() < 1
		&& Skus.Num() < 1)
	{
		OnError.ExecuteIfBound(EHttpResponseCodes::NotFound, TEXT("Please provide at least one itemId, AppId or Sku."));
	}
	else
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
		FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/entitlements/ownership/any"), *Settings.PlatformServerUrl, *Settings.PublisherNamespace);

		int paramCount = 0;
		for (int i = 0; i < ItemIds.Num(); i++)
		{
			Url.Append((paramCount == 0) ? TEXT("?") : TEXT("&")).Append(TEXT("itemIds=")).Append(ItemIds[i]);
			paramCount++;
		}
		for (int i = 0; i < AppIds.Num(); i++)
		{
			Url.Append((paramCount == 0) ? TEXT("?") : TEXT("&")).Append(TEXT("appIds=")).Append(AppIds[i]);
			paramCount++;
		}
		for (int i = 0; i < Skus.Num(); i++)
		{
			Url.Append((paramCount == 0) ? TEXT("?") : TEXT("&")).Append(TEXT("skus=")).Append(Skus[i]);
			paramCount++;
		}

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
}

void Entitlement::ConsumeUserEntitlement(const FString& EntitlementId, const int32& UseCount, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FAccelByteModelsConsumeUserEntitlementRequest ConsumeUserEntitlementRequest;
	ConsumeUserEntitlementRequest.UseCount = UseCount;

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/%s/decrement"), *Settings.PlatformServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *EntitlementId);

	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(ConsumeUserEntitlementRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Entitlement::CreateDistributionReceiver(const FString& ExtUserId, const FAccelByteModelsAttributes Attributes, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/receivers/%s"), *Settings.PlatformServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *ExtUserId);

	FAccelByteModelsDistributionAttributes DistributionAttributes;
	DistributionAttributes.Attributes = Attributes;

	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(DistributionAttributes, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Entitlement::DeleteDistributionReceiver(const FString& ExtUserId, const FString& UserId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/receivers/%s"), *Settings.PlatformServerUrl, *Credentials.GetUserNamespace(), *UserId, *ExtUserId);

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

void Entitlement::GetDistributionReceiver(const FString& PublisherNamespace, const FString& PublisherUserId, const THandler<TArray<FAccelByteModelsDistributionReceiver>>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/receivers"), *Settings.PlatformServerUrl, *PublisherNamespace, *PublisherUserId);
	
	FString Query = TEXT("");
	if (!Credentials.GetUserNamespace().IsEmpty())
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("targetNamespace=%s"), *Credentials.GetUserNamespace()));
	}
	Url.Append(Query.IsEmpty() ? TEXT("") : FString::Printf(TEXT("?%s"),*Query));

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

void Entitlement::UpdateDistributionReceiver(const FString& ExtUserId, const FAccelByteModelsAttributes Attributes, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/receivers/%s"), *Settings.PlatformServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *ExtUserId);

	FAccelByteModelsDistributionAttributes DistributionAttributes;
	DistributionAttributes.Attributes = Attributes;

	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(DistributionAttributes, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Entitlement::SyncPlatformPurchase(EAccelBytePlatformSync PlatformType, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString PlatformText = TEXT("");
	FString Content = TEXT("{}");
	FString platformUserId = Credentials.GetPlatformBoundUserId();

	switch (PlatformType)
	{
	case EAccelBytePlatformSync::STEAM:
		PlatformText = TEXT("steam");
		if (platformUserId.IsEmpty()) {
			OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("User not logged in with 3rd Party Platform"));
			return;
		}
		Content = FString::Printf(TEXT("{\"steamId\": \"%s\", \"appId\": %s}"), *Credentials.GetPlatformBoundUserId(), *Settings.AppId);
		break;
	case EAccelBytePlatformSync::XBOX_LIVE:
		PlatformText = TEXT("xbl");
		break;
	case EAccelBytePlatformSync::PLAYSTATION:
		PlatformText = TEXT("psn");
		break;
	default:
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Platform Sync Type is not found"));
		return;
	}

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/%s/sync"), *Settings.PlatformServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *PlatformText);

	FString Verb = TEXT("PUT");
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

} // Namespace Api
}
