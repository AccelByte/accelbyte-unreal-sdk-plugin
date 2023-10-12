// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/StatsD/AccelByteStatsDMetricCollector.h"
#include "Engine/GameEngine.h"
#include "Misc/ConfigCacheIni.h"
#include "Misc/App.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteStatsDMetricCollector, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteStatsDMetricCollector);

namespace AccelByte
{
	FAccelByteStatsDMetricCollector::~FAccelByteStatsDMetricCollector()
	{
	}

	int32 FAccelByteStatsDMetricCollector::GetPlayerCapacity()
	{
		int32 PlayerCapacity = 0;
		if (!GConfig->GetInt(TEXT("/Script/Engine.GameSession"), TEXT("MaxPlayers"), PlayerCapacity, GGameIni))
		{
			UE_LOG(LogAccelByteStatsDMetricCollector, Warning, TEXT("Missing PlayerCapacity key in [/Script/Engine.GameSession] of BaseGame.ini"));
		}
		return PlayerCapacity;
	}

	int32 FAccelByteStatsDMetricCollector::GetPlayerCount()
	{
		int32 Result = 0;
#if !WITH_EDITOR
		if (UGameEngine* GameEngine = CastChecked<UGameEngine>(GEngine))
		{
			UWorld* World = GameEngine->GetGameWorld();
			if (World != nullptr)
			{
				Result = World->GetAuthGameMode()->GetNumPlayers();
			}
		}
#else
		UE_LOG(LogAccelByteStatsDMetricCollector, Warning, TEXT("GetPlayerCount is not implemented"));
#endif
		return Result;
	}

	int32 FAccelByteStatsDMetricCollector::GetClientCount()
	{
		int32 Result = 0;
#if !WITH_EDITOR
		if (UGameEngine* GameEngine = CastChecked<UGameEngine>(GEngine))
		{
			UWorld* World = GameEngine->GetGameWorld();
			if (World != nullptr)
			{
				Result = World->GetAuthGameMode()->GetNumPlayers() + World->GetAuthGameMode()->GetNumSpectators();
			}
		}
#else
		UE_LOG(LogAccelByteStatsDMetricCollector, Warning, TEXT("GetClientCount is not implemented"));
#endif
		return Result;
	}

	int32 FAccelByteStatsDMetricCollector::GetAiCount()
	{
		UE_LOG(LogAccelByteStatsDMetricCollector, Warning, TEXT("GetAiCount is not implemented"));
		return 0;
	}

	double FAccelByteStatsDMetricCollector::GetFrameTimeAverage()
	{
		return FApp::GetDeltaTime();
	}

	double FAccelByteStatsDMetricCollector::GetFrameTimeMax()
	{
		double Result = 0.0;
#if !WITH_EDITOR
		if (UGameEngine* GameEngine = CastChecked<UGameEngine>(GEngine))
		{
			Result = GameEngine->MaxDeltaTime;
		}
#else
		UE_LOG(LogAccelByteStatsDMetricCollector, Warning, TEXT("GetFrameTimeMax is not implemented"));
#endif
		return Result;
	}

	double FAccelByteStatsDMetricCollector::GetFrameStartDelayAverage()
	{
		UE_LOG(LogAccelByteStatsDMetricCollector, Warning, TEXT("GetFrameStartDelayAverageNot is not implemented"));
		return 0.0;
	}

	double FAccelByteStatsDMetricCollector::GetFrameStartDelayMax()
	{
		UE_LOG(LogAccelByteStatsDMetricCollector, Warning, TEXT("GetFrameStartDelayMax is not implemented"));
		return 0.0;
	}
}