// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteMessageParser.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{
bool MessageParser::MoveToEndQuote(const TCHAR*& Cursor)
{
	check(*Cursor == '"');
	bool bIsEscape = false;
	++Cursor;
	while (*Cursor)
	{
		if (bIsEscape)
		{
			bIsEscape = false;
		}
		else if (*Cursor == '\\')
		{
			bIsEscape = true;
		}
		else if (*Cursor == '"')
		{
			return true;
		}
		++Cursor;
	}

	return false;
}

bool MessageParser::MoveToEndObject(const TCHAR*& Cursor)
{
	check(*Cursor == '{');
	const TCHAR* Current = Cursor;
	int Braces = 1;
	++Current;
	while (*Current)
	{
		if (*Current == '"')
		{
			if (!MoveToEndQuote(Current))
			{
				UE_LOG(LogAccelByte, Verbose, TEXT("[JsonObject] Unclosed quote"));
				break;
			}
		}
		// inner object
		else if (*Current == '{')
		{
			++Braces;
		}
		// end of current object
		else if (*Current == '}')
		{
			--Braces;
			if (Braces == 0)
			{
				break;
			}
		}

		++Current;
	}

	// update cursor position 
	Cursor = Current;
	return Braces == 0;
}

bool EscapeStr(const TCHAR*& Cursor, FString& OutJsonString)
{
	bool bIsEscape = false;
	OutJsonString.Append("\"");
	while (*Cursor)
	{
		if (*Cursor == '\\' || *Cursor == '"')
		{
			OutJsonString.Append("\\");
		}
		OutJsonString.AppendChar(*Cursor);
		++Cursor;
	}
	OutJsonString.Append("\"");
	return true;
}

FString MessageParser::EscapeString(const FString& InString, bool bIsEnclosedWithQuote)
{
	const TCHAR* Cursor = GetData(InString);

	// We will check if the string DOES NOT CONTAIN quote && backslash
	// If so, we don't need to escape it.
	if (!Cursor || 
		(!InString.Contains("\"") && !InString.Contains("\\")))
	{
		// Checking the value is an OBJECT or ARRAY
		// We only need to enclose it with quote.
		if (bIsEnclosedWithQuote ||
			InString.StartsWith("{") ||
			InString.StartsWith("[")
			)
		{
			return FString::Printf(TEXT("\"%s\""), *InString);
		}
		return InString;
	}

	FString Escaped;
	EscapeStr(Cursor, Escaped);
	return Escaped;
}

bool MessageParser::ParseString(const TCHAR*& Cursor, FString& OutJsonString)
{
	bool bIsEscape = false;
	if (*Cursor == '"') ++Cursor;
	const TCHAR* Begin = Cursor;
	OutJsonString.Append("\"");
	while (*Cursor)
	{
		if (bIsEscape)
		{
			bIsEscape = false;
		}
		else if (*Cursor == '\\')
		{
			bIsEscape = true;
		}
		else if (*Cursor == '"')
		{
			OutJsonString.Append(Begin, Cursor - Begin);
			if (Cursor[1] != '\0')
			{
				// escape quote in the middle of string
				OutJsonString.Append("\\\"");
			}
			Begin = Cursor + 1;
		}
		++Cursor;
	}
	OutJsonString.Append(Begin, Cursor - Begin);
	OutJsonString.Append("\"");
	return true;
}

bool MessageParser::ParseObject(const TCHAR*& Cursor, FString& OutJsonString)
{
	check(*Cursor == '{');
	const TCHAR* Current = Cursor;
	const TCHAR* Begin = Cursor;
	++Current;
	while (*Current)
	{
		if (*Current == '"')
		{
			if (!MoveToEndQuote(Current))
			{
				UE_LOG(LogAccelByte, Verbose, TEXT("[JsonObject] Unclosed quote"));
				break;
			}
		}
		// inner object
		else if (*Current == '{')
		{
			// append name before appending object value
			OutJsonString.Append(Begin, Current - Begin);
			// parse and append the object value
			if (!ParseObject(Current, OutJsonString))
			{
				// unclosed inner object
				break;
			}
			// move cursor after inner object, so we only need to append the close bracket
			Begin = Current + 1;
		}
		// end of current object
		else if (*Current == '}')
		{
			const TCHAR* End = Current - 1;
			// move backward to remove trailing comma before end of object
			while (End >= Begin && (*End == ',' || *End == ' ')) --End;
			// append close bracket for outer object and whole value for most inner object
			OutJsonString.Append(Begin, End - Begin + 1);
			OutJsonString.Append("}");
			break;
		}

		++Current;
	}

	// update cursor position 
	Cursor = Current;
	if (*Current == '}')
	{
		return true;
	}

	UE_LOG(LogAccelByte, Verbose, TEXT("[JsonObject] Unclosed object"));
	return false;
}

