// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

#include "Async/Async.h"
#include "Async/Future.h"
#include "Containers/Ticker.h"
#include "HAL/Runnable.h"
#include "HAL/ThreadSafeBool.h"
#include "Icmp.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Core/AccelByteDefines.h"
#include "Core/Ping/AccelBytePing.h"

using FPingResultStatus = EIcmpEchoManyStatus;
using FPingResponseStatus = EIcmpResponseStatus;

struct FPingResponse
{
	FString DataSent;
	FPingResponseStatus Status;
	float RoundTripTime;

	FPingResponse()
	{
		DataSent = TEXT("");
		Status = FPingResponseStatus::InternalError;
		RoundTripTime = 0.0f;
	}
};

DECLARE_DELEGATE_OneParam(FGotResponseDelegate, FPingResponse);
DECLARE_DELEGATE_OneParam(FPingResultStatusDelegate, FPingResultStatus);

class ACCELBYTEUE4SDK_API FAccelByteUdpPingThread : public FRunnable
{
public:
	FAccelByteUdpPingThread(ISocketSubsystem* const InSocketSub
		, FPingConfig const& InConfig
		, FGotResponseDelegate OnGotResponseDelegate
		, FPingResultStatusDelegate OnPingCompleteDelegate);

	virtual ~FAccelByteUdpPingThread() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

protected:
	virtual bool CreateSocket();
	virtual void DestroySocket();
	virtual uint64 GetCurrentTime();
	virtual bool CreateAddress();
	virtual bool SocketReceiveData(uint8* Data, int32 BufferSize, int32& BytesRead);
	virtual bool SocketSendData(const FString& StringToSend, int32& BytesSent);
	virtual bool SocketWaitData();
	virtual bool SocketIsValid();

	FPingResultStatus SendPing(const FString& PingString);

	FPingResultStatusDelegate OnComplete;
	FGotResponseDelegate OnGotResponse;
	FPingConfig Config;

private:
	FPingResponse CreateResponse(uint8* Data, int32 BytesRead, uint64 SentTime, uint64 CurrentTime);
	ISocketSubsystem* SocketSubsystem;
	FSocket* Socket;
	TSharedPtr<FInternetAddr> IpAddress;

	bool bIsComplete;
	bool bIsCanceled;
};

class ACCELBYTEUE4SDK_API FAccelByteUdpPingAsync: public FTickerObjectBaseAlias
{
public:
	FAccelByteUdpPingAsync(ISocketSubsystem* const InSocketSub, const FPingCompleteDelegate& InCompletionDelegate);

	virtual ~FAccelByteUdpPingAsync() override;

	virtual void Start(const FPingConfig& Config);

protected:
	virtual bool Tick(float DeltaTime) override;
	static FPingResult PingResponsesToResult(const TArray<FPingResponse>& PingResponses, FPingResultStatus EndStatus);

	ISocketSubsystem* SocketSubsystem;
	FPingCompleteDelegate OnPingCompleteDelegate;
	TFuture<FPingResult> FutureResult;
	FThreadSafeBool bThreadCompleted;

private:
	static FPingResult RunWorker(ISocketSubsystem* const SocketSub, FPingConfig const& Config);
};