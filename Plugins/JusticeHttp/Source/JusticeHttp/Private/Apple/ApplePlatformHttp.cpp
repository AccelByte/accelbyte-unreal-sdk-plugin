// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ApplePlatformHttp.h"
#include "AppleHTTP.h"


void FApplePlatformHttp::Init()
{
}


void FApplePlatformHttp::Shutdown()
{
}


IJusticeHttpRequest* FApplePlatformHttp::ConstructRequest()
{
	return new FAppleHttpRequest();
}
