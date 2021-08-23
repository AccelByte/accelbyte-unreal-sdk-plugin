// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "TestUtilities.h"
#include "ReportingTestAdmin.h"
#include "Api/AccelByteReportingApi.h"
#include "Core/AccelByteRegistry.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteReportingTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteReportingTest);

const int32 AutomationFlagMaskReporting = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
void FlushHttpRequests();
void ReportingAddReason(const FReportingAddReasonRequest& BodyRequest, const THandler<FReportingAddReasonResponse>& OnSuccess, const FErrorHandler& OnError);
void ReportingDeleteReason(const FString& ReasonID, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
void ReportingAdminGetReasons(const FString& ReasonGroup, const THandler<FAccelByteModelsAdminReasonsResponse>& OnSuccess, const FErrorHandler& OnError);
void ReportingAddReasonGroup(const FReportingAddReasonGroupRequest& BodyRequest, const THandler<FReportingAddReasonGroupResponse>& OnSuccess, const FErrorHandler& OnError);
void ReportingAdminGetReasonGroups(const THandler<FAccelByteModelsReasonGroupsResponse>& OnSuccess, const FErrorHandler& OnError);
void ReportingDeleteReasonGroup(const FString& ReasonGroupID, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

// Setup variables.
TArray<FString> ReportingReasonsTest = { TEXT("ReasonTest0"), TEXT("ReasonTest1"), TEXT("ReasonTest2"), TEXT("ReasonTest3") };
TArray<FString> ReportingReasonGroupsTest = { TEXT("ReasonGroupTest0"), TEXT("ReasonGroupTest1"), TEXT("ReasonGroupTest2")};

static Credentials User2Creds;
static TSharedPtr<Api::User> User2;

const auto ReportingOnError = FErrorHandler::CreateLambda([](int32 Code, const FString& Message)
{
	UE_LOG(LogAccelByteReportingTest, Error, TEXT("Error code: %d\nError message:%s"), Code, *Message);
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ReportingSetup, "AccelByte.Tests.Reporting.A.Setup", AutomationFlagMaskReporting)
bool ReportingSetup::RunTest(const FString& Parameters)
{
	// Clean up previous user info.
	bool bReportingLoginUserSuccess;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bReportingLoginUserSuccess]()
	{
		UE_LOG(LogAccelByteReportingTest, Log, TEXT("User login Success"));
		bReportingLoginUserSuccess = true;
	}), ReportingOnError);
	FlushHttpRequests();
	Waiting(bReportingLoginUserSuccess, "Waiting for login...");

	AB_TEST_TRUE(bReportingLoginUserSuccess);

	bool bReportingDeleteUserSuccess = false;
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bReportingDeleteUserSuccess]()
	{
		UE_LOG(LogAccelByteReportingTest, Log, TEXT("Delete user Success"));
		bReportingDeleteUserSuccess = true;
	}), ReportingOnError);
	FlushHttpRequests();
	Waiting(bReportingDeleteUserSuccess, "Waiting for user deletion...");

	AB_TEST_TRUE(bReportingDeleteUserSuccess);

	// Login again.
	bReportingLoginUserSuccess = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bReportingLoginUserSuccess]()
	{
		UE_LOG(LogAccelByteReportingTest, Log, TEXT("User login Success"));
		bReportingLoginUserSuccess = true;
	}), ReportingOnError);
	FlushHttpRequests();
	Waiting(bReportingLoginUserSuccess, "Waiting for login...");

	AB_TEST_TRUE(bReportingLoginUserSuccess);
	
	// Setup Reporting configuration.
	// Clean up reason test.
	bool bGetReasonsSuccess = false;
	FAccelByteModelsAdminReasonsResponse ReasonsResponse;
	FString ReasonGroup = "";
	ReportingAdminGetReasons(ReasonGroup, THandler<FAccelByteModelsAdminReasonsResponse>::CreateLambda([&bGetReasonsSuccess, &ReasonsResponse](const FAccelByteModelsAdminReasonsResponse& Response)
		{
			UE_LOG(LogAccelByteReportingTest, Log, TEXT("Get reasons Success"));
			ReasonsResponse = Response;
			bGetReasonsSuccess = true;
		}), ReportingOnError);
	FlushHttpRequests();
	Waiting(bGetReasonsSuccess, "Waiting for getting reasons...");

	AB_TEST_TRUE(bGetReasonsSuccess);

	for (const FString& ReportingReasonTest : ReportingReasonsTest) 
	{
		for (const FAccelByteModelsAdminReasonItem& ReasonItem : ReasonsResponse.Data)
		{
			if (ReportingReasonTest == ReasonItem.Title) {
				bool bDeleteReasonSuccess = false;
				ReportingDeleteReason(ReasonItem.ID, FVoidHandler::CreateLambda([&bDeleteReasonSuccess]()
					{
						UE_LOG(LogAccelByteReportingTest, Log, TEXT("Delete Reason Success"));
						bDeleteReasonSuccess = true;
					}), ReportingOnError);

				Waiting(bDeleteReasonSuccess, "Waiting Delete Reason Group...");
			}
		}
	}

	// Clean up reason group test.
	bool bGetReasonsGroupSuccess = false;
	FAccelByteModelsReasonGroupsResponse ReasonsGroupResponse;
	ReportingAdminGetReasonGroups(THandler<FAccelByteModelsReasonGroupsResponse>::CreateLambda([&bGetReasonsGroupSuccess, &ReasonsGroupResponse](const FAccelByteModelsReasonGroupsResponse& Response)
		{
			UE_LOG(LogAccelByteReportingTest, Log, TEXT("Get reasons Success"));
			ReasonsGroupResponse = Response;
			bGetReasonsGroupSuccess = true;
		}), ReportingOnError);
	FlushHttpRequests();
	Waiting(bGetReasonsGroupSuccess, "Waiting for getting reason groups...");

	AB_TEST_TRUE(bGetReasonsGroupSuccess);

	for (const FString& ReportingReasonGroupTest : ReportingReasonGroupsTest)
	{
		for (const FAccelByteModelsReasonGroupsItem& ReasonGroupItem : ReasonsGroupResponse.Data)
		{
			if (ReportingReasonGroupTest == ReasonGroupItem.Title) {
				bool bDeleteReasonSuccess = false;
				ReportingDeleteReasonGroup(ReasonGroupItem.ID, FVoidHandler::CreateLambda([&bDeleteReasonSuccess]()
					{
						UE_LOG(LogAccelByteReportingTest, Log, TEXT("Delete Reason Group Success"));
						bDeleteReasonSuccess = true;
					}), ReportingOnError);

				Waiting(bDeleteReasonSuccess, "Waiting Delete Reason Group...");
			}
		}
	}

	// Add Reasons
	for (const FString& ReportingReasonTest : ReportingReasonsTest)
	{
		FReportingAddReasonRequest AddReasonRequest;
		AddReasonRequest.Title = ReportingReasonTest;
		AddReasonRequest.Description = ReportingReasonTest + " Description";
		bool bSubmitReasonSuccess = false;
		ReportingAddReason(AddReasonRequest, THandler<FReportingAddReasonResponse>::CreateLambda([&bSubmitReasonSuccess](const FReportingAddReasonResponse& Result)
			{
				UE_LOG(LogAccelByteReportingTest, Log, TEXT("Submit Reason Success"));
				bSubmitReasonSuccess = true;
			}), ReportingOnError);

		Waiting(bSubmitReasonSuccess, "Waiting Submit Reason...");

		AB_TEST_TRUE(bSubmitReasonSuccess);
	}

	// Get reasons
	bGetReasonsSuccess = false;
	ReasonsResponse = FAccelByteModelsAdminReasonsResponse();
	ReasonGroup = "";
	ReportingAdminGetReasons(ReasonGroup, THandler<FAccelByteModelsAdminReasonsResponse>::CreateLambda([&bGetReasonsSuccess, &ReasonsResponse](const FAccelByteModelsAdminReasonsResponse& Response)
		{
			UE_LOG(LogAccelByteReportingTest, Log, TEXT("Get reasons Success"));
			ReasonsResponse = Response;
			bGetReasonsSuccess = true;
		}), ReportingOnError);
	FlushHttpRequests();
	Waiting(bGetReasonsSuccess, "Waiting for getting reasons...");

	// Add ReasonGroup
	bool bSubmitReasonGroupSuccess = false;
	THandler<FReportingAddReasonGroupResponse> AddReasonGroupSuccessHandler =
		THandler<FReportingAddReasonGroupResponse>::CreateLambda([&bSubmitReasonGroupSuccess](const FReportingAddReasonGroupResponse& Result) {
				UE_LOG(LogAccelByteReportingTest, Log, TEXT("Submit reasons group Success"));
				bSubmitReasonGroupSuccess = true;
			});

	// Add ReasonGroup link with reason
	bSubmitReasonGroupSuccess = false;
	FReportingAddReasonGroupRequest AddReasonRequest1;
	AddReasonRequest1.ReasonIds.Add(ReasonsResponse.Data[0].ID);
	AddReasonRequest1.ReasonIds.Add(ReasonsResponse.Data[1].ID);
	AddReasonRequest1.ReasonIds.Add(ReasonsResponse.Data[2].ID);
	AddReasonRequest1.Title = ReportingReasonGroupsTest[0];
	ReportingAddReasonGroup(AddReasonRequest1, AddReasonGroupSuccessHandler, ReportingOnError);

	Waiting(bSubmitReasonGroupSuccess, "Waiting Submit Reason Group 1...");
	AB_TEST_TRUE(bSubmitReasonGroupSuccess);

	// Add ReasonGroup link without reason
	bSubmitReasonGroupSuccess = false;
	FReportingAddReasonGroupRequest AddReasonRequest2;
	AddReasonRequest2.Title = ReportingReasonGroupsTest[1];
	ReportingAddReasonGroup(AddReasonRequest2, AddReasonGroupSuccessHandler, ReportingOnError);

	Waiting(bSubmitReasonGroupSuccess, "Waiting Submit Reason Group 2...");
	AB_TEST_TRUE(bSubmitReasonGroupSuccess);

	// Add ReasonGroup link without reason
	bSubmitReasonGroupSuccess = false;
	FReportingAddReasonGroupRequest AddReasonRequest3;
	AddReasonRequest3.Title = ReportingReasonGroupsTest[2];
	ReportingAddReasonGroup(AddReasonRequest3, AddReasonGroupSuccessHandler, ReportingOnError);

	Waiting(bSubmitReasonGroupSuccess, "Waiting Submit Reason Group 3...");
	AB_TEST_TRUE(bSubmitReasonGroupSuccess);

	// Retrieve User2 info or register it.
	// user2 preps
	User2 = MakeShared<Api::User>(User2Creds, FRegistry::Settings, FRegistry::HttpRetryScheduler);
	FString Email = FString::Printf(TEXT("reporting+test+ue4sdk@example.com"));
	Email.ToLowerInline();
	FString const Password = TEXT("1Password1");
	FString const DisplayName = FString::Printf(TEXT("reportingUE4"));
	FString const Country = "US";
	FDateTime const DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 35));
	FString const Format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bUserCreationSuccess = false;
	User2->Register(
		Email,
		Password,
		DisplayName,
		Country,
		Format,
		THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
			{
				bUserCreationSuccess = true;
				UE_LOG(LogAccelByteReportingTest, Log, TEXT("Test Reporting User2 is Created"));

			}),
		FErrorHandler::CreateLambda([&](int32 Code, FString Message)
			{
				UE_LOG(LogAccelByteReportingTest, Log, TEXT("Code=%d"), Code);
				if (static_cast<ErrorCodes>(Code) == ErrorCodes::UserEmailAlreadyUsedException || static_cast<ErrorCodes>(Code) == ErrorCodes::UserDisplayNameAlreadyUsedException) //email already used
				{
					bUserCreationSuccess = true;
					UE_LOG(LogAccelByteReportingTest, Log, TEXT("Test Reporting User2 is already"));
				}
				else
				{
					UE_LOG(LogAccelByteReportingTest, Log, TEXT("Test Reporting User can't be created"));
				}
			}));

	Waiting(bUserCreationSuccess, "Waiting for User2 created...");

	bool bUser2LoginSuccess = false;
	User2->LoginWithUsername(
		Email,
		Password,
		FVoidHandler::CreateLambda([&]()
			{
				bUser2LoginSuccess = true;
			}),
		ReportingOnError);

	Waiting(bUser2LoginSuccess, "Waiting for User2 Login...");

	AB_TEST_TRUE(bUser2LoginSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ReportingTeardown, "AccelByte.Tests.Reporting.Z.Teardown", AutomationFlagMaskReporting)
