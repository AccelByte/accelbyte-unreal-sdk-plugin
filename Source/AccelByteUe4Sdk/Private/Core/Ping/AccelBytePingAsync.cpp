// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelBytePingAsync.h"
#include "Core/Ping/AccelBytePing.h"

FAccelByteUdpPingThread::FAccelByteUdpPingThread(ISocketSubsystem* const InSocketSub
	, const FPingConfig& InConfig
	, FGotResponseDelegate OnGotResponseDelegate
	, FPingResultStatusDelegate OnPingCompleteDelegate)
	: OnComplete(OnPingCompleteDelegate)
	, OnGotResponse(OnGotResponseDelegate)
	, Config(InConfig)
	, SocketSubsystem(InSocketSub)
	, bIsComplete(false)
	, bIsCanceled(false)
{
}

FAccelByteUdpPingThread::~FAccelByteUdpPingThread()
{
}

uint32 FAccelByteUdpPingThread::Run()
{
	FPingResultStatus PingResultStatus = FPingResultStatus::Invalid;
	if (!CreateAddress() || !CreateSocket())
	{
		OnComplete.ExecuteIfBound(PingResultStatus);
		return 0;
	}

	PingResultStatus = SendPing(TEXT("PING"));
	OnComplete.ExecuteIfBound(PingResultStatus);

	DestroySocket();
	return 0;
}

void FAccelByteUdpPingThread::Stop()
{
	bIsCanceled = true;
}

void FAccelByteUdpPingThread::Exit()
{

}

bool FAccelByteUdpPingThread::CreateSocket()
{
	if (!SocketSubsystem)
	{
		return false;
	}

	if (!IpAddress.IsValid())
	{
		return false;
	}

	Socket = SocketSubsystem->CreateSocket(NAME_DGram, TEXT("ABPing"), IpAddress->GetProtocolType());
	if (!SocketIsValid())
	{
		return false;
	}

	if (!Socket->SetNonBlocking(true))
	{
		SocketSubsystem->DestroySocket(Socket);
		return false;
	}

	return true;
}

void FAccelByteUdpPingThread::DestroySocket()
{
	if (SocketSubsystem && Socket)
	{
		SocketSubsystem->DestroySocket(Socket);
		Socket = nullptr;
	}
}

uint64 FAccelByteUdpPingThread::GetCurrentTime()
{
	return FPlatformTime::Cycles64();
}

bool FAccelByteUdpPingThread::CreateAddress()
{
	if (!SocketSubsystem)
	{
		return false;
	}

	IpAddress = SocketSubsystem->CreateInternetAddr();

	bool bIpStringValid{};
	IpAddress->SetIp(*Config.Address, bIpStringValid);

	if (!bIpStringValid)
	{
		return false;
	}

	if (!IpAddress->IsValid())
	{
		return false;
	}

	IpAddress->SetPort(Config.Port);
	return true;
}

bool FAccelByteUdpPingThread::SocketReceiveData(uint8* Data, int32 BufferSize, int32& BytesRead)
{
	if (!SocketIsValid())
	{
		return false;
	}

	TSharedRef<FInternetAddr> RecvAddr = SocketSubsystem->CreateInternetAddr();
	return Socket->RecvFrom(Data, BufferSize, BytesRead, *RecvAddr);
}

bool FAccelByteUdpPingThread::SocketSendData(const FString& StringToSend, int32& BytesSent)
{
	if (!SocketIsValid())
	{
		return false;
	}
	
	return Socket->SendTo((uint8*)TCHAR_TO_UTF8(*StringToSend), StringToSend.Len(), BytesSent, *IpAddress);
}

bool FAccelByteUdpPingThread::SocketWaitData()
{
	if (!SocketIsValid())
	{
		return false;
	}

	return Socket->Wait(ESocketWaitConditions::WaitForRead, FTimespan::FromSeconds(Config.Timeout));
}

bool FAccelByteUdpPingThread::SocketIsValid()
{
	return Socket != nullptr;
}

FPingResultStatus FAccelByteUdpPingThread::SendPing(const FString& PingString)
{
	for (int PingIndex = 0; PingIndex < Config.PingNum; ++PingIndex)
	{
		if (!SocketIsValid())
		{
			return FPingResultStatus::Invalid;
		}

		if (bIsCanceled)
		{
			return FPingResultStatus::Canceled;
		}

		int32 BytesSent;
		FPingResponse Response{};
		if (!SocketSendData(PingString, BytesSent))
		{
			Response.Status = FPingResponseStatus::Unreachable;
			OnGotResponse.ExecuteIfBound(Response);
			continue;
		}

		uint64 SentTime = GetCurrentTime();
		Response.DataSent = PingString.Left(BytesSent);
		if (SocketWaitData())
		{
			uint64 CurrentTime = GetCurrentTime();
			const uint8 ResultPacketSize = 4;
			uint8 ResultBuffer[ResultPacketSize + 1]; // expected "PONG" and taking accounts of null terminators 
			int32 BytesRead = 0;
			if (SocketReceiveData(ResultBuffer, ResultPacketSize, BytesRead))
			{
				// make sure buffer is null terminated
				ResultBuffer[ResultPacketSize] = 0;
				
				Response = CreateResponse(ResultBuffer, BytesRead, SentTime, CurrentTime);
			}
			else
			{
				Response.Status = FPingResponseStatus::Unreachable;
			}
		}
		else
		{
			Response.Status = FPingResponseStatus::Timeout;
		}

		OnGotResponse.ExecuteIfBound(Response);
	}

	return FPingResultStatus::Success;
}

