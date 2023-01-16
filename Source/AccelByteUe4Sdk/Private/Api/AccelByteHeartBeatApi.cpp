// Copyright(c) 2022 AccelByte Inc.All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#include "Api/AccelByteHeartBeatApi.h"

#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteSettings.h"
#include <AccelByteUe4SdkModule.h>
#include <Core/AccelByteUtilities.h>

namespace AccelByte
{
namespace Api
{
	HeartBeat::HeartBeat(Credentials const& InCredentialsRef,
		Settings const& InSettingsRef,
		FHttpRetryScheduler& InHttpRef)
		: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
	{}

	HeartBeat::~HeartBeat()
	{
		Shutdown();
	}

	void HeartBeat::Startup()
	{
		bShuttingDown = false;
		if (bHeartBeatJobStarted)
		{
			return;
		}

		StartHeartBeatScheduler();
	}

	void HeartBeat::Shutdown()
	{
		bShuttingDown = true;
		if (UObjectInitialized())
		{
			HeartBeatResponseDelegate.Unbind();
			OnHeartBeatError.Unbind();
			StopHeartBeatScheduler();
		}
	}
	void HeartBeat::SetHeartBeatResponseDelegate(FHeartBeatResponse OnHeartBeatResponse, FErrorHandler OnError)
	{
		if (HeartBeatResponseDelegate.IsBound())
		{
			HeartBeatResponseDelegate.Unbind();
		}

		if (OnHeartBeatError.IsBound())
		{
			OnHeartBeatError.Unbind();
		}

		HeartBeatResponseDelegate = OnHeartBeatResponse;
		OnHeartBeatError = OnError;
	}
	void HeartBeat::SetHeartBeatEnabled(bool bEnable) 
	{
		bEnableHeartBeat = bEnable;
		if (bEnableHeartBeat)
		{
			StartHeartBeatScheduler();
		}
		else
		{
			StopHeartBeatScheduler();
		}
	}

	void HeartBeat::StartHeartBeatScheduler()
	{
		if (HeartBeatTickDelegateHandle.IsValid())
		{
			return;
		}
		HeartBeatTickDelegate = FTickerDelegate::CreateRaw(this, &HeartBeat::SendHeartBeatEvent);
		HeartBeatTickDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(HeartBeatTickDelegate, static_cast<float>(HeartBeatInterval.GetTotalSeconds()));
		bHeartBeatJobStarted = true;
	}

	void HeartBeat::StopHeartBeatScheduler()
	{
		if (HeartBeatTickDelegateHandle.IsValid())
		{
			FTickerAlias::GetCoreTicker().RemoveTicker(HeartBeatTickDelegateHandle);
			HeartBeatTickDelegateHandle.Reset();
		}
	}

	bool HeartBeat::SendHeartBeatEvent(float DeltaTime)
	{
		if (bShuttingDown)
		{
			return false;
		}

		if (!bEnableHeartBeat)
		{
			StopHeartBeatScheduler();
			return false;
		}

		FReport::Log(FString(__FUNCTION__));

		const FString Url = TEXT("https://heartbeat.accelbyte.io/add");

		TMap<FString, FString> Headers = {
			{TEXT("Content-Type"), TEXT("application/json")},
			{TEXT("Accept"), TEXT("application/json")}
		};
		
		HttpClient.Request(TEXT("POST"), Url, SettingsRef.HeartBeatData, Headers, HeartBeatResponseDelegate, OnHeartBeatError);

		return true;
	}

}
}
