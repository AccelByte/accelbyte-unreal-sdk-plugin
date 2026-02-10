// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#include "Blueprints/ABAgreement.h"

using namespace AccelByte;

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
	const auto AgreementPtr = ApiClientPtr->GetAgreementApi().Pin();
	if (AgreementPtr.IsValid())
	{
		AgreementPtr->GetLegalPolicies(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAgreement::GetLegalPoliciesByNamespace(
	FString const& Namespace,
	EAccelByteAgreementPolicyType const& AgreementPolicyType,
	bool DefaultOnEmpty,
	FDArrayModelsPublicPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto AgreementPtr = ApiClientPtr->GetAgreementApi().Pin();
	if (AgreementPtr.IsValid())
	{
		AgreementPtr->GetLegalPolicies(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAgreement::GetLegalPoliciesByTags(
	EAccelByteAgreementPolicyType const& AgreementPolicyType,
	TArray<FString> const& tags,
	bool DefaultOnEmpty,
	FDArrayModelsPublicPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto AgreementPtr = ApiClientPtr->GetAgreementApi().Pin();
	if (AgreementPtr.IsValid())
	{
		AgreementPtr->GetLegalPolicies(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAgreement::GetLegalPoliciesByCountry(
	FString const& CountryCode,
	EAccelByteAgreementPolicyType const& AgreementPolicyType,
	bool DefaultOnEmpty,
	FDArrayModelsPublicPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto AgreementPtr = ApiClientPtr->GetAgreementApi().Pin();
	if (AgreementPtr.IsValid())
	{
		AgreementPtr->GetLegalPoliciesByCountry(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAgreement::GetLegalPoliciesByCountryAndTags(
	FString const& CountryCode,
	EAccelByteAgreementPolicyType const& AgreementPolicyType,
	TArray<FString> const& Tags,
	bool DefaultOnEmpty,
	FDArrayModelsPublicPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto AgreementPtr = ApiClientPtr->GetAgreementApi().Pin();
	if (AgreementPtr.IsValid())
	{
		AgreementPtr->GetLegalPoliciesByCountry(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAgreement::GetLegalPoliciesByNamespaceAndCountry(
    FString const &Namespace,
	FString const& CountryCode,
	EAccelByteAgreementPolicyType const& AgreementPolicyType,
	bool DefaultOnEmpty,
	FDArrayModelsPublicPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto AgreementPtr = ApiClientPtr->GetAgreementApi().Pin();
	if (AgreementPtr.IsValid())
	{
		AgreementPtr->GetLegalPoliciesByNamespaceAndCountry(
			Namespace,
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAgreement::GetLegalPoliciesByNamespaceCountryAndTags(
    FString const &Namespace,
	FString const& CountryCode,
	EAccelByteAgreementPolicyType const& AgreementPolicyType,
	TArray<FString> const& Tags,
	bool DefaultOnEmpty,
	FDArrayModelsPublicPolicyResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto AgreementPtr = ApiClientPtr->GetAgreementApi().Pin();
	if (AgreementPtr.IsValid())
	{
		AgreementPtr->GetLegalPoliciesByNamespaceAndCountry(
			Namespace,
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAgreement::BulkAcceptPolicyVersions(
	FArrayModelsAcceptAgreementRequest const& AgreementRequests,
	FDModelsAcceptAgreementResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto AgreementPtr = ApiClientPtr->GetAgreementApi().Pin();
	if (AgreementPtr.IsValid())
	{
		AgreementPtr->BulkAcceptPolicyVersions(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAgreement::AcceptPolicyVersion(
	FString const& LocalizedPolicyVersionId,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto AgreementPtr = ApiClientPtr->GetAgreementApi().Pin();
	if (AgreementPtr.IsValid())
	{
		AgreementPtr->AcceptPolicyVersion(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAgreement::ChangePolicyPreferences(
	TArray<FAccelByteModelsChangeAgreementRequest> const& ChangeAgreementRequests,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto AgreementPtr = ApiClientPtr->GetAgreementApi().Pin();
	if (AgreementPtr.IsValid())
	{
		AgreementPtr->ChangePolicyPreferences(
			ChangeAgreementRequests,
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAgreement::QueryLegalEligibilities(
	FString const& Namespace,
	FDArrayModelsRetrieveUserEligibilitiesResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto AgreementPtr = ApiClientPtr->GetAgreementApi().Pin();
	if (AgreementPtr.IsValid())
	{
		AgreementPtr->QueryLegalEligibilities(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABAgreement::GetLegalDocument(
	FString const& Url,
	FDString const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto AgreementPtr = ApiClientPtr->GetAgreementApi().Pin();
	if (AgreementPtr.IsValid())
	{
		AgreementPtr->GetLegalDocument(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}