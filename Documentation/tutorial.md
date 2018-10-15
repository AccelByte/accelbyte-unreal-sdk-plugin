# Tutorial

## User authentication
See AccelByte::Api::User.

### Client login
This is to get access token from `client_credentials` grant type, then store the access token in memory. It is "required" for user management (create new user, reset password, etc). See AccelByte IAM documentation for how this OAuth2 token endpoint works.

```cpp
AccelByte::Api::User::ClientLogin("https://api.gamepublisher.example", "my_client_id", "my_client_secret", AccelByte::Api::User::FClientLoginSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::User::ClientLogin().
See AccelByte::Credentials.

### Login with device ID
This is to get access token with device ID and store it, though the device ID defined here is only a call to Unreal Engine 4 [FGenericPlatformMisc::GetDeviceId()](https://api.unrealengine.com/INT/API/Runtime/Core/GenericPlatform/FGenericPlatformMisc/GetDeviceId/index.html) (is this secure?). See AccelByte IAM documentation for how this OAuth2 token endpoint works.

Example:

```cpp
AccelByte::Api::User::LoginWithDeviceId("https://api.gamepublisher.example", "my_client_id", "my_client_secret", "game_1001", AccelByte::Api::User::FLoginWithDeviceIdSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::User::LoginWithDeviceId().
See AccelByte::Credentials.

### Login with email and password
This is to get access token with `password` `grant_type` and store it to memory.

Example:
```cpp
AccelByte::Api::User::LoginWithEmailAccount("https://api.gamepublisher.example", "my_client_id", "my_client_secret", "game_1001", "my.email@gamepublisher.example", "MY SUPER TOP SECRET PASSWORD 123", AccelByte::Api::User::FLoginWithEmailAccountSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```
See AccelByte::Api::User::LoginWithEmailAccount().
See AccelByte::Credentials.

### Login with Steam session ticket
This is to get access token with Steam session ticket. See AccelByte IAM documentation for how this OAuth2 token endpoint works.

Example:
```cpp
AccelByte::Api::User::LoginWithOtherPlatformAccount("https://api.gamepublisher.example", "my_client_id", "my_client_secret", "game_1001", static_cast<std::underlying_type<AccelByte::Api::User::EPlatformType>>::type>(AccelByte::Api::User::EPlatformType::Steam), "my_steam_session_ticket", AccelByte::Api::User::FLoginWithOtherPlatformAccountSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```
See AccelByte::Api::User::LoginWithOtherPlatformAccount().
See AccelByte::Credentials.

### Login with launcher
This doesn't work (yet). Do not use.

### Logout
This simply remove the stored access tokens and user ID from memory.

Example:

```cpp
AccelByte::Api::User::ResetCredentials();
```

See AccelByte::Api::User::ResetCredentials().
See AccelByte::Credentials.

## User management
See AccelByte::Api::User.

### Create email account
Create a new user with email based account, then send a verification code to that email.

Example:
```cpp
AccelByte::Api::User::CreateEmailAccount("https://api.gamepublisher.example", "my.email@gamepublisher.example", "MY SUPER TOP SECRET PASSWORD 123", "Example", AccelByte::Api::User::FCreateEmailAccountSuccess::CreateLambda([](const FAccelByteModelsUserCreateResponse& Result)
{
    MyUserId = Result.UserId;
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Fail: %d %s"), ErrorCode, *ErrorMessage);
}));
```
See AccelByte::Api::User::CreateEmailAccount().

### Verify email account
Verify the email after creating an email account. The user ID is from AccelByte::Api::User::FCreateEmailAccountSuccess.

Example:
```cpp
AccelByte::Api::User::VerifyEmailAccount("https://api.gamepublisher.example", MyUserId, "123456", AccelByte::Api::User::FVerifyEmailAccountSuccess::CreateLambda([]()
{
    UE_LOG(LogAccelByteTest, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogAccelByteTest, Log, TEXT("Fail: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::User::VerifyEmailAccount().

### Request password reset
Request for a password reset. Verification code is sent to the email.

Example:
```cpp
AccelByte::Api::User::RequestPasswordReset("https://api.gamepublisher.example", "my_client_id", "my_client_secret", "game_1001", "my.email@gamepublisher.example", AccelByte::Api::User::FRequestPasswordResetSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::User::RequestPasswordReset().

### Verify password reset
Verify the password reset request with the verification code sent to email.

Example:
```cpp
AccelByte::Api::User::VerifyPasswordReset("https://api.gamepublisher.example", "my_client_id", "my_client_secret", "game_1001", "my.email@gamepublisher.example", "123456", "MY NEW PASSwORD IS MORE TOP SECRET 123", AccelByte::Api::User::FVerifyPasswordResetSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::User::VerifyPasswordReset().

## Identity provider

### Get identity information (get profile)
Get profile from specified user. The result is FAccelByteModelsUserProfileInfo.

Example:
```cpp
AccelByte::Api::User::GetProfile("https://api.gamepublisher.example", AccelByte::Api::User::FGetProfileSuccess::CreateLambda([](const FAccelByteModelsUserProfileInfo& Result)
{
    UE_LOG(LogTemp, Log, TEXT("Success; welcome, %s %s"), *Result.FirstName, *Result.LastName);
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::User::GetProfile().

### Update identity information (update profile)
Update profile from specified user. The parameter is FAccelByteModelsUserProfileInfo.

Example:
```cpp
FAccelByteModelsUserProfileInfoUpdate ProfileUpdateRequest;
ProfileUpdateRequest.Country = "US";
ProfileUpdateRequest.Language = "en";
ProfileUpdateRequest.Timezone = "Etc/UTC";
ProfileUpdateRequest.DateOfBirth = "2000-01-01";
ProfileUpdateRequest.Email = "new.my.email@gamepublisher.example";
ProfileUpdateRequest.DisplayName = "My New Display Name Example";

AccelByte::Api::User::UpdateProfile("https://api.gamepublisher.example", ProfileUpdateRequest, AccelByte::Api::User::FUpdateProfileSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::User::UpdateProfile().

## Ecommerce
API related to Ecommerce.

### Categories
The category has [tree data structure](https://en.wikipedia.org/wiki/Tree_(data_structure)). Each category has path, for example "/equipments/armor/legs". Each category has items inside it. You can get a list of items by criteria or by its ID.

#### Get root categories
This is equivalent of getting categories in "/". This will yield, for example, "/equipments", "/consumables", "/pay2win", "/boosts".

#### Get category
This is to get one category with the specified path, for example, "/equipments/armor/torso".

#### Get child categories
This is to get the children of a category by category path.

#### Get descendant categories
This is to get the descendants of a category by category path.

### Items
An item represents a single product sold in the online store.

#### Get item ID
Get one item by its ID.

#### Get items by criteria
Get a list of item matching the criteria. You can think of it like a SQL query.

### Order
Order is used to purchase something from the online store.

#### Get order
Get a single order with order number.

#### Get orders
Get user's orders.

#### Get order history
Get order history of a specified user.

#### Fulfill order
Fulfill an order if the order is charged but the fulfillment fail.

### Wallet
A wallet can be virtual or real curreny.

#### Get wallet by currency code
This is to get wallet by its currency code.
