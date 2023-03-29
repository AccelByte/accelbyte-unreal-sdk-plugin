// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Core/StatsD/IAccelByteStatsDMetricCollector.h"
#include "Models/AccelByteMetricModels.h"
#pragma once

namespace AccelByte
{
	class FAccelByteStatsDMetricCollector : public IAccelByteStatsDMetricCollector
	{
	public:
		~FAccelByteStatsDMetricCollector();

		//Basic Metrics
		int32 GetPlayerCapacity() override;
		int32 GetPlayerCount() override;
		int32 GetClientCount() override;
		int32 GetAiCount() override;

		//Performance Metrics
		double GetFrameTimeAverage() override;
		double GetFrameTimeMax() override;
		double GetFrameStartDelayAverage() override;
		double GetFrameStartDelayMax() override;
	};
}