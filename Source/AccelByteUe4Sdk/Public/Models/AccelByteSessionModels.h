// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "AccelByteSessionModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionUser
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
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionConfig
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
struct ACCELBYTEUE4SDK_API FAccelByteModelsBaseSession
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FString ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		TArray<FAccelByteModelsSessionUser> Members;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FString LeaderID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FJsonObjectWrapper Attributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FString JoinType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FDateTime CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FDateTime UpdatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FAccelByteModelsSessionConfig Config;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyInviteRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyInviteRequest")
		FString UserID;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
    		TArray<FAccelByteModelsSessionUser> Members;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		FJsonObjectWrapper Attributes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		FString JoinType;
};
