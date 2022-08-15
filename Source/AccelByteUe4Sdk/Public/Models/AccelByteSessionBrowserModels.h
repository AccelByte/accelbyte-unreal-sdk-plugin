// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteGeneralModels.h"
#include "Models/AccelByteMatchmakingModels.h"
#include "JsonObjectWrapper.h"
#include "AccelByteSessionBrowserModels.generated.h"

#pragma region EnumField

UENUM(BlueprintType)
enum class EAccelByteSessionType : uint8
{
	NONE = 0,
	p2p,
	dedicated,
};

#pragma endregion EnumField

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserServer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Ip{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	int Port{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Region{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Status{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Pod_name{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Deployment{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Game_version{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserGameSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Mode{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Map_name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	int Num_bot{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	int Max_player{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	int Current_player{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	int Max_internal_player{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	int Current_internal_player{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	bool Allow_join_in_progress{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Password{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FJsonObjectWrapper Settings{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserCreateRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Session_type{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Username{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Game_version{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FAccelByteModelsSessionBrowserGameSetting Game_session_setting{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Session_id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString User_id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Username{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Session_type{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FString Game_version{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	bool Joinable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FAccelByteModelsSessionBrowserGameSetting Game_session_setting{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FAccelByteModelsSessionBrowserServer Server{};

	/** @brief all players including player that leave the session. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	TArray<FString> All_players{};

	/** @brief Current active players */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	TArray<FString> Players{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	TArray<FString> Spectators{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	FAccelByteModelsMatchmakingResult Match{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserUpdateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	int Game_current_player{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
	int Game_max_player{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserGetResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | SessionBrowserGetResult")
	TArray<FAccelByteModelsSessionBrowserData> Sessions{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | SessionBrowserGetResult")
	FAccelByteModelsPaging pagination{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserGetByUserIdsResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | SessionBrowserGetResult")
	TArray<FAccelByteModelsSessionBrowserData> Data{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserAddPlayerRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | SessionBrowserGetResult")
	FString User_id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | SessionBrowserGetResult")
	bool As_spectator{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserAddPlayerResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | SessionBrowserGetResult")
	bool Status{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserRecentPlayerData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | SessionBrowserGetResult")
	FString Other_id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | SessionBrowserGetResult")
	FString Other_display_name{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserRecentPlayerGetResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | SessionBrowserGetResult")
	TArray<FAccelByteModelsSessionBrowserRecentPlayerData> Data{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserJoinSessionRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | SessionBrowserGetResult")
	FString Password{};
};