#include "AutomationTest.h"
#include "AutomationCommon.h"
#include "OnlineIdentityJustice.h"
#define DETAIL_AUTOMATION_TEST_LOG

const int AutomationFlagMask = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter); //Context: EditorContext (run via editor cli) ClientContext (run via external cli)

bool RunQuery(TSharedPtr<FUserOnlineAccount> AccountMock, FString &Query, bool &bExpectedResult)
{
	FString OutQuery;
	bool bResult = AccountMock->GetUserAttribute(Query, OutQuery);

	//Catch detailed result from query processing
#ifdef DETAIL_AUTOMATION_TEST_LOG
	if (bResult)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grant permission for %s, Picking %s"), *Query, *OutQuery);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Deny permission for %s, %s"), *Query, *OutQuery);
	}
#endif // DETAIL_AUTOMATION_TEST_LOG

	return bResult;
}

bool AssertResult(bool &bExpectedResult, bool &bQueryResult)
{
	if (bExpectedResult == bQueryResult)
	{
		return true;
	}
	return false;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnQueryCW, "PermissionTest.WildcardMatch.OnQuery.ConsecutiveWildcard", AutomationFlagMask);
bool FWildcardMatchOnQueryCW::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:USER:accelpower:ORDER:Rec0x02"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:justice:USER:*:ORDER:*:action:15");
	bool bExpectedResult = true;
	
	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnQueryPR, "PermissionTest.WildcardMatch.OnQuery.PreceedingResourceNotMatch", AutomationFlagMask);
bool FWildcardMatchOnQueryPR::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:USER:accelpower:ORDER:Rec0x02"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:evil:USER:*:ORDER:*:action:*");
	bool bExpectedResult = false;

	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnPermissionAQ, "PermissionTest.WildcardMatch.OnPermission.AnyQuery", AutomationFlagMask);
bool FWildcardMatchOnPermissionAQ::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:*:FULFILLMENT:*"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:evil:FULFILLMENT:GrantItem:action:7");
	bool bExpectedResult = true;

	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnPermissionMR, "PermissionTest.WildcardMatch.OnPermission.MissmatchResult", AutomationFlagMask);
bool FWildcardMatchOnPermissionMR::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:*:FULFILLMENT:*"), TEXT("7"));
	FString Query = TEXT("NAMESPACE:justice:DUTY:*:action:7");
	bool bExpectedResult = false;

	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnActionCA, "PermissionTest.WildcardMatch.OnAction.CompleteAction", AutomationFlagMask);
bool FWildcardMatchOnActionCA::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:ACHIEVEMENT:*"), TEXT("7"));
	FString Query = TEXT("NAMESPACE:evil:ACHIEVEMENT:Daily:action:7");
	bool bExpectedResult = true;

	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FWildcardMatchOnActionMA, "PermissionTest.WildcardMatch.OnAction.MissingAction", AutomationFlagMask);
bool FWildcardMatchOnActionMA::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:ACHIEVEMENT:*"), TEXT("7"));
	FString Query = TEXT("NAMESPACE:evil:ACHIEVEMENT:*");
	bool bExpectedResult = false;

	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLengthMatchPM, "PermissionTest.LengthMatch.PerfectMatch", AutomationFlagMask);
bool FLengthMatchPM::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:USER:accelpower:LEVEL:Pet"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:evil:USER:accelpower:LEVEL:Pet:action:15");
	bool bExpectedResult = true;

	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLengthMatchMM, "PermissionTest.LengthMatch.MissMatch", AutomationFlagMask);
bool FLengthMatchMM::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:USER:accelpower:LEVEL:Pet"), TEXT("15"));
	FString Query = TEXT("NAMESPACE:evil:LEVEL:Pet:action:15");
	bool bExpectedResult = false;

	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBitmaskMatchIA, "PermissionTest.BitmaskMatch.InsufficientAction", AutomationFlagMask);
bool FBitmaskMatchIA::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:FILEUPLOAD"), TEXT("10"));
	FString Query = TEXT("NAMESPACE:justice:FILEUPLOAD:action:1");
	bool bExpectedResult = false;

	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBitmaskMatchSA, "PermissionTest.BitmaskMatch.SufficientAction", AutomationFlagMask);
bool FBitmaskMatchSA::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:FILEUPLOAD"), TEXT("10"));
	FString Query = TEXT("NAMESPACE:justice:FILEUPLOAD:action:8");
	bool bExpectedResult = true;

	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRoleMAtchRE, "PermissionTest.RoleMatch.RoleExist", AutomationFlagMask);
bool FRoleMAtchRE::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:SUPPORT_MINIMUM"), TEXT("1"));
	FString Query = TEXT("NAMESPACE:justice:ROLEID:SUPPORT_MINIMUM");
	bool bExpectedResult = true;

	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRoleMAtchNR, "PermissionTest.RoleMatch.NoRole", AutomationFlagMask);
bool FRoleMAtchNR::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:SUPPORT_MINIMUM"), TEXT("1"));
	FString Query = TEXT("NAMESPACE:justice:ROLEID:SUPPORT_SUPER_ADMIN");
	bool bExpectedResult = false;

	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTrailMatchAS, "PermissionTest.TrailMatch.AcceptString", AutomationFlagMask);
bool FTrailMatchAS::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:GODMODE_HEALTH_ONLY"), TEXT("1"));
	FString Query = TEXT("NAMESPACE:justice:ROLEID:GODMODE*");
	bool bExpectedResult = true;

	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FTrailMatchRS, "PermissionTest.TrailMatch.RejectString", AutomationFlagMask);
bool FTrailMatchRS::RunTest(const FString & Parameters)
{
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:GODMODE_HEALTH_ONLY"), TEXT("1"));
	FString Query = TEXT("NAMESPACE:justice:ROLEID:GODMODE");
	bool bExpectedResult = false;

	bool bQueryResult = RunQuery(AccountMock, Query, bExpectedResult);
	AccountMock.Reset();

	return AssertResult(bExpectedResult, bQueryResult);
}