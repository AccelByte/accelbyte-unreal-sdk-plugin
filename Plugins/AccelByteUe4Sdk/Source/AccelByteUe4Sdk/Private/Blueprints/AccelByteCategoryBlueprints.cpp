// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteCategoryBlueprints.h"
#include "AccelByteCategoryApi.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"

using AccelByte::Api::Category;
using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsCategory::GetRootCategories(const FString& Language, const FGetRootCategoriesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Category::GetRootCategories(Language, THandler<TArray<FAccelByteModelsFullCategoryInfo>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
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
	Category::GetCategory(CategoryPath, Language, THandler<FAccelByteModelsFullCategoryInfo>::CreateLambda([OnSuccess](const FAccelByteModelsFullCategoryInfo& Result)
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
	Category::GetChildCategories(Language, CategoryPath, THandler<TArray<FAccelByteModelsFullCategoryInfo>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
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
	Category::GetDescendantCategories(Language, CategoryPath, THandler<TArray<FAccelByteModelsFullCategoryInfo>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
