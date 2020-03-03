// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteUserProfileApi.h"
#include "Api/AccelByteCategoryApi.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Api/AccelByteOrderApi.h"
#include "Api/AccelByteItemApi.h"
#include "Api/AccelByteWalletApi.h"
#include "Api/AccelByteCloudStorageApi.h"
#include "Api/AccelByteLobbyApi.h"
#include "Api/AccelByteGameProfileApi.h"
#include "Api/AccelByteStatisticApi.h"
#include "Api/AccelByteQosManagerApi.h"
#include "Api/AccelByteQos.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerDSMApi.h"
#include "GameServerApi/AccelByteServerStatisticApi.h"
#include "GameServerApi/AccelByteServerEcommerceApi.h"

using namespace AccelByte;
using namespace AccelByte::Api;

Settings FRegistry::Settings;
Credentials FRegistry::Credentials;
ServerSettings FRegistry::ServerSettings;
ServerCredentials FRegistry::ServerCredentials;
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
Api::QosManager FRegistry::QosManager(FRegistry::Credentials, FRegistry::Settings);
Api::Qos FRegistry::Qos;
GameServerApi::ServerOauth2 FRegistry::ServerOauth2(FRegistry::ServerCredentials, FRegistry::ServerSettings);
GameServerApi::ServerDSM FRegistry::ServerDSM(FRegistry::ServerCredentials, FRegistry::ServerSettings);
GameServerApi::ServerStatistic FRegistry::ServerStatistic(FRegistry::ServerCredentials, FRegistry::ServerSettings);
GameServerApi::ServerEcommerce FRegistry::ServerEcommerce(FRegistry::ServerCredentials, FRegistry::ServerSettings);