bool ReportingTeardown::RunTest(const FString& Parameters)
{
	
	bool bDeleteUserSuccess;
	DeleteUserById(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&]()
	{
		UE_LOG(LogAccelByteReportingTest, Log, TEXT("Delete user Success"));
		bDeleteUserSuccess = true;
	}), ReportingOnError);
	FlushHttpRequests();
	Waiting(bDeleteUserSuccess, "Waiting for deleting user...");

	AB_TEST_TRUE(bDeleteUserSuccess);

	bool bUser2LogoutSuccess = false;
	User2->Logout(
		FVoidHandler::CreateLambda([&]()
			{
				bUser2LogoutSuccess = true;
			}),
		ReportingOnError);

	Waiting(bUser2LogoutSuccess, "Waiting for User2 Logout...");

	AB_TEST_TRUE(bUser2LogoutSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ReportingGetReasonGroups, "AccelByte.Tests.Reporting.B.GetReasonGroups", AutomationFlagMaskReporting);
bool ReportingGetReasonGroups::RunTest(const FString& Parameters)
{
	// Get Reason Groups
	bool bGetReasonGroupsSuccess = false;
	FAccelByteModelsReasonGroupsResponse ReasonGroupsResponse;
	FRegistry::Reporting.GetReasonGroups(0, 2, THandler<FAccelByteModelsReasonGroupsResponse>::CreateLambda([&bGetReasonGroupsSuccess, &ReasonGroupsResponse](const FAccelByteModelsReasonGroupsResponse& Response)
		{
			UE_LOG(LogAccelByteReportingTest, Log, TEXT("Get reason groups Success"));
			ReasonGroupsResponse = Response;
			bGetReasonGroupsSuccess = true;
		}), ReportingOnError);
	FlushHttpRequests();
	Waiting(bGetReasonGroupsSuccess, "Waiting for get reason groups...");

	AB_TEST_NOT_EQUAL(ReasonGroupsResponse.Data.Num(), 0);
	AB_TEST_EQUAL(ReasonGroupsResponse.Data.Num(), 2);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ReportingGetReasons, "AccelByte.Tests.Reporting.B.GetReasons", AutomationFlagMaskReporting);
