// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteGeneralModels.h"
#include "AccelByteSessionBrowserModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserData
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
		FString Session_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
		FString User_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
		FString User_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
		FString Session_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
		FString Game_version;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
		FString Game_mode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
		FString Game_map_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
		int Game_num_bot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
		int Game_max_player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
		int Game_current_player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | SessionBrowser | Models | SessionBrowserData")
		bool Game_allow_join_in_progress;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionBrowserGetResult
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | SessionBrowserGetResult")
		TArray<FAccelByteModelsSessionBrowserData> Sessions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | SessionBrowserGetResult")
		FAccelByteModelsPaging pagination;
};