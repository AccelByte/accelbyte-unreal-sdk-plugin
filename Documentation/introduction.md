# Introduction

AccelByte UE4 SDK is a plugin for Unreal Engine 4. 

## Assumptions

This documentation assumes that you are already familiar with modern C++, Unreal Engine 4 (including Blueprint), HTTP, REST API, microservice architecture, OAuth2, OpenID Connect, JWT, WebSocket.

## Features

AccelByte UE4 SDK features:

- Access the API from C++ and blueprint. C++ 
- Provides easy-to-use client APIs for:
    - User authentication (access tokens stored in memory)
    - User management (create user, reset password, verify user.)
    - User profile (create, update, view).
    - Ecommerce (get item categories, get items, create orders, etc.)
    - Lobby (chatting, party management)
- Maybe server API in the future.

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

### `Namespace` vs `GameId` vs `PublisherId`

The backend call it `namespace`, but since namespace can be many things, in here it's called `GameId` and `PublisherId`. They are like AppId in Steam, ProductId in GOG, GameId in GameSparks, TitleId in PlayFab. 

### `UserId` vs `LoginId` vs `Username`

The backend call it `LoginId`, but in here it's called username. It can be email address or phone number. `UserId` is the user unique ID.

### AccelByte IAM

The AccelByte IAM is an identity and access management service for online video games.

It is written in Go.

### AccelByte Platform

AccelByte Platform is a platform service for online video games.

These modules are written in Java to take advantage of the rich eco-system of libraries, mature development environment and tools, and rapid development and flexibility and customization.

### AccelByte Lobby

AccelByte Lobby is for chatting and party management. Unlike other servers which use HTTP, Lobby server uses WebSocket (RFC 6455).

It is also written in Go.

