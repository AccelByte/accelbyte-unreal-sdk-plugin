// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteChatApi.h"

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteSettings.h"
#include "Engine/Engine.h"
#include "Core/IWebSocketFactory.h"
#include "Core/FUnrealWebSocketFactory.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteChat, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteChat);

namespace AccelByte
{
	namespace Api
	{
#pragma region PRIVATE UTILITY
		namespace ChatToken
		{
			namespace Method
			{
				const FString Connect = TEXT("eventConnected");
				const FString Disconnect = TEXT("eventDisconnect");

				const FString SendChat = TEXT("sendChat");
				const FString ReadChat = TEXT("readChat");
				const FString CreateTopic = TEXT("actionCreateTopic");
				const FString QueryTopic = TEXT("actionQueryTopic");
				const FString QueryTopicById = TEXT("actionQueryTopicById");
				const FString QueryPersonalTopic = TEXT("actionQueryPersonalTopic");
				const FString QueryGroupTopic = TEXT("actionQueryGroupTopic");
				const FString QueryPublicTopic = TEXT("actionQueryPublicTopic");
				const FString QueryChat = TEXT("queryChat");

				const FString DeleteSystemMessages = TEXT("actionDeleteSystemMessages");
				const FString UpdateSystemMessages = TEXT("actionUpdateSystemMessages");
				const FString QuerySystemMessage = TEXT("actionQuerySystemMessage");
				const FString GetSystemMessageStats = TEXT("actionGetSystemMessageStats");

				const FString UpdateTopic = TEXT("actionUpdateTopic");
				const FString DeleteTopic = TEXT("actionDeleteTopic");
				const FString AddUserToTopic = TEXT("actionAddUserToTopic");
				const FString RemoveUserFromTopic = TEXT("actionRemoveUserFromTopic");

				const FString JoinTopic = TEXT("actionJoinTopic");
				const FString QuitTopic = TEXT("actionQuitTopic");

				const FString RefreshToken = TEXT("actionRefreshToken");

				const FString BlockUser = TEXT("actionBlockUser");
				const FString UnblockUser = TEXT("actionUnblockUser");

				//Events
				const FString ChatNotif = TEXT("eventNewChat");
				const FString ReadChatNotif = TEXT("eventReadChat");
				const FString AddToTopicNotif = TEXT("eventAddedToTopic"); 
				const FString RemoveFromTopicNotif = TEXT("EventRemovedFromTopic");
				const FString DeleteTopicNotif = TEXT("eventTopicDeleted");
				const FString UpdateTopicNotif = TEXT("eventTopicUpdated");
				const FString SystemMessageNotif  = TEXT("eventNewSystemMessage");

				const FString UserBanNotif = TEXT("eventBanChat");
				const FString UserUnbanNotif = TEXT("eventUnbanChat");
				const FString UserMutedNotif = TEXT("eventUserMuted");
				const FString UserUnmutedNotif = TEXT("eventUserUnmuted");
			}

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

					const FString ChatMessage = TEXT("message");
					const FString TopicId = TEXT("topicId");
					const FString UserId = TEXT("userId");
					const FString MessageId = TEXT("id");
					const FString IsJoinable = TEXT("isJoinable");
					const FString IsChannel = TEXT("isChannel");

					const FString Result = TEXT("result");
					const FString Processed = TEXT("processed");
					const FString CreatedAt = TEXT("createdAt");
					const FString UpdatedAt = TEXT("updatedAt");
					const FString ReadAt = TEXT("readAt");
					const FString ExpiredAt = TEXT("expiredAt");
					const FString StartCreatedAt = TEXT("startCreatedAt");
					const FString EndCreatedAt = TEXT("endCreatedAt");
					const FString OldestUnread = TEXT("oldestUnread");

					const FString Data = TEXT("data");

					const FString Token = TEXT("token");

					const FString SystemMessageIds = TEXT("messageIds");
					const FString SystemMessageRead = TEXT("read");
					const FString SystemMessageKeep = TEXT("keep");
					const FString UnreadOnly = TEXT("unreadOnly");
					const FString Category = TEXT("category");
				}

				namespace Value
				{
					const FString JsonRPC = TEXT("2.0");
					const FString ChatGroupTypePrivate = TEXT("PERSONAL"); //! Chat group for 2 members only
					const FString ChatGroupTypePublic = TEXT("GROUP");  //!< Chat group for >2 members

