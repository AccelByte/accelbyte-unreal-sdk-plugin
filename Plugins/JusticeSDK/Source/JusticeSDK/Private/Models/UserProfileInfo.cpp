// Fill out your copyright notice in the Description page of Project Settings.

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