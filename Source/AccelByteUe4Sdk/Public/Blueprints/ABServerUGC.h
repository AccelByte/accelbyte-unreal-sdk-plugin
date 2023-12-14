// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteUGCModels.h"
#include "ABServerUGC.generated.h"


#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCSearchContentsResponse, FAccelByteModelsUGCSearchContentsPagingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUGCServerResponse, FAccelByteModelsUGCResponse, Response);

#pragma endregion

UCLASS(Blueprintable, BlueprintType)
class UABServerUGC final : public UObject
{
	GENERATED_BODY()
public:
	void SetServerApiClient(AccelByte::FServerApiClientPtr const& NewServerApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | UGC | Api")
	void SearchContents(FAccelByteModelsUGCSearchContentsRequest const& Request
		, FDModelsUGCSearchContentsResponse const& OnSuccess
		, FDErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | UGC | Api")
	void SearchContentsSpecificToChannel(FString const& ChannelId
		, FAccelByteModelsUGCSearchContentsRequest const& Request
		, FDModelsUGCSearchContentsResponse const& OnSuccess
		, FDErrorHandler const& OnError
		, int32 Limit = 20
		, int32 Offset = 0);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | UGC | Api")
	void ModifyContentByShareCode(FString const& UserId
		, FString const& ChannelId
		, FString const& ShareCode
		, FAccelByteModelsUGCUpdateRequest const& ModifyRequest
		, FDModelsUGCServerResponse const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | UGC | Api")
	void DeleteContentByShareCode(FString const& UserId
		, FString const& ChannelId
		, FString const& ShareCode
		, FDHandler const& OnSuccess
		, FDErrorHandler const& OnError);

private:
	AccelByte::FServerApiClientPtr ApiClientPtr;
};
