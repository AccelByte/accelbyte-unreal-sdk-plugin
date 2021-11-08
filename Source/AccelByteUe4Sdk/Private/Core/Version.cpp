// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Version.h"

#include "CoreMinimal.h"
#include "Misc/DefaultValueHelper.h"
#include "JsonObjectConverter.h"
#include "Core/AccelByteError.h"

bool TryParseNumeric(FString const& InString, int32& OutNumeric)
{
	int32 LeadingZeros = 0;

	for (TCHAR const C : InString)
	{
		if (C != '0')
		{
			break;
		}

		LeadingZeros++;
	}

	OutNumeric = FCString::Strtoi(*InString, nullptr, 10);

	return (OutNumeric != 0 && LeadingZeros == 0) || InString == "0";
}

bool ValidateAlphaNumeric(FString const& String)
{
	bool const bHasAlpha = String.FindLastCharByPredicate([](TCHAR const C) { return TChar<TCHAR>::IsAlpha(C); }) >= 0;
	bool const bIsAlNum = String.FindLastCharByPredicate([](TCHAR const C) { return !TChar<TCHAR>::IsAlnum(C); }) < 0;

	return bHasAlpha && bIsAlNum;
}

bool TryParseDotSplitSegments(FString const& InIdentifier, TArray<FString>& OutSegments)
{
	if (InIdentifier.IsEmpty())
	{
		return false;
	}

	TArray<FString> Segments;
	InIdentifier.ParseIntoArray(Segments, TEXT("."), false);

	for (auto Segment : Segments)
	{
		if (Segment.IsEmpty())
		{
			return false;
		}

		int32 NumericSegment = 0;
		if (!ValidateAlphaNumeric(Segment) && !TryParseNumeric(Segment, NumericSegment))
		{
			return false;
		}
	}

	OutSegments = Segments;

	return true;
}

bool TryParseBuild(FString const& InVersion, FString& OutVersion)
{
	int32 BuildStart;

	if (!InVersion.FindChar('+', BuildStart))
	{
		return true;
	}

	FString const Build = InVersion.Right(InVersion.Len() - BuildStart - 1);
	TArray<FString> BuildSegments;

	if (!TryParseDotSplitSegments(Build, BuildSegments))
	{
		return false;
	}

	OutVersion = InVersion.Left(BuildStart);

	return true;
}

bool TryParsePreRelease(FString const& InVersion, TArray<FString>& OutPreReleaseSegments, FString& OutVersion)
{
	int32 BuildStart;

	if (!InVersion.FindChar('-', BuildStart))
	{
		return true;
	}

	FString const PreRelease = InVersion.Right(InVersion.Len() - BuildStart - 1);

	if (!TryParseDotSplitSegments(PreRelease, OutPreReleaseSegments))
	{
		return false;
	}

	OutVersion = InVersion.Left(BuildStart);

	return true;
}

bool TryParseVersionCore(FString const& InVersion, FVersion& OutVersionCore)
{
	TArray<FString> VersionCoreSegments;
	InVersion.ParseIntoArray(VersionCoreSegments, TEXT("."));

	if (VersionCoreSegments.Num() != 3)
	{
		return false;
	}

	if (!TryParseNumeric(VersionCoreSegments[0], OutVersionCore.Major) ||
		!TryParseNumeric(VersionCoreSegments[1], OutVersionCore.Minor) ||
		!TryParseNumeric(VersionCoreSegments[2], OutVersionCore.Patch))
	{
		return false;
	}

	return true;
}

int32 CompareVersionNumber(int32 Left, int32 Right)
{
	int32 CompareResult = 0;
	
	if (Left < Right)
	{
		CompareResult = -1;
	} 
	else if (Left > Right)
	{
		CompareResult = 1;
	}
	
	return CompareResult;
}

FVersion::FVersion(int32 const Major, int32 const Minor, int32 const Patch, TArray<FString> PreReleaseSegments) :
	Major{Major},
	Minor{Minor},
	Patch{Patch},
	PreReleaseSegments{PreReleaseSegments} {}

FVersion::FVersion(FString Version)
{
	if (!TryParseBuild(Version, Version))
	{
		return;
	}

	if (!TryParsePreRelease(Version, PreReleaseSegments, Version))
	{
		return;
	}

	if (!TryParseVersionCore(Version, *this))
	{
		Major = 0;
		Minor = 0;
		Patch = 0;
	}
}

bool FVersion::IsValid() const
{
	return Major != 0 || Minor != 0 || Patch != 0;
}

