// Copyright Bakhtiiar Akhmednov. All Rights Reserved. 2025

#include "MeshSpawnMode.h"
#include "Engine/World.h"
#include "MeshSpawnToolkit.h"
#include "Toolkits/ToolkitManager.h"
#include "EditorModeManager.h"
#include "ToolContextInterfaces.h"
#include "Engine/Selection.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Editor.h"
#include "EditorViewportClient.h"
#include "SceneView.h"
#include "Engine/StaticMeshActor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Selection.h"
#include "Chaos/PBDRigidsEvolutionFwd.h"
#include "PBDRigidsSolver.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Physics/Experimental/PhysScene_Chaos.h"

#define LOCTEXT_NAMESPACE "FMeshSpawnMode"

const FEditorModeID FMeshSpawnMode::EM_SpawnModeId = TEXT("MeshSpawnMode");

FMeshSpawnMode::FMeshSpawnMode()
{
	UMaterial* BrushMaterial = LoadObject<UMaterial>(nullptr, TEXT("/Engine/EditorLandscapeResources/FoliageBrushSphereMaterial.FoliageBrushSphereMaterial"), nullptr, LOAD_None, nullptr);
	BrushMI = UMaterialInstanceDynamic::Create(BrushMaterial, GetTransientPackage());
	BrushMI->SetVectorParameterValue(TEXT("HighlightColor"), FLinearColor::Blue);
	check(BrushMI != nullptr);

	Brush = NewObject<UStaticMeshComponent>(GetTransientPackage(), TEXT("SphereBrushComponent"));
	Brush->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	Brush->SetCollisionObjectType(ECC_WorldDynamic);
	Brush->SetMaterial(0, BrushMI);
	Brush->SetAbsolute(true, true, true);
	Brush->CastShadow = false;
}

void FMeshSpawnMode::OnLevelActorsAdded(AActor* InActor)
{
	if (InActor && InActor->IsA<AStaticMeshActor>() && !InActor->IsActorBeingDestroyed())
	{
		if (InActor->GetLevel() == GetWorld()->GetCurrentLevel())
		{
			if (!SpawnedActors.Contains(InActor))
			{
				UpdatePhysics(InActor, ToolkitPtr->IsEnableGravity());
				SpawnedActors.Add(InActor);

			}
		}
	}
}

void FMeshSpawnMode::OnLevelActorsDeleted(AActor* InActor)
{
	if (SpawnedActors.Contains(InActor))
	{
		SpawnedActors.Remove(InActor);
	}

	if (LastSpawnedActors.Contains(InActor))
	{
		LastSpawnedActors.Remove(InActor);
	}

	if (LastSelectedActors.Contains(InActor))
	{
		LastSelectedActors.Remove(InActor);
	}
	if (LevelActors.Contains(InActor))
	{
		LevelActors.Remove(InActor);
	}

	auto Prims = GetPrimitives(InActor);
	for (auto& Prim : Prims)
	{
		if (Mobilities.Contains(Prim))
		{
			Mobilities.Remove(Prim);
		}
		if (Physics.Contains(Prim))
		{
			Physics.Remove(Prim);
		}
		if (Gravities.Contains(Prim))
		{
			Gravities.Remove(Prim);
		}
		if (Positions.Contains(Prim))
		{
			Positions.Remove(Prim);
		}
		if (Rotations.Contains(Prim))
		{
			Rotations.Remove(Prim);
		}
	}
}

void FMeshSpawnMode::OnPreBeginPie(bool InStarted)
{
	GetModeManager()->ActivateMode(EM_SpawnModeId, true);
}

void FMeshSpawnMode::Enter()
{
	FEdMode::Enter();
	GEngine->OnLevelActorAdded().AddSP(this, &FMeshSpawnMode::OnLevelActorsAdded);
	GEngine->OnLevelActorDeleted().AddSP(this, &FMeshSpawnMode::OnLevelActorsDeleted);
	FEditorDelegates::PreBeginPIE.AddSP(this, &FMeshSpawnMode::OnPreBeginPie);

	if (IsValid(Brush))
	{
		Brush->SetVisibility(true);
		if (!Brush->IsRegistered())
		{
			Brush->RegisterComponentWithWorld(GetWorld());
		}
	}

	if (!Toolkit.IsValid() && UsesToolkits())
	{
		ToolkitPtr = MakeShareable(new FMeshSpawnToolkit);
		Toolkit = ToolkitPtr;
		ToolkitPtr.Get()->SetEdMode(this);
		Toolkit->Init(Owner->GetToolkitHost());
	}

	CachePhysics();
}

