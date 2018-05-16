// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#include "ItemInfo.h"

void UItemInfo::FromItemInfo(ItemInfo item)
{	
	this->Title				= item.Title;
	this->Description		= item.Description;
	this->LongDescription	= item.LongDescription;
	this->Images			= item.Images;
	this->ThumbnailImage	= item.ThumbnailImage;
	this->ItemId			= item.ItemId;
	this->AppId				= item.AppId;
	this->Namespace			= item.Namespace;
	this->StoreId			= item.StoreId;
	this->EntitlementName	= item.EntitlementName;
	this->EntitlementType	= item.EntitlementType;
	this->UseCount			= item.UseCount;
	this->CategoryPath		= item.CategoryPath;
	this->Status			= item.Status;
	this->ItemType			= item.ItemType;
	this->CreatedAt			= item.CreatedAt;
	this->UpdatedAt			= item.UpdatedAt;
	this->RegionDatas		= item.RegionDatas;
	this->ItemIds			= item.ItemIds;
	this->Tags = item.Tags;
}