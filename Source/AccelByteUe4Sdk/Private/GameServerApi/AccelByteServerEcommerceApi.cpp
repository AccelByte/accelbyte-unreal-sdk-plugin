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
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

ServerEcommerce::~ServerEcommerce()
{}

void ServerEcommerce::QueryUserEntitlements(const FString& UserId
	, bool bActiveOnly
	, const FString& EntitlementName
	, const TArray<FString>& ItemIds
	, const int32& Offset
	, const int32& Limit
	, const THandler<FAccelByteModelsEntitlementPagingSlicedResult>& OnSuccess
	, const FErrorHandler& OnError
	, EAccelByteEntitlementClass EntitlementClass
	, EAccelByteAppType AppType
	, const TArray<FString>& Features)
{
	FReport::Log(FString(__FUNCTION__));

	FString Verb = TEXT("GET");
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);
		
	TMultiMap<FString, FString> QueryParams{};
	QueryParams.Add( TEXT("activeOnly"), bActiveOnly ? TEXT("true") : TEXT("false") );
	
	if (!EntitlementName.IsEmpty())
	{
		QueryParams.Add( TEXT("entitlementName"), EntitlementName);
	}
	
	for (const auto& ItemId : ItemIds)
	{
		if (!ItemId.IsEmpty())
		{
			QueryParams.AddUnique(TEXT("itemId"), ItemId);
		}
	}
	
	for (const auto& Feature : Features)
	{
		if (!Feature.IsEmpty())
		{
			QueryParams.AddUnique(TEXT("features"), Feature);
		}
	}

	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}

	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	if (EntitlementClass != EAccelByteEntitlementClass::NONE)
	{
		QueryParams.Add(TEXT("entitlementClazz"),FAccelByteUtilities::GetUEnumValueAsString(EntitlementClass));
	}

	if (AppType != EAccelByteAppType::NONE)
	{
		QueryParams.Add(TEXT("appType"), FAccelByteUtilities::GetUEnumValueAsString(AppType));
	}
	
	HttpClient.ApiRequest(Verb, Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerEcommerce::GetUserEntitlementById(const FString& Entitlementid
	, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/entitlements/%s")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *Entitlementid);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerEcommerce::GetUserEntitlementById(const FString& UserId
	, const FString& EntitlementId
	, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *EntitlementId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerEcommerce::GrantUserEntitlements(const FString& UserId
	, const TArray<FAccelByteModelsEntitlementGrant>& EntitlementGrant
	, const THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

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

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Contents, OnSuccess, OnError);
}

void ServerEcommerce::CreditUserWallet(const FString& UserId
	, const FString& CurrencyCode
	, const FAccelByteModelsCreditUserWalletRequest& CreditUserWalletRequest
	, const THandler<FAccelByteModelsWalletInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "This does not support for multiplatform wallet, use CreditUserWalletV2 instead.");

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/wallets/%s/credit")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *CurrencyCode);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, CreditUserWalletRequest, OnSuccess, OnError);
}

void ServerEcommerce::CreditUserWalletV2(const FString& UserId
	, const FString& CurrencyCode
	, const FAccelByteModelsCreditUserWalletRequest& CreditUserWalletRequest
	, const THandler<FAccelByteModelsWalletCreditResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/wallets/%s/credit")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *CurrencyCode);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, CreditUserWalletRequest, OnSuccess, OnError);
}

