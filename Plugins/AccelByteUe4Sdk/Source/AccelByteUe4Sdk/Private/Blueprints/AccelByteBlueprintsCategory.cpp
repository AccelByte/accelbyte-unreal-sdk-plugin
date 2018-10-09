// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteBlueprintsCategory.h"
#include "AccelByteServicesCategory.h"

using namespace AccelByte::Services;
using namespace AccelByte;

void UAccelByteBlueprintsCategory::GetRootCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FGetRootCategoriesSuccess OnSuccess, FBlueprintError OnError)
{
	Category::GetRootCategories(ServerBaseUrl, AccessToken, Namespace, Language,
		Category::FGetRootCategoriesSuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString ParentPath, FString Language, FGetCategoriesSuccess OnSuccess, FBlueprintError OnError)
{
	Category::GetCategories(ServerBaseUrl, AccessToken, Namespace, ParentPath, Language,
		Category::FGetCategoriesSuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetChildCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, FBlueprintError OnError)
{
	Category::GetChildCategories(ServerBaseUrl, AccessToken, Namespace, Language, CategoryPath,
		Category::FGetChildCategoriesSuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsCategory::GetDescendantCategories(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString Language, FString CategoryPath, FGetDescendantCategoriesSuccess OnSuccess, FBlueprintError OnError)
{
	Category::GetDescendantCategories(ServerBaseUrl, AccessToken, Namespace, Language, CategoryPath,
		Category::FGetDescendantCategoriesSuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		ErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
