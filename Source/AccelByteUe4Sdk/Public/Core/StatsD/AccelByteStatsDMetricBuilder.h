// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#include "Models/AccelByteMetricModels.h"
#pragma once

namespace AccelByte
{
	class  ACCELBYTEUE4SDK_API FAccelByteStatsDMetricBuilder
	{
	public:
		FAccelByteStatsDMetricBuilder(const FString& Name, const FString& Value);
		FAccelByteStatsDMetricBuilder& AddTag(const FString& TagValue);
		FString Build();
	private:
		FAccelByteStatsDMetricBuilder() = delete;
		FStatsDMetric Metric;
		static const int32 BufferSize = 1 << 16;
		int TagIndex = 0;
	};
}