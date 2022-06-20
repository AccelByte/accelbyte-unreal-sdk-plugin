// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteReport.h"
#include "UObject/UObjectBase.h"

DEFINE_LOG_CATEGORY(LogAccelByte);

namespace AccelByte
{
	void FReport::LogHttpRequest(const FHttpRequestPtr & Request)
	{
		if (!UObjectInitialized()) return;

		if (!Request.IsValid())
		{
			UE_LOG(LogAccelByte, Log, TEXT("INVALID REQ HTTP"));
			return;
		}

		UE_LOG(LogAccelByte, Log, TEXT("HTTP REQ %s %s, %p"), *Request->GetVerb(), *Request->GetURL(), Request.Get());

		if (UE_LOG_ACTIVE(LogAccelByte, Verbose))
		{
			FString LogMessage = "";
			LogMessage += "\n---";
			for (const FString& Header : Request->GetAllHeaders())
			{
				LogMessage += "\n" + Header;
			}
			//INTENTIONAL: Request->GetContent() && Request->GetContentLength() could throw an error if it doesn't have content

			LogMessage += "Content-Length: " + FString::FromInt(Request->GetContentLength());

			LogMessage += "\n\n";
			for (auto a : Request->GetContent())
			{
				LogMessage += static_cast<char>(a);
			}
			LogMessage += "\n---\n";

			UE_LOG(LogAccelByte, Verbose, TEXT("%s"), *LogMessage);
		}
	}

	void FReport::LogHttpResponse(const FHttpRequestPtr Request, const FHttpResponsePtr Response)
	{
		if (!UObjectInitialized()) return;
		
		if (!Response.IsValid())
		{
			UE_LOG(LogAccelByte, Log, TEXT("INVALID RSP HTTP"));
			return;
		}

		const FString ShortLogMessage = FString::Printf(TEXT("HTTP %d %s %s, %p"), Response->GetResponseCode(), *Request->GetVerb(), *Response->GetURL(), Request.Get());
		
		if (Response->GetResponseCode() >= 400)
		{
			UE_LOG(LogAccelByte, Warning, TEXT("%s"), *ShortLogMessage);
		}
		else if (Response->GetResponseCode() >= 300)
		{
			UE_LOG(LogAccelByte, Warning, TEXT("%s"), *ShortLogMessage);
		}
		else
		{
			UE_LOG(LogAccelByte, Log, TEXT("%s"), *ShortLogMessage);
		}

		if (UE_LOG_ACTIVE(LogAccelByte, Verbose))
		{
			FString LogMessage = "";
			LogMessage += "\n---";
			for (const FString& Header : Response->GetAllHeaders())
			{
				LogMessage += "\n" + Header;
			}
			LogMessage += "\nContent-Length: " + FString::FromInt(Response->GetContent().Num());
			LogMessage += "\n\n" + Response->GetContentAsString();
			LogMessage += "\n---\n";

			UE_LOG(LogAccelByte, Verbose, TEXT("%s"), *LogMessage);
		}
	}

	void FReport::Log(const FString Message)
	{
		if (!UObjectInitialized()) return;

		UE_LOG(LogAccelByte, Log, TEXT("%s"), *Message);
	}

	void FReport::LogDeprecated(FString FunctionName, FString Message)
	{
		UE_LOG(LogAccelByte, Warning, TEXT("%s. %s is deprecated and will be r"),  *Message, *FunctionName);
	}
}
