// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/Misc/Guid.h"

class FOpenTracingJustice
{
	
public:

	FOpenTracingJustice()
	{
		TraceId = FGuid::NewGuid();
	}

	~FOpenTracingJustice()
	{ }

	bool IsValid() const { return TraceId.IsValid(); };
	FGuid GetTraceId() const { return TraceId;  };
	FString XRayTraceIDStr() const;

private:
	
	FGuid TraceId;
	FGuid SpanId;
	FGuid ParentSpanId;
};
