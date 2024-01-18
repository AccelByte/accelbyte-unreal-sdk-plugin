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

	class FUnrealTracing 
		: public FRunnable {	

	private:
		struct tracing_data {
			FString MessageId;
			int Type; // http = 1, websocket = 2
			int Direction; // sending = 1, receive = 2
			TSharedPtr<FJsonObject> Data;
			FDateTime TimeStamp;				
		};

		TQueue<tracing_data> MainQueue;
		FThreadSafeBool bIsShuttingDown = false;
		FRunnableThread* Thread;
		FUnrealTracing();
		~FUnrealTracing();


	public:
		virtual uint32 Run() override;

		void AddQueue(FString message_id, int type, int direction, TSharedPtr<FJsonObject> data);

		static FUnrealTracing& GetTracing();
		static void http_request(FHttpRequestPtr& Request);
		static void http_response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bFinished);
		static void websocket_request(FString message);
		static void websocket_response(FString message);


	};


}




#endif // ! ACCELBYTE_TRACING
