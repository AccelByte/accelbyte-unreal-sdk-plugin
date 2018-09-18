// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Category.h"
#include "ItemInfo.h"
#include "JusticeCatalogBlueprints.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetChildCategoriesOutputPin, const TArray<UCategory*>&, Categories);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UGetChildCategories : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FGetChildCategoriesOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FGetChildCategoriesOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | Catalog"))
		static UGetChildCategories* GetChildCategories(FString Language, FString CategoryPath);
	virtual void Activate() override;
private:
	FString Language;
	FString CategoryPath;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetDescendantCategoryOutputPin, const TArray<UCategory*>&, Categories);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UGetDescendantCategories : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FGetDescendantCategoryOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FGetDescendantCategoryOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | Catalog"))
		static UGetDescendantCategories* GetDescendantCategories(FString Language, FString CategoryPath);
	virtual void Activate() override;
private:
	FString Language;
	FString CategoryPath;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetItemByIdOutputPin, const UItemInfo*, Item);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UGetItemById : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FGetItemByIdOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FGetItemByIdOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | Catalog"))
		static UGetItemById* GetItemById(FString ItemID, FString Region, FString Language);
	virtual void Activate() override;
private:
	FString ItemID;
	FString Region;
	FString Language;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetItemsByQueryOutputPin, const TArray<UItemInfo*>&, ItemArray);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UGetItemsByQuery : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FGetItemsByQueryOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FGetItemsByQueryOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | Catalog"))
		static UGetItemsByQuery* GetItemsByQuery(FString Language, FString Region, FString CategoryPath, FString ItemType, FString Status, int32 Page, int32 Size);
	virtual void Activate() override;
private:
	FString Language;
	FString Region;
	FString CategoryPath;
	FString ItemType;
	FString Status;
	int32 Page;
	int32 Size;
};
