// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/CatalogBlueprint/GetDescendantCategory.h"
#include "Services/JusticeCatalog.h"

UGetDescendantCategory * UGetDescendantCategory::GetDescendantCategory(FString Language, FString CategoryPath)
{
	UGetDescendantCategory* Node = NewObject<UGetDescendantCategory>();
	Node->Language = Language;
	Node->CategoryPath = CategoryPath;  
	return Node;
}

void UGetDescendantCategory::Activate()
{
	JusticeCatalog::GetDescendantCategory(this->Language, this->CategoryPath, FCategoryDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString, TArray<Category> Result) {		
        TArray<UCategory*> DescendantCategories;
        if (bSuccessful)
		{
			for (int32 i = 0; i < Result.Num(); i++)
			{
				UCategory* DescendantCategory = UCategory::Deserialize(Result[i]);				
				check(DescendantCategory);
				DescendantCategories.Add(DescendantCategory);
			}            
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(DescendantCategories);
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(DescendantCategories);
			}
		}
	}));
}