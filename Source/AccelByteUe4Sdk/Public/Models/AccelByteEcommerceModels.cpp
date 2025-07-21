// Copyright (c) 2025 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteEcommerceModels.h"
#include "JsonObjectConverter.h"

TSharedPtr<FJsonObject> FAccelByteModelsFulfillRewards::GenerateFilteredJsonObject() const
{
	auto JsonObject = FJsonObjectConverter::UStructToJsonObject(*this);
	if (JsonObject == nullptr)
	{
		return nullptr;
	}

	if (Origin == EAccelBytePlatformRewardOrigin::NONE &&
		JsonObject->HasField(TEXT("Origin")))
	{
		JsonObject->RemoveField(TEXT("Origin"));
	}

	if (Source == EAccelBytePlatformRewardSource::NONE &&
		JsonObject->HasField(TEXT("Source")))
	{
		JsonObject->RemoveField(TEXT("Source"));
	}

	return JsonObject;
}