void FMeshSpawnMode::Exit()
{
	GEngine->OnLevelActorAdded().RemoveAll(this);
	GEngine->OnLevelActorDeleted().RemoveAll(this);
	FEditorDelegates::PreBeginPIE.RemoveAll(this);

	// Reset physics-related state
	ResetPhysics();

	// Clear all arrays that might hold references to destroyed actors
	Mobilities.Reset();
	Gravities.Reset();
	Physics.Reset();
	Positions.Reset();
	Rotations.Reset();
	LevelActors.Reset();
	LastSpawnedActors.Reset();
	LastSelectedActors.Reset();
	SpawnedActors.Reset();

	// Remove the brush component
	if (Brush)
	{
		Brush->UnregisterComponent();
		Brush->SetStaticMesh(nullptr);
	}

	// Clean up the toolkit and unbind references
	if (Toolkit.IsValid())
	{
		TSharedPtr<FMeshSpawnToolkit> TypedToolkit = StaticCastSharedPtr<FMeshSpawnToolkit>(Toolkit);
		if (TypedToolkit.IsValid())
		{
			TypedToolkit->UnbindAll();
		}

		FToolkitManager::Get().CloseToolkit(Toolkit.ToSharedRef());
		Toolkit.Reset();
	}

	// Always call base class Exit to finish cleanup
	FEdMode::Exit();
}

void FMeshSpawnMode::ActorSelectionChangeNotify()
{
	FEdMode::ActorSelectionChangeNotify();
	
	if (ToolkitPtr.IsValid())
	{
		if (ToolkitPtr.Get()->IsSelectingPlacedActors())
		{
			TArray<AActor*> Actors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), Actors);
			for (auto& Actor : Actors)
			{
				if (!SpawnedActors.Contains(Actor))
				{
					GEditor->SelectActor(Actor, false, false);
				}
			}
		}
	}

	TArray<AActor*> Actors = GetSelectedActors();
	bool SelectionChanged = false;
	for (auto& Actor : Actors)
	{
		if (!LastSelectedActors.Contains(Actor))
		{
			SelectionChanged = true;
			break;
		}
	}
	
	if (SelectionChanged)
	{
		LastSelectedActors = GetSelectedActors();
		UpdateSelectionPhysics();
	}
}

