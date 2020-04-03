// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteReport.h"

DEFINE_LOG_CATEGORY(AccelByteReportLog);

namespace AccelByte
{

	void Report::GetHttpRequest(const FHttpRequestPtr & Request)
	{
		FString LogMessage = "";

		LogMessage += "\nHTTP Request:";
		LogMessage += FString::Printf(TEXT("\nPtr: %p"), Request.Get());
		LogMessage += "\n---";
		LogMessage += "\n" + Request->GetVerb() + " " + Request->GetURL();
		LogMessage += "\n";
		for (auto a : Request->GetAllHeaders())
		{
			LogMessage += a + "\n";
		}
		LogMessage += "Content-Length: " + FString::FromInt(Request->GetContentLength());
		LogMessage += "\n\n";
		for (auto a : Request->GetContent())
		{
			LogMessage += static_cast<char>(a);
		}
		LogMessage += "\n---";
		LogMessage += "\n";
		
		UE_LOG(AccelByteReportLog, Log, TEXT("%s"), *LogMessage);
	}

	void Report::GetHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response)
	{
		FString LogMessage = "";

		if (Response.IsValid()) 
		{
			LogMessage += "\nHTTP Response:";
			LogMessage += FString::Printf(TEXT("\nPtr: %p"), Request.Get());
			if (Request.IsValid())
			{
				LogMessage += FString::Printf(TEXT("\nRequest: %s %s"), *Request->GetVerb(), *Request->GetURL());
			}
			LogMessage += "\n---";
			LogMessage += "\nHTTP/1.1 " + FString::FromInt(Response->GetResponseCode());
			LogMessage += "\nDate: " + GetStandardTime();
			LogMessage += "\nContent-Length: " + FString::FromInt(Response->GetContent().Num());
			LogMessage += "\n \n" + Response->GetContentAsString();
			LogMessage += "\n---\n";
		}

		UE_LOG(AccelByteReportLog, Log, TEXT("%s"), *LogMessage);
	}

	void Report::GetFunctionLog(FString FunctionMacroName) 
	{
		int count = 0;
		
		FString FunctionName = FunctionMacroName;
		while (FunctionName.Find(TEXT("::")) > 0)
		{
			FunctionName = FunctionName.Right(FunctionName.Len() - FunctionName.Find(TEXT("::")) - 2 );
			count ++;
		};

		FString ClassName = FunctionMacroName;
		for (int i = 0; i < count-1; i++)
		{
			ClassName = ClassName.Right(ClassName.Len() - ClassName.Find(TEXT("::")) - 2 );
			UE_LOG(AccelByteReportLog, Log, TEXT("%s"), *ClassName);
		}
		ClassName = ClassName.Left(ClassName.Len() - FunctionName.Len() - 2 );

		FString LogMessage = "";
		LogMessage += "\nCurrent Function Called:";
		LogMessage += "\n---";
		LogMessage += "\nDate: " + GetStandardTime();
		LogMessage += "\nClass: " + ClassName;
		LogMessage += "\nFunction: " + FunctionName;
		LogMessage += "\n---\n";

		UE_LOG(AccelByteReportLog, Log, TEXT("%s"), *LogMessage);
	}

	FString Report::GetStandardTime()
	{
		FDateTime Now = FDateTime::Now();
		FString StandardTime = FString::Printf(TEXT("%d-%d-%dT%d-%d-%dZ"), Now.GetYear(), Now.GetMonth(), Now.GetDay(), Now.GetHour(), Now.GetMinute(), Now.GetSecond());

		return StandardTime;
	}

}
