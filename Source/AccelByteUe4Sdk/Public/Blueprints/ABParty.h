// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ABParty.generated.h"

using namespace AccelByte;

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

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyInviteNotif, FAccelByteModelsInvitationNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyDataUpdateNotif, FAccelByteModelsPartyDataNotif, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyMemberConnectNotif, FAccelByteModelsPartyMemberConnectionNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyMemberDisconnectNotif, FAccelByteModelsPartyMemberConnectionNotice, Notif);

#pragma endregion

//Sub-API from AccelByte Lobby
UCLASS(Blueprintable, BlueprintType)
class UABParty : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

//Request-Response
public: 
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void PartyInfo(FDInfoPartyResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void CreateParty(FDPartyCreateResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void PartyLeave(FDLeavePartyResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void PartyInvite(FPartyInviteRequest Request, FDPartyInviteResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void PartyJoin(FPartyJoinRequest const& Request, FDPartyJoinResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void PartyReject(FPartyRejectRequest Request, FDPartyRejectResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void PartyKick(FPartyKickRequest const& Request, FDPartyKickResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void PartyGenerateCode(FDPartyGenerateCodeResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void PartyGetCode(FDPartyGetCodeResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void PartyDeleteCode(FDPartyDeleteCodeResponse OnResponse, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void PartyJoinViaCode(FPartyJoinViaCodeRequest const& Request, FDPartyJoinViaCodeResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void PartyPromoteLeader(FPartyPromoteLeaderRequest const& Request, FDPartyPromoteLeaderResponse OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void GetPartyData(FGetPartyDataRequest const& Request, FDPartyGetDataResponse OnResponse, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void GetPartyStorage(FGetPartyStorageRequest const& Request, FDPartyGetStorageResponse OnResponse, FDErrorHandler OnError);
	
	//Can not be run at the moment. Need to find a way to expose TFunction Json Modifier to lua
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Api")
	void WritePartyStorage(FWritePartyStorageRequest const& Request, FDPartyWriteDataResponse OnResponse, FDErrorHandler OnError);

//Notification
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Delegate")
	void SetOnPartyDataUpdate(FDPartyDataUpdateNotif OnNotif);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Delegate")
	void SetOnPartyGetInvited(FDPartyGetInvitedNotif OnNotif);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Delegate")
	void SetOnPartyJoin(FDPartyJoinNotif OnNotif);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Delegate")
	void SetOnPartyReject(FDPartyRejectNotif OnNotif);

	/**
	 * @brief Set a trigger function when a party member leave from the party. This function is DEPRECATED
	 * @param OnNotif delegate parameter with returned model called FAccelByteModelsLeavePartyNotice
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Delegate")
	void SetOnPartyLeave(FDPartyLeaveNotif OnNotif);

	/**
	 * @brief Set a trigger function when a party member leave from the party
	 * @param OnNotif delegate parameter with returned model called FAccelByteModelsLeavePartyNotice
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Delegate")
	void SetOnPartyMemberLeave(FDPartyMemberLeaveNotif OnNotif);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Delegate")
	void SetOnPartyKick(FDPartyKickNotif OnNotif);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Delegate")
	void SetOnPartyInvite(FDPartyInviteNotif OnNotif);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Delegate")
	void SetOnPartyDataUpdateNotifDelegate(FDPartyDataUpdateNotif OnNotif);

	/**
	 * @brief Set a trigger function when a party member reconnecting to the lobby
	 * @param OnNotif return models called FAccelByteModelsPartyMemberConnectionNotice
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Delegate")
	void SetOnPartyMemberConnect(FDPartyMemberConnectNotif OnNotif);

	/**
	 * @brief Set a trigger function when a party member disconnect from lobby
	 * @param OnNotif return models called FAccelByteModelsPartyMemberConnectionNotice
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party | Delegate")
	void SetOnPartyMemberDisconnect(FDPartyMemberDisconnectNotif OnNotif);

private:
	FApiClientPtr ApiClientPtr;
};
