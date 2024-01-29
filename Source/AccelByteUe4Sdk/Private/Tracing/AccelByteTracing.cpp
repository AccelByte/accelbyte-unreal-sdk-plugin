#include "Tracing/AccelByteTracing.h"
#include "Api/AccelByteLobbyApi.h"
#include "Networking.h"
#include "IPAddress.h"
#include "SocketSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"

using namespace accelbyte;


FWorkerThread::FWorkerThread(TSharedPtr<TQueue<tracing_data>, ESPMode::ThreadSafe> MainQueue_):
	MainQueue(MainQueue_)
{
	bStopThread = false;
	Thread = FRunnableThread::Create(this, TEXT("Accelbyte Tracing Worker"), 128 * 1024, TPri_Normal, FPlatformAffinity::GetPoolThreadMask());		
}

uint32 FWorkerThread::Run()
{

	// save the file into saved folder
	FString IssueFolder = FPaths::Combine(*FPaths::ProjectSavedDir(), TEXT("accelbyte_tracing.json"));

	//// Prepare a file
	//IFileHandle* pFile = FPlatformFileManager::Get().GetPlatformFile().OpenWrite(*IssueFolder);
	//pFile->Write((uint8*)"[", 1);

	while (!bStopThread)
	{
		// process queue append into file
		tracing_data data_to_process;
		if (MainQueue->Dequeue(data_to_process))
		{
			TSharedPtr<FJsonObject> message = MakeShared< FJsonObject>();
			message->SetField(TEXT("message_id"), MakeShared<FJsonValueString>(data_to_process.MessageId));
			message->SetField(TEXT("timestamp"), MakeShared<FJsonValueString>(data_to_process.TimeStamp.ToIso8601()));
			message->SetField(TEXT("type"), MakeShared<FJsonValueNumber>(data_to_process.Type));
			message->SetField(TEXT("direction"), MakeShared<FJsonValueNumber>(data_to_process.Direction));
			message->SetField(TEXT("data"), MakeShared<FJsonValueObject>(data_to_process.Data));



			FString json_row = TEXT("");
			const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&json_row);
			FJsonSerializer::Serialize(message.ToSharedRef(), Writer);


			//pFile->Write((uint8*)TCHAR_TO_ANSI(*json_row), json_row.Len());
			//pFile->Write((uint8*)",", 1);

			// execute callback
			OnWritingData.ExecuteIfBound(json_row); // TODO can be optimized
		}


		FPlatformProcess::Sleep(0.1);
	}
		
	// End of stream
	//FString EOS = "{\"message_id\": \"0\"}";
	//pFile->Write((uint8*)TCHAR_TO_ANSI(*EOS), EOS.Len());
	//pFile->Write((uint8*)"]", 1);
	//pFile->Flush();
	return 0;
}

	

// ----------------------------------------------


class FNetworkThread : public FRunnable
{
private:
	bool bStopThread;
	FRunnableThread* Thread;
	FSocket* ListenSocket;
	FSocket* ClientSocket; // object to communicate to connected client

	TQueue<FString> NetworkSendQueue; // should be reference?
	
public:
	FNetworkThread()		
	{
		ClientSocket = nullptr;
	}

	void Start() // start TCP server
	{
		// Prepare a socket
		FIPv4Address Address;
		FIPv4Address::Parse(TEXT("0.0.0.0"), Address);

		FIPv4Endpoint Endpoint(Address, 5050);
		Endpoint.Initialize();


		int BufferMaxSize = 128 * 1024;	// should match with the server


		ListenSocket = FTcpSocketBuilder(TEXT("accelbyte-trace-server"))
			.AsNonBlocking()
			.AsReusable()
			.BoundToEndpoint(Endpoint)
			.WithReceiveBufferSize(BufferMaxSize);

		ListenSocket->SetReceiveBufferSize(BufferMaxSize, BufferMaxSize);
		ListenSocket->SetSendBufferSize(BufferMaxSize, BufferMaxSize);
		ListenSocket->Listen(8);
		bool bShouldListen = true;


		bStopThread = false;
		Thread = FRunnableThread::Create(this, TEXT("Accelbyte Network Worker"), 128 * 1024, TPri_Normal, FPlatformAffinity::GetPoolThreadMask());


	}

