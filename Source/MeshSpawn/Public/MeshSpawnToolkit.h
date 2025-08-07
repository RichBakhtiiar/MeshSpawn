// Copyright Bakhtiiar Akhmednov. All Rights Reserved. 2025

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/BaseToolkit.h"
#include "Engine/StaticMesh.h"
#include "AssetThumbnail.h"
#include "MeshSpawnPreset.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SSlider.h"

class FMeshSpawnMode;

/** Reference mesh struct */
struct FReferenceMesh
{
	UPROPERTY()
	UStaticMesh* StaticMesh = nullptr;
	
	float Chance=100;
	bool bIsAdjusting;
	FVector MinRotate;
	FVector MaxRotate;
	FVector MinScale;
	FVector MaxScale;
};

/** Mesh spawn toolkit */
class FMeshSpawnToolkit : public FModeToolkit
{
public:
	/** Constrcutor */
	FMeshSpawnToolkit();
	
	/** FModeToolkit interface */
	virtual void Init(const TSharedPtr<IToolkitHost>& InitToolkitHost) override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual class FEdMode* GetEditorMode() const override;
	virtual TSharedPtr<class SWidget> GetInlineContent() const override { return ToolkitWidget; }

	/** Current spawn mode */
	FString CurrentSpawnMode;
	
	/** Returns previous spawn mode */
	FString GetLastSpawnMode() const { return LastSpawnMode; }
	
	/** Returns spawn mode */
	FString GetCurrentSpawnMode() const { return CurrentSpawnMode; }

	/** Returns current spawn mode text */
	FText GetCurrentSpawnModeText() const { return FText::FromString(CurrentSpawnMode); }

	/** Binds the toolkit commands */
	void BindCommands();

	/** Returns minimum distance between each placed actor */
	float GetMinDistance();

	/** Returns the current actor to place */
	UStaticMesh* GetRandomMesh();

	/** Changes the spawn mode by name */
	void ChangeMode(FString InSpawnMode);
	
	/** Changes the spawn mode by index */
	void ChangeMode(int InDirection);
	
	/** Returns true if we are selecting the placed actor  */
	bool IsSelectingPlacedActors() { return bSelectPlacedActors; }
	
	/** Returns true if minimum scale random is lock */
	bool IsMinScaleLock () { return bMinScaleLock; }
	
	/** Returns true if maximum scale random is lock */
	bool IsMaxScaleLock () { return bMaxScaleLock; }
	
	/** Returns true if gravity is enable */
	bool IsEnableGravity () { return bEnableGravity; }

	/** Returns true if offsets are enabled */
	bool IsEnableOffsets() const { return bEnableOffsets; }
	
	/** Returns true if use selectd is enable */
	bool IsUseSelected () { return bUseSelected; }
	
	/** Returns normal distance for hited polygon */
	float GetNormalDistance () { return fNormalDistance; }
	
	/** Returns position randomness */
	FVector GetPositionRandom() { return PositionRandom; }
	
	/** Returns rotating randomness */
	FRotator GetRotateRandom() { return RotateRandom; }
	
	/** Returns scale randomness */
	FVector GetScaleRandom() { return ScaleRandom; }
	
	/** Returns normal rotation offset */
	FRotator GetNormalRotation() { return NormalRotation; }

	/** Sets the next actor to place */
	void SetRandomMesh();

	/** Sets the spawn mode */
	void SetEdMode(FMeshSpawnMode* InEdMode) { EdMode = InEdMode; }

	/** Bakes the places actors into instance mesh */
	void BakeToInstanceMesh(bool BakeSelected);

	void UnbindAll();

private:
	/** Previous spawn mode */
	FString LastSpawnMode;

	TSharedPtr<SListView<TSharedPtr<FReferenceMesh>>> ReferenceMeshesListView; 

	/** List of reference meshes */
	TArray<TSharedPtr<FReferenceMesh>> ReferenceMeshes;
	
	/** Minimum position random */
	FVector MinPositionOffset = FVector::ZeroVector;
	
	/** Maximum position random */
	FVector MaxPositionOffset = FVector::ZeroVector;
	
	/** Position random */
	FVector PositionRandom = FVector::ZeroVector;

	/** Minimum rotation random */
	FRotator MinRotateOffset = FRotator::ZeroRotator;
	
	/** Maximum rotation random */
	FRotator MaxRotateOffset = FRotator::ZeroRotator;
	
	/** Rotation random */
	FRotator RotateRandom = FRotator::ZeroRotator;

	/** Minimum scale random */
	FVector MinScaleOffset = FVector::OneVector;
	
	/** Maximum scale random */
	FVector MaxScaleOffset = FVector::OneVector;
	
	/** Scale random */
	FVector ScaleRandom = FVector::OneVector;

	/** Nornaml rotation offset */
	FRotator NormalRotation = FRotator::ZeroRotator;

	/** Spawn mode */
	FMeshSpawnMode* EdMode = nullptr;

	/** Normal distance for hited polygon */
	float fNormalDistance = 0;

	/** Selected reference mesh index */
	int SelectedMeshIndex = -1;
	
	/** Minimum distance to last placed actor */
	float fMinDistance = 2;
	
	/** Is select placed actor enable */
	bool bSelectPlacedActors = true;
	
	/** Is minimum scale lock */
	bool bMinScaleLock = false;
	
	/** Is maximum scale lock */
	bool bMaxScaleLock = false;
	
	/** Is gravity enable */
	bool bEnableGravity = true;

	/** Is offsets enable */
	bool bEnableOffsets = true;
	
	/** Is use selected enable */
	bool bUseSelected = false;

	/** Spawn mode name */
	FString SpawnMode;

	/** Selected reference mesh */
	UStaticMesh* PickedMesh = nullptr;
	
	/** Is percents are relative */
	bool bIsPercentRelative;
	
	/** SSlider references for reference mesh chance */
	TMap<TSharedPtr<FReferenceMesh>, TSharedPtr<SSlider>> Sliders;
	
	/** Spawn mode SCombobox reference */
	TSharedPtr<SComboBox<TSharedPtr<FString>>> SpawnModeComboBox;
	
	/** Spawn mode names */
	TArray<TSharedPtr<FString>> SpawnModes;
	
	/** Toolkit's SWidget */
	TSharedPtr<SWidget> ToolkitWidget;
	
	/** Current spawn mode index */
	int SpawnModeIndex = 0;
	
	/** Reference mesh thumbnail pool */
	TSharedPtr<FAssetThumbnailPool> ThumbnailPool;
	
	/** Generates the toolkit widget */
	TSharedRef<SWidget> GenerateWidget(TSharedPtr<FString> InItem);
	
	/** Spawn mode change event */
	void ModeChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo);
	
	/** Returns reference meshes */
	TArray<TSharedPtr<FReferenceMesh>> GetReferenceMeshes() const;
	
	/** Reference mesh widget delegate */
	TSharedRef<ITableRow> GetReferenceMeshWidget(TSharedPtr<FReferenceMesh> InItem, const TSharedRef<STableViewBase>& OwnerTable);

	/** Creates paint mode widget */
	TSharedPtr<SWidget> CreatePaintModeWidget();
	
	/** Creates shared widgets */
	TSharedPtr<SWidget> CreateSharedWidget();

	private:
		FReply OnPaintClicked();
		// Brush size accessors for the UI
		TOptional<float> GetBrushSize() const;
		void OnBrushSizeChanged(float NewValue);
		float BrushSize = 512.0f;
};
