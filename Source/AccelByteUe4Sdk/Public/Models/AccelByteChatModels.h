// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Misc/DateTime.h"
#include "JsonUtilities.h"
#include "Models/AccelByteUserModels.h"
#include "Core/AccelByteUtilities.h"
#include "AccelByteChatModels.generated.h"

/**
 * @brief Chat Topic types
 */
UENUM(BlueprintType)
enum class ETopicType : uint8
{
	Unknown = 0	UMETA(DisplayName = "unknown"),
	PERSONAL	UMETA(DisplayName = "Personal"), //< member of 2
	GROUP		UMETA(DisplayName = "Group"), //< member > 2
};

//!@brief Notification structure of a chat websocket connected event  
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatConnectNotif
{
	GENERATED_BODY()
	
	//! Chat websocket connection session ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ConnectNotif")
	FString SessionID {};
};

//@brief Notification struct of a websocket chat connection disconnected event
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatDisconnectNotif
{
	GENERATED_BODY()

	//! Chat websocket disconnect message
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | DisconnectNotif")
	FString Message {};
};

//!@brief Response structure for chat refresh token
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatRefreshTokenResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | RefreshTokenResponse")
	FString Code;
};

#pragma region SEND CHAT

//!@brief Response structure of send chat
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatSendChatResponse
{
	GENERATED_BODY()

	//! Processed time of send chat action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SendChatResponse")
	FDateTime Processed {0};

	//! Topic Id as destination for the sent chat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SendChatResponse")
	FString TopicId {};

	//! Id of sent chat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SendChatResponse")
	FString ChatId {};

	//! The chat message that was sent, will return filtered message
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SendChatResponse")
	FString Message {};
};

//!@brief Chat notification  
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatNotif
{
	GENERATED_BODY()

	//! Id of incoming chat, useful for flagging chat state to read
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatNotif")
	FString ChatId {};

	//! Chat message content
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatNotif")
	FString Message {};

	//! Creation time of sent chat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatNotif")
	FDateTime CreatedAt {0};

	//! Topic id where this chat belongs to
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatNotif")
	FString TopicId {};

	//! Sender's UserId
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatNotif")
	FString From {};

	//! Platform ID of the player that sent this chat message.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatNotif")
	EAccelBytePlatformType SenderPlatformId {};
};

#pragma endregion

#pragma region QUERY TOPIC

//!@brief Request for Query Topic
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatQueryTopicRequest
{
	GENERATED_BODY()

	FAccelByteModelsChatQueryTopicRequest(
		const FString& Keyword = TEXT(""),
		int Offset = 0,
		int Limit = 10) :
		Keyword(Keyword),
		Offset(Offset),
		Limit(Limit)
	{};

	//! Alias for group name, used as a filter. Leave at default value empty to hit all matched result
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryTopicRequest")
	FString Keyword {};

	//! Result offset from most recent
	//! e.g. if user has 100 topics, offset 25 means result will start from 75
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryTopicRequest")
	int Offset{0};

	//! @param Limit - Max count of returned results from the most recent
	//! e.g. if user has 100 topics, limit 20 will only return the last 20 (100 - 81)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryTopicRequest")
	int Limit{10};
};

//!@brief Topic Query result data  
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatTopicQueryData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | TopicQueryData")
	FString TopicId {};

	//! Last updated topic's time
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | TopicQueryData")
	FDateTime UpdatedAt {0};

	//! ETopicType::GROUP -> Predefined TopicId alias
	//! ETopicType::PERSONAL -> Other chat participant UserId
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | TopicQueryData")
	FString Name {};

	//! Chat topic types; either PERSONAL or GROUP; differs in member capacity. PERSONAL==2; GROUP>2
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | TopicQueryData")
	ETopicType Type { ETopicType::Unknown };

	//! User Id of topic members
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | TopicQueryData")
	TArray<FString> Members {};

	//! [DEPRECATED] Number of unread chats by logged in user. Excluding chat sent by the querier
	//! e.g. In PERSONAL topic, between A and B. if A sent 2 chats, then A queries, it will return 0 unreadChats
	//! if B queries, then it will return 2 unread chats
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | TopicQueryData")
	int UnreadChats{0}; // This variable has been deprecated because the backend also flags it as such.
};

