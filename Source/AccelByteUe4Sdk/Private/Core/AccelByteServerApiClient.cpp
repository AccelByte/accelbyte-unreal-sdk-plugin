// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteServerApiClient.h"

#include "Core/AccelByteApiUtilities.h"
#include "Core/AccelByteInstance.h"

namespace AccelByte
{
	FServerApiClient::FServerApiClient(ServerSettingsPtr InServerSettings, FAccelByteTimeManagerPtr InTimeManager, TSharedRef<FAccelByteInstance, ESPMode::ThreadSafe> InAccelByteInstance)
		: bUseSharedCredentials(false)
		, AccelByteInstanceWeak(InAccelByteInstance)
		, HttpRef(MakeShared<AccelByte::FHttpRetryScheduler, ESPMode::ThreadSafe>())
		, ServerCredentialsRef(MakeShared<AccelByte::ServerCredentials, ESPMode::ThreadSafe>(*HttpRef, InServerSettings->IamServerUrl))
		, ServerSettings(InServerSettings)
		, ServerTimeManager(InTimeManager)
		, ServerApiUtilitiesPtr(MakeShared<FAccelByteApiUtilities, ESPMode::ThreadSafe>(ServerCredentialsRef, ServerSettings.ToSharedRef(), HttpRef, ServerTimeManager))
	{
		HttpRef->Startup();
		ServerCredentialsRef->Startup();
	}

	FServerApiClient::~FServerApiClient()
	{
		if (!bUseSharedCredentials)
		{
			ServerCredentialsRef->Shutdown();
			HttpRef->Shutdown();
		}
	}

	void FServerApiClient::Init()
	{
		ServerAchievement.SetServerApiClient(AsShared());
		ServerCloudSave.SetServerApiClient(AsShared());
		ServerDSM.SetServerApiClient(AsShared());
		ServerEcommerce.SetServerApiClient(AsShared());
		ServerGameTelemetry.SetServerApiClient(AsShared());
		ServerLobby.SetServerApiClient(AsShared());
		ServerChat.SetServerApiClient(AsShared());
		ServerMatchmaking.SetServerApiClient(AsShared());
		ServerOauth2.SetServerApiClient(AsShared());
		ServerQosManager.SetServerApiClient(AsShared());
		ServerSeasonPass.SetServerApiClient(AsShared());
		ServerSessionBrowser.SetServerApiClient(AsShared());
		ServerStatistic.SetServerApiClient(AsShared());
		ServerUGC.SetServerApiClient(AsShared());
		ServerUser.SetServerApiClient(AsShared());
		ServerSession.SetServerApiClient(AsShared());
		ServerDSHub.SetServerApiClient(AsShared());
		ServerMatchmakingV2.SetServerApiClient(AsShared());
		ServerAMS.SetServerApiClient(AsShared());
		ServerMetric.SetServerApiClient(AsShared());
		ServerPredefinedEvent.SetServerApiClient(AsShared());
		ServerGameStandardEvent.SetServerApiClient(AsShared());
		ServerBinaryCloudSave.SetServerApiClient(AsShared());
		ServerChallenge.SetServerApiClient(AsShared());
		ServerInventory.SetServerApiClient(AsShared());
	}

	FString FServerApiClient::GetDeviceId() const
	{
		const FAccelByteInstancePtr AccelByteInstancePtr = AccelByteInstanceWeak.Pin();
		if(AccelByteInstancePtr.IsValid())
		{
			return AccelByteInstancePtr->GetDeviceId();
		}

		return TEXT("");
	}

	FAccelByteApiUtilitiesWPtr FServerApiClient::GetServerApiUtilities()
	{
		return ServerApiUtilitiesPtr;
	}
}
