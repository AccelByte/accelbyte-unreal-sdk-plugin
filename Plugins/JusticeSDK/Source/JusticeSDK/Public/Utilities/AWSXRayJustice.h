// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/Misc/Guid.h"

class FAWSXRayJustice
{
public:
	FAWSXRayJustice()
	{}

	~FAWSXRayJustice()
	{ }
	
	static inline FString XRayTraceID() {
		FGuid TraceId = FGuid::NewGuid();
		const int32 version = 1;
		int64 UnixTime = FDateTime::UtcNow().ToUnixTimestamp();
		FString TraceID = FString::Printf(TEXT("%x%x%x%x"), TraceId.A, TraceId.B, TraceId.C, TraceId.D).Left(24);
		return FString::Printf(TEXT("%i-%X-%s"), version, UnixTime, *TraceID);
	}
};