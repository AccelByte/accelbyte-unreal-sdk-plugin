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

void Entitlement::QueryUserEntitlements(FString const& EntitlementName, FString const& ItemId, int32 const& Offset, int32 const& Limit, THandler<FAccelByteModelsEntitlementPagingSlicedResult> const& OnSuccess, FErrorHandler const& OnError, EAccelByteEntitlementClass EntitlementClass = EAccelByteEntitlementClass::NONE, EAccelByteAppType AppType = EAccelByteAppType::NONE, TArray<FString> const& Features)
{
	FReport::Log(FString(__FUNCTION__));

	TArray<FString> ItemIdsArray = { ItemId };
	QueryUserEntitlements(EntitlementName, ItemIdsArray, Offset, Limit, OnSuccess, OnError, EntitlementClass, AppType, Features);

}

void Entitlement::QueryUserEntitlements(
	FString const& EntitlementName,
	TArray<FString> const& ItemIds,
	int32 const& Offset,
	int32 const& Limit,
	THandler<FAccelByteModelsEntitlementPagingSlicedResult> const& OnSuccess,
	FErrorHandler const& OnError, EAccelByteEntitlementClass EntitlementClass, EAccelByteAppType AppType,
	TArray<FString> const& Features)
{ 

	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());
 	
	FString QueryParams = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("entitlementName"), EntitlementName },
		{ "itemId", FString::Join(ItemIds, TEXT("&itemId=")) },
		{ "features", FString::Join(Features, TEXT("&features=")) },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("entitlementClazz"), EntitlementClass != EAccelByteEntitlementClass::NONE ?
				*FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccelByteEntitlementClass"), true)->GetNameStringByValue((int32)EntitlementClass) : TEXT("")},
		{ TEXT("appType"), AppType != EAccelByteAppType::NONE ?
				*FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccelByteAppType"), true)->GetNameStringByValue((int32)AppType) : TEXT("")},
	});
	Url.Append(QueryParams);

	HttpClient.ApiRequest("GET", Url, {}, FString(), OnSuccess, OnError);
}

void Entitlement::GetUserEntitlementById(FString const& Entitlementid, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/%s"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *Entitlementid);

	HttpClient.ApiRequest("GET", Url, {}, FString(), OnSuccess, OnError);
}

void Entitlement::GetUserEntitlementOwnershipByAppId(FString const& AppId, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/entitlements/ownership/byAppId"), *SettingsRef.PlatformServerUrl, *SettingsRef.PublisherNamespace, *AppId);

	const TMap<FString, FString> QueryParams
	{
		{"appId", AppId}
	};
	
	HttpClient.ApiRequest("GET", Url, QueryParams, FString(), OnSuccess, OnError);
}

void Entitlement::GetUserEntitlementOwnershipBySku(FString const& Sku, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/entitlements/ownership/bySku"), *SettingsRef.PlatformServerUrl, *SettingsRef.PublisherNamespace, *Sku);

	const TMap<FString, FString> QueryParams
	{
		{"sku", Sku}
	};
	
	HttpClient.ApiRequest("GET", Url, QueryParams, FString(), OnSuccess, OnError);
}

void Entitlement::GetUserEntitlementOwnershipByItemId(FString const& ItemId, THandler<FAccelByteModelsEntitlementOwnership> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/entitlements/ownership/byItemId"), *SettingsRef.PlatformServerUrl, *SettingsRef.Namespace, *ItemId);

	const TMap<FString, FString> QueryParams
	{
		{"itemId", ItemId}
	};
	
	HttpClient.ApiRequest("GET", Url, QueryParams, FString(), OnSuccess, OnError);
}

void Entitlement::GetUserEntitlementOwnershipAny(
	TArray<FString> const ItemIds,
	TArray<FString> const AppIds,
	TArray<FString> const Skus,
	THandler<FAccelByteModelsEntitlementOwnership> const OnSuccess,
	FErrorHandler const& OnError)
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
		FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/entitlements/ownership/any"), *SettingsRef.PlatformServerUrl, *SettingsRef.PublisherNamespace);

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
		
		HttpClient.ApiRequest("GET", Url, {}, FString(), OnSuccess, OnError);
	}
}

