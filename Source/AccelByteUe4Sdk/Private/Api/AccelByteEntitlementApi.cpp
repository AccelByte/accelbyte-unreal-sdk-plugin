// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteEntitlementApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteHttpClient.h"
#include "JsonUtilities.h"
#include "EngineMinimal.h"
#include "Core/AccelByteJwtWrapper.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

Entitlement::Entitlement(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{
}

Entitlement::~Entitlement(){}

FAccelByteTaskWPtr Entitlement::GetCurrentUserEntitlementHistory(THandler<FAccelByteModelsUserEntitlementHistoryPagingResult> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteEntitlementClass const& EntitlementClass
	, FDateTime const& StartDate
	, FDateTime const& EndDate
	, int32 Limit
	, int32 Offset) 
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/history")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	FString TempStartDate;
	FString TempEndDate;

	if (StartDate != 0)
	{
		FString AvailableDateRounded{};
		FString AvailableDateDecimal{};
		StartDate.ToIso8601().Split(TEXT("."), &AvailableDateRounded, &AvailableDateDecimal);
		TempStartDate = FString::Printf(TEXT("%sZ"), *AvailableDateRounded);
	}

	if (EndDate != 0)
	{
		FString AvailableDateRounded{};
		FString AvailableDateDecimal{};
		EndDate.ToIso8601().Split(TEXT("."), &AvailableDateRounded, &AvailableDateDecimal);
		TempEndDate = FString::Printf(TEXT("%sZ"), *AvailableDateRounded);
	}

	const TMultiMap<FString, FString> QueryParams{
		{ TEXT("entitlementClazz"), EntitlementClass != EAccelByteEntitlementClass::NONE ? FAccelByteUtilities::GetUEnumValueAsString(EntitlementClass) : TEXT("") },
		{ TEXT("startDate"), StartDate > 0 ? TempStartDate : TEXT("")},
		{ TEXT("endDate"), EndDate > 0 ? TempEndDate : TEXT("") },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::QueryUserEntitlements(FString const& EntitlementName
	, FString const& ItemId
	, int32 Offset
	, int32 Limit
	, THandler<FAccelByteModelsEntitlementPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteEntitlementClass EntitlementClass
	, EAccelByteAppType AppType
	, TArray<FString> const& Features)
{
	FReport::Log(FString(__FUNCTION__));

	TArray<FString> ItemIdsArray = { ItemId };
	return QueryUserEntitlements(EntitlementName, ItemIdsArray, Offset, Limit, OnSuccess, OnError, EntitlementClass, AppType, Features);
}

FAccelByteTaskWPtr Entitlement::QueryUserEntitlements(FString const& EntitlementName
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

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements")
			, *SettingsRef.PlatformServerUrl
			, *CredentialsRef->GetNamespace()
			, *CredentialsRef->GetUserId());
	
	TMultiMap<FString, FString> QueryParams{};

	if (!EntitlementName.IsEmpty())
	{
		QueryParams.Add( TEXT("entitlementName"), FGenericPlatformHttp::UrlEncode(EntitlementName));
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
	
	return HttpClient.ApiRequest("GET", Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::GetUserEntitlementById(FString const& Entitlementid
	, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/%s")
		, *SettingsRef.PlatformServerUrl, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *Entitlementid);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::GetUserEntitlementOwnershipByAppId(FString const& AppId
	, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess
	, FErrorHandler const& OnError
	, bool bUsePublisherNamespace)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/entitlements/ownership/byAppId")
		, *SettingsRef.PlatformServerUrl
		, bUsePublisherNamespace ? *SettingsRef.PublisherNamespace : *SettingsRef.Namespace);

	const TMultiMap<FString, FString> QueryParams
	{
		{"appId", AppId}
	};
	
	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::GetUserEntitlementOwnershipBySku(FString const& Sku
	, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess
	, FErrorHandler const& OnError
	, bool bUsePublisherNamespace)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/entitlements/ownership/bySku")
		, *SettingsRef.PlatformServerUrl
		, bUsePublisherNamespace ? *SettingsRef.PublisherNamespace : *SettingsRef.Namespace);

	const TMultiMap<FString, FString> QueryParams
	{
		{"sku", Sku}
	};
	
	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::GetUserEntitlementOwnershipByItemId(FString const& ItemId
	, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess
	, FErrorHandler const& OnError
	, bool bUsePublisherNamespace)
{
	FReport::Log(FString(__FUNCTION__));
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/entitlements/ownership/byItemId")
		, *SettingsRef.PlatformServerUrl
		, bUsePublisherNamespace ? *SettingsRef.PublisherNamespace : *SettingsRef.Namespace );

	const TMultiMap<FString, FString> QueryParams
	{
		{"itemId", ItemId}
	};
	
	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::GetUserEntitlementOwnershipAny(TArray<FString> const& ItemIds
	, TArray<FString> const& AppIds
	, TArray<FString> const& Skus
	, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess
	, FErrorHandler const& OnError
	, bool bUsePublisherNamespace)
{
	FReport::Log(FString(__FUNCTION__));

	if (ItemIds.Num() < 1
		&& AppIds.Num() < 1
		&& Skus.Num() < 1)
	{
		OnError.ExecuteIfBound(EHttpResponseCodes::NotFound, TEXT("Please provide at least one itemId, AppId or Sku."));
	}
	else
	{
		FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/entitlements/ownership/any")
			, *SettingsRef.PlatformServerUrl
			, bUsePublisherNamespace ? *SettingsRef.PublisherNamespace : *SettingsRef.Namespace);

		TMultiMap<FString, FString> QueryParams{};
		
		for (const auto& ItemId : ItemIds)
		{
			if (!ItemId.IsEmpty())
			{
				QueryParams.AddUnique(TEXT("itemIds"), ItemId);
			}
		}

		for (const auto& AppId : AppIds)
		{
			if (!AppId.IsEmpty())
			{
				QueryParams.AddUnique(TEXT("appIds"), AppId);
			}
		}
		
		for (const auto& Sku : Skus)
		{
			if (!Sku.IsEmpty())
			{
				QueryParams.AddUnique(TEXT("skus"), Sku);
			}
		}
		
		return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
	}
	return nullptr;
}

FAccelByteTaskWPtr Entitlement::GetUserEntitlementOwnershipViaToken(FString const& PublicKey
	, TArray<FString> const& ItemIds
	, TArray<FString> const& AppIds
	, TArray<FString> const& Skus
	, THandler<FAccelByteModelsEntitlementOwnershipDetails> const& OnSuccess
	, FErrorHandler const& OnError
	, bool VerifySignature
	, bool VerifyExpiration
	, FString const& VerifySub
	, bool bUsePublisherNamespace)
{
	FReport::Log(FString(__FUNCTION__));

	if (ItemIds.Num() < 1
		&& AppIds.Num() < 1
		&& Skus.Num() < 1)
	{
		OnError.ExecuteIfBound(EHttpResponseCodes::NotFound, TEXT("Please provide at least one itemId, AppId or Sku."));
	}
	else if (PublicKey.IsEmpty())
	{
		OnError.ExecuteIfBound(EHttpResponseCodes::NotFound, TEXT("No Public key provided."));
	}
	else
	{
		auto ProcessOnSuccess = THandler<FAccelByteModelsOwnershipToken>::CreateLambda(
			[OnSuccess, OnError, PublicKey, VerifySignature, VerifyExpiration, VerifySub](const FAccelByteModelsOwnershipToken& Result)
			{
				TSharedPtr<FJsonObject> DecodedResult;
				FAccelByteJwtError Error;
				AccelByteJwtWrapper::TryDecode(Result.OwnershipToken
					, PublicKey
					, DecodedResult
					, Error
					, VerifySignature
					, VerifyExpiration
					, VerifySub);

				if (Error.Code == 0)
				{
					FAccelByteModelsEntitlementOwnershipDetails EntitlementDetails;
					bool isSuccess = FJsonObjectConverter::JsonObjectToUStruct<FAccelByteModelsEntitlementOwnershipDetails>(DecodedResult.ToSharedRef(), &EntitlementDetails);
					if (isSuccess)
					{
						OnSuccess.ExecuteIfBound(EntitlementDetails);
					}
					else
					{
						OnError.ExecuteIfBound(-1, "Cannot parse decoded token.");
					}
				}
				else
				{
					OnError.ExecuteIfBound(Error.Code, Error.Message);
				}
			});
		
		return GetUserEntitlementOwnershipTokenOnly(ItemIds, AppIds, Skus, ProcessOnSuccess, OnError, bUsePublisherNamespace);
	}
	return nullptr;
}
	
FAccelByteTaskWPtr Entitlement::GetUserEntitlementOwnershipTokenOnly(TArray<FString> const& ItemIds
	, TArray<FString> const& AppIds
	, TArray<FString> const& Skus
	, THandler<FAccelByteModelsOwnershipToken> const& OnSuccess
	, FErrorHandler const& OnError
	, bool bUsePublisherNamespace)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (ItemIds.Num() < 1
		&& AppIds.Num() < 1
		&& Skus.Num() < 1)
	{
		OnError.ExecuteIfBound(EHttpResponseCodes::NotFound, TEXT("Please provide at least one itemId, AppId or Sku."));
	}
	else
	{
		FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/entitlements/ownershipToken")
			, *SettingsRef.PlatformServerUrl
			, bUsePublisherNamespace ? *SettingsRef.PublisherNamespace : *SettingsRef.Namespace);

		TMultiMap<FString, FString> QueryParams{};
		
		for (const auto& ItemId : ItemIds)
		{
			if (!ItemId.IsEmpty())
			{
				QueryParams.AddUnique(TEXT("itemIds"), ItemId);
			}
		}

		for (const auto& AppId : AppIds)
		{
			if (!AppId.IsEmpty())
			{
				QueryParams.AddUnique(TEXT("appIds"), AppId);
			}
		}
		
		for (const auto& Sku : Skus)
		{
			if (!Sku.IsEmpty())
			{
				QueryParams.AddUnique(TEXT("skus"), Sku);
			}
		}
		
		return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
	}
	return nullptr;
}

