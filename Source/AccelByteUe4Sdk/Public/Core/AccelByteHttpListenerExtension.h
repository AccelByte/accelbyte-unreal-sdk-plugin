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
	
	DECLARE_DELEGATE(FHttpNotif);
	FString GetAvailableLocalUrl();
	
	void StartHttpListener();
	
	void SetHttpNotifDelegate(const FHttpNotif& OnHttpNotif)
	{
		HttpNotif = OnHttpNotif;
	}

	FString AvailableLocalUrl;
	FHttpNotif HttpNotif;
	int32 TTL;
private:
	bool ListenerCallback(FSocket* Socket, const FIPv4Endpoint& Endpoint);
	
	void StopHttpListener();
	
	FTcpListener* ListenerSocket;
	FString LocalUrl;
	int32 Port;
};
}