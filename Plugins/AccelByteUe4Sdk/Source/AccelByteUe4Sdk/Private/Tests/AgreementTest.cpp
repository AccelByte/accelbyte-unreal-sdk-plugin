// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Api/AccelByteAgreementApi.h"
#include "TestUtilities.h"

using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteAgreementTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteAgreementTest);

const int32 AutomationFlagMaskAgreement = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto AgreementTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteAgreementTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

struct AgreementTestUserInfo
{
	const FString EmailAddress = "Agreement_UE4SDKTest@example.com";
	const FString Password = "Password+123";
	const FString CountryCode = "MG";
	const FString DisplayName = "AgreeMan";
	FString UserId;
};
static AgreementTestUserInfo AgreementTestUserInfo_;

static FString PolicyId;
static TArray<FString> PolicyTags = TArray<FString>({ TEXT("tag1"), TEXT("tag2") });;

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AgreementSetup, "AccelByte.Tests.Agreement.A.Setup", AutomationFlagMaskAgreement);
bool AgreementSetup::RunTest(const FString& Parameters)
{
	bool UsersDeletionDone = false;
	bool bUserCreated = false;
	bool bUserLoggedIn = false;

	// CLEAN UP PREVIOUS USER
	User_Delete_By_Email_Address(AgreementTestUserInfo_.EmailAddress, FSimpleDelegate::CreateLambda([&UsersDeletionDone]() {
		UsersDeletionDone = true;
	}), FErrorHandler::CreateLambda([&UsersDeletionDone](int32 Code, FString Message) {
		UsersDeletionDone = true;
	}));
	Waiting(UsersDeletionDone, "Waiting for cleanup previous user...");

	// USER CREATION
	UE_LOG(LogAccelByteAgreementTest, Log, TEXT("Agreement user creation ..."));

	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21 + 7));
	const FString DoB_String = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	bool bUsersCreationDone = false;
	FRegistry::User.Register(AgreementTestUserInfo_.EmailAddress, AgreementTestUserInfo_.Password, AgreementTestUserInfo_.DisplayName, AgreementTestUserInfo_.CountryCode, DoB_String, THandler<FRegisterResponse>::CreateLambda([&bUserCreated, &bUsersCreationDone](const FRegisterResponse& Response)
	{
		bUserCreated = true;
		UE_LOG(LogAccelByteAgreementTest, Log, TEXT("Test User is successfuly created."));
		bUsersCreationDone = true;
	}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
	{
		if (static_cast<ErrorCodes>(Code) == ErrorCodes::UserEmailAlreadyUsedException)
		{
			bUserCreated = true;
			UE_LOG(LogAccelByteAgreementTest, Log, TEXT("Test User is already created."));
		}
		else
		{
			UE_LOG(LogAccelByteAgreementTest, Log, TEXT("Test User can't be created"));
		}
		bUsersCreationDone = true;
	}));
	Waiting(bUsersCreationDone, "Waiting for user created...");

	// USER LOG IN
	UE_LOG(LogAccelByteAgreementTest, Log, TEXT("Agreement user login..."));

	bUserLoggedIn = false;
	FRegistry::User.LoginWithUsername(
		AgreementTestUserInfo_.EmailAddress,
		AgreementTestUserInfo_.Password,
		FVoidHandler::CreateLambda([&bUserLoggedIn]()
	{
		AgreementTestUserInfo_.UserId = FRegistry::Credentials.GetUserId();
		bUserLoggedIn = true;
		UE_LOG(LogAccelByteAgreementTest, Log, TEXT("\t\tSuccessfully Login."));
	}), AgreementTestErrorHandler);
	Waiting(bUserLoggedIn, "Waiting for login with user name...");
	UE_LOG(LogAccelByteAgreementTest, Log, TEXT("User creds: %s"), *FRegistry::Credentials.GetUserId());

	AB_TEST_TRUE(bUserCreated);
	AB_TEST_TRUE(bUserLoggedIn);

	bool bGetBasePoliciesSuccess = false;
	TArray<FAgreementBasePolicy> basePolicies;
	Agreement_Get_Base_Policies(THandler<TArray<FAgreementBasePolicy>>::CreateLambda([&basePolicies, &bGetBasePoliciesSuccess](const TArray<FAgreementBasePolicy>& BasePolicies) {
		basePolicies = BasePolicies;
		bGetBasePoliciesSuccess = true;
	}), AgreementTestErrorHandler);

	Waiting(bGetBasePoliciesSuccess, "Waiting for get base policies...");

	FString countryPolicyId;
	FAgreementBasePolicy basePolicy;
	for (const auto& policy : basePolicies)
	{
		if (policy.Namespace == FRegistry::Settings.Namespace)
		{
			basePolicy = policy;
		}
	}

	if (basePolicy.Id.IsEmpty())
	{
		bool bGetPolicyTypesDone = false;
		TArray<FAgreementPolicyTypeObject> policyTypes;
		Agreement_Get_Policy_Types(THandler<TArray<FAgreementPolicyTypeObject>>::CreateLambda([&bGetPolicyTypesDone, &policyTypes](const TArray<FAgreementPolicyTypeObject>& PolicyTypes) {
			policyTypes = PolicyTypes;
			bGetPolicyTypesDone = true;
		}), AgreementTestErrorHandler);

		Waiting(bGetPolicyTypesDone, "Waiting for get policy types...");

		FString policyTypeId;
		for (const auto& type : policyTypes)
		{
			if (type.PolicyTypeName.Contains("Legal Document"))
			{
				policyTypeId = type.Id;
				break;
			}
		}

		bool bCreatePoliciesDone = false;
		FAgreementBasePolicyCreate CreateRequest;
		CreateRequest.BasePolicyName = "SDK Test Policy";
		CreateRequest.AffectedCountries = TArray<FString>({ AgreementTestUserInfo_.CountryCode });
		CreateRequest.AffectedClientIds = TArray<FString>();
		CreateRequest.TypeId = policyTypeId;
		CreateRequest.Namespace = FRegistry::Settings.Namespace;
		Agreement_Create_Base_Policy(CreateRequest, THandler<FAgreementBasePolicy>::CreateLambda([&bCreatePoliciesDone, &basePolicy](const FAgreementBasePolicy& BasePolicy)
		{
			basePolicy = BasePolicy;
			bCreatePoliciesDone = true;
		}), AgreementTestErrorHandler);

		Waiting(bCreatePoliciesDone, "Waiting for creating policy...");
	}

	AB_TEST_FALSE(basePolicy.Id.IsEmpty());

	for (const auto& policy : basePolicy.Policies)
	{
		if (policy.CountryCode == AgreementTestUserInfo_.CountryCode)
		{
			PolicyId = policy.Id;
			break;
		}
	}

	AB_TEST_FALSE(PolicyId.IsEmpty());

	bool bGetCountryPolicySuccess = false;
	FAgreementCountryPolicy countryPolicy;
	Agreement_Get_Country_Base_Policy(basePolicy.Id, AgreementTestUserInfo_.CountryCode, THandler<FAgreementCountryPolicy>::CreateLambda([&bGetCountryPolicySuccess, &countryPolicy](const FAgreementCountryPolicy& Policy)
	{
		countryPolicy = Policy;
		bGetCountryPolicySuccess = true;
	}), AgreementTestErrorHandler);

	Waiting(bGetCountryPolicySuccess, "Waiting for get country policy...");

	AB_TEST_FALSE(countryPolicy.Id.IsEmpty())

	FAgreementPolicyVersion policyVersion;
	bool bPolicyPublished = false;
	if (countryPolicy.PolicyVersions.Num() == 0)
	{
		bool bCreatePolicyVersionSuccess = false;
		FAgreementPolicyVersionCreate CreateRequest;
		CreateRequest.DisplayVersion = "1.0.0";
		CreateRequest.IsCommitted = true;
		CreateRequest.Description = "SDK Policy Test";
		Agreement_Create_Policy_Version(PolicyId, CreateRequest, THandler<FAgreementPolicyVersion>::CreateLambda([&policyVersion, &bCreatePolicyVersionSuccess](const FAgreementPolicyVersion& PolicyVersion) {
			policyVersion = PolicyVersion;
			bCreatePolicyVersionSuccess = true;
		}), AgreementTestErrorHandler);

		Waiting(bCreatePolicyVersionSuccess, "Waiting for create policy version...");
	}
	else
	{
		for (const auto& policy : countryPolicy.PolicyVersions)
		{
			if (policy.IsInEffect)
			{
				bPolicyPublished = true;
				policyVersion = policy;
				break;
			}
		}
		// no active policy
		if (!bPolicyPublished)
		{
			policyVersion = countryPolicy.PolicyVersions[0];
		}
	}

	AB_TEST_FALSE(policyVersion.Id.IsEmpty());

	TArray<FAgreementLocalizedPolicy> localizedPolicies;
	bool bGetLocalizedPolicySuccess = false;
	Agreement_Get_Localized_Policies(policyVersion.Id, THandler<TArray<FAgreementLocalizedPolicy>>::CreateLambda([&bGetLocalizedPolicySuccess, &localizedPolicies](const TArray<FAgreementLocalizedPolicy>& LocalizedPolicies) {
		localizedPolicies = LocalizedPolicies;
		bGetLocalizedPolicySuccess = true;
	}), AgreementTestErrorHandler);

	Waiting(bGetLocalizedPolicySuccess, "Waiting for get localized policies...");

	if (localizedPolicies.Num() == 0)
	{
		FAgreementLocalizedPolicy localizedPolicy;
		bool bCreateLocalizedPolicySuccess = false;
		FAgreementLocalizedPolicyCreate CreateRequest;
		CreateRequest.LocaleCode = "en";
		CreateRequest.ContentType = "text/plain";
		CreateRequest.Description = "SDK Policy Test";
		Agreement_Create_Localized_Policy(policyVersion.Id, CreateRequest, THandler<FAgreementLocalizedPolicy>::CreateLambda([&localizedPolicy, &bCreateLocalizedPolicySuccess](const FAgreementLocalizedPolicy& LocalizedPolicy) {
			localizedPolicy = LocalizedPolicy;
			bCreateLocalizedPolicySuccess = true;
		}), AgreementTestErrorHandler);

		Waiting(bCreateLocalizedPolicySuccess, "Waiting for create localized policy...");
	}

	if (!bPolicyPublished)
	{
#if ENGINE_MINOR_VERSION >= 23
		bool bPublishPolicyVersionSuccess = false;
		Agreement_Publish_Policy_Version(policyVersion.Id, false, FSimpleDelegate::CreateLambda([&bPublishPolicyVersionSuccess]() {
			bPublishPolicyVersionSuccess = true;
		}), AgreementTestErrorHandler);
		Waiting(bPublishPolicyVersionSuccess, "Publish policy version...");
#else
		UE_LOG(LogAccelByteAgreementTest, Fatal, TEXT("agreement setup failed: current ue4 version is not supported patch method, please publish policy %s manually"), *policyVersion.Id);
#endif
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AgreementTearDown, "AccelByte.Tests.Agreement.Z.Teardown", AutomationFlagMaskAgreement);
bool AgreementTearDown::RunTest(const FString& Parameters)
{
	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;

	UE_LOG(LogAccelByteAgreementTest, Log, TEXT("DeleteUserById"));
	DeleteUserById(AgreementTestUserInfo_.UserId, FSimpleDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteAgreementTest, Log, TEXT("    Success"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), AgreementTestErrorHandler);
	Waiting(bDeleteDone, "Waiting for deletion...");

	AB_TEST_TRUE(bDeleteSuccessful);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AgreementGetLegalPoliciesByCountryWithoutTag, "AccelByte.Tests.Agreement.C.GetLegalPoliciesByCountryWithoutTag", AutomationFlagMaskAgreement);