void FMeshSpawnMode::Tick(FEditorViewportClient* ViewportClient, float DeltaTime)
{
	if (GEditor->IsSimulateInEditorInProgress() || GEditor->IsPlaySessionInProgress()
		|| GEditor->IsPlaySessionRequestQueued())
	{
		GetModeManager()->ActivateMode(EM_SpawnModeId, true);
		return;
	}

	if (ToolkitPtr.IsValid())
	{
		FHitResult Hit;
		bool Hited = Trace(Hit, ViewportClient);

		if (ToolkitPtr.Get()->GetCurrentSpawnMode() == ESpawnMode::PaintSelect)
		{
			if (Hited && bIsPainting)
			{
				if (ToolkitPtr.Get()->IsSelectingPlacedActors())
				{
					if (SpawnedActors.Contains(Hit.GetActor()))
					{
						GEditor->SelectActor(Hit.GetActor(), !bIsCtrlDown, true);
					}
				}
				else
				{
					GEditor->SelectActor(Hit.GetActor(), !bIsCtrlDown, true);
				}
			}
		}
		else if (ToolkitPtr.Get()->GetCurrentSpawnMode() == ESpawnMode::Paint)
		{
			if (Hited)
			{
				auto Rotation = ToolkitPtr.Get()->GetRotateRandom();
				auto PickedMesh = ToolkitPtr.Get()->GetRandomMesh();

				if (IsValid(PickedMesh))
				{
					if (IsValid(Brush))
					{
						if (!Brush->IsRegistered())
						{
							Brush->RegisterComponentWithWorld(GetWorld());
						}

						Brush->SetStaticMesh(PickedMesh);
						Brush->SetWorldLocation(GetPosition());
						Brush->SetWorldScale3D(ToolkitPtr.Get()->GetScaleRandom());

						if (bIsShiftDown && bIsCtrlDown)
						{
							Rotation = UKismetMathLibrary::FindLookAtRotation(
								BrushPosition,
								BrushPosition + BrushDirection) + ToolkitPtr.Get()->GetNormalRotation();
						}
						else if (bIsShiftDown)
						{
							Rotation = UKismetMathLibrary::FindLookAtRotation(
								BrushPosition,
								BrushPosition + BrushNormal) + ToolkitPtr.Get()->GetNormalRotation();
						}

						Brush->SetWorldRotation(Rotation);
					}
					else
					{
						Brush->SetVisibility(false);
					}

					if (bIsPainting)
					{
						if (bIsCtrlDown && !bIsShiftDown && !bIsQDown)
						{
							if (SpawnedActors.Contains(Hit.GetActor()))
							{
								GetWorld()->DestroyActor(Hit.GetActor());
							}
						}
						else
						{
							auto ReferenceMesh = ToolkitPtr.Get()->GetRandomMesh();
							if (IsValid(ReferenceMesh))
							{
								float dist = FVector::Dist(GetPosition(), LastSpawnedPosition);
								if (dist > ToolkitPtr.Get()->GetMinDistance())
								{
									LastSpawnedPosition = GetPosition();

									FActorSpawnParameters Params = FActorSpawnParameters();
									FString Name = FString::Format(TEXT("Actor_{0}"), { ReferenceMesh->GetFName().ToString() });
									FName Fname = MakeUniqueObjectName(nullptr, AStaticMeshActor::StaticClass(), FName(*Name));
									Params.Name = Fname;
									Params.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;

									AStaticMeshActor* Actor = GetWorld()->SpawnActor<AStaticMeshActor>(GetPosition(), Rotation, Params);

									Actor->SetActorLabel(Fname.ToString());
									Actor->SetActorScale3D(ToolkitPtr.Get()->GetScaleRandom());
									LastSpawnedActors.Add(Actor);
									Actor->GetStaticMeshComponent()->SetStaticMesh(ReferenceMesh);

									UpdatePhysics(Actor, ToolkitPtr.Get()->IsEnableGravity());
									ToolkitPtr.Get()->SetRandomMesh();
								}
							}
						}

						if (IsValid(Brush))
						{
							if (!Brush->IsRegistered())
							{
								Brush->RegisterComponentWithWorld(GetWorld());
							}
							Brush->SetStaticMesh(ToolkitPtr.Get()->GetRandomMesh());
						}
					}
				}
			}
		}
		auto World = GetWorld();
		auto Solver = World->GetPhysicsScene()->GetSolver();

		if (ToolkitPtr.Get()->GetCurrentSpawnMode() == ESpawnMode::Paint)
		{
			TArray<AActor*> Actors = GetSelectedActors();
			Solver->StartingSceneSimulation();
			UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), LevelActors);
				if (LevelActors.Num() > 0)
				{
					for (int i = 0; i < LevelActors.Num(); ++i)
					{
						AActor* LevelActor = LevelActors[i];
						if (LevelActor)
						{
							if (AGeometryCollectionActor* CollectionActor = Cast<AGeometryCollectionActor>(LevelActor))
							{
								auto CollectionComponent = CollectionActor->GetGeometryCollectionComponent();
							}
						}
					}
				}
			}
			Solver->AdvanceAndDispatch_External(DeltaTime);
		}

		if (ToolkitPtr.Get()->GetCurrentSpawnMode() != ESpawnMode::Transform)
		{
			DrawDebugLine(GetWorld(), BrushPosition, (BrushPosition + BrushDirection), BrushColor.ToFColor(false), false, -1, 0, 5);
		}
}

bool FMeshSpawnMode::InputDelta(FEditorViewportClient* InViewportClient, FViewport* InViewport, FVector& InDrag, FRotator& InRot, FVector& InScale)
{
	bool Handled = FEdMode::InputDelta(InViewportClient, InViewport, InDrag, InRot, InScale);
	
	if (!InScale.IsZero())
	{
		auto Prims = GetSelectedPrimitives();
		for (auto& Prim : Prims)
		{
			FVector Pivot = GetWidgetLocation();
			FVector Direction = (Prim->GetComponentLocation() - Pivot) * 100;
			float Sign = InScale.X + InScale.Y + InScale.Z;
			Prim->AddForce(Sign * Direction, NAME_None, true);
		}

		Handled = true;
	}

	return Handled;
}

