# Models

## FBalanceInfo

|Field|Type|Description|
|---|---|---|
ID              |FString    |User ID that owns this balance.
WalletID        |FString    |Wallet ID from the balance.
CurrencyCode    |FString    |Currency code from the balance (e.g. USD)
Balance         |int32      |Amount of the balance.
BalanceSource   |FString    |Balance's source.
CreatedAt       |FString    |Balance creation time.
UpdatedAt       |FString    |Latest balance update time.
Status          |FString    |Status of the balance.

## FCategory

|Field|Type|Description|
|---|---|---|
Namespace           |FString    |Category's namespace.
ParentCategoryPath  |FString    |Hold category's parent's path.
CategoryPath        |FString    |Current category's path.
DisplayName         |FString    |Category's display name.
CreatedAt           |FString    |Category creation time.
UpdatedAt           |FString    |Category latest update time.
Root                |bool       |Is this category is a root category?

## FCurrencySummary

|Field|Type|Description|
|---|---|---|
CurrencyCode    |FString|The code of the currency
CurrencySymbol  |FString|The symbol of the currency.
CurrencyType    |FString|The type of the currency ('REAL', 'VIRTUAL')
Namespace       |FString|The namespace of the currency.
Decimals        |int32  |The decimals number of the currency.

## FItemInfo

