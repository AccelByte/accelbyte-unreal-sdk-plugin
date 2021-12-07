// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from Accelbyte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABAgreement.h"

void UABAgreement::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABAgreement::GetLegalPolicies(
	EAccelByteAgreementPolicyType const& AgreementPolicyType,
	bool DefaultOnEmpty,
	FDArrayModelsPublicPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->Agreement.GetLegalPolicies(
		AgreementPolicyType,
		DefaultOnEmpty,
		THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsPublicPolicy> const& Response)
			{
				FArrayModelsPublicPolicyResponse Result;
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

void UABAgreement::GetLegalPoliciesByNamespace(
	FString const& Namespace,
	EAccelByteAgreementPolicyType const& AgreementPolicyType,
	bool DefaultOnEmpty,
	FDArrayModelsPublicPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->Agreement.GetLegalPolicies(
		Namespace,
		AgreementPolicyType,
		DefaultOnEmpty,
		THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsPublicPolicy> const& Response)
			{
				FArrayModelsPublicPolicyResponse Result;
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

void UABAgreement::GetLegalPoliciesByTags(
	EAccelByteAgreementPolicyType const& AgreementPolicyType,
	TArray<FString> const& tags,
	bool DefaultOnEmpty,
	FDArrayModelsPublicPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->Agreement.GetLegalPolicies(
		AgreementPolicyType,
		tags,
		DefaultOnEmpty,
		THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsPublicPolicy> const& Response)
			{
				FArrayModelsPublicPolicyResponse Result;
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

void UABAgreement::GetLegalPoliciesByCountry(
	FString const& CountryCode,
	EAccelByteAgreementPolicyType const& AgreementPolicyType,
	bool DefaultOnEmpty,
	FDArrayModelsPublicPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->Agreement.GetLegalPoliciesByCountry(
		CountryCode,
		AgreementPolicyType,
		DefaultOnEmpty,
		THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsPublicPolicy> const& Response)
			{
				FArrayModelsPublicPolicyResponse Result;
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

void UABAgreement::GetLegalPoliciesByCountryAndTags(
	FString const& CountryCode,
	EAccelByteAgreementPolicyType const& AgreementPolicyType,
	TArray<FString> const& Tags,
	bool DefaultOnEmpty,
	FDArrayModelsPublicPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->Agreement.GetLegalPoliciesByCountry(
		CountryCode,
		AgreementPolicyType,
		Tags,
		DefaultOnEmpty,
		THandler<TArray<FAccelByteModelsPublicPolicy>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsPublicPolicy> const& Response)
			{
				FArrayModelsPublicPolicyResponse Result;
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

void UABAgreement::BulkAcceptPolicyVersions(
	FArrayModelsAcceptAgreementRequest const& AgreementRequests,
	FDModelsAcceptAgreementResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->Agreement.BulkAcceptPolicyVersions(
		AgreementRequests.Content,
		THandler<FAccelByteModelsAcceptAgreementResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsAcceptAgreementResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABAgreement::AcceptPolicyVersion(
	FString const& LocalizedPolicyVersionId,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->Agreement.AcceptPolicyVersion(
		LocalizedPolicyVersionId,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABAgreement::QueryLegalEligibilities(
	FString const& Namespace,
	FDArrayModelsRetrieveUserEligibilitiesResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->Agreement.QueryLegalEligibilities(
		Namespace,
		THandler<TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse> const& Response)
			{
				FArrayModelsRetrieveUserEligibilitiesResponse Result;
				Result.Content = Response;
				OnSuccess.ExecuteIfBound(Result);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABAgreement::GetLegalDocument(
	FString const& Url,
	FDString const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->Agreement.GetLegalDocument(
		Url,
		THandler<FString>::CreateLambda(
			[OnSuccess](FString const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}