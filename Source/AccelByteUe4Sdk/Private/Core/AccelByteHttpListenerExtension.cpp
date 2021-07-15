// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteHttpListenerExtension.h"

DEFINE_LOG_CATEGORY(AccelByteHttpListenerLog);

namespace AccelByte
{
	HttpListenerExtension::HttpListenerExtension()
	{
		LocalUrl = "127.0.0.1";
		ListenerSocket = NULL;
		// Assign Port 7779 for communication between Game and Player Portal
		Port = 7779;
		TTL = 30;
	}

	HttpListenerExtension::~HttpListenerExtension()
	{
		if (ListenerSocket != NULL) 
		{
			delete ListenerSocket;
		}
	}

	FString HttpListenerExtension::GetAvailableLocalUrl()
	{
		AvailableLocalUrl = FString::Printf(TEXT("http://%s:%i/"), *LocalUrl, Port);

		return AvailableLocalUrl;
	}

	void HttpListenerExtension::StartHttpListener()
	{
		if (ListenerSocket != NULL) {
			StoptHttpListener();
		}

		FIPv4Endpoint EndPoint;
		FString LocalUrlAndPort = FString::Printf(TEXT("%s:%i"), *LocalUrl, Port);
		FIPv4Endpoint::Parse((TEXT("%s"), *LocalUrlAndPort), EndPoint);
		
		ListenerSocket = new FTcpListener(EndPoint);
		ListenerSocket->OnConnectionAccepted().BindRaw(this, &HttpListenerExtension::ListenerCallback);

	}

	bool HttpListenerExtension::ListenerCallback(FSocket* Socket, const FIPv4Endpoint& Endpoint)
	{
		FString ResponseBody = "Upgrade headless account has been completed. You can close this window and return back to the game.\r\n";
		FTCHARToUTF8 ResponseBodyUtf8(*ResponseBody);

		FString ResponseHeader = FString::Printf(
			TEXT(
				"HTTP/1.1 200 OK\r\n"
				"Connection: keep-alive\r\n"
				"Content-Type: text/html; charset=UTF-8\r\n"
				"Content-Length: %d\r\n"
				"\r\n"
			), 
			ResponseBodyUtf8.Length());

		FString Response = ResponseHeader.Append(ResponseBody);

		FTCHARToUTF8 ResponseUtf8(*Response);
		int32 SentBytes = 0;


		Socket->Send((const uint8_t*)ResponseUtf8.Get(), ResponseUtf8.Length(), SentBytes);
		Socket->Close();

		HttpNotif.ExecuteIfBound();
		HttpListenerExtension::StoptHttpListener();

		return true;
	} 
	
	void HttpListenerExtension::StoptHttpListener() 
	{
		ListenerSocket->Stop();
		delete ListenerSocket;
	}
} 
