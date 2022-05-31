// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Api/AccelByteReportingApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "TestUtilities.h"
#include "ReportingTestAdmin.h"
#include "UserTestAdmin.h"
#include "Api/AccelByteUserApi.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteReportingTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteReportingTest);

const auto ReportingOnError = FErrorHandler::CreateLambda([](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteReportingTest, Error, TEXT("Error code: %d\nError message:%s"), Code, *Message);
	});

int32 AutomationFlagMaskReporting = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);
void FlushHttpRequests();

BEGIN_DEFINE_SPEC(FReportingTestSpec, "AccelByte.Tests.Reporting", EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	TArray<FString> ReportingReasonsTest = { TEXT("ReasonTest0"), TEXT("ReasonTest1"), TEXT("ReasonTest2"), TEXT("ReasonTest3") };
	TArray<FString> ReportingReasonGroupsTest = { TEXT("ReasonGroupTest0"), TEXT("ReasonGroupTest1"), TEXT("ReasonGroupTest2") };
	TArray<FString> TestReportingCodes;
	FTestUser TestUser{0};
END_DEFINE_SPEC(FReportingTestSpec)

void FReportingTestSpec::Define()
{
	Describe("A.ReportingSetup", [this]() {
		It("Should Provide TEST Setup config", [this]() {
			// Clean up previous user info.
			bool bReportingLoginUserSuccess;
			FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bReportingLoginUserSuccess]()
				{
					UE_LOG(LogAccelByteReportingTest, Log, TEXT("User login Success"));
					bReportingLoginUserSuccess = true;
				}), ReportingOnError);
			FlushHttpRequests();
			WaitUntil(bReportingLoginUserSuccess, "Waiting for login...");

			AB_TEST_TRUE(bReportingLoginUserSuccess);

			bool bReportingDeleteUserSuccess = false;
			AdminDeleteUser(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bReportingDeleteUserSuccess]()
				{
					UE_LOG(LogAccelByteReportingTest, Log, TEXT("Delete user Success"));
					bReportingDeleteUserSuccess = true;
				}), ReportingOnError);
			FlushHttpRequests();
			WaitUntil(bReportingDeleteUserSuccess, "Waiting for user deletion...");

			AB_TEST_TRUE(bReportingDeleteUserSuccess);

			// Login again.
			bReportingLoginUserSuccess = false;
			FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bReportingLoginUserSuccess]()
				{
					UE_LOG(LogAccelByteReportingTest, Log, TEXT("User login Success"));
					bReportingLoginUserSuccess = true;
				}), ReportingOnError);
			FlushHttpRequests();
			WaitUntil(bReportingLoginUserSuccess, "Waiting for login...");

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
			WaitUntil(bGetReasonsSuccess, "Waiting for getting reasons...");

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

						WaitUntil(bDeleteReasonSuccess, "Waiting Delete Reason Group...");
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
			WaitUntil(bGetReasonsGroupSuccess, "Waiting for getting reason groups...");

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

						WaitUntil(bDeleteReasonSuccess, "Waiting Delete Reason Group...");
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

				WaitUntil(bSubmitReasonSuccess, "Waiting Submit Reason...");

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
			WaitUntil(bGetReasonsSuccess, "Waiting for getting reasons...");

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

			WaitUntil(bSubmitReasonGroupSuccess, "Waiting Submit Reason Group 1...");
			AB_TEST_TRUE(bSubmitReasonGroupSuccess);

			// Add ReasonGroup link without reason
			bSubmitReasonGroupSuccess = false;
			FReportingAddReasonGroupRequest AddReasonRequest2;
			AddReasonRequest2.Title = ReportingReasonGroupsTest[1];
			ReportingAddReasonGroup(AddReasonRequest2, AddReasonGroupSuccessHandler, ReportingOnError);

			WaitUntil(bSubmitReasonGroupSuccess, "Waiting Submit Reason Group 2...");
			AB_TEST_TRUE(bSubmitReasonGroupSuccess);

			// Add ReasonGroup link without reason
			bSubmitReasonGroupSuccess = false;
			FReportingAddReasonGroupRequest AddReasonRequest3;
			AddReasonRequest3.Title = ReportingReasonGroupsTest[2];
			ReportingAddReasonGroup(AddReasonRequest3, AddReasonGroupSuccessHandler, ReportingOnError);

			WaitUntil(bSubmitReasonGroupSuccess, "Waiting Submit Reason Group 3...");
			AB_TEST_TRUE(bSubmitReasonGroupSuccess);

			// Retrieve User2 info or register it.
			// user2 preps
			TestUser.Email = FString::Printf(TEXT("reporting+test+ue4sdk@example.com"));
			TestUser.Email.ToLowerInline();
			TestUser.Password = TEXT("1Password1");
			TestUser.DisplayName = TEXT("reportingUE4");
			TestUser.Country = "US";
			FDateTime const DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 35));
			TestUser.DateOfBirth = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

			bool bDoneSearchUser = false;
			FPagedPublicUsersInfo ReceivedUserData;
			FRegistry::User.SearchUsers(TestUser.DisplayName
				, EAccelByteSearchType::DISPLAYNAME
				, THandler<FPagedPublicUsersInfo>::CreateLambda([&bDoneSearchUser, &ReceivedUserData](const FPagedPublicUsersInfo& Result)
					{
						bDoneSearchUser = true;
						ReceivedUserData = Result;
						UE_LOG(LogAccelByteReportingTest, Log, TEXT("Success Search User"));
					})
				, FErrorHandler::CreateLambda([&](int32 Code, FString Message)
					{
						UE_LOG(LogAccelByteReportingTest, Log, TEXT("Error code: %d\nError message:%s"), Code, *Message);
						bDoneSearchUser = true;
					})
				);
			WaitUntil(bDoneSearchUser, "Waiting for search user");
			AB_TEST_TRUE(bDoneSearchUser);

			if (ReceivedUserData.Data.Num() > 0)
			{
				bool bDeleteUser2 = false;
				AdminDeleteUser(ReceivedUserData.Data[0].UserId
					, FSimpleDelegate::CreateLambda([&bDeleteUser2]()
						{
							UE_LOG(LogAccelByteReportingTest, Log, TEXT("Delete user Success"));
							bDeleteUser2 = true;
						})
					, ReportingOnError);
				FlushHttpRequests();
				WaitUntil(bDeleteUser2, "Waiting for user deletion...");

				AB_TEST_TRUE(bDeleteUser2);
			}

			bool bUser2CreationSuccess = SetupTestUser(TestUser);

			AB_TEST_TRUE(bUser2CreationSuccess);

			return true;
			});
		});

	Describe("Z.ReportingTeardown", [this]() {
		It("Should Provide TEST Teardown config", [this]()
			{
				for (auto& reportingcode : TestReportingCodes)
				{
					bool bDeleteUserSuccess;
					AdminDeleteUser(reportingcode, FSimpleDelegate::CreateLambda([&]()
						{
							UE_LOG(LogAccelByteReportingTest, Log, TEXT("Delete user Success"));
							bDeleteUserSuccess = true;
						}), ReportingOnError);
					FlushHttpRequests();
					WaitUntil(bDeleteUserSuccess, "Waiting for deleting user...");
					AB_TEST_TRUE(bDeleteUserSuccess);
				}
				TeardownTestUser(TestUser);
				return true;
			}
		);
		});

	Describe("B.ReportingGetReasonGroups", [this]() {
		It("Should Provide TEST Get Reason Groups", [this]()
			{
				bool bGetReasonGroupsSuccess = false;
				FAccelByteModelsReasonGroupsResponse ReasonGroupsResponse;
				FRegistry::Reporting.GetReasonGroups(0, 2, THandler<FAccelByteModelsReasonGroupsResponse>::CreateLambda([&bGetReasonGroupsSuccess, &ReasonGroupsResponse](const FAccelByteModelsReasonGroupsResponse& Response)
					{
						UE_LOG(LogAccelByteReportingTest, Log, TEXT("Get reason groups Success"));
						ReasonGroupsResponse = Response;
						bGetReasonGroupsSuccess = true;
					}), ReportingOnError);
				FlushHttpRequests();
				WaitUntil(bGetReasonGroupsSuccess, "Waiting for get reason groups...");

				AB_TEST_NOT_EQUAL(ReasonGroupsResponse.Data.Num(), 0);
				AB_TEST_EQUAL(ReasonGroupsResponse.Data.Num(), 2);

				return true;
			}
		);
		});

	Describe("B.ReportingGetReason", [this]() {
		It("Should Provide TEST Get Reason", [this]()
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
				WaitUntil(bGetReasonsSuccess, "Waiting for get reasons...");

				AB_TEST_TRUE(bGetReasonsSuccess);
				AB_TEST_NOT_EQUAL(ReasonsResponse.Data.Num(), 0);
				AB_TEST_EQUAL(ReasonsResponse.Data.Num(), 2);

				return true;
			}
		);
		});

	Describe("B.ReportingGetReasons_WithReasonGroup", [this]() {
		It("Should Provide TEST Get Reason With Grouped", [this]()
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
				FlushHttpRequests();//clean
				WaitUntil(bGetReasonGroupsSuccess, "Waiting for get reason groups...");

				// Get Reason With Reason Group
				FRegistry::Reporting.GetReasons(ReasonGroupsResponse.Data[0].Title, 0, 2, THandler<FAccelByteModelsReasonsResponse>::CreateLambda([&bGetReasonsSuccess_Group1, &ReasonsResponse_Group1](const FAccelByteModelsReasonsResponse& Response)
					{
						UE_LOG(LogAccelByteReportingTest, Log, TEXT("Get reasons Success"));
						ReasonsResponse_Group1 = Response;
						UE_LOG(LogAccelByteReportingTest, Log, TEXT("Data : %s"), *ReasonsResponse_Group1.Data[0].Title);
						bGetReasonsSuccess_Group1 = true;
					}), ReportingOnError);
				FlushHttpRequests();
				WaitUntil(bGetReasonsSuccess_Group1, "Waiting for get reasons...");

				// Get Reason With Unlinked Reason Group
				FRegistry::Reporting.GetReasons(ReasonGroupsResponse.Data[1].Title, 0, 0, THandler<FAccelByteModelsReasonsResponse>::CreateLambda([&bGetReasonsSuccess_Group2, &ReasonsResponse_Group2](const FAccelByteModelsReasonsResponse& Response)
					{
						UE_LOG(LogAccelByteReportingTest, Log, TEXT("Get reasons Success"));
						ReasonsResponse_Group2 = Response;
						bGetReasonsSuccess_Group2 = true;
					}), ReportingOnError);
				FlushHttpRequests();
				WaitUntil(bGetReasonsSuccess_Group2, "Waiting for get reasons...");

				AB_TEST_TRUE(bGetReasonsSuccess_Group1);
				AB_TEST_NOT_EQUAL(ReasonsResponse_Group1.Data.Num(), 0);
				AB_TEST_EQUAL(ReasonsResponse_Group1.Data.Num(), 2);
				AB_TEST_TRUE(bGetReasonsSuccess_Group2);
				AB_TEST_EQUAL(ReasonsResponse_Group2.Data.Num(), 0);

				return true;
			}
		);
		});

	Describe("B.ReportingSubmitReport", [this]() {
		It("Should Provide TEST SubmitReport Config", [this]()
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
				WaitUntil(bGetReasonsSuccess, "Waiting for getting reasons...");

				bool bSubmitReport = false;
				FAccelByteModelsReportingSubmitData SubmitData;
				SubmitData.AdditionalInfo.Add(TEXT("Prop1"), TEXT("Addditional Property 1"));
				SubmitData.AdditionalInfo.Add(TEXT("Prop2"), TEXT("Addditional Property 2"));
				SubmitData.Category = EAccelByteReportingCategory::UGC;
				SubmitData.Comment = "AdditionalComment";
				SubmitData.ObjectId = "d2cedbbd8751442bb7077fb5c4940dba"; //picked from UUID Generator Online
				SubmitData.ObjectType = "";
				SubmitData.Reason = ReasonsResponse.Data[0].Title;
				SubmitData.UserID = TestUser.UserId;
				FRegistry::Reporting.SubmitReport(SubmitData
					, THandler<FAccelByteModelsReportingSubmitResponse>::CreateLambda([&bSubmitReport](const FAccelByteModelsReportingSubmitResponse& Response)
						{
							UE_LOG(LogAccelByteReportingTest, Log, TEXT("Submit Report Success"));
							bSubmitReport = true;
						})
					, ReportingOnError);

				WaitUntil(bSubmitReport, "Waiting for Submit Report...");
				AB_TEST_TRUE(bSubmitReport);

				return true;
			}
		);
	});
}
