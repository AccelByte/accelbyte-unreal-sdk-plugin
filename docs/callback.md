### FDefaultCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if the request unsuccessful.

### FRegisterPlayerCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if the request unsuccessful.
UserCreateResponse|**[UserCreateResponse](../models/#usercreateresponse)**|User's account that created.

### FUserLoginCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if the request unsuccessful.
OAuthTokenJustice|**[OAuthTokenJustice](../models/#oauthtokenjustice)**|User's token.

### FGetLinkedPlatformCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if the request unsuccessful.
|TArray<[LinkedPlatform](../models/#linkedplatform)>|Array of linked platform.

### FCategoryDefaultCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if the request unsuccessful.
|TArray<[Category](../models/#category)>|Array of linked platform.

### FGetItemCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if the request unsuccessful.
|[ItemInfo](../models/#iteminfo)|Item information.

### FItemCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if the request unsuccessful.
|TArray<[ItemInfo](../models/#iteminfo)>|Array of item information.

### FReqestCurrentPlayerProfileCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if the request unsuccessful.
|[UserProfileInfo](../models/#userprofileinfo)|User profile.

### FOrderInfoCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if the request unsuccessful.
|[OrderInfo](../models/#orderinfo)|Order information.

### FGetUserOrdersCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if the request unsuccessful.
|TArray<[OrderInfo](../models/#orderinfo)>|Array of order information.

### FGetUserOrderHistoryCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if the request unsuccessful.
|TArray<[OrderHistoryInfo](../models/#orderhistoryinfo)>|Array of order history.

### FGetWalletBalanceCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if the request unsuccessful.
|**int32**|Amount of wallet balance.

### FReqestCurrentPlayerProfileCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if the request unsuccessful.
|[UserProfileInfo](../models/#userprofileinfo)|User profile information.