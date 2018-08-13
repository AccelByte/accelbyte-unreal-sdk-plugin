#include "JusticePurchaseFunctions.h"
#include "JusticePurchase.h"

void UJusticePurchaseFunctions::CreateNewOrder(FString ItemID, int32 Price, int32 DiscountedPrice, FString Currency, FCreateNewOrderCompleteDynamicDelegate OnComplete)
{
	JusticePurchase::CreateNewOrder(ItemID, 
		Price, 
		DiscountedPrice, 
		Currency, 
		FOrderInfoCompleteDelegate::CreateLambda([OnComplete](bool bSuccessful, FString ErrorString, FOrderInfo Order) {
			UOrderInfo* Result = UOrderInfo::Deserialize(Order);
			check(Result);
			OnComplete.ExecuteIfBound(bSuccessful, ErrorString, Result);
	}));
}
