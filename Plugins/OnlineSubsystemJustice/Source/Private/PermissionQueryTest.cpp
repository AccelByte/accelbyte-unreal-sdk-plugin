#include "AutomationTest.h"
#include "AutomationCommon.h"
#include "OnlineIdentityJustice.h"

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FPermissionQueryTest, "PermissionQueryTest", (EAutomationTestFlags::EditorContext | EAutomationTestFlags::NonNullRHI | EAutomationTestFlags::EngineFilter))
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(FPermissionQueryTest, "PermissionQueryTest", (EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter))

//TODO : GetTests for complex automation
//void FPermissionQueryTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
//{
//	TArray<FString> QueryArray;
//	QueryArray.Add("NAMESPACE:justice:USER:t9sk283j421i8:ORDER:rec231:action:15");	//check exact query 				(exp deny)	
//	QueryArray.Add("NAMESPACE:justice:USER:342t2345yw:ORDER:rec231:action:15");		//check 1 wc		 				(exp deny)	
//	QueryArray.Add("NAMESPACE:evil:USER:3k123l4k1:ACHIEVEMENT:ach342w:action:15");	//check incorrct resource			(exp deny)
//	OutBeautifiedNames.Append(QueryArray);
//	OutTestCommands.Append(QueryArray);
//}

bool FPermissionQueryTest::RunTest(const FString & Parameters)
{
	UE_LOG(LogTemp, Warning, TEXT("RUNNING SIMPLE TEST PERMISSION QUERY"));

	//mocking account
	TSharedPtr<FUserOnlineAccount> AccountMock = MakeShareable(new FUserOnlineAccountJustice("test@accelbyte.com"));

	//mock permission token
	// TODO : add moro for complex testing 
	AccountMock->SetUserAttribute("NAMESPACE:*:FILEUPLOAD", "10");

	//querying permission token
	// TODO : change "CheckedResource"
	FString CheckedResource = "NAMESPACE:justice:FILEUPLOAD:action:8";
	FString Result;
	if (AccountMock->GetUserAttribute(Parameters, Result))
	{
		//granting permissions
		UE_LOG(LogTemp, Warning, TEXT("Granting permission of %s, result %s"), *Parameters, *Result);
	}
	else
	{
		//denying permissions
		UE_LOG(LogTemp, Warning, TEXT("Denying permission of %s, result %s"), *Parameters, *Result);
	}
	return true;
}
