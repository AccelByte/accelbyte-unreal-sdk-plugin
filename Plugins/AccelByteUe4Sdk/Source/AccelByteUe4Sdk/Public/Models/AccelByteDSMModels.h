// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteDSMModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRegisterServerRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | RegisterRequest")
		FString Pod_name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | RegisterRequest")
		int32 Port;
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
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchingAlly
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchingAlly")
		TArray<FAccelByteModelsMatchingParty> Matching_parties;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchmakingInfoNotif
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchmakingInfo")
		FString Session_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchmakingInfo")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchmakingInfo")
		FString Game_mode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchmakingInfo")
		TArray<FAccelByteModelsMatchingAlly> Matching_allies;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | MatchmakingInfo")
		FString Pod_name;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDSMMessage
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | Message")
		FString Msg_type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | DSM | Models | Message")
		FAccelByteModelsMatchmakingInfoNotif Message;
};

