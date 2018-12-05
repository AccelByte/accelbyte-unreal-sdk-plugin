// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "JsonUtilities.h"

#include <unordered_map>

#include "AccelByteError.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsErrorEntity
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
		int32 NumericErrorCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
		FString ErrorCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
		FString ErrorMessage;
};

namespace AccelByte
{

DECLARE_DELEGATE_TwoParams(FErrorHandler, int32 /* ErrorCode */, const FString& /* ErrorMessage */);

UENUM(BlueprintType)
enum class ErrorCodes : int32
{
	// If there are no error models, then we just use these HTTP codes
	StatusContinue = 100, // Reference: RFC 7231, Section 6.2.1
	StatusSwitchingProtocols = 101, // Reference: RFC 7231, Section 6.2.2
	StatusProcessing = 102, // Reference: RFC 2518, Section 10.1

	StatusOk = 200, // Reference: RFC 7231, Section 6.3.1
	StatusCreated = 201, // Reference: RFC 7231, Section 6.3.2
	StatusAccepted = 202, // Reference: RFC 7231, Section 6.3.3
	StatusNonAuthoritativeInfo = 203, // Reference: RFC 7231, Section 6.3.4
	StatusNoContent = 204, // Reference: RFC 7231, Section 6.3.5
	StatusResetContent = 205, // Reference: RFC 7231, Section 6.3.6
	StatusPartialContent = 206, // Reference: RFC 7233, Section 4.1
	StatusMultiStatus = 207, // Reference: RFC 4918, Section 11.1
	StatusAlreadyReported = 208, // Reference: RFC 5842, Section 7.1
	StatusImUsed = 226, // Reference: RFC 3229, Section 10.4.1

	StatusMultipleChoices = 300, // Reference: RFC 7231, Section 6.4.1
	StatusMovedPermanently = 301, // Reference: RFC 7231, Section 6.4.2
	StatusFound = 302, // Reference: RFC 7231, Section 6.4.3
	StatusSeeOther = 303, // Reference: RFC 7231, Section 6.4.4
	StatusNotModified = 304, // Reference: RFC 7232, Section 4.1
	StatusUseProxy = 305, // Reference: RFC 7231, Section 6.4.5

	StatusTemporaryRedirect = 307, // Reference: RFC 7231, Section 6.4.7
	StatusPermanentRedirect = 308, // Reference: RFC 7538, Section 3

	StatusBadRequest = 400, // Reference: RFC 7231, Section 6.5.1
	StatusUnauthorized = 401, // Reference: RFC 7235, Section 3.1
	StatusPaymentRequired = 402, // Reference: RFC 7231, Section 6.5.2
	StatusForbidden = 403, // Reference: RFC 7231, Section 6.5.3
	StatusNotFound = 404, // Reference: RFC 7231, Section 6.5.4
	StatusMethodNotAllowed = 405, // Reference: RFC 7231, Section 6.5.5
	StatusNotAcceptable = 406, // Reference: RFC 7231, Section 6.5.6
	StatusProxyAuthRequired = 407, // Reference: RFC 7235, Section 3.2
	StatusRequestTimeout = 408, // Reference: RFC 7231, Section 6.5.7
	StatusConflict = 409, // Reference: RFC 7231, Section 6.5.8
	StatusGone = 410, // Reference: RFC 7231, Section 6.5.9
	StatusLengthRequired = 411, // Reference: RFC 7231, Section 6.5.10
	StatusPreconditionFailed = 412, // Reference: RFC 7232, Section 4.2
	StatusRequestEntityTooLarge = 413, // Reference: RFC 7231, Section 6.5.11
	StatusRequestUriTooLong = 414, // Reference: RFC 7231, Section 6.5.12
	StatusUnsupportedMediaType = 415, // Reference: RFC 7231, Section 6.5.13
	StatusRequestedRangeNotSatisfiable = 416, // Reference: RFC 7233, Section 4.4
	StatusExpectationFailed = 417, // Reference: RFC 7231, Section 6.5.14
	StatusTeapot = 418, // Reference: RFC 7168, Section 2.3.3
	StatusMisdirectedRequest = 421, // Reference: RFC 7540, Section 9.1.2
	StatusUnprocessableEntity = 422, // Reference: RFC 4918, Section 11.2
	StatusLocked = 423, // Reference: RFC 4918, Section 11.3
	StatusFailedDependency = 424, // Reference: RFC 4918, Section 11.4
	StatusUpgradeRequired = 426, // Reference: RFC 7231, Section 6.5.15
	StatusPreconditionRequired = 428, // Reference: RFC 6585, Section 3
	StatusTooManyRequests = 429, // Reference: RFC 6585, Section 4
	StatusRequestHeaderFieldsTooLarge = 431, // Reference: RFC 6585, Section 5
	StatusUnavailableForLegalReasons = 451, // Reference: RFC 7725, Section 3

