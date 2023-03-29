// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "Networking.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/StatsD/IAccelByteStatsDMetricCollector.h"
#include "Models/AccelByteMetricModels.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;
namespace GameServerApi
{
	class ACCELBYTEUE4SDK_API ServerMetricExporter
	{
	public:
		ServerMetricExporter(ServerSettings const& InServerSettingsRef);
		virtual ~ServerMetricExporter();

		/**
		 * @brief Initialize Metric Exporter with default value.
		 */
		void Initialize();

		/**
		 * @brief Initialize Metric Exporter.
		 * @param InAddress StatsD string address
		 * @param Port StatsD port
		 * @param IntervalSeconds StatsD
		 */
		void Initialize(const FString& InAddress, uint16 Port, uint32 IntervalSeconds = 60);

		/**
		 * @brief Initialize Metric Exporter.
		 * @param InAddress StatsD IPv4 address
		 * @param Port StatsD port
		 * @param IntervalSeconds StatsD
		 */
		void Initialize(const FIPv4Address& InAddress, uint16 Port, uint32 IntervalSeconds = 60);

		/**
		 * @brief Set Metric Exporter interval
		 * @param IntervalSeconds Interval to set
		 */
		void SetIntervalSeconds(uint32 IntervalSeconds);

		/**
		 * @brief Set Label to a specific Key of metric
		 * @param Key Key to add label
		 * @param Value label name for the key
		 */
		void SetLabel(const FString& Key, const FString& Value);

		/**
		 * @brief Set send buffer size for UDP Client
		 * @param BufferSize Buffer size for UDP Client
		 */
		void SetSendBufferSize(int32 BufferSize);

		/**
		 * @brief Get buffer sizo for UDP Client
		 */
		int32 GetSendBufferSize() const;

		/**
		 * @brief Get tagged labels of specific key.
		 * @param Key The key of the Metric
		 * @param OutLabel The tagged labels of specific key
		 */
		void GetLabels(const FString& Key, TArray<FString>& OutLabel);

		/**
		 * @brief Enqueue Metric
		 * @param Key The key of the metric
		 * @param Value Floating number value of the metric
		 */
		void EnqueueMetric(const FString& Key, double Value);

		/**
		 * @brief Enqueue Metric
		 * @param Key The key of the metric
		 * @param Value Integer value of the metric
		 */
		void EnqueueMetric(const FString& Key, int32 Value);

		/**
		 * @brief Enqueue Metric
		 * @param Key The key of the metric
		 * @param Value String value of the metric
		 */
		void EnqueueMetric(const FString& Key, const FString& Value);

		/**
		 * @brief Set Sending optional metrics or not
		 * @param Enable
		 */
		void SetOptionalMetricsEnabled(bool Enable);

		/**
		 * @brief Set the StatsD Metric Collector.
		 * By default it will use AccelByteStatsDMetricCollector class.
		 * Should be set if custom collector is needed.
		 *
		 * @param Collecetor The collector object inherited from IAccelByteStatsDMetricCollector
		 */
		void SetStatsDMetricCollector(const TSharedPtr<IAccelByteStatsDMetricCollector>& Collector);

		/**
		 * @brief Collect Basic and Performance Metric and enqueue them.
		 */
		void CollectMetrics();

	protected:
		virtual bool ExportMetrics(float DeltaTime);
		TQueue<TSharedPtr<FString>> MetricQueue;
		FString MultiMetricPacket{};

	private:
		ServerMetricExporter() = delete;
		ServerMetricExporter(ServerMetricExporter const&) = delete;
		ServerMetricExporter(ServerMetricExporter&&) = delete;

		void StartExporting(uint32 IntervalSeconds);
		void StopExporting();

		ServerSettings const& ServerSettingsRef;
		TSharedPtr<IAccelByteStatsDMetricCollector> StatsDMetricCollector;
		TSharedPtr<FSocket> Socket;
		FTickerDelegate MetricExporterTickDelegate;
		FDelegateHandleAlias MetricExporterTickDelegateHandle;

		TMultiMap<FString /*Key*/, FString /*Value*/> MetricLabel;
		FIPv4Endpoint Endpoint;
		FTimespan MetricInterval = FTimespan(0, 1, 0);
		FIPv4Address Address;
		FString MetricUrl;
		const FString SocketDescription = "Metric Exporter";
		int32 SendBufferSize = 1 << 16;
		bool bOptionalMetricsEnabled = true;
	};
}
}