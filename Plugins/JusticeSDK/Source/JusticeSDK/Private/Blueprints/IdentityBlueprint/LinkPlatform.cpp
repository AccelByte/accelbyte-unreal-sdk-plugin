// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/LinkPlatform.h"
#include "Services/JusticeIdentity.h"

UAsyncLinkPlatform * UAsyncLinkPlatform::LinkPlatform(FString PlatformID, FString Ticket)
{
	UAsyncLinkPlatform* Node = NewObject<UAsyncLinkPlatform>();
	Node->PlatformID = PlatformID;
	Node->Ticket = Ticket;
	return Node;
}

void UAsyncLinkPlatform::Activate()
{
	JusticeIdentity::LinkPlatform(this->PlatformID, this->Ticket, FLinkPlatformCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr) {
		if (bSuccessful)
		{
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast();
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast();
			}
		}
	}));
}