// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Online/ICMP/Public/Icmp.h"
#include "Networking.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteQosModels.h"

namespace AccelByte
{
namespace Api
{

/**
	* @brief Qos to manage specific Qos Server(s).
	*/
class ACCELBYTEUE4SDK_API Qos
{
public:
	Qos();
	~Qos();

	/**
	* @brief Get List of Qos Server(s) Latencies.
	*
	* @param OnSuccess This will be called when the operation succeeded. The result is an array of TMap with FString as key and int32 as value.
	* @param OnError This will be called when the operation failed.
	*/
	void GetServerLatencies(const THandler<TArray<TPair<FString, float>>>& OnSuccess, const FErrorHandler& OnError);

	TArray<TPair<FString, float>> Latencies;
private:
	Qos(Qos const&) = delete;
	Qos(Qos&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
