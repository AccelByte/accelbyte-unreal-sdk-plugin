#include "AutomationTest.h"
#include "AutomationCommon.h"
#include "OnlineIdentityJustice.h"
#define DETAIL_AUTOMATION_TEST_LOG

const int AutomationFlagMask = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter); //Context: EditorContext (run via editor cli) ClientContext (run via external cli)
TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));;

bool RunQuery(const FString & Parameters)
{
	//Param format [0]Query,[1]ExpectedResult
	TArray<FString> QueryResultArray;
	Parameters.ParseIntoArray(QueryResultArray, TEXT(","), false);
	//querying permission
	FString OutQuery;
	bool Result = AccountMock->GetUserAttribute(QueryResultArray[0], OutQuery);
	//AccountMock.Reset();
	if (Result)
	{
		//granting permissions
		if (QueryResultArray[1] == TEXT("0"))
		{
#ifdef DETAIL_AUTOMATION_TEST_LOG
			UE_LOG(LogTemp, Warning, TEXT("QUERY TEST FAIL : GRANT permission of %s, action result %s"), *QueryResultArray[0], *OutQuery);
#endif // DETAIL_AUTOMATION_TEST_LOG
			return false;
		}
	}
	else
	{
		//denying permissions
		if (QueryResultArray[1] == TEXT("1"))
		{
#ifdef DETAIL_AUTOMATION_TEST_LOG
			UE_LOG(LogTemp, Warning, TEXT("QUERY TEST FAIL : DENY permission of %s, action result %s"), *QueryResultArray[0], *OutQuery);
#endif // DETAIL_AUTOMATION_TEST_LOG
			return false;
		}
	}
	return true;
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(WildcardMatchOnQuery, "PermissionTest.WildcardMatch.OnQuery", AutomationFlagMask);
void WildcardMatchOnQuery::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	TArray<FString> QueryArray;		//format : {query},{expectedresult}

	//Wilcards on query test
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:USER:accelpower:ORDER:Rec0x02"), TEXT("15"));

	QueryArray.Add(FString("NAMESPACE:justice:USER:*:ORDER:*:action:*")		+ FString(",1")); //put wildcard in query
	QueryArray.Add(FString("NAMESPACE:evil:USER:*:ORDER:*:action:*")		+ FString(",0")); //preceeding resource must match

	OutTestCommands.Append(QueryArray);
	OutBeautifiedNames.Append(QueryArray);

	QueryArray.Empty();
}
bool WildcardMatchOnQuery::RunTest(const FString & Parameters)
{
	return RunQuery(Parameters);
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(WildcardMatchOnPermission, "PermissionTest.WildcardMatch.OnPermission", AutomationFlagMask);
void WildcardMatchOnPermission::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	TArray<FString> QueryArray;		//format : {query},{expectedresult}

	//wildcard on permission test
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:*:FULFILLMENT:*"), TEXT("7"));

	QueryArray.Add(FString("NAMESPACE:evil:FULFILLMENT:GrantItem:action:7")	+ FString(",1"));//any resource specification will be granted
	QueryArray.Add(FString("NAMESPACE:justice:DUTY:*:action:7")				+ FString(",0"));//intended resource must matching

	OutTestCommands.Append(QueryArray);
	OutBeautifiedNames.Append(QueryArray);

	QueryArray.Empty();
}
bool WildcardMatchOnPermission::RunTest(const FString & Parameters)
{
	return RunQuery(Parameters);
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(WildcardMatchOnAction, "PermissionTest.WildcardMatch.OnAction", AutomationFlagMask);
void WildcardMatchOnAction::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	TArray<FString> QueryArray;	//format : {query},{expectedresult}

	//wildcard on permission test
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:ACHIEVEMENT:*"), TEXT("7"));

	QueryArray.Add(FString("NAMESPACE:evil:ACHIEVEMENT:Daily:action:7") + FString(",1"));
	QueryArray.Add(FString("NAMESPACE:evil:ACHIEVEMENT:*")				+ FString(",0"));//action need to be specified

	OutTestCommands.Append(QueryArray);
	OutBeautifiedNames.Append(QueryArray);

	QueryArray.Empty();
}
bool WildcardMatchOnAction::RunTest(const FString & Parameters)
{
	return RunQuery(Parameters);
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(LengthMatch, "PermissionTest.LengthMatch", AutomationFlagMask);
void LengthMatch::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	TArray<FString> QueryArray;		//format : {query},{expectedresult}

	//Query length Test
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:evil:USER:accelpower:LEVEL:PET"), TEXT("15"));

	QueryArray.Add(FString("NAMESPACE:evil:USER:accelpower:LEVEL:PET:action:15")	+ FString(",1"));
	QueryArray.Add(FString("NAMESPACE:evil:LEVEL:PET:action:15")					+ FString(",0")); //query format incomplete

	OutTestCommands.Append(QueryArray);
	OutBeautifiedNames.Append(QueryArray);

	QueryArray.Empty();
}
bool LengthMatch::RunTest(const FString & Parameters)
{
	return RunQuery(Parameters);
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(BitmaskMatch, "PermissionTest.BitmaskMatch", AutomationFlagMask);
void BitmaskMatch::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	TArray<FString> QueryArray;		//format : {query},{expectedresult}

	//Bitmasking Test
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:FILEUPLOAD"), TEXT("10"));

	QueryArray.Add(FString("NAMESPACE:justice:FILEUPLOAD:action:1") + FString(",0"));//bitmask 10(read delete) doesnt contain 1(create)
	QueryArray.Add(FString("NAMESPACE:justice:FILEUPLOAD:action:8") + FString(",1"));//bitmask 10(read delete) contains 8(delete)

	OutTestCommands.Append(QueryArray);
	OutBeautifiedNames.Append(QueryArray);

	QueryArray.Empty();
}
bool BitmaskMatch::RunTest(const FString & Parameters)
{
	return RunQuery(Parameters);
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(RoleMatch, "PermissionTest.RoleMatch", AutomationFlagMask);
void RoleMatch::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	TArray<FString> QueryArray;		//format : {query},{expectedresult}

	//Role Match Test
	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:SUPPORT_MINIMUM"), TEXT("1"));

	//Role is set of permissions on several resources, no action needed to be specified
	QueryArray.Add(FString("NAMESPACE:justice:ROLEID:SUPPORT_MINIMUM")		+ FString(",1"));
	QueryArray.Add(FString("NAMESPACE:justice:ROLEID:SUPPORT_SUP_ADMIN")	+ FString(",0"));

	OutTestCommands.Append(QueryArray);
	OutBeautifiedNames.Append(QueryArray);

	QueryArray.Empty();
}
bool RoleMatch::RunTest(const FString & Parameters)
{
	return RunQuery(Parameters);
}

IMPLEMENT_COMPLEX_AUTOMATION_TEST(TrailMatch, "PermissionTest.TrailMatch", AutomationFlagMask);
void TrailMatch::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	TArray<FString> QueryArray;		//format : {query},{expectedresult}

	AccountMock->SetUserAttribute(TEXT("NAMESPACE:justice:ROLEID:GODMODE_HEALTH_ONLY"), TEXT("1"));

	QueryArray.Add(FString("NAMESPACE:justice:ROLEID:GODMODE")	+ FString(",0"));
	QueryArray.Add(FString("NAMESPACE:justice:ROLEID:GODMODE*")	+ FString(",1"));//querying prefixed permission/role is allowed

	OutTestCommands.Append(QueryArray);
	OutBeautifiedNames.Append(QueryArray);

	QueryArray.Empty();
}
bool TrailMatch::RunTest(const FString & Parameters)
{
	return RunQuery(Parameters);
}