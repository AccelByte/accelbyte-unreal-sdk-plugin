// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Core/AccelByteUtilities.h"
#include "Models/AccelByteGameTelemetryModels.h"
#include "Models/AccelByteSessionModels.h"
#include "Models/AccelByteMatchmakingModels.h"
#include "AccelBytePredefinedEventModels.generated.h"

/** @brief Predefined Event Name list enum. */
UENUM(BlueprintType)
enum class EAccelBytePredefinedEventName : uint8
{
	SDK_Initialized,
	Login_Succeeded,
	Login_Failed,
	UserAgreement_Accepted,
	UserAgreement_NotAccepted,
	UserProfile_Created,
	UserProfile_Updated,
	UserProfile_Deleted,
	GameProfile_Created,
	GameProfile_Updated,
	GameProfile_Deleted,
	UserStatItem_Created,
	UserStatItem_Updated,
	UserStatItem_Deleted,
	UserStatItem_Reset,
	UserStatItem_GetItemsByCodes,
	UserStatItem_GetSameItemsFromUsers,
	UserStatItem_GetItems,
	GlobalStatItem_GetItemByCode,
	StatCycleItem_GetItemList,
	StatCycleItem_GetConfigByCycleId,
	StatCycleItem_GetListCycle,
	StatCycleItem_GetCycleItems,
	PlayerRecord_Created,
	PlayerRecord_Updated,
	PlayerRecord_Deleted,
	PlayerRecord_GetRecords,
	PublicPlayerRecord_GetSameRecordsFromUsers,
	PublicPlayerRecord_GetRecord,
	PublicPlayerRecord_Updated,
	PublicPlayerRecord_GetOtherUserKeys,
	PublicPlayerRecord_GetOtherUserRecords,
	GameRecord_Created,
	GameRecord_GetRecord,
	GameRecord_Updated,
	GameRecord_Deleted,
	GameRecord_GetRecords,
	PlayerBinaryRecord_Created,
	PlayerBinaryRecord_GetRecord,
	PlayerBinaryRecord_GetRecords,
	PlayerBinaryRecord_GetSameRecordsFromUsers,
	PlayerBinaryRecord_GetRecordsByQuery,
	PlayerBinaryRecord_GetAll,
	PlayerBinaryRecord_Updated,
	PlayerBinaryRecord_Deleted,
	GameBinaryRecord_GetRecord,
	GameBinaryRecord_GetRecords,
	GameBinaryRecord_GetRecordsByQuery,
	Store_Opened,
	Store_Closed,
	ItemInspect_Opened,
	ItemInspect_Closed,
	Currency_Updated,
	Entitlement_Granted,
	Entitlement_Revoked,
	CampaignCode_Redeemed,
	Item_Fulfilled,
	Item_Rewarded,
	PaymentConfig_Updated,
	Payment_Succeeded,
	Payment_Failed,
	Subscription_Started,
	Subscription_Cancelled,
	Wallet_Credited,
	Wallet_Debited,
	Lobby_Connected,
	Lobby_Disconnected,
	MPV1_Matchmaking_Started,
	MPV1_Matchmaking_MatchNotif_Received,
	MPV1_Matchmaking_ReadyConsent,
	MPV1_Matchmaking_RejectMatch,
	MPV1_Matchmaking_Canceled,
	MPV1_Party_Joined,
	MPV1_Party_Leave,
	MPV1_Party_Invite,
	MPV1_Party_Kick,
	MPV2_GameSession_Created,
	MPV2_GameSession_Invited,
	MPV2_GameSession_Joined,
	MPV2_GameSession_Left,
	MPV2_GameSession_LeaderPromoted,
	MPV2_PartySession_Created,
	MPV2_PartySession_Invited,
	MPV2_PartySession_Joined,
	MPV2_PartySession_Left,
	MPV2_PartySession_Kicked,
	MPV2_PartySession_LeaderPromoted,
	MPV2_Matchmaking_Requested,
	MPV2_Matchmaking_Started,
	MPV2_Matchmaking_Canceled,
	DS_DSHub_Connected,
	DS_DSHub_Disconnected,
	DS_Registered,
	DS_Unregistered,
	DS_Claimed,
	DS_MemberChangedNotif_Received,
	DS_GameClient_Joined,
	DS_GameClient_Left,
	DS_BackfillProposal_Received,
	DS_BackfillProposal_Accepted,
	DS_BackfillProposal_Rejected,
	DS_BackfillTicket_Expired,
	FriendRequest_Sent,
	FriendRequest_Cancelled,
	FriendRequest_Accepted,
	FriendRequest_Rejected,
	Friend_Unfriended,
	User_Blocked,
	User_Unblocked,
	UserPresence_StatusUpdated,
	User_Banned,
	User_Unbanned,
	ChatV2_Connected,
	ChatV2_Disconnected,
	ChatV2_PersonalTopic_Created,
	ChatV2_GroupTopic_Created,
	ChatV2_Topic_Joined,
	ChatV2_Topic_Quit,
	ChatV2_Topic_UserAdded,
	ChatV2_Topic_UserRemoved,
	ChatV2_Topic_Updated,
	ChatV2_Topic_Deleted,
	ChatV2_UserBlocked,
	ChatV2_UserUnblocked,
	ChatV2_GroupChat_ModeratorMutedUser,
	ChatV2_GroupChat_ModeratorUnmutedUser,
	ChatV2_GroupChat_ModeratorBannedUser,
	ChatV2_GroupChat_ModeratorUnbannedUser,
	ChatV2_GroupChat_ModeratorDeletedGroupChat,
	Chat_GetConfig,
	Chat_GetUserConfig,
	Chat_SetUserConfig,
	Group_Created,
	Group_Updated,
	Group_CustomAttributesUpdated,
	Group_CustomRuleUpdated,
	Group_PredefinedRuleUpdated,
	Group_PredefinedRuleDeleted,
	Group_GetInformation,
	Group_Find,
	Group_FindByIds,
	Group_Joined,
	Group_Deleted,
	Group_Left,
	Group_UserInvited,
	Group_InvitationAccepted,
	Group_InvitationRejected,
	Group_InvitationCanceled,
	Group_GetInvitationList,
	Group_JoinRequestAccepted,
	Group_JoinRequestRejected,
	Group_JoinRequestCanceled,
	Group_GetJoinRequest,
	Group_MemberKicked,
	Group_MemberRoleUpdated,
	Group_MemberRoleDeleted,
	Group_GetGroupMember,
	Achievement_Unlocked,
	Achievement_GetAll,
	Achievement_GetSpecific,
	Achievement_GetUserAchievements,
	GlobalAchievement_Get,
	GlobalAchievement_GetContributors,
	GlobalAchievement_GetContributed,
	GlobalAchievement_Claimed,
	Achievement_GetTags,
	Leaderboard_GetRankings,
	Leaderboard_GetUserRanking,
	Leaderboard_GetLeaderboards,
	Leaderboard_GetRankingByCycleId,
	Leaderboard_GetUsersRankings,
	SeasonPass_RewardClaimed,
	SeasonPass_BulkRewardClaimed,
	SeasonPass_GetCurrentSeason,
	SeasonPass_GetUserSpecificSeasonData,
	SeasonPass_GetUserCurrentSeasonData,
	Reward_GetRewardByCode,
	Reward_GetRewardById,
	Reward_GetAllReward,
	UGCChannel_Created,
	UGCChannel_Updated,
	UGCContent_Added,
	UGCContent_Deleted
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString PreDefinedEventName{};

