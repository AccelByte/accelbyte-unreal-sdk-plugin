#include "UpgradeHeadlessAccount.h"

#include "Blueprints/IdentityBlueprint/UpgradeHeadlessAccount.h"
#include "Services/JusticeIdentity.h"

UUpgradeHeadlessAccount * UUpgradeHeadlessAccount::UpgradeHeadlessAccount(FString Email, FString Password)
{
	UUpgradeHeadlessAccount* Node = NewObject<UUpgradeHeadlessAccount>();
	Node->Email = Email;
	Node->Password = Password;
	return Node;
}

void UUpgradeHeadlessAccount::Activate()
{
	JusticeIdentity::UpgradeHeadlessAccount(FJusticeNamespace, FJusticeUserToken->AccessToken, FJusticeUserID, this->Email, this->Password, FUpgradeHeadlessAccountCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr) {
		if (bSuccessful)
		{
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(TEXT(""));
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(ErrorStr);
			}
		}
	}));
}