bool MessageParser::ParseArrayOfObject(const TCHAR*& Cursor, FString& OutJsonString)
{
	// cursor need to on the position after '['
	check(*Cursor == '{');
	FString JsonObjectString;
	const TCHAR* Current = Cursor;
	bool bIsValid = true;
	bool bFirst = true;

	OutJsonString.Append("[");
	auto AppendCurrentItem = [&JsonObjectString, &OutJsonString, &bFirst](bool bIgnoreEmpty = false)
	{
		JsonObjectString.TrimStartAndEndInline();

		if (bIgnoreEmpty && JsonObjectString.IsEmpty()) return;

		if (JsonObjectString.IsEmpty())
		{
			UE_LOG(LogAccelByte, Verbose, TEXT("[JsonArrayObject] Empty array item, set to default"));
			JsonObjectString = TEXT("{}");
		}

		if (bFirst)
		{
			bFirst = false;
		}
		else
		{
			OutJsonString.Append(",");
		}

		OutJsonString.Append(JsonObjectString);
		JsonObjectString = TEXT("");
	};

	while (*Current)
	{
		if (*Current == '{')
		{
			if (!ParseObject(Current, JsonObjectString))
			{
				bIsValid = false;
				// don't append invalid object
				JsonObjectString = TEXT("");
				break;
			}
		}
		// comma outside array item
		else if (*Current == ',')
		{
			AppendCurrentItem();
		}
		else if (*Current == ']')
		{
			break;
		}

		++Current;
	}

	// update cursor position 
	Cursor = Current;

	if (!JsonObjectString.IsEmpty())
	{
		// ignore empty for trailing comma at the end of array
		AppendCurrentItem(true);
	}

	OutJsonString.Append("]");

	return bIsValid;
}

bool MessageParser::ParseArrayOfString(const TCHAR*& Cursor, FString& OutJsonString)
{
	// cursor need to on the position after '['
	check(*Cursor != '[');
	const TCHAR* Begin;
	const TCHAR* Current = Cursor;
	bool bIsValid = true;
	bool bFirst = true;
	
	OutJsonString.Append("[");
	auto AppendCurrentItem = [&OutJsonString, &Begin, &Current, &bFirst](bool bIgnoreEmpty = false)
	{
		FString Item(Current - Begin, Begin);
		Item.TrimStartAndEndInline();

		if (bIgnoreEmpty && Item.IsEmpty()) return;
		if (bFirst)
		{
			bFirst = false;
		}
		else
		{
			OutJsonString.Append(",");
		}
		
		if (Item.StartsWith("\""))
		{
			const TCHAR* ItemCursor = GetData(Item);
			if (ItemCursor)
			{
				ParseString(ItemCursor, OutJsonString);
			}
			else
			{
				OutJsonString.Appendf(TEXT("\"%s\""), *Item);
			}
		}
		else
		{
			OutJsonString.Appendf(TEXT("\"%s\""), *Item);
		}
	};

	Begin = Current;
	while (*Current)
	{
		// will ignore anything inside quote
		if (*Current == '"')
		{
			if (!MoveToEndQuote(Current))
			{
				UE_LOG(LogAccelByte, Verbose, TEXT("[JsonObject] Unclosed quote"));

				// move to the end of array item to append correctly
				--Current;
				while (*Current == ']' || *Current == ' ' || *Current == ',')  --Current;
				++Current;

				AppendCurrentItem();
				
				Begin = Current + 1;
				break;
			}
		}
		else
		{
			// comma outside array item
			if (*Current == ',')
			{
				// append an item (Begin position to Current position)
				AppendCurrentItem();
				// set the beginning of the next item
				Begin = ++Current;
				continue;
			}
			// end of array
			if (*Current == ']')
			{
				break;
			}
		}
		++Current;
	}

	// update cursor position 
	Cursor = Current;

	if (Current > Begin)
	{
		// ignore empty for trailing comma at the end of array
		AppendCurrentItem(true);
	}

	OutJsonString.Append("]");

	return bIsValid;
}
}
