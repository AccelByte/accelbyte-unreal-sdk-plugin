// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Core/AccelByteUtilities.h"
#include "Models/AccelByteGameTelemetryModels.h"
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
    PlayerRecord_Created,
    PlayerRecord_Updated,
    PlayerRecord_Deleted,
    GameRecord_Created,
    GameRecord_Updated,
    GameRecord_Deleted,
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
    GameSession_Created,
    GameSession_Joined,
    GameSession_PlayerAdded,
    GameSession_PlayerRemoved,
    DS_RegisterServer,
    DS_DeRegisterServer,
    DS_ShutdownServer,
    DS_Claimed,
    Lobby_Joined,
    Lobby_Disconnected,
    Matchmaking_V1_Requested,
    Matchmaking_V1_Started,
    Matchmaking_V1_ReadyConsent,
    Matchmaking_V1_RejectMatch,
    Matchmaking_V1_Canceled,
    User_RequestDS,
    User_Banned,
    User_Unbanned,
    FriendRequest_Sent,
    FriendRequest_Cancelled,
    FriendRequest_Accepted,
    FriendRequest_Rejected,
    Friend_Unfriended,
    Party_Joined,
    Party_Leave,
    Party_Invite,
    Party_Kick,
    User_Block,
    User_Unblock,
    Group_Created,
    Group_Updated,
    Group_Joined,
    Group_Deleted,
    Group_Left,
    GroupInvite_Accepted,
    GroupInvite_Rejected,
    GroupInvite_Cancelled,
    GroupJoinRequest_Accepted,
    GroupJoinRequest_Rejected,
    GroupMember_Kicked,
    GroupMember_RoleUpdated,
    UserPresence_StatusUpdated,
    Achievement_Unlocked,
    Leaderboard_Ascended,
    Leaderboard_Descended,
    BattlePassMenu_Opened,
    BattlePassMenu_Closed,
    UGCChannel_Created,
    UGCChannel_Updated,
    UGCContent_Added,
    UGCContent_Deleted
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    virtual const FString GetEventName()
    {
        return TEXT("PredefinedEvent");
    };

    FAccelByteModelsPredefinedEvent() = default;
    virtual ~FAccelByteModelsPredefinedEvent() = default;
};

struct ACCELBYTEUE4SDK_API FAccelByteModelsCachedEventPayload : public FAccelByteModelsPredefinedEvent
{
    FAccelByteModelsTelemetryBody Payload{};

    virtual const FString GetEventName() override
    {
        return Payload.EventName;
    };
};

#pragma region CoreGame

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPluginInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | SDKInitialized")
        FString Name{};
        
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | SDKInitialized")
        FString Version{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSDKInitializedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | SDKInitialized")
        TArray<FAccelByteModelsPluginInfo> Plugins{};
    
    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::SDK_Initialized);
    };
};
#pragma endregion CoreGame

