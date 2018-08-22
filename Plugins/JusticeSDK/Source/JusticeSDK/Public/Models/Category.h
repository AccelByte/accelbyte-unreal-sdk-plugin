// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeBaseModel.h"
#include "Category.generated.h"

struct FCategory : public FJsonSerializable
{
	FString Namespace;
	FString ParentCategoryPath;
	FString CategoryPath;
	FString DisplayName;
	FString CreatedAt;
	FString UpdatedAt;
	bool	Root;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("namespace", Namespace);
		JSON_SERIALIZE("parentCategoryPath", ParentCategoryPath);
		JSON_SERIALIZE("categoryPath", CategoryPath);
		JSON_SERIALIZE("displayName", DisplayName);
		JSON_SERIALIZE("createdAt", CreatedAt);
		JSON_SERIALIZE("updatedAt", UpdatedAt);
		JSON_SERIALIZE("root", Root);
	END_JSON_SERIALIZER
};


UCLASS()
class UCategory:public UObject, public FCategory, public FBaseModelJustice<UCategory, FCategory>
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "Category")
		FString GetCategoryDisplayName() { return DisplayName; };

	UFUNCTION(BlueprintPure, Category = "Category")
		FString GetCategoryPath() { return CategoryPath; };
};