FAccelByteTaskWPtr Entitlement::ConsumeUserEntitlement(FString const& EntitlementId
	, int32 UseCount
	, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, TArray<FString> const& Options
	, FString const& RequestId )
{
	FReport::Log(FString(__FUNCTION__));

	FAccelByteModelsConsumeUserEntitlementRequest ConsumeUserEntitlementRequest;
	ConsumeUserEntitlementRequest.UseCount = UseCount;
	ConsumeUserEntitlementRequest.Options = Options;
	ConsumeUserEntitlementRequest.RequestId = RequestId;

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/%s/decrement")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *EntitlementId);

	FString Content;
	TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(ConsumeUserEntitlementRequest);
	FAccelByteUtilities::RemoveEmptyStrings(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);
	
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::CreateDistributionReceiver(FString const& ExtUserId
	, FAccelByteModelsAttributes const& Attributes
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "Platform Service version 3.4.0 and above doesn't support this anymore");

	OnError.ExecuteIfBound(410, TEXT("This feature is already removed."));
	return nullptr;
}

FAccelByteTaskWPtr Entitlement::DeleteDistributionReceiver(FString const& ExtUserId
	, FString const& UserId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "Platform Service version 3.4.0 and above doesn't support this anymore");

	OnError.ExecuteIfBound(410, TEXT("This feature is already removed."));
	return nullptr;
}