//!@brief Response structure of action query topic 
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatQueryTopicResponse
{
	GENERATED_BODY()

	//! Processed time of topic query
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryTopicResponse")
	FDateTime Processed {0};

	//! Matched results
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryTopicResponse")
	TArray<FAccelByteModelsChatTopicQueryData> Data {};
};

//!@brief Response structure of action query topic by Id
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatQueryTopicByIdResponse
{
	GENERATED_BODY()

	//! Processed time of topic query
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryTopicByIdResponse")
	FDateTime Processed {0};

	//! Matched result
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryTopicByIdResponse")
	FAccelByteModelsChatTopicQueryData Data {};
};

//!@brief Public Topic Query result data  
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatPublicTopicQueryData
{
	GENERATED_BODY()

	//! Public topic ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | PublicTopicQueryData")
	FString TopicId {};

	//! Alias for public topic id
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | PublicTopicQueryData")
	FString Name {};

	//! Number of public topic members
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | PublicTopicQueryData")
	int NumberOfMembers{0};
};

//!@brief Response structure of action query public topic
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatQueryPublicTopicResponse
{
	GENERATED_BODY()

	//! Processed time of topic query
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryPublicTopicResponse")
	FDateTime Processed {0};

	//! Matched results
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryPublicTopicResponse")
	TArray<FAccelByteModelsChatPublicTopicQueryData> Data {};
};

#pragma endregion

#pragma region QUERY CHAT 

//!@brief Request for Query Chat
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatQueryChatRequest
{
	GENERATED_BODY()

	FAccelByteModelsChatQueryChatRequest(
		const FString& TopicId = TEXT(""),
		int Limit = 10) :
		TopicId(TopicId),
		Limit(Limit)
	{};

	//! Target topic from which the chats are being queried
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryChatRequest")
	FString TopicId {};

	//! Max count of returned results from the most recent
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryChatRequest")
	int Limit{10};

};

//!@brief Chat Query result data  
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatQueryChatData
{
	GENERATED_BODY()

	//! Unique identifier for a chat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryChatData")
	FString ChatId {};

	//! Topic id where this chat belongs to
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryChatData")
	FString TopicId {};

	//! Chat message content
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryChatData")
	FString Message {};

	//! Creation time of sent chat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryChatData")
	FDateTime CreatedAt{ 0 };

	//! Sender's UserId
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryChatData")
	FString From {};

	//! Read time of sent chat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryChatData")
	FDateTime ReadAt{ 0 };

};

//!@brief Response structure of action query chat 
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatQueryChatResponse
{
	GENERATED_BODY()

	//! Processed time of chat query
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryChatResponse")
	FDateTime Processed {0};

	//! Chat query results
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QueryChatResponse")
	TArray<FAccelByteModelsChatQueryChatData> Data {};
};

#pragma endregion

#pragma region READ CHAT 

//!@brief Response structure of action read chat 
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatReadChatData
{
	GENERATED_BODY()

	//! Topic id where the read chat belongs to
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ReadChatData")
	FString TopicId {};

	//! Ids of read chats
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ReadChatData")
	TSet<FString> ChatIds {};

	FORCEINLINE bool operator==(const FAccelByteModelsChatReadChatData& Other) const
	{
		bool bEqual = TopicId == Other.TopicId;
		if (bEqual && ChatIds.Num() == Other.ChatIds.Num())
		{
			for (const FString& aChatId : ChatIds)
			{
				if (!Other.ChatIds.Contains(aChatId))
				{
					bEqual = false;
					break;
				}
			}
		}

		return bEqual;
	}

};

//!@brief Response structure of action read chat 
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatReadChatResponse
{
	GENERATED_BODY()

	//! Processed time of read chat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ReadChatResponse")
	FDateTime Processed {0};

	//! Data of read chats
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ReadChatResponse")
	TArray<FAccelByteModelsChatReadChatData> Result {};
};

//!@brief Read Chat notification 
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsReadChatNotif
{
	GENERATED_BODY()

	//! Data of read chats
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ReadChatNotif")
	TArray<FAccelByteModelsChatReadChatData> ReadChat {};
};

