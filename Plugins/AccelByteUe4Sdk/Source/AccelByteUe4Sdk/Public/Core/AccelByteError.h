// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "AccelByteModelsError.h"
#include "JsonUtilities.h"

#include <unordered_map>

namespace AccelByte
{

DECLARE_DELEGATE_TwoParams(ErrorDelegate, int32, FString);

enum class ErrorCodes : int32
{
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
	// debug message
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