void ServerEcommerce::RevokeUserEntitlements(const FString& UserId
	, const TArray<FString>& EntitlementIds
	, const THandler<FAccelByteModelsBulkRevokeEntitlements>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/revoke/byIds")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	const TMultiMap<FString, FString> QueryParams= {
		{TEXT("entitlementIds"), FString::Join(EntitlementIds, TEXT(","))}
	};

	HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerEcommerce::RevokeUserEntitlement(const FString& UserId
	, const FString& EntitlementId
	, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s/revoke")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *EntitlementId);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerEcommerce::ConsumeUserEntitlement(const FString& UserId
	, const FString& EntitlementId
	, int32 UseCount
	, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess
	, const FErrorHandler& OnError
	, TArray<FString> Options
	, FString const& RequestId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s/decrement")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *EntitlementId);

	FAccelByteModelsConsumeUserEntitlementRequest ConsumeUserEntitlementRequest;
	ConsumeUserEntitlementRequest.UseCount = UseCount;
	ConsumeUserEntitlementRequest.Options = Options;
	ConsumeUserEntitlementRequest.RequestId = RequestId;

	FString Content;
	TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(ConsumeUserEntitlementRequest);
	FAccelByteUtilities::RemoveEmptyStrings(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

void ServerEcommerce::DisableUserEntitlement(const FString& UserId
	, const FString& EntitlementId
	, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s/disable")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *EntitlementId);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerEcommerce::EnableUserEntitlement(const FString& UserId
	, const FString& EntitlementId
	, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s/enable")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *EntitlementId);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerEcommerce::DebitUserWallet(const FString& UserId
	, const FString& WalletId
	, const FAccelByteModelsDebitUserWalletRequest& DebitUserWalletRequest
	, const THandler<FAccelByteModelsWalletInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "This end point will be deprecated, old wallet id means one currency, but now it only means one wallet, so we wanna use currency and source instead it.");

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/wallets/%s/debit")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *WalletId);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, DebitUserWalletRequest, OnSuccess, OnError);
}

void ServerEcommerce::FulfillUserItem(const FString& UserId
	, const FAccelByteModelsFulfillmentRequest& FulfillmentRequest
	, const THandler<FAccelByteModelsFulfillmentResult>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/fulfillment")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FulfillmentRequest, OnSuccess, OnError);
}

void ServerEcommerce::BulkGetItemsBySkus(TArray<FString> const& Skus
	, THandler<TArray<FAccelByteModelsBulkGetItemsBySkus>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/items/itemId/bySkus")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

	TMultiMap<FString, FString> QueryParams{};
	
	for (const auto& Sku : Skus)
	{
		if (!Sku.IsEmpty())
		{
			QueryParams.AddUnique(TEXT("sku"), Sku);
		}
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerEcommerce::ListStores(THandler<TArray<FAccelByteModelsPlatformStore>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/stores")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerEcommerce::QueryItemsByCriteria(const FAccelByteModelsItemCriteriaV2& ItemCriteria
	, THandler<FAccelByteModelsItemPagingSlicedResultV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/items/byCriteria")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

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

	TMultiMap<FString, FString> QueryParams = {
		{ TEXT("storeId"), ItemCriteria.StoreId },
		{ TEXT("categoryPath"), ItemCriteria.CategoryPath },
		{ TEXT("includeSubCategoryItem"), ItemCriteria.IncludeSubCategoryItem ? TEXT("true"):TEXT("false") },
		{ TEXT("baseAppId"), ItemCriteria.BaseAppId },
		{ TEXT("tags"), FString::Join(ItemCriteria.Tags, TEXT(",")) },
		{ TEXT("features"), FString::Join(ItemCriteria.Features, TEXT(","))  },
		{ TEXT("activeOnly"), ItemCriteria.ActiveOnly ? TEXT("true"):TEXT("false") },
		{ TEXT("region"), ItemCriteria.Region },
		{ TEXT("availableDate"), AvailableDate },
		{ TEXT("targetNamespace"), ItemCriteria.TargetNamespace },
		{ TEXT("sortBy"), FString::Join(SortByStringArray, TEXT(","))  },
		{ TEXT("itemType"), ItemCriteria.ItemType != EAccelByteItemType::NONE ?
			FAccelByteUtilities::GetUEnumValueAsString(ItemCriteria.ItemType) : TEXT("") },
		{ TEXT("appType"), ItemCriteria.AppType != EAccelByteAppType::NONE ?
			FAccelByteUtilities::GetUEnumValueAsString(ItemCriteria.AppType) : TEXT("")  },
		{ TEXT("offset"), ItemCriteria.Offset > 0 ? FString::FromInt(ItemCriteria.Offset) : TEXT("") },
		{ TEXT("limit"), ItemCriteria.Limit > 0 ? FString::FromInt(ItemCriteria.Limit) : TEXT("") },
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerEcommerce::QueryItemsByCriteriaV2(const FAccelByteModelsItemCriteriaV3& ItemCriteria
	, THandler<FAccelByteModelsItemPagingSlicedResultV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/items/byCriteria")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

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

	TMultiMap<FString, FString> QueryParams = {
		{ TEXT("storeId"), ItemCriteria.StoreId },
		{ TEXT("categoryPath"), ItemCriteria.CategoryPath },
		{ TEXT("includeSubCategoryItem"), ItemCriteria.IncludeSubCategoryItem ? TEXT("true"):TEXT("false") },
		{ TEXT("itemType"), ItemCriteria.ItemType != EAccelByteItemType::NONE ?
			FAccelByteUtilities::GetUEnumValueAsString(ItemCriteria.ItemType) : TEXT("") },
		{ TEXT("appType"), ItemCriteria.AppType != EAccelByteAppType::NONE ?
			FAccelByteUtilities::GetUEnumValueAsString(ItemCriteria.AppType) : TEXT("")  },
		{ TEXT("baseAppId"), ItemCriteria.BaseAppId },
		{ TEXT("tags"), FString::Join(ItemCriteria.Tags, TEXT(",")) },
		{ TEXT("features"), FString::Join(ItemCriteria.Features, TEXT(","))  },
		{ TEXT("region"), ItemCriteria.Region },
		{ TEXT("availableDate"), AvailableDate },
		{ TEXT("targetNamespace"), ItemCriteria.TargetNamespace },
		{ TEXT("itemName"), ItemCriteria.ItemName },
		{ TEXT("sectionExclusive"), ItemCriteria.bSectionExclusive ? TEXT("true") : TEXT("false") },
		{ TEXT("offset"), ItemCriteria.Offset > 0 ? FString::FromInt(ItemCriteria.Offset) : TEXT("") },
		{ TEXT("limit"), ItemCriteria.Limit > 0 ? FString::FromInt(ItemCriteria.Limit) : TEXT("") },
		{ TEXT("sortBy"), FString::Join(SortByStringArray, TEXT(","))  },
	};
 
	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerEcommerce::FulfillRewards(const FString& UserId, const FAccelByteModelsFulfillRewards& FulfillRewardsRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/fulfillment/rewards")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FulfillRewardsRequest, OnSuccess, OnError);
}

} // Namespace GameServerApi
} // Namespace AccelByte
