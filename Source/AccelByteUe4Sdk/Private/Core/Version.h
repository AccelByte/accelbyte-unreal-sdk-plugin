// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Version.generated.h"

USTRUCT(BlueprintType)
struct FVersionInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AccelByte | VersionInfo")
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AccelByte | VersionInfo")
	FString Version;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AccelByte | VersionInfo")
	FString GitHash;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "AccelByte | VersionInfo")
	FString Realm;
};


struct FVersion
{
	int32 Major{0};
	int32 Minor{0};
	int32 Patch{0};
	TArray<FString> PreReleaseSegments;

	FVersion(int32 Major, int32 Minor, int32 Patch, TArray<FString> PreReleaseSegments = TArray<FString>{});
	FVersion(FString Version);
	bool IsValid() const;
	int32 Compare(FVersion const& Other) const;
	FString ToString() const;
};


struct FVersionRange
{
	FVersion Min;
	FVersion Max;
};


struct FResult
{
	bool const bIsError{ false };
	FString const Message;
};


class FServiceCompatibilityMap
{
public:
	static FServiceCompatibilityMap FromJson(FString JsonString);

	explicit FServiceCompatibilityMap(TMap<FString, FVersionRange> Map);
	FResult Check(FString const& ServiceName, FString const& VersionStr) const;
	TArray<FString> GetServices() const;

private:
	static bool IsCompatible(FVersionRange const& Range, FVersion const& Version);
	TMap<FString, FVersionRange> const Map;
};
