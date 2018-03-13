// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeSingleton.h"
#include "Engine.h"
#include "JusticeLog.h"


static UJusticeSingleton* _Instance = nullptr;

UJusticeSingleton* UJusticeSingleton::Instance()
{
	if (_Instance == nullptr)
	{
		_Instance = NewObject<UJusticeSingleton>();
		// initialize local variable
		if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), _Instance->BaseURL, GEngineIni))
		{
			UE_LOG(LogJustice, Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
		}
		if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("Namespace"), _Instance->Namespace, GEngineIni))
		{
			UE_LOG(LogJustice, Error, TEXT("Missing Namespace= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
		}
		if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientId"), _Instance->ClientID, GEngineIni))
		{
			UE_LOG(LogJustice, Error, TEXT("Missing ClientId= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
		}
		if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientSecret"), _Instance->ClientSecret, GEngineIni))
		{
			UE_LOG(LogJustice, Error, TEXT("Missing ClientSecret= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
		}

		_Instance->UserToken = NewObject<UOAuthTokenJustice>();
		_Instance->GameClientToken = NewObject<UOAuthTokenJustice>();
	}
	return _Instance;


}