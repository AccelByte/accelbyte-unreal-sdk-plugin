// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "AccelByteModelsError.h"

#include <unordered_map>

namespace AccelByte
{

DECLARE_DELEGATE_TwoParams(ErrorDelegate, int32, FString);

enum class ErrorCodes : int32
{
	ErrorCodeNotSet = 0,
	UnknownError = 14000,
	JsonDeserializationFailed = 14001,
	HttpOther = 8000,
	HttpBadRequest = 8400,
	HttpUnauthorized = 8401,
	HttpPaymentRequired = 8402,
	HttpForbidden = 8403,
	HttpNotFound = 8404,
	HttpMethodNotAllowed = 8405,
	HttpNotAcceptable = 8406,
	HttpProxyAuthenticationRequired = 8407,
	HttpRequestTimeout = 8408,
	HttpConflict = 8409,
	HttpGone = 8410,
};

class ErrorMessages
{
public:
	/**
	 * @brief Contains dictionary for default error messages (English).
	 */
	const static std::unordered_map<int32, FString> Default;
};

inline void HandleHttpError(FHttpRequestPtr Request, FHttpResponsePtr Response, int& OutCode, FString& OutMessage)
{

	int32 Code = OutCode + 8000;
	if (Response->GetResponseCode() < 1000)
	{
		Code = static_cast<int32>(ErrorCodes::HttpOther) + Response->GetResponseCode();
	}
	else
	{
		Code = static_cast<int32>(ErrorCodes::HttpOther);
	}
	OutCode = Code;
	// if the ErrorCode exist in the map, use that error string
	auto it = ErrorMessages::Default.find(Code);
	if (it != ErrorMessages::Default.cend())
	{
		OutMessage = it->second;
		return;
	}
	OutMessage = ErrorMessages::Default.at(static_cast<int32>(ErrorCodes::HttpOther));
}

} // Namespace AccelByte


