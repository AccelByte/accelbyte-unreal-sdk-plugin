#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "AccelByteBPLobby.generated.h"

using namespace AccelByte;

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyCreateResponse, FAccelByteModelsCreatePartyResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyDataUpdateNotif, FAccelByteModelsPartyDataNotif, Notif);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDInfoPartyResponse, FAccelByteModelsInfoPartyResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDLeavePartyResponse, FAccelByteModelsLeavePartyResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDInviteToPartyResponse, FAccelByteModelsPartyInviteResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyJoinResponse, FAccelByteModelsPartyJoinReponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyRejectResponse, FAccelByteModelsPartyRejectResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyKickResponse, FAccelByteModelsKickPartyMemberResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyGetInvitedNotif, FAccelByteModelsPartyGetInvitedNotice, Notif);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyJoinNotif, FAccelByteModelsPartyJoinNotice, Notif);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyRejectNotif, FAccelByteModelsPartyRejectNotice, Notif);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyLeaveNotif, FAccelByteModelsLeavePartyNotice, Notif);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyKickNotif, FAccelByteModelsGotKickedFromPartyNotice, Notif);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyInviteNotif, FAccelByteModelsInvitationNotice, Notif);

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
	void SetConnectSuccessDelegate(FDHandler OnConnectSuccess) const;

	UFUNCTION(BlueprintCallable)
	void SetCreatePartyResponseDelegate(FDPartyCreateResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void SendCreatePartyRequest() const;

	UFUNCTION(BlueprintCallable)
	void SetPartyDataUpdateNotifDelegate(FDPartyDataUpdateNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetInfoPartyResponseDelegate(FDInfoPartyResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendInfoPartyRequest() const;

	UFUNCTION(BlueprintCallable)
	void SetLeavePartyResponseDelegate(FDLeavePartyResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendLeavePartyRequest() const;

	UFUNCTION(BlueprintCallable)
	void SetInviteToPartyResponseDelegate(FDInviteToPartyResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendInviteToPartyRequest(FString const& UserId);

	UFUNCTION(BlueprintCallable)
	void SetInvitePartyJoinResponseDelegate(FDPartyJoinResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendAcceptInvitationRequest(FString const& PartyId, FString const& InvitationToken) const;

	UFUNCTION(BlueprintCallable)
	void SetRejectInvitationResponseDelegate(FDPartyRejectResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendRejectInvitationRequest(FString const& PartyId, FString const& InvitationToken) const;

	UFUNCTION(BlueprintCallable)
	void SetKickPartyMemberResponseDelegate(FDPartyKickResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendKickPartyMemberRequest(FString const& UserId) const;

	UFUNCTION(BlueprintCallable)
	void SetPartyGetInvitedNotifDelegate(FDPartyGetInvitedNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetPartyJoinNotifDelegate(FDPartyJoinNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetPartyInvitationRejectedNotifDelegate(FDPartyRejectNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetPartyLeaveNotifDelegate(FDPartyLeaveNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetPartyKickNotifDelegate(FDPartyKickNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetPartyInviteNotifDelegate(FDPartyInviteNotif OnNotif) const;
private:
	bool bConnected{false};
	FApiClientPtr ApiClientPtr;
};