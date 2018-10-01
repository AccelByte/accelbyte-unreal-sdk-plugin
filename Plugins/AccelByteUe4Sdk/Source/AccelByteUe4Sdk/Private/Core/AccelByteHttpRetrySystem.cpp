// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteHttpRetrySystem.h"

namespace AccelByte
{

bool FRetrySystemTicker::Tick(float DeltaTime)
{
	return RetrySystem.Manager.Update();
}

}