	virtual const FString GetPreDefinedEventName()
	{
		return TEXT("PreDefinedEvent");
	};

	FAccelByteModelsPredefinedEvent() = default;
	virtual ~FAccelByteModelsPredefinedEvent() = default;
};

struct ACCELBYTEUE4SDK_API FAccelByteModelsCachedPredefinedEventPayload : public FAccelByteModelsPredefinedEvent
{
	FAccelByteModelsTelemetryBody Payload{};

	virtual const FString GetPreDefinedEventName() override
	{
		return PreDefinedEventName;
	};
};

#pragma region CoreGame

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPluginInfo
{
	GENERATED_BODY()

	UPROPERTY()
		FString Name{};

	UPROPERTY()
		FString Version{};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsSDKInitializedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		TArray<FAccelByteModelsPluginInfo> Plugins{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::SDK_Initialized);
	};
};
#pragma endregion CoreGame

#pragma region Access
USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsLoginFailedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString Namespace{};
	UPROPERTY()
		FString PlatformId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Login_Failed);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsLoginSuccededPayload : public FAccelByteModelsLoginFailedPayload
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString PlatformUserId{};
	UPROPERTY()
		FString DeviceId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Login_Succeeded);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsAgreementDocument
{
	GENERATED_BODY()

	UPROPERTY()
		FString LocalizedPolicyVersionId{};
	UPROPERTY()
		FString PolicyVersionId{};
	UPROPERTY()
		FString PolicyId{};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserAgreementAcceptedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		TArray<FAccelByteModelsAgreementDocument> AgreementDocuments{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserAgreement_Accepted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserAgreementNotAcceptedPayload : public FAccelByteModelsUserAgreementAcceptedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserAgreement_NotAccepted);
	}
};

