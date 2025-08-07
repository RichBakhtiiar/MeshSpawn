// Copyright Bakhtiiar Akhmednov. All Rights Reserved. 2025

#pragma once

#include "CoreMinimal.h"
#include "EdMode.h"
#include "MeshSpawnToolkit.h"

class FViewportClient;

/** Spawn mode names Enum */
namespace ESpawnMode
{
	static const FString Select = TEXT("Select");
	static const FString PaintSelect = TEXT("Paint Select");
	static const FString Transform = TEXT("Transform");
	static const FString Paint = TEXT("Paint");
}

/** Spawn mode colors Enum */
namespace ESpawnModeColor
{
	static const FLinearColor Add = FLinearColor::Green;
	static const FLinearColor Remove = FLinearColor::Red;
	static const FLinearColor Select = FLinearColor::Blue;
	static const FLinearColor Deselect = FLinearColor(1.0f, 1.0f, 0.0f);
}

/** Spawn Edit Mode */
class FMeshSpawnMode : public FEdMode
{
public:
	const static FEditorModeID EM_SpawnModeId;

	/** Constructor */
	FMeshSpawnMode();

	/** FEdMode interface */
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void ActorSelectionChangeNotify() override;
	virtual void Tick(FEditorViewportClient* ViewportClient, float DeltaTime) override;
	virtual bool InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale) override;
	virtual bool MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) override;
	virtual bool StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport);
	virtual bool EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport);
	virtual bool InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
	virtual bool MouseEnter(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y) override;
	virtual bool ProcessCapturedMouseMoves(FEditorViewportClient* InViewportClient, FViewport* InViewport, const TArrayView<FIntPoint>& CapturedMouseMoves) override;
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual bool ShowModeWidgets() const;
	virtual bool UsesTransformWidget() const;
	virtual FVector GetWidgetLocation() const override;
	bool UsesToolkits() const override;

	/** Resets the selected actor's transform */
	void ResetTransform();
	
	/** Returns the spawned primitive components */
	TArray<UPrimitiveComponent*> GetSpawnedComponents();
	
	/** Returns the selected actors */
	TArray<AActor*> GetSelectedActors();
	
	/** Returns the spawned actors */
	TArray<AActor*> GetSpawnedActors() { return SpawnedActors;}
	
	/** Destroys the all or selected actors */
	void DestroyActors(bool InSelected);
	
	/** Returns the selected primitive components */
	TArray<UPrimitiveComponent*> GetSelectedPrimitives();
	
	/** Unregisters the brush actor */
	void UnregisterBrush();
	
	/** Registers the brush actor */
	void RegisterBrush();
	
	/** Spawn mode change event */
	void OnSpawnModeChange(FString InMode);
	
	/** Updates an actor's physics */
	void UpdatePhysics(AActor* InActor, bool bInEnableGravity);
	
	/** Makes the selected actors static */
	void MakeSelectedStatic();

	void SelectPlacedActors(UStaticMesh* InStaticMesh);

	void AddSelectedActor(AActor *InActor);
	void CachePhysics();

private:
	/** Brush actor's material */
	class UMaterialInstanceDynamic* BrushMI = nullptr;
	
	/** Brush actor */
	class UStaticMeshComponent* Brush = nullptr;
	
	/** Is painting enable */
	bool bIsPainting = false;
	
	/** Returns the brush actor position */
	FVector GetPosition();
	
	/** Brush actor uniform size */
	float fBrushSize = 20;
	
	/** Cursor move world direction for brush actor */
	FVector BrushDirection;
	
	/** Brush actor position */
	FVector BrushPosition;
	
	/** Brush actor rotation */
	FVector BrushNormal;
	
	/** Cursor Last world position */
	FVector BrushLastPosition;
	
	/** Brush actor color */
	FLinearColor BrushColor = FLinearColor::Blue;
	
	/** Last place actor world position */
	FVector LastSpawnedPosition;
	
	/** Cursor view position */
	FVector2D CursorPosition;
	
	/** Is Shift key down */
	bool bIsShiftDown = false;
	
	/** Is Ctrl key down */
	bool bIsCtrlDown = false;
	
	/** Is Q key down */
	bool bIsQDown = false;
	
	/** Last selected actor list */
	TArray<AActor*> LastSelectedActors;
	
	/** Last placed acotr list */
	TArray<AActor*> LastSpawnedActors;
	
	/** Placed actor list */
	TArray<AActor*> SpawnedActors;
	
	/** All map's actors */
	TArray<AActor*> LevelActors;
	
	/** Physics dictionary for all map's actors */
	TMap<UPrimitiveComponent*, bool> Physics;
	
	/** Gravity dictionary for all map's actors */
	TMap<UPrimitiveComponent*, bool> Gravities;
	
	/** Position dictionary for all map's actors */
	TMap<UPrimitiveComponent*, FVector> Positions;
	
	/** Rotation dictionary for all map's actors */
	TMap<UPrimitiveComponent*, FRotator> Rotations;
	
	/** Mobility dictionary for all map's actors */
	TMap<UPrimitiveComponent*, EComponentMobility::Type> Mobilities;
	
	/** Returns primitive components of an actor */
	TArray<UPrimitiveComponent*> GetPrimitives(const AActor* InActor);
		
	/** Spawn mode toolkit pointer */
	TSharedPtr<class FMeshSpawnToolkit> ToolkitPtr;

	/** Updates the selected actor's physics */
	void UpdateSelectionPhysics();
	
	/** Resets the physics and transform for a primitive component */
	void ResetPrimitivePhysics(UPrimitiveComponent* InPrim, bool bResetTransform, bool bForceStatic=false);
	
	/** Traces the actor under cursor */
	bool Trace(FHitResult& outHits, FEditorViewportClient *InViewportClient);
	
	/** Resets all actors physics */
	void ResetPhysics();
		
	/** On Level actors added event */
	void OnLevelActorsAdded(AActor* InActor);
	
	/** On level actors deleted event */
	void OnLevelActorsDeleted(AActor* InActor);

	/** On Pre Begin Pie event */
	void OnPreBeginPie(bool InStarted);

	bool bSimulatePhysics = true;
};
