// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

enum EAccelByteIdHypensRule
{
	NO_RULE,
	NO_HYPENS,
	WITH_HYPENS
};

namespace AccelByte
{
class ACCELBYTEUE4SDK_API FAccelByteIdValidator
{
public:
	static bool IsAccelByteIdValid(FString const& AccelByteId, EAccelByteIdHypensRule HypenRule = EAccelByteIdHypensRule::NO_RULE);
	static FString GetChatIdInvalidMessage(FString const& ChatId);
	static FString GetPartyIdInvalidMessage(FString const& PartyId);
	static FString GetSessionIdInvalidMessage(FString const& SessionId);
	static FString GetUserIdInvalidMessage(FString const& UserId);
	static FString GetChannelIdInvalidMessage(FString const& ChannelId);
	static FString GetContentIdInvalidMessage(FString const& ContentId);
	static FString GetScreenshotIdInvalidMessage(FString const& ScreenshotId);
	static FString GetEntitlementIdInvalidMessage(FString const& EntitlementId);
	static FString GetObjectIdInvalidMessage(FString const& ObjectId);
};
}