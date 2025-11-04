// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteAgreementModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Agreement API to obtain and accept legal in a namespace.
 */
class ACCELBYTEUE4SDK_API Agreement 
	: public FApiBase
	, public TSharedFromThis<Agreement, ESPMode::ThreadSafe>
{
public:
	Agreement(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient = nullptr);

	Agreement(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);

	virtual ~Agreement();

	/**
	 * @brief Retrieve all active latest policies based on a namespace and country. The namespace is current client namespace. 
	 *			The country will be read from user token.
	 *
	 * @param AgreementPolicyType Filter the responded policy by policy type. Choose the EAccelByteAgreementPolicyType::EMPTY 
	 *			if you want to be responded with all policy type.
	 * @param DefaultOnEmpty Specify with true if you want to be responded with default country-specific policy if 
	 *			your requested country is not exist.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a TArray<FAccelByteModelsPublicPolicy>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetLegalPolicies(EAccelByteAgreementPolicyType const& AgreementPolicyType
		, bool DefaultOnEmpty
		, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Retrieve all active latest policies based on a namespace and country. The namespace can be filled with namespace or publisher namespace. The country will be read from user token.
	 *
	 * @param Namespace Filter the responded policy by namespace.
	 * @param AgreementPolicyType Filter the responded policy by policy type. Choose the EAccelByteAgreementPolicyType::EMPTY 
	 *			if you want to be responded with all policy type.
	 * @param DefaultOnEmpty Specify with true if you want to be responded with default country-specific policy if your 
	 *			requested country is not exist.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a TArray<FAccelByteModelsPublicPolicy>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetLegalPolicies(FString const& Namespace
		, EAccelByteAgreementPolicyType const& AgreementPolicyType
		, bool DefaultOnEmpty
		, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Retrieve all active latest policies based on a namespace and country. The namespace is current client namespace. 
	 *			The country will be read from user token.
	 *
	 * @param AgreementPolicyType Filter the responded policy by policy type. Choose the EAccelByteAgreementPolicyType::EMPTY 
	 *			if you want to be responded with all policy type.
	 * @param Tags Filter the responded policy by tags.
	 * @param DefaultOnEmpty Specify with true if you want to be responded with default country-specific policy if your 
	 *			requested country is not exist.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a TArray<FAccelByteModelsPublicPolicy>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetLegalPolicies(EAccelByteAgreementPolicyType const& AgreementPolicyType
		, TArray<FString> const& Tags
		, bool DefaultOnEmpty
		, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Retrieve all active latest policies based on a country.
	 *
	 * @param CountryCode Requested policy country code.
	 * @param AgreementPolicyType Filter the responded policy by policy type. Choose the EAccelByteAgreementPolicyType::EMPTY 
	 *			if you want to be responded with all policy type.
	 * @param DefaultOnEmpty Specify with true if you want to be responded with default country-specific policy if your 
	 *			requested country is not exist.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a TArray<FAccelByteModelsPublicPolicy>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetLegalPoliciesByCountry(FString const& CountryCode
		, EAccelByteAgreementPolicyType const& AgreementPolicyType
		, bool DefaultOnEmpty
		, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Retrieve all active latest policies based on a country.
	 *
	 * @param CountryCode Requested policy country code.
	 * @param AgreementPolicyType Filter the responded policy by policy type. Choose the EAccelByteAgreementPolicyType::EMPTY 
	 *			if you want to be responded with all policy type.
	 * @param Tags Filter the responded policy by tags.
	 * @param DefaultOnEmpty Specify with true if you want to be responded with default country-specific policy if your 
	 *			requested country is not exist.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a TArray<FAccelByteModelsPublicPolicy>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetLegalPoliciesByCountry(FString const& CountryCode
		, EAccelByteAgreementPolicyType const& AgreementPolicyType
		, TArray<FString> const& Tags
		, bool DefaultOnEmpty
		, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Retrieve all active latest policies based on a country and namespace.
	 *
	 * @param Namespace Requested namespace.
	 * @param CountryCode Requested policy country code.
	 * @param AgreementPolicyType Filter the responded policy by policy type. Choose the EAccelByteAgreementPolicyType::EMPTY 
	 *			if you want to be responded with all policy type.
	 * @param DefaultOnEmpty Specify with true if you want to be responded with default country-specific policy if your 
	 *			requested country is not exist.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a TArray<FAccelByteModelsPublicPolicy>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetLegalPoliciesByNamespaceAndCountry(FString const& Namespace
		, FString const& CountryCode
		, EAccelByteAgreementPolicyType const& AgreementPolicyType
		, bool DefaultOnEmpty
		, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Retrieve all active latest policies based on a country and namespace.
	 *
	 * @param Namespace Requested namespace.
	 * @param CountryCode Requested policy country code.
	 * @param AgreementPolicyType Filter the responded policy by policy type. Choose the EAccelByteAgreementPolicyType::EMPTY 
	 *			if you want to be responded with all policy type.
	 * @param Tags Filter the responded policy by tags.
	 * @param DefaultOnEmpty Specify with true if you want to be responded with default country-specific policy if your 
	 *			requested country is not exist.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a TArray<FAccelByteModelsPublicPolicy>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetLegalPoliciesByNamespaceAndCountry(FString const& Namespace
		, FString const& CountryCode
		, EAccelByteAgreementPolicyType const& AgreementPolicyType
		, TArray<FString> const& Tags
		, bool DefaultOnEmpty
		, THandler<TArray<FAccelByteModelsPublicPolicy>> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Accepts many legal policy versions all at once. Supply with localized version policy id to accept an agreement.
	 *
	 * @param AgreementRequests List of localized policy versions to accept.
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsAcceptAgreementResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkAcceptPolicyVersions(TArray<FAccelByteModelsAcceptAgreementRequest> const& AgreementRequests
		, THandler<FAccelByteModelsAcceptAgreementResponse> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Retrieve accepted Legal Agreements.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsAcceptAgreementResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RetrieveAcceptedPolicies(THandler<TArray<FAccelByteModelsRetrieveAcceptedAgreementResponse>> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Accepts a legal policy version. Supply with localized version policy id to accept an agreement.
	 *
	 * @param LocalizedPolicyVersionId A localized policy version Id to accept.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr AcceptPolicyVersion(FString const& LocalizedPolicyVersionId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Accept or changes preference to specific legal policy versions.
	 *
	 * @param ChangeAgreementRequests Request for localized policy version id to change.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 *
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ChangePolicyPreferences(TArray<FAccelByteModelsChangeAgreementRequest> const& ChangeAgreementRequests
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query all player's legal eligibilities on a namespace, use to check is player already commited to legal or not.
	 *
	 * @param Namespace Filter the eligibilities by namespace.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a TArray<RetrieveUserEligibilitiesResponse>.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryLegalEligibilities(FString const& Namespace
		, THandler<TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get the content of the legal document.
	 *
	 * @param Url The url of the legal document, you can get it from GetLegalPolicies query.
	 * @param OnSuccess This will be called when the operation succeeded. The result is a string.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetLegalDocument(FString const& Url
		, THandler<FString> const& OnSuccess
		, FErrorHandler const& OnError);

private:
	Agreement() = delete;
	Agreement(Agreement const&) = delete;
	Agreement(Agreement&&) = delete;

	static FString ConvertAgreementPolicyType(EAccelByteAgreementPolicyType const& AgreementPolicyType);
};

using AgreementRef = TSharedRef<Agreement, ESPMode::ThreadSafe>;
using AgreementPtr = TSharedPtr<Agreement, ESPMode::ThreadSafe>;
using AgreementWPtr = TWeakPtr<Agreement, ESPMode::ThreadSafe>;

} // Namespace Api
} // Namespace AccelByte