|Field|Type|Description|
|---|---|---|
Title               |FString     |Item's title
Description         |FString     |Item's description
LongDescription     |FString     |Item's long description
Images              |TArray<[FJusticeImage](#fjusticeimage)>    |Item's image(s)
ThumbnailImage      |[FJusticeImage](#fjusticeimage)     |Item's thumbnail
ItemID              |FString     |Item's ID
AppID               |FString     |App ID for the item.
AppType             |FString     |App type for the item.
SKU                 |FString     |Item's SKU.
Namespace           |FString     |Item's namespace.
EntitlementName     |FString     |Item's entitlement name.
EntitlementType     |FString     |Item's entitlement type.
UseCount            |int32       |Item's usage count.
CategoryPath        |FString     |Category path containing the item.
Status              |FString     |Item's status.
ItemType            |FString     |Item's type.
CreatedAt           |FString     |Item's creation time.
UpdatedAt           |FString   |Item latest update time.
TargetCurrencyCode  |FString     |Item's target currency code.
TargetNamespace     |FString     |Item's target namespace.
RegionData          |TArray<[FRegionData](#regiondata)>|Region data of this item.
ItemIds             |TArray< FString>   |Array of item IDs if this item is a bundle item.
Tags                |TArray< FString>   |Item's tag(s).

## FItemPagingSlicedResult

|Field|Type|Description|
|---|---|---|
Data    |TArray<[FItemInfo](#fiteminfo)>|Array of items in the result page.
Paging  |[FPaging](#fpaging)|Paging system from the result.

## FJusticeImage

|Field|Type|Description|
|---|---|---|
Height          |int32  |Image's height in pixel.
Width           |int32  |Image's width in pixel.
ImageURL        |FString|Image's URL.
SmallImageURL   |FString|Small image's URL.

## FLinkedPlatform

|Field|Type|Description|
|---|---|---|
PlatformID      |FString|ID of the platform linked to user.
PlatformUserID  |FString|User ID that links the user to platform.
Namespace       |FString|Namespace of the linked account.
UserID          |FString|User ID that is linked to platform.

## FOAuthTokenJustice

|Field|Type|Description|
|---|---|---|
AccessToken     |FString|Client's access token.
UserRefreshToken|FString|Client's refresh token.
TokenType       |FString|Type of token.
ExpiresIn       |double|Token expiration time.
Permissions     |TArray<[FPermissionJustice](#fpermissionjustice)>|Permission(s) of the client.
Roles           |TArray< FString>|Role(s) of the client.
Bans            |TArray< FString>|Ban(s) of the client.
UserID          |FString|ID of the client.
DisplayName     |FString|Display name of the client.
Namespace       |FString|Namespace of the client.
LastTokenRefreshUtc|FDateTime|
NextTokenRefreshUtc|FDateTime|
TokenRefreshBackoff|FTimespan|
JFlags          |int32|

## FOrderCreate

|Field|Type|Description|
|---|---|---|
ItemID          |FString|ID of item ordered.
Quantity        |FString|Quantity of item ordered.
Price           |int32  |Total price of item ordered.
DiscountedPrice |int32  |Total price of item ordered after discount.
CurrencyCode    |FString|The currency code of item ordered.
ReturnURL       |FString|URL that is opened after order is fulfilled.

## FOrderHistoryInfo

|Field|Type|Description|
|---|---|---|
OrderNo     |FString     |Order's number.
Operator    |FString     |Operator of the order.
Action      |FString     |Order's current action.
Reason      |FString     |Order's reason.
UserID      |FString     |ID of user dealing with this order.
CreatedAt   |FString     |Order's creation time.
UpdatedAt   |FString     |Order latest update time.

## FOrderInfo

|Field|Type|Description|
|---|---|---|
OrderNo             |FString    |Order's number.
UserID              |FString    |ID of user that creates this order.
ItemID              |FString    |Item's ID included in this order.
bSandBox            |bool       |Is this order sandbox mode?
Quantity            |int32      |The amount of item ordered.
Price               |int32      |Item's price.
DiscountedPrice     |int32      |Item's discounted price.
Vat                 |int32      |Order's VAT.
SalesTax            |int32      |Order's sales tax.
PaymentProviderFee  |int32      |Payment provider fee for this order.
PaymentMethodFee    |int32      |Payment method fee for this order.
Currency            |[FCurrencySummary](#fcurrencysummary)|Currency used in this order.
PaymentURL          |[FPaymentURL](#fpaymenturl)|Order's payment information.
PaymentStationURL   |FString    |URL used to pay this order.
Transactions        |TArray<[FTransaction](#ftransaction)>|Order's transaction step.
EntitlementIds      |TArray< FString>   |Entitlement ID from this order.
Status              |FString    |Order's status.
StatusReason        |FString    |Reason for order's status.
Namespace           |FString    |Namespace of this order.
CreatedTime         |FString    |Order creation time.
ChargedTime         |FString    |Order charging time.
FulfilledTime       |FString    |Order fulfilling time.
RefundedTime        |FString    |Order refunding time.
CreatedAt           |FString    |Order creation time.
UpdatedAt           |FString    |Order latest activity.

## FOrderInfoPaging

|Field|Type|Description|
|---|---|---|
Data    |TArray<[FOrderInfo](#forderinfo)>|Array of orders' informations in the result page.
PagingInfo  |[FPaging](#fpaging)|Paging system from the result.

## FPaging

|Field|Type|Description|
|---|---|---|
Previous    |FString|Previous page.
Next        |FString|Next page.

## FPaymentURL

|Field|Type|Description|
|---|---|---|
PaymentProvider |FString |Provider of this payment.
PaymentURL      |FString |URL to continue the payment.
PaymentToken    |FString |Payment's token.
ReturnURL       |FString |URL returned from [OrderCreate](#ordercreate)
PaymentType     |FString |Payment's type.

## FPermissionJustice

|Field|Type|Description|
|---|---|---|
Resource    |FString|Resource of the permission.
Action      |int32  |Code number for the possible action.

## FPrice

|Field|Type|Description|
|---|---|---|
Value       |int32   |Price value.
CurrencyCode|FString |Code of currency for this price.
CurrencyType|FString |Type of the currency.
Namespace   |FString |Namespace of the currency.

## FRegionData

|Field|Type|Description|
|---|---|---|
Price                       |int32      |Price of item in this region.
DiscountPercentage          |int32      |Percentage of the discount for item in this region.
DiscountAmount              |int32      |Amount of discount for item in this region.
DiscountedPrice             |int32      |Price of item after discounted in this region.
CurrencyCode                |FString    |Code of currency used for the item in this region.
CurrencyType                |FString    |Type of currency for item in this region.
CurrencyNamespace           |FString    |Namespace of currency used.
PurchaseAt                  |FString    |Item purchase time.
ExpireAt                    |FString    |Item expiration time.
TotalNum                    |int32      |Item amount.
TotalNumPerAccount          |int32      |Amount of item per user.
DiscountPurchaseAt          |FString    |Discount purchasing time.
DiscountExpireAt            |FString    |Discount expiration time.
DiscountTotalNum            |int32      |Total discount amount.
DiscountTotalNumPerAccount  |int32      |Total discount amount per user.

## FResetPasswordRequest

|Field|Type|Description|
|---|---|---|
Code        |FString |Verification code to reset old password.
LoginID     |FString |LoginID (e.g. email) of the user that forgets the password.
NewPassword |FString |New password to replace the old password.

## FTransaction

|Field|Type|Description|
|---|---|---|
TransactionID           |FString |Transaction ID.
Amount                  |int32   |Transaction's amount.
Vat                     |int32   |Transcation's VAT.
SalesTax                |int32   |Transaction's sales tax.
Currency                |[FCurrencySummary](#fcurrencysummary)|Transaction currency information.
Type                    |FString |Transaction type ('CHARGE', 'REFUND').
Status                  |FString |Transaction status ('INIT', 'FINISHED', 'FAILED').
Provider                |FString |Transaction's provider (e.g. [XSOLLA](https://xsolla.com/)).
PaymentProviderFee      |int32   |Transaction's fee for provider.
PaymentMethod           |FString |Transaction's payment method.
PaymentMethodFee        |int32   |Transaction's fee for payment method used.
MerchantID              |FString |Transaction merchant's ID
ExternalTransactionID   |FString |External transaction's ID.
ExternalStatusCode      |FString |External transaction status code.
ExternalMessage         |FString |External transaction message.
TransactionStartTime    |FString |Transaction's starting time.
TransactionEndTime      |FString |Transaction's ending time.

## FUserAuthTypeJustice

|Field|Type|Description|
|---|---|---|
Email|enum(readonly)|Email-based account.
Phone|enum(readonly)|Phone-based account.

## FUserCreateRequest

|Field|Type|Description|
|---|---|---|
AuthType    |FString|Type of authentication.
DisplayName |FString|User's display name.
LoginID     |FString|User's login ID.
Password    |FString|User's password.

## FUserCreateResponse

|Field|Type|Description|
|---|---|---|
Namespace   |FString|Namespace of the user that has been created.
UserID      |FString|ID of the user that has been created.
AuthType    |FString|Authentication type of the user that has been created.
DisplayName |FString|Display name of the user that has been created.
LoginID     |FString|Login ID (e.g. email) of the user that has been created.

## FUserProfileInfo

|Field|Type|Description|
|---|---|---|
UserID          |FString|User's ID.
Namespace       |FString|Namespace of the user.
DisplayName     |FString|User's display name.
FirstName       |FString|User's first name.
LastName        |FString|User's last name.
Country         |FString|User's country.
AvatarSmallURL  |FString|URL of user's small avatar.
AvatarURL       |FString|URL of user's avatar.
AvatarLargeURL  |FString|URL of user's large avatar.
Email           |FString|User's email.
Status          |FString|User's status.
Language        |FString|User's language.
Timezone        |FString|User's timezone.
DateOfBirth     |FString|User's date of birth.
CustomAttributes|TMap< FString, FString>|User's custom attributes.

## FUserProfileInfoUpdate

|Field|Type|Description|
|---|---|---|
DisplayName     |FString|Update user's display name.
FirstName       |FString|Update user's first name.
LastName        |FString|Update user's last name.
AvatarSmallURL  |FString|Update user's URL of their small avatar.
AvatarURL       |FString|Update user's URL of their avatar.
AvatarLargeURL  |FString|Update user's URL of their large avatar.
Email           |FString|Update user's email.
Language        |FString|Update user's language.
Timezone        |FString|Update user's timezone.
DateOfBirth     |FString|Update user's date of birth.
CustomAttributes|TMap< FString, FString>|User's custom attributes.
Country         |FString|Update user's country.
Status          |FString|Update user's status.

## FWalletInfo

|Field|Type|Description|
|---|---|---|
ID              |FString|Wallet's ID.
Namespace       |FString|Wallet's namespace.
UserID          |FString|Wallet's owner's ID.
CurrencyCode    |FString|Currency code for this wallet.
CurrencySymbol  |FString|Symbol of the currency code.
Balance         |int32  |Wallet's balance.
CreatedAt       |FString|Wallet creation time.
UpdatedAt       |FString|Wallet update time.
Status          |FString|Wallet's status.