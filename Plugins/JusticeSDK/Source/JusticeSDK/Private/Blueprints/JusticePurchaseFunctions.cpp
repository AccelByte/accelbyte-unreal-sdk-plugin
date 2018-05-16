#include "JusticePurchaseFunctions.h"
#include "JusticePurchase.h"

void UJusticePurchaseFunctions::CreateNewOrder(FString itemId, int Price, int DiscountedPrice, FString Currency, FString StoreId, FCreateNewOrderCompleteDynamicDelegate OnComplete)
{
	JusticePurchase::CreateNewOrder(itemId, Price, DiscountedPrice, Currency, StoreId, FCreateNewOrderCompleteDelegate::CreateLambda([OnComplete](bool isSuccess, FString errorString, OrderInfo order) {
		UOrderInfo* result = NewObject<UOrderInfo>();
		result->FromOrderInfo(order);
		OnComplete.ExecuteIfBound(isSuccess, errorString, result);
	}));
}
