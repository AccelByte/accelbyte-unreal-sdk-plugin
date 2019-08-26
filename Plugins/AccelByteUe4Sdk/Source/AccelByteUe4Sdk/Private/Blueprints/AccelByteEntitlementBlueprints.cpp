// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteEntitlementBlueprints.h"
#include "Core/AccelByteSettings.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::Api::Entitlement;
using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsEntitlement::QueryUserEntitlement(const FString & EntitlementName, const FString & ItemId, int32 Page, int32 Size, const FQueryUserEntitlementSuccess& OnSuccess, const FBlueprintErrorHandler& OnError, EAccelByteEntitlementClass EntitlementClass = EAccelByteEntitlementClass::NONE, EAccelByteAppType AppType = EAccelByteAppType::NONE)
{
	FRegistry::Entitlement.QueryUserEntitlement(EntitlementName, ItemId, Page, Size, THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda([OnSuccess](const FAccelByteModelsEntitlementPagingSlicedResult& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}), EntitlementClass = EAccelByteEntitlementClass::NONE, AppType = EAccelByteAppType::NONE);
}
