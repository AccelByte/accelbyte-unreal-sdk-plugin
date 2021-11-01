// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteUserModels.h"

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

FString FAccelByteUtilities::GetPlatformString(EAccelBytePlatformType Platform)
{
	switch (Platform)
	{
	case EAccelBytePlatformType::Steam:
		return "steam";
	case EAccelBytePlatformType::PS4:
		return "ps4";
	case EAccelBytePlatformType::PS4CrossGen:
	case EAccelBytePlatformType::PS5:
		return "ps5";
	case EAccelBytePlatformType::Live:
		return "live";
	case EAccelBytePlatformType::Google:
		return "google";
	case EAccelBytePlatformType::Apple:
		return "apple";	
	case EAccelBytePlatformType::Facebook:
		return "facebook";
	case EAccelBytePlatformType::Android:
		return "android";
	case EAccelBytePlatformType::iOS:
		return "ios";
	case EAccelBytePlatformType::Device:
		return "device";
	case EAccelBytePlatformType::Twitch:
		return "twitch";
	case EAccelBytePlatformType::Oculus:
		return "oculus";
	case EAccelBytePlatformType::Twitter:
		return "twitter";
	case EAccelBytePlatformType::EpicGames:
		return "epicgames";
	case EAccelBytePlatformType::Stadia:
		return "stadia";
	case EAccelBytePlatformType::AwsCognito:
		return "awscognito";
	case EAccelBytePlatformType::Nintendo:
		return "nintendo";	
	default:
		return "unknown";
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

void FAccelByteNetUtilities::DownloadFrom(const FString& Url, const FHttpRequestProgressDelegate& OnProgress, const THandler<TArray<uint8>>& OnDownloaded, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Verb = TEXT("GET");
	FString Accept = TEXT("application/octet-stream");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->OnRequestProgress() = OnProgress;
	Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnDownloaded, OnError), FPlatformTime::Seconds());
}

void FAccelByteNetUtilities::UploadTo(const FString& Url, const TArray<uint8>& DataUpload, const FHttpRequestProgressDelegate& OnProgress, const AccelByte::FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/octet-stream");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->OnRequestProgress() = OnProgress;
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetContent(DataUpload);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	
}
