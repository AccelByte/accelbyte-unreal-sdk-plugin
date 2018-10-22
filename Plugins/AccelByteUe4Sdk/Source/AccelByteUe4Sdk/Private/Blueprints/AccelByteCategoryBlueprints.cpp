// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteCategoryBlueprints.h"
#include "AccelByteCategoryApi.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"

using AccelByte::Api::Category;
using AccelByte::FErrorHandler;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsCategory::GetRootCategories(const FString& AccessToken, const FString& Namespace, const FString& Language, const FGetRootCategoriesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Category::GetRootCategories(AccessToken, Namespace, Language, Category::FGetRootCategoriesSuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetRootCategoriesEasy(const FString& Language, const FGetRootCategoriesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	GetRootCategories(Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, OnSuccess, OnError);
}

void UAccelByteBlueprintsCategory::GetCategory(const FString& AccessToken, const FString& Namespace, const FString& CategoryPath, const FString& Language, const FGetCategorySuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Category::GetCategory(AccessToken, Namespace, CategoryPath, Language, Category::FGetCategorySuccess::CreateLambda([OnSuccess](const FAccelByteModelsFullCategoryInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetCategoryEasy(const FString& ParentPath, const FString& Language, const FGetCategorySuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	GetCategory(Credentials::Get().GetUserAccessToken(), AccelByte::Credentials::Get().GetUserNamespace(), ParentPath, Language, OnSuccess, OnError);
}

void UAccelByteBlueprintsCategory::GetChildCategories(const FString& AccessToken, const FString& Namespace, const FString& Language, const FString& CategoryPath, const FGetChildCategoriesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Category::GetChildCategories(AccessToken, Namespace, Language, CategoryPath, Category::FGetChildCategoriesSuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetChildCategoriesEasy(const FString& Language, const FString& CategoryPath, const FGetChildCategoriesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	GetChildCategories(Credentials::Get().GetUserAccessToken(), AccelByte::Credentials::Get().GetUserNamespace(), Language, CategoryPath, OnSuccess, OnError);
}

void UAccelByteBlueprintsCategory::GetDescendantCategories(const FString& AccessToken, const FString& Namespace, const FString& Language, const FString& CategoryPath, const FGetDescendantCategoriesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	Category::GetDescendantCategories(AccessToken, Namespace, Language, CategoryPath, Category::FGetDescendantCategoriesSuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetDescendantCategoriesEasy(const FString& Language, const FString& CategoryPath, const FGetDescendantCategoriesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	GetDescendantCategories(Credentials::Get().GetUserAccessToken(), AccelByte::Credentials::Get().GetUserNamespace(), Language, CategoryPath, OnSuccess, OnError);
}
