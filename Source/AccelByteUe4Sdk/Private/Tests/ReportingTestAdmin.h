// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteReportingModels.h"
#include "TestUtilities.h"
#include "ReportingTestAdmin.generated.h"

USTRUCT(BlueprintType)
struct FReportingAddReasonRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Reporting | AddReasonRequest")
		FString Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Reporting | AddReasonRequest")
		FString Description;
};

USTRUCT(BlueprintType)
struct FReportingAddReasonGroupRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Reporting | AddReasonGroupRequest")
		TArray<FString> ReasonIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Reporting | AddReasonGroupRequest")
		FString Title;
};

USTRUCT(BlueprintType)
struct FReportingAddReasonResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Reporting | AddReasonResponse")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Reporting | AddReasonResponse")
		int32 Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Reporting | AddReasonResponse")
		FString IDc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Reporting | AddReasonResponse")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Reporting | AddReasonResponse")
		FString Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Reporting | AddReasonResponse")
		FString UpdatedAt;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAdminReasonItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test | Reporting | AdminReasonItem")
		FString ID;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test | Reporting | AdminReasonItem")
		FString Title;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test | Reporting | AdminReasonItem")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test | Reporting | AdminReasonItem")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test | Reporting | AdminReasonItem")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test | Reporting | AdminReasonItem")
		FString UpdatedAt;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAdminReasonsResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test | Reporting | AdminReasonsResponse")
		TArray<FAccelByteModelsAdminReasonItem> Data;
};

USTRUCT(BlueprintType)
struct FReportingAddReasonGroupResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Reporting | AddReasonGroupResponse")
		FString ID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Reporting | AddReasonGroupResponse")
		TArray<FAccelByteModelsReasonItem> Reasons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Reporting | AddReasonGroupResponse")
		FString Title;
};

void ReportingAddReason(const FReportingAddReasonRequest& BodyRequest, const THandler<FReportingAddReasonResponse>& OnSuccess, const FErrorHandler& OnError);
void ReportingDeleteReason(const FString& ReasonID, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
void ReportingAdminGetReasons(const FString& ReasonGroup, const THandler<FAccelByteModelsAdminReasonsResponse>& OnSuccess, const FErrorHandler& OnError);
void ReportingAddReasonGroup(const FReportingAddReasonGroupRequest& BodyRequest, const THandler<FReportingAddReasonGroupResponse>& OnSuccess, const FErrorHandler& OnError);
void ReportingAdminGetReasonGroups(const THandler<FAccelByteModelsReasonGroupsResponse>& OnSuccess, const FErrorHandler& OnError);
void ReportingDeleteReasonGroup(const FString& ReasonGroupID, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);