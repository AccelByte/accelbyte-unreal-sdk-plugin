#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "AccelByteBPLobby.generated.h"

using namespace AccelByte;

USTRUCT(BlueprintType)
struct FPartyInviteRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString friendID;
};

USTRUCT(BlueprintType)
struct FPartyJoinRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString partyID;

	UPROPERTY(BlueprintReadWrite)
	FString invitationToken;
};

USTRUCT(BlueprintType)
struct FPartyRejectRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString partyID;

	UPROPERTY(BlueprintReadWrite)
	FString invitationToken;
};

USTRUCT(BlueprintType)
struct FPartyKickRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString memberID;
};

USTRUCT(BlueprintType)
struct FStartMatchmakingRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString gameMode;
};

USTRUCT(BlueprintType)
struct FCancelMatchmakingRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString gameMode;

	UPROPERTY(BlueprintReadWrite)
	bool isTempParty{false};
};

USTRUCT(BlueprintType)
struct FSetReadyConsentRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString matchId;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyCreateResponse, FAccelByteModelsCreatePartyResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyDataUpdateNotif, FAccelByteModelsPartyDataNotif, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDInfoPartyResponse, FAccelByteModelsInfoPartyResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDLeavePartyResponse, FAccelByteModelsLeavePartyResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyInviteResponse, FAccelByteModelsPartyInviteResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyJoinResponse, FAccelByteModelsPartyJoinReponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyRejectResponse, FAccelByteModelsPartyRejectResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyKickResponse, FAccelByteModelsKickPartyMemberResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyGetInvitedNotif, FAccelByteModelsPartyGetInvitedNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyJoinNotif, FAccelByteModelsPartyJoinNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyRejectNotif, FAccelByteModelsPartyRejectNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyLeaveNotif, FAccelByteModelsLeavePartyNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyKickNotif, FAccelByteModelsGotKickedFromPartyNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyInviteNotif, FAccelByteModelsInvitationNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDStartMatchmakingResponse, FAccelByteModelsMatchmakingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDCancelMatchmakingResponse, FAccelByteModelsMatchmakingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDSetReadyConsentResponse, FAccelByteModelsReadyConsentRequest, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDMatchmakingNotif, FAccelByteModelsMatchmakingNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDSetReadyConsentNotif, FAccelByteModelsReadyConsentNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDRematchmakingNotif, FAccelByteModelsRematchmakingNotice, Notif);


UCLASS(Blueprintable, BlueprintType)
class ULobby : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable)
	bool IsConnected() const;

	UFUNCTION(BlueprintCallable)
	void Connect() const;

	UFUNCTION(BlueprintCallable)
	void Disconnect() const;

	UFUNCTION(BlueprintCallable)
	void UnbindEvent() const;

	UFUNCTION(BlueprintCallable)
	void SetOnConnected(FDHandler OnConnected) const;

	UFUNCTION(BlueprintCallable)
	void CreateParty(FDPartyCreateResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void PartyInfo(FDInfoPartyResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void PartyLeave(FDLeavePartyResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void PartyInvite(FPartyInviteRequest Request, FDPartyInviteResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void PartyJoin(FPartyJoinRequest const& Request, FDPartyJoinResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void PartyReject(FPartyRejectRequest Request, FDPartyRejectResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void PartyKick(FPartyKickRequest const& Request, FDPartyKickResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyDataUpdate(FDPartyDataUpdateNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyGetInvited(FDPartyGetInvitedNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyJoin(FDPartyJoinNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyReject(FDPartyRejectNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyLeave(FDPartyLeaveNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyKick(FDPartyKickNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyInvite(FDPartyInviteNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void StartMatchmaking(
		FStartMatchmakingRequest const& Request,
		FDStartMatchmakingResponse OnResponse,
		FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void CancelMatchmaking(
		FCancelMatchmakingRequest const& Request,
		FDCancelMatchmakingResponse OnResponse,
		FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void SetReadyConsent(
		FSetReadyConsentRequest const& Request,
		FDSetReadyConsentResponse OnResponse,
		FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void SetOnMatchmaking(FDMatchmakingNotif OnNotif);

	UFUNCTION(BlueprintCallable)
	void SetOnSetReadyConsent(FDSetReadyConsentNotif OnNotif);

	UFUNCTION(BlueprintCallable)
	void SetOnRematchmaking(FDRematchmakingNotif OnNotif);
private:
	bool bConnected{false};
	FApiClientPtr ApiClientPtr;
};
