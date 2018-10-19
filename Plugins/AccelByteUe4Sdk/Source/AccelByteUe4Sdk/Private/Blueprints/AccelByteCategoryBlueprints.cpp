// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteCategoryBlueprints.h"
#include "AccelByteCategoryApi.h"
#include "AccelByteSettings.h"
#include "AccelByteCredentials.h"

using AccelByte::Api::Category;
using AccelByte::FErrorDelegate;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsCategory::GetRootCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FGetRootCategoriesSuccess OnSuccess, FBlueprintError OnError)
{
	Category::GetRootCategories(ServerBaseUrl, AccessToken, Namespace, Language,
		Category::FGetRootCategoriesSuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetRootCategoriesEasy(FString Language, FGetRootCategoriesSuccess OnSuccess, FBlueprintError OnError)
{
	GetRootCategories(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Language, OnSuccess, OnError);
}

void UAccelByteBlueprintsCategory::GetCategory(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString CategoryPath, FString Language, FGetCategorySuccess OnSuccess, FBlueprintError OnError)
{
	Category::GetCategory(ServerBaseUrl, AccessToken, Namespace, CategoryPath, Language,
		Category::FGetCategorySuccess::CreateLambda([OnSuccess](const FAccelByteModelsFullCategoryInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetCategoryEasy(FString ParentPath, FString Language, FGetCategorySuccess OnSuccess, FBlueprintError OnError)
{
	GetCategory(AccelByte::Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), AccelByte::Credentials::Get().GetUserNamespace(), ParentPath, Language, OnSuccess, OnError);
}

void UAccelByteBlueprintsCategory::GetChildCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, FBlueprintError OnError)
{
	Category::GetChildCategories(ServerBaseUrl, AccessToken, Namespace, Language, CategoryPath,
		Category::FGetChildCategoriesSuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetChildCategoriesEasy(FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, FBlueprintError OnError)
{
	 GetChildCategories(AccelByte::Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), AccelByte::Credentials::Get().GetUserNamespace(), Language, CategoryPath, OnSuccess, OnError);
}

void UAccelByteBlueprintsCategory::GetDescendantCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetDescendantCategoriesSuccess OnSuccess, FBlueprintError OnError)
{
	Category::GetDescendantCategories(ServerBaseUrl, AccessToken, Namespace, Language, CategoryPath,
		Category::FGetDescendantCategoriesSuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetDescendantCategoriesEasy(FString Language, FString CategoryPath, FGetDescendantCategoriesSuccess OnSuccess, FBlueprintError OnError)
{
	GetDescendantCategories(AccelByte::Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), AccelByte::Credentials::Get().GetUserNamespace(), Language, CategoryPath, OnSuccess, OnError);
}
