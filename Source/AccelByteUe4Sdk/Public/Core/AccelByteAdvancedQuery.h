// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

namespace AccelByte
{
	class ACCELBYTEUE4SDK_API FAccelByteAdvancedQuery
	{
	public:

		static TSharedRef<FAccelByteAdvancedQuery> Start(const FString& Option);
		bool Or(const FString& Option);
		bool And(const FString& Option);
		FString Build();

	private:

		const FString OR_OPERATOR = "|";
		const FString AND_OPERATOR = "&";
		const FString OPEN_PARENTHESES = "(";
		const FString CLOSE_PARENTHESES = ")";

		TQueue<FString> Options;
		TQueue<FString> Operators;

		FString StringifyTagObject(const FString& Option);
		static bool ValidateStringValue(const FString& Option);

		FAccelByteAdvancedQuery(const FString& Option);
		FAccelByteAdvancedQuery() = delete;
	};
}