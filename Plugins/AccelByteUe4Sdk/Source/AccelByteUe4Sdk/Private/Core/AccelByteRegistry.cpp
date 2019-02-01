// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteRegistry.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"
#include "AccelByteHttpRetryScheduler.h"
#include "AccelByteLobbyApi.h"

using namespace AccelByte;

Settings FRegistry::Settings;
Credentials FRegistry::Credentials;
FHttpRetryScheduler FRegistry::HttpRetryScheduler;
Api::Lobby FRegistry::Lobby(FRegistry::Credentials, FRegistry::Settings);
