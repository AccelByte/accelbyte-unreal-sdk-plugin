// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteNotificationBuffer.h"
#include "Core/AccelByteUtilities.h"

DEFINE_LOG_CATEGORY(LogAccelByteNotificationBuffer);

bool AccelByte::FAccelByteNotificationBuffer::TryAddBuffer(const FAccelByteModelsUserNotification& InNotification)
{
	if (!HasValidSequence(InNotification))
	{
		return false;
	}

	if (IsDuplicateNotification(InNotification))
	{
		UpdateLastSequence(InNotification);
		return false;
	}

	if (IsNotificationMissing(InNotification) || bBuffering)
	{
		if (!bBuffering)
		{
			bBuffering = true;
		}

		AddToBuffer(InNotification);
		SortBuffer();

		return true;
	}

	UpdateLastSequence(InNotification);

	return false;
}

bool AccelByte::FAccelByteNotificationBuffer::AddMissingNotifications(const TArray<FAccelByteModelsUserNotification>& Notifications)
{
	if (!bBuffering)
	{
		return false;
	}

	// Add missing notification if it doesn't exist in buffer and its not the last valid notification
	for (const FAccelByteModelsUserNotification& Notification : Notifications)
	{
		if ( (Notification.SequenceID.Equals(LastSequenceID) && Notification.SequenceNumber <= LastSequenceNumber)
			|| IsExistInBuffer(Notification.SequenceID, Notification.SequenceNumber))
		{
			continue;
		}

		AddToBuffer(Notification);
	}

	SortBuffer();

	return true;
}

FDateTime AccelByte::FAccelByteNotificationBuffer::GetLastNotificationReceivedTime()
{
	return LastSentAt;
}

FString AccelByte::FAccelByteNotificationBuffer::GetLastNotificationSequenceID()
{
	return LastSequenceID;
}

int32 AccelByte::FAccelByteNotificationBuffer::GetLastNotificationSequenceNumber()
{
	return LastSequenceNumber;
}

void AccelByte::FAccelByteNotificationBuffer::Clear()
{
	if (Buffer.Num() <= 0)
	{
		return;
	}

	UpdateLastSequence(Buffer.Last());

	Buffer.Empty();
	bBuffering = false;
}

TArray<FAccelByteModelsUserNotification> AccelByte::FAccelByteNotificationBuffer::GetSortedBuffer()
{
	return Buffer;
}

bool AccelByte::FAccelByteNotificationBuffer::IsBuffering()
{
	return bBuffering;
}

void AccelByte::FAccelByteNotificationBuffer::UpdateLastSequence(const FAccelByteModelsUserNotification& InNotification)
{
	LastSequenceID = InNotification.SequenceID;
	LastSequenceNumber = InNotification.SequenceNumber;
	LastSentAt = InNotification.SentAt;
}

bool AccelByte::FAccelByteNotificationBuffer::HasValidSequence(const FAccelByteModelsUserNotification& InNotification)
{
	if (InNotification.SequenceNumber <= 0 || InNotification.SequenceID.IsEmpty())
	{
		UE_LOG(LogAccelByteNotificationBuffer, Warning, TEXT("Notification has invalid sequence, sequenceID: %s, sequenceNumber: %d"), *InNotification.SequenceID, InNotification.SequenceNumber)
		return false;
	}
	return true;
}

bool AccelByte::FAccelByteNotificationBuffer::IsNotificationMissing(const FAccelByteModelsUserNotification& InNotification)
{
	// This is the first notification.
	if (LastSequenceID.IsEmpty() && LastSequenceNumber == 0 && LastSentAt == FDateTime{0})
	{
		return false;
	}

	// No reconnection occured (same SequenceID) and SequenceNumber Incremented by one.
	if (LastSequenceID.Equals(InNotification.SequenceID) && LastSequenceNumber == InNotification.SequenceNumber - 1)
	{
		return false;
	}

	UE_LOG(LogAccelByteNotificationBuffer, Warning
		, TEXT("Missing notification detected, last valid notification (LastSequenceID: %s, LastSequenceNumber: %d, LastSentAt: %s), incoming notification (SequenceID: %s, SequenceNumber: %d, SentAt: %s)")
		, *LastSequenceID, LastSequenceNumber, *LastSentAt.ToIso8601()
		, *InNotification.SequenceID, InNotification.SequenceNumber, *InNotification.SentAt.ToIso8601())
	return true;
}

bool AccelByte::FAccelByteNotificationBuffer::IsDuplicateNotification(const FAccelByteModelsUserNotification& InNotification)
{
	if (LastSequenceID.Equals(InNotification.SequenceID) && InNotification.SequenceNumber <= LastSequenceNumber)
	{
		FString OutJsonString;
		FJsonObjectConverter::UStructToJsonObjectString(InNotification, OutJsonString);
		UE_LOG(LogAccelByteNotificationBuffer, Warning, TEXT("Duplicate notification detected: %s"), *OutJsonString);
		return true;
	}
	return false;
}

bool AccelByte::FAccelByteNotificationBuffer::IsExistInBuffer(const FString& InSequenceID, const int32& InSequenceNumber)
{
	return Buffer.ContainsByPredicate(
		[&InSequenceID, &InSequenceNumber](const FAccelByteModelsUserNotification& Notif)
		{
			return Notif.SequenceID.Equals(InSequenceID) && Notif.SequenceNumber == InSequenceNumber;
		});
}

void AccelByte::FAccelByteNotificationBuffer::AddToBuffer(const FAccelByteModelsUserNotification& InNotification)
{
	FScopeLock Lock(&BufferLock);
	Buffer.Add(InNotification);
}

void AccelByte::FAccelByteNotificationBuffer::SortBuffer()
{
	Buffer.Sort([](const FAccelByteModelsUserNotification& A, const FAccelByteModelsUserNotification& B)
	{
		if (A.SentAt == B.SentAt)
		{
			return A.SequenceNumber < B.SequenceNumber;
		}

		return A.SentAt < B.SentAt;
	});
}