					const uint8 IdDigitSuffixCount = 5;
				}

			}
		}

		enum HandleType : uint8
		{
			Invalid,
			Connect,
			Disconnect,

			//Response
			SendChatResponse,
			ReadChatResponse,
			CreateTopicResponse,
			QueryTopicResponse,
			QueryTopicByIdResponse,
			QueryPersonalTopicResponse,
			QueryGroupTopicResponse,
			QueryPublicTopicResponse,
			QueryChatResponse,
			UpdateTopicResponse,
			DeleteTopicResponse,
			AddUserToTopicResponse,
			RemoveUserFromTopicResponse,
			JoinTopicResponse,
			QuitTopicResponse,

			DeleteSystemMessagesResponse,
			UpdateSystemMessagesResponse,
			QuerySystemMessageResponse,
			GetSystemMessageStatsResponse,

			RefreshTokenResponse,

			BlockUserResponse,
			UnblockUserResponse,
			
			//notif
			ChatNotif,
			ReadChatNotif,
			RemoveFromTopicNotif,
			AddToTopicNotif,
			DeleteTopicNotif,
			UpdateTopicNotif,
			SystemMessageNotif,
			UserMutedNotif,
			UserUnmutedNotif,
			
			UserBanNotif,
			UserUnbanNotif,
		};

		namespace IncomingMessage
		{
			/**
			 * @brief Deterministic handler type enum getter from an incoming message json object
			 * @param HandlerStringEnumMap - static map as dictionary
			 * @param MessageAsJsonObj - Incoming message as JsonObject
			 * @return Enum of message notif/response for switch case handler
			 */
			HandleType GetHandleType(const TMap<FString, HandleType>& HandlerStringEnumMap, const TSharedPtr<FJsonObject>& MessageAsJsonObj)
			{

				FString StringKey = TEXT("");
				if (const bool bIsNotif = MessageAsJsonObj->HasField(ChatToken::Json::Field::Method))
				{
					//Check the method for connect and disconnect
					StringKey = MessageAsJsonObj->GetStringField(ChatToken::Json::Field::Method);
				}
				else if (const bool bIsResponse = MessageAsJsonObj->HasField(ChatToken::Json::Field::MessageId))
				{
					TSharedPtr<FJsonValue> IDValue = MessageAsJsonObj->TryGetField(ChatToken::Json::Field::MessageId);
					const FString MessageId = IDValue.Get()->AsString();
					StringKey = MessageId.LeftChop(ChatToken::Json::Value::IdDigitSuffixCount);
				}
				else
				{
					UE_LOG(LogAccelByteChat, Warning, TEXT("Incoming message dont have a recognizeable handle type"));
				}

				HandleType HandleType = HandleType::Invalid;
				if (HandlerStringEnumMap.Contains(StringKey))
				{
					HandleType = HandlerStringEnumMap[StringKey];
				}

				return HandleType;
			}

			/**
			 * @brief Time field value converter from original backend Unix to internal Iso8601
			 * @param JsonObject - Incoming Message as Json Object that will has it's DateTimeField value updated
			 * @param DateTimeField - Json time field name 
			 * @return true if a date time field has been converted
			 */
			bool ConvertJsonFieldTimeUnixToIso8601(TSharedPtr<FJsonObject> JsonObject, const FString& DateTimeField)
			{
				bool bUpdatedObject = false;
				if (JsonObject->HasField(DateTimeField))
				{
					const int UnixTime = JsonObject->GetIntegerField(DateTimeField);
					const FString Iso8601Time = 
						UnixTime == 0 ? 
						FDateTime(0).ToIso8601() :
						FDateTime().FromUnixTimestamp(UnixTime).ToIso8601();

					JsonObject->SetStringField(DateTimeField, Iso8601Time);
					bUpdatedObject = true;

					{ // Log
						const int ReferenceTime = FDateTime::Now().ToUnixTimestamp();
						const FString ReferenceIso8601Time = FDateTime().FromUnixTimestamp(ReferenceTime).ToIso8601();
						UE_LOG(LogAccelByteChat, VeryVerbose, TEXT("reference now\t\t\t. Json: %d -> %s\n\t\t\tupdated [%s] field\t\t. Json: %d -> %s"), ReferenceTime, *ReferenceIso8601Time, *DateTimeField, UnixTime, *Iso8601Time);
					}
				}

				return bUpdatedObject;
			}

			/**
			 * @brief Higher level for multiple date time fields Unix to FDateTime converter
			 * @param Message - Incoming Message as Json Object that will has it's DateTimeField value updated
			 * @return true if a date time field has been converted
			 */
			bool ConvertJsonTimeFormatToFDateTimeFriendly(const TSharedPtr<FJsonObject>& Message)
			{
				bool bHasDateTimeJsonField = false;

				// all known date time fields in chat service specification
				TSet<FString> FieldDataDateTimeFields;
				FieldDataDateTimeFields.Add(ChatToken::Json::Field::CreatedAt);
				FieldDataDateTimeFields.Add(ChatToken::Json::Field::UpdatedAt);
				FieldDataDateTimeFields.Add(ChatToken::Json::Field::ReadAt);
				FieldDataDateTimeFields.Add(ChatToken::Json::Field::ExpiredAt);

				FString SubObjectField, RootDateTimeField;
				if (Message->HasField(ChatToken::Json::Field::Params)) // Notif
				{
					SubObjectField = ChatToken::Json::Field::Params;

					const FString Method = Message->GetStringField(ChatToken::Json::Field::Method);
					if (Method.Equals(ChatToken::Method::UserMutedNotif))
					{
						RootDateTimeField = ChatToken::Json::Field::ExpiredAt;
					}
					else
					{
						RootDateTimeField = ChatToken::Json::Field::CreatedAt;
					}
				}
				else if(Message->HasField(ChatToken::Json::Field::Result))// Response
				{
					SubObjectField = ChatToken::Json::Field::Result;
					RootDateTimeField = ChatToken::Json::Field::Processed;
				}

				if (!SubObjectField.IsEmpty())
				{
					TSharedPtr<FJsonObject> SubObject = Message->GetObjectField(SubObjectField);
					bHasDateTimeJsonField = ConvertJsonFieldTimeUnixToIso8601(SubObject, RootDateTimeField);
					if (bHasDateTimeJsonField)
					{
						if(RootDateTimeField != ChatToken::Json::Field::ExpiredAt
							&& SubObject->HasField(ChatToken::Json::Field::ExpiredAt))
						{
							ConvertJsonFieldTimeUnixToIso8601(SubObject, ChatToken::Json::Field::ExpiredAt);
						}
						// Avoid re-converting createdAt field
						if(RootDateTimeField != ChatToken::Json::Field::CreatedAt
							&& SubObject->HasField(ChatToken::Json::Field::CreatedAt))
						{
							ConvertJsonFieldTimeUnixToIso8601(SubObject, ChatToken::Json::Field::CreatedAt);
						}
						if(RootDateTimeField != ChatToken::Json::Field::OldestUnread
							&& SubObject->HasField(ChatToken::Json::Field::OldestUnread))
						{
							ConvertJsonFieldTimeUnixToIso8601(SubObject, ChatToken::Json::Field::OldestUnread);
						}

						if (SubObject->HasField(ChatToken::Json::Field::Data))
						{
							TArray<TSharedPtr<FJsonValue>> Datas;
							//Field Data can be an array. e.g. FAccelByteModelsChatQueryTopicResponse
							if (SubObject->HasTypedField<EJson::Array>(ChatToken::Json::Field::Data))
							{
								Datas = SubObject->GetArrayField(ChatToken::Json::Field::Data);
							}
							// or a single value. e.g. FAccelByteModelsChatQueryTopicByIdResponse
							else if(SubObject->HasField(ChatToken::Json::Field::Data))
							{
								const TSharedPtr<FJsonValue> aSingleData = SubObject->TryGetField(ChatToken::Json::Field::Data);
								Datas.Add(aSingleData);
							}

							for (const TSharedPtr<FJsonValue>& Data : Datas) // multiple data
							{
								TSharedPtr<FJsonObject> SubDataObject = Data.Get()->AsObject();
								for (const FString& aDataDateTimeField : FieldDataDateTimeFields) // possible several date field in a data
								{
									ConvertJsonFieldTimeUnixToIso8601(SubDataObject, aDataDateTimeField);
								}
							}
						}
					}
				}

				return bHasDateTimeJsonField;
			}

		} // namespace IncomingMessage

#define FORM_STRING_ENUM_PAIR(MessageType) \
    { ChatToken::Method::MessageType, HandleType::MessageType } \

