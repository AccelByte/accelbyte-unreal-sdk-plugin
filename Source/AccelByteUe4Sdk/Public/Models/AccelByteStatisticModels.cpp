// Copyright (c) 2025 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteStatisticModels.h"
#include "Core/AccelByteUtilities.h"
#include "Api/AccelByteStatisticApi.h"

TMultiMap<FString, FString> FAccelByteModelsStatGetListStatCycleQueryRequest::GenerateQueryParam()
{
	TMultiMap<FString, FString> QueryParams = {};

	if (CycleType.IsSet())
	{
		FString EnumText = FAccelByteUtilities::GetUEnumValueAsString(CycleType.GetValue());
		QueryParams.Add(TEXT("cycleType"), EnumText.ToUpper());
	}

	if (Name.IsSet() &&
		!Name.GetValue().IsEmpty())
	{
		QueryParams.Add(TEXT("name"), Name.GetValue());
	}

	if (Status.IsSet() &&
		Status.GetValue() != EAccelByteStatisticStatCycleStatus::NONE)
	{
		FString EnumText = FAccelByteUtilities::GetUEnumValueAsString(Status.GetValue());
		QueryParams.Add(TEXT("status"), EnumText.ToUpper());
	}

	if (SortBy.Num() != 0)
	{
		FString CombinedSortBy{};
		for (const auto& Entry : SortBy)
		{
			auto ConvertedSorting = AccelByte::Api::Statistic::ConvertStatCycleStatisticSortByToString(Entry);
			CombinedSortBy += ConvertedSorting;
			//Separator defined from backend
			CombinedSortBy += ",";
		}
		
		//Remove last comma at end of the index
		if (!CombinedSortBy.IsEmpty())
		{
			CombinedSortBy.RemoveAt(CombinedSortBy.Len() - 1);
		}

		// Need to be encoded due to colon & comma separator from the conversion result.
		auto EncodedSorting = FGenericPlatformHttp::UrlEncode(CombinedSortBy);
		
		QueryParams.Add(TEXT("sortBy"), EncodedSorting);
	}

	return QueryParams;
}