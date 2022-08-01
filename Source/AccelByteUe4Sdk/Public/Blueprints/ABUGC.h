// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteUGCModels.h"
#include "ABUGC.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCResponse, FAccelByteModelsUGCResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCContentResponse, FAccelByteModelsUGCContentResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCPreviewResponse, FAccelByteModelsUGCPreview, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayUint8, TArray<uint8>, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCTagsPagingResponse, FAccelByteModelsUGCTagsPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCTypesPagingResponse, FAccelByteModelsUGCTypesPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCChannelResponse, FAccelByteModelsUGCChannelResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCChannelsPagingResponse, FAccelByteModelsUGCChannelsPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCUpdateLikeStatusToContentResponse, FAccelByteModelsUGCUpdateLikeStatusToContentResponse, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABUGC : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void CreateContentByStruct(
		FString const& ChannelId,
		FAccelByteModelsUGCRequest const& UGCRequest,
		FDModelsUGCResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void CreateContent(
		FString const& ChannelId,
		FString const& Name,
		FString const& Type,
		FString const& SubType,
		TArray<FString> const& Tags,
		TArray<uint8> const& Preview,
		FString const& FileExtension,
		FDModelsUGCResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		FString ContentType = TEXT("application/octet-stream")
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void ModifyContentByStruct(
		FString const& ChannelId,
		FString const& ContentId,
		FAccelByteModelsUGCRequest const& UGCRequest,
		FDModelsUGCResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void ModifyContent(
		FString const& ChannelId,
		FString const& ContentId,
		FString const& Name,
		FString const& Type,
		FString const& SubType,
		TArray<FString> const& Tags,
		TArray<uint8> const& Preview,
		FString const& FileExtension,
		FDModelsUGCResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		FString ContentType = TEXT("application/octet-stream")
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void DeleteContent(
		FString const& ChannelId,
		FString const& ContentId,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetContentByContentId(
		FString const& ContentId,
		FDModelsUGCContentResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetContentByShareCode(
		FString const& ShareCode,
		FDModelsUGCContentResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetContentPreviewAsStruct(
		FString const& ContentId,
		FDModelsUGCPreviewResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetContentPreviewAsIntArray(
		FString const& ContentId,
		FDArrayUint8 const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetTags(
		FDModelsUGCTagsPagingResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000,
		int32 Offset = 0
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetTypes(
		FDModelsUGCTypesPagingResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000,
		int32 Offset = 0
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void CreateChannel(
		FString const& ChannelName,
		FDModelsUGCChannelResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void GetChannels(
		FDModelsUGCChannelsPagingResponse const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 Limit = 1000,
		int32 Offset = 0
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void DeleteChannel(
		FString const& ChannelId,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UGC | Api")
	void UpdateLikeStatusToContent(const FString& ContentId,
		bool bLikeStatus,
		FDModelsUGCUpdateLikeStatusToContentResponse const& OnSuccess,
		FDErrorHandler const& OnError);

private:
	FApiClientPtr ApiClientPtr;
};
