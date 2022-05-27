﻿// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "AccelByteSessionModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionUser
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FString ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FString Status;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FDateTime UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FString PlatformID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FString PlatformUserID;
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfig")
		int64 MaxSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfig")
		int32 InitialJoinTimeout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfig")
		int32 RejoinTimeout;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartySession
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FString ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		TArray<FAccelByteModelsV2SessionUser> Members;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FString LeaderID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FJsonObjectWrapper Attributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FString JoinType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FDateTime CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FDateTime UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FAccelByteModelsV2SessionConfig Config;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyInviteRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyInviteRequest")
		FString UserID;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
    		TArray<FAccelByteModelsV2SessionUser> Members;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		FJsonObjectWrapper Attributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		FString JoinType;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyUpdateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateReuqest")
	FJsonObjectWrapper Attributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateReuqest")
	FString JoinType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateReuqest")
	int64 Version;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyUserInvitedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUserInvitedEvent")
		FString SenderID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUserInvitedEvent")
		FString PartyID;
};