bool AgreementGetLegalPoliciesByCountryWithoutTag::RunTest(const FString& Parameters)
{
	bool bGetPoliciesDone = false;
	TArray<FAccelByteModelsPublicPolicy> policies;
	const EAccelByteAgreementPolicyType AgreementPolicyType = EAccelByteAgreementPolicyType::EMPTY;

	FRegistry::Agreement.GetLegalPoliciesByCountry(AgreementTestUserInfo_.CountryCode, AgreementPolicyType, false, 
		THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda([&bGetPoliciesDone, &policies](const TArray<FAccelByteModelsPublicPolicy>& Policies) {
		policies = Policies;
		bGetPoliciesDone = true;
	}), AgreementTestErrorHandler);
	Waiting(bGetPoliciesDone, "Waiting for get policies by country...");

	AB_TEST_TRUE(policies.Num() > 0);
	AB_TEST_EQUAL(policies[0].CountryCode , AgreementTestUserInfo_.CountryCode);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AgreementGetLegalPoliciesByCountryWithSomeTags, "AccelByte.Tests.Agreement.C.GetLegalPoliciesByCountryWithSomeTags", AutomationFlagMaskAgreement);
bool AgreementGetLegalPoliciesByCountryWithSomeTags::RunTest(const FString& Parameters)
{
	bool bGetPoliciesDone = false;
	TArray<FAccelByteModelsPublicPolicy> policies;
	const EAccelByteAgreementPolicyType AgreementPolicyType = EAccelByteAgreementPolicyType::EMPTY;

	FRegistry::Agreement.GetLegalPoliciesByCountry(AgreementTestUserInfo_.CountryCode, AgreementPolicyType, PolicyTags, false, 
		THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda([&bGetPoliciesDone, &policies](const TArray<FAccelByteModelsPublicPolicy>& Policies) {
		policies = Policies;
		bGetPoliciesDone = true;
	}), AgreementTestErrorHandler);
	Waiting(bGetPoliciesDone, "Waiting for get policies by country...");

	AB_TEST_TRUE(policies.Num() > 0);
	AB_TEST_EQUAL(policies[0].CountryCode , AgreementTestUserInfo_.CountryCode);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AgreementGetLegalPoliciesByCountryWithOneTag, "AccelByte.Tests.Agreement.C.GetLegalPoliciesByCountryWithOneTag", AutomationFlagMaskAgreement);
