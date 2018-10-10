// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteError.h"

namespace AccelByte
{

const std::unordered_map<int32, FString> ErrorMessages::Default
{
	// HTTP 1xx
	{ static_cast<int32>(ErrorCodes::StatusContinue), TEXT("StatusContinue Reference: RFC 7231, Section 6.2.1") },
	{ static_cast<int32>(ErrorCodes::StatusSwitchingProtocols), TEXT("StatusSwitchingProtocols Reference: RFC 7231, Section 6.2.2") },
	{ static_cast<int32>(ErrorCodes::StatusProcessing), TEXT("StatusProcessing Reference: RFC 2518, Section 10.1") },
	
	// HTTP 2xx
	{ static_cast<int32>(ErrorCodes::StatusOk), TEXT("StatusOk Reference: RFC 7231, Section 6.3.1") },
	{ static_cast<int32>(ErrorCodes::StatusCreated), TEXT("StatusCreated Reference: RFC 7231, Section 6.3.2") },
	{ static_cast<int32>(ErrorCodes::StatusAccepted), TEXT("StatusAccepted Reference: RFC 7231, Section 6.3.3") },
	{ static_cast<int32>(ErrorCodes::StatusNonAuthoritativeInfo), TEXT("StatusNonAuthoritativeInfo Reference: RFC 7231, Section 6.3.4") },
	{ static_cast<int32>(ErrorCodes::StatusNoContent), TEXT("StatusNoContent Reference: RFC 7231, Section 6.3.5") },
	{ static_cast<int32>(ErrorCodes::StatusResetContent), TEXT("StatusResetContent Reference: RFC 7231, Section 6.3.6") },
	{ static_cast<int32>(ErrorCodes::StatusPartialContent), TEXT("StatusPartialContent Reference: RFC 7233, Section 4.1") },
	{ static_cast<int32>(ErrorCodes::StatusMultiStatus), TEXT("StatusMultiStatus Reference: RFC 4918, Section 11.1") },
	{ static_cast<int32>(ErrorCodes::StatusAlreadyReported), TEXT("StatusAlreadyReported Reference: RFC 5842, Section 7.1") },
	{ static_cast<int32>(ErrorCodes::StatusImUsed), TEXT("StatusImUsed Reference: RFC 3229, Section 10.4.1") },
	
	// HTTP 3xx
	{ static_cast<int32>(ErrorCodes::StatusMultipleChoices), TEXT("StatusMultipleChoices Reference: RFC 7231, Section 6.4.1") },
	{ static_cast<int32>(ErrorCodes::StatusMovedPermanently), TEXT("StatusMovedPermanently Reference: RFC 7231, Section 6.4.2") },
	{ static_cast<int32>(ErrorCodes::StatusFound), TEXT("StatusFound Reference: RFC 7231, Section 6.4.3") },
	{ static_cast<int32>(ErrorCodes::StatusSeeOther), TEXT("StatusSeeOther Reference: RFC 7231, Section 6.4.4") },
	{ static_cast<int32>(ErrorCodes::StatusNotModified), TEXT("StatusNotModified Reference: RFC 7232, Section 4.1") },
	{ static_cast<int32>(ErrorCodes::StatusUseProxy), TEXT("StatusUseProxy Reference: RFC 7231, Section 6.4.5") },

	{ static_cast<int32>(ErrorCodes::StatusTemporaryRedirect), TEXT("StatusTemporaryRedirect Reference: RFC 7231, Section 6.4.7") },
	{ static_cast<int32>(ErrorCodes::StatusPermanentRedirect), TEXT("StatusPermanentRedirect Reference: RFC 7538, Section 3") },
	
	// HTTP 4xx
	{ static_cast<int32>(ErrorCodes::StatusBadRequest), TEXT("StatusBadRequest Reference: RFC 7231, Section 6.5.1") },
	{ static_cast<int32>(ErrorCodes::StatusUnauthorized), TEXT("StatusUnauthorized Reference: RFC 7235, Section 3.1") },
	{ static_cast<int32>(ErrorCodes::StatusPaymentRequired), TEXT("StatusPaymentRequired Reference: RFC 7231, Section 6.5.2") },
	{ static_cast<int32>(ErrorCodes::StatusForbidden), TEXT("StatusForbidden Reference: RFC 7231, Section 6.5.3") },
	{ static_cast<int32>(ErrorCodes::StatusNotFound), TEXT("StatusNotFound Reference: RFC 7231, Section 6.5.4") },
	{ static_cast<int32>(ErrorCodes::StatusMethodNotAllowed), TEXT("StatusMethodNotAllowed Reference: RFC 7231, Section 6.5.5") },
	{ static_cast<int32>(ErrorCodes::StatusNotAcceptable), TEXT("StatusNotAcceptable Reference: RFC 7231, Section 6.5.6") },
	{ static_cast<int32>(ErrorCodes::StatusProxyAuthRequired), TEXT("StatusProxyAuthRequired Reference: RFC 7235, Section 3.2") },
	{ static_cast<int32>(ErrorCodes::StatusRequestTimeout), TEXT("StatusRequestTimeout Reference: RFC 7231, Section 6.5.7") },
	{ static_cast<int32>(ErrorCodes::StatusConflict), TEXT("StatusConflict Reference: RFC 7231, Section 6.5.8") },
	{ static_cast<int32>(ErrorCodes::StatusGone), TEXT("StatusGone Reference: RFC 7231, Section 6.5.9") },
	{ static_cast<int32>(ErrorCodes::StatusLengthRequired), TEXT("StatusLengthRequired Reference: RFC 7231, Section 6.5.10") },
	{ static_cast<int32>(ErrorCodes::StatusPreconditionFailed), TEXT("StatusPreconditionFailed Reference: RFC 7232, Section 4.2") },
	{ static_cast<int32>(ErrorCodes::StatusRequestEntityTooLarge), TEXT("StatusRequestEntityTooLarge Reference: RFC 7231, Section 6.5.11") },
	{ static_cast<int32>(ErrorCodes::StatusRequestUriTooLong), TEXT("StatusRequestUriTooLong Reference: RFC 7231, Section 6.5.12") },
	{ static_cast<int32>(ErrorCodes::StatusUnsupportedMediaType), TEXT("StatusUnsupportedMediaType Reference: RFC 7231, Section 6.5.13") },
	{ static_cast<int32>(ErrorCodes::StatusRequestedRangeNotSatisfiable), TEXT("StatusRequestedRangeNotSatisfiable Reference: RFC 7233, Section 4.4") },
	{ static_cast<int32>(ErrorCodes::StatusExpectationFailed), TEXT("StatusExpectationFailed Reference: RFC 7231, Section 6.5.14") },
	{ static_cast<int32>(ErrorCodes::StatusTeapot), TEXT("StatusTeapot Reference: RFC 7168, Section 2.3.3") },
	{ static_cast<int32>(ErrorCodes::StatusMisdirectedRequest), TEXT("StatusMisdirectedRequest Reference: RFC 7540, Section 9.1.2") },
	{ static_cast<int32>(ErrorCodes::StatusUnprocessableEntity), TEXT("StatusUnprocessableEntity Reference: RFC 4918, Section 11.2") },
	{ static_cast<int32>(ErrorCodes::StatusLocked), TEXT("StatusLocked Reference: RFC 4918, Section 11.3") },
	{ static_cast<int32>(ErrorCodes::StatusFailedDependency), TEXT("StatusFailedDependency Reference: RFC 4918, Section 11.4") },
	{ static_cast<int32>(ErrorCodes::StatusUpgradeRequired), TEXT("StatusUpgradeRequired Reference: RFC 7231, Section 6.5.15") },
	{ static_cast<int32>(ErrorCodes::StatusPreconditionRequired), TEXT("StatusPreconditionRequired Reference: RFC 6585, Section 3") },
	{ static_cast<int32>(ErrorCodes::StatusTooManyRequests), TEXT("StatusTooManyRequests Reference: RFC 6585, Section 4") },
	{ static_cast<int32>(ErrorCodes::StatusRequestHeaderFieldsTooLarge), TEXT("StatusRequestHeaderFieldsTooLarge Reference: RFC 6585, Section 5") },
	{ static_cast<int32>(ErrorCodes::StatusUnavailableForLegalReasons), TEXT("StatusUnavailableForLegalReasons Reference: RFC 7725, Section 3") },
	
	// HTTP 5xx
	{ static_cast<int32>(ErrorCodes::StatusInternalServerError), TEXT("StatusInternalServerError Reference: RFC 7231, Section 6.6.1") },
	{ static_cast<int32>(ErrorCodes::StatusNotImplemented), TEXT("StatusNotImplemented Reference: RFC 7231, Section 6.6.2") },
	{ static_cast<int32>(ErrorCodes::StatusBadGateway), TEXT("StatusBadGateway Reference: RFC 7231, Section 6.6.3") },
	{ static_cast<int32>(ErrorCodes::StatusServiceUnavailable), TEXT("StatusServiceUnavailable Reference: RFC 7231, Section 6.6.4") },
	{ static_cast<int32>(ErrorCodes::StatusGatewayTimeout), TEXT("StatusGatewayTimeout Reference: RFC 7231, Section 6.6.5") },
	{ static_cast<int32>(ErrorCodes::StatusHttpVersionNotSupported), TEXT("StatusHttpVersionNotSupported Reference: RFC 7231, Section 6.6.6") },
	{ static_cast<int32>(ErrorCodes::StatusVariantAlsoNegotiates), TEXT("StatusVariantAlsoNegotiates Reference: RFC 2295, Section 8.1") },
	{ static_cast<int32>(ErrorCodes::StatusInsufficientStorage), TEXT("StatusInsufficientStorage Reference: RFC 4918, Section 11.5") },
	{ static_cast<int32>(ErrorCodes::StatusLoopDetected), TEXT("StatusLoopDetected Reference: RFC 5842, Section 7.2") },
	{ static_cast<int32>(ErrorCodes::StatusNotExtended), TEXT("StatusNotExtended Reference: RFC 2774, Section 7") },
	{ static_cast<int32>(ErrorCodes::StatusNetworkAuthenticationRequired), TEXT("StatusNetworkAuthenticationRequired Reference: RFC 6585, Section 6") },
	
	// Platform error
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

void HandleHttpError(FHttpRequestPtr Request, FHttpResponsePtr Response, int& OutCode, FString& OutMessage)
{
	int32 Code = 0;
	FAccelByteModelsErrorEntity Error;
	FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Error, 0, 0);
	Code = Error.NumericErrorCode;
	auto it = ErrorMessages::Default.find(Code);
	if (it != ErrorMessages::Default.cend())
	{
		OutMessage += ErrorMessages::Default.at(Code);
		OutMessage += " " + Error.ErrorMessage;
	}
	else
	{
		Code = Response->GetResponseCode();
	}
	// Debug message. Delete this code section for production
	#if 1
	OutMessage += "\n\nResponse";
	OutMessage += "\nCode: " + FString::FromInt(Response->GetResponseCode());
	OutMessage += "\nContent: \n" + Response->GetContentAsString();

	OutMessage += " \n\nRequest";
	OutMessage += "\nElapsed time (seconds): " + FString::SanitizeFloat(Request->GetElapsedTime());
	OutMessage += "\nVerb: " + Request->GetVerb();
	OutMessage += "\nURL: " + Request->GetURL();
	OutMessage += "\nHeaders: \n";
	for (auto a : Request->GetAllHeaders())
	{
		OutMessage += a + "\n";
	}
	OutCode = Code;
	OutMessage += "\nContent: \n";
	for (auto a : Request->GetContent())
	{
		OutMessage += static_cast<char>(a);
	}
#endif
}

} // Namespace AccelByte

