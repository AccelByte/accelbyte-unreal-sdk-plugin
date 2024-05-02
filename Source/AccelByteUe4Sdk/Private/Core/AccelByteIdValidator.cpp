// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteIdValidator.h"

using namespace AccelByte;

// AccelByte IDs have a max length of 32, as they are UUIDs that are striped of their hyphens
#define ACCELBYTE_ID_LENGTH 32
#define ACCELBYTE_ID_LENGTH_WITH_HYPENS 36

// Contains list of user IDs that don't have the UUIDv4 format
const TArray<FString> AllowedUserIdList =
{
	"official"
};

bool FAccelByteIdValidator::IsAccelByteIdValid(FString const& AccelByteId, EAccelByteIdHypensRule HypenRule)
{
	if (AccelByteId.IsEmpty())
	{
		return false;
	}
	if (AllowedUserIdList.Contains(AccelByteId))
	{
		return true;
	}

	// delete the "-client" prefix, session service will add this prefix if a session is created by non user client ID
	FString ProcessedAccelByteId = AccelByteId;
	ProcessedAccelByteId.RemoveFromStart(TEXT("client-"));

	int32 ExpectedAccelByteIdLength = ACCELBYTE_ID_LENGTH;
	EGuidFormats RuleFormat = EGuidFormats::Digits;

	if (HypenRule == EAccelByteIdHypensRule::NO_RULE)
	{
		if (ProcessedAccelByteId.Len() > ACCELBYTE_ID_LENGTH)
		{
			ExpectedAccelByteIdLength = ACCELBYTE_ID_LENGTH_WITH_HYPENS;
			RuleFormat = EGuidFormats::DigitsWithHyphens;
		}
	}
	else if (HypenRule == EAccelByteIdHypensRule::WITH_HYPENS)
	{
		ExpectedAccelByteIdLength = ACCELBYTE_ID_LENGTH_WITH_HYPENS;
		RuleFormat = EGuidFormats::DigitsWithHyphens;
	}

	if (ProcessedAccelByteId.Len() != ExpectedAccelByteIdLength)
	{
		return false;
	}

	FGuid OutId;
	return FGuid::ParseExact(ProcessedAccelByteId, RuleFormat, OutId);
}

FString FAccelByteIdValidator::GetChatIdInvalidMessage(FString const& ChatId)
{
	return FString::Printf(TEXT("Invalid request, Chat Id format is invalid\nValue: %s"), *ChatId);
}

FString FAccelByteIdValidator::GetPartyIdInvalidMessage(FString const& PartyId)
{
	return FString::Printf(TEXT("Invalid request, Party Id format is invalid\nValue: %s"), *PartyId);
}

FString FAccelByteIdValidator::GetSessionIdInvalidMessage(FString const& SessionId)
{
	return FString::Printf(TEXT("Invalid request, Session Id format is invalid\nValue: %s"), *SessionId);
}

FString FAccelByteIdValidator::GetUserIdInvalidMessage(FString const& UserId)
{
	return FString::Printf(TEXT("Invalid request, User Id format is invalid\nValue: %s"), *UserId);
}

FString AccelByte::FAccelByteIdValidator::GetChannelIdInvalidMessage(FString const& ChannelId)
{
	return FString::Printf(TEXT("Invalid request, Channel Id format is invalid\nValue: %s"), *ChannelId);
}

FString AccelByte::FAccelByteIdValidator::GetContentIdInvalidMessage(FString const& ContentId)
{
	return FString::Printf(TEXT("Invalid request, Content Id format is invalid\nValue: %s"), *ContentId);
}

FString AccelByte::FAccelByteIdValidator::GetScreenshotIdInvalidMessage(FString const& ScreenshotId)
{
	return FString::Printf(TEXT("Invalid request, Screenshot Id format is invalid\nValue: %s"), *ScreenshotId);
}

FString AccelByte::FAccelByteIdValidator::GetEntitlementIdInvalidMessage(FString const& EntitlementId)
{
	return FString::Printf(TEXT("Invalid request, Entitlement Id format is invalid\nValue: %s"), *EntitlementId);
}
