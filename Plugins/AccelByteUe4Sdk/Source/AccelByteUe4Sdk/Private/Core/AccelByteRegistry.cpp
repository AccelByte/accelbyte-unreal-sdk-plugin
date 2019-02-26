// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteRegistry.h"
#include "AccelByteHttpRetryScheduler.h"
#include "AccelByteLobbyApi.h"
#include "AccelByteGameProfileApi.h"

using namespace AccelByte;
using namespace AccelByte::Api;

Settings FRegistry::Settings;
Credentials FRegistry::Credentials;
FHttpRetryScheduler FRegistry::HttpRetryScheduler;
Api::Lobby FRegistry::Lobby(FRegistry::Credentials, FRegistry::Settings);
Api::GameProfile FRegistry::GameProfile(FRegistry::Credentials, FRegistry::Settings);