bool FMeshSpawnMode::MouseMove(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y)
{
	CursorPosition.Set(x, y);
	return FEdMode::MouseMove(ViewportClient, Viewport, x, y);
}

bool FMeshSpawnMode::StartTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	GEditor->EndTransaction();
	GEditor->BeginTransaction(LOCTEXT("MeshSpawnMode_Transformation", "Transformation"));
	
	return FEdMode::StartTracking(InViewportClient, InViewport);
}

bool FMeshSpawnMode::EndTracking(FEditorViewportClient* InViewportClient, FViewport* InViewport)
{
	GEditor->EndTransaction();
	GEditor->NoteSelectionChange();
	
	return 	FEdMode::EndTracking(InViewportClient, InViewport);
}

bool FMeshSpawnMode::InputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event)
{
	bIsShiftDown = IsShiftDown(Viewport);
	
	bool bHandled = false;
	
	if (bIsCtrlDown != IsCtrlDown(Viewport))
	{
		bIsCtrlDown = IsCtrlDown(Viewport);
	}

	if (ToolkitPtr.IsValid())
	{
		if (Key == EKeys::Tab && Event == EInputEvent::IE_Pressed)
		{
			ToolkitPtr.Get()->ChangeMode(bIsShiftDown ? -1 : 1);
			bHandled = true;
		}

		if (Key == EKeys::Q && Event == EInputEvent::IE_Pressed)
		{
			bHandled = true;
			bIsQDown = true;

			ToolkitPtr.Get()->ChangeMode(ESpawnMode::PaintSelect);
		}
		else if (Key == EKeys::Q && Event == EInputEvent::IE_Released)
		{
			bIsQDown = false;
			ToolkitPtr.Get()->ChangeMode(ToolkitPtr.Get()->GetLastSpawnMode());
		}

		if (ToolkitPtr.Get()->GetCurrentSpawnMode() == ESpawnMode::Paint)
		{
			if (!bIsCtrlDown || (bIsCtrlDown && bIsShiftDown))
			{
				BrushColor = ESpawnModeColor::Add;
				Brush->SetVisibility(true);
			}
			else
			{
				Brush->SetVisibility(false);
				BrushColor = ESpawnModeColor::Remove;

			}
		}
		else if (ToolkitPtr.Get()->GetCurrentSpawnMode() == ESpawnMode::PaintSelect ||
				 ToolkitPtr.Get()->GetCurrentSpawnMode() == ESpawnMode::Select)
		{
			Brush->SetVisibility(false);

			if (!bIsCtrlDown)
			{
				BrushColor = ESpawnModeColor::Select;
			}
			else
			{
				BrushColor = ESpawnModeColor::Deselect;
			}
		}
		else
		{
			Brush->SetVisibility(false);
		}

		BrushMI->SetVectorParameterValue(TEXT("HighlightColor"), BrushColor);

		if (Key == EKeys::LeftMouseButton && Event == IE_Pressed && !IsAltDown(Viewport))
		{

			if (ToolkitPtr && ToolkitPtr.Get()->GetCurrentSpawnMode() == ESpawnMode::Paint ||
				ToolkitPtr && ToolkitPtr.Get()->GetCurrentSpawnMode() == ESpawnMode::PaintSelect)
			{
				bHandled = true;
			}

			bIsPainting = true;

			if (bIsQDown)
			{
				GEditor->EndTransaction();
				GEditor->BeginTransaction(LOCTEXT("MeshSpawnMode_Paint", "Select Objects"));
			}
			else
			{
				GEditor->EndTransaction();
				GEditor->BeginTransaction(LOCTEXT("MeshSpawnMode_Paint", "Add Objects"));
			}
		}

		if (Key == EKeys::LeftMouseButton && Event == IE_Released && !IsAltDown(Viewport))
		{
			if (bIsPainting)
			{
				GEditor->EndTransaction();
			}

			bIsPainting = false;
			LastSpawnedActors.Reset();
		}
	}

	return bHandled? bHandled : FEdMode::InputKey(ViewportClient, Viewport, Key, Event);;
}

