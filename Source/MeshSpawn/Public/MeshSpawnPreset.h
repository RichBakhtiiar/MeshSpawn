// Copyright Bakhtiiar Akhmednov. All Rights Reserved. 2025

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeCategories.h"
#include "Engine/DataAsset.h"
#include "Engine/StaticMesh.h"
#include "Factories/Factory.h"
#include "UObject/Object.h"
#include "MeshSpawnPreset.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct MESHSPAWN_API FPLPaintObject
{
	GENERATED_BODY()
public:
	FPLPaintObject(){}
	FPLPaintObject(UStaticMesh* InStaticMesh, float InChance)
		: StaticNesh(InStaticMesh)
		, Chance(InChance)
	{
	}
	
	UPROPERTY(EditAnywhere, Category="Preset")
	UStaticMesh* StaticNesh;

	UPROPERTY(EditAnywhere, Category="Preset", meta=(UIMin="0", UIMax = "100"))
	float Chance=100;
};

UCLASS(BlueprintType)
class MESHSPAWN_API UMeshSpawnPreset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category="Preset")
	TArray<FPLPaintObject> PaintObjects;
};

UCLASS()
class MESHSPAWN_API UMeshSpawnPreset_Factory : public UFactory
{
	GENERATED_UCLASS_BODY()
public:
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override
	{
		auto Preset = NewObject<UMeshSpawnPreset>(InParent, InClass, InName, Flags);
		Preset->Modify();
		Preset->MarkPackageDirty();
		Preset->PostEditChange();
		return Preset;
	}

	virtual FText GetDisplayName() const override
	{
		return FText::FromString("Physical Design Preset");
	}

	virtual uint32 GetMenuCategories() const override
	{
		return EAssetTypeCategories::Misc;
	}

	virtual bool CanCreateNew() const override
	{
		return true;
	}
};