FPingResponse FAccelByteUdpPingThread::CreateResponse(uint8* Data, int32 BytesRead, uint64 SentTime, uint64 CurrentTime)
{
	FPingResponse Response{};
	if (BytesRead > 3)
	{
		FString Received = UTF8_TO_TCHAR(Data);
		if (Received.StartsWith(TEXT("PONG")))
		{
			double DeltaTime = (CurrentTime - SentTime) * FPlatformTime::GetSecondsPerCycle64();
			Response.Status = FPingResponseStatus::Success;
			Response.RoundTripTime = DeltaTime;
		}
	}
	return Response;

}

FAccelByteUdpPingAsync::FAccelByteUdpPingAsync(ISocketSubsystem* const InSocketSub, const FPingCompleteDelegate& InCompletionDelegate)
	: SocketSubsystem(InSocketSub)
	, OnPingCompleteDelegate(InCompletionDelegate)
	, bThreadCompleted(false)
{}

FAccelByteUdpPingAsync::~FAccelByteUdpPingAsync()
{
	check(IsInGameThread());

	if (FutureResult.IsValid())
	{
		FutureResult.Wait();
	}
}

void FAccelByteUdpPingAsync::Start(const FPingConfig& Config)
{
	if (!SocketSubsystem)
	{
		bThreadCompleted = true;
		return;
	}

	bThreadCompleted = false;
	TFunction<FPingResult()> WorkerTask = [this, Config]()
	{
		auto Result = FAccelByteUdpPingAsync::RunWorker(SocketSubsystem, Config);
		bThreadCompleted = true;
		return Result;
	};

	FutureResult = AsyncThread(WorkerTask);
}

bool FAccelByteUdpPingAsync::Tick(float DeltaTime)
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_ABPing_Tick);

	if (bThreadCompleted)
	{
		FPingResult Result{};
		if (FutureResult.IsValid())
		{
			Result = FutureResult.Get();
		}

		OnPingCompleteDelegate.ExecuteIfBound(Result);

		delete this;
		return false;
	}

	return true;
}

FPingResult FAccelByteUdpPingAsync::RunWorker(ISocketSubsystem* const SocketSub, const FPingConfig& Config)
{
	FPingResult Result{};

	TArray<FPingResponse> Responses;
	FGotResponseDelegate OnGotResponse;
	OnGotResponse.BindLambda([&Responses](FPingResponse Result)
		{
			Responses.Add(Result);
		});

	FPingResultStatusDelegate OnCompleted;
	OnCompleted.BindLambda([&Result, &Responses](FPingResultStatus EndStatus)
		{
			Result = PingResponsesToResult(Responses, EndStatus);
		});

	FAccelByteUdpPingThread PingWorker(SocketSub, Config, OnGotResponse, OnCompleted);
	PingWorker.Init();
	PingWorker.Run();

	return Result;
}

FPingResult FAccelByteUdpPingAsync::PingResponsesToResult(const TArray<FPingResponse>& PingResponses, FPingResultStatus EndStatus)
{
	FPingResult PingResult{};
	PingResult.Status = EndStatus;
	if (PingResponses.Num() > 0)
	{
		PingResult.AverageRoundTrip = 0.0f;
		PingResult.MaximumRoundTrip = PingResponses[0].RoundTripTime;
		PingResult.MinimumRoundTrip = PingResponses[0].RoundTripTime;
		
		for (auto const& Response : PingResponses)
		{
			if (Response.Status == FPingResponseStatus::Success)
			{
				PingResult.AverageRoundTrip += Response.RoundTripTime;
				PingResult.Received++;

				if (Response.RoundTripTime > PingResult.MaximumRoundTrip)
				{
					PingResult.MaximumRoundTrip = Response.RoundTripTime;
				}
				if (Response.RoundTripTime < PingResult.MinimumRoundTrip)
				{
					PingResult.MinimumRoundTrip = Response.RoundTripTime;
				}
			}
			else
			{
				PingResult.Lost++;
			}

			PingResult.Sent++;
		}

		if (PingResult.Received > 0)
		{
			PingResult.AverageRoundTrip /= PingResult.Received;
		}
	}
	return PingResult;
}
