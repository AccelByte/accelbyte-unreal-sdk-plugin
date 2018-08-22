#include "JusticePurchaseFunctions.h"
#include "JusticePurchase.h"

void UJusticePurchaseFunctions::CreateNewOrder(int32 Quantity, FString ItemID, int32 Price, int32 DiscountedPrice, FString Currency, FCreateNewOrderCompleteDynamicDelegate OnComplete)
{
	JusticePurchase::CreateNewOrder(*FJusticeUserToken, Quantity, ItemID,
		Price * Quantity, 
		DiscountedPrice * Quantity, 
		Currency, 
		FOrderInfoCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorString, FOrderInfo* Order) {
			UOrderInfo* Result = UOrderInfo::Deserialize(Order);
			check(Result);
			OnComplete.ExecuteIfBound(bSuccessful, ErrorString, Result);
	}));
}
