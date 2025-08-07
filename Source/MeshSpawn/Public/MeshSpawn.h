// Copyright Bakhtiiar Akhmednov. All Rights Reserved. 2025

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**  
* Mesh spawn module
*/
class FMeshSpawnModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