bool AgreementGetLegalPoliciesByCountryWithOneTag::RunTest(const FString& Parameters)
{
	bool bGetPoliciesDone = false;
	TArray<FAccelByteModelsPublicPolicy> policies;
	const EAccelByteAgreementPolicyType AgreementPolicyType = EAccelByteAgreementPolicyType::EMPTY;
	const TArray<FString> Tags = TArray<FString> { TEXT("%S"), PolicyTags[0] };

	FRegistry::Agreement.GetLegalPoliciesByCountry(AgreementTestUserInfo_.CountryCode, AgreementPolicyType, Tags, false, 
		THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda([&bGetPoliciesDone, &policies](const TArray<FAccelByteModelsPublicPolicy>& Policies) {
		policies = Policies;
		bGetPoliciesDone = true;
	}), AgreementTestErrorHandler);
	Waiting(bGetPoliciesDone, "Waiting for get policies by country...");

	AB_TEST_TRUE(policies.Num() > 0);
	AB_TEST_EQUAL(policies[0].CountryCode , AgreementTestUserInfo_.CountryCode);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AgreementGetLegalPoliciesByCountryWithUnmatchTag, "AccelByte.Tests.Agreement.C.GetLegalPoliciesByCountryWithUnmatchTag", AutomationFlagMaskAgreement);
