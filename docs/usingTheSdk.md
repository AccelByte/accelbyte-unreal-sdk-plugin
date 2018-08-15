## C++

### a. OAuth2 Login
Before you use other services, you must call `ClientLogin()` function first.
`ClientLogin()` function is actually OAuth2 Login to our service. It will login against **ClientID** and **ClientSecret** from `DefaultEngine.ini` file under `JusticeSDK` section.
If you build Game Client and Dedicated Server, you should call this function. There is **no** `ServerLogin` Function.

For more information about OAuth2 you can follow [this link](https://oauth.net/2/) and [this link](https://docs.microsoft.com/en-us/azure/active-directory/develop/active-directory-protocols-oauth-code).

On your start up game class, which name by default is `YourAwesomeGameModeBase.cpp` with class named `AYourAwesomeGameModeBase`, add this code on `BeginPlay` Function or any function that calls on startup.
<pre class="prettyprint lang-cpp">
void AYourAwesomeGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    JusticeIdentity::ClientLogin();
}
void AYourAwesomeGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    JusticeIdentity::ClientLogout();
}
</pre>

### b. User Login
Here is what you do to call login with email and password
<pre class="prettyprint lang-cpp">
void AJusticeSDKDemoGameModeBase::BeginPlay()
{
	...
    JusticeIdentity::UserLogin(TEXT("user@example.com"), 
      TEXT("userpassword"),       
      FUserLoginCompleteDelegate::CreateLambda([](bool IsSuccess, FString ErrorString, UOAuthTokenJustice* token) {
  		// this will be called when login request receive a response 
    }));
    ...
}
</pre>

### c. Device Login
If you’d like to login without using user and password, you have an option to use Device Login. Device Id `FGenericPlatformMisc::GetDeviceId()` will act as player’s identifier. The example case is if a player wants to try our game out without doing registration.
<pre class="prettyprint lang-cpp">
void AJusticeSDKDemoGameModeBase::BeginPlay()
{
	...
    JusticeIdentity::DeviceLogin(
      FUserLoginCompleteDelegate::CreateLambda([](bool IsSuccess, FString ErrorString, UOAuthTokenJustice* token) {
  		// this will be called when login request receive a response 
    }));
    ...
}
</pre>

## Blueprint
We are able to call all Justice SDK function from the blueprint. We have 2 different kinds of blueprint function:

1.  Blueprint function with delegates parameter 
1.  Blueprint async function with multiple output pins

<script src="https://cdn.rawgit.com/google/code-prettify/master/loader/run_prettify.js"></script>