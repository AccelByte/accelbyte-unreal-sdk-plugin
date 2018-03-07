#pragma once

#include "CoreMinimal.h"
#include "CoreTypes.h"


class IPlayerProfile
{
public:
	virtual FString GetPlayerId() = 0;
	virtual FString GetDisplayName() = 0;
	virtual FString GetFirstName() = 0;
	virtual FString GetLastName() = 0;
	virtual FString GetAvatarSmallUrl() = 0;
	virtual FString GetAvatarLargeUrl() = 0;
	virtual FString GetAvatarUrl() = 0;
	virtual FString GetEmail() = 0;
	virtual FString GetStatus() = 0;
	virtual FString GetNamespace() = 0;
};

