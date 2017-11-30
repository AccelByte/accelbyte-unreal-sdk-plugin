// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#include "OpenTracingJustice.h"
#include "OnlineSubsystemJusticeModule.h"

FString FOpenTracingJustice::XRayTraceIDStr() const {
	int version = 1;
	int UnixTime = FDateTime::UtcNow().ToUnixTimestamp();
	FGuid Guid = FGuid::NewGuid();
	FString Identifier = FString::Printf(TEXT("%x%x%x%x"), Guid.A, Guid.B, Guid.C, Guid.D);
	return FString::Printf(TEXT("Root=<%i-%i-%s>"), version, UnixTime, *Identifier);
}