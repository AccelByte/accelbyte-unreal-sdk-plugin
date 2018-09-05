// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IJusticeHttpBase.h"

class IJusticeHttpRequest;
class IJusitceHttpResponse;

namespace EJusticeHttpRequestStatus
{
	/**
	 * Enumerates the current state of an Http request
	 */
	enum Type
	{
		/** Has not been started via ProcessRequest() */
		NotStarted,
		/** Currently being ticked and processed */
		Processing,
		/** Finished but failed */
		Failed,
		/** Failed because it was unable to connect (safe to retry) */
		Failed_ConnectionError,
		/** Finished and was successful */
		Succeeded
	};

	/** @return the stringified version of the enum passed in */
	inline const TCHAR* ToString(EJusticeHttpRequestStatus::Type EnumVal)
	{
		switch (EnumVal)
		{
			case NotStarted:
			{
				return TEXT("NotStarted");
			}
			case Processing:
			{
				return TEXT("Processing");
			}
			case Failed:
			{
				return TEXT("Failed");
			}
			case Failed_ConnectionError:
			{
				return TEXT("ConnectionError");
			}
			case Succeeded:
			{
				return TEXT("Succeeded");
			}
		}
		return TEXT("");
	}
}

typedef TSharedPtr<class IJusticeHttpRequest> FJusticeHttpRequestPtr;
typedef TSharedPtr<class IJusitceHttpResponse,ESPMode::ThreadSafe> FJusticeHttpResponsePtr;

/**
 * Delegate called when an Http request completes
 *
 * @param first parameter - original Http request that started things
 * @param second parameter - response received from the server if a successful connection was established
 * @param third parameter - indicates whether or not the request was able to connect successfully
 */
DECLARE_DELEGATE_ThreeParams(FJusticeHttpRequestCompleteDelegate, FJusticeHttpRequestPtr, FJusticeHttpResponsePtr, bool);

/**
 * Delegate called per tick to update an Http request upload or download size progress
 *
 * @param first parameter - original Http request that started things
 * @param second parameter - the number of bytes sent / uploaded in the request so far.
 * @param third parameter - the number of bytes received / downloaded in the response so far.
 */
DECLARE_DELEGATE_ThreeParams(FJusticeHttpRequestProgressDelegate, FJusticeHttpRequestPtr, int32, int32);

/**
 * Interface for Http requests (created using FHttpFactory)
 */
class IJusticeHttpRequest : 
	public IJusticeHttpBase, public TSharedFromThis<IJusticeHttpRequest>
{
public:

	/**
	 * Gets the verb (GET, PUT, POST) used by the request.
	 * 
	 * @return the verb string
	 */
	virtual FString GetVerb() = 0;

	/**
	 * Sets the verb used by the request.
	 * Eg. (GET, PUT, POST)
	 * Should be set before calling ProcessRequest.
	 * If not specified then a GET is assumed.
	 *
	 * @param Verb - verb to use.
	 */
	virtual void SetVerb(const FString& Verb) = 0;

	/**
	 * Sets the URL for the request 
	 * Eg. (http://my.domain.com/something.ext?key=value&key2=value).
	 * Must be set before calling ProcessRequest.
	 *
	 * @param URL - URL to use.
	 */
	virtual void SetURL(const FString& URL) = 0;

	/**
	 * Sets the content of the request (optional data).
	 * Usually only set for POST requests.
	 *
	 * @param ContentPayload - payload to set.
	 */
	virtual void SetContent(const TArray<uint8>& ContentPayload) = 0;

	/**
	 * Sets the content of the request as a string encoded as UTF8.
	 *
	 * @param ContentString - payload to set.
	 */
	virtual void SetContentAsString(const FString& ContentString) = 0;

	/**
	 * Sets optional header info.
	 * SetHeader for a given HeaderName will overwrite any previous values
	 * Use AppendToHeader to append more values for the same header
	 * Content-Length is the only header set for you.
	 * Required headers depends on the request itself.
	 * Eg. "multipart/form-data" needed for a form post
	 *
	 * @param HeaderName - Name of the header (ie, Content-Type)
	 * @param HeaderValue - Value of the header
	 */
	virtual void SetHeader(const FString& HeaderName, const FString& HeaderValue) = 0;

	/**
	* Appends to the value already set in the header. 
	* If there is already content in that header, a comma delimiter is used.
	* If the header is as of yet unset, the result is the same as calling SetHeader
	* Content-Length is the only header set for you.
	* Also see: SetHeader()
	*
	* @param HeaderName - Name of the header (ie, Content-Type)
	* @param AdditionalHeaderValue - Value to add to the existing contents of the specified header.
	*	comma is inserted between old value and new value, per HTTP specifications
	*/
	virtual void AppendToHeader(const FString& HeaderName, const FString& AdditionalHeaderValue) = 0;

	/**
	 * Called to begin processing the request.
	 * OnProcessRequestComplete delegate is always called when the request completes or on error if it is bound.
	 * A request can be re-used but not while still being processed.
	 *
	 * @return if the request was successfully started.
	 */
	virtual bool ProcessRequest() = 0;

	/**
	 * Delegate called when the request is complete. See FJusticeHttpRequestCompleteDelegate
	 */
	virtual FJusticeHttpRequestCompleteDelegate& OnProcessRequestComplete() = 0;

	/**
	 * Delegate called to update the request/response progress. See FJusticeHttpRequestProgressDelegate
	 */
	virtual FJusticeHttpRequestProgressDelegate& OnRequestProgress() = 0;

	/**
	 * Called to cancel a request that is still being processed
	 */
	virtual void CancelRequest() = 0;

	/**
	 * Get the current status of the request being processed
	 *
	 * @return the current status
	 */
	virtual EJusticeHttpRequestStatus::Type GetStatus() = 0;

	/**
	 * Get the associated Response
	 *
	 * @return the response
	 */
	virtual const FJusticeHttpResponsePtr GetResponse() const = 0;

	/**
	 * Used to tick the request
	 *
	 * @param DeltaSeconds - seconds since last ticked
	 */
	virtual void Tick(float DeltaSeconds) = 0;

	/**
	 * Gets the time that it took for the server to fully respond to the request.
	 * 
	 * @return elapsed time in seconds.
	 */
	virtual float GetElapsedTime() = 0;

	/** 
	 * Destructor for overrides 
	 */
	virtual ~IJusticeHttpRequest() {};
};

