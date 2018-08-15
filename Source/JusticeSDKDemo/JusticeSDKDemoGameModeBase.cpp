// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeSDKDemoGameModeBase.h"
#include "Misc/ConfigCacheIni.h"


AJusticeSDKDemoGameModeBase::AJusticeSDKDemoGameModeBase()
{
}

void AJusticeSDKDemoGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void AJusticeSDKDemoGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	JusticeIdentity::ClientLogin();

	FTelemetryEventDataExample TelemetryData;
	TelemetryData.ExampleField1 = 256;
	TelemetryData.ExampleField2 = "512";
	TelemetryData.ExampleField3 = 1024;
	FString Data = TelemetryData.ToJson();

	FGuid Guid = FGuid::NewGuid();
	FString UUID = FString::Printf(TEXT("%x%x%x%x"), Guid.A, Guid.B, Guid.C, Guid.D);

	FTelemetryEvent Telemetry;
	Telemetry.AppID = 1555;
	Telemetry.ClientID = FJusticeSDKModule::Get().ClientID;
	Telemetry.Data = Data;
	Telemetry.DeviceID = FPlatformMisc::GetDeviceId();
	Telemetry.EventID = 1555;
	Telemetry.EventLevel = 1555;
	Telemetry.EventTime = FDateTime::Now().ToIso8601();
	Telemetry.EventType = 1555;
	Telemetry.UserID = "exampleUserID";
	Telemetry.UUID = UUID;
	Telemetry.UX = 1555;
	
	JusticeTelemetry::SendTelemetryEvent(Telemetry);
}

void AJusticeSDKDemoGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	JusticeIdentity::ClientLogout();
}

void AJusticeSDKDemoGameModeBase::Tick(float DeltaSeconds)
{
}