// Error Code List
//2000 - InternalServerErrorException errors.net.accelbyte.platform.internal_server_error
//2001 - UnauthorizedException errors.net.accelbyte.platform.unauthorized
//2002 - ValidationException errors.net.accelbyte.platform.validation_error
//2003 - ForbiddenException errors.net.accelbyte.platform.forbidden
//2004 - ServiceUnavailableException errors.net.accelbyte.platform.service_unavailable
//2005 - UnsupportedMediaTypeException errors.net.accelbyte.platform.unsupported_media_type
//2006 - OptimisticLockException errors.net.accelbyte.platform.optimistic_lock
//
//Namespace Error Code List
//
//2141 - NamespaceNotFoundException errors.net.accelbyte.platform.namespace.namespace_not_found
//2171 - NamespaceAlreadyExistsException errors.net.accelbyte.platform.namespace.namespace_already_exists
//
//Profile Error Code List
//
//2241 - UserProfileNotFoundException errors.net.accelbyte.platform.profile.user_profile_not_found
//
//2271 - UserProfileAlreadyExistsException errors.net.accelbyte.platform.profile.user_profile_already_exists
//
//FileStorage Error Code List
//
//2321 - FileTypeNotSupportedException errors.net.accelbyte.platform.filestorage.file_type_not_supported
//
//2371 - UserFileStorageExceedLimitException errors.net.accelbyte.platform.filestorage.user_file_storage_exceed_limit
//
//Buildinfo Error Code List
//
//2303 - EntitlementRequiredException errors.net.accelbyte.platform.buildinfo.entitlement_required
//
//2304 - EntitlementRequiredException errors.net.accelbyte.platform.buildinfo.conditional_write_failed
//
//2321 - BlockEmptyException errors.net.accelbyte.platform.buildinfo.block_empty
//2322 - BlockUuidRequiredException errors.net.accelbyte.platform.buildinfo.block_uuid_required
//2323 - InvalidPlatformIdException errors.net.accelbyte.platform.buildinfo.platform_id_not_found
//2324 - MaximumDRMActivationLimitExceeded errors.net.accelbyte.platform.buildinfo.max_drm_activation_limit_exceeded
//
//2325 - BlockHashMismatchException errors.net.accelbyte.platform.buildinfo.block_hash_mismatch
//
//2341 - BuildNotFoundException errors.net.accelbyte.platform.buildinfo.build_not_found
//2342 - BlockNotFoundException errors.net.accelbyte.platform.buildinfo.block_not_found
//2343 - BaseBuildNotFoundException errors.net.accelbyte.platform.buildinfo.base_build_not_found
//
//2344 - ReleaseBuildNotFoundException errors.net.accelbyte.platform.buildinfo.release_build_not_found
//
//2371 - BuildAlreadyCommittedException errors.net.accelbyte.platform.buildinfo.build_already_committed
//2372 - BuildIncompleteException errors.net.accelbyte.platform.buildinfo.build_incomplete
//2373 - UploadAlreadyCompleteException errors.net.accelbyte.platform.buildinfo.upload_already_complete
//2374 - UploadAlreadyStartedException errors.net.accelbyte.platform.buildinfo.upload_already_started
//2375 - DuplicateBuildIdException errors.net.accelbyte.platform.buildinfo.duplicate_build_id
//
//Category Error Code List
//
//3021 - DefaultLanguageRequiredException errors.net.accelbyte.platform.catalog.default_language_required
//3022 - InvalidDiscountValueException errors.net.accelbyte.platform.catalog.invalid_discount_amount
//3023 - ItemBundleException errors.net.accelbyte.platform.catalog.item_bundle
//3024 - InvalidStoreDataException errors.net.accelbyte.platform.catalog.invalid_store_data
//3025 - StoreMetaMismatchException errors.net.accelbyte.platform.catalog.store_meta_mismatch
//
//3026 - DefaultRegionRequiredException errors.net.accelbyte.platform.catalog.default_region_required
//
//3041 - CategoryNotFoundException errors.net.accelbyte.platform.catalog.category_not_found
//3042 - ItemNotFoundException errors.net.accelbyte.platform.catalog.item_not_found
//3043 - StoreNotFoundException errors.net.accelbyte.platform.catalog.store_not_found
//
//3044 - PublishedStoreNotFoundException errors.net.accelbyte.platform.catalog.published_store_not_found
//
//3071 - CategoryAlreadyExistsException errors.net.accelbyte.platform.catalog.category_already_exists
//3072 - CategoryNotEmptyException errors.net.accelbyte.platform.catalog.category_not_empty
//3073 - ItemAlreadyBundledException errors.net.accelbyte.platform.catalog.item_already_bundled
//3074 - LanguageNotSupportedException errors.net.accelbyte.platform.catalog.store_language_not_supported
//3075 - RegionNotSupportedException errors.net.accelbyte.platform.catalog.store_region_not_supported
//3076 - PublishedStoreNotUpdatableException errors.net.accelbyte.platform.catalog.published_store_not_updatable
//
//Entitlement Error Code List
//
//3121 - EntitlementNotDistributableException errors.net.accelbyte.platform.entitlement.entitlement_not_distributable
//3122 - ExceedDistributeQuantityException errors.net.accelbyte.platform.entitlement.exceed_distribute_quantity
//
//3123 - PublisherNamespaceNotDistributableException errors.net.accelbyte.platform.entitlement.publisher_namespace_not_distributable
//
//3141 - UserEntitlementNotFoundException errors.net.accelbyte.platform.entitlement.user_entitlement_not_found
//3142 - EntitlementNotFoundException errors.net.accelbyte.platform.entitlement.entitlement_not_found
//3143 - UserAppEntitlementNotFoundException errors.net.accelbyte.platform.entitlement.user_app_entitlement_not_found
//
//3144 - DistributionReceiverNotFoundException errors.net.accelbyte.platform.entitlement.distribution_receiver_not_found
//
//3171 - EntitlementAlreadyRevokedException errors.net.accelbyte.platform.entitlement.entitlement_already_revoked
//3172 - EntitlementNotActiveException errors.net.accelbyte.platform.entitlement.entitlement_not_active
//3173 - EntitlementNotConsumableException errors.net.accelbyte.platform.entitlement.entitlement_not_consumable
//3174 - EntitlementUsedUpException errors.net.accelbyte.platform.entitlement.entitlement_used_up
//3175 - EntitlementAlreadyDistributedException errors.net.accelbyte.platform.entitlement.entitlement_already_distributed
//3176 - DistributionReceiverAlreadyExistsException errors.net.accelbyte.platform.entitlement.distribution_receiver_already_exists
//
//Order Error Code List
//
//3221 - OrderPriceMismatchException errors.net.accelbyte.platform.order.order_price_mismatch
//
//3241 - OrderNotFoundException errors.net.accelbyte.platform.order.order_not_found
//
//3271 - OrderNotRefundableException errors.net.accelbyte.platform.order.order_not_refundable
//3272 - InvalidOrderStatusException errors.net.accelbyte.platform.order.invalid_order_status
//3273 - OrderReceiptNotDownloadableException errors.net.accelbyte.platform.order.order_receipt_not_downloadable
//3274 - OrderTransactionAmountMismatchException errors.net.accelbyte.platform.order.order_transaction_amount_mismatch
//3275 - ExceedItemMaxCountPerUserException errors.net.accelbyte.platform.order.exceed_item_max_count_per_user
//
//Payment Error Code List
//
//3321 - PaymentCurrencyNotSupportedException errors.net.accelbyte.platform.payment.currency_not_supported
//3322 - PaymentProviderNotSupportedException errors.net.accelbyte.platform.payment.provider_not_supported
//3323 - AliPayRefundFailedException errors.net.accelbyte.platform.payment.alipay_refund_failed
//
//3324 - WxPayRefundFailedException errors.net.accelbyte.platform.payment.wxpay_refund_failed
//
//3341 - PaymentOrderNotFoundException errors.net.accelbyte.platform.payment.payment_order_not_found
//
//3342 - PaymentOrderChargeTransactionNotFoundException errors.net.accelbyte.platform.payment.payment_order_charge_transaction_not_found
//
//3371 - InvalidPaymentOrderStatusException errors.net.accelbyte.platform.payment.invalid_payment_order_status
//3372 - InvalidPaymentOrderTransactionStatusException errors.net.accelbyte.platform.payment.invalid_payment_order_transaction_status
//3373 - InvalidPaymentOrderTransactionTypeException errors.net.accelbyte.platform.payment.invalid_payment_order_transaction_type
//3374 - PaymentOrderNotRefundableException errors.net.accelbyte.platform.payment.payment_order_not_refundable
//3375 - PaymentOrderTransactionAmountMismatchException errors.net.accelbyte.platform.payment.payment_order_transaction_amount_mismatch
//3376 - WxPayUnifiedOrderFailedException errors.net.accelbyte.platform.payment.wxpay_unified_order_failed
//3377 - PaymentExtOrderNoAlreadyExistsException errors.net.accelbyte.platform.payment.ext_order_no_already_exists