bool AgreementGetLegalPoliciesByCountryWithUnmatchTag::RunTest(const FString& Parameters)
{
	bool bGetPoliciesDone = false;
	TArray<FAccelByteModelsPublicPolicy> policies;
	const EAccelByteAgreementPolicyType AgreementPolicyType = EAccelByteAgreementPolicyType::EMPTY;
	const TArray<FString> Tags = TArray<FString> {TEXT("tag3")};

	FRegistry::Agreement.GetLegalPoliciesByCountry(AgreementTestUserInfo_.CountryCode, AgreementPolicyType, Tags, false, 
		THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda([&bGetPoliciesDone, &policies](const TArray<FAccelByteModelsPublicPolicy>& Policies) {
		policies = Policies;
		bGetPoliciesDone = true;
	}), AgreementTestErrorHandler);
	Waiting(bGetPoliciesDone, "Waiting for get policies by country...");

	AB_TEST_EQUAL(policies.Num() , 0);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AgreementGetLegalPoliciesAndBulkAcceptPolicyVersions, "AccelByte.Tests.Agreement.D.GetLegalPoliciesAndBulkAcceptPolicyVersions", AutomationFlagMaskAgreement);
bool AgreementGetLegalPoliciesAndBulkAcceptPolicyVersions::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteAgreementTest, Log, TEXT("User creds: %s"), *FRegistry::Credentials.GetUserId());

	bool bGetPoliciesSuccess = false;
	TArray<FAccelByteModelsPublicPolicy> policies;
	const EAccelByteAgreementPolicyType AgreementPolicyType = EAccelByteAgreementPolicyType::EMPTY;

	FRegistry::Agreement.GetLegalPolicies(AgreementPolicyType, false,
		THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda([&bGetPoliciesSuccess, &policies](const TArray<FAccelByteModelsPublicPolicy>& Policies) {
		policies = Policies;
		bGetPoliciesSuccess = true;
	}), AgreementTestErrorHandler);
	Waiting(bGetPoliciesSuccess, "Waiting for get policies...");

	AB_TEST_TRUE(policies.Num() > 0);
	AB_TEST_EQUAL(policies[0].Namespace , FRegistry::Settings.Namespace);
	AB_TEST_EQUAL(policies[0].CountryCode , AgreementTestUserInfo_.CountryCode);

	TArray<FAccelByteModelsAcceptAgreementRequest> AcceptAgreementRequests;
	for (const auto& policy : policies)
	{
		for (const auto& policyVersion : policy.PolicyVersions)
		{
			if (policyVersion.IsInEffect)
			{
				FAccelByteModelsAcceptAgreementRequest AcceptRequest;
				AcceptRequest.IsAccepted = true;
				AcceptRequest.PolicyId = policy.Id;
				AcceptRequest.PolicyVersionId = policyVersion.Id;
				AcceptRequest.LocalizedPolicyVersionId = policyVersion.LocalizedPolicyVersions[0].Id;
				AcceptAgreementRequests.Add(AcceptRequest);
			}
		}
	}

	AB_TEST_TRUE(AcceptAgreementRequests.Num() > 0);

	bool bAcceptPoliciesSuccess = false;
	bool bProceed = false;
	FRegistry::Agreement.BulkAcceptPolicyVersions(AcceptAgreementRequests, 
		THandler<FAccelByteModelsAcceptAgreementResponse>::CreateLambda([&bProceed, &bAcceptPoliciesSuccess](const FAccelByteModelsAcceptAgreementResponse& Response) {
		bProceed = Response.Proceed;
		bAcceptPoliciesSuccess = true;
	}), AgreementTestErrorHandler);
	Waiting(bAcceptPoliciesSuccess, "Waiting for accept policies...");

	AB_TEST_TRUE(bProceed);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AgreementGetLegalPoliciesAndAcceptPolicyVersion, "AccelByte.Tests.Agreement.D.GetLegalPoliciesAndAcceptPolicyVersion", AutomationFlagMaskAgreement);
