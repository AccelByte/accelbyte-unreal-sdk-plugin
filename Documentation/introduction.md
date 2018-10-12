# Introduction

AccelByte UE4 SDK is a plugin for Unreal Engine 4. 

## Assumptions

This documentation assumes that you are already familiar with modern C++, Unreal Engine 4 (including Blueprint), HTTP, REST API, microservice architecture, OAuth2, OpenID Connect, JWT.

## Features

AccelByte UE4 SDK features:

- Access the API from C++ and blueprint. C++ 
- Provides easy to use API for:
    - User authentication (access tokens, stored in memory)
    - User management (create user, reset password, etc.)
    - Identity provider (user profile)
    - Ecommerce (online store, view item catalog, create order, etc.)

## Architecture

The classes are categorized as follow:

- The `Model` classes are used for JSON deserialization and serialization using Unreal Engine 4 [JsonUtilities.h](https://api.unrealengine.com/INT/API/Runtime/JsonUtilities/).
- The `Api` classes provides interface to C++ functions.
- The `Blueprint` classes are simple _glue code_ to provide interface to Unreal Engine 4 Blueprint.
- The `Core` classes are for core functionalities (error handler, settings, credential store, and HTTP retry system). 

## Packaging

- `/Plugins` The actual plugin is located in `/Plugins/AccelByteUe4Sdk`. The custom web browser widget plugin `/Plugins/AccelByteCustomWebBrowserWidget` is experimental and untested. Please don't use.
- `/Content` This contains example widget blueprints and other asset.
- `/Source` This contains C++ source code for the examples.
- `/Documentation` This contains Doxyfile and this documentation.
  
## Terminology

> There are only two hard things in Computer Science: cache invalidation and naming things. ―Phil Karlton

### Namespace

Namespace is like AppId in Steam, ProductId in GOG, GameId in GameSparks, TitleId in PlayFab. Since namespace can be many things, let's just call it game ID.

### `UserId` vs `LoginId` vs `Username` vs `Email`

`LoginId` is like username, this can be email or phone, though phone is actually unused. Note that in the future, `LoginId` will be renamed to `Username`, because people are familiar with `Username` so it will be less confusing. `UserId` is the user unique ID.

### AccelByte IAM

The AccelByte IAM is an identity provider, user management, and access management for online video games, so game studios and game publisher publishers can have their own IAM service.

It is written in Go.

### AccelByte Platform

AccelByte Platform is a collection of modules providing backend for games.

These modules are written in Java to take advantage of the rich eco-system of libraries, mature development environment and tools, and rapid development and flexibility and customization that Java allows.

Modules are combined into services allowing for cost effective deployments, where a small service may have multiple modules in a single JVM, and a large service may choose to break out a module into its own JVM to independently scale it without the overhead of having to scale the other modules that are not heavily used.

