// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/Ping/AccelBytePing.h"

constexpr int DefaultPingNum = 3;

void SendIcmpPingImpl(FString const& Address, int32 Port, float Timeout, int32 PingNum, FPingCompleteDelegate const& OnPingCompleteDelegate)
{
	TArray<FIcmpTarget> Targets;
	for (int i = 0; i < PingNum; ++i)
	{
		Targets.Add({ Address, Port });
	}

	FUDPPing::UDPEchoMany(Targets, Timeout, FIcmpEchoManyCompleteDelegate::CreateLambda(
		[OnPingCompleteDelegate](FIcmpEchoManyCompleteResult const& FinalResult)
		{
			FPingResult PingResult{};
			PingResult.Status = FinalResult.Status;

			if (FinalResult.AllResults.Num() > 0)
			{
				PingResult.AverageRoundTrip = FinalResult.AllResults[0].EchoResult.Time;
				PingResult.MinimumRoundTrip = FinalResult.AllResults[0].EchoResult.Time;
				PingResult.MaximumRoundTrip = FinalResult.AllResults[0].EchoResult.Time;

				for (auto const& Result : FinalResult.AllResults)
				{
					if (Result.EchoResult.Status == EIcmpResponseStatus::Success)
					{
						PingResult.AverageRoundTrip += Result.EchoResult.Time;
						PingResult.Sent++;

						if (Result.EchoResult.Time > PingResult.MaximumRoundTrip)
						{
							PingResult.MaximumRoundTrip = Result.EchoResult.Time;
						}
						if (Result.EchoResult.Time < PingResult.MinimumRoundTrip)
						{
							PingResult.MinimumRoundTrip = Result.EchoResult.Time;
						}
					}
					else
					{
						PingResult.Lost++;
					}
				}

				if (PingResult.Sent > 0)
				{
					PingResult.AverageRoundTrip /= PingResult.Sent;
				}
			}

			OnPingCompleteDelegate.ExecuteIfBound(PingResult);
		}));
}

void FAccelBytePing::SendIcmpPing(FPingConfig const& Config, FPingCompleteDelegate const& OnPingCompleteDelegate)
{
	SendIcmpPingImpl(Config.Address, Config.Port, Config.Timeout, Config.PingNum, OnPingCompleteDelegate);
}

void FAccelBytePing::SendIcmpPing(FString const& Address, int32 Port, float Timeout, FPingCompleteDelegate const& OnPingCompleteDelegate)
{
	SendIcmpPingImpl(Address, Port, Timeout, DefaultPingNum, OnPingCompleteDelegate);
}