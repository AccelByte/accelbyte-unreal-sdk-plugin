// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABUtilities.h"

FString UABUtilities::DateTimeToString(const FDateTime& DateTime)
{
	return DateTime.ToIso8601();
}