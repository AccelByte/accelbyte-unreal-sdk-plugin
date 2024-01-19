#include "Tracing/AccelByteTracing.h"
#include "Api/AccelByteLobbyApi.h"


using namespace accelbyte;

FUnrealTracing& FUnrealTracing::GetTracing()
{
	static FUnrealTracing instance;
	return instance;
}

FUnrealTracing::FUnrealTracing()
{
	Thread = FRunnableThread::Create(this, TEXT("Accelbyte FUnrealTracing Worker"), 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

FUnrealTracing::~FUnrealTracing()
{
	bIsShuttingDown = true;
	Thread->WaitForCompletion();
}

uint32 FUnrealTracing::Run()
{
	// Prepare a file

	IFileHandle* pFile = FPlatformFileManager::Get().GetPlatformFile().OpenWrite(TEXT("D:\\Temp\\ags_debug_tool\\output.json"));
	pFile->Write((uint8*)"[", 1);

	for (;;)
	{
		if (bIsShuttingDown) {

			// End of stream
			FString EOS = "{\"message_id\": \"0\"}";
			pFile->Write((uint8*)TCHAR_TO_ANSI(*EOS), EOS.Len());
			pFile->Write((uint8*)"]", 1);
			pFile->Flush();
			return 0;
		}


		// process queue append into file
		tracing_data data_to_process;
		if (MainQueue.Dequeue(data_to_process))
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


			pFile->Write((uint8*)TCHAR_TO_ANSI(*json_row), json_row.Len());
			pFile->Write((uint8*)",", 1);
			//
		}


		FPlatformProcess::Sleep(0.1);
	}
	return 0;
}

void FUnrealTracing::AddQueue(FString message_id, int type, int direction, TSharedPtr<FJsonObject> data)
{
	MainQueue.Enqueue(tracing_data{ message_id , type , direction, data, FDateTime::Now() });
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
