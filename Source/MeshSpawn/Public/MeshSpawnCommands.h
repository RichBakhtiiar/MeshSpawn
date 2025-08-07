// Copyright Bakhtiiar Akhmednov. All Rights Reserved. 2025

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Framework/Commands/UICommandInfo.h"
#include "MeshSpawnStyle.h"

/**
 * Mesh spawn mode commands
 */
class FMeshSpawnCommands : public TCommands< FMeshSpawnCommands >
{

public:
	/** Constructor */
	FMeshSpawnCommands()
		: TCommands<FMeshSpawnCommands>(
			TEXT("MeshSpawn"), // Context name for fast lookup
			NSLOCTEXT("Contexts", "MeshSpawn", "Mesh Spawn"), // Localized context name for displaying
			NAME_None, // Parent
			FMeshSpawnStyle::GetStyleSetName()
			)
	{
	}

	/** Command interface */
	virtual void RegisterCommands() override;
	
	/** Paint place command */
	TSharedPtr<FUICommandInfo> PaintCommand;

};
