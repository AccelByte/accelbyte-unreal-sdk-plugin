// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteTurnManagerApi.h"

#include "Icmp.h"

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteReport.h"
#include "Core/Ping/AccelBytePing.h"

namespace AccelByte
{
namespace Api
{
FAccelByteModelsTurnServerList TurnManager::TurnServers = {};
TArray<TPair<FString, float>> TurnManager::Latencies = {};

TurnManager::TurnManager(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
	, FastestPing{.0f}
	, Counter{0}
{}

TurnManager::~TurnManager()
{}

FAccelByteTaskWPtr TurnManager::GetTurnServers(THandler<FAccelByteModelsTurnServerList> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/turn"), *GetTurnManagerServerUrl());

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr TurnManager::GetTurnServersV2(THandler<FAccelByteModelsTurnServerList> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/turn"), *GetTurnManagerServerUrl());

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr TurnManager::GetClosestTurnServer(const THandler<FAccelByteModelsTurnServer>& OnSuccess
	, FErrorHandler const& OnError)
{
	TurnManagerWPtr TurnManagerWeak = AsShared();
	return GetTurnServers(THandler<FAccelByteModelsTurnServerList>::CreateLambda(
			[TurnManagerWeak, OnError, OnSuccess](const FAccelByteModelsTurnServerList& Result)
			{
				if(Result.Servers.Num() == 0)
				{
					OnError.ExecuteIfBound(400, TEXT("turn server is empty"));
				}
				else
				{
					const auto TurnManagerPtr = TurnManagerWeak.Pin();
					if(TurnManagerPtr.IsValid())
					{
						TurnManagerPtr->FastestPing = 1000;
						TurnManagerPtr->Counter = 0;
						for (int i=0;i< Result.Servers.Num();i++)
						{
							auto Server = Result.Servers[i];
							int Count = Result.Servers.Num();
							FAccelBytePing::SendUdpPing(*Server.Ip, Server.Qos_port
								, 10.0f
								, FPingCompleteDelegate::CreateLambda(
									[TurnManagerWeak, Server, Count, OnSuccess, OnError](FPingResult PingResult)
									{
										const auto TurnManagerPtr = TurnManagerWeak.Pin();
										if (TurnManagerPtr.IsValid())
										{
											TurnManagerPtr->Counter++;
											if (TurnManagerPtr->FastestPing > PingResult.AverageRoundTrip)
											{
												TurnManagerPtr->FastestPing = PingResult.AverageRoundTrip;
												TurnManagerPtr->ClosestServer = Server;
											}
											if (TurnManagerPtr->Counter == Count)
											{
												OnSuccess.ExecuteIfBound(TurnManagerPtr->ClosestServer);
											}
										}
										else
										{
											OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("turnManager Api is invalid"));
										}
									})
							);
						}
					}
					else
					{
						OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("turnManager Api is invalid"));
					}
				}
			})
		, FErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
}

FAccelByteTaskWPtr TurnManager::GetClosestTurnServerV2(const THandler<FAccelByteModelsTurnServer>& OnSuccess
	, FErrorHandler const& OnError)
{
	TurnManagerWPtr TurnManagerWeak = AsShared();
	return GetTurnServersV2(THandler<FAccelByteModelsTurnServerList>::CreateLambda(
			[TurnManagerWeak, OnError, OnSuccess](const FAccelByteModelsTurnServerList& Result)
			{
				if(Result.Servers.Num() == 0)
				{
					OnError.ExecuteIfBound(400, TEXT("turn server is empty"));
				}
				else
				{
					const auto TurnManagerPtr = TurnManagerWeak.Pin();
					if (TurnManagerPtr.IsValid())
					{
						TurnManagerPtr->FastestPing = 1000;
						TurnManagerPtr->Counter = 0;
						for (int i = 0; i < Result.Servers.Num(); i++)
						{
							auto Server = Result.Servers[i];
							int Count = Result.Servers.Num();
							FAccelBytePing::SendUdpPing(*Server.Ip, Server.Qos_port
								, 10.0f
								, FPingCompleteDelegate::CreateLambda(
									[TurnManagerWeak, Server, Count, OnSuccess, OnError](FPingResult PingResult)
									{
										const auto TurnManagerPtr = TurnManagerWeak.Pin();
										if (TurnManagerPtr.IsValid())
										{
											TurnManagerPtr->Counter++;
											if (TurnManagerPtr->FastestPing > PingResult.AverageRoundTrip)
											{
												TurnManagerPtr->FastestPing = PingResult.AverageRoundTrip;
												TurnManagerPtr->ClosestServer = Server;
											}
											if (TurnManagerPtr->Counter == Count)
											{
												OnSuccess.ExecuteIfBound(TurnManagerPtr->ClosestServer);
											}
										}
										else
										{
											OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("turnManager Api is invalid"));
										}
									})
							);
						}
					}
					else
					{
						OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("turnManager Api is invalid"));
					}
				}
			})
		, FErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
}

FAccelByteTaskWPtr TurnManager::GetTurnServerLatencyByRegion(const FString& Region,
	THandler<int32> const& OnSuccess, FErrorHandler const& OnError)
{
	return GetTurnServersV2(THandler<FAccelByteModelsTurnServerList>::CreateLambda(
			[OnError, OnSuccess, &Region](const FAccelByteModelsTurnServerList& Result)
			{
				if(Result.Servers.Num() == 0)
				{
					OnError.ExecuteIfBound(400, TEXT("Turn server is empty!"));
				}
				else
				{
					const FAccelByteModelsTurnServer* TurnServer = Result.Servers.FindByPredicate([&](const FAccelByteModelsTurnServer& TurnServer)
					{
						return TurnServer.Region == Region;
					});

					if (TurnServer != nullptr)
					{
						FAccelBytePing::SendUdpPing(*TurnServer->Ip, TurnServer->Qos_port
							, 10.0f
							, FPingCompleteDelegate::CreateLambda(
								[OnSuccess](FPingResult PingResult)
								{
									const int32 Milliseconds = FMath::RoundToInt(PingResult.AverageRoundTrip * 1000);
									OnSuccess.ExecuteIfBound(Milliseconds);
								})
						);
					}
					else
					{
						OnError.ExecuteIfBound(400, FString::Printf(TEXT("Turn server with region %s is not found!"), *Region));
					}
				}
			})
		, FErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
}

FAccelByteTaskWPtr TurnManager::GetTurnCredential(FString const& Region
                                                  , FString const& Ip
                                                  , int Port
                                                  , THandler<FAccelByteModelsTurnServerCredential> const& OnSuccess
                                                  , FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/turn/secret/%s/%s/%d")
		, *GetTurnManagerServerUrl()
		, *Region
		, *Ip
		, Port);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FString TurnManager::GetTurnManagerServerUrl() const
{
	return SettingsRef.TurnManagerServerUrl.IsEmpty()
		? FString::Printf(TEXT("%s/turnmanager"), *SettingsRef.BaseUrl)
		: SettingsRef.TurnManagerServerUrl;
}

FAccelByteTaskWPtr TurnManager::SendMetric(FString const& SelectedTurnServerRegion
		, EP2PConnectionType P2PConnectionType
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Latency)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/metrics/namespaces/%s/connected")
		, *GetTurnManagerServerUrl()
		, *FGenericPlatformHttp::UrlEncode(SettingsRef.Namespace));

	FAccelByteModelsTurnManagerMetric Data;
	Data.Region = SelectedTurnServerRegion;
	Data.Type = FAccelByteUtilities::GetUEnumValueAsString(P2PConnectionType).ToLower();
	if (Latency != INDEX_NONE)
	{
		Data.Latency = Latency;
	}
	
	FString Content = TEXT("");
	TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(Data);
	FAccelByteUtilities::RemoveEmptyFieldsFromJson(Json, FAccelByteUtilities::FieldRemovalFlagNumbersZeroValues);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content,  OnSuccess, OnError);
}

void TurnManager::GetTurnServerLatencies(const THandler<TArray<TPair<FString, float>>>& OnPingRegionsSuccess
	, const FErrorHandler& OnError)
{
	TurnManagerWPtr TurnManagerWeak = AsShared();
	GetTurnServersV2(THandler<FAccelByteModelsTurnServerList>::CreateLambda(
		[TurnManagerWeak, OnPingRegionsSuccess, OnError](const FAccelByteModelsTurnServerList Result)
		{
			TurnManager::TurnServers = Result; // Cache for the session

			const auto TurnManagerPtr = TurnManagerWeak.Pin();
			if (TurnManagerPtr.IsValid())
			{
				TurnManagerPtr->PingRegionsSetLatencies(TurnManager::TurnServers, OnPingRegionsSuccess, OnError);
			}
		}), OnError);
}

const TArray<TPair<FString, float>>& TurnManager::GetCachedLatencies()
{
	return TurnManager::Latencies;
}

void TurnManager::PingRegionsSetLatencies(const FAccelByteModelsTurnServerList& TurnServerList
                                          , const THandler<TArray<TPair<FString, float>>>& OnSuccess
                                          , const FErrorHandler& OnError) const
{
	TSharedRef<TArray<TPair<FString, float>>> SuccessLatencies = MakeShared<TArray<TPair<FString, float>>>();
	TSharedRef<TArray<FString>> FailedLatencies = MakeShared<TArray<FString>>();

	int32 Count = TurnServerList.Servers.Num();
	
	if (Count > 0)
	{
		// For each server, ping them and record add to Latency TArray.
		for (int count = 0; count < TurnServerList.Servers.Num(); count++)
		{
			auto Server = TurnServerList.Servers[count];
			FString Region = Server.Region;

			// Ping -> Get the latencies on pong.
			FAccelBytePing::SendUdpPing(Server.Ip, Server.Port, SettingsRef.QosPingTimeout, FPingCompleteDelegate::CreateLambda(
				[Count, SuccessLatencies, FailedLatencies, Region, OnSuccess, OnError](const FPingResult& PingResult)
				{
					if (PingResult.Status == FPingResultStatus::Success)
					{
						float PingDelay = PingResult.AverageRoundTrip * 1000; // convert to milliseconds
						SuccessLatencies->Add(TPair<FString, float>(Region, PingDelay));
					}
					else
					{
						FailedLatencies->Add(Region);
					}

					int TotalLatencies = SuccessLatencies->Num() + FailedLatencies->Num();
					if (Count == TotalLatencies)
					{
						TurnManager::Latencies.Empty();
						TurnManager::Latencies.Append(*SuccessLatencies);

						if (SuccessLatencies->Num() > 0)
						{
							OnSuccess.ExecuteIfBound(*SuccessLatencies);
						}
						else
						{
							OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to ping all servers"));
						}
					}
				}));
		}
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to ping because no QoS server"));
	}
}

} // Namespace Api
} // Namespace AccelByte
