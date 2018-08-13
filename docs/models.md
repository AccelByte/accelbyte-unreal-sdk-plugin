#### BalanceInfo ####
|field|type|description|
|---|---|---|
ID              |FString    |User's id that own this balance.
walletId        |FString    |Wallet ID from this balance.
CurrencyCode    |FString    |Currency code from this balance (e.g. USD)
balance         |int32      |Amount of this balance.
balanceSource   |FString    |Balance's source.
createdAt       |FString    |Balance creation time.
updatedAt       |FString    |Latest balance update time.
status          |FString    |Status of a balance.

#### Category ####
|field|type|description|
|---|---|---|
Namespace           |FString    |Category's namespace.
ParentCategoryPath  |FString    |Hold category's parent's path.
CategoryPath        |FString    |Current category's path.
DisplayName         |FString    |Category's display name.
CreatedAt           |FString    |Category creation time.
UpdatedAt           |FString    |Category latest update time.
Root                |bool       |Is this category is a root category?

#### CurrencySummary ####
|field|type|description|
|---|---|---|
CurrencyCode    |FString|The code of this currency.
CurrencySymbol  |FString|The symbol of this currency.
CurrencyType    |FString|The type of this currency ('REAL', 'VIRTUAL')
Namespace       |FString|The namespace of this currency.
Decimals        |int32  |The decimals number of this curreny.

