// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerChatApi.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerChat, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteServerChat);

namespace AccelByte
{
namespace GameServerApi
{

#pragma region private utilities
namespace Json
{
namespace Field
{
	const FString JsonRPC = TEXT("jsonrpc");
	const FString Method = TEXT("method");
	const FString Params = TEXT("params");

	const FString Type = TEXT("type");
	const FString Name = TEXT("name");
	const FString Members = TEXT("members");
	const FString Admins = TEXT("admins");
	const FString Error = TEXT("error");
	const FString ErrorCode = TEXT("code");
	const FString ErrorMessage = TEXT("message");
	const FString Keyword = TEXT("keyword");
	const FString Offset = TEXT("offset");
	const FString Limit = TEXT("limit");
	const FString ChatIds = TEXT("chatIds");
	const FString Namespace = TEXT("namespace");
	const FString IsChannel = TEXT("isChannel");

	const FString ChatMessage = TEXT("message");
	const FString TopicId = TEXT("topicId");
	const FString UserId = TEXT("userId");
	const FString MessageId = TEXT("id");
	const FString IsJoinable = TEXT("isJoinable");

	const FString Result = TEXT("result");
	const FString Processed = TEXT("processed");
	const FString CreatedAt = TEXT("createdAt");
	const FString UpdatedAt = TEXT("updatedAt");
	const FString ReadAt = TEXT("readAt");

	const FString Data = TEXT("data");

}

namespace Value
{
	const FString JsonRPC = TEXT("2.0");
	const FString ChatGroupTypePrivate = TEXT("PERSONAL"); //! Chat group for 2 members only
	const FString ChatGroupTypePublic = TEXT("GROUP");  //!< Chat group for >2 members

	const uint8 IdDigitSuffixCount = 5;
}
	
}

template <typename T>
bool ConvertDateTimeFieldsFromUnixTimestamp(TSet<FString> const& InFieldNames
	, FJsonObject const& InJsonObject
	, T& OutObject)
{
	TSharedRef<FJsonObject> NewJsonObject = MakeShared<FJsonObject>(InJsonObject);
	for (const FString& FieldName : InFieldNames)
	{
		if(!NewJsonObject->HasField(FieldName))
		{
			UE_LOG(LogAccelByteServerChat, Warning, TEXT("Field %s not found when converting UnixTimestamp to Iso8601"), *FieldName);
			FJsonObjectConverter::JsonObjectToUStruct(NewJsonObject, &OutObject);
			return false;
		}
		
		int32 ProcessedUnixTime = NewJsonObject->GetIntegerField(FieldName);
		NewJsonObject->SetStringField(FieldName, FDateTime::FromUnixTimestamp(ProcessedUnixTime).ToIso8601());
	}
	
	FJsonObjectConverter::JsonObjectToUStruct(NewJsonObject, &OutObject);
	return true;
}

#pragma endregion 

ServerChat::ServerChat(ServerCredentials const& Credentials
	, ServerSettings const& Settings
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(Credentials, Settings, InHttpRef)
{}

ServerChat::~ServerChat()
{}

FAccelByteTaskWPtr ServerChat::CreateChannelChat(TSet<FString> const& MemberUserIds
	, TSet<FString> const& AdminUserIds
	, FString const& TopicName
	, THandler<FAccelByteModelsChatActionCreateTopicServerResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FAccelByteModelsChatCreateTopicRequest Body {
		TopicName,
		MemberUserIds,
		AdminUserIds,
		true
	};
	TSharedRef<FJsonObject> Params = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(FAccelByteModelsChatCreateTopicRequest::StaticStruct(), &Body, Params, 0, 0);
	Params.Get().SetStringField(Json::Field::Type, Json::Value::ChatGroupTypePublic);
	Params->SetBoolField(Json::Field::IsChannel, true);

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/topic")
		, *ServerSettingsRef.ChatServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	FString Content = TEXT("");
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>> > JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Content, 0);
	bool bSuccess = FJsonSerializer::Serialize(Params, JsonWriter);
	JsonWriter->Close();

	// need to convert "processed" field from unix timestamp to FDateTime manually.
	auto OnRequestSuccess{ THandler<FJsonObject>::CreateLambda([OnSuccess, OnError](const FJsonObject& Result)
	{
		FAccelByteModelsChatActionCreateTopicServerResponse ModifiedResponse;
		bool bConvertSuccess = ConvertDateTimeFieldsFromUnixTimestamp({Json::Field::CreatedAt}, Result, ModifiedResponse);
		if(!bConvertSuccess)
		{
			UE_LOG(LogAccelByteServerChat, Warning, TEXT("Failed converting Created At field"));
		}
		OnSuccess.ExecuteIfBound(ModifiedResponse);
	})};

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnRequestSuccess, OnError);
}

FAccelByteTaskWPtr ServerChat::DeleteChannelChat(FString const& TopicId
	, THandler<FAccelByteModelsChatActionTopicResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/topic/%s")
		, *ServerSettingsRef.ChatServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *TopicId);