bool FMeshSpawnMode::MouseEnter(FEditorViewportClient* ViewportClient, FViewport* Viewport, int32 x, int32 y)
{
	return FEdMode::MouseEnter(ViewportClient, Viewport, x, y);;
}

bool FMeshSpawnMode::ProcessCapturedMouseMoves(FEditorViewportClient* InViewportClient, FViewport* InViewport, const TArrayView<FIntPoint>& CapturedMouseMoves)
{
	bool bHandled = FEdMode::ProcessCapturedMouseMoves(InViewportClient, InViewport, CapturedMouseMoves);

	for (auto& move : CapturedMouseMoves)
	{
		CursorPosition.Set(move.X, move.Y);
	}

	return bHandled;
}

bool FMeshSpawnMode::Trace(FHitResult& outHits, FEditorViewportClient *InViewportClient)
{
	FSceneViewFamilyContext ViewContext(FSceneViewFamilyContext::ConstructionValues(
				InViewportClient->Viewport, 
				InViewportClient->GetScene(), 
				InViewportClient->EngineShowFlags)
				.SetRealtimeUpdate(InViewportClient->IsRealtime()));

	FSceneView* View = InViewportClient->CalcSceneView(&ViewContext);
	FViewportCursorLocation Cursor(View, InViewportClient, CursorPosition.X, CursorPosition.Y);

	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredActors(LastSpawnedActors);

	bool Hited = GetWorld()->LineTraceSingleByChannel(
		outHits, 
		Cursor.GetOrigin(), 
		Cursor.GetOrigin() + Cursor.GetDirection() * HALF_WORLD_MAX, 
		ECollisionChannel::ECC_Visibility, Params);

	if (Hited)
	{
		BrushLastPosition = BrushPosition;
		BrushPosition = outHits.ImpactPoint;
		BrushDirection = BrushLastPosition - BrushPosition;
		BrushNormal = outHits.ImpactNormal;
	}

	return Hited;
}

void FMeshSpawnMode::AddReferencedObjects(FReferenceCollector& Collector)
{
	FEdMode::AddReferencedObjects(Collector);
	Collector.AddReferencedObject(Brush);
}

bool FMeshSpawnMode::ShowModeWidgets() const
{
	if (ToolkitPtr.IsValid())
	{
		if (ToolkitPtr.Get()->GetCurrentSpawnMode() == ESpawnMode::Transform)
		{
			return true;
		}
	}

	return false;
}

bool FMeshSpawnMode::UsesTransformWidget() const
{
	if (ToolkitPtr.IsValid())
	{
		if (ToolkitPtr.Get()->GetCurrentSpawnMode() == ESpawnMode::Transform)
		{
			return true;
		}
	}

	return false;
}

FVector FMeshSpawnMode::GetWidgetLocation() const
{
	if (LastSelectedActors.Num() > 0)
	{
		if (LastSelectedActors.Last())
		{
			return LastSelectedActors.Last()->GetActorLocation();
		}
	}
	return FEdMode::GetWidgetLocation();
}

bool FMeshSpawnMode::UsesToolkits() const
{
	return true;
}

void FMeshSpawnMode::RegisterBrush()
{
	if (ToolkitPtr.IsValid())
	{
		if (Brush && !Brush->IsRegistered())
		{
			Brush->RegisterComponentWithWorld(GetWorld());
			auto PickedMesh = ToolkitPtr.Get()->GetRandomMesh();

			if (IsValid(PickedMesh))
			{
				Brush->SetStaticMesh(PickedMesh);
				Brush->SetVisibility(true);
			}
		}
	}
}

void FMeshSpawnMode::OnSpawnModeChange(FString InMode)
{
	if (InMode == ESpawnMode::PaintSelect || InMode == ESpawnMode::Select)
	{
		bSimulatePhysics = false;
	}
	else
	{
		bSimulatePhysics = true;
	}
	if (InMode == ESpawnMode::Transform)
	{
		GEditor->NoteSelectionChange();
	}
}

FVector FMeshSpawnMode::GetPosition()
{
	if (ToolkitPtr.IsValid())
	{
		return BrushPosition + (BrushNormal * ToolkitPtr.Get()->GetNormalDistance()) + ToolkitPtr.Get()->GetPositionRandom();
	}

	return BrushPosition;
}

