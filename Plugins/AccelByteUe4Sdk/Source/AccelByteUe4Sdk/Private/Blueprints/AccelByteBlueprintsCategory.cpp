// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteBlueprintsCategory.h"
#include "AccelByteServicesCategory.h"

using namespace AccelByte;
using namespace AccelByte::Services;

UAccelByteCatalogGetChildCategories* UAccelByteCatalogGetChildCategories::GetChildCategories(FString Language, FString CategoryPath, FGetChildCategoriesSuccess OnSuccess, FBlueprintErrorDelegate OnError)
{
	UAccelByteCatalogGetChildCategories* Node = NewObject<UAccelByteCatalogGetChildCategories>();
	Node->Language = Language;
	Node->CategoryPath = CategoryPath;
	Node->OnSuccess = OnSuccess;
	Node->OnError = OnError;
	return Node;
}

void UAccelByteCatalogGetChildCategories::Activate()
{
#if 0
	Category::GetChildCategories(this->Language, this->CategoryPath, Category::FGetChildCategoriesSuccess::CreateLambda([this](const TArray<FAccelByteModelsFullCategoryInfo>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
		/*Result.Reset();*/
	}),
		ErrorDelegate::CreateLambda([this](int32 ErrorCode, FString ErrorString)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorString);
	}));
#endif
}