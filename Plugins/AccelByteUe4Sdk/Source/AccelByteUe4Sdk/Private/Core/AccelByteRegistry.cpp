// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteRegistry.h"
#include "AccelByteHttpRetryScheduler.h"
#include "AccelByteUserApi.h"
#include "AccelByteUserProfileApi.h"
#include "AccelByteCategoryApi.h"
#include "AccelByteEntitlementApi.h"
#include "AccelByteOrderApi.h"
#include "AccelByteItemApi.h"
#include "AccelByteWalletApi.h"
#include "AccelByteCloudStorageApi.h"
#include "AccelByteLobbyApi.h"
#include "AccelByteGameProfileApi.h"
#include "AccelByteStatisticApi.h"

using namespace AccelByte;
using namespace AccelByte::Api;

Settings FRegistry::Settings;
Credentials FRegistry::Credentials;
FHttpRetryScheduler FRegistry::HttpRetryScheduler;
Api::User FRegistry::User(FRegistry::Credentials, FRegistry::Settings);
Api::UserProfile FRegistry::UserProfile(FRegistry::Credentials, FRegistry::Settings);
Api::Category FRegistry::Category(FRegistry::Credentials, FRegistry::Settings);
Api::Entitlement FRegistry::Entitlement(FRegistry::Credentials, FRegistry::Settings);
Api::Order FRegistry::Order(FRegistry::Credentials, FRegistry::Settings);
Api::Item FRegistry::Item(FRegistry::Credentials, FRegistry::Settings);
Api::Wallet FRegistry::Wallet(FRegistry::Credentials, FRegistry::Settings);
Api::CloudStorage FRegistry::CloudStorage(FRegistry::Credentials, FRegistry::Settings);
Api::Lobby FRegistry::Lobby(FRegistry::Credentials, FRegistry::Settings);
Api::GameProfile FRegistry::GameProfile(FRegistry::Credentials, FRegistry::Settings);
Api::Statistic FRegistry::Statistic(FRegistry::Credentials, FRegistry::Settings);