#define FORM_STRING_ENUM_PAIR_RESPONSE(MessageType) \
    { ChatToken::Method::MessageType, HandleType::MessageType ## Response } \

		TMap<FString, HandleType> Chat::HandlerStringEnumMap
		{
			FORM_STRING_ENUM_PAIR(Connect),
			FORM_STRING_ENUM_PAIR(Disconnect),
			
			FORM_STRING_ENUM_PAIR_RESPONSE(SendChat),
			FORM_STRING_ENUM_PAIR_RESPONSE(ReadChat),
			FORM_STRING_ENUM_PAIR_RESPONSE(CreateTopic),
			FORM_STRING_ENUM_PAIR_RESPONSE(QueryTopic),
			FORM_STRING_ENUM_PAIR_RESPONSE(QueryTopicById),
			FORM_STRING_ENUM_PAIR_RESPONSE(QueryPersonalTopic),
			FORM_STRING_ENUM_PAIR_RESPONSE(QueryGroupTopic),
			FORM_STRING_ENUM_PAIR_RESPONSE(QueryPublicTopic),
			FORM_STRING_ENUM_PAIR_RESPONSE(QueryChat),
			FORM_STRING_ENUM_PAIR_RESPONSE(UpdateTopic),
			FORM_STRING_ENUM_PAIR_RESPONSE(DeleteTopic),
			FORM_STRING_ENUM_PAIR_RESPONSE(AddUserToTopic),
			FORM_STRING_ENUM_PAIR_RESPONSE(RemoveUserFromTopic),
			FORM_STRING_ENUM_PAIR_RESPONSE(JoinTopic),
			FORM_STRING_ENUM_PAIR_RESPONSE(QuitTopic),
			FORM_STRING_ENUM_PAIR_RESPONSE(RefreshToken),
			FORM_STRING_ENUM_PAIR_RESPONSE(DeleteSystemMessages),
			FORM_STRING_ENUM_PAIR_RESPONSE(UpdateSystemMessages),
			FORM_STRING_ENUM_PAIR_RESPONSE(QuerySystemMessage),
			FORM_STRING_ENUM_PAIR_RESPONSE(GetSystemMessageStats),

			FORM_STRING_ENUM_PAIR_RESPONSE(BlockUser),
			FORM_STRING_ENUM_PAIR_RESPONSE(UnblockUser),


			FORM_STRING_ENUM_PAIR(ChatNotif),
			FORM_STRING_ENUM_PAIR(ReadChatNotif),
			FORM_STRING_ENUM_PAIR(AddToTopicNotif),
			FORM_STRING_ENUM_PAIR(RemoveFromTopicNotif),
			FORM_STRING_ENUM_PAIR(DeleteTopicNotif),
			FORM_STRING_ENUM_PAIR(UpdateTopicNotif),
			FORM_STRING_ENUM_PAIR(SystemMessageNotif),

			FORM_STRING_ENUM_PAIR(UserBanNotif),
			FORM_STRING_ENUM_PAIR(UserUnbanNotif),

			FORM_STRING_ENUM_PAIR(UserMutedNotif),
			FORM_STRING_ENUM_PAIR(UserUnmutedNotif)
		};

#undef FORM_STRING_ENUM_PAIR

#pragma endregion

		Chat::Chat(Credentials& InCredentialsRef
			, Settings const& InSettingsRef
			, FHttpRetryScheduler& InHttpRef
			, float PingDelay
			, float InitialBackoffDelay
			, float MaxBackoffDelay
			, float TotalTimeout
			, TSharedPtr<IWebSocket> WebSocket)
			: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
			, ChatCredentialsRef{InCredentialsRef}
			, PingDelay{PingDelay}
			, InitialBackoffDelay{InitialBackoffDelay}
			, MaxBackoffDelay{MaxBackoffDelay}
			, TotalTimeout{TotalTimeout}
			, BackoffDelay{InitialBackoffDelay}
			, RandomizedBackoffDelay{InitialBackoffDelay}
			, TimeSinceLastPing{0.0f}
			, TimeSinceLastReconnect{0.0f}
			, TimeSinceConnectionLost{0}
		{
		}

		Chat::~Chat() {}

#pragma region CONNECTION

		void Chat::CreateWebSocket()
		{
			if (WebSocket.IsValid())
			{
				WebSocket.Reset();
			}
			
			TMap<FString, FString> Headers;
			Headers.Add(ChatSessionHeaderName, ChatSessionId.SessionID);
			Headers.Add(TEXT("X-Ab-RpcEnvelopeStart"), WsEnvelopeStart);
			Headers.Add(TEXT("X-Ab-RpcEnvelopeEnd"), WsEnvelopeEnd);
			FAccelByteUtilities::AppendModulesVersionToMap(Headers);

			WebSocket = AccelByteWebSocket::Create(*SettingsRef.ChatServerWsUrl, TEXT("wss"), CredentialsRef, Headers, TSharedRef<IWebSocketFactory>(new FUnrealWebSocketFactory()), PingDelay, InitialBackoffDelay, MaxBackoffDelay, TotalTimeout);

			WebSocket->OnConnected().AddRaw(this, &Chat::OnConnected);
			WebSocket->OnMessageReceived().AddRaw(this, &Chat::OnMessage);
			WebSocket->OnConnectionError().AddRaw(this, &Chat::OnConnectionError);
			WebSocket->OnConnectionClosed().AddRaw(this, &Chat::OnClosed);
		}

		void Chat::Connect()
		{
			FReport::Log(FString(__FUNCTION__));

			if (!WebSocket.IsValid())
			{
				CreateWebSocket();
			}

			if (WebSocket->IsConnected())
			{
				return;
			}

			WebSocket->Connect();
			UE_LOG(LogAccelByteChat, Log, TEXT("Connecting to %s"), *SettingsRef.ChatServerWsUrl);

		}

		void Chat::Disconnect()
		{
			FReport::Log(FString(__FUNCTION__));
			ChatCredentialsRef.OnTokenRefreshed().Remove(TokenRefreshDelegateHandle);
			if (WebSocket.IsValid())
			{
				WebSocket->Disconnect();
			}

			if (GEngine) UE_LOG(LogAccelByteChat, Log, TEXT("Disconnected"));
		}

		bool Chat::IsConnected() const
		{
			return WebSocket.IsValid() && WebSocket->IsConnected();
		}

		void Chat::SendPing()
		{
			FReport::Log(FString(__FUNCTION__));

			if (WebSocket.IsValid() && WebSocket->IsConnected())
			{
				WebSocket->Send(FString());
			}
		}

		void Chat::OnConnected()
		{
			UE_LOG(LogAccelByteChat, Log, TEXT("Connected"));
			TokenRefreshDelegateHandle = ChatCredentialsRef.OnTokenRefreshed().AddLambda([this](bool bSuccess)
			{
				if (bSuccess)
				{
					RefreshToken(CredentialsRef.GetAccessToken(), RefreshTokenResponse);
				}
				else
				{
					Disconnect();
				}
			});
			ConnectSuccess.ExecuteIfBound();
		}

		void Chat::OnConnectionError(const FString& Error)
		{
			UE_LOG(LogAccelByteChat, Warning, TEXT("Error connecting: %s"), *Error);
			ConnectError.ExecuteIfBound(static_cast<std::underlying_type<ErrorCodes>::type>(ErrorCodes::WebSocketConnectFailed), ErrorMessages::Default.at(static_cast<std::underlying_type<ErrorCodes>::type>(ErrorCodes::WebSocketConnectFailed)) + TEXT(" Reason: ") + Error);
		}

		void Chat::OnClosed(int32 StatusCode
			, const FString& Reason
			, bool WasClean)
		{
			// disconnect only if status code > 4000 and we don't receive a login ban,
			// other ban will try to reconnect the websocket
			if (StatusCode > 4000 && !(bBanNotifReceived && BanType != EBanType::LOGIN))
			{
				Disconnect();
			}
			else
			{
				WebSocket->Reconnect();
			}

			bBanNotifReceived = false;
			BanType = EBanType::EMPTY;
			
			UE_LOG(LogAccelByteChat, Log, TEXT("Connection closed. Status code: %d  Reason: %s Clean: %d"), StatusCode, *Reason, WasClean);
			ConnectionClosed.ExecuteIfBound(StatusCode, Reason, WasClean);
		}

#pragma endregion // CONNECTION

#pragma region DELEGATE HANDLERS

		void Chat::UnbindEvent()
		{
			ClearCachedMessageIdResponseMaps();
			ClearResponseHandlers();
			ClearErrorHandlers();
			ClearNotificationHandlers();
		}

		void Chat::ClearCachedMessageIdResponseMaps()
		{
			MessageIdCreateTopicResponseMap.Empty();
			MessageIdSendChatResponseMap.Empty();
			MessageIdQueryTopicResponseMap.Empty();
			MessageIdQueryTopicByIdResponseMap.Empty();
			MessageIdQueryPersonalTopicResponseMap.Empty();
			MessageIdQueryGroupTopicResponseMap.Empty();
			MessageIdQueryPublicTopicResponseMap.Empty();
			MessageIdQueryChatResponseMap.Empty();
			MessageIdReadChatResponseMap.Empty();
			MessageIdUpdateTopicResponseMap.Empty();
			MessageIdDeleteTopicResponseMap.Empty();
			MessageIdAddUserToTopicResponseMap.Empty();
			MessageIdRemoveUserFromTopicResponseMap.Empty();
			MessageIdJoinTopicResponseMap.Empty();
			MessageIdQuitTopicResponseMap.Empty();
			MessageIdRefreshTokenResponseMap.Empty();
			MessageIdBlockUserResponseMap.Empty();
			MessageIdUnblockUserResponseMap.Empty();
			MessageIdUpdateSystemMessagesResponseMap.Empty();
			MessageIdDeleteSystemMessagesResponseMap.Empty();
			MessageIdQuerySystemMessageResponseMap.Empty();
		}

		void Chat::ClearResponseHandlers()
		{
			CreateTopicResponse.Unbind();
			SendChatResponse.Unbind();
			QueryTopicResponse.Unbind();
			QueryPersonalTopicResponse.Unbind();
			QueryGroupTopicResponse.Unbind();
			QueryChatResponse.Unbind();
			ReadChatResponse.Unbind();
			UpdateTopicResponse.Unbind();
			DeleteTopicResponse.Unbind();
			AddUserToTopicResponse.Unbind();
			RemoveUserFromTopicResponse.Unbind();
			JoinTopicResponse.Unbind();
			QuitTopicResponse.Unbind();
			BlockUserResponse.Unbind();
			UnblockUserResponse.Unbind();
			UpdateSystemMessagesResponse.Unbind();
			DeleteSystemMessagesResponse.Unbind();
		}

		void Chat::ClearErrorHandlers()
		{
			OnCreateTopicError.Unbind();
			OnSendChatError.Unbind();
			OnQueryTopicError.Unbind();
			OnQueryPersonalTopicError.Unbind();
			OnQueryGroupTopicError.Unbind();
			OnQueryChatError.Unbind();
			OnReadChatError.Unbind();
			OnUpdateTopicError.Unbind();
			OnDeleteTopicError.Unbind();
			OnAddUserToTopicError.Unbind();
			OnRemoveUserFromTopicError.Unbind();
			OnJoinTopicError.Unbind();
			OnQuitTopicError.Unbind();
			OnBlockUserError.Unbind();
			OnUnblockUserError.Unbind();
			OnUpdateSystemMessagesError.Unbind();
			OnDeleteSystemMessagesError.Unbind();
		}

		void Chat::ClearNotificationHandlers()
		{
			ChatNotif.Unbind();
			ReadChatNotif.Unbind();
			AddToTopicNotif.Unbind();
			RemoveFromTopicNotif.Unbind();
			DeleteTopicNotif.Unbind();
			UpdateTopicNotif.Unbind();
			SystemMessageNotif.Unbind();
		}

#pragma endregion // DELEGATE HANDLERS

#pragma region RESPONSE / NOTIF (Incoming Message)

		/**
		*@brief templated callback handler
		*@tparam OnError Failure callback
		*@tparam MessageAsJsonObj response message as json object
		*@tparam [in out] IdResponseMap map containing cached id message and it's respective success delegate
		*/
		template <typename FSuccessModel, typename FOnSuccess, typename FOnError>
		void HandleSuccessErrorResponse(const FOnError& OnError
			, const TSharedPtr<FJsonObject>& MessageAsJsonObj
			, TMap<FString, FOnSuccess>& IdResponseMap)
		{
			TSharedPtr<FJsonValue> IDValue = MessageAsJsonObj->TryGetField(ChatToken::Json::Field::MessageId);
			const FString MessageId = IDValue.Get()->AsString();
			if (const bool bIsError = MessageAsJsonObj->HasField(ChatToken::Json::Field::Error))
			{
				const TSharedPtr<FJsonObject> ErrorJsonObj = MessageAsJsonObj->GetObjectField(ChatToken::Json::Field::Error);
				const int ErrorCode = ErrorJsonObj->GetIntegerField(ChatToken::Json::Field::ErrorCode);
				const FString ErrorMessage = ErrorJsonObj->GetStringField(ChatToken::Json::Field::ErrorMessage);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}
			else if (IdResponseMap.Contains(MessageId))
			{
				FOnSuccess* IdResponse = IdResponseMap.Find(MessageId);
				check(IdResponse);
				FSuccessModel Result;
				TSharedPtr<FJsonObject> JsonResult = MessageAsJsonObj->GetObjectField(ChatToken::Json::Field::Result);

				if (FJsonObjectConverter::JsonObjectToUStruct(JsonResult.ToSharedRef(), &Result, 0, 0))
				{
					IdResponse->ExecuteIfBound(Result);
				}
				IdResponseMap.Remove(MessageId);
			}
			else
			{
				UE_LOG(LogAccelByteChat, Error, TEXT("Incoming Message Id [%s] has no designated callback"), *MessageId);
			}
		}

#define CASE_NOTIF_EXPLICIT_MODEL(MessageType, Model) \
	case (HandleType::MessageType): \
	{\
		Model Data;\
		TSharedPtr<FJsonObject> Result = MessageAsJsonObj->GetObjectField(ChatToken::Json::Field::Params);\
		if (const bool bSuccess = FJsonObjectConverter::JsonObjectToUStruct(Result.ToSharedRef(), &Data, 0, 0))\
		{\
			MessageType.ExecuteIfBound(Data);\
		}\
		break;\
	}

#define ID_RESPONSE_MAP(MessageType) MessageId ## MessageType ## ResponseMap
#define MESSAGE_ERROR_HANDLER(MessageType) On ## MessageType ## Error
#define MESSAGE_SUCCESS_HANDLER(MessageType) MessageType ## Response
#define MODEL_NOTIF(MessageType) FAccelByteModelsChat ## MessageType
#define CASE_NOTIF(MessageType) CASE_NOTIF_EXPLICIT_MODEL(MessageType, MODEL_NOTIF(MessageType))
#define DELEGATE_TYPE(MessageType, ReplyType) F ## MessageType ## ReplyType

/**
* @brief convenient switch case for RESPONSE context; will cache the (MessageId,SuccessDelegate) pair
* @param DelegateType explicit param for cases/responses that has different format naming between MessageType and DelegateType
*/
#define CASE_RESPONSE_ID_EXPLICIT_MODEL(MessageType, Model) \
	case (HandleType::MessageType ## Response) : \
		{ \
			HandleSuccessErrorResponse<Model>(MESSAGE_ERROR_HANDLER(MessageType), MessageAsJsonObj, ID_RESPONSE_MAP(MessageType));\
			break; \
		} \

#define MODEL_RESPONSE(MessageType) FAccelByteModelsChat ## MessageType ## Response
#define CASE_RESPONSE_ID(MessageType) CASE_RESPONSE_ID_EXPLICIT_MODEL(MessageType, MODEL_RESPONSE(MessageType))

		void Chat::ProcessFragmentedMessage(const FString& InMessage
			, const FString& InEnvelopeStart
			, const FString& InEnvelopeEnd
			, FString& InOutEnvelopeBuffer
			, FString& OutMessage
			, bool& OutIsMessageEnd)
		{
			OutMessage = "";
			if(!InEnvelopeStart.IsEmpty() || !InEnvelopeStart.IsEmpty())
			{
				FString MessageCopy {InMessage};
				if(!InEnvelopeStart.IsEmpty() && MessageCopy.StartsWith(InEnvelopeStart))
				{
					InOutEnvelopeBuffer = "";
					MessageCopy.RemoveFromStart(InEnvelopeStart);
				}

				InOutEnvelopeBuffer.Append(MessageCopy);

				if(InEnvelopeEnd.IsEmpty())
				{
					UE_LOG(LogAccelByteChat, Warning, TEXT("WsEnvelopeEnd is empty string, "
						"event though WsEnvelopeStart is not empty.\nWill not detect fragmented message"));
				}
				else
				{
					if(!InOutEnvelopeBuffer.EndsWith(InEnvelopeEnd))
					{
						// message is fragmented, should wait next message
						OutIsMessageEnd = false;
						return;
					}
					else
					{
						InOutEnvelopeBuffer.RemoveFromEnd(InEnvelopeEnd);
					}
				}

				OutMessage = InOutEnvelopeBuffer;
				InOutEnvelopeBuffer = "";
			}

			OutIsMessageEnd = true;
		}
		
		void Chat::OnMessage(const FString& Message)
		{
			UE_LOG(LogAccelByteChat, Log, TEXT("Raw Message\n%s"), *Message);

			if (Message.IsEmpty())
			{
				return;
			}

			bool bIsFragmentedEnd {false};
			FString ProcessedMessage;
			ProcessFragmentedMessage(Message, WsEnvelopeStart, WsEnvelopeEnd, EnvelopeContentBuffer, ProcessedMessage, bIsFragmentedEnd);

			if(!bIsFragmentedEnd)
				return;

			TSharedPtr<FJsonObject> MessageAsJsonObj;
			TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(ProcessedMessage);
			if (!FJsonSerializer::Deserialize(JsonReader, MessageAsJsonObj))
			{
				UE_LOG(LogAccelByteChat, Warning, TEXT("Failed to Deserialize Chat Message. Json: %s"), *ProcessedMessage);
				return;
			}

			IncomingMessage::ConvertJsonTimeFormatToFDateTimeFriendly(MessageAsJsonObj);

			const HandleType HandleType = IncomingMessage::GetHandleType(HandlerStringEnumMap, MessageAsJsonObj);

			switch (HandleType)
			{
				case(HandleType::Connect):
				{
					TSharedPtr<FJsonObject> Result = MessageAsJsonObj->GetObjectField(ChatToken::Json::Field::Params);
					const bool bSuccess = FJsonObjectConverter::JsonObjectToUStruct(Result.ToSharedRef(), &ChatSessionId, 0, 0);
					if (bSuccess)
					{
						WebSocket->UpdateUpgradeHeaders(ChatSessionHeaderName, ChatSessionId.SessionID);
					}
					break;
				}
				case(HandleType::Disconnect):
				{
					FAccelByteModelsChatDisconnectNotif ChatDisconnectNotif;
					DisconnectNotif.ExecuteIfBound(ChatDisconnectNotif);
					break;
				}

				CASE_RESPONSE_ID_EXPLICIT_MODEL(CreateTopic, FAccelByteModelsChatActionTopicResponse)
				CASE_RESPONSE_ID(SendChat)
				CASE_RESPONSE_ID(QueryTopic)
				CASE_RESPONSE_ID(QueryTopicById)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(QueryPersonalTopic, FAccelByteModelsChatQueryTopicResponse)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(QueryGroupTopic, FAccelByteModelsChatQueryTopicResponse)
				CASE_RESPONSE_ID(QueryPublicTopic)
				CASE_RESPONSE_ID(QueryChat)
				CASE_RESPONSE_ID(ReadChat)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(UpdateTopic, FAccelByteModelsChatActionTopicResponse)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(DeleteTopic, FAccelByteModelsChatActionTopicResponse)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(AddUserToTopic, FAccelByteModelsChatActionUserTopicResponse)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(RemoveUserFromTopic, FAccelByteModelsChatActionUserTopicResponse)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(JoinTopic, FAccelByteModelsChatActionTopicResponse)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(QuitTopic, FAccelByteModelsChatActionTopicResponse)
				CASE_RESPONSE_ID(RefreshToken)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(BlockUser, FAccelByteModelsChatBlockUserResponse)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(UnblockUser, FAccelByteModelsChatUnblockUserResponse)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(UpdateSystemMessages, FAccelByteModelsUpdateSystemMessagesResponse)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(DeleteSystemMessages, FAccelByteModelsDeleteSystemMessagesResponse)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(QuerySystemMessage, FAccelByteModelsQuerySystemMessagesResponse)
				CASE_RESPONSE_ID_EXPLICIT_MODEL(GetSystemMessageStats, FAccelByteGetSystemMessageStatsResponse)

				CASE_NOTIF_EXPLICIT_MODEL(ChatNotif, FAccelByteModelsChatNotif)
				CASE_NOTIF_EXPLICIT_MODEL(ReadChatNotif, FAccelByteModelsReadChatNotif)
				CASE_NOTIF_EXPLICIT_MODEL(AddToTopicNotif, FAccelByteModelsChatUpdateUserTopicNotif)
				CASE_NOTIF_EXPLICIT_MODEL(RemoveFromTopicNotif, FAccelByteModelsChatUpdateUserTopicNotif)
				CASE_NOTIF_EXPLICIT_MODEL(DeleteTopicNotif, FAccelByteModelsChatUpdateTopicNotif)
				CASE_NOTIF_EXPLICIT_MODEL(SystemMessageNotif, FAccelByteModelsChatSystemMessageNotif)
				CASE_NOTIF_EXPLICIT_MODEL(UserMutedNotif, FAccelByteModelsChatMutedNotif)
				CASE_NOTIF_EXPLICIT_MODEL(UserUnmutedNotif, FAccelByteModelsChatUnmutedNotif)
				CASE_NOTIF(UpdateTopicNotif)
				// Shadow Ban
				case (HandleType::UserBanNotif): // intended fallthrough
				case (HandleType::UserUnbanNotif):
				{
					bBanNotifReceived = true;
					FAccelByteModelsChatUserBanUnbanNotif Data;
					ChatCredentialsRef.OnTokenRefreshed().Remove(TokenRefreshDelegateHandle);
					TSharedPtr<FJsonObject> Result = MessageAsJsonObj->GetObjectField(ChatToken::Json::Field::Params);
					if (const bool bParseSuccess = FJsonObjectConverter::JsonObjectToUStruct(Result.ToSharedRef(), &Data, 0, 0))
					{
						HttpRef.BearerAuthRejected();
						if (HandleType == HandleType::UserBanNotif)
						{
							UserBanNotif.ExecuteIfBound(Data);
						}
						else if (HandleType == HandleType::UserUnbanNotif)
						{
							UserUnbanNotif.ExecuteIfBound(Data);
						}
					}
					break;
				}
				default:
					ParsingError.ExecuteIfBound(-1, FString::Printf(TEXT("Error; no suitable handler found. Raw: %s"), *Message));
					break;
			}

		}

#undef MESSAGE_ERROR_HANDLER
#undef CASE_RESPONSE_MESSAGE_ID
#undef DELEGATE_TYPE
#undef CASE_RESPONSE_ID
#undef CASE_RESPONSE_ID_EXPLICIT_MODEL
#undef MODEL_RESPONSE
#undef CASE_NOTIF
#undef MODEL_NOTIF
#undef CASE_NOTIF_EXPLICIT_MODEL

#pragma endregion // RESPONSE / NOTIF (Incoming Message)

#pragma region REQUEST (Outgoing Message)

		FString Chat::GenerateMessageID(const FString& Prefix) const
		{
			const FString MessageId = FString::Printf(TEXT("%s-%d"), *Prefix, FMath::RandRange(1000, 9999));

#if !UE_BUILD_SHIPPING
			const FString ValidKey = MessageId.LeftChop(ChatToken::Json::Value::IdDigitSuffixCount);
			if (!HandlerStringEnumMap.Contains(ValidKey))
			{
				UE_LOG(LogAccelByteChat, Warning, TEXT("MessageId:[%s] -> Key:[%s]"), *MessageId, *ValidKey)
			}
			check(HandlerStringEnumMap.Contains(ValidKey) && "MessageId has correct format with fixed suffix count");
#endif
			return MessageId;
		}

		// Assign the delegates, generate message id, send the content and cache the handlers
#define SEND_CONTENT_CACHE_ID(MessageType)\
		Set ## MessageType ## ResponseDelegate(OnSuccess, OnError);\
		const FString MessageId = SendWebSocketContent(ChatToken::Method::MessageType, Params);\
		ID_RESPONSE_MAP(MessageType).Emplace(MessageId, MESSAGE_SUCCESS_HANDLER(MessageType));\

		// Request model can be generated using the message type using a predefined format
#define REQUEST_MODEL(MessageType) FAccelByteModelsChat ## MessageType ## Request

		// Sending a request internally is all the same, only differs in MessageType
#define SEND_REQUEST_EXPLICIT_MODEL(MessageType, RequestModel)\
		FReport::Log(FString(__FUNCTION__));\
		TSharedRef<FJsonObject> Params = MakeShared<FJsonObject>();\
		FJsonObjectConverter::UStructToJsonObject(RequestModel::StaticStruct(),&Request,Params,0, 0);\
		SEND_CONTENT_CACHE_ID(MessageType)\

		// Macro to reduce boiler plate code for sending request through websocket that cant be done using template function
#define SEND_REQUEST(MessageType) SEND_REQUEST_EXPLICIT_MODEL(MessageType, REQUEST_MODEL(MessageType))

		void Chat::CreatePersonalTopic(const FString& TargetUserId
			, const FChatActionTopicResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			if (!ValidateAccelByteId(TargetUserId, EAccelByteIdHypensRule::NO_HYPENS
				, FAccelByteIdValidator::GetUserIdInvalidMessage(TargetUserId)
				, OnError))
			{
				return;
			}

			const FString Self = CredentialsRef.GetUserId();
			FJsonDomBuilder::FObject Params;
			Params.Set(ChatToken::Json::Field::Type, ChatToken::Json::Value::ChatGroupTypePrivate);
			Params.Set(ChatToken::Json::Field::Namespace, CredentialsRef.GetNamespace());
			FJsonDomBuilder::FArray Members;
			Members.Add(Self);
			Members.Add(TargetUserId);
			Params.Set(ChatToken::Json::Field::Members, Members);

			SEND_CONTENT_CACHE_ID(CreateTopic)
		}

		void Chat::CreateGroupTopic(const FAccelByteModelsChatCreateTopicRequest& Request
			, const FChatActionTopicResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FReport::LogDeprecated(FString(__FUNCTION__),
				TEXT("Manual topic creation is deprecated - please use V2 Sessions to auto-create chat topics"));

			if (Request.Members.Num() == 0)
			{
				UE_LOG(LogAccelByteChat, Warning, TEXT("Cant create group with empty user, need at least 1 other user (excluding self)"))
				return;
			}

			const FString Self = CredentialsRef.GetUserId();

			FAccelByteModelsChatCreateTopicRequest ToSendRequest = Request;
			if (!ToSendRequest.Members.Contains(Self))
			{
				ToSendRequest.Members.Add(Self);
			}

			if (!ToSendRequest.Admins.Contains(Self))
			{
				ToSendRequest.Admins.Add(Self);
			}

			TSharedRef<FJsonObject> Params = MakeShared<FJsonObject>();
			FJsonObjectConverter::UStructToJsonObject(FAccelByteModelsChatCreateTopicRequest::StaticStruct(), &ToSendRequest, Params, 0, 0);
			Params.Get().SetStringField(ChatToken::Json::Field::Namespace, CredentialsRef.GetNamespace());
			Params.Get().SetStringField(ChatToken::Json::Field::Type, ChatToken::Json::Value::ChatGroupTypePublic);
			SEND_CONTENT_CACHE_ID(CreateTopic)
		}

		void Chat::UpdateTopic(const FAccelByteModelsChatUpdateTopicRequest& Request
			, const FChatActionTopicResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			SEND_REQUEST(UpdateTopic)
		}

		void Chat::DeleteTopic(const FString& ToDeleteTopicId
			, const FChatActionTopicResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
			FJsonDomBuilder::FObject Params;
			Params.Set(ChatToken::Json::Field::TopicId, ToDeleteTopicId);
			SEND_CONTENT_CACHE_ID(DeleteTopic)
		}

		void Chat::AddUserToTopic(const FAccelByteModelsChatAddUserToTopicRequest& Request
			, const FAddRemoveUserFromTopicResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			SEND_REQUEST(AddUserToTopic)
		}

		void Chat::RemoveUserFromTopic(const FAccelByteModelsChatRemoveUserFromTopicRequest& Request
			, const FAddRemoveUserFromTopicResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			SEND_REQUEST(RemoveUserFromTopic)
		}

		void Chat::JoinTopic(const FString& TopicId
			, const FChatActionTopicResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
			FJsonDomBuilder::FObject Params;
			Params.Set(ChatToken::Json::Field::TopicId, TopicId);
			SEND_CONTENT_CACHE_ID(JoinTopic)
		}

		void Chat::QuitTopic(const FString& TopicId
			, const FChatActionTopicResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
			FJsonDomBuilder::FObject Params;
			Params.Set(ChatToken::Json::Field::TopicId, TopicId);
			SEND_CONTENT_CACHE_ID(QuitTopic)
		}

		void Chat::SendChat(const FString& TopicId
			, const FString& Message
			, const FSendChatResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			if (Message.IsEmpty())
			{
				UE_LOG(LogAccelByteChat, Warning, TEXT("Cant send an empty chat"))
				return;
			}

			FJsonDomBuilder::FObject Params;
			Params.Set(ChatToken::Json::Field::TopicId, TopicId);
			Params.Set(ChatToken::Json::Field::ChatMessage, Message);

			SEND_CONTENT_CACHE_ID(SendChat)
		}

		void Chat::DeleteSystemMessages(TSet<FString> MessageIds, const FDeleteSystemMessagesResponse& OnSuccess,
			const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			if (MessageIds.Num() <= 0)
			{
				UE_LOG(LogAccelByteChat, Warning, TEXT("Can't delete inbox message(s), MessageIds empty"));
				return;
			}

			FJsonDomBuilder::FArray JsonMessageIds;
			for (const FString& MessageId : MessageIds)
			{
				JsonMessageIds.Add(MessageId);
			}

			FJsonDomBuilder::FObject Params;
			Params.Set(ChatToken::Json::Field::SystemMessageIds, JsonMessageIds);

			SEND_CONTENT_CACHE_ID(DeleteSystemMessages)
		}

		void Chat::UpdateSystemMessages(TArray<FAccelByteModelsActionUpdateSystemMessage> ActionUpdateSystemMessages,
			const FUpdateSystemMessagesResponse& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			if (ActionUpdateSystemMessages.Num() <= 0)
			{
				UE_LOG(LogAccelByteChat, Warning, TEXT("Can't delete inbox message(s), MessageIds empty"));
				return;
			}

			FJsonDomBuilder::FArray JsonActionUpdateSystemMessages;
			for (const FAccelByteModelsActionUpdateSystemMessage& ActionUpdateSystemMessage : ActionUpdateSystemMessages)
			{
				FJsonDomBuilder::FObject Item;
				Item.Set(ChatToken::Json::Field::MessageId, ActionUpdateSystemMessage.ID);
				Item.Set(ChatToken::Json::Field::SystemMessageRead
					, ActionUpdateSystemMessage.GetReadAsString());
				Item.Set(ChatToken::Json::Field::SystemMessageKeep
					, ActionUpdateSystemMessage.GetKeepAsString());
				
				JsonActionUpdateSystemMessages.Add(Item);
			}

			FJsonDomBuilder::FObject Params;
			Params.Set(ChatToken::Json::Field::Data, JsonActionUpdateSystemMessages);

			SEND_CONTENT_CACHE_ID(UpdateSystemMessages)
		}

		void Chat::QuerySystemMessage(const FQuerySystemMessageResponse& OnSuccess, const FErrorHandler& OnError,
			const FQuerySystemMessageOptions OptionalParams)
		{
			FReport::Log(FString(__FUNCTION__));

			FJsonDomBuilder::FObject Params;

			if(OptionalParams.Limit == 0)
			{
				Params.Set(ChatToken::Json::Field::Limit, DefaultQuerySystemMessageLimit);
			}
			if(OptionalParams.Offset < 0)
			{
				Params.Set(ChatToken::Json::Field::Offset, DefaultQuerySystemMessageOffset);
			}
			if(OptionalParams.UnreadOnly)
			{
				Params.Set(ChatToken::Json::Field::UnreadOnly, OptionalParams.UnreadOnly);
			}
			if(!OptionalParams.Category.IsEmpty())
			{
				Params.Set(ChatToken::Json::Field::Category, OptionalParams.Category);
			}
			if(OptionalParams.StartCreatedAt != FDateTime{0})
			{
				Params.Set(ChatToken::Json::Field::StartCreatedAt, OptionalParams.StartCreatedAt.ToUnixTimestamp());
			}
			if(OptionalParams.EndCreatedAt != FDateTime{0})
			{
				Params.Set(ChatToken::Json::Field::EndCreatedAt, OptionalParams.EndCreatedAt.ToUnixTimestamp());
			}

			SEND_CONTENT_CACHE_ID(QuerySystemMessage);
		}

		void Chat::GetSystemMessageStats(const FGetSystemMessageStatsResponse& OnSuccess, const FErrorHandler& OnError,
			const FAccelByteGetSystemMessageStatsRequest Request)
		{
			FReport::Log(FString(__FUNCTION__));

			FJsonDomBuilder::FObject Params;

			SEND_CONTENT_CACHE_ID(GetSystemMessageStats);
		}

		void Chat::QueryTopic(const FAccelByteModelsChatQueryTopicRequest& Request
			, const FQueryTopicResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
			TSharedRef<FJsonObject> Params = MakeShared<FJsonObject>();
			FJsonObjectConverter::UStructToJsonObject(FAccelByteModelsChatQueryTopicRequest::StaticStruct(), &Request, Params, 0, 0);
			Params.Get().SetStringField(ChatToken::Json::Field::Namespace, CredentialsRef.GetNamespace());
			SEND_CONTENT_CACHE_ID(QueryTopic)
		}

		void Chat::QueryTopicById(const FString& TopicId
			, const FQueryTopicByIdResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
			FJsonDomBuilder::FObject Params;
			Params.Set(ChatToken::Json::Field::TopicId, TopicId);

			SEND_CONTENT_CACHE_ID(QueryTopicById)
		}

		void Chat::QueryPersonalTopic(int Offset
			, int Limit
			, const FQueryTopicResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
			FJsonDomBuilder::FObject Params;
			Params.Set(ChatToken::Json::Field::Namespace, CredentialsRef.GetNamespace());
			Params.Set(ChatToken::Json::Field::Offset, Offset);
			Params.Set(ChatToken::Json::Field::Limit, Limit);
			SEND_CONTENT_CACHE_ID(QueryPersonalTopic)
		}

		void Chat::QueryGroupTopic(const FAccelByteModelsChatQueryTopicRequest& Request
			, const FQueryTopicResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
			TSharedRef<FJsonObject> Params = MakeShared<FJsonObject>();
			FJsonObjectConverter::UStructToJsonObject(FAccelByteModelsChatQueryTopicRequest::StaticStruct(), &Request, Params, 0, 0);
			Params.Get().SetStringField(ChatToken::Json::Field::Namespace, CredentialsRef.GetNamespace());
			SEND_CONTENT_CACHE_ID(QueryGroupTopic)
		}

		void Chat::QueryPublicTopic(const FAccelByteModelsChatQueryTopicRequest& Request
			, const FQueryPublicTopicResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
			TSharedRef<FJsonObject> Params = MakeShared<FJsonObject>();
			FJsonObjectConverter::UStructToJsonObject(FAccelByteModelsChatQueryTopicRequest::StaticStruct(), &Request, Params, 0, 0);
			Params.Get().SetStringField(ChatToken::Json::Field::Namespace, CredentialsRef.GetNamespace());
			SEND_CONTENT_CACHE_ID(QueryPublicTopic)
		}

		void Chat::QueryChat(const FAccelByteModelsChatQueryChatRequest& Request
			, const FQueryChatResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			SEND_REQUEST(QueryChat)
		}

		void Chat::ReadChat(const TSet<FString>& ChatIds
			, const FReadChatResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
			FJsonDomBuilder::FObject Params;

			FJsonDomBuilder::FArray JsonChatIds;
			for (const FString& aChatId : ChatIds)
			{
				JsonChatIds.Add(aChatId);
			}
			Params.Set(ChatToken::Json::Field::ChatIds, JsonChatIds);
			SEND_CONTENT_CACHE_ID(ReadChat)
		}

		void Chat::BlockUser(const FString& UserId
			, const FChatBlockUserResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
				, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
				, OnError))
			{
				return;
			}

			FJsonDomBuilder::FObject Params;
			Params.Set(ChatToken::Json::Field::UserId, UserId);
			SEND_CONTENT_CACHE_ID(BlockUser);
		}

		void Chat::UnblockUser(const FString& UserId
			, const FChatUnblockUserResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
				, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
				, OnError))
			{
				return;
			}

			FJsonDomBuilder::FObject Params;
			Params.Set(ChatToken::Json::Field::UserId, UserId);
			SEND_CONTENT_CACHE_ID(UnblockUser);
		}

		FString Chat::RefreshToken(const FString& AccessToken
			, const FChatRefreshTokenResponse& OnSuccess
			, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
			FJsonDomBuilder::FObject Params;
			Params.Set(ChatToken::Json::Field::Token, AccessToken);
			SEND_CONTENT_CACHE_ID(RefreshToken);

			return MessageId;
		}

