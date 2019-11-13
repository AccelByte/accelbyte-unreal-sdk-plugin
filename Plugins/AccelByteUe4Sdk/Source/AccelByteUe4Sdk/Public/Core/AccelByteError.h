// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "JsonUtilities.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Core/Public/Templates/UnrealTypeTraits.h"

#include <unordered_map>

#include "Core/AccelByteReport.h"
#include "AccelByteError.generated.h"

DECLARE_DYNAMIC_DELEGATE(FDHandler);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FErrorInfo
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
		int32 NumericErrorCode = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
		FString ErrorCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
		FString ErrorMessage;
};

namespace AccelByte
{

	template <typename T> using THandler = TBaseDelegate<void, const T&>;
	using FVoidHandler = TBaseDelegate<void>;
	using FErrorHandler = TBaseDelegate<void, int32 /*ErrorCode*/, const FString& /* ErrorMessage */>;

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
		PlatformInternalServerErrorException = 20000,
		UnauthorizedException,
		ValidationException,
		UnauthorizedAccessException,
		ServiceUnavailableException,
		UnsupportedMediaTypeException,
		OptimisticLockException,
		TooManyRequestException,
		UserNotFoundException,
		UnknownErrorException,
		UserRevokedException,
		TokenExpiredException,
		TokenRevokedException,
		insufficientPermissionException,
		InvalidAudienceException,
		insufficientScopeException,
		ActionBannedException,
		UserIsNotLinkedToNamespaceException,
		BadRequestException,
		// Namespace Error Code List
		NamespaceNotFoundException = 2141,
		NamespaceAlreadyExistsException = 2171,
		// Profile Error Code List
		UserProfileNotFoundException = 2241,
		UserProfileAlreadyExistsException = 2271,
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
		ItemBundleException = 3023,
		DefaultRegionRequiredException = 3026,
		ItemAlreadyBundledException = 3073,
		LanguageNotSupportedException = 3074,
		RegionNotSupportedException = 3075,
		PublishedStoreNotUpdatableException = 3076,
		//
		//
		InvalidStoreDataException = 30121,
		StoreMetaMismatchException = 30122,
		InvalidDiscountValueException = 30321,
		StoreNotFoundException = 30141,
		PublishedStoreNotFoundException = 30142,
		CategoryNotFoundException = 30241,
		CategoryAlreadyExistsException = 30271,
		CategoryNotEmptyException = 30272,
		ItemNotFoundException = 30341,
		ItemAppIdNotFoundException = 30342,
		//
		//Entitlement Error Code List
		//
		UserEntitlementNotFoundException = 3141,
		EntitlementAlreadyRevokedException = 3171,
		EntitlementNotActiveException = 3172,
		EntitlementNotConsumableException = 3173,
		EntitlementUsedUpException = 3174,
		EntitlementAlreadyDistributedException = 3175,
		//
		//
		EntitlementNotDistributableException = 31121,
		ExceedDistributeQuantityException = 31122,
		PublisherNamespaceNotDistributableException = 31123,
		EntitlementIdNotFoundException = 31141,
		EntitlementAppIdNotFoundException = 31142,
		DistributionReceiverNotFoundException = 31241,
		DistributionReceiverAlreadyExistsException = 31271,
		//
		//Order Error Code List
		//
		OrderNotRefundableException = 3271,
		InvalidOrderStatusException = 3272,
		OrderTransactionAmountMismatchException = 3274,
		//
		//
		OrderPriceMismatchException = 32121,
		OrderNotFoundException = 32141,
		OrderReceiptNotDownloadableException = 32173,
		ExceedItemMaxCountPerUserException = 32175,
		ExceedItemMaxCountException = 32176,
		OrderNotCancelableException = 32177,
		//
		//Payment Error Code List
		//
		InvalidPaymentOrderTransactionStatusException = 3372,
		InvalidPaymentOrderTransactionTypeException = 3373,
		PaymentOrderTransactionAmountMismatchException = 3375,
		WxPayUnifiedOrderFailedException = 3376,
		PaymentExtOrderNoAlreadyExistsException = 3377,
		//
		//
		PaymentOrderNotFoundException = 33141,
		PaymentOrderChargeTransactionNotFoundException = 33142,
		InvalidPaymentOrderStatusException = 33171,
		PaymentOrderNotRefundableException = 33172,
		PaymentCurrencyNotSupportedException = 33321,
		PaymentProviderNotSupportedException = 33322,
		AliPayRefundFailedException = 33323,
		WxPayRefundFailedException = 33324,
		//
		//Wallet Error Code List
		//
		WalletExceedMaxTransactionAmountPerDayException = 35121,
		WalletExceedMaxAmountPerTransactionException = 35122,
		WalletInactiveException = 35123,
		WalletInsufficientBalanceException = 35124,
		WalletExceedMaxBalanceAmountException = 35125,
		WalletNotFoundException = 35141,
		//
		//Currency Error Code List
		//
		CurrencyAlreadyExistsException = 36171,
		CurrencyNotFoundException = 36141,
		//
		//Campaign Error Code List
		//
		CampaignNotFoundException = 37141,
		CodeNotFoundException = 37142,
		CampaignAlreadyExistsException = 37171,
		CampaignIsInactiveException = 37172,
		CodeIsInactiveException = 37173,
		ExceedMaxRedeemCountPerCodeException = 37174,
		ExceedMaxRedeemCountPerUserException = 37175,
		CodeHasBeenRedeemedException = 37176,
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
		//
		//Statistic Error Code List
		//
		StatisticNotFoundException = 70131,
		InvalidStatOperatorException = 70330,
		StatNotDecreasableException = 70334,
		UserStatsNotFoundException = 70335,
		UserStatAlreadyExistException = 70336,
		StatValueOutOfRangeException = 70337,
		//
		//Client side Error Code List
		//
		UnknownError = 14000,
		JsonDeserializationFailed = 14001,
		NetworkError = 14005,
		WebSocketConnectFailed = 14201
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