	virtual uint32 Run() override
	{
		bool newClient = false;
		while (!bStopThread) {

			//Do we have clients trying to connect? connect them
			bool bHasPendingConnection;
			ListenSocket->HasPendingConnection(bHasPendingConnection);
			if (bHasPendingConnection)
			{
				ClientSocket = ListenSocket->Accept(TEXT("tcp-client"));
				newClient = true;
			}


			// check client socket state

			if (ClientSocket != nullptr) {
				ESocketConnectionState ConnectionState = ESocketConnectionState::SCS_NotConnected;
				
				ConnectionState = ClientSocket->GetConnectionState();
				if (ConnectionState != ESocketConnectionState::SCS_Connected) {
					// DISCONNECTED
					ClientSocket->Close();
					ClientSocket = nullptr;
					newClient = false;
					continue;
				}

				if (newClient)
				{
					// send the welcome sequence

					int32 byteSend = 0;
					std::string welcomeMessage = "WELCOME";
					ClientSocket->Send((uint8*)welcomeMessage.c_str(), welcomeMessage.size(), byteSend);


					newClient = false;

				}


				FPlatformProcess::Sleep(0.01);

				FString MessageToSend;
				if (NetworkSendQueue.Dequeue(MessageToSend))
				{
					// Split package into several packet
					int32 byteSend = 0;
					ClientSocket->Send((uint8*)"PACK", 4, byteSend); //  MAGIC, start of the packet


					std::string Payload = TCHAR_TO_ANSI(*MessageToSend);

					uint32 sizeToSend = MessageToSend.Len();
					byteSend = 0;
					ClientSocket->Send((uint8*)&sizeToSend, 4, byteSend); //  4 byte of length

					byteSend = 0;
					ClientSocket->Send((uint8*)Payload.c_str(), sizeToSend, byteSend); //  the payload
				}
			}
		}
		
		return 0;
	}

	void AddToSendQueue(FString message)
	{
		NetworkSendQueue.Enqueue(message);
	}
};

TSharedPtr<FNetworkThread> NetworkThread;

//--------------------------------------------------------------------


class MissionOutputDevice : public FOutputDevice {
public:
	MissionOutputDevice()
	{
		LogQueue = MakeShared<TQueue<FString>>();
		check(GLog);
		GLog->AddOutputDevice(this);
		if (GLog->IsRedirectingTo(this))
			return; // Never gets hit

		return;
	};

	~MissionOutputDevice()
	{
		if (GLog != nullptr) {
			GLog->RemoveOutputDevice(this);
		}
	};

	void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category) override
	{
		FString LogString = FOutputDeviceHelper::FormatLogLine(Verbosity, Category, V, GPrintLogTimes);
		LogQueue->Enqueue(LogString);
	}

	void SerializeGlobalLogBacklog()
	{
		if (GLog != nullptr) {
			GLog->SerializeBacklog(this);
		}
	}

	TSharedPtr<TQueue<FString>, ESPMode::ThreadSafe> LogQueue;
};

//--------------------------------------------------------------------
TUniquePtr<MissionOutputDevice> AccelByteLogger;

FUnrealTracing& FUnrealTracing::GetTracing()
{
	static FUnrealTracing instance;
	return instance;
}



FUnrealTracing::FUnrealTracing()
{
	// 1
	MainQueue = MakeShared<TQueue<tracing_data>>();
	WorkerThread = MakeShared< FWorkerThread>(MainQueue);
	WorkerThread->OnWritingData.BindRaw(this, &FUnrealTracing::SendPacket);

	// 2
	NetworkThread = MakeShared<FNetworkThread>();
	NetworkThread->Start();	

	//3
	AccelByteLogger = MakeUnique<MissionOutputDevice>();
	AccelByteLogger->SerializeGlobalLogBacklog();

}

FUnrealTracing::~FUnrealTracing()
{
	bIsShuttingDown = true;
	AccelByteLogger.Reset();
}


void FUnrealTracing::AddQueue(FString message_id, int type, int direction, TSharedPtr<FJsonObject> data)
{
	MainQueue->Enqueue(tracing_data{ message_id , type , direction, data, FDateTime::Now() });
}

void FUnrealTracing::SendPacket(FString message)
{
	NetworkThread->AddToSendQueue(message);
}