void Entitlement::GetUserEntitlementOwnershipViaToken(const FString& PublicKey, const TArray<FString>& ItemIds, const TArray<FString>& AppIds, const TArray<FString>& Skus, const THandler<FAccelByteModelsEntitlementOwnershipDetails>& OnSuccess, const FErrorHandler& OnError, const bool VerifySignature, const bool VerifyExpiration, const FString& VerifySub)
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
		auto ProcessOnSuccess = THandler<FAccelByteModelsOwnershipToken>::CreateLambda([OnSuccess, OnError, PublicKey, VerifySignature, VerifyExpiration, VerifySub](const FAccelByteModelsOwnershipToken& Result)
		{
			TSharedPtr<FJsonObject> DecodedResult;
			FAccelByteJwtError Error;
			AccelByteJwtWrapper::TryDecode(Result.OwnershipToken, PublicKey, DecodedResult, Error, VerifySignature, VerifyExpiration, VerifySub);

			if(Error.Code == 0)
			{
				FAccelByteModelsEntitlementOwnershipDetails EntitlementDetails;
				bool isSuccess = FJsonObjectConverter::JsonObjectToUStruct<FAccelByteModelsEntitlementOwnershipDetails>(DecodedResult.ToSharedRef(), &EntitlementDetails);
				if(isSuccess)
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
		
		GetUserEntitlementOwnershipTokenOnly(ItemIds, AppIds, Skus, ProcessOnSuccess, OnError);
	}
}
	
void Entitlement::GetUserEntitlementOwnershipTokenOnly(const TArray<FString>& ItemIds, const TArray<FString>& AppIds, const TArray<FString>& Skus, const THandler<FAccelByteModelsOwnershipToken>& OnSuccess, const FErrorHandler& OnError)
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
		FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/entitlements/ownershipToken"), *SettingsRef.PlatformServerUrl, *SettingsRef.PublisherNamespace);

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
		
		HttpClient.ApiRequest("GET", Url, {}, FString(), OnSuccess, OnError);
	}
}

void Entitlement::ConsumeUserEntitlement(FString const& EntitlementId, int32 const& UseCount, THandler<FAccelByteModelsEntitlementInfo> const& OnSuccess, FErrorHandler const& OnError,
	TArray<FString> Options)
{
	FReport::Log(FString(__FUNCTION__));

	FAccelByteModelsConsumeUserEntitlementRequest ConsumeUserEntitlementRequest;
	ConsumeUserEntitlementRequest.UseCount = UseCount;
	ConsumeUserEntitlementRequest.Options = Options;

	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/%s/decrement"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *EntitlementId);

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(ConsumeUserEntitlementRequest, Content);

	HttpClient.ApiRequest("PUT", Url, {}, Content, OnSuccess, OnError);
}

void Entitlement::CreateDistributionReceiver(FString const& ExtUserId, FAccelByteModelsAttributes const Attributes, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "Platform Service version 3.4.0 and above doesn't support this anymore");

	OnError.ExecuteIfBound(410, TEXT("This feature is already removed."));
	return;
}

void Entitlement::DeleteDistributionReceiver(FString const& ExtUserId, FString const& UserId, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "Platform Service version 3.4.0 and above doesn't support this anymore");

	OnError.ExecuteIfBound(410, TEXT("This feature is already removed."));
	return;
}

void Entitlement::GetDistributionReceiver(FString const& PublisherNamespace, FString const& PublisherUserId, THandler<TArray<FAccelByteModelsDistributionReceiver>> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "Platform Service version 3.4.0 and above doesn't support this anymore");

	OnError.ExecuteIfBound(410, TEXT("This feature is already removed."));
	return;
}

void Entitlement::UpdateDistributionReceiver(FString const& ExtUserId, FAccelByteModelsAttributes const Attributes, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "Platform Service version 3.4.0 and above doesn't support this anymore");

	OnError.ExecuteIfBound(410, TEXT("This feature is already removed."));
	return;
}

void Entitlement::SyncPlatformPurchase(EAccelBytePlatformSync PlatformType, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FAccelByteModelsPlayStationDLCSync PSSyncModel;
	SyncPlatformPurchase(PlatformType, PSSyncModel, OnSuccess, OnError);
}

void Entitlement::SyncPlatformPurchase(EAccelBytePlatformSync PlatformType, FAccelByteModelsPlayStationDLCSync const& PSSyncModel, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString PlatformText = TEXT("");
	FString Content = TEXT("{}");
	FString platformUserId = CredentialsRef.GetPlatformUserId();

	switch (PlatformType)
	{
	case EAccelBytePlatformSync::STEAM:
		PlatformText = TEXT("steam");
		if (platformUserId.IsEmpty()) {
			OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("User not logged in with 3rd Party Platform"));
			return;
		}
		Content = FString::Printf(TEXT("{\"steamId\": \"%s\", \"appId\": %s}"), *CredentialsRef.GetPlatformUserId(), *SettingsRef.AppId);
		break;
	case EAccelBytePlatformSync::XBOX_LIVE:
		PlatformText = TEXT("xbl");
		break;
	case EAccelBytePlatformSync::PLAYSTATION:
		PlatformText = TEXT("psn");
		Content = FString::Printf(TEXT("{\"serviceLabel\": \"%d\"}"), PSSyncModel.ServiceLabel);
		break;
	case EAccelBytePlatformSync::EPIC_GAMES:
		PlatformText = TEXT("epicgames");
		if (platformUserId.IsEmpty())
		{
			OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("User not logged in with 3rd Party Platform"));
			return;
		}
		break;
	default:
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Platform Sync Type is not found"));
		return;
	}

	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/%s/sync"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *PlatformText);


	HttpClient.ApiRequest("PUT", Url, {}, Content, OnSuccess, OnError);
}

