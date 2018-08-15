// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/CatalogBlueprint/GetChildCategory.h"
#include "Services/JusticeCatalog.h"

UGetChildCategory * UGetChildCategory::GetChildCategory(FString Language, FString CategoryPath)
{
	UGetChildCategory* Node = NewObject<UGetChildCategory>();
	Node->Language = Language;
	Node->CategoryPath = CategoryPath;
	return Node;
}

void UGetChildCategory::Activate()
{
	JusticeCatalog::GetChildCategory(this->Language, this->CategoryPath, FCategoryDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, TArray<FCategory> Result) {
        TArray<UCategory*> ChildCategories;
        if (bSuccessful)
		{
			for (int32 i = 0; i < Result.Num(); i++)
			{
				UCategory* ChildCategory = UCategory::Deserialize(Result[i]);				
				check(ChildCategory);
				ChildCategories.Add(ChildCategory);
			}            
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(ChildCategories);
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(ChildCategories);
			}
		}
	}));
}