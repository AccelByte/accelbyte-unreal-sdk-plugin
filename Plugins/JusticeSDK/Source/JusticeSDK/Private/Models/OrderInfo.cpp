// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "OrderInfo.h"

void UOrderInfo::FromOrderInfo(OrderInfo order)
{
	this->OrderNo		 = order.OrderNo;
	this->UserId		 = order.UserId;
	this->ItemId		 = order.ItemId;
	this->Price			 = order.Price;
	this->Currency		 = order.Currency;
	this->PaymentUrl	 = order.PaymentUrl;
	this->Transactions	 = order.Transactions;
	this->EntitlementIds = order.EntitlementIds;
	this->Status		 = order.Status;
	this->StatusReason	 = order.StatusReason;
	this->Namespace      = order.Namespace;
	this->CreatedAt		 = order.CreatedAt;
	this->UpdatedAt		 = order.UpdatedAt;
}
