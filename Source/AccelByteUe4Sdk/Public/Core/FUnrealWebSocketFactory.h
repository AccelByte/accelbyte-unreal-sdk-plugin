// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "IWebSocketFactory.h"

class FUnrealWebSocketFactory : public IWebSocketFactory
{
public:
	FUnrealWebSocketFactory() = default;
	virtual TSharedRef<IWebSocket> CreateWebSocket(const FString&, const FString&, const TMap<FString, FString>&) override;
};