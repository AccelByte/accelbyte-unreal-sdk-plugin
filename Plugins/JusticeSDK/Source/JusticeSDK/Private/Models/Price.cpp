// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Price.h"

void UPrice::FromPrice(Price price)
{
	this->CurrencyCode = price.CurrencyCode;
	this->CurrencyType = price.CurrencyType;
	this->Value = price.Value;
	this->Namespace = price.Namespace;
}