#### ItemInfo ####
|field|type|description|
|---|---|---|
Title               |FString     |Item's title
Description         |FString     |Item's description
LongDescription     |FString     |Item's long description
Images              |TArray<[JusticeImage](#justiceimage)>    |Item's image(s)
ThumbnailImage      |[JusticeImage](#justiceimage)     |Item's thumbnail
ItemId              |FString     |Item's ID
AppId               |FString     |App ID for this item.
AppType             |FString     |App type for this item.
SKU                 |FString     |Item's SKU.
Namespace           |FString     |Item's namespace.
EntitlementName     |FString     |Item's entitlement name.
EntitlementType     |FString     |Item's entitlement type.
UseCount            |int32       |Item's usage count.
CategoryPath        |FString     |Path of category that contain this item.
Status              |FString     |Item's status.
ItemType            |FString     |Item's type.
CreatedAt           |FString     |Item's creation time.
UpdatedAt           |FString   |Item latest update time.
TargetCurrencyCode  |FString     |Item's target currency code.
TargetNamespace     |FString     |Item's target namespace.
RegionData          |TArray<[RegionData](#regiondata)>|Region data of this item.
ItemIds             |TArray< FString>   |Array of item IDs if this item is a bundle item.
Tags                |TArray< FString>   |Item's tag(s).

#### ItemPagingSlicedResult ####
|field|type|description|
|---|---|---|
Data    |TArray<[ItemInfo](#iteminfo)>|Array of items in the result page.
Paging  |[Paging](#paging)|Paging system from the result.

#### JusticeImage ####
|field|type|description|
|---|---|---|
Height          |int32  |Image's height in pixel.
Width           |int32  |Image's width in pixel.
ImageUrl        |FString|Image's URL.
SmallImageUrl   |FString|Small image's URL.

#### LinkedPlatform ####
|field|type|description|
|---|---|---|
PlatformID      |FString|ID of the platform that linked to user.
PlatformUserId  |FString|User ID that linking the user to platform.
Namespace       |FString|Namespace of the linked account.
UserID          |FString|User's ID that linked with platform.

#### OAuthTokenJustice ####
|field|type|description|
|---|---|---|
AccessToken     |FString|Client's access token.
UserRefreshToken|FString|Client's refresh token.
TokenType       |FString|Type of token.
ExpiresIn       |double|Token expiration time.
Permissions     |TArray<[PermissionJustice](#permissionjustice)>|Permission(s) of the client.
Roles           |TArray< FString>|Role(s) of the client.
Bans            |TArray< FString>|Ban(s) of the client.
UserID          |FString|ID of the client.
DisplayName     |FString|Display name of the client.
Namespace       |FString|Namespace of the client.
LastTokenRefreshUtc|FDateTime|
NextTokenRefreshUtc|FDateTime|
TokenRefreshBackoff|FTimespan|
JFlags          |int32|

#### OrderCreate ####
|field|type|description|
|---|---|---|
ItemID          |FString|ID of item that ordered.
Quantity        |FString|Quantity of item that ordered.
Price           |int32  |Total price of item that ordered.
DiscountedPrice |int32  |Total price of item that ordered after discounted.
CurrencyCode    |FString|The currency code of item that ordered.
ReturnUrl       |FString|URL that will be opened after order fulfilled.

#### OrderHistoryInfo ####
|field|type|description|
|---|---|---|
OrderNo     |FString     |Order's number.
Operator    |FString     |Operator of this order.
Action      |FString     |Order's current action.
Reason      |FString     |Order's reason.
UserId      |FString     |ID of user that deal with this order.
CreatedAt   |FString     |Order's creation time.
UpdatedAt   |FString     |Order latest update time.

#### OrderInfo ####
|field|type|description|
|---|---|---|
OrderNo             |FString    |Order's number.
UserID              |FString    |ID of user that create this order.
ItemID              |FString    |Item's ID that included in this order.
bSandBox            |bool       |Is this order sandbox mode.
Quantity            |int32      |The amount of item that ordered.
Price               |int32      |Item's price.
DiscountedPrice     |int32      |Item's discounted price.
Vat                 |int32      |Order's VAT.
SalesTax            |int32      |Order's sales tax.
PaymentProviderFee  |int32      |Payment provider fee for this order.
PaymentMethodFee    |int32      |Payment method fee for this order.
Currency            |[CurrencySummary](#currencysummary)|Currency that used in this order.
PaymentUrl          |[PaymentUrl](#paymenturl)|Order's payment information.
PaymentStationUrl   |FString    |URL that will be used to pay this order.
Transactions        |TArray<[Transaction](#transaction)>|Order's transaction step.
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

#### OrderInfoPaging ####
|field|type|description|
|---|---|---|
Data    |TArray<[OrderInfo](#orderinfo)>|Array of orders' informations in the result page.
PagingInfo  |[Paging](#paging)|Paging system from the result.

#### Paging ####
|field|type|description|
|---|---|---|
Previous    |FString|Previous page.
Next        |FString|Next page.

#### PaymentUrl ####
|field|type|description|
|---|---|---|
paymentProvider |FString |Provider of this payment.
paymentUrl      |FString |URL to continue the payment.
paymentToken    |FString |Payment's token.
returnUrl       |FString |URL that returned from [OrderCreate](#ordercreate)
paymentType     |FString |Payment's type.

#### PermissionJustice ####
|field|type|description|
|---|---|---|
Resource    |FString|Resource of the permission.
Action      |int32  |Code number for the possible action.

#### Price ####
|field|type|description|
|---|---|---|
Value       |int32   |Price value.
CurrencyCode|FString |Code of currency for this price.
CurrencyType|FString |Type of the currency.
Namespace   |FString |Namespace of the currency.

#### RegionData ####
|field|type|description|
|---|---|---|
Price                       |int32      |Price of the item in this region.
DiscountPercentage          |int32      |Percentage of the discount for the item in this region.
DiscountAmount              |int32      |Amount of discount for the item in this region.
DiscountedPrice             |int32      |Price of the item after discounted in this region.
CurrencyCode                |FString    |Code of currency that used for the item in this region.
CurrencyType                |FString    |Type of currency for this item in this region.
CurrencyNamespace           |FString    |Namespace of curreny that used.
PurchaseAt                  |FString    |Item purchase time.
ExpireAt                    |FString    |Item expiration time.
TotalNum                    |int32      |Amount of the item.
TotalNumPerAccount          |int32      |Amount of item per user.
DiscountPurchaseAt          |FString    |Discount purchasing time.
DiscountExpireAt            |FString    |Discount expiration time.
DiscountTotalNum            |int32      |Total discount amount.
DiscountTotalNumPerAccount  |int32      |Total discount amount per user.

#### ResetPasswordRequest ####
|field|type|description|
|---|---|---|
Code        |FString |Verification code to reset old password.
LoginID     |FString |LoginId (e.g. email) of the user that forgets the password.
NewPassword |FString |New password that will replace the old password.

#### Transaction ####
|field|type|description|
|---|---|---|
txId                |FString |Transaction ID.
amount              |int32   |Transaction's amount.
vat                 |int32   |Transcation's VAT.
salesTax            |int32   |Transaction's sales tax.
currency            |[CurrencySummary](#currencysummary)|Transaction currency information.
type                |FString |Transaction type ('CHARGE', 'REFUND').
status              |FString |Transaction status ('INIT', 'FINISHED', 'FAILED').
provider            |FString |Transaction's provider (e.g. [XSOLLA](https://xsolla.com/)).
paymentProviderFee  |int32   |Transaction's fee for provider.
paymentMethod       |FString |Transaction's payment method.
paymentMethodFee    |int32   |Transaction's fee for payment method that used.
merchantId          |FString |Transaction's merchant's ID
extTxId             |FString |External transaction's ID.
extStatusCode       |FString |External transaction status code.
extMessage          |FString |External transaction message.
txStartTime         |FString |Transaction's starting time.
txEndTime           |FString |Transaction's ending time.

#### FUserAuthTypeJustice ####
|field|type|description|
|---|---|---|
Email|enum(readonly)|Email-based account.
Phone|enum(readonly)|Phone-based account.

#### UserCreateRequest ####
|field|type|description|
|---|---|---|
AuthType    |FString|Type of authentication.
DisplayName |FString|User's display name.
LoginID     |FString|User's login ID.
Password    |FString|User's password.

#### UserCreateResponse ####
|field|type|description|
|---|---|---|
Namespace   |FString|Namespace of the user that has been created.
UserID      |FString|ID of the user that has been created.
AuthType    |FString|Authentication type of the user that has been created.
DisplayName |FString|Display name of the user that has been created.
LoginID     |FString|Login ID (e.g. email) of the user that has been created.

#### UserProfileInfo ####
|field|type|description|
|---|---|---|
UserID          |FString|User's ID.
Namespace       |FString|Namespace of the user.
DisplayName     |FString|User's display name.
FirstName       |FString|User's first name.
LastName        |FString|User's last name.
Country         |FString|User's country.
AvatarSmallUrl  |FString|URL of user's small avatar.
AvatarUrl       |FString|URL of user's avatar.
AvatarLargeUrl  |FString|URL of user's large avatar.
Email           |FString|User's email.
Status          |FString|User's status.
Language        |FString|User's language.
Timezone        |FString|User's timezone.
DateOfBirth     |FString|User's date of birth.
CustomAttributes|TMap< FString, FString>|User's custom attributes.

#### UserProfileInfoUpdate ####
|field|type|description|
|---|---|---|
DisplayName     |FString|Update user's display name.
FirstName       |FString|Update user's first name.
LastName        |FString|Update user's last name.
AvatarSmallUrl  |FString|Update user's URL of their small avatar.
AvatarUrl       |FString|Update user's URL of their avatar.
AvatarLargeUrl  |FString|Update user's URL of their large avatar.
Email           |FString|Update user's email.
Language        |FString|Update user's language.
Timezone        |FString|Update user's timezone.
DateOfBirth     |FString|Update user's date of birth.
CustomAttributes|TMap< FString, FString>|User's custom attributes.
Country         |FString|Update user's country.
Status          |FString|Update user's status.

#### WalletInfo ####
|field|type|description|
|---|---|---|
ID              |FString|Wallet's ID.
Namespace       |FString|Wallet's namespace.
userId          |FString|Wallet's owner's ID.
CurrencyCode    |FString|Currency code for this wallet.
CurrencySymbol  |FString|Symbol of the currency code.
balance         |int32  |Wallet's balance.
createdAt       |FString|Wallet creation time.
updatedAt       |FString|Wallet update time.
status          |FString|Wallet's status.