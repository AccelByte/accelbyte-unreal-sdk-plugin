// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "GameServerApi/AccelByteServerLobby.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"
#include "Models/AccelByteLobbyModels.h"

namespace AccelByte
{
namespace GameServerApi
{
	ServerLobby::ServerLobby(const ServerCredentials& Credentials, const ServerSettings& Settings) : Credentials(Credentials), Settings(Settings)
	{}

	ServerLobby::~ServerLobby()
	{}

	void ServerLobby::GetPartyDataByUserId(const FString & UserId, const THandler<FAccelByteModelsDataPartyResponse> OnSuccess, const FErrorHandler & OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		if (UserId.IsEmpty())
		{
			OnError.ExecuteIfBound(404, TEXT("Url is invalid. User Id is empty."));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/users/%s/party"), *Settings.LobbyServerUrl, *Credentials.GetClientNamespace(), *UserId);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerLobby::WritePartyStorage(const FString& PartyId, TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError, uint32 RetryAttempt)
	{
		TSharedPtr<PartyStorageWrapper> Wrapper = MakeShared<PartyStorageWrapper>();
		Wrapper->PartyId = PartyId;
		Wrapper->OnSuccess = OnSuccess;
		Wrapper->OnError = OnError;
		Wrapper->RemainingAttempt = RetryAttempt;
		Wrapper->PayloadModifier = PayloadModifier;
		WritePartyStorageRecursive(Wrapper);
	}

	void ServerLobby::GetPartyStorage(const FString & PartyId, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler & OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/parties/%s"), *Settings.LobbyServerUrl, *Credentials.GetClientNamespace(), *PartyId);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		FRegistry::HttpRetryScheduler.ProcessRequest(Request,
			CreateHttpResultHandler(THandler<FJsonObject>::CreateLambda([OnSuccess](FJsonObject result)
			{
				FAccelByteModelsPartyDataNotif PartyData;
				PartyData.UpdatedAt = result.GetStringField("updatedAt");
				TSharedRef<FJsonObject> JsonObjectRef = MakeShared<FJsonObject>(result);
				FJsonObjectConverter::JsonObjectToUStruct(JsonObjectRef, &PartyData, 0, 0);
				OnSuccess.Execute(PartyData);
			}), OnError),
			FPlatformTime::Seconds());
	}

	void ServerLobby::GetActiveParties(const int32& Limit, const int32& Offset, const THandler<FAccelByteModelsActivePartiesData>& OnSuccess, const FErrorHandler& OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		if (Limit <= 0)
		{
			OnError.ExecuteIfBound(404, TEXT("Limit must be greater than zero."));
			return;
		}
		if (Offset < 0) 
		{
			OnError.ExecuteIfBound(404, TEXT("Offset should not negative."));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/parties"), *Settings.LobbyServerUrl, *Credentials.GetClientNamespace());
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerLobby::RequestWritePartyStorage(const FString &PartyId, const FAccelByteModelsPartyDataUpdateRequest& PartyDataRequest, const THandler<FAccelByteModelsPartyDataNotif>& OnSuccess, const FErrorHandler& OnError, FSimpleDelegate OnConflicted)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/parties/%s/attributes"), *Settings.LobbyServerUrl, *Credentials.GetClientNamespace(), *PartyId);
		FString Verb = TEXT("PUT");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FString Contents = "{\n";
		FString CustomAttribute;
		FJsonObjectConverter::UStructToJsonObjectString(PartyDataRequest.Custom_attribute, CustomAttribute);
		FString UpdatedAt = FString::Printf(TEXT("\"updatedAt\": %lld"), PartyDataRequest.UpdatedAt);
		FString CustomString = FString::Printf(TEXT("\"custom_attribute\": %s"), *CustomAttribute);
		Contents += UpdatedAt;
		Contents += ",\n";
		Contents += CustomString;
		Contents += "}";

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Contents);

		FErrorHandler ErrorHandler = AccelByte::FErrorHandler::CreateLambda([OnConflicted](int32 Code, FString Message)
		{
			if (Code == (int32)ErrorCodes::StatusPreconditionFailed || Code == (int32)ErrorCodes::PartyStorageOutdatedUpdateData)
			{
				OnConflicted.ExecuteIfBound();
			}
		});

		FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, ErrorHandler), FPlatformTime::Seconds());
	}

	void ServerLobby::WritePartyStorageRecursive(TSharedPtr<PartyStorageWrapper> DataWrapper)
	{
		if (DataWrapper->RemainingAttempt <= 0)
		{
			DataWrapper->OnError.ExecuteIfBound(412, TEXT("Exhaust all retry attempt to modify party storage.."));
		}


		GetPartyStorage(DataWrapper->PartyId,
			THandler<FAccelByteModelsPartyDataNotif>::CreateLambda([this, DataWrapper](FAccelByteModelsPartyDataNotif Result)
			{
				Result.Custom_attribute = DataWrapper->PayloadModifier(Result.Custom_attribute);

				FAccelByteModelsPartyDataUpdateRequest PartyStorageBodyRequest;

				PartyStorageBodyRequest.UpdatedAt = FCString::Atoi64(*Result.UpdatedAt);
				PartyStorageBodyRequest.Custom_attribute = Result.Custom_attribute;

				RequestWritePartyStorage(DataWrapper->PartyId, PartyStorageBodyRequest, DataWrapper->OnSuccess, DataWrapper->OnError, FSimpleDelegate::CreateLambda([this, DataWrapper]() {
					DataWrapper->RemainingAttempt--;
					WritePartyStorageRecursive(DataWrapper);
				}));
			}),
			FErrorHandler::CreateLambda([DataWrapper](int32 ErrorCode, FString ErrorMessage)
			{
				DataWrapper->OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		);
	}
} // namespace GameServerApi
} // namespace AccelByte