#include "AutomationTest.h"
#include "AutomationCommon.h"
#include "OnlineIdentityJustice.h"
#include "FileHelper.h"
#include "Paths.h"
//#define DETAIL_AUTOMATION_TEST_LOG

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FPermissionQueryTest, "PermissionQueryTest", (EAutomationTestFlags::ClientContext | EAutomationTestFlags::NonNullRHI | EAutomationTestFlags::ProductFilter))
//Context: EditorContext (run via editor cli) ClientContext (run via external cli)
TSharedPtr<FUserOnlineAccount> AccountMock;

void FPermissionQueryTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	TArray<FString> FileArray;
	TArray<FString> QueryArray;

	AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));

	//TestCaseFileFormat [0]Source, [1]Action, [2]Query, [3]ExpectedResult(1 Grant 0 Deny)
	FString FilePath = FString (FPaths::GameDir() + "Plugins/OnlineSubsystemJustice/TestCase/PermissionQueryTestCase.csv");
	FFileHelper::LoadANSITextFileToStrings(*FilePath, NULL, FileArray);
	if(FileArray.Num()>0) //error checking for  empty csv file
	{
		for (FString FileLine : FileArray)
		{
			if (FileLine.IsEmpty()) //error checking for empty csv line
			{
				continue;
			}
			TArray<FString> FileLineParsed;
			FileLine.ParseIntoArray(FileLineParsed, TEXT(","), false);
			if ( !FileLineParsed[0].IsEmpty() && !FileLineParsed[1].IsEmpty() )
			{
				AccountMock->SetUserAttribute(FileLineParsed[0], FileLineParsed[1]);
			}
			QueryArray.Add(FileLineParsed[2]+","+FileLineParsed[3]);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[FAIL] Test cases was not loaded"));
	}

	OutBeautifiedNames.Append(QueryArray);
	OutTestCommands.Append(QueryArray);
}

bool FPermissionQueryTest::RunTest(const FString & Parameters)
{
	//Param format [0]Query,[1]ExpectedResult
	TArray<FString> QueryResultArray;
	Parameters.ParseIntoArray(QueryResultArray, TEXT(","), false);
	//querying permission
	FString Result;
	if (AccountMock->GetUserAttribute(QueryResultArray[0], Result))
	{
		//granting permissions
		if (QueryResultArray[1] == TEXT("0"))
		{
#ifdef DETAIL_AUTOMATION_TEST_LOG
			UE_LOG(LogTemp, Warning, TEXT("QUERY TEST FAIL : GRANT permission of %s, action result %s"), *QueryResultArray[0], *Result);
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
			UE_LOG(LogTemp, Warning, TEXT("QUERY TEST FAIL : DENY permission of %s, action result %s"), *QueryResultArray[0], *Result);
#endif // DETAIL_AUTOMATION_TEST_LOG
			return false;
		}
	}
	return true;
}