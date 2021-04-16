// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/AccelByteError.h"
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
{
public:
	Agreement(const Credentials& Credentials, const Settings& Settings);
	~Agreement();
private:
	const Credentials& Credentials;
	const Settings& Settings;
public:
	/**
	* @brief Retrieve all active latest policies based on a namespace and country. The namespace is current client namespace. The country will be read from user token.
	*
	* @param AgreementPolicyType Filter the responded policy by policy type. Choose the EAccelByteAgreementPolicyType::EMPTY if you want to be responded with all policy type.
	* @param DefaultOnEmpty Specify with true if you want to be responded with default country-specific policy if your requested country is not exist.
	* @param OnSuccess This will be called when the operation succeeded. The result is a TArray<FAccelByteModelsPublicPolicy>.
	* @param OnError This will be called when the operation failed.
	*/
	void GetLegalPolicies(const EAccelByteAgreementPolicyType& AgreementPolicyType, bool DefaultOnEmpty, const THandler<TArray<FAccelByteModelsPublicPolicy>>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Retrieve all active latest policies based on a namespace and country. The namespace can be filled with namespace or publisher namespace. The country will be read from user token.
	*
	* @param Namespace Filter the responded policy by namespace.
	* @param AgreementPolicyType Filter the responded policy by policy type. Choose the EAccelByteAgreementPolicyType::EMPTY if you want to be responded with all policy type.
	* @param DefaultOnEmpty Specify with true if you want to be responded with default country-specific policy if your requested country is not exist.
	* @param OnSuccess This will be called when the operation succeeded. The result is a TArray<FAccelByteModelsPublicPolicy>.
	* @param OnError This will be called when the operation failed.
	*/
	void GetLegalPolicies(const FString& Namespace, const EAccelByteAgreementPolicyType& AgreementPolicyType, bool DefaultOnEmpty, const THandler<TArray<FAccelByteModelsPublicPolicy>>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Retrieve all active latest policies based on a namespace and country. The namespace is current client namespace. The country will be read from user token.
	*
	* @param AgreementPolicyType Filter the responded policy by policy type. Choose the EAccelByteAgreementPolicyType::EMPTY if you want to be responded with all policy type.
	* @param Tags Filter the responded policy by tags.
	* @param DefaultOnEmpty Specify with true if you want to be responded with default country-specific policy if your requested country is not exist.
	* @param OnSuccess This will be called when the operation succeeded. The result is a TArray<FAccelByteModelsPublicPolicy>.
	* @param OnError This will be called when the operation failed.
	*/
	void GetLegalPolicies(const EAccelByteAgreementPolicyType& AgreementPolicyType, const TArray<FString>& Tags, bool DefaultOnEmpty, const THandler<TArray<FAccelByteModelsPublicPolicy>>& OnSuccess, const FErrorHandler& OnError);
	
	/**
	* @brief Retrieve all active latest policies based on a country.
	*
	* @param CountryCode Requested policy country code.
	* @param AgreementPolicyType Filter the responded policy by policy type. Choose the EAccelByteAgreementPolicyType::EMPTY if you want to be responded with all policy type.
	* @param DefaultOnEmpty Specify with true if you want to be responded with default country-specific policy if your requested country is not exist.
	* @param OnSuccess This will be called when the operation succeeded. The result is a TArray<FAccelByteModelsPublicPolicy>.
	* @param OnError This will be called when the operation failed.
	*/
	void GetLegalPoliciesByCountry(const FString& CountryCode, const EAccelByteAgreementPolicyType& AgreementPolicyType, bool DefaultOnEmpty, const THandler<TArray<FAccelByteModelsPublicPolicy>>& OnSuccess, const FErrorHandler& OnError);
	
	/**
	* @brief Retrieve all active latest policies based on a country.
	*
	* @param CountryCode Requested policy country code.
	* @param AgreementPolicyType Filter the responded policy by policy type. Choose the EAccelByteAgreementPolicyType::EMPTY if you want to be responded with all policy type.
	* @param Tags Filter the responded policy by tags.
	* @param DefaultOnEmpty Specify with true if you want to be responded with default country-specific policy if your requested country is not exist.
	* @param OnSuccess This will be called when the operation succeeded. The result is a TArray<FAccelByteModelsPublicPolicy>.
	* @param OnError This will be called when the operation failed.
	*/
	void GetLegalPoliciesByCountry(const FString& CountryCode, const EAccelByteAgreementPolicyType& AgreementPolicyType, const TArray<FString>& Tags, bool DefaultOnEmpty, const THandler<TArray<FAccelByteModelsPublicPolicy>>& OnSuccess, const FErrorHandler& OnError);
	
	/**
	* @brief Accepts many legal policy versions all at once. Supply with localized version policy id to accept an agreement.
	*
	* @param AgreementRequests List of localized policy versions to accept.
	* @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsAcceptAgreementResponse.
	* @param OnError This will be called when the operation failed.
	*/
	void BulkAcceptPolicyVersions(const TArray<FAccelByteModelsAcceptAgreementRequest>& AgreementRequests, const THandler<FAccelByteModelsAcceptAgreementResponse>& OnSuccess, const FErrorHandler& OnError);
	
	/**
	* @brief Accepts many legal policy versions all at once. Supply with localized version policy id to accept an agreement.
	* This endpoint used by APIGateway during new user registration and user login.
	* 
	* @param AgreementRequests List of localized policy versions to accept.
	* @param OnSuccess This will be called when the operation succeeded. The result is an FAccelByteModelsAcceptAgreementResponse.
	* @param OnError This will be called when the operation failed.
	*/
	void BulkAcceptPolicyVersionsIndirect(const TArray<FAccelByteModelsAcceptAgreementRequest>& AgreementRequests, const THandler<FAccelByteModelsAcceptAgreementResponse>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Accepts a legal policy version. Supply with localized version policy id to accept an agreement.
	*
	* @param LocalizedPolicyVersionId A localized policy version Id to accept.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed.
	*/
	void AcceptPolicyVersion(const FString& LocalizedPolicyVersionId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Query all player's legal eligibilities on a namespace, use to check is player already commited to legal or not.
	*
	* @param Namespace Filter the eligibilities by namespace.
	* @param OnSuccess This will be called when the operation succeeded. The result is a TArray<RetrieveUserEligibilitiesResponse>.
	* @param OnError This will be called when the operation failed.
	*/
	void QueryLegalEligibilities(const FString& Namespace, const THandler<TArray<FAccelByteModelsRetrieveUserEligibilitiesResponse>>& OnSuccess, const FErrorHandler& OnError);

	/*
	* @brief Get the content of the legal document.
	*
	* @param Url The url of the legal document, you can get it from GetLegalPolicies query.
	* @param OnSuccess This will be called when the operation succeeded. The result is a string.
	* @param OnError This will be called when the operation failed.
	*/
	void GetLegalDocument(const FString& Url, const THandler<FString>& OnSuccess, const FErrorHandler& OnError);
private:
	Agreement() = delete;
	Agreement(Agreement const&) = delete;
	Agreement(Agreement&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