TArray<UPrimitiveComponent*> FMeshSpawnMode::GetPrimitives(const AActor* InActor)
{
	TArray<UPrimitiveComponent*> Comps;
	InActor->GetComponents<UPrimitiveComponent>(Comps, false);
	TArray<UPrimitiveComponent*> Primitives;

	for (auto Comp : Comps)
	{
		if (Comp->IsA<UPrimitiveComponent>())
		{
			UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Comp);
			if (Prim)
			{
				Primitives.Add(Prim);
			}
		}
	}

	return Primitives;
}

void FMeshSpawnMode::DestroyActors(bool InSelected)
{
	TArray<AActor*> Actors;

	if (InSelected)
	{
		Actors = GetSelectedActors();
	}
	else
	{
		Actors = GetSpawnedActors();
	}

	for (auto& Actor : Actors)
	{
		SpawnedActors.Remove(Actor);
		LevelActors.Remove(Actor);
		Actor->Destroy();
	}
}

TArray<UPrimitiveComponent*> FMeshSpawnMode::GetSelectedPrimitives()
{
	TArray<UPrimitiveComponent*> Primitives;
	USelection* SelectedActors = GEditor->GetSelectedActors();

	// For each selected actor
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		if (AActor* LevelActor = Cast<AActor>(*Iter))
		{

			auto Components = LevelActor->GetComponents();
			TArray<UPrimitiveComponent*> Prim;

			for (auto& Comps : Components)
			{
				if (!Comps->IsA<UGeometryCollectionComponent>())
				{
					if (Comps->IsA<UPrimitiveComponent>())
					{
						UPrimitiveComponent* Prims = Cast<UPrimitiveComponent>(Comps);
						if (Prims)
						{
							Prim.Add(Prims);
						}
					}
				}
			}
		}
	}

	return Primitives;
}

void FMeshSpawnMode::UnregisterBrush()
{
	if (Brush && Brush->IsRegistered())
	{
		Brush->SetVisibility(false);
		Brush->UnregisterComponent();

	}
}

TArray<AActor*> FMeshSpawnMode::GetSelectedActors()
{
	TArray<AActor*> Actors;
	USelection* SelectedActors = GEditor->GetSelectedActors();

	// For each selected actor
	for (FSelectionIterator Iter(*SelectedActors); Iter; ++Iter)
	{
		if (AActor* LevelActor = Cast<AActor>(*Iter))
		{
			Actors.Add(LevelActor);
		}
	}

	return Actors;
}

void FMeshSpawnMode::UpdatePhysics(AActor* InActor, bool bInEnableGravity)
{
	auto Prims = GetPrimitives(InActor);
	for (auto& Prim : Prims)
	{
		if (!Mobilities.Contains(Prim))
		{
			Mobilities.Add(Prim, Prim->Mobility.GetValue());
		}
		if (!Physics.Contains(Prim))
		{
			Physics.Add(Prim, Prim->IsSimulatingPhysics());
		}
		if (!Gravities.Contains(Prim))
		{
			Gravities.Add(Prim, Prim->IsGravityEnabled());
		}
		if (!Positions.Contains(Prim))
		{
			Positions.Add(Prim, Prim->GetComponentLocation());
		}
		if (!Rotations.Contains(Prim))
		{
			Rotations.Add(Prim, Prim->GetComponentRotation());
		}

		Prim->SetSimulatePhysics(bSimulatePhysics);
		Prim->SetEnableGravity(bInEnableGravity);
		Prim->SetMobility(EComponentMobility::Movable);

	}
}

void FMeshSpawnMode::MakeSelectedStatic()
{
	auto Actors = GetSelectedActors();
	for (auto& Actor : Actors)
	{
		GEditor->SelectActor(Actor, false, true);

		auto Prims = GetPrimitives(Actor);
		for (auto& Prim : Prims)
		{
			ResetPrimitivePhysics(Prim, false, true);
		}
		
		SpawnedActors.Remove(Actor);
		LastSpawnedActors.Remove(Actor);
	}
}

void FMeshSpawnMode::SelectPlacedActors(UStaticMesh* InStaticMesh)
{
	GEditor->SelectNone(false, true, false);
	if (InStaticMesh)
	{
		auto Components = GetSpawnedComponents();
		for (auto& Component : Components)
		{
			UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Component);
			if (StaticMeshComponent)
			{
				if (StaticMeshComponent->GetStaticMesh() == InStaticMesh)
				{
					GEditor->SelectActor(Component->GetOwner(),true, false);
				}
			}
		}
	}
	else
	{
		for (auto& Actor : SpawnedActors)
		{
			GEditor->SelectActor(Actor,true, false);
		}
	}
}

