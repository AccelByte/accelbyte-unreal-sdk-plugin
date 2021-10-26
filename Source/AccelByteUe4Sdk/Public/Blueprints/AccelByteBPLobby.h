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
struct FMatchmakingStartRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString gameMode;
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
DECLARE_DYNAMIC_DELEGATE_OneParam(FDMatchmakingStartResponse, FAccelByteModelsMatchmakingResponse, Response);

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
	void SetOnCreatePartyResponse(FDPartyCreateResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendCreatePartyRequest() const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyDataUpdateNotif(FDPartyDataUpdateNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyInfoResponse(FDInfoPartyResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendPartyInfoRequest() const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyLeaveResponse(FDLeavePartyResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendPartyLeaveRequest() const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyInviteResponse(FDPartyInviteResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendPartyInviteRequest(FPartyInviteRequest Request);

	UFUNCTION(BlueprintCallable)
	void SetOnPartyJoinResponse(FDPartyJoinResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendPartyJoinRequest(FPartyJoinRequest Request) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyRejectResponse(FDPartyRejectResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendPartyRejectRequest(FPartyRejectRequest Request) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyKickResponse(FDPartyKickResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendPartyKickRequest(FPartyKickRequest Request) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyGetInvitedNotif(FDPartyGetInvitedNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyJoinNotif(FDPartyJoinNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyRejectNotif(FDPartyRejectNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyLeaveNotif(FDPartyLeaveNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyKickNotif(FDPartyKickNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetOnPartyInviteNotif(FDPartyInviteNotif OnNotif) const;

	UFUNCTION(BlueprintCallable)
	void SetOnMatchmakingStartResponse(FDMatchmakingStartResponse OnResponse) const;

	UFUNCTION(BlueprintCallable)
	void SendMatchmakingStartRequest(FMatchmakingStartRequest Request) const;
	
private:
	bool bConnected{false};
	FApiClientPtr ApiClientPtr;
};