// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AutomationTest.h"
#include "Public/Models/UserOnlineAccountJustice.h"

const int32 AutomationPermissionFlagMask = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter); 
// Context: EditorContext (run via editor cli) ClientContext (run via external cli)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnQueryCW, "JusticeTest.Permission.WildcardMatch.OnQuery.ConsecutiveWildcard", AutomationPermissionFlagMask);
bool FWildcardMatchOnQueryCW::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:USER:accelpower:ORDER:Rec0x02"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:justice:USER:*:ORDER:*:action:15");
	FString Result;
	bool bExpectedResult = true;
	
	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult? TEXT("Allow"):TEXT("Deny")), *Query, *Result);

	bool bAssertionResult =(bExpectedResult == bQueryResult);
	return bAssertionResult;	
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnQueryPR, "JusticeTest.Permission.WildcardMatch.OnQuery.PreceedingResourceNotMatch", AutomationPermissionFlagMask);
bool FWildcardMatchOnQueryPR::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:USER:accelpower:ORDER:Rec0x02"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:evil:USER:*:ORDER:*:action:*");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnPermissionAQ, "JusticeTest.Permission.WildcardMatch.OnPermission.AnyQuery", AutomationPermissionFlagMask);
bool FWildcardMatchOnPermissionAQ::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:*:FULFILLMENT:*"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:evil:FULFILLMENT:GrantItem:action:7");
	FString Result;
	bool bExpectedResult = true;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnPermissionMR, "JusticeTest.Permission.WildcardMatch.OnPermission.MissmatchResult", AutomationPermissionFlagMask);
bool FWildcardMatchOnPermissionMR::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:*:FULFILLMENT:*"), TEXT("7"));
	FString Query = TEXT("NAMESPACE:justice:DUTY:*:action:7");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnActionCA, "JusticeTest.Permission.WildcardMatch.OnAction.CompleteAction", AutomationPermissionFlagMask);
bool FWildcardMatchOnActionCA::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:ACHIEVEMENT:*"), TEXT("7"));
	FString Query = TEXT("NAMESPACE:evil:ACHIEVEMENT:Daily:action:7");
	FString Result;
	bool bExpectedResult = true;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnActionMA, "JusticeTest.Permission.WildcardMatch.OnAction.MissingAction", AutomationPermissionFlagMask);
bool FWildcardMatchOnActionMA::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:ACHIEVEMENT:*"), TEXT("7"));
	FString Query = TEXT("NAMESPACE:evil:ACHIEVEMENT:*");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;	
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLengthMatchPM, "JusticeTest.Permission.LengthMatch.PerfectMatch", AutomationPermissionFlagMask);
bool FLengthMatchPM::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:USER:accelpower:LEVEL:Pet"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:evil:USER:accelpower:LEVEL:Pet:action:15");
	FString Result;
	bool bExpectedResult = true;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;	
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLengthMatchMM, "JusticeTest.Permission.LengthMatch.MissMatch", AutomationPermissionFlagMask);
bool FLengthMatchMM::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:USER:accelpower:LEVEL:Pet"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:evil:LEVEL:Pet:action:15");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;	
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBitmaskMatchIA, "JusticeTest.Permission.BitmaskMatch.InsufficientAction", AutomationPermissionFlagMask);
bool FBitmaskMatchIA::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:FILEUPLOAD"), TEXT("10"));
	FString Query = TEXT("NAMESPACE:justice:FILEUPLOAD:action:1");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;	
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBitmaskMatchSA, "JusticeTest.Permission.BitmaskMatch.SufficientAction", AutomationPermissionFlagMask);
bool FBitmaskMatchSA::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:FILEUPLOAD"), TEXT("10"));
	FString Query = TEXT("NAMESPACE:justice:FILEUPLOAD:action:8");
	FString Result;
	bool bExpectedResult = true;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;	
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRoleMatchRE, "JusticeTest.Permission.RoleMatch.RoleExist", AutomationPermissionFlagMask);
bool FRoleMatchRE::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:SUPPORT_MINIMUM"), TEXT("1"));
	FString Query = TEXT("NAMESPACE:justice:ROLEID:SUPPORT_MINIMUM");
	FString Result;
	bool bExpectedResult = true;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;	
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRoleMatchNR, "JusticeTest.Permission.RoleMatch.NoRole", AutomationPermissionFlagMask);
bool FRoleMatchNR::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:SUPPORT_MINIMUM"), TEXT("1"));
	FString Query = TEXT("NAMESPACE:justice:ROLEID:SUPPORT_SUPER_ADMIN");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;	
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTrailMatchAS, "JusticeTest.Permission.TrailMatch.AcceptString", AutomationPermissionFlagMask);
bool FTrailMatchAS::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:GODMODE_HEALTH_ONLY"), TEXT("1"));
	FString Query = TEXT("NAMESPACE:justice:ROLEID:GODMODE*");
	FString Result;
	bool bExpectedResult = true;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;	
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTrailMatchRS, "JusticeTest.Permission.TrailMatch.RejectString", AutomationPermissionFlagMask);
bool FTrailMatchRS::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:GODMODE_HEALTH_ONLY"), TEXT("1"));
	FString Query = TEXT("NAMESPACE:justice:ROLEID:GODMODE");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Verbose, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;	
}