// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "IWebSocket.h"

class IWebSocketFactory : public TSharedFromThis<IWebSocketFactory>
{
public:
	virtual ~IWebSocketFactory() = default;
	virtual TSharedRef<IWebSocket> CreateWebSocket(const FString&, const FString&, const TMap<FString, FString>&) = 0;
};