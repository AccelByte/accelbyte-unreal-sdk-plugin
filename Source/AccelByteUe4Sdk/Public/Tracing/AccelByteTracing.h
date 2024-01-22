#pragma once

#ifndef ACCELBYTE_TRACING
#define ACCELBYTE_TRACING

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "Http.h"
#include "Containers/Queue.h"

#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "HAL/ThreadSafeBool.h"
#include "Json.h"

namespace accelbyte {

	struct tracing_data {
		FString MessageId;
		int Type; // http = 1, websocket = 2
		int Direction; // sending = 1, receive = 2
		TSharedPtr<FJsonObject> Data;
		FDateTime TimeStamp;
	};



	DECLARE_DELEGATE_OneParam(FOnWritingDataDelegate, FString);

	class FWorkerThread : public FRunnable
	{
	private:
		bool bStopThread;
		FRunnableThread* Thread;
		TSharedPtr<TQueue<tracing_data>, ESPMode::ThreadSafe> MainQueue; // should be reference?
	public:
		FWorkerThread(TSharedPtr<TQueue<tracing_data>, ESPMode::ThreadSafe> MainQueue_);
		virtual uint32 Run() override;
		FOnWritingDataDelegate OnWritingData;
	};


	class FUnrealTracing {	
	private:
		TSharedPtr<TQueue<tracing_data>, ESPMode::ThreadSafe> MainQueue;
		FThreadSafeBool bIsShuttingDown = false;
		

		TSharedPtr <FWorkerThread> WorkerThread;


		FUnrealTracing();
		~FUnrealTracing();

		void SendPacket(FString message);


	public:

		void AddQueue(FString message_id, int type, int direction, TSharedPtr<FJsonObject> data);

		static FUnrealTracing& GetTracing();
		static void http_request(FHttpRequestPtr& Request);
		static void http_response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bFinished);
		static void websocket_request(FString message);
		static void websocket_response(FString message);


	};


}




#endif // ! ACCELBYTE_TRACING
