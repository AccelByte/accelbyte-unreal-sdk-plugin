// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "OnlineSubsystemJusticeModule.h"
#include "OnlineSubsystemModule.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemJustice.h"

IMPLEMENT_MODULE(FOnlineSubsystemJusticeModule, OnlineSubsystemJustice);

/**
 * Class responsible for creating instance(s) of the subsystem
 */
class FOnlineFactoryJustice : public IOnlineFactory
{
public:

	FOnlineFactoryJustice() {}
	virtual ~FOnlineFactoryJustice() {}

	virtual IOnlineSubsystemPtr CreateSubsystem(FName InstanceName)
	{
		FOnlineSubsystemJusticePtr OnlineSub = MakeShareable(new FOnlineSubsystemJustice(InstanceName));
		if (OnlineSub->IsEnabled())
		{
			if(!OnlineSub->Init())
			{
				UE_LOG_ONLINE(Warning, TEXT("Justice API failed to initialize!"));
				OnlineSub->Shutdown();
				OnlineSub = NULL;
			}
		}
		else
		{
			UE_LOG_ONLINE(Warning, TEXT("Justice API disabled!"));
			OnlineSub->Shutdown();
			OnlineSub = NULL;
		}

		return OnlineSub;
	}
};

void FOnlineSubsystemJusticeModule::StartupModule()
{
	JusticeFactory = new FOnlineFactoryJustice();

	// Create and register our singleton factory with the main online subsystem for easy access
	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.RegisterPlatformService(JUSTICE_SUBSYSTEM, JusticeFactory);
}

void FOnlineSubsystemJusticeModule::ShutdownModule()
{
	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.UnregisterPlatformService(JUSTICE_SUBSYSTEM);
	
	delete JusticeFactory;
	JusticeFactory = NULL;
}
