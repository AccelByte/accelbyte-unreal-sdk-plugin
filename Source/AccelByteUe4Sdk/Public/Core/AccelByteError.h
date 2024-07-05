// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "JsonUtilities.h"
#include "Runtime/Launch/Resources/Version.h"

#include <unordered_map>

#include "Models/AccelByteLobbyModels.h"
#include "Models/AccelByteErrorModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteHttpCache.h"
#include "Core/AccelByteTypeConverter.h"
#include "Logging/AccelByteServiceLogger.h"
#include "AccelByteError.generated.h"

DECLARE_DYNAMIC_DELEGATE(FDHandler);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FDCustomErrorHandler, int32, ErrorCode, const FString&, ErrorMessage, const FJsonObjectWrapper&, ErrorObject);
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FDOAuthErrorHandler, int32, ErrorCode, const FString&, ErrrMessage, const FErrorOAuthInfo&, ErrorInfo);

namespace AccelByte
{
	// forward declaration
	class FHttpRetryScheduler;

#if (ENGINE_MAJOR_VERSION == 5) || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION > 25)
	template <typename T> using THandler = TDelegate<void(const T&)>;
	template <typename T1, typename T2> using THandlerPayloadModifier = TDelegate<T1(T2)>;
	using FVoidHandler = TDelegate<void()>;
	using FErrorHandler = TDelegate<void(int32 /* ErrorCode */, const FString& /* ErrorMessage */)>;
	using FCustomErrorHandler = TDelegate<void(int32 /* ErrorCode */, const FString& /* ErrorMessage */, const FJsonObject& /* ErrorObject */)>;
	using FOAuthErrorHandler = TDelegate<void(int32 /* ErrorCode */, const FString& /* ErrorMessage */, const FErrorOAuthInfo& /* ErrorObject */)>;
	using FCreateMatchmakingTicketErrorHandler = TDelegate<void(int32 /* ErrorCode */, const FString& /* ErrorMessage */, const FErrorCreateMatchmakingTicketV2& /* CreateTicketErrorInfo */)>;
#else
	template <typename T> using THandler = TBaseDelegate<void, const T&>;
	template <typename T1, typename T2> using THandlerPayloadModifier = TBaseDelegate<T1, T2>;
	using FVoidHandler = TBaseDelegate<void>;
	using FErrorHandler = TBaseDelegate<void, int32 /*ErrorCode*/, const FString& /* ErrorMessage */>;
	using FCustomErrorHandler = TBaseDelegate<void, int32 /* ErrorCode*/, const FString& /* ErrorMessage */, const FJsonObject& /* ErrorObject */>;
	using FOAuthErrorHandler = TBaseDelegate<void, int32 /*ErrorCode*/, const FString& /* ErrorMessage */, const FErrorOAuthInfo& /* ErrorObject */>;
	using FCreateMatchmakingTicketErrorHandler = TBaseDelegate<void(int32 /* ErrorCode */, const FString& /* ErrorMessage */, const FErrorCreateMatchmakingTicketV2& /* CreateTicketErrorInfo */)>;
#endif

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

		// User Error Code List
		UserUnderAgeException = 10130,
		UserEmailAlreadyUsedException = 10133,
		UserCountryNotFoundException = 10154,
		UserNameAlreadyUsedException = 10177,
		UserDisplayNameAlreadyUsedException = 10180,

		// User Simultaneous Login
		SimultaneousLogin_SecondaryPlatformTicketRequired = 10215,
		SimultaneousLogin_BothNativeAndSecondaryPlatformAlreadyLinkedToDifferentAccelByteAccount = 10220,
		SimultaneousLogin_AccountAlreadyLinkedButSecondaryTicketMismatch = 10219,
		SimultaneousLogin_NativePlatformAccountPreviouslyLinkedToAnotherAccount = 10217,
		SimultaneousLogin_AccountAlreadyLinkedButNativeTicketMismatch = 10221,
		SimultaneousLogin_SecondaryPlatformAccountPreviouslyLinkedToAnotherAccount = 10218,

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
		NamespaceAlreadyExistsException = 11336,
		NamespaceNotFoundException,
		NamespaceContainsInvalidCharacterException,
		DisplayNameContainsInvalidCharacterException,
		// Profile Error Code List
		UserProfileNotFoundException = 11440,
		UserProfileAlreadyExistsException = 11441,
		// FileStorage Error Code List
		FileTypeNotSupportedException = 11131,
		UserFileStorageExceedLimitException = 11132,
		// Build info Error Code List
		EntitlementRequiredException = 12103,
		BlockEmptyException = 12121,
		BlockUuidRequiredException = 12122,
		InvalidPlatformIdException = 12123,
		MaximumDRMActivationLimitExceeded = 12124,
		//
		BlockHashMismatchException = 12125,
		//
		BuildNotFoundException = 12141,
		BlockNotFoundException = 12142,
		BaseBuildNotFoundException = 12143,
		//
		ReleaseBuildNotFoundException =12144,
		//
		BuildAlreadyCommittedException = 12171,
		BuildIncompleteException = 12172,
		UploadAlreadyCompleteException = 12173,
		UploadAlreadyStartedException = 12174,
		DuplicateBuildIdException = 12175,
		
