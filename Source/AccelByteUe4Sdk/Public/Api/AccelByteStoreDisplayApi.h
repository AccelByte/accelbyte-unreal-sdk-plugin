// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "Core/AccelByteError.h"
#include "Core/AccelByteApiBase.h"
#include "Models/AccelByteEcommerceModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief StoreDisplayAPI its to display Items in Item Shop/Store and to configure items on the View
 */
class ACCELBYTEUE4SDK_API StoreDisplay : public FApiBase, public TSharedFromThis<StoreDisplay, ESPMode::ThreadSafe>
{
public:
	StoreDisplay(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~StoreDisplay();
	
	/**
	 * @brief Used to get all views.
	 * 
	 * @param StoreId If it's leaved string empty, the value will be got from published store id on the namespace.
	 * @param Language ISO 639-1 language tag, e.g., "en, "zh".
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsViewInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetAllViews(FString const& StoreId
		, FString const& Language
		, THandler<TArray<FAccelByteModelsViewInfo>> const& OnSuccess
		, FErrorHandler const& OnError);
		
	/**
	 * @brief Used to list active section contents.
	 *
	 * @param StoreId If it's leaved string empty, the value will be got from published store id on the namespace.
	 * @param ViewId The ViewId.
	 * @param Region ISO 3166-1 alpha-2 country tag, e.g., "US", "CN".
	 * @param Language ISO 639-1 language tag, e.g., "en, "zh".
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsSectionInfo.
	 * @param OnError This will be called when the operation failed.
	 * @param AutoCalcEstimatedPrice This will Auto Calculate Estimated Price. Default is false, if autoCalcEstimatedPrice is true
	 *		and item is flexible bundle, will auto calc price.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ListActiveSectionContents(FString const& StoreId
		, FString const& ViewId
		, FString const& Region
		, FString const& Language
		, THandler<TArray<FAccelByteModelsSectionInfo>> const& OnSuccess
		, FErrorHandler const& OnError
		, bool AutoCalcEstimatedPrice = false);
		
private:
	StoreDisplay() = delete;
	StoreDisplay(StoreDisplay const&) = delete;
	StoreDisplay(StoreDisplay&&) = delete; 
};
	
typedef TSharedRef<StoreDisplay, ESPMode::ThreadSafe> StoreDisplayRef;
typedef TSharedPtr<StoreDisplay, ESPMode::ThreadSafe> StoreDisplayPtr;
typedef TWeakPtr<StoreDisplay, ESPMode::ThreadSafe> StoreDisplayWPtr;

}// Namespace Api
} // Namespace AccelByte