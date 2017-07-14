// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/Misc/Guid.h"

class FOpenTracingJustice
{
	
public:

	FOpenTracingJustice()
	{ }

	~FOpenTracingJustice()
	{}

	FOpenTracingJustice(const FOpenTracingJustice& Parent)
	{
		if (Parent.IsValid())
		{
			TraceId = Parent.GetTraceId();
			ParentSpanId = Parent.GetSpanId();
		}
		else
		{
			TraceId = FGuid::NewGuid();
			ParentSpanId = TraceId;
		}
		FGuid NewSpan = FGuid::NewGuid();
		SpanId = FGuid(0, 0, NewSpan[2], NewSpan[3]);
	}
	
	bool IsValid()   const { return TraceId.IsValid(); };

	FGuid GetTraceId()  const { return TraceId;  };
	FGuid GetSpanId()   const { return SpanId;   };
	FGuid GetParentSpanId()  const { return ParentSpanId; };
	FString GetTraceIdStr()  const { return FString::Printf(TEXT("%08x%08x%08x%08x"), TraceId[0], TraceId[1], TraceId[2], TraceId[3]); };
	FString GetSpanIdStr()   const { return FString::Printf(TEXT("%08x%08x"), SpanId[2], SpanId[3]); };
	FString GetParentSpanIdStr() const { return FString::Printf(TEXT("%08x%08x"), ParentSpanId[2], ParentSpanId[3]); };

	FString ToString() const
	{
		return FString::Printf(TEXT("TraceId=%s SpanId=%s ParentSpanId=%s"),
							   *GetTraceIdStr(), *GetSpanIdStr(), *GetParentSpanIdStr());
	};

private:
	
	FGuid TraceId;
	FGuid SpanId;
	FGuid ParentSpanId;
};
