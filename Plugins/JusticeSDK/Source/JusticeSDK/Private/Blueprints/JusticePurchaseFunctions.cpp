#include "JusticePurchaseFunctions.h"
#include "JusticePurchase.h"

void UJusticePurchaseFunctions::CreateNewOrder(FString itemId, int Price, FString Currency, FCreateNewOrderCompleteDynamicDelegate OnComplete)
{
	JusticePurchase::CreateNewOrder(itemId, Price, Currency, FCreateNewOrderCompleteDelegate::CreateLambda([OnComplete](bool isSuccess, FString errorString, OrderInfo order) {
		UOrderInfo* result = NewObject<UOrderInfo>();
		result->FromOrderInfo(order);
		OnComplete.ExecuteIfBound(isSuccess, errorString, result);
	}));
}
