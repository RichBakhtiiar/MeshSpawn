// Copyright Bakhtiiar Akhmednov. All Rights Reserved. 2025

#include "MeshSpawnCommands.h"

#define LOCTEXT_NAMESPACE "MeshSpawnCommands"

void FMeshSpawnCommands::RegisterCommands()
{
	UI_COMMAND(PaintCommand, "  Spawn  ", "Spawn Tool", EUserInterfaceActionType::ToggleButton, FInputChord());
}
#undef LOCTEXT_NAMESPACE
