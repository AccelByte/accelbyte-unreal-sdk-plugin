// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/FUnrealWebSocketFactory.h"
#include "WebSocketsModule.h"

TSharedRef<IWebSocket> FUnrealWebSocketFactory::CreateWebSocket(const FString& Url, const FString& Protocol,
	const TMap<FString, FString>& UpgradeHeaders)
{
	return FWebSocketsModule::Get().CreateWebSocket(Url, Protocol, UpgradeHeaders);
}