#pragma endregion

#pragma region CHAT TOPIC

//@brief Create topic request's parameter in struct form
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatCreateTopicRequest
{
	GENERATED_BODY()

	FAccelByteModelsChatCreateTopicRequest(
		const FString& Name = TEXT(""),
		const TSet<FString>& Members = {},
		const TSet<FString>& Admins = {},
		bool IsJoinable = false) :
		Name(Name),
		Members(Members),
		Admins(Admins),
		IsJoinable(IsJoinable)
	{};

	//! Friendly name of the chat topic (alias), usable as keyword in QueryTopic
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | CreateTopicRequest")
	FString Name {};

	//! UserIds to be added to the chat topic
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | CreateTopicRequest")
	TSet<FString> Members {};

	//! UserIds to be added to the chat topic as admin
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | CreateTopicRequest")
	TSet<FString> Admins {};

	//! Whether new user can join this group topic without invitation from admin
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | CreateTopicRequest")
	bool IsJoinable {false};

};

//@brief Update topic request's parameter in struct form
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatUpdateTopicRequest
{
	GENERATED_BODY()

	FAccelByteModelsChatUpdateTopicRequest(
		const FString& TopicId = TEXT(""),
		const FString& Name = TEXT(""),
		bool IsJoinable = false) :
		TopicId(TopicId),
		Name(Name),
		IsJoinable(IsJoinable)
	{};

	//! Topic Id to be updated
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateTopicRequest")
	FString TopicId {};

	//! New Group Name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateTopicRequest")
	FString Name {};

	//! Whether topic is joinable by other non-topic-member or not
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateTopicRequest")
	bool IsJoinable{ false };

};

//@brief Add user to topic request's parameter in struct form
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatAddUserToTopicRequest
{
	GENERATED_BODY()

	FAccelByteModelsChatAddUserToTopicRequest(
		const FString& TopicId = TEXT(""),
		const FString& UserId = TEXT("")) :
		TopicId(TopicId),
		UserId(UserId)
	{};

	//! Topic id that will receive the new user
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | AddUserToTopicRequest")
	FString TopicId {};

	//! User Id that will be added to the topic
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | AddUserToTopicRequest")
	FString UserId {};

};

//@brief Remove user from topic request's parameter in struct form
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatRemoveUserFromTopicRequest
{
	GENERATED_BODY()

	FAccelByteModelsChatRemoveUserFromTopicRequest(
		const FString& TopicId = TEXT(""),
		const FString& UserId = TEXT("")) :
		TopicId(TopicId),
		UserId(UserId)
	{};

	//! Topic id that will remove an existing user
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | RemoveUserFromTopicRequest")
	FString TopicId {};

	//! User Id that will be removed from the topic
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | RemoveUserFromTopicRequest")
	FString UserId {};

};

//!@brief Response structure of action chat topic (create, update, delete, join ,quit) 
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatActionTopicResponse
{
	GENERATED_BODY()

	//! Processed time of action chat topic
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionTopicResponse")
	FDateTime Processed {0};

	//! Topic Id result of requested chat topic creation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionTopicResponse")
	FString TopicId {};

};