bool AgreementGetLegalPoliciesAndAcceptPolicyVersion::RunTest(const FString& Parameters)
{
	UE_LOG(LogAccelByteAgreementTest, Log, TEXT("User creds: %s"), *FRegistry::Credentials.GetUserId());

	bool bGetPoliciesSuccess = false;
	TArray<FAccelByteModelsPublicPolicy> policies;
	const EAccelByteAgreementPolicyType AgreementPolicyType = EAccelByteAgreementPolicyType::EMPTY;

	FRegistry::Agreement.GetLegalPolicies(AgreementPolicyType, 
		false,
		THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda([&bGetPoliciesSuccess, &policies](const TArray<FAccelByteModelsPublicPolicy>& Policies) 
	{
		policies = Policies;
		bGetPoliciesSuccess = true;
	}), AgreementTestErrorHandler);
	Waiting(bGetPoliciesSuccess, "Waiting for get policies...");

	FString LocalizedPolicyVersionId;
	for (const auto& policy : policies)
	{
		for (const auto& policyVersion : policy.PolicyVersions)
		{
			if (policyVersion.IsInEffect)
			{
				LocalizedPolicyVersionId = policyVersion.LocalizedPolicyVersions[0].Id;
				break;
			}
		}
	}

	AB_TEST_FALSE(LocalizedPolicyVersionId.IsEmpty());

	bool bAcceptPolicySuccess = false;
	FRegistry::Agreement.AcceptPolicyVersion(LocalizedPolicyVersionId,
		FVoidHandler::CreateLambda([&bAcceptPolicySuccess]() 
	{
		bAcceptPolicySuccess = true;
	}), AgreementTestErrorHandler);
	Waiting(bAcceptPolicySuccess, "Waiting for accept policies...");

	AB_TEST_TRUE(bAcceptPolicySuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AgreementGetPublisherLegalPoliciesAndGetTheContent, "AccelByte.Tests.Agreement.D.GetPublisherLegalPoliciesAndGetTheContent", AutomationFlagMaskAgreement)
bool AgreementGetPublisherLegalPoliciesAndGetTheContent::RunTest(const FString& Parameters) 
{
	UE_LOG(LogAccelByteAgreementTest, Log, TEXT("User creds: %s"), *FRegistry::Credentials.GetUserId());
	bool bGetPoliciesSuccess = false;
	TArray<FAccelByteModelsPublicPolicy> policies;
	const EAccelByteAgreementPolicyType AgreementPolicyType = EAccelByteAgreementPolicyType::EMPTY;

	UE_LOG(LogAccelByteAgreementTest, Log, TEXT("namespaces: %s"), *GetPublisherNamespace());
	FRegistry::Agreement.GetLegalPolicies(GetPublisherNamespace(), AgreementPolicyType, true,
		THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda([&bGetPoliciesSuccess, &policies](const TArray<FAccelByteModelsPublicPolicy>& Policies) 
	{
		policies = Policies;
		bGetPoliciesSuccess = true;
	}), AgreementTestErrorHandler);
	
	Waiting(bGetPoliciesSuccess, "Waiting for get policies...");
	FString LocalizedPolicyUrl;
	for (const auto& policy : policies) 
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

	FString result;
	bool bGetLegalDocSuccess = false;
	FRegistry::Agreement.GetLegalDocument(LocalizedPolicyUrl, THandler<FString>::CreateLambda([&bGetLegalDocSuccess, &result](const FString& Result) 
	{
		bGetLegalDocSuccess = true;
		result = Result;
	}), AgreementTestErrorHandler);
	Waiting(bGetLegalDocSuccess, "Waiting for get legal docs...");
	
	AB_TEST_TRUE(bGetLegalDocSuccess);
	AB_TEST_FALSE(result.IsEmpty());

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AgreementCheckUserEligibilitiesNotComply, "AccelByte.Tests.Agreement.B.CheckUserEligibilitiesNotComply", AutomationFlagMaskAgreement)
bool AgreementCheckUserEligibilitiesNotComply::RunTest(const FString& Parameters) 
{
	bool bUserLoginSuccess = false;
	FRegistry::User.LoginWithUsername(AgreementTestUserInfo_.EmailAddress, 
		AgreementTestUserInfo_.Password,
		FVoidHandler::CreateLambda([&bUserLoginSuccess]()
	{
		AgreementTestUserInfo_.UserId = FRegistry::Credentials.GetUserId();
		bUserLoginSuccess = true;
	}), AgreementTestErrorHandler);
	Waiting(bUserLoginSuccess, "Waiting for login...");
	
	TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse> EligibilitiesResult;
	bool bGetEligibilitiesSuccess = false;

	FRegistry::Agreement.QueryLegalEligibilities(FRegistry::Settings.Namespace, 
		THandler<TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>>::CreateLambda([&bGetEligibilitiesSuccess, &EligibilitiesResult](const TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>& Result) 
	{
		bGetEligibilitiesSuccess = true;
		EligibilitiesResult = Result;
	}), AgreementTestErrorHandler);
	Waiting(bGetEligibilitiesSuccess, "Waiting for get eligibilities...");
	
	bool bIsComply = false;
	for (const auto& eligibility : EligibilitiesResult) 
	{
		if (eligibility.PolicyId.Equals(PolicyId)) 
		{
			if (eligibility.IsAccepted)
			{
				bIsComply = true;
			}
			break;
		}
	}
	AB_TEST_TRUE(bGetEligibilitiesSuccess);
	AB_TEST_FALSE(bIsComply);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AgreementCheckUserEligibilitiesAlreadyComply, "AccelByte.Tests.Agreement.E.CheckUserEligibilitiesAlreadyComply", AutomationFlagMaskAgreement)
bool AgreementCheckUserEligibilitiesAlreadyComply::RunTest(const FString& Parameters)
{
	TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse> EligibilitiesResult;
	bool bGetEligibilitiesSuccess = false;

	FRegistry::Agreement.QueryLegalEligibilities(FRegistry::Settings.Namespace,
		THandler<TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>>::CreateLambda([&bGetEligibilitiesSuccess, &EligibilitiesResult](const TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>& Result) 
	{
		bGetEligibilitiesSuccess = true;
		EligibilitiesResult = Result;
	}), AgreementTestErrorHandler);
	Waiting(bGetEligibilitiesSuccess, "Waiting for get eligibilities...");

	bool bIsComply = false;
	for (const auto& eligibility : EligibilitiesResult)
	{
		if (eligibility.PolicyId.Equals(PolicyId))
		{
			if (eligibility.IsAccepted)
			{
				bIsComply = true;
			}
			break;
		}
	}
	AB_TEST_TRUE(bGetEligibilitiesSuccess);
	AB_TEST_TRUE(bIsComply);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(AgreementGetLegalPoliciesByCountryWithoutTagAndRegiterUser, "AccelByte.Tests.Agreement.F.AgreementGetLegalPoliciesByCountryWithoutTagAndRegiterUser", AutomationFlagMaskAgreement);
bool AgreementGetLegalPoliciesByCountryWithoutTagAndRegiterUser::RunTest(const FString& Parameters)
{
	bool bGetPoliciesDone = false;
	TArray<FAccelByteModelsPublicPolicy> policies;
	EAccelByteAgreementPolicyType AgreementPolicyType = EAccelByteAgreementPolicyType::EMPTY;

	FRegistry::Agreement.GetLegalPoliciesByCountry(AgreementTestUserInfo_.CountryCode, AgreementPolicyType, false,
		THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda([&bGetPoliciesDone, &policies](const TArray<FAccelByteModelsPublicPolicy>& Policies) {
			policies = Policies;
			bGetPoliciesDone = true;
			}), AgreementTestErrorHandler);
	Waiting(bGetPoliciesDone, "Waiting for get policies by country...");

	AB_TEST_TRUE(policies.Num() > 0);
	AB_TEST_EQUAL(policies[0].CountryCode , AgreementTestUserInfo_.CountryCode);

	TArray<FAcceptedPolicies> AcceptAgreementRequests;
	for (const auto& policy : policies)
	{
		for (const auto& policyVersion : policy.PolicyVersions)
		{
			if (policyVersion.IsInEffect)
			{
				FAcceptedPolicies AcceptRequest;
				AcceptRequest.IsAccepted = true;
				AcceptRequest.PolicyId = policy.Id;
				AcceptRequest.PolicyVersionId = policyVersion.Id;
				AcceptRequest.LocalizedPolicyVersionId = policyVersion.LocalizedPolicyVersions[0].Id;
				AcceptAgreementRequests.Add(AcceptRequest);
			}
		}
	}

	AB_TEST_TRUE(AcceptAgreementRequests.Num() > 0);

	// User Deletion
	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;

	UE_LOG(LogAccelByteAgreementTest, Log, TEXT("Delete User By ID"));
	User_Delete_By_Email_Address(AgreementTestUserInfo_.EmailAddress, FSimpleDelegate::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
	{
		UE_LOG(LogAccelByteAgreementTest, Log, TEXT("Success Delete User By Email"));
		bDeleteSuccessful = true;
		bDeleteDone = true;
	}), AgreementTestErrorHandler);
	Waiting(bDeleteDone, "Waiting for deletion...");

	AB_TEST_TRUE(bDeleteSuccessful);

	// User Creation
	UE_LOG(LogAccelByteAgreementTest, Log, TEXT("Agreement user creation ..."));

	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 21 + 7));
	const FString DoB_String = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

	FRegisterRequestv3 NewUserRequest;
	NewUserRequest.AcceptedPolicies = AcceptAgreementRequests;
	NewUserRequest.DisplayName = AgreementTestUserInfo_.DisplayName;
	NewUserRequest.Password = AgreementTestUserInfo_.Password;
	NewUserRequest.EmailAddress = AgreementTestUserInfo_.EmailAddress;
	NewUserRequest.Username = "SDKTestNewRegister";
	NewUserRequest.AuthType = TEXT("EMAILPASSWD");
	NewUserRequest.Country = AgreementTestUserInfo_.CountryCode;
	NewUserRequest.DateOfBirth = DoB_String;

	bool bUsersCreationDone = false;
	bool bUserCreated = false;
	FRegistry::User.Registerv3(NewUserRequest, THandler<FRegisterResponse>::CreateLambda([&bUserCreated, &bUsersCreationDone](const FRegisterResponse& Response)
		{
			bUserCreated = true;
			UE_LOG(LogAccelByteAgreementTest, Log, TEXT("Test User is successfuly created."));
			bUsersCreationDone = true;
		}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
			{
				if (static_cast<ErrorCodes>(Code) == ErrorCodes::UserEmailAlreadyUsedException)
				{
					bUserCreated = true;
					UE_LOG(LogAccelByteAgreementTest, Log, TEXT("Test User is already created."));
				}
				else
				{
					UE_LOG(LogAccelByteAgreementTest, Log, TEXT("Test User can't be created"));
				}
				bUsersCreationDone = true;
			}));
	Waiting(bUsersCreationDone, "Waiting for user created...");

	//User Loging in
	UE_LOG(LogAccelByteAgreementTest, Log, TEXT("Agreement user login..."));

	bool bUserLoggedIn = false;
	FRegistry::User.LoginWithUsername(
		AgreementTestUserInfo_.EmailAddress,
		AgreementTestUserInfo_.Password,
		FVoidHandler::CreateLambda([&bUserLoggedIn]()
		{
			AgreementTestUserInfo_.UserId = FRegistry::Credentials.GetUserId();
			bUserLoggedIn = true;
			UE_LOG(LogAccelByteAgreementTest, Log, TEXT("Successfully Login."));
		}), AgreementTestErrorHandler);
	Waiting(bUserLoggedIn, "Waiting for login with user name...");
	UE_LOG(LogAccelByteAgreementTest, Log, TEXT("User creds: %s"), *FRegistry::Credentials.GetUserId());

	AB_TEST_TRUE(bUserCreated);
	AB_TEST_TRUE(bUserLoggedIn);

	return true;
}