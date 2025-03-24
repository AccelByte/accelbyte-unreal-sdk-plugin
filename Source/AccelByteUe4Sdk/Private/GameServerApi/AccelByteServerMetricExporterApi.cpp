// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerMetricExporterApi.h"
#include "Core/AccelByteCredentials.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/StatsD/AccelByteStatsDMetricBuilder.h"
#include "Core/StatsD/AccelByteStatsDMetricCollector.h"
#include "IPAddress.h"
#include "Engine/GameEngine.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteMetricExporter, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteMetricExporter);

namespace AccelByte
{
	namespace GameServerApi
	{
		ServerMetricExporter::ServerMetricExporter(ServerSettings const& InServerSettingsRef
			, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient)
			: ServerSettingsRef(InServerSettingsRef)
			, ServerApiClient(InServerApiClient)
		{
			StatsDMetricCollector = MakeShared<FAccelByteStatsDMetricCollector>();
		}

		ServerMetricExporter::~ServerMetricExporter()
		{
			StopExporting();
		}

		void ServerMetricExporter::SetServerApiClient(
			TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient)
		{
			ServerApiClient = InServerApiClient;
		}

		void ServerMetricExporter::Initialize()
		{
			return Initialize(ServerSettingsRef.StatsDServerUrl, ServerSettingsRef.StatsDServerPort, ServerSettingsRef.StatsDMetricInterval);
		}

		void ServerMetricExporter::Initialize(const FString& InAddress, uint16 Port, uint32 IntervalSeconds)
		{
			if (InAddress.Equals("localhost"))
			{
				Address = FIPv4Address::InternalLoopback;
			}
			else if (!FIPv4Address::Parse(InAddress, Address))
			{
				UE_LOG(LogAccelByteMetricExporter, Warning, TEXT("Invalid IPv4 Address"));
				return;
			}

			return Initialize(Address, Port, IntervalSeconds);
		}

		void ServerMetricExporter::Initialize(const FIPv4Address& InAddress, uint16 Port, uint32 IntervalSeconds)
		{
			Endpoint = FIPv4Endpoint(InAddress, Port);
			Socket = MakeShareable<FSocket>(
				FUdpSocketBuilder(SocketDescription)
				.BoundToAddress(FIPv4Address::Any)
				.AsNonBlocking()
				.AsReusable()
				.WithSendBufferSize(SendBufferSize)
				);

			if (!Socket.IsValid())
			{
				UE_LOG(LogAccelByteMetricExporter, Warning, TEXT("Socket is invalid"));
				return;
			}

			StartExporting(IntervalSeconds);
		}

		void ServerMetricExporter::SetIntervalSeconds(uint32 IntervalSeconds)
		{
			StopExporting();
			StartExporting(IntervalSeconds);
		}

		void ServerMetricExporter::SetLabel(const FString& Key, const FString& Label)
		{
			MetricLabel.AddUnique(Key, Label);
		}

		void ServerMetricExporter::SetSendBufferSize(int32 BufferSize)
		{
			SendBufferSize = BufferSize;
		}

		int32 ServerMetricExporter::GetSendBufferSize() const
		{
			return SendBufferSize;
		}

		void ServerMetricExporter::GetLabels(const FString& Key, TArray<FString>& OutLabels)
		{
			MetricLabel.MultiFind(Key, OutLabels, true);
		}

		void ServerMetricExporter::EnqueueMetric(const FString& Key, double Value)
		{
			EnqueueMetric(Key, FString::SanitizeFloat(Value));
		}

		void ServerMetricExporter::EnqueueMetric(const FString& Key, int32 Value)
		{
			EnqueueMetric(Key, FString::FromInt(Value));
		}

		void ServerMetricExporter::EnqueueMetric(const FString& Key, const FString& Value)
		{
			FAccelByteStatsDMetricBuilder Builder(Key, Value);
			TArray<FString> Labels;
			MetricLabel.MultiFind(Key, Labels, true);
			for (int Index = 0; Index < Labels.Num(); Index++)
			{
				Builder.AddTag(Labels[Index]);
			}
			FString Metric = Builder.Build();

			if (MultiMetricPacket.Len() + Metric.Len() < SendBufferSize)
			{
				if (MultiMetricPacket.Len() > 0)
				{
					MultiMetricPacket.Append(TEXT("\n"));
				}
				MultiMetricPacket.Append(Metric);
			}
			else
			{
				MetricQueue.Enqueue(MakeShared<FString>(MultiMetricPacket));
				MultiMetricPacket = Metric;
			}
		}

		void ServerMetricExporter::SetOptionalMetricsEnabled(bool Enable)
		{
			bOptionalMetricsEnabled = Enable;
		}

		void ServerMetricExporter::SetStatsDMetricCollector(const TSharedPtr<IAccelByteStatsDMetricCollector>& Collector)
		{
			StatsDMetricCollector = Collector;
		}

		void ServerMetricExporter::CollectMetrics()
		{
			if (!StatsDMetricCollector.IsValid())
			{
				return;
			}
			//Basic Metrics
			EnqueueMetric("PlayerCapacity", StatsDMetricCollector->GetPlayerCapacity());
			EnqueueMetric("PlayerCount", StatsDMetricCollector->GetPlayerCount());
			EnqueueMetric("ClientCount", StatsDMetricCollector->GetClientCount());
			EnqueueMetric("AiCount", StatsDMetricCollector->GetAiCount());

			//Performance Metrics
			EnqueueMetric("FrameTimeAverage", StatsDMetricCollector->GetFrameTimeAverage());
			EnqueueMetric("FrameTimeMax", StatsDMetricCollector->GetFrameTimeMax());
			EnqueueMetric("FameStartDelayAverage", StatsDMetricCollector->GetFrameStartDelayAverage());
			EnqueueMetric("FrameStartDelayMax", StatsDMetricCollector->GetFrameStartDelayMax());
		}

		bool ServerMetricExporter::ExportMetrics(float DeltaTime)
		{
			if (!MultiMetricPacket.IsEmpty())
			{
				MetricQueue.Enqueue(MakeShared<FString>(MultiMetricPacket));
				MultiMetricPacket = TEXT("");
			}

			TSharedPtr<FString> DequeueResult;
			while (MetricQueue.Dequeue(DequeueResult))
			{
				int32 BytesSent;
				Socket->SendTo((uint8*)TCHAR_TO_UTF8(**DequeueResult), DequeueResult->Len(), BytesSent, *Endpoint.ToInternetAddrIPV4());
			}

			if (StatsDMetricCollector.IsValid() && bOptionalMetricsEnabled)
			{
				CollectMetrics();
			}
			return true;
		}

		void ServerMetricExporter::StartExporting(uint32 IntervalSeconds)
		{
			MetricInterval = FTimespan::FromSeconds(IntervalSeconds);
			MetricExporterTickDelegate = FTickerDelegate::CreateRaw(this, &ServerMetricExporter::ExportMetrics);
			MetricExporterTickDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(MetricExporterTickDelegate, static_cast<float>(MetricInterval.GetTotalSeconds()));
		}

		void ServerMetricExporter::StopExporting()
		{
			if (MetricExporterTickDelegateHandle.IsValid())
			{
				FTickerAlias::GetCoreTicker().RemoveTicker(MetricExporterTickDelegateHandle);
			}
		}
	}
}