// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteError.h"

namespace AccelByte
{

const std::unordered_map<int32, FString> ErrorMessages::Default
{
	{ static_cast<int32>(ErrorCodes::UnauthorizedException), TEXT("errors.net.accelbyte.platform.unauthorized") },
	{ static_cast<int32>(ErrorCodes::ValidationException), TEXT("errors.net.accelbyte.platform.validation_error") },
	{ static_cast<int32>(ErrorCodes::ForbiddenException), TEXT("errors.net.accelbyte.platform.forbidden") },
	{ static_cast<int32>(ErrorCodes::ServiceUnavailableException), TEXT("errors.net.accelbyte.platform.service_unavailable") },
	{ static_cast<int32>(ErrorCodes::UnsupportedMediaTypeException), TEXT("errors.net.accelbyte.platform.unsupported_media_type") },
	{ static_cast<int32>(ErrorCodes::OptimisticLockException), TEXT("errors.net.accelbyte.platform.optimistic_lock") },
	// Namespace Error Code List
	{ static_cast<int32>(ErrorCodes::NamespaceNotFoundException), TEXT("errors.net.accelbyte.platform.namespace.namespace_not_found") },
	{ static_cast<int32>(ErrorCodes::NamespaceAlreadyExistsException), TEXT("errors.net.accelbyte.platform.namespace.namespace_already_exists") },
	// Profile Error Code List
	{ static_cast<int32>(ErrorCodes::UserProfileNotFoundException), TEXT("errors.net.accelbyte.platform.profile.user_profile_not_found") },
	{ static_cast<int32>(ErrorCodes::UserProfileAlreadyExistsException), TEXT("errors.net.accelbyte.platform.profile.user_profile_already_exists") },
	// FileStorage Error Code List
	{ static_cast<int32>(ErrorCodes::FileTypeNotSupportedException), TEXT("errors.net.accelbyte.platform.filestorage.file_type_not_supported") },
	{ static_cast<int32>(ErrorCodes::UserFileStorageExceedLimitException), TEXT("errors.net.accelbyte.platform.filestorage.user_file_storage_exceed_limit") },
	// Buildinfo Error Code List
	{ static_cast<int32>(ErrorCodes::EntitlementRequiredException), TEXT("errors.net.accelbyte.platform.buildinfo.entitlement_required") },
	{ static_cast<int32>(ErrorCodes::EntitlementRequiredException2), TEXT("errors.net.accelbyte.platform.buildinfo.conditional_write_failed") },
	{ static_cast<int32>(ErrorCodes::BlockEmptyException), TEXT("errors.net.accelbyte.platform.buildinfo.block_empty") },
	{ static_cast<int32>(ErrorCodes::BlockUuidRequiredException), TEXT("errors.net.accelbyte.platform.buildinfo.block_uuid_required") },
	{ static_cast<int32>(ErrorCodes::InvalidPlatformIdException), TEXT("errors.net.accelbyte.platform.buildinfo.platform_id_not_found") },
	{ static_cast<int32>(ErrorCodes::MaximumDRMActivationLimitExceeded), TEXT("errors.net.accelbyte.platform.buildinfo.max_drm_activation_limit_exceeded") },
	//
	{ static_cast<int32>(ErrorCodes::BlockHashMismatchException), TEXT("errors.net.accelbyte.platform.buildinfo.block_hash_mismatch") },
	//
	{ static_cast<int32>(ErrorCodes::BuildNotFoundException), TEXT("errors.net.accelbyte.platform.buildinfo.build_not_found") },
	{ static_cast<int32>(ErrorCodes::BlockNotFoundException), TEXT("errors.net.accelbyte.platform.buildinfo.block_not_found") },
	{ static_cast<int32>(ErrorCodes::BaseBuildNotFoundException), TEXT("errors.net.accelbyte.platform.buildinfo.base_build_not_found") },
	//
	{ static_cast<int32>(ErrorCodes::ReleaseBuildNotFoundException), TEXT("errors.net.accelbyte.platform.buildinfo.release_build_not_found") },
	//
	{ static_cast<int32>(ErrorCodes::BuildAlreadyCommittedException), TEXT("errors.net.accelbyte.platform.buildinfo.build_already_committed") },
	{ static_cast<int32>(ErrorCodes::BuildIncompleteException), TEXT("errors.net.accelbyte.platform.buildinfo.build_incomplete") },
	{ static_cast<int32>(ErrorCodes::UploadAlreadyCompleteException), TEXT("errors.net.accelbyte.platform.buildinfo.upload_already_complete") },
	{ static_cast<int32>(ErrorCodes::UploadAlreadyStartedException), TEXT("errors.net.accelbyte.platform.buildinfo.upload_already_started") },
	{ static_cast<int32>(ErrorCodes::DuplicateBuildIdException), TEXT("errors.net.accelbyte.platform.buildinfo.duplicate_build_id") },
	//
	//Category Error Code List
	//
	{ static_cast<int32>(ErrorCodes::DefaultLanguageRequiredException), TEXT("errors.net.accelbyte.platform.catalog.default_language_required") },
	{ static_cast<int32>(ErrorCodes::InvalidDiscountValueException), TEXT("errors.net.accelbyte.platform.catalog.invalid_discount_amount") },
	{ static_cast<int32>(ErrorCodes::ItemBundleException), TEXT("errors.net.accelbyte.platform.catalog.item_bundle") },
	{ static_cast<int32>(ErrorCodes::InvalidStoreDataException), TEXT("errors.net.accelbyte.platform.catalog.invalid_store_data") },
	{ static_cast<int32>(ErrorCodes::StoreMetaMismatchException), TEXT("errors.net.accelbyte.platform.catalog.store_meta_mismatch") },
	//
	{ static_cast<int32>(ErrorCodes::DefaultRegionRequiredException), TEXT("errors.net.accelbyte.platform.catalog.default_region_required") },
	//
	{ static_cast<int32>(ErrorCodes::CategoryNotFoundException), TEXT("errors.net.accelbyte.platform.catalog.category_not_found") },
	{ static_cast<int32>(ErrorCodes::ItemNotFoundException), TEXT("errors.net.accelbyte.platform.catalog.item_not_found") },
	{ static_cast<int32>(ErrorCodes::StoreNotFoundException), TEXT("errors.net.accelbyte.platform.catalog.store_not_found") },
	//
	{ static_cast<int32>(ErrorCodes::PublishedStoreNotFoundException), TEXT("errors.net.accelbyte.platform.catalog.published_store_not_found") },
	//
	{ static_cast<int32>(ErrorCodes::CategoryAlreadyExistsException), TEXT("errors.net.accelbyte.platform.catalog.category_already_exists") },
	{ static_cast<int32>(ErrorCodes::CategoryNotEmptyException), TEXT("errors.net.accelbyte.platform.catalog.category_not_empty") },
	{ static_cast<int32>(ErrorCodes::ItemAlreadyBundledException), TEXT("errors.net.accelbyte.platform.catalog.item_already_bundled") },
	{ static_cast<int32>(ErrorCodes::LanguageNotSupportedException), TEXT("errors.net.accelbyte.platform.catalog.store_language_not_supported") },
	{ static_cast<int32>(ErrorCodes::RegionNotSupportedException), TEXT("errors.net.accelbyte.platform.catalog.store_region_not_supported") },
	{ static_cast<int32>(ErrorCodes::PublishedStoreNotUpdatableException), TEXT("errors.net.accelbyte.platform.catalog.published_store_not_updatable") },
	//
	//Entitlement Error Code List
	//
	{ static_cast<int32>(ErrorCodes::EntitlementNotDistributableException), TEXT("errors.net.accelbyte.platform.entitlement.entitlement_not_distributable") },
	{ static_cast<int32>(ErrorCodes::ExceedDistributeQuantityException), TEXT("errors.net.accelbyte.platform.entitlement.exceed_distribute_quantity") },
	//
	{ static_cast<int32>(ErrorCodes::PublisherNamespaceNotDistributableException), TEXT("errors.net.accelbyte.platform.entitlement.publisher_namespace_not_distributable") },
	//
	{ static_cast<int32>(ErrorCodes::UserEntitlementNotFoundException), TEXT("errors.net.accelbyte.platform.entitlement.user_entitlement_not_found") },
	{ static_cast<int32>(ErrorCodes::EntitlementNotFoundException), TEXT("errors.net.accelbyte.platform.entitlement.entitlement_not_found") },
	{ static_cast<int32>(ErrorCodes::UserAppEntitlementNotFoundException), TEXT("errors.net.accelbyte.platform.entitlement.user_app_entitlement_not_found") },
	//
	{ static_cast<int32>(ErrorCodes::DistributionReceiverNotFoundException), TEXT("errors.net.accelbyte.platform.entitlement.distribution_receiver_not_found") },
	//
	{ static_cast<int32>(ErrorCodes::EntitlementAlreadyRevokedException), TEXT("errors.net.accelbyte.platform.entitlement.entitlement_already_revoked") },
	{ static_cast<int32>(ErrorCodes::EntitlementNotActiveException), TEXT("errors.net.accelbyte.platform.entitlement.entitlement_not_active") },
	{ static_cast<int32>(ErrorCodes::EntitlementNotConsumableException), TEXT("errors.net.accelbyte.platform.entitlement.entitlement_not_consumable") },
	{ static_cast<int32>(ErrorCodes::EntitlementUsedUpException), TEXT("errors.net.accelbyte.platform.entitlement.entitlement_used_up") },
	{ static_cast<int32>(ErrorCodes::EntitlementAlreadyDistributedException), TEXT("errors.net.accelbyte.platform.entitlement.entitlement_already_distributed") },
	{ static_cast<int32>(ErrorCodes::DistributionReceiverAlreadyExistsException), TEXT("errors.net.accelbyte.platform.entitlement.distribution_receiver_already_exists") },
	//
	//Order Error Code List
	//
	{ static_cast<int32>(ErrorCodes::OrderPriceMismatchException), TEXT("errors.net.accelbyte.platform.order.order_price_mismatch") },
	//
	{ static_cast<int32>(ErrorCodes::OrderNotFoundException), TEXT("errors.net.accelbyte.platform.order.order_not_found") },
	//
	{ static_cast<int32>(ErrorCodes::OrderNotRefundableException), TEXT("errors.net.accelbyte.platform.order.order_not_refundable") },
	{ static_cast<int32>(ErrorCodes::InvalidOrderStatusException), TEXT("errors.net.accelbyte.platform.order.invalid_order_status") },
	{ static_cast<int32>(ErrorCodes::OrderReceiptNotDownloadableException), TEXT("errors.net.accelbyte.platform.order.order_receipt_not_downloadable") },
	{ static_cast<int32>(ErrorCodes::OrderTransactionAmountMismatchException), TEXT("errors.net.accelbyte.platform.order.order_transaction_amount_mismatch") },
	{ static_cast<int32>(ErrorCodes::ExceedItemMaxCountPerUserException), TEXT("errors.net.accelbyte.platform.order.exceed_item_max_count_per_user") },
	//
	//Payment Error Code List
	//
	{ static_cast<int32>(ErrorCodes::PaymentCurrencyNotSupportedException), TEXT("errors.net.accelbyte.platform.payment.currency_not_supported") },
	{ static_cast<int32>(ErrorCodes::PaymentProviderNotSupportedException), TEXT("errors.net.accelbyte.platform.payment.provider_not_supported") },
	{ static_cast<int32>(ErrorCodes::AliPayRefundFailedException), TEXT("errors.net.accelbyte.platform.payment.alipay_refund_failed") },
	//
	{ static_cast<int32>(ErrorCodes::WxPayRefundFailedException), TEXT("errors.net.accelbyte.platform.payment.wxpay_refund_failed") },
	//
	{ static_cast<int32>(ErrorCodes::PaymentOrderNotFoundException), TEXT("errors.net.accelbyte.platform.payment.payment_order_not_found") },
	//
	{ static_cast<int32>(ErrorCodes::PaymentOrderChargeTransactionNotFoundException), TEXT("errors.net.accelbyte.platform.payment.payment_order_charge_transaction_not_found") },
	//
	{ static_cast<int32>(ErrorCodes::InvalidPaymentOrderStatusException), TEXT("errors.net.accelbyte.platform.payment.invalid_payment_order_status") },
	{ static_cast<int32>(ErrorCodes::InvalidPaymentOrderTransactionStatusException), TEXT("errors.net.accelbyte.platform.payment.invalid_payment_order_transaction_status") },
	{ static_cast<int32>(ErrorCodes::InvalidPaymentOrderTransactionTypeException), TEXT("errors.net.accelbyte.platform.payment.invalid_payment_order_transaction_type") },
	{ static_cast<int32>(ErrorCodes::PaymentOrderNotRefundableException), TEXT("errors.net.accelbyte.platform.payment.payment_order_not_refundable") },
	{ static_cast<int32>(ErrorCodes::PaymentOrderTransactionAmountMismatchException), TEXT("errors.net.accelbyte.platform.payment.payment_order_transaction_amount_mismatch") },
	{ static_cast<int32>(ErrorCodes::WxPayUnifiedOrderFailedException), TEXT("errors.net.accelbyte.platform.payment.wxpay_unified_order_failed") },
	{ static_cast<int32>(ErrorCodes::PaymentExtOrderNoAlreadyExistsException), TEXT("errors.net.accelbyte.platform.payment.ext_order_no_already_exists") },

	//Wallet Error Code List
	//
	{ static_cast<int32>(ErrorCodes::ExceedMaxTransactionAmountPerDayException), TEXT("errors.net.accelbyte.platform.wallet.exceed_max_transaction_amount_per_day") },
	{ static_cast<int32>(ErrorCodes::ExceedMaxAmountPerTransactionException), TEXT("errors.net.accelbyte.platform.wallet.exceed_max_amount_per_transaction") },
	{ static_cast<int32>(ErrorCodes::InactiveWalletException), TEXT("errors.net.accelbyte.platform.wallet.inactive_wallet") },
	{ static_cast<int32>(ErrorCodes::InsufficientBalanceException), TEXT("errors.net.accelbyte.platform.wallet.insufficient_balance") },
	//
	{ static_cast<int32>(ErrorCodes::ExceedMaxBalanceAmountException), TEXT("errors.net.accelbyte.platform.wallet.exceed_max_balance_amount") },
	//
	{ static_cast<int32>(ErrorCodes::WalletNotFoundException), TEXT("errors.net.accelbyte.platform.wallet.wallet_not_found") },
	//
	//Currency Error Code List
	//
	{ static_cast<int32>(ErrorCodes::CurrencyNotFoundException), TEXT("errors.net.accelbyte.platform.currency.currency_not_found") },
	//
	{ static_cast<int32>(ErrorCodes::CurrencyAlreadyExistsException), TEXT("errors.net.accelbyte.platform.currency.currency_already_exists") },
	//
	//Campaign Error Code List
	//
	{ static_cast<int32>(ErrorCodes::CampaignNotFoundException), TEXT("errors.net.accelbyte.platform.campaign.campaign_not_found") },
	{ static_cast<int32>(ErrorCodes::CodeNotFoundException), TEXT("errors.net.accelbyte.platform.campaign.code_not_found") },
	{ static_cast<int32>(ErrorCodes::CampaignAlreadyExistsException), TEXT("errors.net.accelbyte.platform.campaign.campaign_already_exists") },
	{ static_cast<int32>(ErrorCodes::CampaignIsInactiveException), TEXT("errors.net.accelbyte.platform.campaign.campaign_is_inactive") },
	{ static_cast<int32>(ErrorCodes::CodeIsInactiveException), TEXT("errors.net.accelbyte.platform.campaign.code_is_inactive") },
	{ static_cast<int32>(ErrorCodes::ExceedMaxRedeemCountPerCodeException), TEXT("errors.net.accelbyte.platform.campaign.exceed_max_redeem_count_per_code") },
	{ static_cast<int32>(ErrorCodes::ExceedMaxRedeemCountPerUserException), TEXT("errors.net.accelbyte.platform.campaign.exceed_max_redeem_count_per_user") },
	{ static_cast<int32>(ErrorCodes::CodeHasBeenRedeemedException), TEXT("errors.net.accelbyte.platform.campaign.code_has_been_redeemed") },
	//
	//Social Error Code List
	//Activity
	//
	{ static_cast<int32>(ErrorCodes::ActivityTrackNotFoundException), TEXT("errors.net.accelbyte.platform.activity.activity_track_not_found") },
	//
	//Favorite
	//
	{ static_cast<int32>(ErrorCodes::FavoriteItemNotFoundException), TEXT("errors.net.accelbyte.platform.favorite.favorite_item_not_found") },
	{ static_cast<int32>(ErrorCodes::FavoriteUserNotFoundException), TEXT("errors.net.accelbyte.platform.favorite.favorite_user_not_found") },
	//
	//Offense report
	//
	{ static_cast<int32>(ErrorCodes::OffenseReportNotFoundException), TEXT("errors.net.accelbyte.platform.offensereport.offense_report_not_found") },
	{ static_cast<int32>(ErrorCodes::OffenseTypeNotFoundException), TEXT("errors.net.accelbyte.platform.offensereport.offense_type_not_found") },
	{ static_cast<int32>(ErrorCodes::OffenseTypeAlreadyExistsException), TEXT("errors.net.accelbyte.platform.offensereport.offense_type_already_exists") },
	{ static_cast<int32>(ErrorCodes::OffenseReportAssociatedException), TEXT("errors.net.accelbyte.platform.offensereport.offense_report_associated") },
		
	{ static_cast<int32>(ErrorCodes::UnknownError), TEXT("Unknown error.") },
	{ static_cast<int32>(ErrorCodes::JsonDeserializationFailed), TEXT("JSON deserialization failed.") },


};

} // Namespace AccelByte

