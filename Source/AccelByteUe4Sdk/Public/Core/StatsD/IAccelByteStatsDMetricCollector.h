// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once
#include "CoreMinimal.h"

namespace AccelByte
{
	class IAccelByteStatsDMetricCollector
	{
	public:
		virtual ~IAccelByteStatsDMetricCollector() = default;

		//Basic Metrics
		virtual int32 GetPlayerCapacity() = 0;
		virtual int32 GetPlayerCount() = 0;
		virtual int32 GetClientCount() = 0;
		virtual int32 GetAiCount() = 0;

		//Performance Metrics
		virtual double GetFrameTimeAverage() = 0;
		virtual double GetFrameTimeMax() = 0;
		virtual double GetFrameStartDelayAverage() = 0;
		virtual double GetFrameStartDelayMax() = 0;
	};
}