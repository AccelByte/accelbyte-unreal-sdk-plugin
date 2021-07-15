// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "HttpManager.h"
#include "CoreMinimal.h"

#include "Http.h"
#include "Sockets.h"
#include "Networking.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(AccelByteHttpListenerLog, Log, All);

namespace AccelByte 
{

class ACCELBYTEUE4SDK_API HttpListenerExtension
{
public:
	HttpListenerExtension();
	~HttpListenerExtension();
public:
	DECLARE_DELEGATE(FHttpNotif);
private:
	int32 Port;
	FString LocalUrl;
	FTcpListener* ListenerSocket;
public:
	int32 TTL;
	FString AvailableLocalUrl;
	FHttpNotif HttpNotif;
public:
	FString GetAvailableLocalUrl();
	
	void StartHttpListener();
	
	void SetHttpNotifDelegate(const FHttpNotif& OnHttpNotif)
	{
		HttpNotif = OnHttpNotif;
	}

private:
	bool ListenerCallback(FSocket* Socket, const FIPv4Endpoint& Endpoint);
	
	void StoptHttpListener();
	
};
}