#undef REQUEST_MODEL
#undef SEND_REQUEST_EXPLICIT_MODEL
#undef SEND_REQUEST
#undef SEND_CONTENT_CACHE_ID
#undef MESSAGE_SUCCESS_HANDLER
#undef ID_RESPONSE_MAP

		FString Chat::SendWebSocketContent(const FString& Method
			, const TSharedRef<FJsonObject>& Params)
		{
			if (WebSocket.IsValid() && WebSocket->IsConnected())
			{
				FReport::Log(FString(__FUNCTION__));
				TSharedRef<FJsonObject> JsonObj = MakeShared<FJsonObject>();
				const FString MessageId = GenerateMessageID(Method);
				JsonObj.Get().SetStringField(ChatToken::Json::Field::JsonRPC, ChatToken::Json::Value::JsonRPC);
				JsonObj.Get().SetStringField(ChatToken::Json::Field::MessageId, MessageId);
				JsonObj.Get().SetStringField(ChatToken::Json::Field::Method, Method);
				JsonObj.Get().SetObjectField(ChatToken::Json::Field::Params, Params);

				FString OutJsonString;
				{
					TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>> > JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutJsonString, 0);
					bool bSuccess = FJsonSerializer::Serialize(JsonObj, JsonWriter);
					JsonWriter->Close();
					ensure(bSuccess);
				}

				const FString Content = OutJsonString;
				WebSocket->Send(Content);
				UE_LOG(LogAccelByteChat, Log, TEXT("Sending request:\n %s"), *Content);

				return MessageId;
			}
			return TEXT("");
		}