	// need to convert "processed" field from unix timestamp to FDateTime manually.
	auto OnRequestSuccess{ THandler<FJsonObject>::CreateLambda(
		[OnSuccess, OnError](const FJsonObject& Result)
		{
			FAccelByteModelsChatActionTopicResponse ModifiedResponse;
			bool bConvertSuccess = ConvertDateTimeFieldsFromUnixTimestamp({Json::Field::Processed}, Result, ModifiedResponse);
			if (!bConvertSuccess)
			{
				OnError.ExecuteIfBound(0, "Error converting values UnixTimestamp to Iso8601");
			}
			OnSuccess.ExecuteIfBound(ModifiedResponse);
		})};

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnRequestSuccess, OnError);
}

FAccelByteTaskWPtr ServerChat::AddUserToChannelChat(FString const& TopicId
	, FString const& UserId
	, THandler<FAccelByteModelsChatActionUserTopicResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/topic/%s/user/%s")
		, *ServerSettingsRef.ChatServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *TopicId
		, *UserId);

	// need to convert "processed" field from unix timestamp to FDateTime manually.
	auto OnRequestSuccess{ THandler<FJsonObject>::CreateLambda(
		[OnSuccess, OnError](const FJsonObject& Result)
		{
			FAccelByteModelsChatActionUserTopicResponse ModifiedResponse;
			bool bConvertSuccess = ConvertDateTimeFieldsFromUnixTimestamp({Json::Field::Processed}, Result, ModifiedResponse);
			if(!bConvertSuccess)
			{
				OnError.ExecuteIfBound(0, "Error converting values UnixTimestamp to Iso8601");
			}
			OnSuccess.ExecuteIfBound(ModifiedResponse);
		})};

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnRequestSuccess, OnError);
}

FAccelByteTaskWPtr ServerChat::RemoveUserFromChannelChat(FString const& TopicId
	, FString const& UserId
	, THandler<FAccelByteModelsChatActionUserTopicResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/topic/%s/user/%s")
		, *ServerSettingsRef.ChatServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *TopicId
		, *UserId);

	// need to convert "processed" field from unix timestamp to FDateTime manually.
	auto OnRequestSuccess{ THandler<FJsonObject>::CreateLambda(
		[OnSuccess, OnError](const FJsonObject& Result)
		{
			FAccelByteModelsChatActionUserTopicResponse ModifiedResponse;
			bool bConvertSuccess = ConvertDateTimeFieldsFromUnixTimestamp({Json::Field::Processed}, Result, ModifiedResponse);
			if (!bConvertSuccess)
			{
				OnError.ExecuteIfBound(0, "Error converting values UnixTimestamp to Iso8601");
			}
			OnSuccess.ExecuteIfBound(ModifiedResponse);
		})};

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnRequestSuccess, OnError);
}

} // namespace GameServerApi
} // namespace AccelByte