#pragma endregion Access

#pragma region Storage
USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserProfileCreatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FJsonObjectWrapper UpdatedFields{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserProfile_Created);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserProfileUpdatedPayload : public FAccelByteModelsUserProfileCreatedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserProfile_Updated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatItemCreatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};

	UPROPERTY()
		TArray<FString> StatCodes{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserStatItem_Created);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatItemUpdatedPayload : public FAccelByteModelsUserStatItemCreatedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserStatItem_Updated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatItemDeletedPayload : public FAccelByteModelsUserStatItemCreatedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserStatItem_Deleted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatItemResetPayload : public FAccelByteModelsUserStatItemCreatedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserStatItem_Reset);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatItemGetItemsByCodesPayload : public FAccelByteModelsUserStatItemCreatedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserStatItem_GetItemsByCodes);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatItemGetSameItemsFromUsersPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()
		
	UPROPERTY()
		FString StatCode{};
	UPROPERTY()
		TArray<FString> UserIds{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserStatItem_GetSameItemsFromUsers);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatItemGetItemsPayload : public FAccelByteModelsUserStatItemCreatedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserStatItem_GetItems);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGlobalStatItemGetItemByCodePayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()
		
	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString StatCode{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GlobalStatItem_GetItemByCode);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsStatCycleItemGetItemListPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()
		
	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString CycleId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::StatCycleItem_GetItemList);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsStatCycleItemGetConfigByCycleIdPayload : public FAccelByteModelsStatCycleItemGetItemListPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::StatCycleItem_GetConfigByCycleId);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsStatCycleItemGetListCyclePayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()
		
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::StatCycleItem_GetListCycle);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsStatCycleItemGetCycleItemsPayload : public FAccelByteModelsStatCycleItemGetItemListPayload
{
	GENERATED_BODY()

	UPROPERTY()
		TArray<FString> StatCodes{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::StatCycleItem_GetCycleItems);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerRecordCreatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString Key{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerRecord_Created);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerRecordDeletedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString Key{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerRecord_Deleted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerRecordGetRecordsPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		TArray<FString> Keys{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerRecord_GetRecords);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicPlayerRecordGetSameRecordsFromUsersPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString Key{};
	UPROPERTY()
		TArray<FString> UserIds{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PublicPlayerRecord_GetSameRecordsFromUsers);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicPlayerRecordGetRecordPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Key{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PublicPlayerRecord_GetRecord);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicPlayerRecordUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Key{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PublicPlayerRecord_Updated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicPlayerRecordGetOtherUserKeysPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PublicPlayerRecord_GetOtherUserKeys);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicPlayerRecordGetOtherUserRecordsPayload : public FAccelByteModelsPlayerRecordGetRecordsPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PublicPlayerRecord_GetOtherUserRecords);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerRecordUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString Key{};
	UPROPERTY()
		bool IsPublic{};
	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString SetBy{};
	UPROPERTY()
		FString Strategy{};
	UPROPERTY()
		FJsonObjectWrapper Value{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerRecord_Updated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameRecordDeletedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString Key{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameRecord_Deleted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameRecordCreatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Key{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameRecord_Created);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameRecordGetRecordByKeyPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Key{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameRecord_GetRecord);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameRecordUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString Key{};
	UPROPERTY()
		FString SetBy{};
	UPROPERTY()
		FString Strategy{};
	UPROPERTY()
		FJsonObjectWrapper Value{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameRecord_Updated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameRecordGetRecordsByKeysPayload : public FAccelByteModelsPlayerRecordGetRecordsPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameRecord_GetRecords);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerBinaryRecordCreatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Key{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerBinaryRecord_Created);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerBinaryRecordGetRecordPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Key{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerBinaryRecord_GetRecord);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerBinaryRecordGetRecordsPayload : public FAccelByteModelsPlayerRecordGetRecordsPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerBinaryRecord_GetRecords);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerBinaryRecordGetSameRecordsFromUsersPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString Key{};
	UPROPERTY()
		TArray<FString> UserIds{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerBinaryRecord_GetSameRecordsFromUsers);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerBinaryRecordGetRecordsByQueryPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Query{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerBinaryRecord_GetRecordsByQuery);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerBinaryRecordGetAllPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerBinaryRecord_GetAll);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerBinaryRecordRecordUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Key{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerBinaryRecord_Updated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerBinaryRecordDeletedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Key{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerBinaryRecord_Deleted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameBinaryRecordGetRecordPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Key{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameBinaryRecord_GetRecord);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameBinaryRecordGetRecordsByKeysPayload : public FAccelByteModelsPlayerRecordGetRecordsPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameBinaryRecord_GetRecords);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameBinaryRecordGetRecordsByQueryPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Query{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameBinaryRecord_GetRecordsByQuery);
	}
};
#pragma endregion Storage

