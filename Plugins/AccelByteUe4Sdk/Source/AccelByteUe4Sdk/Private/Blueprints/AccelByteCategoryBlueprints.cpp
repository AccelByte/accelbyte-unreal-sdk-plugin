// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteCategoryBlueprints.h"
#include "Api/AccelByteCategoryApi.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::Api::Category;
using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsCategory::GetRootCategories(const FString& Language, const FGetRootCategoriesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Category.GetRootCategories(Language, THandler<TArray<FAccelByteModelsCategoryInfo>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetCategory(const FString& CategoryPath, const FString& Language, const FGetCategorySuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Category.GetCategory(CategoryPath, Language, THandler<FAccelByteModelsCategoryInfo>::CreateLambda([OnSuccess](const FAccelByteModelsCategoryInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetChildCategories(const FString& Language, const FString& CategoryPath, const FGetChildCategoriesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Category.GetChildCategories(Language, CategoryPath, THandler<TArray<FAccelByteModelsCategoryInfo>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetDescendantCategories(const FString& Language, const FString& CategoryPath, const FGetDescendantCategoriesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Category.GetDescendantCategories(Language, CategoryPath, THandler<TArray<FAccelByteModelsCategoryInfo>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
