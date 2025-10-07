// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"
#include "Models/AccelByteGeneralModels.h"
#include "AccelByteErrorModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FErrorInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
	int32 NumericErrorCode{-1};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
	int32 ErrorCode{-1};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
	int32 Code{-1};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
	FString ErrorMessage{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
	FString Message{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Error")
	FString Error{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUserBan
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserBan")
	EBanReason Reason{EBanReason::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserBan")
	FString EndDate{};
	FDateTime EndDateTime() const {return FDateTime::FromUnixTimestamp(FCString::Atoi(*EndDate));}
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserBan")
	FString Comment{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FErrorOAuthInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OauthError")
	int32 ErrorCode{-1};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OauthError")
	FString ErrorMessage{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OauthError")
	FString Error{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OauthError")
	FString Error_description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OauthError")
	FString Error_uri{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OauthError")
	FString Mfa_token{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OauthError")
	TArray<FString> Factors{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OauthError")
	FString Default_factor{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OAuthError")
	FString Email{};	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OauthError")
	FString PlatformId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OauthError")
	FString LinkingToken{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OauthError")
	FString ClientId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OAuthError")
	FUserBan UserBan{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | OAuthError")
	FJsonObjectWrapper MessageVariables{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2CreateTicketErrorDetails
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | CreateMatchmakingV2Error")
	FString TicketID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FErrorCreateMatchmakingTicketV2
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | CreateMatchmakingV2Error")
	int32 ErrorCode{-1};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | CreateMatchmakingV2Error")
	FString ErrorMessage{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | CreateMatchmakingV2Error")
	FString ExistingTicketID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | CreateMatchmakingV2Error")
	FAccelByteModelsV2CreateTicketErrorDetails MessageVariables{};
};