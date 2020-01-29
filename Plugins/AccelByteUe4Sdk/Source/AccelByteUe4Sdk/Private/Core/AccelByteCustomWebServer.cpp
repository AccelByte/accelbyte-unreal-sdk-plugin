// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

//#include "UnrealEdRemotePrivatePCH.h"
#include "Core/AccelByteCustomWebServer.h"
#include "Runtime/Core/Public/Misc/CString.h"
#include "GameDelegates.h"

FAccelByteCustomWebServer::FAccelByteCustomWebServer()
	: Listener(NULL), Thread(NULL)
{}

FAccelByteCustomWebServer::~FAccelByteCustomWebServer()
{
	Stop();
}

void FAccelByteCustomWebServer::Start(const uint16 Port)
{
	SetPort(Port);
	Thread = FRunnableThread::Create(this, TEXT("FAccelByteCustomWebServer"), 8 * 1024, TPri_Normal);
}

void FAccelByteCustomWebServer::Unbind()
{
	Stop();

	// Stop accepting clients first
	if (Listener != NULL)
	{
		Listener->Stop();
		delete Listener;
		Listener = NULL;
	}

	// Kill all pending connections and current connections
	if (!PendingClients.IsEmpty())
	{
		FSocket *Client = NULL;
		while (PendingClients.Dequeue(Client))
		{
			Client->Close();
		}
	}
	for (TArray<class FSocket*>::TIterator ClientIt(Clients); ClientIt; ++ClientIt)
	{
		(*ClientIt)->Close();
	}

	// And last but not least stop the main thread
	if (Thread != NULL)
	{
		Thread->Kill(true);
		delete Thread;
	}
}

bool FAccelByteCustomWebServer::HandleListenerConnectionAccepted(class FSocket* ClientSocket, const FIPv4Endpoint& ClientEndpoint)
{
	PendingClients.Enqueue(ClientSocket);
	return true;
}

bool FAccelByteCustomWebServer::Init()
{
	if (Listener == NULL)
	{
		const uint16 ServerPort = this->GetPort();
		Listener = new FTcpListener(FIPv4Endpoint(FIPv4Address(0, 0, 0, 0), ServerPort));
		Listener->OnConnectionAccepted().BindRaw(this, &FAccelByteCustomWebServer::HandleListenerConnectionAccepted);
		Stopping = false;
		UE_LOG(LogTemp, Log, TEXT("Web server started...On port %i"), ServerPort);
	}
	return (Listener != NULL);
}

/** Send a string message over to a socket */
bool SendMessage(FSocket *Socket, const FString& Message)
{
	check(Socket);
	int32 BytesSent = 0;
	return Socket->Send((uint8*)TCHAR_TO_UTF8(*Message), Message.Len(), BytesSent);
}

/** Receive a string message from a socket */
bool RecvMessage(FSocket *Socket, uint32 DataSize, FString& Message)
{
	check(Socket);

	FArrayReaderPtr Datagram = MakeShareable(new FArrayReader(true));
	Datagram->Init(FMath::Min(DataSize, 65507u), 500);

	int32 BytesRead = 0;
	if (Socket->Recv(Datagram->GetData(), Datagram->Num(), BytesRead))
	{
		char* Data = (char*)Datagram->GetData();
		Data[BytesRead] = '\0';
		Message = UTF8_TO_TCHAR(Data);
		return true;
	}
	return false;
}

static FString DefaultResponseCode = "200";
static TMap<FString, FString> HttpCodesMap()
{
	TMap<FString, FString> retVal;
	retVal.Add(TEXT("200"), TEXT("OK"));
	retVal.Add(TEXT("201"), TEXT("Created"));
	retVal.Add(TEXT("202"), TEXT("Accepted"));
	retVal.Add(TEXT("203"), TEXT("Non-Authoritative Information"));
	retVal.Add(TEXT("204"), TEXT("No Content"));
	retVal.Add(TEXT("205"), TEXT("Reset Content"));
	retVal.Add(TEXT("206"), TEXT("Partial Content"));
	retVal.Add(TEXT("207"), TEXT("Multi-Status"));
	retVal.Add(TEXT("208"), TEXT("Already Reported"));
	retVal.Add(TEXT("300"), TEXT("Multiple Choices"));
	retVal.Add(TEXT("301"), TEXT("Moved Permanently"));
	retVal.Add(TEXT("302"), TEXT("Found"));
	retVal.Add(TEXT("303"), TEXT("See Other"));
	retVal.Add(TEXT("304"), TEXT("Not Modified"));
	retVal.Add(TEXT("305"), TEXT("Use Proxy"));
	retVal.Add(TEXT("306"), TEXT("Switch Proxy"));
	retVal.Add(TEXT("307"), TEXT("Temporary Redirect"));
	retVal.Add(TEXT("308"), TEXT("Permanent Redirect"));
	retVal.Add(TEXT("400"), TEXT("Bad Request"));
	retVal.Add(TEXT("401"), TEXT("Unauthorized"));
	retVal.Add(TEXT("402"), TEXT("Payment Required"));
	retVal.Add(TEXT("403"), TEXT("Forbidden"));
	retVal.Add(TEXT("404"), TEXT("Not Found"));
	retVal.Add(TEXT("405"), TEXT("Method Not Allowed"));
	retVal.Add(TEXT("406"), TEXT("Not Acceptable"));
	retVal.Add(TEXT("407"), TEXT("Proxy Authentication Required"));
	retVal.Add(TEXT("408"), TEXT("Request Timeout"));
	retVal.Add(TEXT("409"), TEXT("Conflict"));
	retVal.Add(TEXT("410"), TEXT("Gone"));
	retVal.Add(TEXT("500"), TEXT("Internal Server Error"));
	retVal.Add(TEXT("501"), TEXT("Not Implemented"));
	retVal.Add(TEXT("502"), TEXT("Bad Gateway"));
	retVal.Add(TEXT("503"), TEXT("Service Unavailable"));
	retVal.Add(TEXT("504"), TEXT("Gateway Timeout"));
	retVal.Add(TEXT("505"), TEXT("HTTP Version Not Supported"));

	return retVal;
}

