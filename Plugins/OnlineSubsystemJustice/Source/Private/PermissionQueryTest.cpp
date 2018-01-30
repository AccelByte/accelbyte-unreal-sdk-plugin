// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AutomationTest.h"
#include "OnlineIdentityJustice.h"

const int AutomationFlagMask = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter); 
// Context: EditorContext (run via editor cli) ClientContext (run via external cli)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnQueryCW, "PermissionTest.WildcardMatch.OnQuery.ConsecutiveWildcard", AutomationFlagMask);
bool FWildcardMatchOnQueryCW::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:USER:accelpower:ORDER:Rec0x02"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:justice:USER:*:ORDER:*:action:15");
	FString Result;
	bool bExpectedResult = true;
	
	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult? TEXT("Allow"):TEXT("Deny")), *Query, *Result);

	bool bAssertionResult =(bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnQueryPR, "PermissionTest.WildcardMatch.OnQuery.PreceedingResourceNotMatch", AutomationFlagMask);
bool FWildcardMatchOnQueryPR::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:USER:accelpower:ORDER:Rec0x02"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:evil:USER:*:ORDER:*:action:*");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnPermissionAQ, "PermissionTest.WildcardMatch.OnPermission.AnyQuery", AutomationFlagMask);
bool FWildcardMatchOnPermissionAQ::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:*:FULFILLMENT:*"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:evil:FULFILLMENT:GrantItem:action:7");
	FString Result;
	bool bExpectedResult = true;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnPermissionMR, "PermissionTest.WildcardMatch.OnPermission.MissmatchResult", AutomationFlagMask);
bool FWildcardMatchOnPermissionMR::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:*:FULFILLMENT:*"), TEXT("7"));
	FString Query = TEXT("NAMESPACE:justice:DUTY:*:action:7");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnActionCA, "PermissionTest.WildcardMatch.OnAction.CompleteAction", AutomationFlagMask);
bool FWildcardMatchOnActionCA::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:ACHIEVEMENT:*"), TEXT("7"));
	FString Query = TEXT("NAMESPACE:evil:ACHIEVEMENT:Daily:action:7");
	FString Result;
	bool bExpectedResult = true;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnActionMA, "PermissionTest.WildcardMatch.OnAction.MissingAction", AutomationFlagMask);
bool FWildcardMatchOnActionMA::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:ACHIEVEMENT:*"), TEXT("7"));
	FString Query = TEXT("NAMESPACE:evil:ACHIEVEMENT:*");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLengthMatchPM, "PermissionTest.LengthMatch.PerfectMatch", AutomationFlagMask);
bool FLengthMatchPM::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:USER:accelpower:LEVEL:Pet"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:evil:USER:accelpower:LEVEL:Pet:action:15");
	FString Result;
	bool bExpectedResult = true;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLengthMatchMM, "PermissionTest.LengthMatch.MissMatch", AutomationFlagMask);
bool FLengthMatchMM::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:USER:accelpower:LEVEL:Pet"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:evil:LEVEL:Pet:action:15");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBitmaskMatchIA, "PermissionTest.BitmaskMatch.InsufficientAction", AutomationFlagMask);
bool FBitmaskMatchIA::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:FILEUPLOAD"), TEXT("10"));
	FString Query = TEXT("NAMESPACE:justice:FILEUPLOAD:action:1");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBitmaskMatchSA, "PermissionTest.BitmaskMatch.SufficientAction", AutomationFlagMask);
bool FBitmaskMatchSA::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:FILEUPLOAD"), TEXT("10"));
	FString Query = TEXT("NAMESPACE:justice:FILEUPLOAD:action:8");
	FString Result;
	bool bExpectedResult = true;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRoleMatchRE, "PermissionTest.RoleMatch.RoleExist", AutomationFlagMask);
bool FRoleMatchRE::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:SUPPORT_MINIMUM"), TEXT("1"));
	FString Query = TEXT("NAMESPACE:justice:ROLEID:SUPPORT_MINIMUM");
	FString Result;
	bool bExpectedResult = true;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRoleMatchNR, "PermissionTest.RoleMatch.NoRole", AutomationFlagMask);
bool FRoleMatchNR::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:SUPPORT_MINIMUM"), TEXT("1"));
	FString Query = TEXT("NAMESPACE:justice:ROLEID:SUPPORT_SUPER_ADMIN");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTrailMatchAS, "PermissionTest.TrailMatch.AcceptString", AutomationFlagMask);
bool FTrailMatchAS::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:GODMODE_HEALTH_ONLY"), TEXT("1"));
	FString Query = TEXT("NAMESPACE:justice:ROLEID:GODMODE*");
	FString Result;
	bool bExpectedResult = true;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTrailMatchRS, "PermissionTest.TrailMatch.RejectString", AutomationFlagMask);
bool FTrailMatchRS::RunTest(const FString & Parameters)
{
	FUserOnlineAccountJustice* AccountMock = new FUserOnlineAccountJustice("test@accelbyte.com");
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:GODMODE_HEALTH_ONLY"), TEXT("1"));
	FString Query = TEXT("NAMESPACE:justice:ROLEID:GODMODE");
	FString Result;
	bool bExpectedResult = false;

	bool bQueryResult = AccountMock->GetUserAttribute(Query, Result);
	UE_LOG(LogTemp, Warning, TEXT("%s permission for %s, %s"), (bQueryResult ? TEXT("Allow") : TEXT("Deny")), *Query, *Result);

	bool bAssertionResult = (bExpectedResult == bQueryResult);
	return bAssertionResult;
}