#pragma region Monetization
USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsStoreOpenedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString StoreId{};
	UPROPERTY()
		FString StoreName{};
	UPROPERTY()
		FString Category{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Store_Opened);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsStoreClosedPayload : public FAccelByteModelsStoreOpenedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Store_Closed);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemInspectOpenedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString ItemId{};
	UPROPERTY()
		FString ItemNamespace{};
	UPROPERTY()
		FString StoreId{};
	UPROPERTY()
		FString Language{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ItemInspect_Opened);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemInspectClosedPayload : public FAccelByteModelsItemInspectOpenedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ItemInspect_Closed);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsCurrencyUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString WalletId{};
	UPROPERTY()
		FString CurrencyCode{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Currency_Updated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementGrantData
{
	GENERATED_BODY()

	UPROPERTY()
		FString ItemId{};
	UPROPERTY()
		FString ItemNamespace{};
	UPROPERTY()
		FString StoreId{};
	UPROPERTY()
		FString GrantedCode{};
	UPROPERTY()
		FString Source{};
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementGrantedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		TArray<FAccelByteModelsEntitlementGrantData> Entitlements{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Entitlement_Granted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementSummaryEventPayload
{
	GENERATED_BODY()

	UPROPERTY()
		FString Id{};
	UPROPERTY()
		FString Name{};
	UPROPERTY()
		FString Type{};
	UPROPERTY()
		FString Clazz{};
	UPROPERTY()
		FString ItemId{};
	UPROPERTY()
		FString StoreId{};

	static const FAccelByteModelsEntitlementSummaryEventPayload CreateEntitlementSummaryEventPayload(const FAccelByteModelsEntitlementSummary& EntitlementSummary)
	{
		FAccelByteModelsEntitlementSummaryEventPayload EventPayload{};
		EventPayload.Id = EntitlementSummary.Id;
		EventPayload.Name = EntitlementSummary.Name;
		EventPayload.Type = FAccelByteUtilities::GetUEnumValueAsString(EntitlementSummary.Type);
		EventPayload.Clazz = FAccelByteUtilities::GetUEnumValueAsString(EntitlementSummary.Clazz);
		EventPayload.ItemId = EntitlementSummary.ItemId;
		EventPayload.StoreId = EntitlementSummary.StoreId;
		return EventPayload;
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsCreditSummaryEventPayload
{
	GENERATED_BODY()

	UPROPERTY()
		FString WalletId{};
	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		int32 Amount{};
	UPROPERTY()
		FString CurrencyCode{};

	static const FAccelByteModelsCreditSummaryEventPayload CreateCreditSummaryEventPayload(const FAccelByteModelsCreditSummary& CreditSummary)
	{
		FAccelByteModelsCreditSummaryEventPayload EventPayload{};
		EventPayload.WalletId = CreditSummary.WalletId;
		EventPayload.UserId = CreditSummary.UserId;
		EventPayload.Amount = CreditSummary.Amount;
		EventPayload.CurrencyCode = CreditSummary.CurrencyCode;
		return EventPayload;
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsSubscriptionSummaryEventPayload
{
	GENERATED_BODY()

	UPROPERTY()
		FString Id{};
	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString ItemId{};
	UPROPERTY()
		FString Sku{};
	UPROPERTY()
		FString Status{};
	UPROPERTY()
		FString SubscribedBy{};

	static const FAccelByteModelsSubscriptionSummaryEventPayload CreateSubscriptionSummaryEventPayload(const FAccelByteModelsSubscriptionSummary& SubscriptionSummary)
	{
		FAccelByteModelsSubscriptionSummaryEventPayload EventPayload{};
		EventPayload.Id = SubscriptionSummary.Id;
		EventPayload.UserId = SubscriptionSummary.UserId;
		EventPayload.ItemId = SubscriptionSummary.ItemId;
		EventPayload.Sku = SubscriptionSummary.Sku;
		EventPayload.Status = FAccelByteUtilities::GetUEnumValueAsString(SubscriptionSummary.Status);
		EventPayload.SubscribedBy = FAccelByteUtilities::GetUEnumValueAsString(SubscriptionSummary.SubscribedBy);
		return EventPayload;
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemFulfilledPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		TArray<FAccelByteModelsEntitlementSummaryEventPayload> EntitlementSummaries{};
	UPROPERTY()
		TArray<FAccelByteModelsCreditSummaryEventPayload> CreditSummaries{};
	UPROPERTY()
		TArray<FAccelByteModelsSubscriptionSummaryEventPayload> SubscriptionSummaries{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Item_Fulfilled);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsCampaignCodeRedeemedPayload : public FAccelByteModelsItemFulfilledPayload
{
	GENERATED_BODY()

	UPROPERTY()
		FString Code{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::CampaignCode_Redeemed);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaymentSuccededPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString  UserId{};
	UPROPERTY()
		FString ItemId{};
	UPROPERTY()
		int32 Price{};
	UPROPERTY()
		FString  OrderNo{};
	UPROPERTY()
		FString PaymentOrderNo{};
	UPROPERTY()
		FString Status{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Payment_Succeeded);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaymentFailedPayload : public FAccelByteModelsPaymentSuccededPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Payment_Failed);
	}
};
#pragma endregion Monetization

#pragma region Play

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsLobbyConnectedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Lobby_Connected);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsLobbyDisconnectedPayload : public FAccelByteModelsLobbyConnectedPayload
{
	GENERATED_BODY()

	UPROPERTY()
		int32 StatusCode{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Lobby_Disconnected);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV1MatchmakingStartedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString GameMode{};
	UPROPERTY()
		FString ServerName{};
	UPROPERTY()
		FString ClientVersion{};
	UPROPERTY()
		FString Latencies{};
	UPROPERTY()
		TMap<FString, FString> PartyAttributes{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV1_Matchmaking_Started);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV1MatchmakingReadyConsentPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString MatchId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV1_Matchmaking_ReadyConsent);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV1MatchmakingRejectMatchPayload : public FAccelByteModelsMPV1MatchmakingReadyConsentPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV1_Matchmaking_RejectMatch);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV1MatchmakingCanceledPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString GameMode{};
	UPROPERTY()
		bool IsTempParty{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV1_Matchmaking_Canceled);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV1PartyJoinedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString PartyId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV1_Party_Joined);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV1PartyLeavePayload : public FAccelByteModelsMPV1PartyJoinedPayload
{
	GENERATED_BODY()

		virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV1_Party_Leave);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV1PartyInvitePayload : public FAccelByteModelsMPV1PartyJoinedPayload
{
	GENERATED_BODY()

		virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV1_Party_Invite);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV1PartyKickPayload : public FAccelByteModelsMPV1PartyJoinedPayload
{
	GENERATED_BODY()

		virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV1_Party_Kick);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2GameSessionCreatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString GameSessionId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_GameSession_Created);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2GameSessionInvitedPayload : public FAccelByteModelsMPV2GameSessionCreatedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_GameSession_Invited);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2GameSessionJoinedPayload : public FAccelByteModelsMPV2GameSessionCreatedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_GameSession_Joined);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2GameSessionLeftPayload : public FAccelByteModelsMPV2GameSessionCreatedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_GameSession_Left);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2GameSessionLeaderPromotedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString PromotedUserId{};
	UPROPERTY()
		FString GameSessionId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_GameSession_LeaderPromoted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2PartySessionCreatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString PartySessionId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_PartySession_Created);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2PartySessionInvitedPayload : public FAccelByteModelsMPV2PartySessionCreatedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_PartySession_Invited);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2PartySessionJoinedPayload : public FAccelByteModelsMPV2PartySessionCreatedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_PartySession_Joined);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2PartySessionLeftPayload : public FAccelByteModelsMPV2PartySessionCreatedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_PartySession_Left);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2PartySessionKickedPayload : public FAccelByteModelsMPV2PartySessionCreatedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_PartySession_Kicked);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2PartySessionLeaderPromotedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString PromotedUserId{};
	UPROPERTY()
		FString PartySessionId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_PartySession_LeaderPromoted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2MatchmakingRequestedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString MatchPool{};
	UPROPERTY()
		FString PartySessionId{};
	UPROPERTY()
		FJsonObjectWrapper Attributes{};
	UPROPERTY()
		FString MatchTicketId{};
	UPROPERTY()
		int32 QueueTime{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_Matchmaking_Requested);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2MatchmakingStartedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString MatchTicketId{};
	UPROPERTY()
		FString PartySessionId{};
	UPROPERTY()
		FString MatchPool{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_Matchmaking_Started);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsMPV2MatchmakingCanceledPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString MatchTicketId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::MPV2_Matchmaking_Canceled);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSHubConnectedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString PodName{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_DSHub_Connected);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSHubDisconnectedPayload : public FAccelByteModelsDSHubConnectedPayload
{
	GENERATED_BODY()

	UPROPERTY()
		int32 StatusCode{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_DSHub_Disconnected);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSRegisteredPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString PodName{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_Registered);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSClaimedPayload : public FAccelByteModelsDSRegisteredPayload
{
	GENERATED_BODY()

	UPROPERTY()
		FString GameSessionId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_Claimed);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSMemberChangedNotifReceivedPayload : public FAccelByteModelsDSRegisteredPayload
{
	GENERATED_BODY()

	UPROPERTY()
		FString GameSessionId{};
	UPROPERTY()
		TArray<FAccelByteModelsV2SessionUser> Members {};
	UPROPERTY()
		TArray<FAccelByteModelsV2GameSessionTeam> Teams {};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_MemberChangedNotif_Received);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSGameClientJoinedPayload : public FAccelByteModelsDSRegisteredPayload
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_GameClient_Joined);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSGameClientLeftPayload : public FAccelByteModelsDSGameClientJoinedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_GameClient_Left);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSBackfillProposalReceivedPayload : public FAccelByteModelsDSRegisteredPayload
{
	GENERATED_BODY()

	UPROPERTY()
		FString BackfillTicketId{};
	UPROPERTY()
		FString ProposalId{};
	UPROPERTY()
		FString MatchPool{};
	UPROPERTY()
		FString GameSessionId{};
	UPROPERTY()
		TArray<FAccelByteModelsV2GameSessionTeam> ProposedTeams;
	UPROPERTY()
		TArray<FAccelByteModelsV2MatchmakingTicket> AddedTickets;

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_BackfillProposal_Received);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSBackfillProposalAcceptedPayload : public FAccelByteModelsDSBackfillProposalReceivedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_BackfillProposal_Accepted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSBackfillProposalRejectedPayload : public FAccelByteModelsDSBackfillProposalReceivedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_BackfillProposal_Rejected);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSBackfillTicketExpiredPayload : public FAccelByteModelsDSRegisteredPayload
{
	GENERATED_BODY()

	UPROPERTY()
	FString TicketId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_BackfillTicket_Expired);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSUnregisteredPayload : public FAccelByteModelsDSRegisteredPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_Unregistered);
	}
};
#pragma endregion Play

