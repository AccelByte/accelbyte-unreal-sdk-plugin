// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Core/Public/Misc/Guid.h"
#include "UnrealString.h"

class FAWSXRayJustice
{
public:
	FAWSXRayJustice()
	{
		TraceId = FGuid::NewGuid();
	}

	~FAWSXRayJustice()
	{ }

	bool IsValid() const { return TraceId.IsValid(); };
	FGuid GetTraceId() const { return TraceId; };
	
	FString XRayTraceID() const {
		const int version = 1;
		int64 UnixTime = FDateTime::UtcNow().ToUnixTimestamp();
		FString TraceID = FString::Printf(TEXT("%x%x%x%x"), TraceId.A, TraceId.B, TraceId.C, TraceId.D).Left(24);
		return FString::Printf(TEXT("%i-%X-%s"), version, UnixTime, *TraceID);
	}

	FString ToString() const {	
		return FString::Printf(TEXT("Root=%s"), *XRayTraceID());
	}


private:

	FGuid TraceId;

};