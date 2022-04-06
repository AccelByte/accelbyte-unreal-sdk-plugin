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
struct FPartyJoinViaCodeRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString partyCode;
};

USTRUCT(BlueprintType)
struct FPartyPromoteLeaderRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString userId;
};

USTRUCT(BlueprintType)
struct FGetPartyDataRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString partyId;
};

USTRUCT(BlueprintType)
struct FGetPartyStorageRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString partyId;
};

USTRUCT(BlueprintType)
struct FWritePartyStorageRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString partyId;
	
	UPROPERTY(BlueprintReadWrite)
	int32 RetryAttempt;

	TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> PayloadModifier;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDInfoPartyResponse, FAccelByteModelsInfoPartyResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyCreateResponse, FAccelByteModelsCreatePartyResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDLeavePartyResponse, FAccelByteModelsLeavePartyResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyInviteResponse, FAccelByteModelsPartyInviteResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyJoinResponse, FAccelByteModelsPartyJoinReponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyRejectResponse, FAccelByteModelsPartyRejectResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyKickResponse, FAccelByteModelsKickPartyMemberResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyGenerateCodeResponse, FAccelByteModelsPartyGenerateCodeResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyGetCodeResponse, FAccelByteModelsPartyGetCodeResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyDeleteCodeResponse, FAccelByteModelsPartyDeleteCodeResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyJoinViaCodeResponse, FAccelByteModelsPartyJoinReponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyPromoteLeaderResponse, FAccelByteModelsPartyPromoteLeaderResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyGetDataResponse, FAccelByteModelsPartyData, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyGetStorageResponse, FAccelByteModelsPartyDataNotif, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyWriteDataResponse, FAccelByteModelsPartyDataNotif, Response);

#pragma endregion 

#pragma region MODEL_AND_DELEGATE_FOR_NOTIFICATION

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyGetInvitedNotif, FAccelByteModelsPartyGetInvitedNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyJoinNotif, FAccelByteModelsPartyJoinNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyRejectNotif, FAccelByteModelsPartyRejectNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyLeaveNotif, FAccelByteModelsLeavePartyNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyKickNotif, FAccelByteModelsGotKickedFromPartyNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyInviteNotif, FAccelByteModelsInvitationNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyDataUpdateNotif, FAccelByteModelsPartyDataNotif, Notif);

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
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void PartyInfo(FDInfoPartyResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void CreateParty(FDPartyCreateResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void PartyLeave(FDLeavePartyResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void PartyInvite(FPartyInviteRequest Request, FDPartyInviteResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void PartyJoin(FPartyJoinRequest const& Request, FDPartyJoinResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void PartyReject(FPartyRejectRequest Request, FDPartyRejectResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void PartyKick(FPartyKickRequest const& Request, FDPartyKickResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void PartyGenerateCode(FDPartyGenerateCodeResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void PartyGetCode(FDPartyGetCodeResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void PartyDeleteCode(FDPartyDeleteCodeResponse OnResponse, FDErrorHandler OnError) const;
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void PartyJoinViaCode(FPartyJoinViaCodeRequest const& Request, FDPartyJoinViaCodeResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void PartyPromoteLeader(FPartyPromoteLeaderRequest const& Request, FDPartyPromoteLeaderResponse OnResponse, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void GetPartyData(FGetPartyDataRequest const& Request, FDPartyGetDataResponse OnResponse, FDErrorHandler OnError) const;
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void GetPartyStorage(FGetPartyStorageRequest const& Request, FDPartyGetStorageResponse OnResponse, FDErrorHandler OnError) const;
	
	//Can not be run at the moment. Need to find a way to expose TFunction Json Modifier to lua
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void WritePartyStorage(FWritePartyStorageRequest const& Request, FDPartyWriteDataResponse OnResponse, FDErrorHandler OnError) const;

//Notification
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void SetOnPartyDataUpdate(FDPartyDataUpdateNotif OnNotif) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void SetOnPartyGetInvited(FDPartyGetInvitedNotif OnNotif) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void SetOnPartyJoin(FDPartyJoinNotif OnNotif) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void SetOnPartyReject(FDPartyRejectNotif OnNotif) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void SetOnPartyLeave(FDPartyLeaveNotif OnNotif) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void SetOnPartyKick(FDPartyKickNotif OnNotif) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void SetOnPartyInvite(FDPartyInviteNotif OnNotif) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Party")
	void SetOnPartyDataUpdateNotifDelegate(FDPartyDataUpdateNotif OnNotif) const;

private:
	FApiClientPtr ApiClientPtr;
};
