// Copyright 2019 Google LLC All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "AgonesHook.h"
#include "Engine/World.h"

class FAgonesModule : public IModuleInterface
{
public:

	/** Publicly exposes hook to communicate with the Agones sidecar */
	AGONES_API static FAgonesHook& GetHook();

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void OnWorldInitialized(UWorld* World, UWorld::InitializationValues IVS);

	/** Communicates with the Agones sidecar. */
	TSharedPtr<class FAgonesHook> HookPtr;

	/** Singleton for the module while loaded and available */
	static FAgonesModule* ModuleSingleton;
};