#pragma endregion // REQUEST

#pragma region GROUP CHAT AS MODERATOR

		void Chat::DeleteGroupChat(const FString& GroupId, const FString& ChatId, const FVoidHandler& OnSuccess,
			const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			const FString Url = FString::Printf(TEXT("%s/public/namespaces/{namespace}/topic/%s/chats/%s"),
				*SettingsRef.ChatServerUrl, *GenerateGroupTopicId(GroupId), *ChatId);

			HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
		}

		void Chat::MuteGroupUserChat(const FString& GroupId, const FString& UserId, int32 DurationInSeconds, const FVoidHandler& OnSuccess,
			const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
				, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
				, OnError))
			{
				return;
			}

			const FString Url = FString::Printf(TEXT("%s/public/namespaces/{namespace}/topic/%s/mute"),
				*SettingsRef.ChatServerUrl, *GenerateGroupTopicId(GroupId));

			const FAccelByteModelsMuteGroupChatRequest Request {UserId, DurationInSeconds};

			HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Request, OnSuccess, OnError);
		}

		void Chat::UnmuteGroupUserChat(const FString& GroupId, const FString& UserId, const FVoidHandler& OnSuccess,
			const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
				, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
				, OnError))
			{
				return;
			}

			const FString Url = FString::Printf(TEXT("%s/public/namespaces/{namespace}/topic/%s/unmute"),
				*SettingsRef.ChatServerUrl, *GenerateGroupTopicId(GroupId));

			const FAccelByteModelsUnmuteGroupChatRequest Request {UserId};

			HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Request, OnSuccess, OnError);
		}

		void Chat::GetGroupChatSnapshot(const FString& GroupId, const FString& ChatId,
			const THandler<FAccelByteModelsChatSnapshotResponse>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/{namespace}/topic/%s/snapshot/%s"),
				*SettingsRef.ChatServerUrl, *GenerateGroupTopicId(GroupId), *ChatId);

			HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
		}

		void Chat::BanGroupUserChat(const FString& GroupId, const TArray<FString>& UserIds,
			const THandler<FAccelByteModelsBanGroupChatResponse>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			const FString Url = FString::Printf(TEXT("%s/public/namespaces/{namespace}/topic/%s/ban-members"),
				*SettingsRef.ChatServerUrl, *GenerateGroupTopicId(GroupId));

			const FAccelByteModelsBanGroupChatRequest Request {UserIds};

			HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
		}

		void Chat::UnbanGroupUserChat(const FString& GroupId, const TArray<FString>& UserIds,
			const THandler<FAccelByteModelsUnbanGroupChatResponse>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			const FString Url = FString::Printf(TEXT("%s/public/namespaces/{namespace}/topic/%s/unban-members"),
				*SettingsRef.ChatServerUrl, *GenerateGroupTopicId(GroupId));

			const FAccelByteModelsUnbanGroupChatRequest Request {UserIds};

			HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
		}

		FString Chat::GenerateGroupTopicId(const FString& GroupId)
		{
			return FString::Printf(TEXT("g.%s"), *GroupId);
		}

#pragma endregion

	} // Namespace Api

} // Namespace AccelByte