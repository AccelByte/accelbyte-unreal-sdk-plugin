// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#include "ItemInfo.h"

void UItemInfo::FromItemInfo(ItemInfo item)
{	
	this->ItemId			= item.ItemId;
	this->AppId				= item.AppId;
	this->Namespace			= item.Namespace;
	this->EntitlementName	= item.EntitlementName;
	this->EntitlementType	= item.EntitlementType;
	this->UseCount			= item.UseCount;
	this->CategoryPath		= item.CategoryPath;
	this->Title				= item.Title;
	this->Details			= item.Details;
	this->ThumbnailImage	= item.ThumbnailImage;
	this->Images			= item.Images;
	this->PriceInfo			= item.PriceInfo;
	this->Status			= item.Status;
	this->ItemType			= item.ItemType;
	this->CreatedAt			= item.CreatedAt;
	this->UpdatedAt			= item.UpdatedAt;
	this->FavoritCnt		= item.FavoritCnt;
}