	inline void HandleHttpResultOk(FHttpResponsePtr Response, const FVoidHandler& OnSuccess)
	{
		OnSuccess.ExecuteIfBound();
	}

	template<typename T>
	inline void HandleHttpResultOk(FHttpResponsePtr Response, const THandler<TArray<T>>& OnSuccess)
	{
		TArray<T> Result;
		FJsonObjectConverter::JsonArrayStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);

		OnSuccess.ExecuteIfBound(Result);
	}

	template<>
	inline void HandleHttpResultOk<uint8>(FHttpResponsePtr Response, const THandler<TArray<uint8>>& OnSuccess)
	{
		OnSuccess.ExecuteIfBound(Response->GetContent());
	}

	template<typename T>
	inline void HandleHttpResultOk(FHttpResponsePtr Response, const THandler<T>& OnSuccess)
	{
		typename std::remove_const<typename std::remove_reference<T>::type>::type Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);

		OnSuccess.ExecuteIfBound(Result);
	}

	template<>
	inline void HandleHttpResultOk<FString>(FHttpResponsePtr Response, const THandler<FString>& OnSuccess)
	{
		OnSuccess.ExecuteIfBound(Response->GetContentAsString());
	}

	template<typename T>
	FHttpRequestCompleteDelegate CreateHttpResultHandler(const T& OnSuccess, const FErrorHandler& OnError)
	{
		return FHttpRequestCompleteDelegate::CreateLambda(
			[OnSuccess, OnError]
		(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
		{
			Report report;
			report.GetHttpResponse(Request, Response);

			if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				HandleHttpResultOk(Response, OnSuccess);
				return;
			}

			int32 Code;
			FString Message;
			HandleHttpError(Request, Response, Code, Message);
			OnError.ExecuteIfBound(Code, Message);
		});
	}

} // Namespace AccelByte
