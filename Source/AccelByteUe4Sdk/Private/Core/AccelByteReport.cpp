// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteReport.h"
#include "UObject/UObjectBase.h"
#include "Internationalization/Regex.h"

DEFINE_LOG_CATEGORY(LogAccelByte);

namespace AccelByte
{
	bool ShouldLogHttpPayload(FHttpRequestPtr const& Request)
	{
		const FString LogLevelSquelch = Request->GetHeader(GHeaderABLogSquelch);
		if (!LogLevelSquelch.IsEmpty() && LogLevelSquelch == "true")
		{
			return UE_LOG_ACTIVE(LogAccelByte, VeryVerbose);
		}
		return UE_LOG_ACTIVE(LogAccelByte, Verbose);
	}
	
	void FReport::LogHttpRequest(FHttpRequestPtr const& Request)
	{
		if (!UObjectInitialized()) return;

		if (!Request.IsValid())
		{
			UE_LOG(LogAccelByte, Verbose, TEXT("INVALID REQ HTTP"));
			return;
		}

		UE_LOG(LogAccelByte, Verbose, TEXT("HTTP REQ %s %s, %p"), *Request->GetVerb(), *Request->GetURL(), Request.Get());

		if (ShouldLogHttpPayload(Request))
		{
			FString LogMessage = "";
			LogMessage += "\n---";

			if (UE_LOG_ACTIVE(LogAccelByte, VeryVerbose))
			{
				for (const FString& Header : Request->GetAllHeaders())
				{
					LogMessage += "\n" + Header;
				}
			}
			//INTENTIONAL: Request->GetContent() && Request->GetContentLength() could throw an error if it doesn't have content


			LogMessage += "\n\n";

			FString Content;
			TArray<uint8> const& RequestContent = Request->GetContent();
			for (auto a : RequestContent)
			{
				Content += static_cast<char>(a);
			}

			const FRegexPattern PasswordPattern(R"x(password=([^&]*)|"password"\s*:\s*"((\\"|[^"])*)")x");
			FRegexMatcher PasswordMatcher(PasswordPattern, Content);
			while (PasswordMatcher.FindNext())
			{
				// form data param value
				int Start = PasswordMatcher.GetCaptureGroupBeginning(1);
				int End = PasswordMatcher.GetCaptureGroupEnding(1);
				if (Start == INDEX_NONE)
				{
					// json value
					Start = PasswordMatcher.GetCaptureGroupBeginning(2);
					End = PasswordMatcher.GetCaptureGroupEnding(2);
				}
				// minimal 4 characters masked, maximal 3 characters unmasked
				const int MaskStart = FMath::Min(FMath::Max(Start, End - 4), Start + 3);
				for (int i = MaskStart; i < End; i++)
				{
					Content[i] = '*';
				}
			}

			LogMessage += Content;
			LogMessage += "\n---\n";
			UE_LOG(LogAccelByte, Verbose, TEXT("%s"), *LogMessage);
		}
	}

	void FReport::LogHttpResponse(const FHttpRequestPtr Request, const FHttpResponsePtr Response)
	{
		if (!UObjectInitialized()) return;
		
		if (!Response.IsValid())
		{
			UE_LOG(LogAccelByte, Warning, TEXT("INVALID HTTP RESPONSE"));
			return;
		}

		const FString ShortLogMessage = FString::Printf(TEXT("HTTP %d %s %s, %p"), Response->GetResponseCode(), *Request->GetVerb(), *Response->GetURL(), Request.Get());
		
		if (Response->GetResponseCode() >= 300)
		{
			UE_LOG(LogAccelByte, Warning, TEXT("%s"), *ShortLogMessage);
		}
		else
		{
			UE_LOG(LogAccelByte, Verbose, TEXT("%s"), *ShortLogMessage);
		}

		if (ShouldLogHttpPayload(Request))
		{
			FString LogMessage = "";
			LogMessage += "\n---";
			if (UE_LOG_ACTIVE(LogAccelByte, VeryVerbose))
			{
				for (const FString& Header : Response->GetAllHeaders())
				{
					LogMessage += "\n" + Header;
				}
			}
			LogMessage += "\n\n" + Response->GetContentAsString();
			LogMessage += "\n---\n";

			UE_LOG(LogAccelByte, Verbose, TEXT("%s"), *LogMessage);
		}
	}

	void FReport::Log(const FString Message)
	{
		if (!UObjectInitialized()) return;

		UE_LOG(LogAccelByte, Verbose, TEXT("%s"), *Message);
	}

	void FReport::LogDeprecated(FString FunctionName, FString Message)
	{
		UE_LOG(LogAccelByte, Warning, TEXT("%s is marked as deprecated. Message: %s"), *FunctionName, *Message);
	}
}
