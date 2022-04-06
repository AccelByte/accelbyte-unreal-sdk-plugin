// Copyright (c) 2022 AccelByte, Inc. All rights reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager

#include "AgreementTestAdminBP.h"

void UAgreementTestAdmin::AdminCreateAgreementBasePolicyBP(
	FAgreementBasePolicyCreate const& CreateRequest,
	FDAgreementBasePolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	AdminCreateAgreementBasePolicy(
		CreateRequest, 
		THandler<FAgreementBasePolicy>::CreateLambda(
			[OnSuccess](FAgreementBasePolicy const& Result)
			{
				OnSuccess.ExecuteIfBound(Result);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UAgreementTestAdmin::AdminCreateAgreementPolicyVersionBP(
	FString const& PolicyId,
	FAgreementPolicyVersionCreate const& CreateRequest,
	FDAgreementPolicyVersionResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	AdminCreateAgreementPolicyVersion(
		PolicyId, 
		CreateRequest, 
		THandler<FAgreementPolicyVersion>::CreateLambda(
			[OnSuccess](FAgreementPolicyVersion const& Result) 
			{
				OnSuccess.ExecuteIfBound(Result);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UAgreementTestAdmin::AdminCreateAgreementLocalizedPolicyBP(
	FString const& PolicyVersionId,
	FAgreementLocalizedPolicyCreate const& CreateRequest,
	FDAgreementLocalizedPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	AdminCreateAgreementLocalizedPolicy(
		PolicyVersionId,
		CreateRequest,
		THandler<FAgreementLocalizedPolicy>::CreateLambda(
			[OnSuccess](FAgreementLocalizedPolicy const& Result)
			{
				OnSuccess.ExecuteIfBound(Result);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UAgreementTestAdmin::AdminPublishAgreementPolicyVersionBP(
	FString const& PolicyVersionId,
	bool ShouldNotify,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	AdminPublishAgreementPolicyVersion(
		PolicyVersionId,
		ShouldNotify,
		FSimpleDelegate::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UAgreementTestAdmin::AdminGetAgreementBasePoliciesBP(
	FDArrayAgreementBasePolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	AdminGetAgreementBasePolicies(
		THandler<TArray<FAgreementBasePolicy>>::CreateLambda(
			[OnSuccess](TArray<FAgreementBasePolicy> const& Response)
			{
				FArrayAgreementBasePolicy Result;
				Result.Content = Response;
				OnSuccess.ExecuteIfBound(Result);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UAgreementTestAdmin::AdminGetAgreementCountryBasePolicyBP(
	FString const& PolicyId,
	FString const& CountryCode,
	FDAgreementCountryPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	AdminGetAgreementCountryBasePolicy(
		PolicyId,
		CountryCode,
		THandler<FAgreementCountryPolicy>::CreateLambda(
			[OnSuccess](FAgreementCountryPolicy const& Result)
			{
				OnSuccess.ExecuteIfBound(Result);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UAgreementTestAdmin::AdminGetAgreementPolicyTypesBP(
	FDArrayAgreementPolicyTypeObjectResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	AdminGetAgreementPolicyTypes(
		THandler<TArray<FAgreementPolicyTypeObject>>::CreateLambda(
			[OnSuccess](TArray<FAgreementPolicyTypeObject> const& Response)
			{
				FArrayAgreementPolicyTypeObject Result;
				Result.Content = Response;
				OnSuccess.ExecuteIfBound(Result);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UAgreementTestAdmin::AdminGetAgreementLocalizedPoliciesBP(
	FString const& PolicyVersionId,
	FDArrayAgreementLocalizedPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	AdminGetAgreementLocalizedPolicies(
		PolicyVersionId,
		THandler<TArray<FAgreementLocalizedPolicy>>::CreateLambda(
			[OnSuccess](TArray<FAgreementLocalizedPolicy> const& Response)
			{
				FArrayAgreementLocalizedPolicy Result;
				Result.Content = Response;
				OnSuccess.ExecuteIfBound(Result);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}
