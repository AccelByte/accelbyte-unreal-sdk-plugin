// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"

#include "AccelByteDSMModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRegisterLocalServerRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | RegisterLocalRequest")
	FString Ip{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | RegisterLocalRequest")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | RegisterLocalRequest")
	int32 Port{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | RegisterLocalRequest")
	FString Custom_attribute{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRegisterServerRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | RegisterRequest")
	FString Game_version{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | RegisterRequest")
	FString Ip{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | RegisterRequest")
	FString Pod_name{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | RegisterRequest")
	int32 Port{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | RegisterRequest")
	FString Provider{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | RegisterRequest")
	FString Custom_attribute{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsShutdownServerRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ShutdownRequest")
	bool Kill_me{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ShutdownRequest")
	FString Pod_name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ShutdownRequest")
	FString Session_id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDeregisterLocalServerRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | DeregisterRequest")
	FString Name{};
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUser
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | User")
	FString User_id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | User")
	FJsonObjectWrapper ExtraAttributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchingParty
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | MatchingParty")
	FString Party_id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | MatchingParty")
	TArray<FAccelByteModelsUser> Party_members{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | MatchingParty")
	FJsonObjectWrapper Party_attributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchingAlly
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | MatchingAlly")
	TArray<FAccelByteModelsMatchingParty> Matching_parties{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | MatchRequest")
	FString Session_id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | MatchRequest")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | MatchRequest")
	FString Game_mode{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | MatchRequest")
	TArray<FAccelByteModelsMatchingAlly> Matching_allies{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSMClient
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | Client")
	FString Host_address{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | Client")
	FString Region{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | Client")
	FString Provider{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | Client")
	FString Status{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsServerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	FString Pod_name{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	FString Image_version{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	FString Ip{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	TArray<FString> Alternate_ips{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	int32 Port{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	TMap<FString, int32> Ports{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	FString Provider{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	FString Game_version{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	FString Status{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	FString Last_update{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	FString Custom_attribute{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ServerInfo")
	FString Deployment{};	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsServerSessionResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | DSM | Models | SessionResponse")
	FString Session_id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsServerCreateSessionRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | SessionRequest")
	FString Client_version {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | SessionRequest")
	FString Configuration {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | SessionRequest")
	FString Deployment {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | SessionRequest")
	FString Game_mode {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | SessionRequest")
	FString Namespace {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | SessionRequest")
	FString Pod_name {}; // AKA ServerName
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | SessionRequest")
	FString Region {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | SessionRequest")
	FString Session_id {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | SessionRequest")
	TArray<FAccelByteModelsMatchingAlly> Matching_allies{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsServerStatusHistory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | StatusHistory")
	FString Status{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | StatusHistory")
	FString Time_stamp{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSMServer
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	FString Custom_attribute{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	FString Deployment{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	FString Game_version{};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	FString Image_version{};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	FString Ip{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	FString Params{};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	FString Pod_name{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	int Port {0};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	TMap<FString, int> Ports {};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	FString Protocol {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	FString Provider {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	FString Region {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	FString Session_id {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMServer")
	TArray<FAccelByteModelsServerStatusHistory> Status_history {};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSMSession
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMSession")
	FAccelByteModelsDSMServer Server {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMSession")
	FString Id {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMSession")
	FString Namespace {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMSession")
	FString Provider {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DSMSession")
	FString Region {};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsServerCreateSessionResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | SessionResponse")
	FAccelByteModelsDSMSession Session {};
};