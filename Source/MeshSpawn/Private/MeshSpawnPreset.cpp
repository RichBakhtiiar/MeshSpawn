// Copyright Bakhtiiar Akhmednov. All Rights Reserved. 2025

#include "MeshSpawnPreset.h"

UMeshSpawnPreset_Factory::UMeshSpawnPreset_Factory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UMeshSpawnPreset::StaticClass();

	bCreateNew = true;
}


