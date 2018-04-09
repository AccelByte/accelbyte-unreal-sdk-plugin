// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "UserProfileInfo.h"

void UUserProfileJustice::FromUserProfileInfo(UserProfileInfo info)
{
	this->UserId = info.UserId;
	this->Namespace = info.Namespace;
	this->DisplayName = info.DisplayName;
	this->FirstName = info.FirstName;
	this->LastName = info.LastName;
	this->Country = info.Country;
	this->AvatarSmallUrl = info.AvatarSmallUrl;
	this->AvatarUrl = info.AvatarUrl;
	this->AvatarLargeUrl = info.AvatarLargeUrl;
	this->Email = info.Email;
	this->Status = info.Status;
	this->CustomAttributes = info.CustomAttributes;
}