uint32 FAccelByteCustomWebServer::Run()
{

	while (!Stopping)
	{
		if (!PendingClients.IsEmpty())
		{
			FSocket *Client = NULL;
			while (PendingClients.Dequeue(Client))
			{
				Clients.Add(Client);
			}
		}

		// remove closed connections
		for (int32 ClientIndex = Clients.Num() - 1; ClientIndex >= 0; --ClientIndex)
		{
			if (Clients[ClientIndex]->GetConnectionState() != SCS_Connected)
			{
				Clients.RemoveAtSwap(ClientIndex);
			}
		}

		// Poll data from every connected client
		for (FSocket* Client : Clients)
		{
			uint32 DataSize = 0;
			FString Request = TEXT("");
			while (Client->HasPendingData(DataSize))
			{
				FString Chunk;
				if (RecvMessage(Client, DataSize, Chunk))
				{
					Request.Append(Chunk);
				}
			}

			if (!Request.IsEmpty())
			{
				FString URL = TEXT("/");
				FString Action;
				TMap<FString, FString> Params;
				FString Header;

				int32 pos = Request.Find("\r\n\r\n");
				if (pos > 0)
				{
					Header = Request.Left(pos);
					Params.Add("Body", Request.RightChop(pos + 4));
				}
				else
				{
					Header = Request;
				}

				TArray<FString> Out;
				Header.ParseIntoArray(Out, TEXT("\r\n"));

				FString currentLine = Out[0];
				pos = currentLine.Find(" ");
				Action = currentLine.Left(pos);

				int32 start = pos + 1;
				pos = currentLine.Find(" ", ESearchCase::CaseSensitive, ESearchDir::FromStart, start);
				URL = currentLine.Mid(start, pos - start);

				for (int i = 1; i < Out.Num(); i++)
				{
					if (currentLine.Len() == 0) break;

					currentLine = Out[i];
					pos = currentLine.Find(":") + 1;
					while (currentLine[pos] == L' ') ++pos;
					Params.Add(currentLine.Left(pos), currentLine.RightChop(pos));
				}
				TMap<FString, FString> Response;
				FGameDelegates::Get().GetWebServerActionDelegate().ExecuteIfBound(0, Action, URL, Params, Response);
				FString ResponseCode;
				Response.RemoveAndCopyValue(TEXT("Code"), ResponseCode);
				FString* ResponseCodeName = nullptr;
				if (ResponseCode.IsEmpty())
				{
					ResponseCode = DefaultResponseCode;
				}

				TMap<FString, FString> CodesMap = HttpCodesMap();
				ResponseCodeName = CodesMap.Find(ResponseCode);

				FString StringHeaders = "";
				FString ContentBody;
				Response.RemoveAndCopyValue(TEXT("Body"), ContentBody);
				for (const TPair<FString, FString>& KeyValue : Response)
				{
					StringHeaders.Append(FString::Printf(TEXT("%s: %s\r\n"), *KeyValue.Key, *KeyValue.Value));
				}
				StringHeaders.RemoveFromEnd(TEXT("\r\n"));

				FString ResponseString = FString::Printf(TEXT("HTTP/1.0 %s %s\r\n%s\r\n\r\n%s"),
					*ResponseCode,
					**ResponseCodeName,
					*StringHeaders,
					*ContentBody);
				SendMessage(Client, ResponseString);
			}
			Client->Close();
		}

		FPlatformProcess::Sleep(0.2f);
	}

	return 0;
}

FString FAccelByteCustomWebServer::HandleClientMessage(const FSocket *Socket, const FString& Message)
{
	bool bProcessed = false;
	FString Response;

	const TCHAR* Cmd = *Message;
	if (FParse::Command(&Cmd, TEXT("EXEC")) && HandleExecCommand(Socket, Message.Mid(FCString::Strlen(TEXT("CMD")))))
	{
		bProcessed = true;
	}

	return bProcessed ? TEXT("OK") : TEXT("FAILED");
}

bool FAccelByteCustomWebServer::HandleExecCommand(const FSocket *Socket, const FString& Command)
{
	if (GEngine != NULL)
	{
		GEngine->DeferredCommands.Add(Command);
		return true;
	}
	return false;
}