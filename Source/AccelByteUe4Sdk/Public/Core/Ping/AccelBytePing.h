// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Icmp.h"

using FCompletePingResult = FIcmpEchoManyCompleteResult;
using FPingResultStatus = EIcmpEchoManyStatus;

struct FPingConfig
{
	FString Address;
	float Timeout;
	int32 Port;
	int PingNum;
};

struct FPingResult
{
	FPingResultStatus Status;
	float AverageRoundTrip;
	float MaximumRoundTrip;
	float MinimumRoundTrip;
	int Sent;
	int Received;
	int Lost;

};

typedef TFunction<void(FPingResult)> FPingCompleteCallback;
DECLARE_DELEGATE_OneParam(FPingCompleteDelegate, FPingResult);

class ACCELBYTEUE4SDK_API FAccelBytePing
{
public:
	static void SendIcmpPing(FPingConfig const& Config, FPingCompleteDelegate const& OnPingCompleteDelegate);
	static void SendIcmpPing(FString const& Address, int32 Port, float Timeout, FPingCompleteDelegate const& OnPingCompleteDelegate);
};