#pragma region Social

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsFriendRequestSentPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString SenderId{};
	UPROPERTY()
	FString ReceiverId{};
	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::FriendRequest_Sent);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsFriendRequestCancelledPayload : public FAccelByteModelsFriendRequestSentPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::FriendRequest_Cancelled);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsFriendRequestRejectedPayload : public FAccelByteModelsFriendRequestSentPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::FriendRequest_Rejected);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsFriendRequestAcceptedPayload : public FAccelByteModelsFriendRequestSentPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::FriendRequest_Accepted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsFriendUnfriendedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString SenderId{};
	UPROPERTY()
	FString FriendId{};
	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Friend_Unfriended);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserBlockedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString SenderId{};
	UPROPERTY()
	FString ReceiverId{};
	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::User_Blocked);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserUnblockedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString SenderId{};
	UPROPERTY()
	FString ReceiverId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::User_Unblocked);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserPresenceStatusUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString UserId{};
	UPROPERTY()
	FString Status{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserPresence_StatusUpdated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserBannedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		EBanType BanType{};
	UPROPERTY()
		FString EndDate{};
	UPROPERTY()
		EBanReason Reason{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::User_Banned);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserUnbannedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		EBanType BanType{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::User_Unbanned);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2ConnectedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	
	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_Connected);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2DisconnectedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		int32 StatusCode{0};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_Disconnected);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2PersonalTopicCreatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString TargetUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_PersonalTopic_Created);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2GroupTopicCreatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Name{};
	UPROPERTY()
		TSet<FString> Members{};
	UPROPERTY()
		TSet<FString> Admins{};
	UPROPERTY()
		bool IsJoinable{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_GroupTopic_Created);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2TopicJoinedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString TopicId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_Topic_Joined);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2TopicQuitPayload : public FAccelByteModelsChatV2TopicJoinedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_Topic_Quit);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2TopicUserAddedPayload : public FAccelByteModelsChatV2TopicJoinedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_Topic_UserAdded);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2TopicUserRemovedPayload : public FAccelByteModelsChatV2TopicJoinedPayload
{
	GENERATED_BODY()

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_Topic_UserRemoved);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2TopicUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Name{};
	UPROPERTY()
		bool IsChannel{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_Topic_Updated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2TopicDeletedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString TopicId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_Topic_Deleted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2UserBlockedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString BlockedUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_UserBlocked);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2UserUnblockedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString UnblockedUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_UserUnblocked);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2GroupChatModeratorMutedUserPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString ModeratorId{};
	UPROPERTY()
		FString MutedUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_GroupChat_ModeratorMutedUser);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2GroupChatModeratorUnmutedUserPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString ModeratorId{};
	UPROPERTY()
		FString UnmutedUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_GroupChat_ModeratorUnmutedUser);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2GroupChatModeratorBannedUserPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString ModeratorId{};
	UPROPERTY()
		FString BannedUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_GroupChat_ModeratorBannedUser);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2GroupChatModeratorUnbannedUserPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString ModeratorId{};
	UPROPERTY()
		FString UnbannedUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_GroupChat_ModeratorUnbannedUser);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatV2GroupChatModeratorDeleteGroupChatPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString ModeratorId{};
	UPROPERTY()
		FString ChatId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ChatV2_GroupChat_ModeratorDeletedGroupChat);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatGetConfigPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString UserId{};
	
	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Chat_GetConfig);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatGetUserConfigPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString UserId{};
	
	UPROPERTY()
	bool ProfanityDisabled{false};
	
	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Chat_GetUserConfig);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsChatSetUserConfigPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
	FString UserId{};

	UPROPERTY()
	bool ProfanityDisabled{false};
	
	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Chat_SetUserConfig);
	}
};