//!@brief Response structure of action chat topic (create, update, delete, join ,quit) 
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatActionCreateTopicServerResponse
{
	GENERATED_BODY()

	//! Processed time of action chat topic
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionCreateTopicServerResponse")
	FString Namespace {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionCreateTopicServerResponse")
	FString TopicId {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionCreateTopicServerResponse")
	FString Name {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionCreateTopicServerResponse")
	ETopicType Type {ETopicType::Unknown};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionCreateTopicServerResponse")
	bool IsJoinable {false};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionCreateTopicServerResponse")
	bool IsChannel {false};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionCreateTopicServerResponse")
	FDateTime CreatedAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionCreateTopicServerResponse")
	FString CreatedBy {};
};

//!@brief Response structure of action user chat topic (add user, remove user) 
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatActionUserTopicResponse
{
	GENERATED_BODY()

	//! Processed time of create chat topic
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionUserTopicResponse")
	FDateTime Processed {0};

	//! Topic Id result of requested chat topic creation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionUserTopicResponse")
	FString TopicId {};

	//! Relevant UserId (added/removed user)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionUserTopicResponse")
	FString UserId {};
};

//!@brief Notification structure when the user is added / removed from a topic
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatUpdateUserTopicNotif
{
	GENERATED_BODY()
	
	//! Chat topic types; either PERSONAL or GROUP; differs in member capacity. PERSONAL==2; GROUP>2
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateUserTopicNotif")
	ETopicType Type {ETopicType::Unknown};

	//! Name of chat topic, only relevant for type GROUP
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateUserTopicNotif")
	FString Name {};

	//! Topic id 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateUserTopicNotif")
	FString TopicId {};

	//! User id of action initiator (add to topic / remove from topic) 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateUserTopicNotif")
	FString SenderId {};
	
	//! Affected user id that added to topic or removed from topic
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateUserTopicNotif")
	FString UserId {};

	//! Is this topic a channel 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateUserTopicNotif")
	bool IsChannel {false};

	FORCEINLINE bool operator==(const FAccelByteModelsChatUpdateUserTopicNotif& Other) const
	{
		return Type == Other.Type && Name == Other.Name && TopicId == Other.TopicId && SenderId == Other.SenderId && IsChannel == Other.IsChannel && UserId == Other.UserId;
	}
};

//!@brief Update chat topic notification structure (topic update or deletion)
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatUpdateTopicNotif
{
	GENERATED_BODY()

	//! Predefined topic Id alias if GROUP or sender's name if PERSONAL
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateTopicNotif")
	FString Name {};

	//! Updated chat topic id 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateTopicNotif")
	FString TopicId {};

	//! Sender's UserId who triggers the chat topic update
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateTopicNotif")
	FString SenderId {};

	//! Is this topic a channel 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateUserTopicNotif")
	bool IsChannel {false};
};

#pragma endregion

//!@brief Ban state notif structure (for both banned and unbanned)
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatUserBanUnbanNotif
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | BanStateNotif")
	FString UserId{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | BanStateNotif")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | BanStateNotif")
	EBanType Ban {EBanType::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | BanStateNotif")
	FString EndDate {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | BanStateNotif")
	EBanReason Reason {EBanReason::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | BanStateNotif")
	bool Enable {false};
};

//!@brief Response structure of action Block User 
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatBlockUserResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | BlockUserResponse")
	FDateTime Processed {0};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | BlockUserResponse")
	FString UserId{};
};

//!@brief Response structure of action Unblock User 
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatUnblockUserResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | BlockUserResponse")
	FDateTime Processed {0};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | BlockUserResponse")
	FString UserId{};
};

#pragma region SYSTEM INBOX

//!@brief Response structure of delete system inbox messages
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDeleteSystemMessagesResponse
{
	GENERATED_BODY()

	//! Processed time of send chat action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | DeleteSystemMessagesResponse")
	FDateTime Processed {0};
};

//!@brief Response structure of set read status system inbox messages
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateSystemMessagesResponse
{
	GENERATED_BODY()

	//! Processed time of send chat action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UpdateSystemMessagesResponse")
	FDateTime Processed {0};
};

//!@brief Enum representing optional boolean for update system inbox
UENUM(BlueprintType)
enum class EAccelByteOptionalBool : uint8
{
	NONE = 0	UMETA(DisplayName = "NONE"),
	// OPT_ prefix added for these enum members to avoid compile error in mac
	OPT_YES		UMETA(DisplayName = "YES"),
	OPT_NO		UMETA(DisplayName = "NO")
};

//!@brief Model representing an action update system message
USTRUCT(BlueprintType)
struct FAccelByteModelsActionUpdateSystemMessage
{
	GENERATED_BODY()

	//! Id of system message
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionUpdateSystemMessage")
	FString ID {};

	//! Status of system message (read/unread)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionUpdateSystemMessage")
	EAccelByteOptionalBool Read { EAccelByteOptionalBool::NONE };

	//! Mark message as keep to store it in user system inbox
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ActionUpdateSystemMessage")
	EAccelByteOptionalBool Keep { EAccelByteOptionalBool::NONE };

	FString GetReadAsString() const
	{
		return GetStringFromOptionalBool(Read);
	}

	FString GetKeepAsString() const
	{
		return GetStringFromOptionalBool(Keep);
	}

private:
	/** Get string representation of optional bool based on backend convention. */
	static FString GetStringFromOptionalBool(const EAccelByteOptionalBool Value)
	{
		return FAccelByteUtilities::GetUEnumValueAsString(Value).Replace(TEXT("OPT_"), TEXT(""));
	}
};

//!@brief Notification structure for System Message
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FSystemMessageNotifMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SystemMessageNotif")
	FString  MessageId {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SystemMessageNotif")
	FDateTime  CreatedAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SystemMessageNotif")
	FDateTime  ExpiredAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QuerySystemMessagesResponseItem")
	FDateTime UpdatedAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QuerySystemMessagesResponseItem")
	FDateTime ReadAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SystemMessageNotif | Data")
	FString Title{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SystemMessageNotif | Data")
	FString Body{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SystemMessageNotif | Data")
	FJsonObjectWrapper Gift{};
	
};

//!@brief Notification structure for System Message
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatSystemMessageNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SystemMessageNotif")
	FString  MessageId {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SystemMessageNotif")
	FString  Category {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SystemMessageNotif")
	FString  Message {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SystemMessageNotif")
	FDateTime  CreatedAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SystemMessageNotif")
	FDateTime  ExpiredAt {0};

	/**
	 * @brief Parse Message to FAccelByteModelsChatSystemMessageNotifData UStruct
	 * @param OutFormattedMessage Message in FAccelByteModelsChatSystemMessageNotifData UStruct
	 * 
	 * @result returns true when parsing success
	 */
	bool GetSystemMessageData(FSystemMessageNotifMessage& OutFormattedMessage) const
	{
		const bool bMessageDeserialized = FJsonObjectConverter::JsonObjectStringToUStruct(Message, &OutFormattedMessage, 0, 0);

		OutFormattedMessage.MessageId = MessageId;
		OutFormattedMessage.CreatedAt = CreatedAt;
		OutFormattedMessage.ExpiredAt = ExpiredAt;

		return bMessageDeserialized;
	}

	/**
	 * @brief Check if this notification is a transient system message
	 *
	 * @result returns true if this notification is a transient system message
	 */
	bool IsTransientSystemMessage() const
	{
		// Transient system message indicated by the presence of category field. If it empty it means this is regular system message.
		if (Category.IsEmpty())
		{
			return false;
		}

		return true;
	}
};

//!@brief Response structure of set read status system inbox messages
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsQuerySystemMessagesResponseItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QuerySystemMessagesResponseItem")
	FString Id {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QuerySystemMessagesResponseItem")
	FString Category {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QuerySystemMessagesResponseItem")
	FString Message {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QuerySystemMessagesResponseItem")
	FDateTime CreatedAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QuerySystemMessagesResponseItem")
	FDateTime UpdatedAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QuerySystemMessagesResponseItem")
	FDateTime ExpiredAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QuerySystemMessagesResponseItem")
	FDateTime ReadAt {0};

	/**
	 * @brief Parse Message to FAccelByteModelsChatSystemMessageNotifData UStruct
	 * @param OutFormattedMessage Message in FAccelByteModelsChatSystemMessageNotifData UStruct
	 * 
	 * @result returns true when parsing success
	 */
	bool GetSystemMessageData(FSystemMessageNotifMessage& OutFormattedMessage) const
	{
		const bool bMessageDeserialized = FJsonObjectConverter::JsonObjectStringToUStruct(Message, &OutFormattedMessage, 0, 0);

		OutFormattedMessage.MessageId = Id;
		OutFormattedMessage.CreatedAt = CreatedAt;
		OutFormattedMessage.ExpiredAt = ExpiredAt;
		OutFormattedMessage.ReadAt = ReadAt;
		OutFormattedMessage.UpdatedAt = UpdatedAt;

		return bMessageDeserialized;
	}

	/**
	 * @brief Check if this notification is a transient system message
	 *
	 * @result returns true if this notification is a transient system message
	 */
	bool IsTransientSystemMessage() const
	{
		// Transient system message indicated by the presence of category field. If it empty it means this is regular system message.
		if (Category.IsEmpty())
		{
			return false;
		}

		return true;
	}
};

//!@brief Response structure of set read status system inbox messages
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsQuerySystemMessagesResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | QuerySystemMessagesResponse")
	TArray<FAccelByteModelsQuerySystemMessagesResponseItem> Data {};
};

//!@brief Response structure of set read status system inbox messages
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FQuerySystemMessageOptions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | FQuerySystemMessageOptions")
	bool UnreadOnly {false};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | FQuerySystemMessageOptions")
	FDateTime StartCreatedAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | FQuerySystemMessageOptions")
	FDateTime EndCreatedAt {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | FQuerySystemMessageOptions")
	FString Category {};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | FQuerySystemMessageOptions")
	int32 Offset {0};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | FQuerySystemMessageOptions")
	int32 Limit {20};
};

/** Request structure of get system message stats request. */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteGetSystemMessageStatsRequest
{
	GENERATED_BODY()
	// Currently the request is empty, the struct is used as placeholder for future when the request can have parameters
};

/** Response structure of get system message response. */
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteGetSystemMessageStatsResponse
{
	GENERATED_BODY()

	/** Date and time of oldest unread message. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | FGetSystemMessageResponse")
	FDateTime OldestUnread {0};

	/** Total of unread messages in user system inbox. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | FGetSystemMessageResponse")
	int32 Unread {0};
};

#pragma endregion 

#pragma region GROUP CHAT AS MODERATOR

//!@brief Request for mute user in group chat
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMuteGroupChatRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	int32 Duration {0};
};

//!@brief Request for unmute user in group chat
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUnmuteGroupChatRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	FString UserId{};
};

//!@brief Message from chat snapshot
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSnapshotMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	FString ChatId{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	FDateTime CreatedAt {0};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	FString Message{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	FString SenderId{};
};

//!@brief Chat snapshot response structure
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatSnapshotResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	FString ChatId{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	FDateTime CreatedAt {0};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	TArray<FString> JoinedTopics{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	TArray<FAccelByteModelsSnapshotMessage> Messages{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	FString SenderId{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	FString TicketId{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	FString TopicId{};
};

//!@brief Group chat ban request structure
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBanGroupChatRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	TArray<FString> UserIDs{};
};

//!@brief Group chat ban response structure
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBanGroupChatResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	TArray<FString> UserIDs{};
};

//!@brief Group chat unban request structure
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUnbanGroupChatRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	TArray<FString> UserIDs{};
};

//!@brief Group chat unban response structure
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUnbanGroupChatResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GroupChatModerator")
	TArray<FString> UserIDs{};
};

#pragma endregion

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatMutedNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatMutedNotif")
	FString TopicId{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatMutedNotif")
	FDateTime ExpiredAt {0};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatMutedNotif")
	int32 RemainingTime{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatUnmutedNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatUnmutedNotif")
	FString TopicId{};
};

#pragma region CHAT CONFIGURATION

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatPublicConfigResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatConfig")
	int32 ChatRateLimitBurst{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatConfig")
	int64 ChatRateLimitDuration{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatConfig")
	int32 GeneralRateLimitBurst{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatConfig")
	int64 GeneralRateLimitDuration{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatConfig")
	int32 MaxChatMessageLength{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatConfig")
	int32 SpamChatBurst{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatConfig")
	int64 SpamChatDuration{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | ChatConfig")
	int64 SpamMuteDuration{};
};

#pragma endregion

#pragma region USER CHAT CONFIGURATION

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteUserChatConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | UserChatConfig")
	bool ProfanityDisabled{false};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetUserChatConfigurationResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GetUserChatConfigResponse")
	FDateTime Processed {0};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | GetUserChatConfigResponse")
	FAccelByteUserChatConfiguration Config{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSetUserChatConfigurationRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SetUserChatConfigResponse")
	FAccelByteUserChatConfiguration Config{};
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteSetUserChatConfigurationResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Chat | Models | SetUserChatConfigResponse")
	FDateTime Processed {0};
};

#pragma endregion
