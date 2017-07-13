// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/Misc/Guid.h"

class FOpenTracingJustice
{
	
public:

	FOpenTracingJustice()
	{
	}

	FOpenTracingJustice(const FOpenTracingJustice& Parent)
	{
		if (Parent.IsValid())
		{
			TraceId  = Parent.GetTraceId();
			ParentId = Parent.GetSpanId();
		}
		else
		{
			TraceId = FGuid::NewGuid();
		}
		FGuid NewSpan = FGuid::NewGuid();
		SpanId = FGuid(0, 0, NewSpan[2], NewSpan[3]);
	}

	bool HasParent() const { return ParentId.IsValid(); };
	bool IsValid()   const { return TraceId.IsValid(); };
	
	FGuid GetTraceId()  const { return TraceId;  };
	FGuid GetSpanId()   const { return SpanId;   };
	FGuid GetParentId() const { return ParentId; };

	FString TraceIdToString()  const { return FString::Printf(TEXT("%08x%08x%08x%08x"), TraceId[0], TraceId[1], TraceId[2], TraceId[3]); };
	FString SpanIdToString()   const { return FString::Printf(TEXT("%08x%08x"), SpanId[2], SpanId[3]); };
	FString ParentIdToString() const { return FString::Printf(TEXT("%08x%08x"), ParentId[2], ParentId[3]); };

	FString ToString() const
	{
		return FString::Printf(TEXT("TraceId=%s SpanId=%s ParentId=%s"),
							   *TraceIdToString(), *SpanIdToString(), HasParent() ? *ParentIdToString() : TEXT("(root)"));
	};

	~FOpenTracingJustice()
	{
	}

private:
	
	FGuid TraceId;
	FGuid SpanId;
	FGuid ParentId;
};
