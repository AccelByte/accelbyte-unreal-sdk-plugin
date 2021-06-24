// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"

void FAccelByteUtilities::RemoveEmptyStrings(TSharedPtr<FJsonObject> JsonPtr)
{
	TArray<FString> KeysToRemove;
	for (auto& Item: JsonPtr->Values)
	{
		if (Item.Value->Type == EJson::String && Item.Value->AsString() == "")
		{
			KeysToRemove.Add(Item.Key);
		}
		else if (Item.Value->Type == EJson::Object)
		{
			TSharedPtr<FJsonObject> const Child = Item.Value->AsObject();
			RemoveEmptyStrings(Child);
		}
	}

	for (FString const& Key : KeysToRemove)
	{
		JsonPtr->RemoveField(Key);
	}
}

void FAccelByteNetUtilities::GetPublicIP(const THandler<FAccelByteModelsPubIp>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FString Url = FString::Printf(TEXT("https://api.ipify.org?format=json"));
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
