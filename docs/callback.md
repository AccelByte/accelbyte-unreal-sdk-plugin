### FDefaultCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.

### FRegisterPlayerCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
UserCreateResponse|**[UserCreateResponse](../models/#usercreateresponse)**|User's account that created.

### FUserLoginCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
OAuthTokenJustice|**[OAuthTokenJustice](../models/#oauthtokenjustice)**|User's token.

### FGetLinkedPlatformCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|TArray<[LinkedPlatform](../models/#linkedplatform)>|Array of linked platform.

### FCategoryDefaultCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|TArray<[Category](../models/#category)>|Array of linked platform.

### FGetItemCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|[ItemInfo](../models/#iteminfo)|Item information.

### FItemCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|TArray<[ItemInfo](../models/#iteminfo)>|Array of item information.

### FReqestCurrentPlayerProfileCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|[UserProfileInfo](../models/#userprofileinfo)|User profile.

### FOrderInfoCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|[OrderInfo](../models/#orderinfo)|Order information.

### FGetUserOrdersCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|TArray<[OrderInfo](../models/#orderinfo)>|Array of order information.

### FGetUserOrderHistoryCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|TArray<[OrderHistoryInfo](../models/#orderhistoryinfo)>|Array of order history.

### FGetWalletBalanceCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|**int32**|Amount of wallet balance.

### FReqestCurrentPlayerProfileCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|[UserProfileInfo](../models/#userprofileinfo)|User profile information.