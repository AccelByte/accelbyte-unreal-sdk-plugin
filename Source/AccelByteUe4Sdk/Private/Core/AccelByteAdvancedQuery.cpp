// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteAdvancedQuery.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{
	TSharedRef<FAccelByteAdvancedQuery> FAccelByteAdvancedQuery::Start(const FString& Option)
	{
		return MakeShareable(new FAccelByteAdvancedQuery(Option));
	}

	bool FAccelByteAdvancedQuery::Or(const FString& Option)
	{
		if (Options.IsEmpty())
		{
			UE_LOG(LogAccelByte, Warning, TEXT("Initial value is empty and OR function will not fulfilled. Please initiate with Start function"));
			return false;
		}
		else if (!ValidateStringValue(Option))
		{
			return false;
		}
		else
		{
			Options.Enqueue(Option);
			Operators.Enqueue(OR_OPERATOR);
		}
		return true;
	}

	bool FAccelByteAdvancedQuery::And(const FString& Option)
	{
		if (Options.IsEmpty())
		{
			UE_LOG(LogAccelByte, Warning, TEXT("Initial value is empty and AND function will not fulfilled. Please initiate with Start function"));
			return false;
		}
		else if (!ValidateStringValue(Option))
		{
			return false;
		}
		else
		{
			Options.Enqueue(Option);
			Operators.Enqueue(AND_OPERATOR);
		}

		return true;
	}

	FString FAccelByteAdvancedQuery::Build()
	{
		FString Result;
		FString StartValue;

		if (Options.IsEmpty())
		{
			UE_LOG(LogAccelByte, Warning, TEXT("Can not build empty options"));
			return Result;
		}
		else
		{
			Options.Dequeue(StartValue);

			Result.Append(StringifyTagObject(StartValue));

			while (!Options.IsEmpty())
			{
				FString Operand;
				FString Tag;

				Operators.Dequeue(Operand);
				Options.Dequeue(Tag);

				Tag = StringifyTagObject(Tag);

				Result.Append(Operand);
				Result.Append(Tag);
			}

			Options.Empty();
			Operators.Empty();

			return Result;
		}
	}

	FString FAccelByteAdvancedQuery::StringifyTagObject(const FString& Option)
	{
		FString Result;

		if (Option.Contains("&") || Option.Contains("|"))
		{
			Result.Append(OPEN_PARENTHESES);
			Result.Append(Option);
			Result.Append(CLOSE_PARENTHESES);

			return Result;
		}

		return Option;
	}

	bool FAccelByteAdvancedQuery::ValidateStringValue(const FString& Option)
	{
		if (Option.IsEmpty())
		{
			UE_LOG(LogAccelByte, Warning, TEXT("Input data is empty"));
			return false;
		}

		return true;
	}

	FAccelByteAdvancedQuery::FAccelByteAdvancedQuery(const FString& Option)
	{
		if (ValidateStringValue(Option))
		{
			Options.Enqueue(Option);
		}
	}
}