FAccelByteTaskWPtr Entitlement::GetDistributionReceiver(FString const& PublisherNamespace
	, FString const& PublisherUserId
	, THandler<TArray<FAccelByteModelsDistributionReceiver>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "Platform Service version 3.4.0 and above doesn't support this anymore");

	OnError.ExecuteIfBound(410, TEXT("This feature is already removed."));
	return nullptr;
}

FAccelByteTaskWPtr Entitlement::UpdateDistributionReceiver(FString const& ExtUserId
	, FAccelByteModelsAttributes const& Attributes
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "Platform Service version 3.4.0 and above doesn't support this anymore");

	OnError.ExecuteIfBound(410, TEXT("This feature is already removed."));
	return nullptr;
}

FAccelByteTaskWPtr Entitlement::SyncPlatformPurchase(EAccelBytePlatformSync PlatformType
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FAccelByteModelsPlayStationDLCSync PSSyncModel{};
	return SyncPlatformPurchase(PlatformType, PSSyncModel, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncPlatformPurchase(EAccelBytePlatformSync PlatformType
	, FAccelByteModelsPlayStationDLCSync const& PSSyncModel
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FAccelByteModelsEntitlementSyncBase SyncBase;
	SyncBase.ServiceLabel = PSSyncModel.ServiceLabel;
	return SyncPlatformPurchase(SyncBase, PlatformType, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncPlatformPurchase(FAccelByteModelsEntitlementSyncBase const& EntitlementSyncBase
	, EAccelBytePlatformSync PlatformType
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString PlatformText = TEXT("");
	FString PlatformUserId = CredentialsRef->GetPlatformUserId();

	// #TODO: Replace this switch statement with some kind of model backed sync or something
	TSharedRef<FJsonObject> SyncRequestJson = MakeShared<FJsonObject>();
	bool bSyncForSingleProduct = true;
	switch (PlatformType)
	{
	case EAccelBytePlatformSync::STEAM:
		PlatformText = TEXT("steam");
		if (PlatformUserId.IsEmpty())
		{
			OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("User not logged in with 3rd Party Platform"));
			return nullptr;
		}

		SyncRequestJson->SetStringField(TEXT("steamId"), PlatformUserId);
		SyncRequestJson->SetStringField(TEXT("appId"), SettingsRef.AppId);

		// If we don't have a product ID or a currency code set in this base mode, just send the request with user ID and app ID
		if (EntitlementSyncBase.ProductId.IsEmpty() || EntitlementSyncBase.CurrencyCode.IsEmpty())
		{
			bSyncForSingleProduct = false;
			break;
		}

		if (!EntitlementSyncBase.Region.IsEmpty())
		{
			SyncRequestJson->SetStringField(TEXT("region"), EntitlementSyncBase.Region);
		}
		if (!EntitlementSyncBase.Language.IsEmpty())
		{
			SyncRequestJson->SetStringField(TEXT("language"), EntitlementSyncBase.Language);
		}
		break;
	case EAccelBytePlatformSync::XBOX_LIVE:
		PlatformText = TEXT("xbl");
		if (EntitlementSyncBase.ProductId.IsEmpty() || EntitlementSyncBase.CurrencyCode.IsEmpty())
		{
			bSyncForSingleProduct = false;
			break;
		}

		SyncRequestJson->SetStringField(TEXT("xstsToken"), EntitlementSyncBase.XstsToken);
		break;
	case EAccelBytePlatformSync::PLAYSTATION:
		PlatformText = TEXT("psn");
		if (EntitlementSyncBase.ServiceLabel > 0)
		{
			SyncRequestJson->SetNumberField(TEXT("serviceLabel"), EntitlementSyncBase.ServiceLabel);
		}

		if (EntitlementSyncBase.ProductId.IsEmpty() || EntitlementSyncBase.CurrencyCode.IsEmpty())
		{
			bSyncForSingleProduct = false;
			break;
		}
		break;
	case EAccelBytePlatformSync::EPIC_GAMES:
		PlatformText = TEXT("epicgames");
		if (PlatformUserId.IsEmpty())
		{
			OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("User not logged in with 3rd Party Platform"));
			return nullptr;
		}
		bSyncForSingleProduct = false;
		SyncRequestJson->SetStringField(TEXT("epicGamesJwtToken"), EntitlementSyncBase.EpicGamesJwtToken);
		break;
	default:
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Platform Sync Type is not found"));
		return nullptr;
	}

	// If we are trying to sync for a single product, add product information here to request
	if (bSyncForSingleProduct)
	{
		SyncRequestJson->SetStringField(TEXT("productId"), EntitlementSyncBase.ProductId);
		SyncRequestJson->SetNumberField(TEXT("price"), EntitlementSyncBase.Price);
		SyncRequestJson->SetStringField(TEXT("currencyCode"), EntitlementSyncBase.CurrencyCode);
	}

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/%s/sync")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *PlatformText);

	FString ContentString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ContentString);
	if (!FJsonSerializer::Serialize(SyncRequestJson, Writer))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to convert request JSON object to string"));
		return nullptr;
	}

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, ContentString, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncMobilePlatformPurchaseGoogle(FAccelByteModelsPlatformSyncMobileGoogle const& SyncRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),TEXT("This method will be deprecated. Use SyncMobilePlatformPurchaseGooglePlay instead!"));

	FString PlatformText = TEXT("google");

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/%s/receipt")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *PlatformText);
	
	FString Content;
	const TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(SyncRequest);
	FAccelByteUtilities::RemoveEmptyFieldsFromJson(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncMobilePlatformPurchaseGooglePlay(FAccelByteModelsPlatformSyncMobileGoogle const& SyncRequest
	, THandler<FAccelByteModelsPlatformSyncMobileGoogleResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString PlatformText = TEXT("google");
	
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/%s/receipt")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *PlatformText);

	FString Content;
	const TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(SyncRequest);
	FAccelByteUtilities::RemoveEmptyFieldsFromJson(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);
	
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncMobilePlatformPurchaseApple(FAccelByteModelsPlatformSyncMobileApple const& SyncRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString PlatformText = TEXT("apple");
	
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/%s/receipt")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *PlatformText);
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(SyncRequest, Content);
	
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncXBoxDLC(FAccelByteModelsXBoxDLCSync const& XboxDLCSync
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Content = FString::Printf(TEXT("{\"xstsToken\": \"%s\"}"), *XboxDLCSync.XstsToken);
	if (XboxDLCSync.XstsToken.IsEmpty())
	{
		Content = FString::Printf(TEXT("{}"));
	}
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/dlc/xbl/sync")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncSteamDLC(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString platformUserId = CredentialsRef->GetPlatformUserId();

	if (platformUserId.IsEmpty()) 
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("User not logged in with 3rd Party Platform"));
		return nullptr;
	}

	FString Content = TEXT("");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/dlc/steam/sync")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	FJsonObject DataJson;
	DataJson.SetStringField("steamId", platformUserId);
	DataJson.SetStringField("appId", SettingsRef.AppId);
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(DataJson);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncPSNDLC(FAccelByteModelsPlayStationDLCSync const& PSSyncModel
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Content = FString::Printf(TEXT("{\"serviceLabel\": \"%d\"}"), PSSyncModel.ServiceLabel);

	//	URL
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/dlc/psn/sync")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncTwitchDropEntitlement(FAccelByteModelsTwitchDropEntitlement const& TwitchDropModel
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	// Url 
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/twitch/sync")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());
	
	// Content Body 
	FString Content = TEXT("");
	FJsonObject DataJson;
	DataJson.SetStringField("gameId", TwitchDropModel.GameId);
	DataJson.SetStringField("region", TwitchDropModel.Region);
	DataJson.SetStringField("language", TwitchDropModel.Language);
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(DataJson);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
 
	// Api Request 
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncEpicGameDurableItems(FString const& EpicGamesJwtToken
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	// Url 
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/dlc/epicgames/sync")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());	 
	// Content Body 
	FString Content = TEXT("");
	FJsonObject DataJson;
	DataJson.SetStringField("epicGamesJwtToken", EpicGamesJwtToken); 
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(DataJson);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
 
	// Api Request 
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::ValidateUserItemPurchaseCondition(TArray<FString> const& Items
	, THandler<TArray<FAccelByteModelsPlatformValidateUserItemPurchaseResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	// Url 
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/purchase/conditions/validate")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace());

	// Content Body 
	FString Content = TEXT("");
	FJsonObject DataJson;
	TArray<TSharedPtr<FJsonValue>> ItemArray{};
	for (FString Item : Items)
	{
		ItemArray.Add(MakeShareable(new FJsonValueString(Item)));
	}
	DataJson.SetArrayField("itemIds", ItemArray);
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(DataJson);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer); 
	
	// Api Request 
	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::GetUserEntitlementOwnershipByItemIds(TArray<FString> const& Ids
	, THandler<TArray<FAccelByteModelsEntitlementOwnershipItemIds>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	// Url 
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/ownership/byItemIds")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	// Params 
	TMultiMap<FString, FString> QueryParams{};

	for (FString const& Id : Ids)
	{
		if (!Id.IsEmpty())
		{
			QueryParams.AddUnique(TEXT("ids"), Id);
		}
	}

	// Content 
	FString Content = TEXT("");
	
	// Api Request 
	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncWithDLCEntitlementInPSNStore(FAccelByteModelsPlayStationDLCSync const& PSNModel
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__)); 

	FString Content = TEXT("");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/dlc/psn/sync")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	FJsonObject DataJson;
	DataJson.SetStringField("serviceLabel", FString::FromInt(PSNModel.ServiceLabel));
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(DataJson);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncWithEntitlementInPSNStore(const FAccelByteModelsPlayStationIAPSync& PlaystationModel
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__)); 

	FString Content = TEXT("");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/psn/sync")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	FJsonObject DataJson;
	DataJson.SetStringField("productId", PlaystationModel.ProductId);
	DataJson.SetStringField("price", FString::FromInt(PlaystationModel.Price));
	DataJson.SetStringField("currencyCode", PlaystationModel.CurrencyCode);
	DataJson.SetStringField("serviceLabel", FString::FromInt(PlaystationModel.ServiceLabel));
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(DataJson);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SellUserEntitlement(FString const& EntitlementId
	, int32 UseCount
	, FString const& RequestId
	, THandler<FAccelByteModelsSellItemEntitlementInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (EntitlementId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, EntitlementId is empty."));
		return nullptr;
	}
	
	FAccelByteModelsSellUserEntitlementRequest SellUserEntitlementRequest;
	SellUserEntitlementRequest.UseCount = UseCount; 
	SellUserEntitlementRequest.RequestId = RequestId;

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/%s/sell")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *EntitlementId);

	FString Content;
	TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(SellUserEntitlementRequest);
	FAccelByteUtilities::RemoveEmptyStrings(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);
	
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Entitlement::SyncOculusConsumableEntitlements(THandler<TArray<FAccelByteModelsSyncOculusConsumableEntitlementInfo>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));  
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/oculus/sync")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {},  OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Entitlement::SyncOculusDLC(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));  
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/dlc/oculus/sync")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {},  OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncDLCPSNMultipleService(FAccelByteModelsMultipleServicePSNDLCSync const& PlaystationModel
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FString Content = TEXT("");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/dlc/psn/sync/multiServiceLabels")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	const TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(PlaystationModel);
	FAccelByteUtilities::RemoveEmptyStrings(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Entitlement::SyncEntitlementPSNMultipleService(FAccelByteModelsMultipleServicePSNIAPSync const& PlaystationModel
	, THandler<TArray<FAccelByteModelsMultipleServicePlayStationInfo>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__)); 

	FString Content = TEXT("");
	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/psn/sync/multiServiceLabels")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	const TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(PlaystationModel);
	FAccelByteUtilities::RemoveEmptyStrings(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);
	
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}
	
} // Namespace Api
} // Namespace AccelByte