		//Catalog error
		//
		//Category Error Code List
		//
		CategoryNotFoundException = 30241,
		CategoryAlreadyExistsException = 30271,
		CategoryNotEmptyException = 30272,
		//
		//Store Error Code List
		//
		InvalidStoreDataException = 30121,
		StoreMetaMismatchException = 30122,
		StoreNotFoundException = 30141,
		PublishedStoreNotFoundException = 30142,
		//
		//Catalog Error Code List
		//
		DefaultLanguageRequiredException = 30021,
		DefaultRegionRequiredException = 30022,
		//
		//Item Error Code List
		//
		ItemInvalidDiscountValueException = 30321,
		ItemBundleException = 30322,
		//
		ItemNotFoundException = 30341,
		ItemAppIdNotFoundException = 30342,
		ItemSkuNotFoundException = 30343,
		ItemConfigNotFoundInNamespace= 39341,
		//
		//Entitlement Error Code List
		//
		EntitlementNotDistributableException = 31121,
		ExceedDistributeQuantityException,
		PublisherNamespaceNotDistributableException,
		//
		EntitlementIdNotFoundException = 31141,
		EntitlementAppIdNotFoundException,
		//
		EntitlementAlreadyRevokedException = 31171,
		EntitlementNotActiveException,
		EntitlementNotConsumableException,
		EntitlementAlreadyUsedException,
		EntitlementAlreadyDistributedException,
		EntitlementUseCountIsInsufficientException,
		PermanentItemAlreadyOwned,
		EntitlementOutOfTimeRangeException,
		EntitlementIdInvalidFormatException = 20002,
		//
		DistributionReceiverNotFoundException = 31241,
		DistributionReceiverAlreadyExistsException = 31271,
		//
		//Order Error Code List
		//
		OrderPriceMismatchException = 32121,
		//
		OrderNotFoundException = 32141,
		//
		OrderNotRefundableException = 32171,
		InvalidOrderStatusException,
		OrderReceiptNotDownloadableException,
		OrderTransactionAmountMismatchException,
		ExceedItemMaxCountPerUserException,
		ExceedItemMaxCountException,
		OrderNotCancelableException,
		//
		//Payment Error Code List
		//
		InvalidPaymentOrderTransactionStatusException = 3372,
		InvalidPaymentOrderTransactionTypeException = 3373,
		PaymentOrderTransactionAmountMismatchException = 3375,
		//
		PaymentOrderNotFoundException = 33141,
		PaymentOrderChargeTransactionNotFoundException = 33142,
		InvalidPaymentOrderStatusException = 33171,
		PaymentOrderNotRefundableException = 33172,
		PaymentExtOrderNoAlreadyExistsException = 33173,
		PaymentCurrencyNotSupportedException = 33321,
		PaymentProviderNotSupportedException = 33322,
		AliPayRefundFailedException = 33323,
		WxPayRefundFailedException = 33324,
		WxPayUnifiedOrderFailedException = 33327,
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
		CodeRedeemptionNotStartedException = 37177,
		CodeRedeemptionAlreadyEndedException = 37178,
		MaxRedeemCountPerCodePerCampaignExceededException = 37179,
		//
		//Social Error Code List
		//
		//Game Profile Error Code List
		GameProfileAttributeMismatchException = 12022,
		GameProfileIdNotFoundException = 12041,
		//Statistic Error Code List
		InvalidStatOperatorException = 12221,
		StatisticConfigNotFoundException = 12222,
		//
		StatisticNotFoundException = 12241,
		UserStatsNotFoundException = 12242,
		//
		StatNotDecreasableException = 12273,
		UserStatAlreadyExistException = 12274,
		StatValueOutOfRangeException = 12275,
		//
		StatisticsEmptyInputException = 20002,

