// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Platform.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Core/Public/Misc/CoreMisc.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeBaseModel.h"
#include "TelemetryEvent.generated.h"


class TelemetryEvent : public FJsonSerializable
{
private:
	int32 AgentType;
public:
	int32 AppID;
	FString ClientID;
	FString Data;
	FString DeviceID;
	int32 EventID;
	int32 EventLevel;
	FString EventTime;
	int32 EventType;
	FString UUID;
	int32 UX;
	FString UserID;

	TelemetryEvent()
	{
		#if PLATFORM_DESKTOP && PLATFORM_64BITS
			if (!IsRunningDedicatedServer()) { AgentType = 20; }
		#endif
		#if PLATFORM_DESKTOP && PLATFORM_32BITS
			if (!IsRunningDedicatedServer()) { AgentType = 21; }
		#endif
		#if PLATFORM_XBOXONE
			AgentType = 31;
		#endif
		#if PLATFORM_PS4
			AgentType = 40;
		#endif
		#if PLATFORM_SWITCH
			AgentType = 50;
		#endif
		#if PLATFORM_LINUX && PLATFORM_64BITS
			if (IsRunningDedicatedServer()) { AgentType = 60; }
		#endif
		#if PLATFORM_LINUX && PLATFORM_32BITS
			if (IsRunningDedicatedServer()) { AgentType = 61; }
		#endif
		#if PLATFORM_WINDOWS && PLATFORM_64BITS
			if (IsRunningDedicatedServer()) { AgentType = 62; }
		#endif
		#if PLATFORM_WINDOWS && PLATFORM_32BITS
			if (IsRunningDedicatedServer()) { AgentType = 63; }
		#endif
	}

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("AgentType", AgentType);
		JSON_SERIALIZE("AppID", AppID);
		JSON_SERIALIZE("ClientID", ClientID);
		JSON_SERIALIZE("Data", Data);
		JSON_SERIALIZE("DeviceID", DeviceID);
		JSON_SERIALIZE("EventID", EventID);
		JSON_SERIALIZE("EventLevel", EventLevel);
		JSON_SERIALIZE("EventTime", EventTime);
		JSON_SERIALIZE("EventType", EventType);
		JSON_SERIALIZE("UUID", UUID);
		JSON_SERIALIZE("UX", UX);
		JSON_SERIALIZE("UserID", UserID);
	END_JSON_SERIALIZER
};

UCLASS()
class UTelemetryEvent : public UObject, public TelemetryEvent, public JusticeBaseModel<UTelemetryEvent, TelemetryEvent>
{
	GENERATED_BODY()

		UFUNCTION(BlueprintPure, Category = "EventTag")
		int32 GetAppID() { return AppID; };

		UFUNCTION(BlueprintPure, Category = "EventTag")
		FString GetClientID() { return ClientID; };

		UFUNCTION(BlueprintPure, Category = "EventTag")
		FString GetData() { return Data; };

		UFUNCTION(BlueprintPure, Category = "EventTag")
		FString GetDeviceID() { return DeviceID; };

		UFUNCTION(BlueprintPure, Category = "EventTag")
		int32 GetEventID() { return EventID; };

		UFUNCTION(BlueprintPure, Category = "EventTag")
		int32 GetEventLevel() { return EventLevel; };

		UFUNCTION(BlueprintPure, Category = "EventTag")
		FString GetEventTime() { return EventTime; };

		UFUNCTION(BlueprintPure, Category = "EventTag")
		int32 GetEventType() { return EventType; };

		UFUNCTION(BlueprintPure, Category = "EventTag")
		FString GetUUID() { return UUID; };

		UFUNCTION(BlueprintPure, Category = "EventTag")
		int32 GetUX() { return UX; };
		
		UFUNCTION(BlueprintPure, Category = "EventTag")
		FString GetUserID() { return UserID; };
};