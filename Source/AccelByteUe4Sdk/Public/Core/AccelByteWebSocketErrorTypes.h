// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

namespace AccelByte
{
	UENUM(EBlueprintType)
	enum class EWebsocketClosureCodeForSpecificRetry : int32
	{
		None = 0,
		ClosedAbnormally = 1006,
		DisconnectServerShutdown = 4000
	};

	/// <summary>
	/// https://www.rfc-editor.org/rfc/rfc6455#section-7.4.1
	/// </summary>
	UENUM(BlueprintType)
	enum class EWebsocketErrorTypes : int32
	{
		Default = 0,
		NormalClosure = 1000,
		EndpointUnavailable = 1001,
		ProtocolError = 1002,
		InvalidMessageType = 1003,
		LocalFutureUsageCode = 1004,
		LocalNoStatusCodePresent = 1005,
		LocalClosedAbnormally = 1006,
		InvalidPayloadData = 1007,
		PolicyViolation = 1008,
		MessageTooBig = 1009,
		MandatoryExtension = 1010,
		InternalServerError = 1011,
		LocalHandshakeFailure = 1015,

		//RESERVED 0-999
		//Status codes in the range 0 - 999 are not used.
		
		//RESERVED 1000-2999
		//Status codes in the range 1000 - 2999 are reserved for definition by
		//this protocol, its future revisions, and extensions specified in a
		//permanentand readily available public specification.
		
		//3000-3999
		DisconnectSenderBrokenPipe = 3101,
		DisconnectSenderUnhandledError = 3102,
		DisconnectSenderUnableToPing = 3103,
		DisconnectReaderIOTimeout = 3120,
		DisconnectReaderUnexpectedEOF = 3121,
		DisconnectReaderUnhandledCloseError = 3122,
		

		//RESERVED 4000-4999
		DisconnectServerShutdown = 4000,
		DisconnectTokenRevokedCode = 4020,
		DisconnectDueToIAMLoggedOut = 4021,
		DisconnectDueToIAMDisconnect = 4022,
		DisconnectDueToFastReconnect = 4040,
		DisconnectDueToMultipleSessions = 4041,
		DisconnectDueCCULimiter = 4060,
		DisconnectUnknownError = 4099,
		OutsideBoundaryOfDisconnection = 4400,

		DisconnectFromExternalReconnect = 4401,

		//WILL BE DEPRECATED
		//Translated & split into 4021-4041
		DisconnectCloseCode = 4003,
		//Translated & split into 3101-3103
		DisconnectSenderError = 4004,
		//WILL BE DEPRECATED
		//Translated & split into 3120-3121
		DisconnectReaderClosed = 4005,
		//WILL BE DEPRECATED
		//Translated into the 3122
		DisconnectReaderClosedAbnormal = 4006,
		//WILL BE DEPRECATED
		DisconnectReaderError = 4007
	};
}