bool ReportingGetReasons::RunTest(const FString& Parameters)
{
	bool bGetReasonsSuccess = false;
	FAccelByteModelsReasonsResponse ReasonsResponse;

	// Get Reason With empty Reason Group
	FString ReasonGroup = "";
	FRegistry::Reporting.GetReasons(ReasonGroup, 0, 2, THandler<FAccelByteModelsReasonsResponse>::CreateLambda([&bGetReasonsSuccess, &ReasonsResponse](const FAccelByteModelsReasonsResponse& Response)
	{
		UE_LOG(LogAccelByteReportingTest, Log, TEXT("Get reasons Success"));
		ReasonsResponse = Response;
		bGetReasonsSuccess = true;
	}), ReportingOnError);
	FlushHttpRequests();
	Waiting(bGetReasonsSuccess, "Waiting for get reasons...");

	AB_TEST_TRUE(bGetReasonsSuccess);
	AB_TEST_NOT_EQUAL(ReasonsResponse.Data.Num(), 0);
	AB_TEST_EQUAL(ReasonsResponse.Data.Num(), 2);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ReportingGetReasons_WithReasonGroup, "AccelByte.Tests.Reporting.B.GetReasons_WithReasonGroup", AutomationFlagMaskReporting);
bool ReportingGetReasons_WithReasonGroup::RunTest(const FString& Parameters)
{
	FAccelByteModelsReasonsResponse ReasonsResponse_Group1 = FAccelByteModelsReasonsResponse();
	FAccelByteModelsReasonsResponse ReasonsResponse_Group2 = FAccelByteModelsReasonsResponse();
	bool bGetReasonsSuccess_Group1 = false;
	bool bGetReasonsSuccess_Group2 = false;

	// Get Reason Group
	bool bGetReasonGroupsSuccess = false;
	FAccelByteModelsReasonGroupsResponse ReasonGroupsResponse;
	FRegistry::Reporting.GetReasonGroups(0, 0, THandler<FAccelByteModelsReasonGroupsResponse>::CreateLambda([&bGetReasonGroupsSuccess, &ReasonGroupsResponse](const FAccelByteModelsReasonGroupsResponse& Response)
		{
			UE_LOG(LogAccelByteReportingTest, Log, TEXT("Get reasons group Success"));
			ReasonGroupsResponse = Response;
			bGetReasonGroupsSuccess = true;
		}), ReportingOnError);
	FlushHttpRequests();
	Waiting(bGetReasonGroupsSuccess, "Waiting for get reason groups...");

	// Get Reason With Reason Group
	FRegistry::Reporting.GetReasons(ReasonGroupsResponse.Data[0].Title, 0, 2, THandler<FAccelByteModelsReasonsResponse>::CreateLambda([&bGetReasonsSuccess_Group1, &ReasonsResponse_Group1](const FAccelByteModelsReasonsResponse& Response)
		{
			UE_LOG(LogAccelByteReportingTest, Log, TEXT("Get reasons Success"));
			ReasonsResponse_Group1 = Response;
			bGetReasonsSuccess_Group1 = true;
		}), ReportingOnError);
	FlushHttpRequests();
	Waiting(bGetReasonsSuccess_Group1, "Waiting for get reasons...");

	// Get Reason With Unlinked Reason Group
	FRegistry::Reporting.GetReasons(ReasonGroupsResponse.Data[1].Title, 0, 0, THandler<FAccelByteModelsReasonsResponse>::CreateLambda([&bGetReasonsSuccess_Group2, &ReasonsResponse_Group2](const FAccelByteModelsReasonsResponse& Response)
		{
			UE_LOG(LogAccelByteReportingTest, Log, TEXT("Get reasons Success"));
			ReasonsResponse_Group2 = Response;
			bGetReasonsSuccess_Group2 = true;
		}), ReportingOnError);
	FlushHttpRequests();
	Waiting(bGetReasonsSuccess_Group2, "Waiting for get reasons...");

	AB_TEST_TRUE(bGetReasonsSuccess_Group1);
	AB_TEST_NOT_EQUAL(ReasonsResponse_Group1.Data.Num(), 0);
	AB_TEST_EQUAL(ReasonsResponse_Group1.Data.Num(), 2);
	AB_TEST_TRUE(bGetReasonsSuccess_Group2);
	AB_TEST_EQUAL(ReasonsResponse_Group2.Data.Num(), 0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(ReportingSubmitReport, "AccelByte.Tests.Reporting.B.SubmitReport", AutomationFlagMaskReporting);
bool ReportingSubmitReport::RunTest(const FString& Parameters)
{
	bool bGetReasonsSuccess = false;
	FAccelByteModelsReasonsResponse ReasonsResponse;

	// Get Reason With empty Reason Group
	FString ReasonGroup = "";
	FRegistry::Reporting.GetReasons(ReasonGroup, 0, 0, THandler<FAccelByteModelsReasonsResponse>::CreateLambda([&bGetReasonsSuccess, &ReasonsResponse](const FAccelByteModelsReasonsResponse& Response)
		{
			UE_LOG(LogAccelByteReportingTest, Log, TEXT("Get reasons Success"));
			ReasonsResponse = Response;
			bGetReasonsSuccess = true;
		}), ReportingOnError);
	FlushHttpRequests();
	Waiting(bGetReasonsSuccess, "Waiting for getting reasons...");

	bool bSubmitReport = false;
	FAccelByteModelsReportingSubmitData SubmitData;
	SubmitData.AdditionalInfo.Add(TEXT("Prop1"), TEXT("Addditional Property 1"));
	SubmitData.AdditionalInfo.Add(TEXT("Prop2"), TEXT("Addditional Property 2"));
	SubmitData.Category = EAccelByteReportingCategory::UGC;
	SubmitData.Comment = "AdditionalComment";
	SubmitData.ObjectId = "";
	SubmitData.ObjectType = "";
	SubmitData.Reason = ReasonsResponse.Data[0].Title;
	SubmitData.UserID = User2Creds.GetUserId();
	FRegistry::Reporting.SubmitReport(SubmitData, THandler<FAccelByteModelsReportingSubmitResponse>::CreateLambda([&bSubmitReport](const FAccelByteModelsReportingSubmitResponse& Response)
		{
			UE_LOG(LogAccelByteReportingTest, Log, TEXT("Submit Report Success"));
			bSubmitReport = true;
		}), ReportingOnError);

	Waiting(bSubmitReport, "Waiting for Submit Report...");
	AB_TEST_TRUE(bSubmitReport);

	return true;
}