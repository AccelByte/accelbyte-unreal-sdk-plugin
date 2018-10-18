# Tutorial

## User authentication
See AccelByte::Api::UserAuthentication, AccelByte::Api::Oauth2, and AccelByte::Credentials, 

### Login with client credentials
This is to get access token from `client_credentials` grant type, then store the access token in memory. 

It is "required" for user management (create new user, reset password, etc). See AccelByte IAM documentation for how this OAuth2 token endpoint works. We plan to remove this for public clients in the future. It is for servers/confidential clients only.

```cpp
AccelByte::Api::UserAuthentication::LoginWithClientCredentialsEasy(AccelByte::Api::UserAuthentication::FLoginWithClientCredentialsSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::UserAuthentication::LoginWithClientCredentials().

### Login with username and password
This is to get access token with Resource Owner Password Credentials (ROPC)/`password` `grant_type` and store it to memory.

Example:
```cpp
AccelByte::Api::UserAuthentication::LoginWithUsernameAndPasswordEasy("my.email@gamepublisher.example", "MY SUPER TOP SECRET PASSWORD 123", AccelByte::Api::UserAuthentication::FLoginWithUsernameAndPasswordSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```
See AccelByte::Api::UserAuthentication::LoginWithUsernameAndPassword().

### Login with Steam session ticket
This is to get access token with Steam session ticket. See AccelByte IAM documentation for how this OAuth2 token endpoint works.

Example:
```cpp
AccelByte::Api::UserAuthentication::LoginWithOtherPlatformAccountEasy(static_cast<std::underlying_type<AccelByte::Api::UserAuthentication::EPlatformType>>::type>(AccelByte::Api::UserAuthentication::EPlatformType::Steam), "my_steam_session_ticket_blah_blah", AccelByte::Api::UserAuthentication::FLoginWithOtherPlatformAccountSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```
See AccelByte::Api::UserAuthentication::LoginWithOtherPlatformAccount().

### Login with device ID
This is to get access token with device ID and store it, though the device ID defined here is only a call to Unreal Engine 4 [FGenericPlatformMisc::GetDeviceId()](https://api.unrealengine.com/INT/API/Runtime/Core/GenericPlatform/FGenericPlatformMisc/GetDeviceId/index.html) (is this secure?). See AccelByte IAM documentation for how this OAuth2 token endpoint works.

Example:

```cpp
AccelByte::Api::UserAuthentication::LoginWithDeviceIdEasy(AccelByte::Api::UserAuthentication::FLoginWithDeviceIdSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```
See AccelByte::Api::UserAuthentication::LoginWithDeviceId().

### Login with launcher
The `authorization_code grant_type` in IAM doesn't work yet. Do not use.

### Logout
This simply remove the stored access tokens and user ID from memory.

Example:

```cpp
AccelByte::Api::UserAuthentication::ForgetAllCredentials();
```

See AccelByte::Api::UserAuthentication::ForgetAllCredentials().
See AccelByte::Credentials.

## User management
See AccelByte::Api::UserManagement.

### Create email account
Create a new user account.

Example:
```cpp
AccelByte::Api::UserManagement::CreateUserAccountEasy("my.email@gamepublisher.example", "MY SUPER TOP SECRET PASSWORD 123", "Example", AccelByte::Api::UserManagement::FCreateUserAccountSuccess::CreateLambda([](const FAccelByteModelsUserCreateResponse& Result)
{
    MyUser = Result.UserId;
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Fail: %d %s"), ErrorCode, *ErrorMessage);
}));
```
See AccelByte::Api::UserManagement::CreateUserAccount().

### Send email verification code 
Send a verification code to user email address.

Example:
```cpp
AccelByte::Api::UserManagement::SendUserAccountVerificationCodeEasy("my.email@gamepublisher.example", AccelByte::Api::UserManagement::FSendUserAccountVerificationCodeSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::UserManagement::SendUserAccountVerificationCode().

### Verify email account
Verify the email address using the verification code sent to user's email address. User must log in first to get a access token.

Example:
```cpp
AccelByte::Api::UserManagement::VerifyUserAccountEasy("123456", AccelByte::Api::UserManagement::FVerifyUserAccountSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Fail: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::UserManagement::VerifyUserAccount().

### Send password reset code 
For a password reset, verification code is sent to the email.

Example:
```cpp
AccelByte::Api::UserManagement::SendPasswordResetCodeEasy("my.email@gamepublisher.example", AccelByte::Api::UserManagement::FSendPasswordResetCodeSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([&](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::UserManagement::SendPasswordResetCode().

### Reset password
Verify the password reset request with the verification code sent to email.

Example:
```cpp
AccelByte::Api::UserManagement::ResetPasswordEasy("my.email@gamepublisher.example", "123456", "MY NEW PASSwORD IS MORE TOP SECRET 123", AccelByte::Api::UserManagement::FResetPasswordSuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::UserManagement::ResetPassword().

## Identity provider

### Get user profile
Get profile from specified user. The result is FAccelByteModelsUserProfileInfo.

Example:
```cpp
AccelByte::Api::UserProfile::GetUserProfileEasy(AccelByte::Api::UserProfile::FGetUserProfileSuccess::CreateLambda([](const FAccelByteModelsUserProfileInfo& Result)
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
    UE_LOG(LogTemp, Log, TEXT("Welcome, %s %s"), *Result.FirstName, *Result.LastName);
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::UserProfile::GetUserProfile().

### Update user profile
Update profile for a specified user. The parameter is FAccelByteModelsProfileUpdateRequest.

Example:
```cpp
FAccelByteModelsUserProfileInfoUpdate ProfileUpdateRequest;
ProfileUpdateRequest.Country = "US";
ProfileUpdateRequest.Language = "en";
ProfileUpdateRequest.Timezone = "Etc/UTC";
ProfileUpdateRequest.DateOfBirth = "2000-01-01";
ProfileUpdateRequest.Email = "new.my.email@gamepublisher.example";
ProfileUpdateRequest.DisplayName = "My New Display Name Example";

AccelByte::Api::UserProfile::UpdateUserProfileEasy(ProfileUpdateRequest, AccelByte::Api::UserProfile::FUpdateUserProfileEasySuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));

See AccelByte::Api::UserProfile::UpdateUserProfile().

```

### Create user profile
Create profile for a specified user. The parameter is FAccelByteModelsProfileCreateRequest.

Example:
```cpp
FAccelByteModelsProfileCreateRequest ProfileCreateRequest;
ProfileCreateRequest.Country = "US";
ProfileCreateRequest.Language = "en";
ProfileCreateRequest.Timezone = "Etc/UTC";
ProfileCreateRequest.DateOfBirth = "2000-01-01";
ProfileCreateRequest.Email = "new.my.email@gamepublisher.example";
ProfileCreateRequest.DisplayName = "My New Display Name Example";

AccelByte::Api::UserProfile::CreateUserProfileEasy(ProfileCreateRequest, AccelByte::Api::UserProfile::FCreateUserProfileEasySuccess::CreateLambda([]()
{
    UE_LOG(LogTemp, Log, TEXT("Success"));
}), AccelByte::ErrorDelegate::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
    UE_LOG(LogTemp, Log, TEXT("Error: %d %s"), ErrorCode, *ErrorMessage);
}));
```

See AccelByte::Api::UserProfile::CreateUserProfile().

## Ecommerce
API related to Ecommerce.

**Examples coming soon™® (tm). "Coming soon" is a registered trademark of AccelByte, Inc.**

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
