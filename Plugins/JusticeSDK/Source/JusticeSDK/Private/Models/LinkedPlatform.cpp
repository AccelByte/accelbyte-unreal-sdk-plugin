// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "LinkedPlatform.h"

void ULinkedPlatform::FromLinkedPlatform(LinkedPlatform linkedPlatform)
{
	this->PlatformId		= linkedPlatform.PlatformId;
	this->PlatformUserId	= linkedPlatform.PlatformUserId;
	this->Namespace			= linkedPlatform.Namespace;
	this->UserId		=	 linkedPlatform.UserId;
}