#pragma endregion Social

#pragma region Engagement
USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupCreatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString ConfigurationCode{};
	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		int32 GroupMaxMember{0};
	UPROPERTY()
		FString GroupName{};
	UPROPERTY()
		FString GroupRegion{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_Created);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString ConfigurationCode{};
	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		int32 GroupMaxMember{0};
	UPROPERTY()
		FString GroupName{};
	UPROPERTY()
		FString GroupRegion{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_Updated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupCustomAttributesUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString AdminUserId{};
	UPROPERTY()
		FJsonObjectWrapper CustomAttributes{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_CustomAttributesUpdated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupCustomRuleUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString AdminUserId{};
	UPROPERTY()
		FJsonObjectWrapper GroupRules{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_CustomRuleUpdated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupPredefinedRuleUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString AdminUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_PredefinedRuleUpdated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupPredefinedRuleDeletedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString AdminUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_PredefinedRuleDeleted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupInformationPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_GetInformation);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupFindPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString GroupName{};
	UPROPERTY()
		FString GroupRegion{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_Find);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupFindByIdsPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		TArray<FString> GroupIds{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_FindByIds);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupJoinedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString Status{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_Joined);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupDeletedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_Deleted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupLeftPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_Left);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupInviteUserPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString AdminUserId{};
	UPROPERTY()
		FString InvitedUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_UserInvited);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupInviteAcceptedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_InvitationAccepted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupInviteRejectedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_InvitationRejected);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupInviteCanceledPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString AdminUserId{};
	UPROPERTY()
		FString CanceledUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_InvitationCanceled);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupGetInvitationListPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_GetInvitationList);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupJoinRequestAcceptedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString AdminUserId{};
	UPROPERTY()
		FString AcceptedUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_JoinRequestAccepted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupJoinRequestRejectedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString AdminUserId{};
	UPROPERTY()
		FString RejectedUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_JoinRequestRejected);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupJoinRequestCanceledPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_JoinRequestCanceled);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupJoinRequestListPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_GetJoinRequest);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupMemberKickedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString AdminUserId{};
	UPROPERTY()
		FString KickedUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_MemberKicked);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupMemberRoleUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString MemberRoleId{};
	UPROPERTY()
		FString UpdatedUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_MemberRoleUpdated);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupMemberRoleDeletedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString MemberRoleId{};
	UPROPERTY()
		FString UpdatedUserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_MemberRoleDeleted);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGroupMemberByGroupIdPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString GroupId{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Group_GetGroupMember);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementUnlockedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString AchievementCode{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Achievement_Unlocked);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementsGetAllPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Achievement_GetAll);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementGetSpecificPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString AchievementCode{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Achievement_GetSpecific);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementGetUsersPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Achievement_GetUserAchievements );
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGlobalAchievementGetPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString AchievementCode{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GlobalAchievement_Get);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGlobalAchievementGetContributorsPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString AchievementCode{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GlobalAchievement_GetContributors);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGlobalAchievementGetContributedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString AchievementCode{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GlobalAchievement_GetContributed);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsGlobalAchievementClaimPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString AchievementCode{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GlobalAchievement_Claimed);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsAchievementGetTagsPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString Name{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Achievement_GetTags);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsLeaderboardGetRankingsPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString LeaderboardCode{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Leaderboard_GetRankings);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsLeaderboardGetUserRankingPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString LeaderboardCode{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Leaderboard_GetUserRanking);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsLeaderboardGetLeaderboardsPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Leaderboard_GetLeaderboards);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsLeaderboardGetRankingByCycleIdPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString LeaderboardCode{};
	UPROPERTY()
		FString CycleId{};
	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Leaderboard_GetRankingByCycleId);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsLeaderboardGetUsersRankingsPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString LeaderboardCode{};
	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		TArray<FString> TargetUserIds{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Leaderboard_GetUsersRankings);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonPassClaimRewardPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString PassCode{};
	UPROPERTY()
		int32 TierIndex{};
	UPROPERTY()
		FString RewardCode{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::SeasonPass_RewardClaimed);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonPassBulkRewardClaimedPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::SeasonPass_BulkRewardClaimed);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonPassGetCurrentSeasonPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString Language{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::SeasonPass_GetCurrentSeason);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonPassGetUserSpecificSeasonDataPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString SeasonId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::SeasonPass_GetUserSpecificSeasonData);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsSeasonPassGetUserCurrentSeasonDataPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::SeasonPass_GetUserCurrentSeasonData);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsRewardGetRewardByCodePayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString RewardCode{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Reward_GetRewardByCode);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsRewardGetRewardByIdPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString RewardId{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Reward_GetRewardById);
	}
};

USTRUCT()
struct ACCELBYTEUE4SDK_API FAccelByteModelsRewardGetAllRewardPayload : public FAccelByteModelsPredefinedEvent
{
	GENERATED_BODY()

	UPROPERTY()
		FString UserId{};
	UPROPERTY()
		FString EventTopic{};

	virtual const FString GetPreDefinedEventName() override
	{
		return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Reward_GetAllReward);
	}
};
#pragma endregion Engagement