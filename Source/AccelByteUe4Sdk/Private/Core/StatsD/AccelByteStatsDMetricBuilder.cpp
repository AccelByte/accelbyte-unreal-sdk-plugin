// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/StatsD/AccelByteStatsDMetricBuilder.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{
	FAccelByteStatsDMetricBuilder::FAccelByteStatsDMetricBuilder(const FString& InName, const FString& InValue)
	{
		Metric.Name = InName;
		Metric.Value = InValue;
	}

	FAccelByteStatsDMetricBuilder& FAccelByteStatsDMetricBuilder::AddTag(const FString& TagValue)
	{		
		if(!TagValue.IsEmpty())
		{
			Metric.Tags.AddUnique(TagValue);
		}
		return *this;
	}

	FString FAccelByteStatsDMetricBuilder::Build()
	{
		TStringBuilder<BufferSize> StringBuilder;
		StringBuilder.Append(Metric.Name).Append(TEXT(":")).Append(Metric.Value).Append(TEXT("|g"));
		if (Metric.Tags.Num() > 0)
		{
			StringBuilder.Append(TEXT("|#"));
			
			for (int Index = 0; Index < Metric.Tags.Num(); Index++)
			{
				FString TagName = FString::Format(TEXT("Tag{0}"), { TagIndex++ });
				StringBuilder.Append(TagName).Append(TEXT(":")).Append(Metric.Tags[Index]);
				if (Index + 1 != Metric.Tags.Num())
				{
					StringBuilder.Append(TEXT(","));
				}
			}
		}
		FString Result = StringBuilder.ToString();
		
		return Result;
	}
}