		// V2 sessions
		SessionJoinSessionFull = 20032,
		SessionUpdateVersionMismatch = 20071,
		SessionPlayerAttributesNotFound = 20050,
		SessionPartyNotFound = 20041,
		SessionGameNotFound = 20042,
		SessionStorageNonLeaderUpdateLeaderData = 30054,
		SessionStorageMemberNotInSession = 30052,
		SessionStorageUserNotActiveInSession = 30055,

		//
		// V2 Matchmaking
		//
		MatchmakingV2CreateMatchTicketConflict = 520300,
		MatchmakingV2MatchTicketNotFound = 520303,

		//
		//SeasonPass Error Code List
		//
		SeasonPassDefaultLanguageRequiredException = 49121,
		SeasonPassInvalidTimeRangeException,
		//
		SeasonPassTierItemNotFoundException = 49141,
		SeasonPassPassItemNotFoundException,
		SeasonPassSeasonNotFoundException,
		SeasonPassRewardNotFoundException,
		SeasonPassPassCodeNotFoundException,
		//
		SeasonPassSeasonInvalidStatusException = 49171,
		SeasonPassRewardsNotProvidedException,
		SeasonPassPassesNotProvidedException,
		SeasonPassTiersNotProvidedException,
		//Leaderboard Error Code List
		//
		LeaderboardConfigAlreadyExist = 71132,
		LeaderBoardUserRankingDataNotFound = 71233,
		LeaderboardRankingNotFound = 71235,
		//
		//CloudSave Error Code List
		//
		GameRecordNotFoundException = 18003,
		GetGameRecordBadRequestException = 18004,
		CreateGameRecordValueTooBigException = 18015,
		PlayerRecordNotFoundException = 18022,
		PlayerRecordGetterIsNotOwnerException = 18023,
		PlayerRecordEditorIsNotOwnerException = 18035,
		GameRecordValidationErrorException = 18055,
		GameRecordPreconditionFailedException = 18056,
		PlayerPublicRecordNotFoundException = 18081,
		PlayerPublicRecordValidationErrorException = 18102,
		PlayerRecordPreconditionFailedException = 18103,
		//
		//DSM Error Code List
		//
		DedicatedServerNotFoundException = 9014183,
		DedicatedServerConfigNotFoundException = 9014123,
		//
		// Matchmaking Error Code List
		//
		MatchmakingRoleBasedEmptyRole = 510401,
		MatchmakingRoleBasedWrongRole = 510402,
		//
		//Client side Error Code List
		//
		UnknownError = 14000,
		JsonDeserializationFailed = 14001,
		InvalidRequest = 14003,
		InvalidResponse = 14004,
		NetworkError = 14005,
		IsNotLoggedIn = 14006,
		LoginQueueCanceled = 14007,
		WebSocketConnectFailed = 14201,
		CachedTokenNotFound = 14301,
		UnableToSerializeCachedToken = 14302,
		CachedTokenExpired = 14303,
		Deprecated = 14901,
		//
		//GameServer-side Error Code List
		//
		DsRegistrationConflict = 9014143,
		//
		//PartyStorage Error Code List
		// 
		PartyStorageOutdatedUpdateData = 119017,
		PartyNotFound = 11901,
		//
		//Lobby Error Code List
		//
		FriendRequesterMaxFriendsLimitReached = 11590,
		FriendRequesteeMaxFriendsLimitReached = 11591,
		//
		//UGC Error Code List
		//
		UGCCreateContentBadRequestException = 770100,
		UGCCreateContentConflictShareCodeErrorException = 770103,
		UGCGetContentByIDNotFoundException = 770200,
		UGCDownloadContentByShareCodeNotFoundException = 770300,
		UGCCreateChannelMalformedRequestException = 770500,
		UGCUpdateChannelMalformedRequestException = 770600,
		UGCUpdateChannelNotFoundException = 770603,
		UGCGetTagBadRequestException = 771501,
		UGCGetTypeBadRequestException = 771801,
		UGCDeleteChannelNotFoundException = 772502,
		UGCUpdateContentNotFoundException = 772603,
		UGCDeleteContentNotFoundException = 772702,
		UGCGetContentPreviewNotFoundException = 773702,
	};

	class ACCELBYTEUE4SDK_API ErrorMessages
	{
	public:
		/**
		 * @brief Contains dictionary for default error messages (English).
		 */
		const static std::unordered_map<std::underlying_type<ErrorCodes>::type, FString> Default;
	};

	ACCELBYTEUE4SDK_API void HandleHttpError(FHttpRequestPtr Request, FHttpResponsePtr Response, int& OutCode, FString& OutMessage);

	ACCELBYTEUE4SDK_API void HandleHttpCustomError(FHttpRequestPtr Request, FHttpResponsePtr Response, int& OutCode, FString& OutMessage, FJsonObject& OutErrorObject);
	
	ACCELBYTEUE4SDK_API void HandleHttpOAuthError(FHttpRequestPtr Request, FHttpResponsePtr Response, int& OutCode, FString& OutMessage, FErrorOAuthInfo& OutErrorInfo);

	ACCELBYTEUE4SDK_API void HandleHttpCreateMatchmakingTicketError(FHttpRequestPtr Request, FHttpResponsePtr Response, int& OutCode, FString& OutMessage, FErrorCreateMatchmakingTicketV2& OutErrorCreateMatchmakingV2);

	inline bool HandleHttpResultOk(FHttpResponsePtr Response, TArray<uint8> Payload, const FVoidHandler& OnSuccess)
	{
		OnSuccess.ExecuteIfBound();
		return true;
	}

	template<typename T>
	inline bool HandleHttpResultOk(FHttpResponsePtr Response, TArray<uint8> Payload, const THandler<TArray<T>>& OnSuccess)
	{
		FString String = Response == nullptr ? FAccelByteArrayByteFStringConverter::BytesToFString(Payload, true) : Response->GetContentAsString();
		TArray<T> Result;
		bool bSuccess = FAccelByteJsonConverter::JsonArrayStringToUStruct(String, &Result);
		if (bSuccess)
		{
			OnSuccess.ExecuteIfBound(Result);
		}
		return bSuccess;

	}

	template<>
	inline bool HandleHttpResultOk<uint8>(FHttpResponsePtr Response, TArray<uint8> Payload, const THandler<TArray<uint8>>& OnSuccess)
	{
		TArray<uint8> Integers = Response == nullptr ? Payload : Response->GetContent();
		OnSuccess.ExecuteIfBound(Integers);
		return true;
	}

	template<typename T>
	inline bool HandleHttpResultOk(FHttpResponsePtr Response, TArray<uint8> Payload, const THandler<T>& OnSuccess)
	{
		FString String = Response == nullptr ? FAccelByteArrayByteFStringConverter::BytesToFString(Payload, true) : Response->GetContentAsString();

		typename std::remove_const<typename std::remove_reference<T>::type>::type Result;
		bool bSuccess = FAccelByteJsonConverter::JsonObjectStringToUStruct(String, &Result);
		if (bSuccess)
		{
			OnSuccess.ExecuteIfBound(Result);
		}
		return bSuccess;
	}

	template<>
	inline bool HandleHttpResultOk<FString>(FHttpResponsePtr Response, TArray<uint8> Payload, const THandler<FString>& OnSuccess)
	{
		FString String = Response == nullptr ? FAccelByteArrayByteFStringConverter::BytesToFString(Payload, true) : Response->GetContentAsString();
		OnSuccess.ExecuteIfBound(String);
		return true;
	}

	inline bool HandleHttpResultOk(FHttpResponsePtr Response, TArray<uint8> Payload, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess)
	{
		// custom http result for LobbyServer.GetPartyStorage
		FString jsonString = Response == nullptr ? FAccelByteArrayByteFStringConverter::BytesToFString(Payload, true) : Response->GetContentAsString();
		int index = jsonString.Find("\"updatedAt\"");
		int startIndex = 0, endIndex = 0;

		bool foundFirst = false;
		bool foundLast = false;
		for (int32 i = index; i < jsonString.Len(); i++)
		{
			if (FChar::IsDigit(jsonString[i]) && !foundFirst)
			{
				startIndex = i;
				foundFirst = true;
			}
			else if (!FChar::IsDigit(jsonString[i]) && foundFirst && !foundLast)
			{
				endIndex = (i + 1);
				foundLast = true;
			}
		}
		jsonString.InsertAt(startIndex, "\"");
		jsonString.InsertAt(endIndex, "\"");
		FAccelByteModelsPartyDataNotif PartyData;
		bool bSuccess = FAccelByteJsonConverter::JsonObjectStringToUStruct(jsonString, &PartyData);
		if (bSuccess)
		{
			OnSuccess.ExecuteIfBound(PartyData);
		}
		return bSuccess;
	}

	inline bool HandleHttpResultOk(FHttpResponsePtr Response, TArray<uint8> Payload, const THandler<FJsonObject>& OnSuccess)
	{
		FString String = Response == nullptr ? FAccelByteArrayByteFStringConverter::BytesToFString(Payload, true) : Response->GetContentAsString();

		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(String);
		bool bSuccess = FJsonSerializer::Deserialize(Reader, JsonObject);
		if (bSuccess)
		{
			OnSuccess.ExecuteIfBound(*JsonObject.Get());
		}
		return bSuccess;
	}

	template<typename T>
	FHttpRequestCompleteDelegate CreateHttpResultHandler(const T& OnSuccess, const FErrorHandler& OnError, FHttpRetryScheduler* Scheduler = nullptr)
	{
		return FHttpRequestCompleteDelegate::CreateLambda(
			[OnSuccess, OnError, Scheduler](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bFinished)
			{
				ACCELBYTE_SERVICE_LOGGING_HTTP_RESPONSE(Request, Response, bFinished);
				if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
				{
					if (!HandleHttpResultOk(Response, TArray<uint8>(), OnSuccess))
					{
						OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidResponse), "Invalid JSON response");
					}
					return;
				}

				if (!bFinished)
				{
					OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::NetworkError), "Request not sent.");
					return;
				}

				// If response is nullptr then search the actual response in the cache
				if (!Response.IsValid() && Scheduler != nullptr)
				{
					FAccelByteHttpCacheItem* Cache = Scheduler->GetHttpCache().GetSerializedHttpCache(Request);
					if (Cache != nullptr && EHttpResponseCodes::IsOk(Cache->SerializableRequestAndResponse.ResponseCode))
					{
						auto ResponsePayloadByte = Cache->SerializableRequestAndResponse.ResponsePayload;
						HandleHttpResultOk(nullptr, ResponsePayloadByte, OnSuccess);
						return;
					}
				}

				int32 Code;
				FString Message;
				HandleHttpError(Request, Response, Code, Message);
				OnError.ExecuteIfBound(Code, Message);
			});
	}

	template<typename T>
	FHttpRequestCompleteDelegate CreateHttpResultHandler(const T& OnSuccess, const FCustomErrorHandler& OnError, FHttpRetryScheduler* Scheduler = nullptr)
	{
		return FHttpRequestCompleteDelegate::CreateLambda(
			[OnSuccess, OnError, Scheduler](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bFinished)
			{
				ACCELBYTE_SERVICE_LOGGING_HTTP_RESPONSE(Request, Response, bFinished);
				if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
				{
					if (!HandleHttpResultOk(Response, TArray<uint8>(), OnSuccess))
					{
						OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidResponse), "Invalid JSON response", FJsonObject{});
					}
					return;
				}

				if (!bFinished)
				{
					OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::NetworkError), "Request not sent.", FJsonObject{});
					return;
				}

				// If response is nullptr then search the actual response in the cache
				if (!Response.IsValid() && Scheduler != nullptr)
				{
					FAccelByteHttpCacheItem* Cache = Scheduler->GetHttpCache().GetSerializedHttpCache(Request);
					if (Cache != nullptr && EHttpResponseCodes::IsOk(Cache->SerializableRequestAndResponse.ResponseCode))
					{
						auto ResponsePayloadByte = Cache->SerializableRequestAndResponse.ResponsePayload;
						HandleHttpResultOk(nullptr, ResponsePayloadByte, OnSuccess);
						return;
					}
				}

				int32 Code;
				FString Message;
				FJsonObject ErrorObject;
				HandleHttpCustomError(Request, Response, Code, Message, ErrorObject);
				OnError.ExecuteIfBound(Code, Message, ErrorObject);
			});
	}

	template<typename T>
	FHttpRequestCompleteDelegate CreateHttpResultHandler(const T& OnSuccess, const FOAuthErrorHandler& OnError, FHttpRetryScheduler* Scheduler = nullptr)
	{
		return FHttpRequestCompleteDelegate::CreateLambda(
			[OnSuccess, OnError, Scheduler](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bFinished)
			{
				ACCELBYTE_SERVICE_LOGGING_HTTP_RESPONSE(Request, Response, bFinished);
				FErrorOAuthInfo ErrorOauthInfo;
				if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
				{
					if (!HandleHttpResultOk(Response, TArray<uint8>(), OnSuccess))
					{
						OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidResponse), TEXT("Invalid JSON response"), ErrorOauthInfo);
					}
					return;
				}

				if (!bFinished)
				{
	                OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::NetworkError), TEXT("Request not sent."), ErrorOauthInfo);
	                return;
	            }

				// If response is nullptr then search the actual response in the cache
				if (!Response.IsValid() && Scheduler != nullptr)
				{
					FAccelByteHttpCacheItem* Cache = Scheduler->GetHttpCache().GetSerializedHttpCache(Request);
					if (Cache != nullptr && EHttpResponseCodes::IsOk(Cache->SerializableRequestAndResponse.ResponseCode))
					{
						auto ResponsePayloadByte = Cache->SerializableRequestAndResponse.ResponsePayload;
						HandleHttpResultOk(nullptr, ResponsePayloadByte, OnSuccess);
						return;
					}
				}

				int32 ErrorCode;
				FString ErrorMessage;
				HandleHttpOAuthError(Request, Response, ErrorCode, ErrorMessage, ErrorOauthInfo);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorOauthInfo);
			});
	}

	template<typename T, typename U>
	FHttpRequestCompleteDelegate CreateHttpResultHandler(const T& OnSuccess, const U& OnError, FHttpRetryScheduler* Scheduler = nullptr)
	{
		return FHttpRequestCompleteDelegate::CreateLambda(
			[OnSuccess, OnError, Scheduler]
		(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bFinished)
		{
			ACCELBYTE_SERVICE_LOGGING_HTTP_RESPONSE(Request, Response, bFinished);

			if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				if (!HandleHttpResultOk(Response, TArray<uint8>(), OnSuccess))
				{
					OnError.ExecuteIfBound({ TEXT("InvalidResponse"), TEXT("Invalid JSON response") });
				}
				return;
			}

			if (!bFinished)
			{
				OnError.ExecuteIfBound({ TEXT("NetworkError"), TEXT("Request not sent.") });
				return;
			}

			// If response is nullptr then search the actual response in the cache
			if (!Response.IsValid() && Scheduler != nullptr)
			{
				FAccelByteHttpCacheItem* Cache = Scheduler->GetHttpCache().GetSerializedHttpCache(Request);
				if (Cache != nullptr && EHttpResponseCodes::IsOk(Cache->SerializableRequestAndResponse.ResponseCode))
				{ 
					auto ResponsePayloadByte = Cache->SerializableRequestAndResponse.ResponsePayload;
					HandleHttpResultOk(nullptr, ResponsePayloadByte, OnSuccess);
					return;
				}
			}

			HandleHttpError(Request, Response, OnError); 
		});
	}

	template<typename T>
	FHttpRequestCompleteDelegate CreateHttpResultHandler(const T& OnSuccess, const FCreateMatchmakingTicketErrorHandler& OnError, FHttpRetryScheduler* Scheduler = nullptr)
	{
		return FHttpRequestCompleteDelegate::CreateLambda(
			[OnSuccess, OnError, Scheduler](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bFinished)
			{
				ACCELBYTE_SERVICE_LOGGING_HTTP_RESPONSE(Request, Response, bFinished);

				FErrorCreateMatchmakingTicketV2 ErrorCreateMatchmakingV2Info;
				if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
				{
					if (!HandleHttpResultOk(Response, TArray<uint8>(), OnSuccess))
					{
						OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidResponse), TEXT("Invalid JSON response"), ErrorCreateMatchmakingV2Info);
					}
					return;
				}

				if (!bFinished)
				{
					OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::NetworkError), TEXT("Request not sent."), ErrorCreateMatchmakingV2Info);
					return;
				}

				// If response is nullptr then search the actual response in the cache
				if (!Response.IsValid() && Scheduler != nullptr)
				{
					FAccelByteHttpCacheItem* Cache = Scheduler->GetHttpCache().GetSerializedHttpCache(Request);
					if (Cache != nullptr && EHttpResponseCodes::IsOk(Cache->SerializableRequestAndResponse.ResponseCode))
					{
						auto ResponsePayloadByte = Cache->SerializableRequestAndResponse.ResponsePayload;
						HandleHttpResultOk(nullptr, ResponsePayloadByte, OnSuccess);
						return;
					}
				}

				int32 ErrorCode;
				FString ErrorMessage;
				HandleHttpCreateMatchmakingTicketError(Request, Response, ErrorCode, ErrorMessage, ErrorCreateMatchmakingV2Info);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorCreateMatchmakingV2Info);
			});
	}
	
} // Namespace AccelByte