int32 ComparePreRelease(TArray<FString> const& A, TArray<FString> const& B)
{
	if (A.Num() == 0 && B.Num() == 0)
	{
		return 0;
	}

	if (A.Num() == 0 && B.Num() > 0)
	{
		return 1;
	}

	if (A.Num() > 0 && B.Num() == 0)
	{
		return -1;
	}

	for (int i = 0; i < FPlatformMath::Min(A.Num(), B.Num()); i++)
	{
		int32 NumA;
		int32 NumB;
		bool const bIsNumericA = TryParseNumeric(A[i], NumA);
		bool const bIsNumericB = TryParseNumeric(B[i], NumB);

		if (bIsNumericA && !bIsNumericB)
		{
			return -1;
		}

		if (!bIsNumericA && bIsNumericB)
		{
			return 1;
		}

		if (bIsNumericA && bIsNumericB)
		{
			int32 const CompareResult = CompareVersionNumber(NumA, NumB);
			if (CompareResult != 0)
			{
				return CompareResult;
			}
		}

		if (!bIsNumericA && !bIsNumericB)
		{
			int32 const CompareResult = A[i].Compare(B[i]);

			if (CompareResult != 0)
			{
				return CompareResult;
			}
		}
	}

	if (A.Num() > B.Num())
	{
		return 1;
	}

	if (A.Num() < B.Num())
	{
		return -1;
	}

	return 0;
}

int32 FVersion::Compare(FVersion const& Other, bool bPatchIgnored) const
{
	int32 CompareResult = 0;

	CompareResult = CompareVersionNumber(Major, Other.Major);
	
	if (CompareResult != 0)
	{
		return CompareResult;
	}

	CompareResult = CompareVersionNumber(Minor, Other.Minor);
	
	if (CompareResult != 0)
	{
		return CompareResult;
	}
	
	if (!bPatchIgnored)
	{
		CompareResult = CompareVersionNumber(Patch, Other.Patch);

		if (CompareResult != 0)
		{
			return CompareResult;
		}
	}

	return ComparePreRelease(PreReleaseSegments, Other.PreReleaseSegments);
}

FString FVersion::ToString() const
{
	return FString::Printf(TEXT("%u.%u.%u"), Major, Minor, Patch);
}

FServiceCompatibilityMap FServiceCompatibilityMap::FromJson(FString const JsonString)
{
	TSharedPtr<FJsonObject> JsonObjectPtr = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> const JsonReader = TJsonReaderFactory<>::Create(JsonString);
	TMap<FString, FVersionRange> Map;

	if (!FJsonSerializer::Deserialize(JsonReader, JsonObjectPtr) || !JsonObjectPtr.IsValid())
	{
		return FServiceCompatibilityMap{MoveTemp(Map)};
	}

	for (auto const& Service : JsonObjectPtr->Values)
	{
		TSharedPtr<FJsonObject> const* VersionRangePtrPtr = nullptr;
		if (!Service.Value->TryGetObject(VersionRangePtrPtr))
		{
			continue;
		}

		FString StrMinVersion;
		FString StrMaxVersion;
		if (!(*VersionRangePtrPtr)->TryGetStringField("minVersion", StrMinVersion) ||
			!(*VersionRangePtrPtr)->TryGetStringField("maxVersion", StrMaxVersion))
		{
			continue;
		}

		FVersion MinVersion{StrMinVersion};
		FVersion MaxVersion{StrMaxVersion};
		if (!MinVersion.IsValid() || !MaxVersion.IsValid())
		{
			continue;
		}

		Map.Emplace(Service.Key, FVersionRange{MinVersion, MaxVersion});
	}

	return FServiceCompatibilityMap{MoveTemp(Map)};
}

FServiceCompatibilityMap::FServiceCompatibilityMap(TMap<FString, FVersionRange> Map)
	:
	Map{MoveTemp(Map)} {}

FResult FServiceCompatibilityMap::Check(FString const& ServiceName, FString const& VersionStr, bool bPatchIgnored) const
{
	if (ServiceName.IsEmpty())
	{
		return FResult{true, "Service name empty"};
	}

	if (VersionStr.IsEmpty())
	{
		return FResult{true, FString::Printf(TEXT("Version string empty: %s"), *ServiceName)};
	}

	FVersion const Version{VersionStr};
	FVersionRange const* const RangePtr = Map.Find(ServiceName);

	if (!RangePtr)
	{
		return FResult{true, FString::Printf(TEXT("Version name %s not found"), *ServiceName)};
	}

	if(!FServiceCompatibilityMap::IsCompatible(*RangePtr, Version, bPatchIgnored))
	{
		return FResult
		{
			true,
			FString::Printf(
				TEXT("Version mismatch: Expected \"%s\" version \"%s\"-\"%s\", found \"%s\""),
				*ServiceName,
				*RangePtr->Min.ToString(),
				*RangePtr->Max.ToString(),
				*Version.ToString())
		};
	}

	return FResult{};
}

TArray<FString> FServiceCompatibilityMap::GetServices() const
{
	TArray<FString> OutKeys;
	Map.GetKeys(OutKeys);

	return OutKeys;
}

bool FServiceCompatibilityMap::IsCompatible(FVersionRange const& Range, FVersion const& Version, bool bPatchIgnored)
{
	bool Compatible = true;
	int32 MinVersionCompare = Range.Min.Compare(Version, bPatchIgnored);
	Compatible &= (MinVersionCompare <= 0);

	int32 MaxVersionCompare = Range.Max.Compare(Version, bPatchIgnored);
	Compatible &= (MaxVersionCompare >= 0);

	return Compatible;
}