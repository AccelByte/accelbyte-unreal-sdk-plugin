// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteReportingModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
* @brief Reporting API for Reporting content.
*/
class ACCELBYTEUE4SDK_API Reporting
{
public:
	Reporting(Credentials const& CredentialsRef, Settings const& SettingsRef, FHttpRetryScheduler& HttpRef);
	~Reporting();

private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;

public:
	/**
	 * @brief Submit Report.
	 *
	 * @param ReportData contain target report and evidence
	 * @param OnSuccess This will be called when the operation succeeded. The response is a FAccelByteModelsReportingSubmitResponse.
	 * @param OnError This will be called when the operation failed.
	*/
	void SubmitReport(const FAccelByteModelsReportingSubmitData ReportData, const THandler<FAccelByteModelsReportingSubmitResponse>& OnSuccess, const FErrorHandler& OnError);
		
	/**
	 * @brief Get Reason under specified reason group.
	 *
	 * @param ReasonGroup Specified reason group.
	 * @param OnSuccess This will be called when the operation succeeded. The response is a FAccelByteModelsReasonsResponse.
	 * @param OnError This will be called when the operation failed.
	*/
	void GetReasons(const FString& ReasonGroup, const THandler<FAccelByteModelsReasonsResponse>& OnSuccess, const FErrorHandler& OnError);
		
	/**
	 * @brief Get Reason Groups.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The response is a FAccelByteModelsReasonGroupsResponse.
	 * @param OnError This will be called when the operation failed.
	*/
	void GetReasonGroups(const THandler<FAccelByteModelsReasonGroupsResponse>& OnSuccess, const FErrorHandler& OnError);
	
};

} // Namespace Api
} // Namespace AccelByte
