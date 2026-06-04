// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/AccelByteApiClient.h"
#include "ABParty.generated.h"

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

USTRUCT(BlueprintType)
struct FPartyInviteRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Party | Models | Invite")
	FString friendID{};
};

USTRUCT(BlueprintType)
struct FPartyJoinRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Party | Models | Join")
	FString partyID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Party | Models | Join")
	FString invitationToken{};
};

USTRUCT(BlueprintType)
struct FPartyRejectRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Party | Models | Reject")
	FString partyID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Party | Models | Reject")
	FString invitationToken{};
};

USTRUCT(BlueprintType)
struct FPartyKickRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Party | Models | Kick")
	FString memberID{};
};

USTRUCT(BlueprintType)
struct FPartyJoinViaCodeRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Party | Models | Join")
	FString partyCode{};
};

USTRUCT(BlueprintType)
struct FPartyPromoteLeaderRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Party | Models | Promote Leader")
	FString userId{};
};

USTRUCT(BlueprintType)
struct FGetPartyDataRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Party | Models | Party Data")
	FString partyId{};
};

USTRUCT(BlueprintType)
struct FGetPartyStorageRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Party | Models | Party Storage")
	FString partyId{};
};

USTRUCT(BlueprintType)
struct FWritePartyStorageRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Party | Models | Party Storage")
	FString partyId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Party | Models | Party Storage")
	int32 RetryAttempt{};

	TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier{};
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDInfoPartyResponse, FAccelByteModelsInfoPartyResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyCreateResponse, FAccelByteModelsCreatePartyResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDLeavePartyResponse, FAccelByteModelsLeavePartyResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyInviteResponse, FAccelByteModelsPartyInviteResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyJoinResponse, FAccelByteModelsPartyJoinResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyRejectResponse, FAccelByteModelsPartyRejectResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyKickResponse, FAccelByteModelsKickPartyMemberResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyGenerateCodeResponse, FAccelByteModelsPartyGenerateCodeResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyGetCodeResponse, FAccelByteModelsPartyGetCodeResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyDeleteCodeResponse, FAccelByteModelsPartyDeleteCodeResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyJoinViaCodeResponse, FAccelByteModelsPartyJoinResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyPromoteLeaderResponse, FAccelByteModelsPartyPromoteLeaderResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyGetDataResponse, FAccelByteModelsPartyData, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyGetStorageResponse, FAccelByteModelsPartyDataNotif, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyWriteDataResponse, FAccelByteModelsPartyDataNotif, Response);

#pragma endregion 

#pragma region MODEL_AND_DELEGATE_FOR_NOTIFICATION

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyGetInvitedNotif, FAccelByteModelsPartyGetInvitedNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyJoinNotif, FAccelByteModelsPartyJoinNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyRejectNotif, FAccelByteModelsPartyRejectNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyLeaveNotif, FAccelByteModelsLeavePartyNotice, Notif); // This delegate is DEPRECATED

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyMemberLeaveNotif, FAccelByteModelsLeavePartyNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyKickNotif, FAccelByteModelsGotKickedFromPartyNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyUpdateNotif, FAccelByteModelsPartyNotif, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyInviteNotif, FAccelByteModelsInvitationNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyDataUpdateNotif, FAccelByteModelsPartyDataNotif, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyMemberConnectNotif, FAccelByteModelsPartyMemberConnectionNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyMemberDisconnectNotif, FAccelByteModelsPartyMemberConnectionNotice, Notif);

#pragma endregion

/**
 * Blueprint API for AccelByte party-related operations.
 *
 * NOTE: V1 Party methods (create, leave, invite, join, kick, etc.) were removed 
 * as part of Session V1 / MMv1 deprecation. For party management, use the V2 session interface
 * (UABSessionV2 / FOnlineSessionV2AccelByte). The remaining methods in this class cover
 * party storage and data access only.
 */
//Sub-API from AccelByte Lobby
UCLASS(Blueprintable, BlueprintType)
class UABParty : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(AccelByte::FApiClientPtr const& NewApiClientPtr);

//Request-Response
public: 

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void SetPartySizeLimit(const FString& PartyId, const int32 Limit, const FDHandler& OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void GetPartyData(FGetPartyDataRequest const& Request, FDPartyGetDataResponse OnResponse, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void GetPartyStorage(FGetPartyStorageRequest const& Request, FDPartyGetStorageResponse OnResponse, FDErrorHandler OnError);
	
	//Can not be run at the moment. Need to find a way to expose TFunction Json Modifier to lua
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void WritePartyStorage(FWritePartyStorageRequest const& Request, FDPartyWriteDataResponse OnResponse, FDErrorHandler OnError);

private:
	AccelByte::FApiClientPtr ApiClientPtr;
};
