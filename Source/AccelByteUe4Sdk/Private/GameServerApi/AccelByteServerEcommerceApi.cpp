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

FAccelByteTaskWPtr ServerEcommerce::QueryUserEntitlements(FString const& UserId
	, bool bActiveOnly
	, FString const& EntitlementName
	, TArray<FString> const& ItemIds
	, int32 Offset
	, int32 Limit
	, THandler<FAccelByteModelsEntitlementPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteEntitlementClass EntitlementClass
	, EAccelByteAppType AppType
	, TArray<FString> const& Features)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	FString Verb = TEXT("GET");
	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
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
	
	return HttpClient.ApiRequest(Verb, Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::GetUserEntitlementById(FString const& Entitlementid
	, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/entitlements/%s")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Entitlementid);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::GetUserEntitlementById(FString const& UserId
	, FString const& EntitlementId
	, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *EntitlementId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::GrantUserEntitlements(FString const& UserId
	, TArray<FAccelByteModelsEntitlementGrant> const& EntitlementGrant
	, THandler<TArray<FAccelByteModelsStackableEntitlementInfo>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}
	if (UserId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("UserId cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId);

	TArray<TSharedPtr<FJsonValue>> JsonArray;
	for (const FAccelByteModelsEntitlementGrant& Entitlement : EntitlementGrant)
	{
		if (Entitlement.Source == EAccelByteEntitlementSource::NONE)
		{
			// Handle the specific case when Source is NONE
			OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest),
				TEXT("Invalid request. The 'Source' field must be set to a valid item source (e.g., EAccelByteEntitlementSource::PURCHASE, EAccelByteEntitlementSource::REWARD, etc.). Please set a valid 'Source' value to fulfill the request."));
			return nullptr;
		}
		TSharedPtr<FJsonObject> JsonObj = FJsonObjectConverter::UStructToJsonObject(Entitlement);
		FAccelByteUtilities::RemoveEmptyFieldsFromJson(JsonObj, FAccelByteUtilities::FieldRemovalFlagAll);
		JsonArray.Add(MakeShared<FJsonValueObject>(JsonObj));
	}

	FString Contents;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Contents);
	FJsonSerializer::Serialize(JsonArray, Writer);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Contents, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::CreditUserWallet(FString const& UserId
	, FString const& CurrencyCode
	, FAccelByteModelsCreditUserWalletRequest const& CreditUserWalletRequest
	, THandler<FAccelByteModelsWalletInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "This does not support for multiplatform wallet, use CreditUserWalletV2 instead.");

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/wallets/%s/credit")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *CurrencyCode);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, CreditUserWalletRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::CreditUserWalletV2(FString const& UserId
	, FString const& CurrencyCode
	, FAccelByteModelsCreditUserWalletRequest const& CreditUserWalletRequest
	, THandler<FAccelByteModelsWalletCreditResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/wallets/%s/credit")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *CurrencyCode);

	const TDelegate<void(const FAccelByteModelsWalletCreditResponse&)> OnSuccessHttpClient =
		THandler<FAccelByteModelsWalletCreditResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsWalletCreditResponse const& Response)
			{
				FAccelByteModelsWalletCreditResponse Result = Response;
				for (int i = 0; i < Result.timeLimitedBalances.Num(); i++)
				{
					Result.timeLimitedBalances[i].DuplicateExpiredAtValue();
				}
				OnSuccess.ExecuteIfBound(Result);
			});

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, CreditUserWalletRequest, OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::RevokeUserEntitlements(FString const& UserId
	, TArray<FString> const& EntitlementIds
	, THandler<FAccelByteModelsBulkRevokeEntitlements> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/revoke/byIds")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId);

	const TMultiMap<FString, FString> QueryParams= {
		{TEXT("entitlementIds"), FString::Join(EntitlementIds, TEXT(","))}
	};

	return HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::RevokeUserEntitlement(FString const& UserId
	, FString const& EntitlementId
	, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s/revoke")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *EntitlementId);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::ConsumeUserEntitlement(FString const& UserId
	, FString const& EntitlementId
	, int32 UseCount
	, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, TArray<FString> Options
	, FString const& RequestId)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s/decrement")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
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

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::DisableUserEntitlement(FString const& UserId
	, FString const& EntitlementId
	, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s/disable")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *EntitlementId);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::EnableUserEntitlement(FString const& UserId
	, FString const& EntitlementId
	, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s/enable")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *EntitlementId);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::GetUserEntitlementHistory(FString const& UserId
	, FString const& EntitlementId
	, THandler<TArray<FAccelByteModelsUserEntitlementHistory>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	if (!ValidateAccelByteId(EntitlementId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetEntitlementIdInvalidMessage(EntitlementId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/entitlements/%s/history")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *EntitlementId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::DebitUserWallet(FString const& UserId
	, FString const& WalletId
	, FAccelByteModelsDebitUserWalletRequest const& DebitUserWalletRequest
	, THandler<FAccelByteModelsWalletInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "This end point will be deprecated, old wallet id means one currency, but now it only means one wallet, so we wanna use currency and source instead it.");

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/wallets/%s/debit")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *WalletId);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, DebitUserWalletRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::DebitUserWalletV2(FString const& UserId
	, FString const& CurrencyCode
	, FAccelByteModelsDebitUserWalletRequestV2 const& DebitUserWalletRequest
	, THandler<FAccelByteModelsWalletInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	if(CurrencyCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("CurrencyCode parameter cannot be empty!")));
		return nullptr;
	}
	
	if(DebitUserWalletRequest.Amount < 1)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Debit amount must be a minimum of 1, request debit amount is %lld"), DebitUserWalletRequest.Amount));
		return nullptr;
	}

	if(DebitUserWalletRequest.BalanceOrigin == EAccelBytePlatformBalanceOrigin::NONE)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Debit BalanceOrigin field cannot be NONE"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/wallets/currencies/%s/debit")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *CurrencyCode);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, DebitUserWalletRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::PaymentWithUserWallet(FString const& UserId
	, FString const& CurrencyCode
	, FAccelByteModelsPaymentUserWalletRequest const& PaymentRequest
	, THandler<FAccelByteModelsPlatformWallet> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	if(CurrencyCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("CurrencyCode parameter cannot be empty!")));
		return nullptr;
	}
	
	if(PaymentRequest.Amount < 1)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Debit amount must be a minimum of 1, request debit amount is %lld"), PaymentRequest.Amount));
		return nullptr;
	}

	if(PaymentRequest.WalletPlatform == EAccelByteWalletPlatform::NONE)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Payment WalletPlatform field cannot be NONE"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/wallets/%s/payment")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *CurrencyCode);

	const TDelegate<void(const FAccelByteModelsPlatformWallet&)> OnSuccessHttpClient =
		THandler<FAccelByteModelsPlatformWallet>::CreateLambda(
			[OnSuccess](FAccelByteModelsPlatformWallet const& Response)
			{
				FAccelByteModelsPlatformWallet Result = Response;
				for (int i = 0; i < Result.WalletInfos.Num(); i++)
				{
					Result.WalletInfos[i].DuplicateTimeLimitedBalancesExpirationInfo();
				}
				OnSuccess.ExecuteIfBound(Result);
			});

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, PaymentRequest, OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::FulfillUserItem(FString const& UserId
	, FAccelByteModelsFulfillmentRequest const& FulfillmentRequest
	, THandler<FAccelByteModelsFulfillmentResult> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	if (FulfillmentRequest.Source == EAccelByteItemSource::NONE)
	{
		const FString Message = TEXT("Invalid request. The 'Source' field must be set to a valid item source "
							   "(e.g., EAccelByteItemSource::PURCHASED, EAccelByteItemSource::REWARDED, etc.). "
							   "Please set a valid 'Source' value to fulfill the request.");
		// Handle the specific case when Source is NONE
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest),Message);
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/fulfillment")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId);

	FString Content;
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(FulfillmentRequest);
	if (FulfillmentRequest.Origin == EAccelBytePlatformRewardOrigin::NONE)
	{
		JsonObject->RemoveField(TEXT("Origin"));
	}

	if (FulfillmentRequest.Order.Currency.CurrencyType == EAccelByteItemCurrencyType::NONE)
	{
		JsonObject->RemoveField(TEXT("Order"));
	}
	
	if (FulfillmentRequest.StartDate != 0 || FulfillmentRequest.EndDate != 0)
	{
		FString StartDate;
		FString EndDate;
		if (FulfillmentRequest.StartDate != 0)
		{
			FString AvailableDateRounded{};
			FString AvailableDateDecimal{};
			FulfillmentRequest.StartDate.ToIso8601().Split(TEXT("."), &AvailableDateRounded, &AvailableDateDecimal);
			StartDate = FString::Printf(TEXT("%sZ"), *AvailableDateRounded);
			// Add StartDate and EndDate to JsonObject
			JsonObject->SetStringField(TEXT("StartDate"), StartDate);
		}

		if (FulfillmentRequest.EndDate != 0)
		{
			FString AvailableDateRounded{};
			FString AvailableDateDecimal{};
			FulfillmentRequest.EndDate.ToIso8601().Split(TEXT("."), &AvailableDateRounded, &AvailableDateDecimal);
			EndDate = FString::Printf(TEXT("%sZ"), *AvailableDateRounded);
			// Add StartDate and EndDate to JsonObject
			JsonObject->SetStringField(TEXT("EndDate"), EndDate);
		}
	}
	FAccelByteUtilities::RemoveEmptyFieldsFromJson(JsonObject, FAccelByteUtilities::FieldRemovalFlagAll | FAccelByteUtilities::FieldRemovalFlagNumbersZeroValues);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, JsonObject, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::BulkGetItemsBySkus(TArray<FString> const& Skus
	, THandler<TArray<FAccelByteModelsBulkGetItemsBySkus>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/items/itemId/bySkus")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	TMultiMap<FString, FString> QueryParams{};
	
	for (const auto& Sku : Skus)
	{
		if (!Sku.IsEmpty())
		{
			QueryParams.AddUnique(TEXT("sku"), Sku);
		}
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::ListStores(THandler<TArray<FAccelByteModelsPlatformStore>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/stores")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::QueryItemsByCriteria(FAccelByteModelsItemCriteriaV2 const& ItemCriteria
	, THandler<FAccelByteModelsItemPagingSlicedResultV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/items/byCriteria")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

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

	TMultiMap<FString, FString> QueryParams = {
		{ TEXT("storeId"), ItemCriteria.StoreId },
		{ TEXT("categoryPath"), ItemCriteria.CategoryPath },
		{ TEXT("includeSubCategoryItem"), ItemCriteria.IncludeSubCategoryItem ? TEXT("true"):TEXT("false") },
		{ TEXT("baseAppId"), ItemCriteria.BaseAppId },
		{ TEXT("tags"), FString::Join(ItemCriteria.Tags, TEXT(",")) },
		{ TEXT("features"), FString::Join(ItemCriteria.Features, TEXT(","))  },
		{ TEXT("activeOnly"), ItemCriteria.ActiveOnly ? TEXT("true"):TEXT("false") },
		{ TEXT("region"), ItemCriteria.Region },
		{ TEXT("targetNamespace"), ItemCriteria.TargetNamespace },
		{ TEXT("sortBy"), FString::Join(SortByStringArray, TEXT(","))  },
		{ TEXT("itemType"), ItemCriteria.ItemType != EAccelByteItemType::NONE ?
			FAccelByteUtilities::GetUEnumValueAsString(ItemCriteria.ItemType) : TEXT("") },
		{ TEXT("appType"), ItemCriteria.AppType != EAccelByteAppType::NONE ?
			FAccelByteUtilities::GetUEnumValueAsString(ItemCriteria.AppType) : TEXT("")  },
		{ TEXT("offset"), ItemCriteria.Offset > 0 ? FString::FromInt(ItemCriteria.Offset) : TEXT("") },
		{ TEXT("limit"), ItemCriteria.Limit > 0 ? FString::FromInt(ItemCriteria.Limit) : TEXT("") },
	};

	if (ItemCriteria.AvailableDate != 0)
	{
		FString AvailableDateRounded{};
		FString AvailableDateDecimal{};
		ItemCriteria.AvailableDate.ToIso8601().Split(TEXT("."), &AvailableDateRounded, &AvailableDateDecimal);
		FString AvailableDate = FString::Printf(TEXT("%sZ"), *AvailableDateRounded);

		QueryParams.Add(TTuple<FString, FString>{TEXT("availableDate"), AvailableDate });
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::QueryItemsByCriteriaV2(FAccelByteModelsItemCriteriaV3 const& ItemCriteria
	, THandler<FAccelByteModelsItemPagingSlicedResultV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/items/byCriteria")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

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
		{ TEXT("targetNamespace"), ItemCriteria.TargetNamespace },
		{ TEXT("itemName"), ItemCriteria.ItemName },
		{ TEXT("sectionExclusive"), ItemCriteria.bSectionExclusive ? TEXT("true") : TEXT("false") },
		{ TEXT("offset"), ItemCriteria.Offset > 0 ? FString::FromInt(ItemCriteria.Offset) : TEXT("") },
		{ TEXT("limit"), ItemCriteria.Limit > 0 ? FString::FromInt(ItemCriteria.Limit) : TEXT("") },
		{ TEXT("sortBy"), FString::Join(SortByStringArray, TEXT(","))  },
	};
 
	if (ItemCriteria.AvailableDate != 0)
	{
		FString AvailableDateRounded{};
		FString AvailableDateDecimal{};
		ItemCriteria.AvailableDate.ToIso8601().Split(TEXT("."), &AvailableDateRounded, &AvailableDateDecimal);
		FString AvailableDate = FString::Printf(TEXT("%sZ"), *AvailableDateRounded);

		QueryParams.Add(TTuple<FString, FString>{ TEXT("availableDate"), AvailableDate });
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::FulfillRewards(FString const& UserId
	, FAccelByteModelsFulfillRewards const& FulfillRewardsRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/fulfillment/rewards")
		, *ServerSettingsRef.PlatformServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FulfillRewardsRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerEcommerce::BulkGetItemsById(TArray<FString> const& ItemIds
	, const bool bActiveOnly
	, THandler<TArray<FAccelByteModelsItemInfoV2>> const& OnSuccess
	, FErrorHandler const& OnError
	, const FString& StoreId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/items/byIds"), *ServerSettingsRef.PlatformServerUrl, *ServerCredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
	{ TEXT("itemIds"), FString::Join(ItemIds, TEXT(",")) },
	{ TEXT("activeOnly"), bActiveOnly ? TEXT("true") : TEXT("false") },
	{ TEXT("storeId"), StoreId }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

} // Namespace GameServerApi
} // Namespace AccelByte
