// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerEcommerceApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerEcommerce::ServerEcommerce(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
	, HttpRef{InHttpRef}
{}

ServerEcommerce::~ServerEcommerce()
{}

void ServerEcommerce::QueryUserEntitlements(const FString& UserId, bool bActiveOnly, const FString& EntitlementName, const TArray<FString>& ItemIds, const int32& Offset, const int32& Limit,
	const THandler<FAccelByteModelsEntitlementPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError, EAccelByteEntitlementClass EntitlementClass, EAccelByteAppType AppType)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *UserId);

	TMap<FString, FString> QueryParams = {};
	QueryParams.Add("activeOnly", bActiveOnly ? TEXT("true") : TEXT("false"));
	if (!EntitlementName.IsEmpty())
	{
		QueryParams.Add("entitlementName", *EntitlementName);
	}
	for (const FString& ItemId : ItemIds)
	{
		if (!ItemId.IsEmpty())
		{
			QueryParams.Add("itemId", *ItemId);
		}
	}
	if (Offset>=0)
	{
		QueryParams.Add("offset", FString::FromInt(Offset));
	}
	if (Limit>=0)
	{
		QueryParams.Add("limit", FString::FromInt(Limit));
	}
	if (EntitlementClass != EAccelByteEntitlementClass::NONE)
	{
		QueryParams.Add("entitlementClazz", FAccelByteUtilities::GetUEnumValueAsString(EntitlementClass));
	}
	if (AppType != EAccelByteAppType::NONE)
	{
		QueryParams.Add("appType", FAccelByteUtilities::GetUEnumValueAsString(AppType));
	}

	// Converting TMap QueryParams as one line QueryString 
	FString QueryString;
	int i = 0;
	for (const auto& Kvp : QueryParams)
	{
		QueryString.Append(FString::Printf(TEXT("%s%s=%s"), (i++ == 0 ? TEXT("?") : TEXT("&")),
				*FGenericPlatformHttp::UrlEncode(Kvp.Key), *FGenericPlatformHttp::UrlEncode(Kvp.Value)));
	}

	Url.Append(QueryString);
	
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

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::GetUserEntitlementById(const FString& Entitlementid, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/entitlements/%s"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *Entitlementid);

	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::GetUserEntitlementById(const FString& UserId, const FString& EntitlementId,
	const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *EntitlementId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::GrantUserEntitlements(const FString& UserId, const TArray<FAccelByteModelsEntitlementGrant>& EntitlementGrant, const THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Contents = "[";
	for (int i =0; i < EntitlementGrant.Num(); i++)
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
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::CreditUserWallet(const FString& UserId, const FString& CurrencyCode, const FAccelByteModelsCreditUserWalletRequest& CreditUserWalletRequest, const THandler<FAccelByteModelsWalletInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "This does not support for multiplatform wallet, use CreditUserWalletV2 instead.");

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/wallets/%s/credit"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *CurrencyCode);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(CreditUserWalletRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::CreditUserWalletV2(const FString& UserId, const FString& CurrencyCode, const FAccelByteModelsCreditUserWalletRequest& CreditUserWalletRequest, const THandler<FAccelByteModelsWalletCreditResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/wallets/%s/credit"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *CurrencyCode);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(CreditUserWalletRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::RevokeUserEntitlements(const FString& UserId, const TArray<FString>& EntitlementIds,
	const THandler<FAccelByteModelsBulkRevokeEntitlements>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/revoke/byIds?entitlementIds="), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *UserId);
	for(int i = 0; i < EntitlementIds.Num(); i++)
	{
		Url += EntitlementIds[i];
		if(i != EntitlementIds.Num() - 1)
			Url += TEXT("%2C");
	}
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::RevokeUserEntitlement(const FString& UserId, const FString& EntitlementId,
	const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s/revoke"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *EntitlementId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	
}

void ServerEcommerce::ConsumeUserEntitlement(const FString& UserId, const FString& EntitlementId, int32 UseCount,
	const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError,
	TArray<FString> Options, FString const& RequestId)
{
	FReport::Log(FString(__FUNCTION__));

	FAccelByteModelsConsumeUserEntitlementRequest ConsumeUserEntitlementRequest;
	ConsumeUserEntitlementRequest.UseCount = UseCount;
	ConsumeUserEntitlementRequest.Options = Options;
	ConsumeUserEntitlementRequest.RequestId = RequestId;

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s/decrement"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *EntitlementId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FString Content;
	TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(ConsumeUserEntitlementRequest);
	FAccelByteUtilities::RemoveEmptyStrings(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::DisableUserEntitlement(const FString& UserId, const FString& EntitlementId,
	const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s/disable"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *EntitlementId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::EnableUserEntitlement(const FString& UserId, const FString& EntitlementId,
	const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s/enable"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *EntitlementId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::DebitUserWallet(const FString& UserId, const FString& WalletId,
	const FAccelByteModelsDebitUserWalletRequest& DebitUserWalletRequest,
	const THandler<FAccelByteModelsWalletInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "This end point will be deprecated, old wallet id means one currency, but now it only means one wallet, so we wanna use currency and source instead it.");

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/wallets/%s/debit"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *WalletId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(DebitUserWalletRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::FulfillUserItem(const FString& UserId, const FAccelByteModelsFulfillmentRequest& FulfillmentRequest, const THandler<FAccelByteModelsFulfillmentResult>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/fulfillment"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(FulfillmentRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::BulkGetItemsBySkus(TArray<FString> const& Skus, THandler<TArray<FAccelByteModelsBulkGetItemsBySkus>> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Params;
	for(FString const& Sku : Skus)
	{
		if(!Params.IsEmpty())
		{
			Params = FString::Printf(TEXT("%s&"), *Params);
		}
		Params = FString::Printf(TEXT("%ssku=%s"), *Params, *Sku);
	}
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/items/itemId/bySkus?%s"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace(), *Params);

	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::ListStores(THandler<TArray<FAccelByteModelsPlatformStore>> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/stores"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace());

	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerEcommerce::QueryItemsByCriteria(const FAccelByteModelsItemCriteriaV2& ItemCriteria, THandler<FAccelByteModelsItemPagingSlicedResultV2> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/items/byCriteria"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetClientNamespace());

	TArray<FString> SortByStringArray = {};
	if (ItemCriteria.SortBy.Num() > 0 )
	{		 
		for (auto SortByEnum : ItemCriteria.SortBy)
		{
			if (SortByEnum != EAccelByteItemListSortBy::NONE)
			{
				SortByStringArray.Add(FAccelByteUtilities::ConvertItemSortByToString(SortByEnum));
			}
		} 
	} 
	FString AvailableDateRounded{}; 
	FString AvailableDateDecimal{};
	ItemCriteria.AvailableDate.ToIso8601().Split(TEXT("."), &AvailableDateRounded, &AvailableDateDecimal);
	FString AvailableDate = FString::Printf(TEXT("%sZ"), *AvailableDateRounded);
	FString QueryParams = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("storeId"), ItemCriteria.StoreId },
		{ TEXT("categoryPath"), ItemCriteria.CategoryPath },
		{ TEXT("includeSubCategoryItem"), ItemCriteria.IncludeSubCategoryItem ? TEXT("true"):TEXT("false") },
		{ TEXT("itemType"), ItemCriteria.ItemType != EAccelByteItemType::NONE ?
		 	FAccelByteUtilities::GetUEnumValueAsString(ItemCriteria.ItemType) : TEXT("") },
		{ TEXT("appType"), ItemCriteria.AppType != EAccelByteAppType::NONE ?
				FAccelByteUtilities::GetUEnumValueAsString(ItemCriteria.AppType) : TEXT("")  },
		{ TEXT("baseAppId"), ItemCriteria.BaseAppId },
		{ TEXT("tags"), FAccelByteUtilities::CreateQueryParamValueUrlEncodedFromArray(ItemCriteria.Tags) },
		{ TEXT("features"), FAccelByteUtilities::CreateQueryParamValueUrlEncodedFromArray(ItemCriteria.Features)  },
		{ TEXT("activeOnly"), ItemCriteria.ActiveOnly ? TEXT("true"):TEXT("false") },
		{ TEXT("region"), ItemCriteria.Region },
		{ TEXT("availableDate"), AvailableDate },
		{ TEXT("targetNamespace"), ItemCriteria.TargetNamespace },
		{ TEXT("offset"), ItemCriteria.Offset > 0 ? FString::Printf(TEXT("%d"), ItemCriteria.Offset) : TEXT("") },
		{ TEXT("limit"), ItemCriteria.Limit > 0 ? FString::Printf(TEXT("%d"), ItemCriteria.Limit) : TEXT("") },
		{ TEXT("sortBy"), FAccelByteUtilities::CreateQueryParamValueUrlEncodedFromArray(SortByStringArray)  },
		});
	Url.Append(QueryParams);
	
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace GameServerApi
} // Namespace AccelByte