void FMeshSpawnMode::AddSelectedActor(AActor* InActor)
{
	OnLevelActorsAdded(InActor);
}

void FMeshSpawnMode::CachePhysics()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), LevelActors);
	for (auto& actor : LevelActors)
	{
		if (!actor->IsA<AGeometryCollectionActor>())
		{

			auto Prims = GetPrimitives(actor);
			for (auto& Prim : Prims)
			{
				if (!Mobilities.Contains(Prim))
				{
					Mobilities.Add(Prim, Prim->Mobility.GetValue());
				}

				if (!Physics.Contains(Prim))
				{
					Physics.Add(Prim, Prim->IsSimulatingPhysics());
				}

				if (!Gravities.Contains(Prim))
				{
					Gravities.Add(Prim, Prim->IsGravityEnabled());
				}

				if (!Positions.Contains(Prim))
				{
					Positions.Add(Prim, Prim->GetComponentLocation());
				}

				if (!Rotations.Contains(Prim))
				{
					Rotations.Add(Prim, Prim->GetComponentRotation());
				}

				Prim->SetMobility(EComponentMobility::Static);
			}
		}
	}
}
void FMeshSpawnMode::UpdateSelectionPhysics()
{
	if (!bSimulatePhysics)
	{
		return;
	}
	CachePhysics();

	auto Prims = GetSelectedPrimitives();
	for (auto& Prim : Prims)
	{
		if (!Mobilities.Contains(Prim))
		{
			Mobilities.Add(Prim, Prim->Mobility.GetValue());
		}

		if (!Physics.Contains(Prim))
		{
			Physics.Add(Prim, Prim->IsSimulatingPhysics());
		}

		if (!Gravities.Contains(Prim))
		{
			Gravities.Add(Prim, Prim->IsGravityEnabled());
		}

		Prim->SetSimulatePhysics(bSimulatePhysics);
		Prim->SetEnableGravity(false);
		Prim->SetMobility(EComponentMobility::Movable);
	}
}

void FMeshSpawnMode::ResetTransform()
{
	USelection* SelectedActors = GEditor->GetSelectedActors();

	auto Prims = GetSelectedPrimitives();
	for (auto& Prim : Prims)
	{
		ResetPrimitivePhysics(Prim, true);
	}

	SelectedActors->DeselectAll();
}

TArray<UPrimitiveComponent*> FMeshSpawnMode::GetSpawnedComponents()
{ 
	TArray<UPrimitiveComponent*> Components;
	for (auto& Actor : SpawnedActors)
	{
		Components.Append(GetPrimitives(Actor));
	}
	return Components;
}

void FMeshSpawnMode::ResetPhysics()
{
	TArray<UPrimitiveComponent*> Prims;
	Mobilities.GetKeys(Prims);

	for (auto& Prim : Prims)
	{
		ResetPrimitivePhysics(Prim, false);
	}
}

void FMeshSpawnMode::ResetPrimitivePhysics(UPrimitiveComponent* InPrim, bool bResetTransform, bool bForceStatic)
{
	if (InPrim && IsValid(InPrim) && !InPrim->IsBeingDestroyed()
		&& !InPrim->HasAnyFlags(RF_BeginDestroyed)
		&& InPrim->GetFName().IsValid())
	{
		if (Physics.Contains(InPrim))
		{
			InPrim->SetSimulatePhysics(bForceStatic ? false : Physics[InPrim]);
		}

		if (Gravities.Contains(InPrim))
		{
			InPrim->SetEnableGravity(bForceStatic ? false : Gravities[InPrim]);
		}

		if (Mobilities.Contains(InPrim))
		{
			InPrim->SetMobility(bForceStatic ? EComponentMobility::Static : Mobilities[InPrim]);
		}

		if (bResetTransform)
		{
			if (Positions.Contains(InPrim))
			{
				InPrim->SetWorldLocation(Positions[InPrim]);
			}

			if (Rotations.Contains(InPrim))
			{
				InPrim->SetWorldRotation(Rotations[InPrim]);
			}
		}
	}

}

#undef LOCTEXT_NAMESPACE
