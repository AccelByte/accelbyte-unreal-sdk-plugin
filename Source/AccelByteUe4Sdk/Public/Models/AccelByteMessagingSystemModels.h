// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

enum class EAccelByteMessagingTopic : uint8
{
	None,
	LobbyConnected,
	QosRegionLatenciesUpdated
};

struct FAccelByteModelsMessagingSystemMessage
{
	EAccelByteMessagingTopic Topic{EAccelByteMessagingTopic::None};
	FString Payload{};
};
