// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/LinkPlatform.h"
#include "Services/JusticeIdentity.h"

UAsyncLinkPlatform * UAsyncLinkPlatform::LinkPlatform(FString PlatformId, FString Ticket)
{
	UAsyncLinkPlatform* Node = NewObject<UAsyncLinkPlatform>();
	Node->PlatformId = PlatformId;
	Node->Ticket = Ticket;
	return Node;
}

void UAsyncLinkPlatform::Activate()
{
	JusticeIdentity::LinkPlatform(this->PlatformId, this->Ticket, FLinkPlatformCompleteDelegate::CreateLambda([&](bool IsSuccess, FString ErrorStr) {
		if (IsSuccess)
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