	StatusInternalServerError = 500, // Reference: RFC 7231, Section 6.6.1
	StatusNotImplemented = 501, // Reference: RFC 7231, Section 6.6.2
	StatusBadGateway = 502, // Reference: RFC 7231, Section 6.6.3
	StatusServiceUnavailable = 503, // Reference: RFC 7231, Section 6.6.4
	StatusGatewayTimeout = 504, // Reference: RFC 7231, Section 6.6.5
	StatusHttpVersionNotSupported = 505, // Reference: RFC 7231, Section 6.6.6
	StatusVariantAlsoNegotiates = 506, // Reference: RFC 2295, Section 8.1
	StatusInsufficientStorage = 507, // Reference: RFC 4918, Section 11.5
	StatusLoopDetected = 508, // Reference: RFC 5842, Section 7.2
	StatusNotExtended = 510, // Reference: RFC 2774, Section 7
	StatusNetworkAuthenticationRequired = 511, // Reference: RFC 6585, Section 6

	// Platform error
	PlatformInternalServerErrorException = 2000,
	UnauthorizedException,
	ValidationException,
	ForbiddenException,
	ServiceUnavailableException,
	UnsupportedMediaTypeException,
	OptimisticLockException,
	// Namespace Error Code List
	NamespaceNotFoundException = 2141,
	NamespaceAlreadyExistsException = 2171,
	// Profile Error Code List
	UserProfileNotFoundException = 2241,
	UserProfileAlreadyExistsException  = 2271,
	// FileStorage Error Code List
	FileTypeNotSupportedException = 2321,
	UserFileStorageExceedLimitException = 2371,
	// Buildinfo Error Code List
	EntitlementRequiredException = 2303,
	EntitlementRequiredException2 = 2304,
	BlockEmptyException = 2321,
	BlockUuidRequiredException = 2322,
	InvalidPlatformIdException = 2323,
	MaximumDRMActivationLimitExceeded = 2324,
	//
	BlockHashMismatchException = 2325,
	//
	BuildNotFoundException = 2341,
	BlockNotFoundException = 2342,
	BaseBuildNotFoundException = 2343,
	//
	ReleaseBuildNotFoundException = 2344,
	//
	BuildAlreadyCommittedException = 2371,
	BuildIncompleteException = 2372,
	UploadAlreadyCompleteException = 2373,
	UploadAlreadyStartedException = 2374,
	DuplicateBuildIdException = 2375,
	//
	//Category Error Code List
	//
	DefaultLanguageRequiredException = 3021,
	InvalidDiscountValueException = 3022,
	ItemBundleException = 3023,
	InvalidStoreDataException = 3024,
	StoreMetaMismatchException = 3025,
	//
	DefaultRegionRequiredException = 3026,
	//
	CategoryNotFoundException = 3041,
	ItemNotFoundException = 3042,
	StoreNotFoundException = 3043,
	//
	PublishedStoreNotFoundException = 3044,
	//
	CategoryAlreadyExistsException = 3071,
	CategoryNotEmptyException = 3072,
	ItemAlreadyBundledException = 3073,
	LanguageNotSupportedException = 3074,
	RegionNotSupportedException = 3075,
	PublishedStoreNotUpdatableException = 3076,
	//
	//Entitlement Error Code List
	//
	EntitlementNotDistributableException = 3121,
	ExceedDistributeQuantityException = 3122,
	//
	PublisherNamespaceNotDistributableException = 3123,
	//
	UserEntitlementNotFoundException = 3141,
	EntitlementNotFoundException = 3142,
	UserAppEntitlementNotFoundException = 3143,
	//
	DistributionReceiverNotFoundException = 3144,
	//
	EntitlementAlreadyRevokedException = 3171,
	EntitlementNotActiveException = 3172,
	EntitlementNotConsumableException = 3173,
	EntitlementUsedUpException = 3174,
	EntitlementAlreadyDistributedException = 3175,
	DistributionReceiverAlreadyExistsException = 3176,
	//
	//Order Error Code List
	//
	OrderPriceMismatchException = 3221,
	//
	OrderNotFoundException = 3241,
	//
	OrderNotRefundableException = 3271,
	InvalidOrderStatusException = 3272,
	OrderReceiptNotDownloadableException = 3273,
	OrderTransactionAmountMismatchException = 3274,
	ExceedItemMaxCountPerUserException = 3275,
	//
	//Payment Error Code List
	//
	PaymentCurrencyNotSupportedException = 3321,
	PaymentProviderNotSupportedException = 3322,
	AliPayRefundFailedException = 3323,
	//
	WxPayRefundFailedException = 3324,
	//
	PaymentOrderNotFoundException = 3341,
	//
	PaymentOrderChargeTransactionNotFoundException = 3342,
	//
	InvalidPaymentOrderStatusException = 3371,
	InvalidPaymentOrderTransactionStatusException = 3372,
	InvalidPaymentOrderTransactionTypeException = 3373,
	PaymentOrderNotRefundableException = 3374,
	PaymentOrderTransactionAmountMismatchException = 3375,
	WxPayUnifiedOrderFailedException = 3376,
	PaymentExtOrderNoAlreadyExistsException = 3377,

