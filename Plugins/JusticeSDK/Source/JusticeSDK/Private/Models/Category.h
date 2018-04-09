// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "Category.generated.h"

class Category : public FJsonSerializable
{
	FString ID;
	FString Namespace;
	FString ParentCategoryPath;
	FString DisplayName;
	FString ChildCategories;
	FString CreatedAt;
	FString UpdatedAt;
	bool Root;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("id", ID);
		JSON_SERIALIZE("namespace", Namespace);
		JSON_SERIALIZE("parentCategoryPath", ParentCategoryPath);
		JSON_SERIALIZE("displayName", DisplayName);
		JSON_SERIALIZE("childCategories", ChildCategories);
		JSON_SERIALIZE("createdAt", CreatedAt);
		JSON_SERIALIZE("updatedAt", UpdatedAt);
		JSON_SERIALIZE("root", Root);
	END_JSON_SERIALIZER
};



UCLASS()
class UCategory : public UObject, public Category
{
	GENERATED_BODY()
	
public:

		
};
