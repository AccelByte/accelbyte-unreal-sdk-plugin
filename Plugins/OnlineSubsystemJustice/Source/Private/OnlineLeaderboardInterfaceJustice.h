// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineStats.h"
#include "Interfaces/OnlineLeaderboardInterface.h"
#include "OnlineSubsystemJusticePackage.h"

class FOnlineSubsystemJustice;

/**
 * Interface definition for the online services leaderboard services 
 */
class FOnlineLeaderboardsJustice : public IOnlineLeaderboards
{
private:
	
	/** Internal representation of a leadboard */
	struct FLeaderboardJustice : public FOnlineLeaderboardRead
	{
		/**
		 *	Retrieve a single record from the leaderboard for a given user
		 *
		 * @param UserId user id to retrieve a record for
		 * @return the requested user row or NULL if not found
		 */
		FOnlineStatsRow* FindOrCreatePlayerRecord(const FUniqueNetId& UserId)
		{
			FOnlineStatsRow* Row = FindPlayerRecord(UserId);
			if (Row == NULL)
			{
				// cannot have a better nickname here
				FOnlineStatsRow NewRow(UserId.ToString(), MakeShareable(new FUniqueNetIdString(UserId)));
				NewRow.Rank = -1;
				Rows.Add(NewRow);
			}

			check(FindPlayerRecord(UserId));
			return FindPlayerRecord(UserId);
		}
	};

	/** Reference to the main Justice subsystem */
	class FOnlineSubsystemJustice* JusticeSubsystem;

	/** Leaderboards maintained by the subsystem */
	TMap<FName, FLeaderboardJustice> Leaderboards;

	FOnlineLeaderboardsJustice() : 
		JusticeSubsystem(NULL)
	{
	}

	/**
	 * Creates a Justice leaderboard
	 *
	 * If the leaderboard already exists, the leaderboard data will still be retrieved
	 * @param LeaderboardName name of leaderboard to create
	 * @param SortMethod method the leaderboard scores will be sorted, ignored if leaderboard exists
	 * @param DisplayFormat type of data the leaderboard represents, ignored if leaderboard exists
	 */
	FLeaderboardJustice* FindOrCreateLeaderboard(const FName& LeaderboardName, ELeaderboardSort::Type SortMethod, ELeaderboardFormat::Type DisplayFormat);

PACKAGE_SCOPE:

	FOnlineLeaderboardsJustice(FOnlineSubsystemJustice* InJusticeSubsystem) :
		JusticeSubsystem(InJusticeSubsystem)
	{
	}

public:

	virtual ~FOnlineLeaderboardsJustice() {};

	// IOnlineLeaderboards
	virtual bool ReadLeaderboards(const TArray< TSharedRef<const FUniqueNetId> >& Players, FOnlineLeaderboardReadRef& ReadObject) override;
	virtual bool ReadLeaderboardsForFriends(int32 LocalUserNum, FOnlineLeaderboardReadRef& ReadObject) override;
	virtual bool ReadLeaderboardsAroundRank(int32 Rank, uint32 Range, FOnlineLeaderboardReadRef& ReadObject) override;
	virtual bool ReadLeaderboardsAroundUser(TSharedRef<const FUniqueNetId> Player, uint32 Range, FOnlineLeaderboardReadRef& ReadObject) override;
	virtual void FreeStats(FOnlineLeaderboardRead& ReadObject) override;
	virtual bool WriteLeaderboards(const FName& SessionName, const FUniqueNetId& Player, FOnlineLeaderboardWrite& WriteObject) override;
	virtual bool FlushLeaderboards(const FName& SessionName) override;
	virtual bool WriteOnlinePlayerRatings(const FName& SessionName, int32 LeaderboardId, const TArray<FOnlinePlayerScore>& PlayerScores) override;
};

typedef TSharedPtr<FOnlineLeaderboardsJustice, ESPMode::ThreadSafe> FOnlineLeaderboardsJusticePtr;

