// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"

struct EcommerceExpectedVariable
{
	FString ExpectedRootCategoryPath;
	FString ExpectedChildCategoryPath;
	FString ExpectedGrandChildCategoryPath;
	FCurrencyCreateRequest ExpectedCurrency;
	FString campaignCode;
	FString campaignName;
	FString expiredCampaignName;
	FString notStartedCampaignName;
	FString redeemableItemTitle;
	FString ExpectedRootItemTitle;
	FString ExpectedChildItemTitle;
	FString ExpectedGrandChildItemTitle;
	FString ExpectedPurchasingItemTitle;
	FStoreCreateRequest ExpectedStoreArchive;
	FStoreCreateRequest ExpectedStoreTemporary;
	int32 LootCoinQuantity;
	FItemFullInfo redeemableItem;
	FItemFullInfo LootItem;
	FItemFullInfo LootCoin;
	FItemFullInfo LootResult;
	FItemFullInfo BundleResult;
	FCampaignInfo campaignResult;
	FCampaignInfo expiredCampaignResult;
	FCampaignInfo notStartedCampaignResult;
	FCodeInfo codeInfo;
	FCodeInfo expiredCodeInfo;
	FCodeInfo notStartedCodeInfo;
};

void SetupEcommerceCurrency(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void SetupEcommerceStore(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void SetupEcommerceCampaign(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void TearDownEcommerce(EcommerceExpectedVariable& Variables, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceCurrency(const FCurrencyCreateRequest& Currency, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceCurrency(const FString& CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteEcommerceCurrency(const FString& CurrencyCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void AdminGetEcommercePublishedStore(const FString& Namespace, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommercePublishedStore(const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteEcommercePublishedStore(const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceStore(const FStoreCreateRequest& Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateEcommerceStore(const FString& Namespace, const FStoreCreateRequest& Store, const THandler<FStoreInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceStoreAll(const FString& Namespace, const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceStoreAll(const THandler<TArray<FStoreInfo>>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteEcommerceStore(const FString& StoreId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminCloneEcommerceStore(const FString& Source, const FString& Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminCloneEcommerceStore(const FString& Namespace, const FString& Source, const FString& Target, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceCategory(const FString& StoreId, const FCategoryCreateRequest& Category, const THandler<FCategoryInfo>& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceItem(const FString& StoreId, const FItemCreateRequest& Item, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateEcommerceItem(const FString& Namespace, const FString& StoreId, const FItemCreateRequest& Item, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceItemBySKU(const FString& Namespace, const FString& StoreId, const FString& SKU, bool ActiveOnly, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGrantEcommerceUserEntitlements(const FString& Namespace, const FString& UserId, const TArray<FAccelByteModelsEntitlementGrant>& EntitlementGrant, const THandler<TArray<FAccelByteModelsStackableEntitlementInfo>>& OnSuccess, const FErrorHandler& OnError);

void AdminCreateEcommerceCampaign(const FCampaignCreateModel& Body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminUpdateEcommerceCampaign(const FString& CampaignId, const FCampaignUpdateModel& Body, const THandler<FCampaignInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceCampaignByName(const FString& Name, const THandler<FCampaignPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateEcommerceCampaignCodes(const FString& CampaignId, const FCampaignCodeCreateModel& Body, const THandler<FCampaignCodeCreateResult>& OnSuccess, const FErrorHandler& OnError);
void AdminGetEcommerceCampaignCodes(const FString& CampaignId, const THandler<FCodeInfoPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError);
void AdminDisableEcommerceCampaignCode(const FString& CampaignCode, const THandler<FCodeInfo>& OnSuccess, const FErrorHandler& OnError);