void FUnrealTracing::http_request(FHttpRequestPtr& Request)
{
	// build message for http response
	TSharedPtr<FJsonObject> data = MakeShared< FJsonObject>();
	TSharedPtr<FJsonObject> header_json = MakeShared< FJsonObject>();
	data->SetField(TEXT("verb"), MakeShared<FJsonValueString>(Request->GetVerb()));
	data->SetField(TEXT("url"), MakeShared<FJsonValueString>(Request->GetURL()));
	auto http_headers = Request->GetAllHeaders(); // return sting in Key: Value format	
	for (int i = 0; i < http_headers.Num(); i++)
	{
		FString header = http_headers[i];
		FString header_key = TEXT("");
		FString header_value = TEXT("");
		header.Split(": ", &header_key, &header_value);
		header_json->SetField(header_key, MakeShared<FJsonValueString>(header_value));
	}
	data->SetField(TEXT("header"), MakeShared<FJsonValueObject>(header_json));
	FString content_type = Request->GetContentType();
	const TArray<uint8> Payload = Request->GetContent();

	if (Payload.Num() > 0) {
		FString String = FAccelByteArrayByteFStringConverter::BytesToFString(Payload, true);
		if (content_type == TEXT("application/json")) {
			TSharedPtr<FJsonObject> JsonObject = MakeShared < FJsonObject>();
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(String);
			bool bSuccess = FJsonSerializer::Deserialize(Reader, JsonObject);
			if (bSuccess)
			{
				data->SetField(TEXT("payload"), MakeShared<FJsonValueObject>(JsonObject));
			}
			else
			{
				data->SetField(TEXT("payload"), MakeShared<FJsonValueString>(String));
			}

		}
		else {
			data->SetField(TEXT("payload"), MakeShared<FJsonValueString>(String));
		}
	}




	FString message_id = FString::Printf(TEXT("%p"), Request.Get());

	// add to Queue                     HTTP   Request
	GetTracing().AddQueue(message_id, 1, 1, data);


}
void FUnrealTracing::http_response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bFinished)
{
	// build message for http response
	TSharedPtr<FJsonObject> data = MakeShared< FJsonObject>();
	TSharedPtr<FJsonObject> header_json = MakeShared< FJsonObject>();
	data->SetField(TEXT("verb"), MakeShared<FJsonValueString>(Request->GetVerb()));
	data->SetField(TEXT("url"), MakeShared<FJsonValueString>(Request->GetURL()));
	auto http_headers = Request->GetAllHeaders(); // return sting in Key: Value format	
	for (int i = 0; i < http_headers.Num(); i++)
	{
		FString header = http_headers[i];
		FString header_key = TEXT("");
		FString header_value = TEXT("");
		header.Split(": ", &header_key, &header_value);
		header_json->SetField(header_key, MakeShared<FJsonValueString>(header_value));
	}
	data->SetField(TEXT("header"), MakeShared<FJsonValueObject>(header_json));
	data->SetField(TEXT("status"), MakeShared<FJsonValueNumber>(Response->GetResponseCode()));

	FString content_type = Response->GetContentType();
	data->SetField(TEXT("content_type"), MakeShared<FJsonValueString>(content_type));

	const TArray<uint8> Payload = Response->GetContent();
	FString String = (Response == nullptr ? FAccelByteArrayByteFStringConverter::BytesToFString(Payload, true) : Response->GetContentAsString());

	String = String.Replace(TEXT("\r"), TEXT(""));
	String = String.Replace(TEXT("\n"), TEXT(""));

	if (content_type.Contains(TEXT("application/json"))) {
		if (String[0] == '[') {
			String = FString("{\"root\":") + String + '}'; // UNREAL ENGINE JSON Deserialize Bug
		}

		TSharedPtr<FJsonObject> JsonObject = MakeShared < FJsonObject>();
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(String);
		bool bSuccess = FJsonSerializer::Deserialize(Reader, JsonObject);
		if (bSuccess)
		{
			data->SetField(TEXT("payload"), MakeShared<FJsonValueObject>(JsonObject));
		}
		else
		{
			data->SetField(TEXT("payload"), MakeShared<FJsonValueString>(String));
		}

	}
	else {
		data->SetField(TEXT("payload"), MakeShared<FJsonValueString>(String));
	}




	FString message_id = FString::Printf(TEXT("%p"), Request.Get());

	// add to Queue                     HTTP   Response
	GetTracing().AddQueue(message_id, 1, 2, data);

}
void FUnrealTracing::websocket_request(FString message)
{
	// build message for http response
	TSharedPtr<FJsonObject> data = MakeShared< FJsonObject>();
	FString message_copy = message;

	// need to update with Api\AccelByteChatApi.h
	const FString WsEnvelopeStart{ "CaSr" };
	const FString WsEnvelopeEnd{ "CaEd" };
	const FString WSEnvelopeYAML{ "type:" };


	if (message_copy.StartsWith(WsEnvelopeStart) && message_copy.EndsWith(WsEnvelopeEnd))
	{
		message_copy.RemoveFromStart(WsEnvelopeStart);
		message_copy.RemoveFromEnd(WsEnvelopeEnd);

		TSharedPtr<FJsonObject> JsonObject = MakeShared < FJsonObject>();
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(message_copy);
		bool bSuccess = FJsonSerializer::Deserialize(Reader, JsonObject);
		if (bSuccess)
		{
			data->SetField(TEXT("payload"), MakeShared<FJsonValueObject>(JsonObject));
		}
		else {
			data->SetField(TEXT("payload"), MakeShared<FJsonValueString>(message_copy));
		}

	}
	else if (message_copy.StartsWith(WSEnvelopeYAML))
	{
		FString Json = AccelByte::Api::Lobby::LobbyMessageToJson(message_copy);

		TSharedPtr<FJsonObject> JsonObject = MakeShared < FJsonObject>();
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Json);
		bool bSuccess = FJsonSerializer::Deserialize(Reader, JsonObject);
		if (bSuccess)
		{
			data->SetField(TEXT("payload"), MakeShared<FJsonValueObject>(JsonObject));
		}
		else {
			data->SetField(TEXT("payload"), MakeShared<FJsonValueString>(message_copy));
		}

	}
	else
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared < FJsonObject>();
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(message_copy);
		bool bSuccess = FJsonSerializer::Deserialize(Reader, JsonObject);
		if (bSuccess)
		{
			data->SetField(TEXT("payload"), MakeShared<FJsonValueObject>(JsonObject));
		}
		else {
			data->SetField(TEXT("payload"), MakeShared<FJsonValueString>(message_copy));
		}
	}


	static uint64 message_id_request = 1;
	FString message_id = FString::Printf(TEXT("%d"), message_id_request);

	// add to Queue                     websocket   Request
	GetTracing().AddQueue(message_id, 2, 1, data);
	message_id_request += 1;
}
void FUnrealTracing::websocket_response(FString message)
{
	// build message for http response
	TSharedPtr<FJsonObject> data = MakeShared< FJsonObject>();



	FString message_copy = message;

	// need to update with Api\AccelByteChatApi.h
	const FString WsEnvelopeStart{ "CaSr" };
	const FString WsEnvelopeEnd{ "CaEd" };
	const FString WSEnvelopeYAML{ "type:" };

	if (message_copy.StartsWith(WsEnvelopeStart) && message_copy.EndsWith(WsEnvelopeEnd))
	{
		message_copy.RemoveFromStart(WsEnvelopeStart);
		message_copy.RemoveFromEnd(WsEnvelopeEnd);

		TSharedPtr<FJsonObject> JsonObject = MakeShared < FJsonObject>();
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(message_copy);
		bool bSuccess = FJsonSerializer::Deserialize(Reader, JsonObject);
		if (bSuccess)
		{
			data->SetField(TEXT("payload"), MakeShared<FJsonValueObject>(JsonObject));
		}
		else {
			data->SetField(TEXT("payload"), MakeShared<FJsonValueString>(message_copy));
		}

	}
	else if (message_copy.StartsWith(WSEnvelopeYAML))
	{
		FString Json = AccelByte::Api::Lobby::LobbyMessageToJson(message_copy);

		TSharedPtr<FJsonObject> JsonObject = MakeShared < FJsonObject>();
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Json);
		bool bSuccess = FJsonSerializer::Deserialize(Reader, JsonObject);
		if (bSuccess)
		{
			data->SetField(TEXT("payload"), MakeShared<FJsonValueObject>(JsonObject));
		}
		else {
			data->SetField(TEXT("payload"), MakeShared<FJsonValueString>(message_copy));
		}

	}
	else
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared < FJsonObject>();
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(message_copy);
		bool bSuccess = FJsonSerializer::Deserialize(Reader, JsonObject);
		if (bSuccess)
		{
			data->SetField(TEXT("payload"), MakeShared<FJsonValueObject>(JsonObject));
		}
		else {
			data->SetField(TEXT("payload"), MakeShared<FJsonValueString>(message_copy));
		}
	}

	static uint64 message_id_response = 1;
	FString message_id = FString::Printf(TEXT("%d"), message_id_response);


	// add to Queue                     websocket   Response
	GetTracing().AddQueue(message_id, 2, 2, data);
	message_id_response += 1;
}
