// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteSettings.h"

namespace AccelByte
{

const FString Settings::ServerBaseUrl = TEXT("");
const FString Settings::ClientId = TEXT("");
const FString Settings::ClientSecret = TEXT(""); // IETF does not recommend that you put your client secrets in "the the source code of the application or an associated resource bundle". Reference: RFC 6819 Section 5.2.3.1.
const FString Settings::Namespace = TEXT("");

} // Namespace AccelByte
