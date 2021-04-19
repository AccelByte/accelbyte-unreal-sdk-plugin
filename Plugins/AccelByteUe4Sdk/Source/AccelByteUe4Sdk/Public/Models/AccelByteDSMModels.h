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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | RegisterLocalRequest")
	FString Ip;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | RegisterLocalRequest")
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | RegisterLocalRequest")
	int32 Port;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRegisterServerRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | RegisterRequest")
	FString Game_version;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | RegisterRequest")
	FString Ip;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | RegisterRequest")
	FString Pod_name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | RegisterRequest")
	int32 Port;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | RegisterRequest")
	FString Provider;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsShutdownServerRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ShutdownRequest")
	bool Kill_me;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ShutdownRequest")
	FString Pod_name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ShutdownRequest")
	FString Session_id;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDeregisterLocalServerRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | DeregisterRequest")
	FString Name;
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUser
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | User")
	FString User_id;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchingParty
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchingParty")
	FString Party_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchingParty")
	TArray<FAccelByteModelsUser> Party_members;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchingParty")
	FJsonObjectWrapper Party_attributes;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchingAlly
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchingAlly")
	TArray<FAccelByteModelsMatchingParty> Matching_parties;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchRequest")
	FString Session_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchRequest")
	FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchRequest")
	FString Game_mode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchRequest")
	TArray<FAccelByteModelsMatchingAlly> Matching_allies;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSMClient
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | Client")
		FString Host_address;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | Client")
		FString Region;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | Client")
		FString Provider;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | Client")
		FString Status;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPubIp
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | Ip")
		FString Ip;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsServerInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ServerInfo")
		FString Pod_name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ServerInfo")
		FString Image_version;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ServerInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ServerInfo")
		FString Ip;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ServerInfo")
		TArray<FString> Alternate_ips;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ServerInfo")
		int32 Port;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ServerInfo")
		FString Provider;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ServerInfo")
		FString Game_version;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ServerInfo")
		FString Status;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | ServerInfo")
		FString Last_update;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsServerSessionResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | SessionResponse")
		FString Session_id;
};