//Wallet Error Code List
//
//3521 - ExceedMaxTransactionAmountPerDayException errors.net.accelbyte.platform.wallet.exceed_max_transaction_amount_per_day
//3522 - ExceedMaxAmountPerTransactionException errors.net.accelbyte.platform.wallet.exceed_max_amount_per_transaction
//3523 - InactiveWalletException errors.net.accelbyte.platform.wallet.inactive_wallet
//3524 - InsufficientBalanceException errors.net.accelbyte.platform.wallet.insufficient_balance
//
//3525 - ExceedMaxBalanceAmountException errors.net.accelbyte.platform.wallet.exceed_max_balance_amount
//
//3541 - WalletNotFoundException errors.net.accelbyte.platform.wallet.wallet_not_found
//
//Currency Error Code List
//
//3641 - CurrencyNotFoundException errors.net.accelbyte.platform.currency.currency_not_found
//
//3671 - CurrencyAlreadyExistsException errors.net.accelbyte.platform.currency.currency_already_exists
//
//Campaign Error Code List
//
//3741 - CampaignNotFoundException errors.net.accelbyte.platform.campaign.campaign_not_found
//3742 - CodeNotFoundException errors.net.accelbyte.platform.campaign.code_not_found
//3771 - CampaignAlreadyExistsException errors.net.accelbyte.platform.campaign.campaign_already_exists
//3772 - CampaignIsInactiveException errors.net.accelbyte.platform.campaign.campaign_is_inactive
//3773 - CodeIsInactiveException errors.net.accelbyte.platform.campaign.code_is_inactive
//3774 - ExceedMaxRedeemCountPerCodeException errors.net.accelbyte.platform.campaign.exceed_max_redeem_count_per_code
//3775 - ExceedMaxRedeemCountPerUserException errors.net.accelbyte.platform.campaign.exceed_max_redeem_count_per_user
//3776 - CodeHasBeenRedeemedException errors.net.accelbyte.platform.campaign.code_has_been_redeemed
//
//Social Error Code List
//Activity
//
//4041 - ActivityTrackNotFoundException errors.net.accelbyte.platform.activity.activity_track_not_found
//
//Favorite
//
//4141 - FavoriteItemNotFoundException errors.net.accelbyte.platform.favorite.favorite_item_not_found
//4142 - FavoriteUserNotFoundException errors.net.accelbyte.platform.favorite.favorite_user_not_found
//
//Offense report
//
//4241 - OffenseReportNotFoundException errors.net.accelbyte.platform.offensereport.offense_report_not_found
//4242 - OffenseTypeNotFoundException errors.net.accelbyte.platform.offensereport.offense_type_not_found
//4271 - OffenseTypeAlreadyExistsException errors.net.accelbyte.platform.offensereport.offense_type_already_exists
//4272 - OffenseReportAssociatedException errors.net.accelbyte.platform.offensereport.offense_report_associated