#pragma region Access
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLoginFailedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | LoginInfo")
        FString Namespace{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | LoginInfo")
        FString PlatformId{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Login_Failed);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLoginSuccededPayload : public FAccelByteModelsLoginFailedPayload
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | LoginSuccededInfo")
        FString UserId{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | LoginSuccededInfo")
        FString PlatformUserId{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | LoginSuccededInfo")
        FString DeviceId{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Login_Succeeded);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAgreementDocument
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | AgreementDocument")
        FString LocalizedPolicyVersionId{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | AgreementDocument")
        FString PolicyVersionId{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | AgreementDocument")
        FString PolicyId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserAgreementAcceptedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | AgreementDocumentsInfo")
        TArray<FAccelByteModelsAgreementDocument> AgreementDocuments{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserAgreement_Accepted);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserAgreementNotAcceptedPayload : public FAccelByteModelsUserAgreementAcceptedPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserAgreement_NotAccepted);
    }
};

#pragma endregion Access

#pragma region Storage
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserProfileCreatedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | UserProfileInfo")
        FJsonObjectWrapper UpdatedFields{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserProfile_Created);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserProfileUpdatedPayload : public FAccelByteModelsUserProfileCreatedPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserProfile_Updated);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatItemCreatedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | StatItemInfo")
        TArray<FString> StatCodes{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserStatItem_Created);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatItemUpdatedPayload : public FAccelByteModelsUserStatItemCreatedPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserStatItem_Updated);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatItemDeletedPayload : public FAccelByteModelsUserStatItemCreatedPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserStatItem_Deleted);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerRecordDeletedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | PlayerRecordInfo")
        FString Key{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | PlayerRecordInfo")
        bool IsPublic{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerRecord_Deleted);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerRecordUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | PlayerRecordUpdateInfo")
        FString Key{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | PlayerRecordUpdateInfo")
        bool IsPublic{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | PlayerRecordUpdateInfo")
        FString Strategy{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::PlayerRecord_Updated);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameRecordDeletedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | GameRecordInfo")
        FString Key{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameRecord_Deleted);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameRecordUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | GameRecordUpdateInfo")
        FString Key{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | GameRecordUpdateInfo")
        FString Strategy{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameRecord_Updated);
    }
};
#pragma endregion Storage

#pragma region Monetization
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsStoreOpenedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | StoreInfo")
        FString StoreId{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | StoreInfo")
        FString StoreName{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | StoreInfo")
        FString Category{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Store_Opened);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsStoreClosedPayload : public FAccelByteModelsStoreOpenedPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Store_Closed);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemInspectOpenedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | ItemInfo")
        FString ItemId{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | ItemInfo")
        FString ItemNamespace{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | ItemInfo")
        FString StoreId{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | ItemInfo")
        FString Language{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ItemInspect_Opened);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsItemInspectClosedPayload : public FAccelByteModelsItemInspectOpenedPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::ItemInspect_Closed);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCurrencyUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | CurrencyInfo")
        FString WalletId{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | CurrencyInfo")
        FString CurrencyCode{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Currency_Updated);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementGrantData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | EntitlementGrantData")
        FString ItemId{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | EntitlementGrantData")
        FString ItemNamespace{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | EntitlementGrantData")
        FString StoreId{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | EntitlementGrantData")
        FString GrantedCode{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | EntitlementGrantData")
        FString Source{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsEntitlementGrantedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | EntitlementGrantInfo")
        TArray<FAccelByteModelsEntitlementGrantData> Entitlements{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Entitlement_Granted);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCampaignCodeRedeemedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | CampaignCodeInfo")
        FString Code{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::CampaignCode_Redeemed);
    }
};
#pragma endregion Monetization

#pragma region Play
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameSessionCreatedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | GameSessionInfo")
        FString SessionId{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameSession_Created);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameSessionJoinedPayload : public FAccelByteModelsGameSessionCreatedPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameSession_Joined);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameSessionPlayerAddedPayload : public FAccelByteModelsGameSessionCreatedPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameSession_PlayerAdded);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameSessionPlayerRemovedPayload : public FAccelByteModelsGameSessionCreatedPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::GameSession_PlayerRemoved);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSRegisterServerPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | DSInfo")
        FString PodName{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_RegisterServer);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSDeRegisterServerPayload : public FAccelByteModelsDSRegisterServerPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_DeRegisterServer);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSShutdownServerPayload : public FAccelByteModelsDSRegisterServerPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_ShutdownServer);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSClaimedPayload : public FAccelByteModelsDSRegisterServerPayload
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | DSClaimedInfo")
        FString SessionId{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::DS_Claimed);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchmakingV1StartedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Matchmaking_V1_Started);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchmakingV1CanceledPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | MatchmakingInfo")
        FString GameMode{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Matchmaking_V1_Canceled);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchmakingV1RequestedPayload : public FAccelByteModelsMatchmakingV1CanceledPayload
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | MatchmakingRequestedInfo")
        FJsonObjectWrapper OptionalParams{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Matchmaking_V1_Requested);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchmakingV1ReadyConsentPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | MatchmakingIdInfo")
        FString MatchId{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Matchmaking_V1_ReadyConsent);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchmakingV1RejectMatchPayload : public FAccelByteModelsMatchmakingV1ReadyConsentPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Matchmaking_V1_RejectMatch);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserRequestDSPayload : public FAccelByteModelsMatchmakingV1CanceledPayload
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | MatchmakingDSRequestInfo")
        FString MatchId{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::User_RequestDS);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserBannedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | UserBannedInfo")
        EBanType BanType{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | UserBannedInfo")
        FString EndDate{};
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | UserBannedInfo")
        EBanReason Reason{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::User_Banned);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserUnbannedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | UserBannedInfo")
        EBanType BanType{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::User_Unbanned);
    }
};
#pragma endregion Play

#pragma region Social
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyJoinedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | PartyInfo")
        FString PartyId{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Party_Joined);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyLeavePayload : public FAccelByteModelsPartyJoinedPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Party_Leave);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyInvitePayload : public FAccelByteModelsPartyJoinedPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Party_Invite);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyKickPayload : public FAccelByteModelsPartyJoinedPayload
{
    GENERATED_BODY()

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::Party_Kick);
    }
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserPresenceStatusUpdatedPayload : public FAccelByteModelsPredefinedEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Telemetry | PredefinedEvent | UserPresenceInfo")
        FString Status{};

    virtual const FString GetEventName() override
    {
        return FAccelByteUtilities::GetUEnumValueAsString(EAccelBytePredefinedEventName::UserPresence_StatusUpdated);
    }
};
#pragma endregion Social