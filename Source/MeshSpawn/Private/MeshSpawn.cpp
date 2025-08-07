// Copyright Bakhtiiar Akhmednov. All Rights Reserved. 2025

#include "MeshSpawn.h"
#include "MeshSpawnMode.h"
#include "MeshSpawnCommands.h"
#include "MeshSpawnStyle.h"

#define LOCTEXT_NAMESPACE "FMeshSpawnModule"

void FMeshSpawnModule::StartupModule()
{
	FMeshSpawnStyle::Initialize();
	FMeshSpawnStyle::ReloadTextures();
	FMeshSpawnCommands::Register();

	// This code will executes after module is loaded into memory, the exact timing is specified in the .uplugin file per-module
	FEditorModeRegistry::Get().RegisterMode<FMeshSpawnMode>(
		FMeshSpawnMode::EM_SpawnModeId, 
		LOCTEXT("MeshSpawnEdModeName", "Mesh Spawn"),
		FSlateIcon(FMeshSpawnStyle::GetStyleSetName(), "ClassIcon.MeshSpawnMode", "ClassThumbnail.MeshSpawnMode"),
		true);
}

void FMeshSpawnModule::ShutdownModule()
{
	FMeshSpawnStyle::Shutdown();
	FMeshSpawnCommands::Unregister();

	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FEditorModeRegistry::Get().UnregisterMode(FMeshSpawnMode::EM_SpawnModeId);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMeshSpawnModule, MeshSpawn)
