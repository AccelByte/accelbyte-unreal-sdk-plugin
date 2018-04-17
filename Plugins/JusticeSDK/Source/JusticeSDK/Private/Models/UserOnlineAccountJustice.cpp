// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "UserOnlineAccountJustice.h"

bool FUserOnlineAccountJustice::GetUserAttribute(const FString & AttrName, FString & OutAttrValue) const
{
	//instant return attribute value for other usage
	TArray<FString> FullQuery;
	const FString* FoundAttr;

	AttrName.ParseIntoArray(FullQuery, TEXT(":"), true);
	if (FullQuery.Num() < 2)
	{
		FoundAttr = UserAttributes.Find(AttrName);
		if (FoundAttr != NULL)
		{
			OutAttrValue = *FoundAttr;
			return true;
		}
	}

	//check the action bit and make clean copy of query
	int32 CheckedActionBit = -1;
	TArray<FString> Query;
	TArray<FString> KeyArray;
	if (FullQuery[FullQuery.Num() - 2] == TEXT("action"))
	{
		CheckedActionBit = FCString::Atoi(*FullQuery.Last());
		for (int32 i = 0; i < FullQuery.Num() - 2; i++)
		{
			Query.Add(FullQuery[i]);
		}
	}
	else
	{
		Query.Append(FullQuery);
	}

	//permission query without action specified will be denied instantly
	if (!AttrName.Contains(TEXT("ROLE"), ESearchCase::CaseSensitive, ESearchDir::FromStart) && CheckedActionBit == -1)
	{
		OutAttrValue = TEXT("Action must be specified for permission query");
		return false;
	}

	//find perms and action bit to compare
	FString CheckedTrails;
	UserAttributes.GetKeys(KeyArray);
	bool bMatchFound = false;
	for (FString Key : KeyArray)
	{
		//check for match possibility by the element number
		TArray<FString> Permission;
		Key.ParseIntoArray(Permission, TEXT(":"), true);
		if (Permission.Num() != Query.Num())
		{
			continue;
		}

		//check the element of each query and permission position
		bMatchFound = true;
		for (int i = 0; i < Permission.Num(); i++)
		{
			//check if any match or wild card
			if (Permission[i].Equals(Query[i], ESearchCase::CaseSensitive)
				|| Permission[i].Equals(*FString(TEXT("*")), ESearchCase::CaseSensitive)
				|| Query[i].Equals(*FString(TEXT("*")), ESearchCase::CaseSensitive))
			{
				continue;
			}

			//if nothing match check the string trails
			CheckedTrails = Query[i];
			if (!(
				(CheckedTrails.Len() > 1)
				&& CheckedTrails.RemoveFromEnd(TEXT("*"), ESearchCase::CaseSensitive)
				&& Permission[i].Contains(CheckedTrails, ESearchCase::CaseSensitive, ESearchDir::FromStart)
				))
			{
				bMatchFound = false;
				break;
			}
		}

		//check if bMatchFond has been invalidated
		if (bMatchFound)
		{
			FoundAttr = UserAttributes.Find(Key);
			if (FoundAttr != NULL)
			{
				OutAttrValue = *FoundAttr;
				int Verify = (CheckedActionBit & ~FCString::Atoi(**UserAttributes.Find(Key)));
				if (CheckedActionBit == -1 || Verify == 0)
				{
					OutAttrValue = Key + ":action:" + *FoundAttr;
					return true;
				}
				OutAttrValue = "Match found for " + Key + " but action permitted is " + *UserAttributes.Find(Key);
				return false;
			}
		}
	}
	OutAttrValue = TEXT("No match found");
	return false;
}

bool FUserOnlineAccountJustice::SetUserAttribute(const FString & AttrName, const FString & AttrValue)
{
	const FString* FoundAttr = UserAttributes.Find(AttrName);
	if (FoundAttr == NULL || *FoundAttr != AttrValue)
	{
		UserAttributes.Add(AttrName, AttrValue);
		return true;
	}
	return false;
}

bool FUserOnlineAccountJustice::GetAuthAttribute(const FString & AttrName, FString & OutAttrValue) const
{
	const FString* FoundAttr = AdditionalAuthData.Find(AttrName);
	if (FoundAttr != NULL)
	{
		OutAttrValue = *FoundAttr;
		return true;
	}
	return false;
}