void Entitlement::SyncMobilePlatformPurchaseGoogle(FAccelByteModelsPlatformSyncMobileGoogle const& SyncRequest, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),TEXT("This method will be deprecated. Use SyncMobilePlatformPurchaseGooglePlay instead!"));

	FString PlatformText = TEXT("google");

	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/%s/receipt"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *PlatformText);
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(SyncRequest, Content);

	HttpClient.ApiRequest("PUT", Url, {}, Content, OnSuccess, OnError);
}

void Entitlement::SyncMobilePlatformPurchaseGooglePlay(FAccelByteModelsPlatformSyncMobileGoogle const& SyncRequest, THandler<FAccelByteModelsPlatformSyncMobileGoogleResponse> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString PlatformText = TEXT("google");
	
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/%s/receipt"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *PlatformText);
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(SyncRequest, Content);
	
	HttpClient.ApiRequest("PUT", Url, {}, Content, OnSuccess, OnError);
}

void Entitlement::SyncMobilePlatformPurchaseApple(FAccelByteModelsPlatformSyncMobileApple const& SyncRequest, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString PlatformText = TEXT("apple");
	
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/%s/receipt"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *PlatformText);
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(SyncRequest, Content);
	
	HttpClient.ApiRequest("PUT", Url, {}, Content, OnSuccess, OnError);
}

void Entitlement::SyncXBoxDLC(FAccelByteModelsXBoxDLCSync const& XboxDLCSync, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Content = FString::Printf(TEXT("{\"xstsToken\": \"%s\"}"), *XboxDLCSync.XstsToken);
	if (XboxDLCSync.XstsToken.IsEmpty())
	{
		Content = FString::Printf(TEXT("{}"));
	}
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/dlc/xbl/sync"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());

	HttpClient.ApiRequest("PUT", Url, {}, Content, OnSuccess, OnError); 
}

void Entitlement::SyncSteamDLC(FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString platformUserId = CredentialsRef.GetPlatformUserId();

	if (platformUserId.IsEmpty()) 
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::IsNotLoggedIn), TEXT("User not logged in with 3rd Party Platform"));
		return;
	}

	FString Content = TEXT("");
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/dlc/steam/sync"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());

	FJsonObject DataJson;
	DataJson.SetStringField("steamId", platformUserId);
	DataJson.SetStringField("appId", SettingsRef.AppId);
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(DataJson);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	
	HttpClient.ApiRequest("PUT", Url, {}, Content, OnSuccess, OnError);
}

void Entitlement::SyncPSNDLC(FAccelByteModelsPlayStationDLCSync const& PSSyncModel, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Content = FString::Printf(TEXT("{\"serviceLabel\": \"%d\"}"), PSSyncModel.ServiceLabel);

	//	URL
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/dlc/psn/sync"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());

	HttpClient.ApiRequest("PUT", Url, {}, Content, OnSuccess, OnError); 
}

void Entitlement::SyncTwitchDropEntitlement(FAccelByteModelsTwitchDropEntitlement const& TwitchDropModel,FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	// Url 
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/iap/twitch/sync"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());	 
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
	HttpClient.ApiRequest("PUT", Url, {}, Content, OnSuccess, OnError); 
}

void Entitlement::SyncEpicGameDurableItems(FString const& EpicGamesJwtToken, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	// Url 
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/dlc/epicgames/sync"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());	 
	// Content Body 
	FString Content = TEXT("");
	FJsonObject DataJson;
	DataJson.SetStringField("epicGamesJwtToken", EpicGamesJwtToken); 
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(DataJson);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
 
	// Api Request 
	HttpClient.ApiRequest("PUT", Url, {}, Content, OnSuccess, OnError); 
}

void Entitlement::ValidateUserItemPurchaseCondition(TArray<FString> const& Items, THandler<TArray<FAccelByteModelsPlatformValidateUserItemPurchaseResponse>> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	// Url 
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/items/purchase/conditions/validate"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace());

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
	HttpClient.ApiRequest("POST", Url, {}, Content, OnSuccess, OnError); 
}

void Entitlement::GetUserEntitlementOwnershipByItemIds(TArray<FString> const& Ids, THandler<TArray<FAccelByteModelsEntitlementOwnershipItemIds>> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	// Url 
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/entitlements/ownership/byItemIds"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());

	// Params 
	FString IdsQueryParamString = TEXT("");

	for (FString const& Id : Ids)
	{
		if (!Id.IsEmpty())
		{
			IdsQueryParamString.Append(IdsQueryParamString.IsEmpty() ? TEXT("?") : TEXT("&"));
			IdsQueryParamString.Append(FString::Printf(TEXT("ids=%s"), *Id));
		}
	} 

	// Here we use append string to Url; we couldn't use TMap for ids, since the key should be unique 

	Url.Append(IdsQueryParamString); 

	// Content 
	FString Content = TEXT("");
	
	// Api Request 
	HttpClient.ApiRequest("GET", Url, {}, Content, OnSuccess, OnError); 
}

} // Namespace Api
}