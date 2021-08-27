// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteAgreementApi.h"
#include "TestUtilities.h"
#include "UserTestAdmin.h"
#include "AgreementTestAdmin.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteAgreementTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteAgreementTest);

const auto AgreementTestErrorHandler =
		FErrorHandler::CreateLambda([](const int32 ErrorCode, const FString ErrorMessage)
		{
			UE_LOG(LogAccelByteAgreementTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode,
				*ErrorMessage);
		});

BEGIN_DEFINE_SPEC(FAgreementTestSpec, "AccelByte.Tests.Agreement",
                  EAutomationTestFlags::ProductFilter | EAutomationTestFlags::ApplicationContextMask)
	// XXX Need to use predefined policies in backend (policy name, tags, country)
	// because we cannot freely add and remove policies.
	const FString AgreementTestPolicyName = TEXT("SDK Test Policy");
	const FString AgreementTestPolicyCountry = TEXT("MG");
	const TArray<FString> AgreementTestPolicyTags = {TEXT("tag1"), TEXT("tag2")};

	bool bSetupPolicyOnce = false;
	FString AgreementTestPolicyId;
END_DEFINE_SPEC(FAgreementTestSpec)

void FAgreementTestSpec::Define()
{
	const auto SetupOnce = [this]()
	{
		if (bSetupPolicyOnce)
		{
			return true;
		}

		bSetupPolicyOnce = true;

		// Check and create policies in backend for this test only if necessary.

		// Check policy in backend logic is expected to run only once before running agreement tests.
		// Create policy in backend logic is expected to run only when existing policies does not exist.
		// However, since we cannot freely add and remove policies in backend, the create policy in backend logic
		// is actually only run once when this test is first created.
		// The create policy in backend logic is kept for documentation purpose.

		bool bGetBasePoliciesSuccess = false;
		TArray<FAgreementBasePolicy> BasePolicies;
		AdminGetAgreementBasePolicies(
			THandler<TArray<FAgreementBasePolicy>>::CreateLambda(
				[&](const TArray<FAgreementBasePolicy>& Response)
				{
					BasePolicies = Response;
					bGetBasePoliciesSuccess = true;
				}), AgreementTestErrorHandler);
		WaitUntil(bGetBasePoliciesSuccess, "Waiting for AdminGetAgreementBasePolicies ...");

		FAgreementBasePolicy BasePolicy;
		for (const auto& Policy : BasePolicies)
		{
			if (Policy.BasePolicyName == AgreementTestPolicyName
				&& Policy.Namespace == FRegistry::Settings.Namespace)
			{
				BasePolicy = Policy;
				break;
			}
		}

		if (BasePolicy.Id.IsEmpty())
		{
			bool bGetPolicyTypesDone = false;
			TArray<FAgreementPolicyTypeObject> PolicyTypes;
			AdminGetAgreementPolicyTypes(
				THandler<TArray<FAgreementPolicyTypeObject>>::CreateLambda(
					[&](
					const TArray<FAgreementPolicyTypeObject>& Response)
					{
						PolicyTypes = Response;
						bGetPolicyTypesDone = true;
					}), AgreementTestErrorHandler);
			WaitUntil(bGetPolicyTypesDone, "Waiting for AdminGetAgreementPolicyTypes ...");

			FString PolicyTypeId;
			for (const auto& PolicyType : PolicyTypes)
			{
				if (PolicyType.PolicyTypeName.Contains("Legal Document"))
				{
					PolicyTypeId = PolicyType.Id;
					break;
				}
			}

			bool bCreatePoliciesDone = false;
			FAgreementBasePolicyCreate CreateRequest;
			CreateRequest.BasePolicyName = AgreementTestPolicyName;
			CreateRequest.AffectedCountries = TArray<FString>({AgreementTestPolicyCountry});
			CreateRequest.AffectedClientIds = TArray<FString>();
			CreateRequest.TypeId = PolicyTypeId;
			CreateRequest.Tags = AgreementTestPolicyTags;
			CreateRequest.Namespace = FRegistry::Settings.Namespace;
			AdminCreateAgreementBasePolicy(
				CreateRequest,
				THandler<FAgreementBasePolicy>::CreateLambda(
					[&bCreatePoliciesDone, &BasePolicy](const FAgreementBasePolicy& Response)
					{
						BasePolicy = Response;
						bCreatePoliciesDone = true;
					}), AgreementTestErrorHandler);
			WaitUntil(bCreatePoliciesDone, "Waiting for AdminCreateAgreementBasePolicy ...");
		}

		AB_TEST_FALSE(BasePolicy.Id.IsEmpty());

		for (const auto& Policy : BasePolicy.Policies)
		{
			if (Policy.PolicyName == AgreementTestPolicyName
				&& Policy.CountryCode == AgreementTestPolicyCountry)
			{
				AgreementTestPolicyId = Policy.Id;
				break;
			}
		}

		AB_TEST_FALSE(AgreementTestPolicyId.IsEmpty());

		bool bGetCountryPolicySuccess = false;
		FAgreementCountryPolicy CountryPolicy;
		AdminGetAgreementCountryBasePolicy(
			BasePolicy.Id,
			AgreementTestPolicyCountry,
			THandler<FAgreementCountryPolicy>::CreateLambda(
				[&bGetCountryPolicySuccess, &CountryPolicy](
				const FAgreementCountryPolicy& Response)
				{
					CountryPolicy = Response;
					bGetCountryPolicySuccess = true;
				}), AgreementTestErrorHandler);
		WaitUntil(bGetCountryPolicySuccess, "Waiting for AdminGetAgreementCountryBasePolicy ...");

		AB_TEST_FALSE(CountryPolicy.Id.IsEmpty())

		FAgreementPolicyVersion PolicyVersion;
		bool bPolicyPublished = false;

		if (CountryPolicy.PolicyVersions.Num() == 0)
		{
			bool bCreatePolicyVersionSuccess = false;
			FAgreementPolicyVersionCreate CreateRequest;
			CreateRequest.DisplayVersion = "1.0.0";
			CreateRequest.IsCommitted = true;
			CreateRequest.Description = AgreementTestPolicyName;
			AdminCreateAgreementPolicyVersion(
				AgreementTestPolicyId,
				CreateRequest,
				THandler<FAgreementPolicyVersion>::CreateLambda(
					[&PolicyVersion, &bCreatePolicyVersionSuccess](
					const FAgreementPolicyVersion& Response)
					{
						PolicyVersion = Response;
						bCreatePolicyVersionSuccess = true;
					}), AgreementTestErrorHandler);
			WaitUntil(bCreatePolicyVersionSuccess, "Waiting for AdminCreateAgreementPolicyVersion ...");
		}
		else
		{
			for (const auto& CountryPolicyVersion : CountryPolicy.PolicyVersions)
			{
				if (CountryPolicyVersion.IsInEffect)
				{
					bPolicyPublished = true;
					PolicyVersion = CountryPolicyVersion;
					break;
				}
			}

			if (!bPolicyPublished)
			{
				PolicyVersion = CountryPolicy.PolicyVersions[0];
			}
		}

		AB_TEST_FALSE(PolicyVersion.Id.IsEmpty());

		TArray<FAgreementLocalizedPolicy> LocalizedPolicies;
		bool bGetLocalizedPolicySuccess = false;
		AdminGetAgreementLocalizedPolicies(
			PolicyVersion.Id,
			THandler<TArray<FAgreementLocalizedPolicy>>::CreateLambda(
				[&bGetLocalizedPolicySuccess, &LocalizedPolicies](
				const TArray<FAgreementLocalizedPolicy>& Result)
				{
					LocalizedPolicies = Result;
					bGetLocalizedPolicySuccess = true;
				}), AgreementTestErrorHandler);
		WaitUntil(bGetLocalizedPolicySuccess, "Waiting for AdminGetAgreementLocalizedPolicies ...");

		if (LocalizedPolicies.Num() == 0)
		{
			bool bCreateLocalizedPolicySuccess = false;
			FAgreementLocalizedPolicy LocalizedPolicy;
			FAgreementLocalizedPolicyCreate CreateRequest;
			CreateRequest.LocaleCode = "en";
			CreateRequest.ContentType = "text/plain";
			CreateRequest.Description = AgreementTestPolicyName;
			AdminCreateAgreementLocalizedPolicy(
				PolicyVersion.Id,
				CreateRequest,
				THandler<FAgreementLocalizedPolicy>::CreateLambda(
					[&LocalizedPolicy, &bCreateLocalizedPolicySuccess](
					const FAgreementLocalizedPolicy& Response)
					{
						LocalizedPolicy = Response;
						bCreateLocalizedPolicySuccess = true;
					}), AgreementTestErrorHandler);
			WaitUntil(bCreateLocalizedPolicySuccess, "Waiting for AdminCreateAgreementLocalizedPolicy ...");
		}

		if (!bPolicyPublished)
		{
			bool bPublishPolicyVersionSuccess = false;
			AdminPublishAgreementPolicyVersion(
				PolicyVersion.Id,
				false,
				FSimpleDelegate::CreateLambda(
					[&bPublishPolicyVersionSuccess]()
					{
						bPublishPolicyVersionSuccess = true;
					}), AgreementTestErrorHandler);
			WaitUntil(bPublishPolicyVersionSuccess, "Waiting for AdminPublishAgreementPolicyVersion ...");
		}

		return true;
	};

	BeforeEach(SetupOnce);

	Describe("GetLegalPolicies", [this]()
	{
		It("should return some legal policies", [this]()
		{
			FTestUser TestUser;
			TestUser.Country = AgreementTestPolicyCountry;

			AB_TEST_TRUE(RegisterTestUser(TestUser));
			AB_TEST_TRUE(LoginTestUser(TestUser));

			Api::Agreement AgreementApi(TestUser.Credentials, FRegistry::Settings,
			                            FRegistry::HttpRetryScheduler);

			TArray<FAccelByteModelsPublicPolicy> Policies;
			bool bGetPoliciesSuccess = false;
			AgreementApi.GetLegalPolicies(
				EAccelByteAgreementPolicyType::EMPTY,
				false,
				THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda(
					[&](
					const TArray<FAccelByteModelsPublicPolicy>& Response)
					{
						Policies = Response;
						bGetPoliciesSuccess = true;
					}), AgreementTestErrorHandler);
			WaitUntil(bGetPoliciesSuccess, "Waiting for GetLegalPolicies ...");

			AB_TEST_TRUE(bGetPoliciesSuccess);

			AB_TEST_TRUE(DeleteTestUser(TestUser));

			return true;
		});

		// TODO Add more test for GetLegalPolicies with Tags
	});

	Describe("GetLegalPoliciesByCountry", [this]()
	{
		Describe("WithoutTags", [this]()
		{
			It("should return some legal policies", [this]()
			{
				FTestUser TestUser;
				TestUser.Country = AgreementTestPolicyCountry;

				bool bGetPoliciesDone = false;
				TArray<FAccelByteModelsPublicPolicy> Policies;

				Api::Agreement AgreementApi(TestUser.Credentials, FRegistry::Settings,
				                            FRegistry::HttpRetryScheduler);
				AgreementApi.GetLegalPoliciesByCountry(
					AgreementTestPolicyCountry,
					EAccelByteAgreementPolicyType::EMPTY,
					false,
					THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda(
						[&bGetPoliciesDone, &Policies](
						const TArray<FAccelByteModelsPublicPolicy>& Response)
						{
							Policies = Response;
							bGetPoliciesDone = true;
						}), AgreementTestErrorHandler);
				WaitUntil(bGetPoliciesDone, "Waiting for GetLegalPoliciesByCountry ...");

				AB_TEST_TRUE(Policies.Num() > 0);
				AB_TEST_EQUAL(Policies[0].CountryCode, AgreementTestPolicyCountry);

				return true;
			});
		});

		Describe("WithMatchingTags", [this]()
		{
			const TArray<TArray<FString>> MatchingPolicyTagsCombinations = {
				{AgreementTestPolicyTags[0]},
				{AgreementTestPolicyTags[0], AgreementTestPolicyTags[1]}
			};
			
			for (int Index = 0; Index < MatchingPolicyTagsCombinations.Num(); Index++)
			{
				It(FString::Printf(TEXT("should return some legal policies with matching tags (%d)"), Index + 1),
				   [this, MatchingPolicyTagsCombinations, Index]()
				   {
					   FTestUser TestUser;
					   TestUser.Country = AgreementTestPolicyCountry;
				   	
					   bool bGetPoliciesDone = false;
					   TArray<FAccelByteModelsPublicPolicy> Policies;

					   Api::Agreement AgreementApi(TestUser.Credentials, FRegistry::Settings,
					                               FRegistry::HttpRetryScheduler);
					   AgreementApi.GetLegalPoliciesByCountry(
						   AgreementTestPolicyCountry,
						   EAccelByteAgreementPolicyType::EMPTY,
						   MatchingPolicyTagsCombinations[Index],
						   false,
						   THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda(
							   [&bGetPoliciesDone, &Policies](
							   const TArray<FAccelByteModelsPublicPolicy>& Result)
							   {
								   Policies = Result;
								   bGetPoliciesDone = true;
							   }), AgreementTestErrorHandler);
					   WaitUntil(bGetPoliciesDone, "Waiting for GetLegalPoliciesByCountry ...");

					   AB_TEST_TRUE(Policies.Num() > 0);
					   AB_TEST_EQUAL(Policies[0].CountryCode, AgreementTestPolicyCountry);

					   return true;
				   });
			}
		});

		Describe("WithNonMatchingTags", [this]()
		{
			It("should not return any legal policies", [this]()
			{
				FTestUser TestUser;
				TestUser.Country = AgreementTestPolicyCountry;

				bool bGetPoliciesDone = false;
				TArray<FAccelByteModelsPublicPolicy> Policies;

				Api::Agreement AgreementApi(TestUser.Credentials, FRegistry::Settings,
				                            FRegistry::HttpRetryScheduler);
				AgreementApi.GetLegalPoliciesByCountry(
					AgreementTestPolicyCountry,
					EAccelByteAgreementPolicyType::EMPTY,
					{TEXT("nonmatchingtag")},
					false,
					THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda([&](
						const TArray<FAccelByteModelsPublicPolicy>& Response)
						{
							Policies = Response;
							bGetPoliciesDone = true;
						}), AgreementTestErrorHandler);
				WaitUntil(bGetPoliciesDone, "Waiting for GetLegalPoliciesByCountry...");

				AB_TEST_EQUAL(Policies.Num(), 0);

				return true;
			});
		});
	});

	Describe("GetPublisherLegalPoliciesAndItsDocument", [this]()
	{
		It("should get a publisher legal policy and its document", [this]()
		{
			FTestUser TestUser;
			TestUser.Country = AgreementTestPolicyCountry;

			AB_TEST_TRUE(RegisterTestUser(TestUser));
			AB_TEST_TRUE(LoginTestUser(TestUser));

			Api::Agreement AgreementApi(TestUser.Credentials, FRegistry::Settings,
			                            FRegistry::HttpRetryScheduler);

			bool bGetPoliciesSuccess = false;
			TArray<FAccelByteModelsPublicPolicy> Policies;

			AgreementApi.GetLegalPolicies(
				GetPublisherNamespace(),
				EAccelByteAgreementPolicyType::EMPTY,
				true,
				THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda(
					[&bGetPoliciesSuccess, &Policies](
					const TArray<FAccelByteModelsPublicPolicy>& Response)
					{
						Policies = Response;
						bGetPoliciesSuccess = true;
					}), AgreementTestErrorHandler);

			WaitUntil(bGetPoliciesSuccess, "Waiting for GetLegalPolicies ...");
			FString LocalizedPolicyUrl;
			for (const auto& policy : Policies)
			{
				for (const auto& policyVersion : policy.PolicyVersions)
				{
					if (policyVersion.IsInEffect)
					{
						FString LocalizedPolicyVersionId = policyVersion.LocalizedPolicyVersions[0].Id;
						LocalizedPolicyUrl = policyVersion.LocalizedPolicyVersions[0].AttachmentLocation;
						break;
					}
				}
			}

			FString Result;
			bool bGetLegalDocSuccess = false;
			AgreementApi.GetLegalDocument(
				LocalizedPolicyUrl,
				THandler<FString>::CreateLambda(
					[&bGetLegalDocSuccess, &Result](const FString& Response)
					{
						bGetLegalDocSuccess = true;
						Result = Response;
					}), AgreementTestErrorHandler);
			WaitUntil(bGetLegalDocSuccess, "Waiting for GetLegalDocument ...");

			AB_TEST_TRUE(bGetLegalDocSuccess);
			AB_TEST_FALSE(Result.IsEmpty());

			AB_TEST_TRUE(DeleteTestUser(TestUser));

			return true;
		});
	});

	Describe("AcceptPolicyVersion", [this]()
	{
		It("should get user to accept a policy", [this]()
		{
			// Setup

			FTestUser TestUser;
			TestUser.Country = AgreementTestPolicyCountry;

			AB_TEST_TRUE(RegisterTestUser(TestUser));
			AB_TEST_TRUE(LoginTestUser(TestUser));

			Api::Agreement AgreementApi(TestUser.Credentials, FRegistry::Settings,
			                            FRegistry::HttpRetryScheduler);

			// Verify that policy has NOT been accepted by user

			TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse> EligibilitiesBeforeResult;
			bool bGetEligibilitiesBeforeSuccess = false;

			AgreementApi.QueryLegalEligibilities(
				FRegistry::Settings.Namespace,
				THandler<TArray<
					FAccelByteModelsRetrieveUserEligibilitiesResponse>>::CreateLambda(
					[&](
					const TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>&
					Result)
					{
						bGetEligibilitiesBeforeSuccess = true;
						EligibilitiesBeforeResult = Result;
					}), AgreementTestErrorHandler);
			WaitUntil(bGetEligibilitiesBeforeSuccess, "Waiting for get QueryLegalEligibilities ...");
			AB_TEST_TRUE(bGetEligibilitiesBeforeSuccess);

			{
				bool bIsComply = false;
				for (const auto& Eligibility : EligibilitiesBeforeResult)
				{
					if (Eligibility.IsAccepted)
					{
						bIsComply = true;
						break;
					}
				}
				AB_TEST_FALSE(bIsComply);
			}

			// Accept policy

			FString LocalizedPolicyVersionId;
			for (const auto& Policy : EligibilitiesBeforeResult)
			{
				for (const auto& PolicyVersion : Policy.PolicyVersions)
				{
					if (PolicyVersion.IsInEffect)
					{
						LocalizedPolicyVersionId = PolicyVersion.LocalizedPolicyVersions[0].Id;
						break;
					}
				}
			}
			AB_TEST_FALSE(LocalizedPolicyVersionId.IsEmpty());

			bool bAcceptPolicySuccess = false;
			AgreementApi.AcceptPolicyVersion(
				LocalizedPolicyVersionId,
				FVoidHandler::CreateLambda([&]()
				{
					bAcceptPolicySuccess = true;
				}), AgreementTestErrorHandler);
			WaitUntil(bAcceptPolicySuccess, "Waiting for AcceptPolicyVersion ...");
			AB_TEST_TRUE(bAcceptPolicySuccess);

			// Verify that now policy has been accepted by user

			TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse> EligibilitiesAfterResult;
			bool bGetEligibilitiesAfterSuccess = false;

			AgreementApi.QueryLegalEligibilities(
				FRegistry::Settings.Namespace,
				THandler<TArray<
					FAccelByteModelsRetrieveUserEligibilitiesResponse>>::CreateLambda(
					[&](
					const TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>&
					Result)
					{
						bGetEligibilitiesAfterSuccess = true;
						EligibilitiesAfterResult = Result;
					}), AgreementTestErrorHandler);
			WaitUntil(bGetEligibilitiesAfterSuccess, "Waiting for QueryLegalEligibilities ...");
			AB_TEST_TRUE(bGetEligibilitiesAfterSuccess);

			{
				bool bIsComply = false;
				for (const auto& Eligibility : EligibilitiesAfterResult)
				{
					if (Eligibility.IsAccepted)
					{
						bIsComply = true;
						break;
					}
				}
				AB_TEST_TRUE(bIsComply);
			}

			// Teardown

			AB_TEST_TRUE(DeleteTestUser(TestUser));

			return true;
		});
	});

	Describe("BulkAcceptPolicyVersion", [this]()
	{
		It("should get user to bulk accept policies", [this]()
		{
			// Setup

			FTestUser TestUser;
			TestUser.Country = AgreementTestPolicyCountry;

			AB_TEST_TRUE(RegisterTestUser(TestUser));
			AB_TEST_TRUE(LoginTestUser(TestUser));

			Api::Agreement AgreementApi(TestUser.Credentials, FRegistry::Settings,
			                            FRegistry::HttpRetryScheduler);

			TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse> EligibilitiesBeforeResult;
			bool bGetEligibilitiesBeforeSuccess = false;

			AgreementApi.QueryLegalEligibilities(
				FRegistry::Settings.Namespace,
				THandler<TArray<
					FAccelByteModelsRetrieveUserEligibilitiesResponse>>::CreateLambda(
					[&](
					const TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>&
					Result)
					{
						bGetEligibilitiesBeforeSuccess = true;
						EligibilitiesBeforeResult = Result;
					}), AgreementTestErrorHandler);
			WaitUntil(bGetEligibilitiesBeforeSuccess, "Waiting for get QueryLegalEligibilities ...");
			AB_TEST_TRUE(bGetEligibilitiesBeforeSuccess);

			{
				bool bIsComply = false;
				for (const auto& Eligibility : EligibilitiesBeforeResult)
				{
					if (Eligibility.IsAccepted)
					{
						bIsComply = true;
						break;
					}
				}
				AB_TEST_FALSE(bIsComply);
			}

			// Bulk accept policy

			TArray<FAccelByteModelsAcceptAgreementRequest> AcceptAgreementRequests;
			for (const auto& Policy : EligibilitiesBeforeResult)
			{
				for (const auto& PolicyVersion : Policy.PolicyVersions)
				{
					if (PolicyVersion.IsInEffect)
					{
						FAccelByteModelsAcceptAgreementRequest AcceptRequest;
						AcceptRequest.IsAccepted = true;
						AcceptRequest.PolicyId = Policy.PolicyId;
						AcceptRequest.PolicyVersionId = PolicyVersion.Id;
						AcceptRequest.LocalizedPolicyVersionId = PolicyVersion.LocalizedPolicyVersions[0].Id;
						AcceptAgreementRequests.Add(AcceptRequest);
					}
				}
			}
			AB_TEST_TRUE(AcceptAgreementRequests.Num() > 0);

			bool bAcceptPoliciesSuccess = false;
			bool bProceed = false;
			AgreementApi.BulkAcceptPolicyVersions(
				AcceptAgreementRequests,
				THandler<FAccelByteModelsAcceptAgreementResponse>::CreateLambda(
					[&](
					const FAccelByteModelsAcceptAgreementResponse& Response)
					{
						bProceed = Response.Proceed;
						bAcceptPoliciesSuccess = true;
					}), AgreementTestErrorHandler);
			WaitUntil(bAcceptPoliciesSuccess, "Waiting for BulkAcceptPolicyVersions ...");
			AB_TEST_TRUE(bProceed);

			// Verify that now policy has been accepted by user

			TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse> EligibilitiesAfterResult;
			bool bGetEligibilitiesAfterSuccess = false;

			AgreementApi.QueryLegalEligibilities(
				FRegistry::Settings.Namespace,
				THandler<TArray<
					FAccelByteModelsRetrieveUserEligibilitiesResponse>>::CreateLambda(
					[&bGetEligibilitiesAfterSuccess, &EligibilitiesAfterResult](
					const TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>&
					Result)
					{
						bGetEligibilitiesAfterSuccess = true;
						EligibilitiesAfterResult = Result;
					}), AgreementTestErrorHandler);
			WaitUntil(bGetEligibilitiesAfterSuccess, "Waiting for QueryLegalEligibilities ...");
			AB_TEST_TRUE(bGetEligibilitiesAfterSuccess);

			{
				bool bIsComply = false;
				for (const auto& Eligibility : EligibilitiesAfterResult)
				{
					if (Eligibility.IsAccepted)
					{
						bIsComply = true;
						break;
					}
				}
				AB_TEST_TRUE(bIsComply);
			}

			// Teardown

			AB_TEST_TRUE(DeleteTestUser(TestUser));

			return true;
		});
	});

	Describe("RegisterUserWithAcceptedPolicies", [this]()
	{
		It("should register user with accepted policies", [this]()
		{
			// Setup

			FTestUser TestUser;
			TestUser.Country = AgreementTestPolicyCountry;

			Api::Agreement AgreementApi(TestUser.Credentials, FRegistry::Settings,
			                            FRegistry::HttpRetryScheduler);
			Api::User UserApi(TestUser.Credentials, FRegistry::Settings,
			                  FRegistry::HttpRetryScheduler);

			// Get legal policies

			TArray<FAccelByteModelsPublicPolicy> Policies;
			bool bGetPoliciesDone = false;
			AgreementApi.GetLegalPoliciesByCountry(
				TestUser.Country,
				EAccelByteAgreementPolicyType::EMPTY,
				false,
				THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda(
					[&](const TArray<FAccelByteModelsPublicPolicy>& Result)
					{
						Policies = Result;
						bGetPoliciesDone = true;
					}), AgreementTestErrorHandler);
			WaitUntil(bGetPoliciesDone, "Waiting for GetLegalPoliciesByCountry ...");

			AB_TEST_TRUE(Policies.Num() > 0);
			AB_TEST_EQUAL(Policies[0].CountryCode, AgreementTestPolicyCountry);

			TArray<FAcceptedPolicies> AcceptAgreementRequests;
			for (const auto& Policy : Policies)
			{
				for (const auto& PolVersion : Policy.PolicyVersions)
				{
					if (PolVersion.IsInEffect)
					{
						FAcceptedPolicies AcceptRequest;
						AcceptRequest.IsAccepted = true;
						AcceptRequest.PolicyId = Policy.Id;
						AcceptRequest.PolicyVersionId = PolVersion.Id;
						AcceptRequest.LocalizedPolicyVersionId = PolVersion.LocalizedPolicyVersions[0].Id;
						AcceptAgreementRequests.Add(AcceptRequest);
					}
				}
			}
			AB_TEST_TRUE(AcceptAgreementRequests.Num() > 0);

			// Register test user with accepted policies

			FRegisterRequestv3 NewUserRequest;
			NewUserRequest.AcceptedPolicies = AcceptAgreementRequests;
			NewUserRequest.DisplayName = TestUser.DisplayName;
			NewUserRequest.Password = TestUser.Password;
			NewUserRequest.EmailAddress = TestUser.Email;
			NewUserRequest.Username = TestUser.DisplayName;
			NewUserRequest.AuthType = TEXT("EMAILPASSWD");
			NewUserRequest.Country = TestUser.Country;
			NewUserRequest.DateOfBirth = TestUser.DateOfBirth;

			bool bUsersCreationDone = false;
			bool bUserCreated = false;
			UserApi.Registerv3(
				NewUserRequest,
				THandler<FRegisterResponse>::CreateLambda(
					[&](const FRegisterResponse& Response)
					{
						bUserCreated = true;
						bUsersCreationDone = true;
					}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
				{
					bUsersCreationDone = true;
				}));
			WaitUntil(bUsersCreationDone, "Waiting for Registerv3 ...");
			AB_TEST_TRUE(bUserCreated);

			AB_TEST_TRUE(LoginTestUser(TestUser));

			// Teardown

			AB_TEST_TRUE(DeleteTestUser(TestUser));

			return true;
		});
	});
}