	//Wallet Error Code List
	//
	ExceedMaxTransactionAmountPerDayException = 3521,
	ExceedMaxAmountPerTransactionException = 3522,
	InactiveWalletException = 3523,
	InsufficientBalanceException = 3524,
	//
	ExceedMaxBalanceAmountException = 3525,
	//
	WalletNotFoundException = 3541,
	//
	//Currency Error Code List
	//
	CurrencyNotFoundException = 3641,
	//
	CurrencyAlreadyExistsException = 3671,
	//
	//Campaign Error Code List
	//
	CampaignNotFoundException = 3741,
	CodeNotFoundException = 3742,
	CampaignAlreadyExistsException = 3771,
	CampaignIsInactiveException = 3772,
	CodeIsInactiveException = 3773,
	ExceedMaxRedeemCountPerCodeException = 3774,
	ExceedMaxRedeemCountPerUserException = 3775,
	CodeHasBeenRedeemedException = 3776,
	//
	//Social Error Code List
	//Activity
	//
	ActivityTrackNotFoundException = 4041,
	//
	//Favorite
	//
	FavoriteItemNotFoundException = 4141,
	FavoriteUserNotFoundException = 4142,
	//
	//Offense report
	//
	OffenseReportNotFoundException = 4241,
	OffenseTypeNotFoundException = 4242,
	OffenseTypeAlreadyExistsException = 4271,
	OffenseReportAssociatedException = 4272,
		
	UnknownError = 14000,
	JsonDeserializationFailed = 14001,
	WebSocketConnectFailed = 14201,
};

class ErrorMessages
{
public:
	/**
	 * @brief Contains dictionary for default error messages (English).
	 */
	const static std::unordered_map<std::underlying_type<ErrorCodes>::type, FString> Default;
};

ACCELBYTEUE4SDK_API void HandleHttpError(FHttpRequestPtr Request, FHttpResponsePtr Response, int& OutCode, FString& OutMessage);

} // Namespace AccelByte

