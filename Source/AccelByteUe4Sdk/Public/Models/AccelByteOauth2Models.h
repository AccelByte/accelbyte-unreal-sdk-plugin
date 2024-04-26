// Copyright (c) 2018-2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Misc/DateTime.h"
#include "CoreMinimal.h"
#include "AccelByteGeneralModels.h"
#include "JsonObjectWrapper.h"
#include "AccelByteOauth2Models.generated.h"

// Emulate namespace with long class names

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FOauth2TokenPermission
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token | Permission")
	FString Resource{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token | Permission")
	int32 Action{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FOauth2TokenBan
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token | Ban")
	FString Ban{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token | Ban")
	FString TargetedNamespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token | Ban")
	FDateTime EndDate{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token | Ban")
	bool Enabled{false};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FOauth2TokenNamespaceRole
{
	GENERATED_BODY()	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token | NamespaceRole")
	FString RoleId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token | NamespaceRole")
	FString Namespace{};
};
	
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FOauth2Token
{
	// UE4 JSON parser is case-insensitive and we have to match the variable names against the backend's model.
	// But "namespace" is a reserved keyword so we capitalize the first character.
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Access_token{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Auth_Trust_Id{};	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Refresh_token{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Token_type{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	float Expires_in{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	float Refresh_expires_in{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	bool Is_comply{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	TArray<FOauth2TokenPermission> Permissions{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	TArray<FString> Roles{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	TArray<FOauth2TokenNamespaceRole> Namespace_roles{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	TArray<FOauth2TokenBan> Bans{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString User_id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Display_name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Platform_id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Platform_user_id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Unique_display_name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Simultaneous_platform_id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Simultaneous_platform_user_id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FOauth2Session
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Session")
	FString Session_id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Session")
	int32 Expires_in{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Session")
	FString Refresh_id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FJwkSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | JWKSet")
	TArray<FJsonObjectWrapper> keys{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FRefreshInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | FRefreshInfo")
	FString RefreshToken {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | FRefreshInfo")
	FDateTime Expiration {0}; //Stored as UTC Now + TimeSpan Expiration Time
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPlatformTokenRefreshResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | FPlatformTokenRefreshResponse")
	FString AvatarUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | FPlatformTokenRefreshResponse")
	FString Country{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | FPlatformTokenRefreshResponse")
	FString DisplayName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | FPlatformTokenRefreshResponse")
	FString PlatformTokenExpiresAt{}; //Invalid date time format therefore it can't use FDateTime

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | FPlatformTokenRefreshResponse")
	FString PlatformUserId{};
};

USTRUCT(BluePrintType)
struct ACCELBYTEUE4SDK_API FOauth2TokenV4 : public FOauth2Token
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | OAuth2V4")
	FString Ticket{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | OAuth2V4")
	int32 Position{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | OAuth2V4")
	int32 EstimatedWaitingTimeInSeconds{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | OAuth2V4")
	int32 ReconnectExpiredAt{0};// UNIX seconds since epoch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | OAuth2V4")
	int32 PlayerPollingTimeInSeconds{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLoginQueueTicketInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | LoginQueueTicketInfo")
	FString Ticket{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | LoginQueueTicketInfo")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | LoginQueueTicketInfo")
	int32 Position{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | LoginQueueTicketInfo")
	int32 EstimatedWaitingTimeInSeconds{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | LoginQueueTicketInfo")
	int32 ReconnectExpiredAt{0};// UNIX seconds since epoch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | LoginQueueTicketInfo")
	int32 PlayerPollingTimeInSeconds{0};

	FAccelByteModelsLoginQueueTicketInfo() {};

	FAccelByteModelsLoginQueueTicketInfo(const FAccelByteModelsLoginQueueTicketInfo& TicketInfo) 
	{
		Ticket = TicketInfo.Ticket;
		Namespace = TicketInfo.Namespace;
		Position = TicketInfo.Position;
		EstimatedWaitingTimeInSeconds = TicketInfo.EstimatedWaitingTimeInSeconds;
		ReconnectExpiredAt = TicketInfo.ReconnectExpiredAt;
		PlayerPollingTimeInSeconds = TicketInfo.PlayerPollingTimeInSeconds;
	};

	FAccelByteModelsLoginQueueTicketInfo(const FOauth2TokenV4& TicketInfo)
	{
		Ticket = TicketInfo.Ticket;
		Position = TicketInfo.Position;
		EstimatedWaitingTimeInSeconds = TicketInfo.EstimatedWaitingTimeInSeconds;
		ReconnectExpiredAt = TicketInfo.ReconnectExpiredAt;
		PlayerPollingTimeInSeconds = TicketInfo.PlayerPollingTimeInSeconds;
	}

	~FAccelByteModelsLoginQueueTicketInfo() {};
};