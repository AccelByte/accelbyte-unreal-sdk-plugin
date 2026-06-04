// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

// INCLUDE ORDER REQUIREMENT: This header must be included before CoreMinimal.h (or any UE
// header that declares namespace UI) in each translation unit. If a UE header has already
// been included, the #define UI below will silently rename subsequent uses of UI in that TU.
//
// In practice: include AccelByteOpenSSL.h (which pulls this file in) before any UE includes.
// For .cpp files using a precompiled header that already includes CoreMinimal.h, add this
// header only to .cpp files that do not use the UE PCH, or restructure includes accordingly.

#ifndef ACCELBYTE_OPENSSL_ST_UI_GUARD
#define ACCELBYTE_OPENSSL_ST_UI_GUARD

// OpenSSL 1.1.1 defines a C typedef `typedef struct ui_st UI` which conflicts with Unreal
// Engine's `namespace UI` declared in ObjectMacros.h. We temporarily rename OpenSSL's UI
// symbol during header inclusion to avoid the conflict, then restore the name with #undef UI
// so Unreal's UI namespace works normally. Wrapping in a C++ namespace is not viable because
// OpenSSL C headers rely on forward declarations (e.g. X509_ALGOR in x509.h) that break under
// Clang when placed inside a namespace.
#define UI ACCELBYTE_OPENSSL_ST_UI
THIRD_PARTY_INCLUDES_START
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/rand.h>
#include <openssl/aes.h>
THIRD_PARTY_INCLUDES_END
#undef UI

#endif // ACCELBYTE_OPENSSL_ST_UI_GUARD
