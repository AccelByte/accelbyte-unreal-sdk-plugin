// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteGameTelemetryApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

GameTelemetry::GameTelemetry(const AccelByte::Credentials & Credentials, const AccelByte::Settings & Settings)
: Credentials(Credentials)
, Settings(Settings)
{
}

GameTelemetry::~GameTelemetry()
{
}

void GameTelemetry::SendProtectedEvent(FString EventName, FJsonObject Payload, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v1/protected/events"), *Settings.GameTelemetryServerUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField("EventNamespace", Settings.Namespace);
	JsonObject->SetStringField("EventName", EventName);
	JsonObject->SetObjectField("Payload", MakeShared<FJsonObject>(Payload));
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void GameTelemetry::SendProtectedEvents(TMap<FString, TSharedPtr<FJsonObject>> Events, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v1/protected/events"), *Settings.GameTelemetryServerUrl);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	for (const auto& Event : Events)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
		JsonObject->SetStringField("EventNamespace", Settings.Namespace);
		JsonObject->SetStringField("EventName", Event.Key);
		JsonObject->SetObjectField("Payload", Event.Value);

		JsonArray.Add(MakeShared<FJsonValueObject>(JsonObject));
	}	
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonArray, Writer);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} 
}