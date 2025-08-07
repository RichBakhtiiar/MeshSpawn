// Copyright Bakhtiiar Akhmednov. All Rights Reserved. 2025

#include "MeshSpawnToolkit.h"
#include "Engine/Selection.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Text/STextBlock.h"
#include "EditorModeManager.h"
#include "FileHelpers.h"
#include "ISourceControlModule.h"
#include "ISourceControlProvider.h"
#include "PropertyCustomizationHelpers.h"
#include "Modules/ModuleManager.h"
#include "Framework/MultiBox/MultiBox.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Misc/Attribute.h"
#include "Textures/SlateIcon.h"
#include "MeshSpawnCommands.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SVectorInputBox.h"
#include "Widgets/Input/SRotatorInputBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "SlateUtil.h"
#include "MeshSpawnMode.h"
#include "Components/StaticMeshComponent.h"
#include "MeshMergeModule.h"
#include "MeshSpawnPreset.h"
#include "SourceControlOperations.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/MeshMerging.h"
#include "UObject/SavePackage.h"
#include "Widgets/Layout/SUniformGridPanel.h"

#define LOCTEXT_NAMESPACE "FMeshSpawnToolkit"

FMeshSpawnToolkit::FMeshSpawnToolkit()
{
	ThumbnailPool = MakeShareable(new FAssetThumbnailPool(24));
	bIsPercentRelative = false;
	bUseSelected = false;
	bEnableGravity = true;
	bEnableOffsets = true;
	CurrentSpawnMode = ESpawnMode::Select;
}


void FMeshSpawnToolkit::Init(const TSharedPtr<IToolkitHost>& InitToolkitHost)
{
	BindCommands();

	TSharedPtr<FReferenceMesh> mesh = MakeShareable(new FReferenceMesh());
	mesh->Chance = 100;
	ReferenceMeshes.Add(mesh);

	SpawnModes.Add(MakeShared<FString>(ESpawnMode::Paint));
	CurrentSpawnMode = ESpawnMode::Transform;

	const FMeshSpawnCommands& Commands = FMeshSpawnCommands::Get();
	TSharedPtr<FUICommandList> CommandList = GetToolkitCommands();

	FToolBarBuilder ToolbarBuilder(CommandList, FMultiBoxCustomization::None);
	ToolbarBuilder.SetStyle(&FAppStyle::Get(), "ToolBar");
	ToolbarBuilder.AddToolBarButton(Commands.PaintCommand);
	
	SAssignNew(ToolkitWidget, SBorder)
		.HAlign(HAlign_Fill)
		.Padding(15)
		[
			SNew(SVerticalBox)

				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(4, 0, 4, 5)
				[
					ToolbarBuilder.MakeWidget()
				]
				
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(2.0f)
				[
					SNew(SVerticalBox)

						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(FMargin(0, 6, 0, 2))
						[
							SNew(SHorizontalBox)

								// Left line separator
								+ SHorizontalBox::Slot()
								.FillWidth(0.1f)
								.VAlign(VAlign_Center)
								[
									SNew(SBox)
										.HeightOverride(1.0f)
										[
											SNew(SImage)
												.ColorAndOpacity(FLinearColor::White)
												.Image(FAppStyle::Get().GetBrush("WhiteBrush"))
										]
								]

							// Text between separation lines
							+ SHorizontalBox::Slot()
								.AutoWidth()
								.Padding(FMargin(8, 0))
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
										.Text(FText::FromString("Brush Options"))
										.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
								]

								// Right line separator
								+ SHorizontalBox::Slot()
								.FillWidth(1.0f)
								.VAlign(VAlign_Center)
								[
									SNew(SBox)
										.HeightOverride(1.0f)
										[
											SNew(SImage)
												.ColorAndOpacity(FLinearColor::White)
												.Image(FAppStyle::Get().GetBrush("WhiteBrush"))
										]
								]
						]

					// Add spacing between "Brush Options" and first settings
					+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0, 6, 0, 0)
						[
							SNew(SBox)
								.HeightOverride(4.0f)
						]

					// Brush size
					+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								.FillWidth(0.5f)
								[
									SNew(STextBlock)
										.Text(FText::FromString("Brush Size"))
										.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
								]
								+ SHorizontalBox::Slot()
								.FillWidth(0.5f)
								[
									SNew(SNumericEntryBox<float>)
										.Value(this, &FMeshSpawnToolkit::GetBrushSize)
										.OnValueChanged(this, &FMeshSpawnToolkit::OnBrushSizeChanged)
										.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
								]
						
						]

					+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0, 5)
						[
							SNew(SHorizontalBox)

							// Place with Gravity checkbox
							+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(SCheckBox)
										.IsChecked(IsEnableGravity())
										.OnCheckStateChanged_Lambda([this](ECheckBoxState InCheck)
											{
												bEnableGravity = InCheck == ECheckBoxState::Checked;
											})
										.Content()
										[
											SNew(STextBlock)
												.Text(LOCTEXT("EnableGravity", "Place With Gravity"))
												.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
										]
								
								]


							// Enable Offsets checkbox
							+SHorizontalBox::Slot()
								.Padding(10, 0, 0, 0)
								[
									SNew(SCheckBox)
										.IsChecked(true)
										.OnCheckStateChanged_Lambda([this](ECheckBoxState InCheck)
											{
												bEnableOffsets = InCheck == ECheckBoxState::Checked ? true : false;
											})
										.Content()
										[
											SNew(STextBlock)
												.Text(LOCTEXT("EnableOffsets", "Enable Offsets"))
												.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
										]
								]
						]
					
		
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(4)
				[
					CreatePaintModeWidget().ToSharedRef()
				]
		]];

	FModeToolkit::Init(InitToolkitHost);

	OnPaintClicked();
}

void FMeshSpawnToolkit::ModeChanged(TSharedPtr<FString> Item, ESelectInfo::Type SelectInfo)
{
	if (Item)
	{
		CurrentSpawnMode = *Item.Get();
	}
}

TArray<TSharedPtr<FReferenceMesh>> FMeshSpawnToolkit::GetReferenceMeshes() const
{
	return ReferenceMeshes;
}

TSharedRef<ITableRow> FMeshSpawnToolkit::GetReferenceMeshWidget(TSharedPtr<FReferenceMesh> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	TSharedPtr<SSlider> Slider;
	TSharedPtr<STableRow<TSharedPtr<FReferenceMesh>>> Row;

	SAssignNew(Row, STableRow<TSharedPtr<FReferenceMesh>>, OwnerTable)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.FillWidth(1)
		[
			// Reference mesh widget
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(FMargin(4.0f, 2.0f))
			.AutoHeight()
			[
				SNew(SObjectPropertyEntryBox)
				.ObjectPath_Lambda([this, InItem]() -> FString
				{
					return InItem.Get()->StaticMesh->GetPathName();
				})
				.OnObjectChanged_Lambda([this, InItem](const FAssetData& InAsset)
					{
						if (InAsset.IsValid())
						{
							InItem.Get()->StaticMesh = Cast<UStaticMesh>(InAsset.GetAsset());
							SetRandomMesh();
						}
						else
						{
							InItem.Get()->StaticMesh = nullptr;
							PickedMesh = nullptr;
						}

						SetRandomMesh();

						if (EdMode)
						{
							EdMode->RegisterBrush();
						}
					})
				.AllowedClass(UStaticMesh::StaticClass())
				.DisplayThumbnail(true)
				.DisplayUseSelected(false)
				.DisplayBrowse(true)
				.DisplayUseSelected(true)
				.ThumbnailPool(ThumbnailPool)
			]
		]
	];
	return Row.ToSharedRef();
}


TSharedPtr<SWidget> FMeshSpawnToolkit::CreatePaintModeWidget()
{
	int RowHeight = 25;
	float LeftWidth = 0.5f;
	float RightWidth = 0.5f;
	auto VerticalScrollbar = SNew(SScrollBar)
		.Orientation(Orient_Vertical)
		.Thickness(FVector2D(14.0f, 14.0f));
	return SNew(SBox)
	.Visibility_Lambda([this]() { return GetCurrentSpawnMode() == ESpawnMode::Paint ? EVisibility::Visible : EVisibility::Collapsed; })
	.WidthOverride(300)
	[
		SNew(SVerticalBox)

		// Min Position
		+ SVerticalBox::Slot()
		.Padding(0, 2, 0, 0)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.FillWidth(LeftWidth)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("MinPositionOffsetText", "Min Position Offset"))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(RightWidth)
			[
				SNew(SBox)
					.IsEnabled_Lambda([this]() { return bEnableOffsets; }) // Blocks editing
					[
						SNew(SVectorInputBox)
						.bColorAxisLabels(true)
						.AllowSpin(true)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
						.X_Lambda([this]()->TOptional<float> { return MinPositionOffset.X; })
						.Y_Lambda([this]()->TOptional<float> { return MinPositionOffset.Y; })
						.Z_Lambda([this]()->TOptional<float> { return MinPositionOffset.Z; })
						.OnXChanged_Lambda([this](float InValue) { MinPositionOffset.X = InValue; SetRandomMesh(); })
						.OnYChanged_Lambda([this](float InValue) { MinPositionOffset.Y = InValue; SetRandomMesh(); })
						.OnZChanged_Lambda([this](float InValue) { MinPositionOffset.Z = InValue; SetRandomMesh(); })
					]
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SButton)
				.OnClicked_Lambda([this]() { MinPositionOffset = FVector::ZeroVector; return FReply::Handled();})
				.Visibility_Lambda([this]() { return MinPositionOffset.IsZero() ? EVisibility::Hidden : EVisibility::Visible; })
				.ButtonStyle( FAppStyle::Get(), "NoBorder" )
				.Content()
				[
					SNew(SImage)
					.Image( FAppStyle::GetBrush("PropertyWindow.DiffersFromDefault") )
				]
			]
		]

		// Max Position
		+ SVerticalBox::Slot()
		.Padding(0, 0, 0, 2)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.FillWidth(LeftWidth)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("MaxPositionOffsetText", "Max Position Offset"))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(RightWidth)
			[
				SNew(SBox)
					.IsEnabled_Lambda([this]() { return bEnableOffsets; }) // Blocks editing
					[	
						SNew(SVectorInputBox)
						.bColorAxisLabels(true)
						.AllowSpin(true)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
						.X_Lambda([this]()->TOptional<float> { return MaxPositionOffset.X; })
						.Y_Lambda([this]()->TOptional<float> { return MaxPositionOffset.Y; })
						.Z_Lambda([this]()->TOptional<float> { return MaxPositionOffset.Z; })
						.OnXChanged_Lambda([this](float InValue) { MaxPositionOffset.X = InValue; SetRandomMesh(); })
						.OnYChanged_Lambda([this](float InValue) { MaxPositionOffset.Y = InValue; SetRandomMesh(); })
						.OnZChanged_Lambda([this](float InValue) { MaxPositionOffset.Z = InValue; SetRandomMesh(); })
					]
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SButton)
				.OnClicked_Lambda([this]() { MaxPositionOffset = FVector::ZeroVector; return FReply::Handled(); })
				.Visibility_Lambda([this]() { return MaxPositionOffset.IsZero() ? EVisibility::Hidden : EVisibility::Visible; })
				.ButtonStyle( FAppStyle::Get(), "NoBorder" )
				.Content()
				[
					SNew(SImage)
					.Image( FAppStyle::GetBrush("PropertyWindow.DiffersFromDefault") )
				]
			]
		]

		// Min rotation
		+ SVerticalBox::Slot()
		.Padding(0, 2, 0, 0)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.FillWidth(LeftWidth)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("MinRotateOffsetText", "Min Rotation Offset"))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(RightWidth)
			[	
				SNew(SBox)
					.IsEnabled_Lambda([this]() { return bEnableOffsets; })
					[
						SNew(SRotatorInputBox)
						.bColorAxisLabels(true)
						.AllowSpin(true)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
						.Yaw_Lambda([this]()->TOptional<float> { return	MinRotateOffset.Yaw; })
						.Pitch_Lambda([this]()->TOptional<float> { return MinRotateOffset.Pitch; })
						.Roll_Lambda([this]()->TOptional<float> { return MinRotateOffset.Roll; })
						.OnYawChanged_Lambda([this](float InValue) { MinRotateOffset.Yaw = InValue;	 SetRandomMesh(); })
						.OnPitchChanged_Lambda([this](float InValue) { MinRotateOffset.Pitch = InValue; SetRandomMesh(); })
						.OnRollChanged_Lambda([this](float InValue) { MinRotateOffset.Roll = InValue;	 SetRandomMesh(); })
					]
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SButton)
				.OnClicked_Lambda([this]() { MinRotateOffset = FRotator::ZeroRotator; return FReply::Handled();})
				.Visibility_Lambda([this]() { return MinRotateOffset.IsZero() ? EVisibility::Hidden : EVisibility::Visible; })
				.ButtonStyle( FAppStyle::Get(), "NoBorder" )
				.Content()
				[
					SNew(SImage)
					.Image( FAppStyle::GetBrush("PropertyWindow.DiffersFromDefault") )
				]
			]
		]

		// Max rotation
		+ SVerticalBox::Slot()
		.Padding(0, 0, 0, 2)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.FillWidth(LeftWidth)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("MaxRotateOffsetText", "Max Rotation Offset"))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
			]
			+ SHorizontalBox::Slot()
			.FillWidth(RightWidth)
			[
				SNew(SBox)
					.IsEnabled_Lambda([this]() { return bEnableOffsets; })
					[	SNew(SRotatorInputBox)
						.bColorAxisLabels(true)
						.AllowSpin(true)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
						.Yaw_Lambda([this]()->TOptional<float> { return	MaxRotateOffset.Yaw; })
						.Pitch_Lambda([this]()->TOptional<float> { return MaxRotateOffset.Pitch; })
						.Roll_Lambda([this]()->TOptional<float> { return MaxRotateOffset.Roll; })
						.OnYawChanged_Lambda([this](float InValue)		{ MaxRotateOffset.Yaw = InValue;	SetRandomMesh();})
						.OnPitchChanged_Lambda([this](float InValue)	{ MaxRotateOffset.Pitch = InValue;	SetRandomMesh();})
						.OnRollChanged_Lambda([this](float InValue)	{ MaxRotateOffset.Roll = InValue;	SetRandomMesh();})
					]
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SButton)
				.OnClicked_Lambda([this]() { MaxRotateOffset = FRotator::ZeroRotator; return FReply::Handled();})
				.Visibility_Lambda([this]() { return MaxRotateOffset.IsZero() ? EVisibility::Hidden : EVisibility::Visible; })
				.ButtonStyle( FAppStyle::Get(), "NoBorder" )
				.Content()
				[
					SNew(SImage)
					.Image( FAppStyle::GetBrush("PropertyWindow.DiffersFromDefault") )
				]
			]
		]

		// Min scale
		+ SVerticalBox::Slot()
		.Padding(0, 2, 0, 0)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.FillWidth(LeftWidth)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("MinScaleOffsetText", "Min Scale Offset"))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.MaxWidth( 18.0f )
			[
				SNew( SCheckBox )
				.IsChecked_Lambda([this]()
					{
						return IsMinScaleLock()?ECheckBoxState::Checked:ECheckBoxState::Unchecked;
					})
				.OnCheckStateChanged_Lambda([this](ECheckBoxState InState)
					{
						bMinScaleLock = InState == ECheckBoxState::Checked ? true : false;
					})
				.Style( FAppStyle::Get(), "TransparentCheckBox" )
				[
					SNew( SImage )
					.Image_Lambda([this]()
					{
						return IsMinScaleLock() ? FAppStyle::GetBrush( TEXT("Icons.Lock") ) : FAppStyle::GetBrush( TEXT("Icons.Unlock") ) ;
					})
					.ColorAndOpacity( FSlateColor::UseForeground() )
				]
			]
			+ SHorizontalBox::Slot()
			.FillWidth(RightWidth)
			[
				SNew(SBox)
					.IsEnabled_Lambda([this]() { return bEnableOffsets; })
					[	SNew(SVectorInputBox)
						.bColorAxisLabels(true)
						.AllowSpin(true)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
						.X_Lambda([this]()->TOptional<float> { return MinScaleOffset.X; })
						.Y_Lambda([this]()->TOptional<float> { return MinScaleOffset.Y; })
						.Z_Lambda([this]()->TOptional<float> { return MinScaleOffset.Z; })
						.OnXChanged_Lambda([this](float InValue)
						{MinScaleOffset.X = InValue; if (IsMinScaleLock()) { MinScaleOffset.Y = InValue; MinScaleOffset.Z = InValue; SetRandomMesh(); }})
						.OnYChanged_Lambda([this](float InValue)
						{MinScaleOffset.Y = InValue; if (IsMinScaleLock()) { MinScaleOffset.X = InValue; MinScaleOffset.Z = InValue; SetRandomMesh(); }})
						.OnZChanged_Lambda([this](float InValue)
						{MinScaleOffset.Z = InValue; if (IsMinScaleLock()) { MinScaleOffset.X = InValue; MinScaleOffset.Y = InValue; SetRandomMesh(); }})
					]
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SButton)
				.OnClicked_Lambda([this]() { MinScaleOffset = FVector::OneVector; return FReply::Handled();})
				.Visibility_Lambda([this]() { return MinScaleOffset.X==1 && MinScaleOffset.Y==1 && MinScaleOffset.Z==1 ? EVisibility::Hidden : EVisibility::Visible; })
				.ButtonStyle( FAppStyle::Get(), "NoBorder" )
				.Content()
				[
					SNew(SImage)
					.Image( FAppStyle::GetBrush("PropertyWindow.DiffersFromDefault") )
				]
			]
		]

		// Max rotation
		+ SVerticalBox::Slot()
		.Padding(0, 0, 0, 2)
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.VAlign(EVerticalAlignment::VAlign_Center)
			.FillWidth(LeftWidth)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("MaxScaleOffsetText", "Max Scale Offset"))
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			.MaxWidth( 18.0f )
			[
				SNew( SCheckBox )
				.IsChecked_Lambda([this]()
					{
						return IsMaxScaleLock() ? ECheckBoxState::Checked:ECheckBoxState::Unchecked;
					})
				.OnCheckStateChanged_Lambda([this](ECheckBoxState InState)
					{
						bMaxScaleLock = InState == ECheckBoxState::Checked ? true : false;
					})
				.Style( FAppStyle::Get(), "TransparentCheckBox" )
				[
					SNew( SImage )
					.Image_Lambda([this]()
					{
						return IsMaxScaleLock() ? FAppStyle::GetBrush( TEXT("Icons.Lock") ) : FAppStyle::GetBrush( TEXT("Icons.Unlock") ) ;
					})
					.ColorAndOpacity( FSlateColor::UseForeground() )
				]
			]
			+ SHorizontalBox::Slot()
			.FillWidth(RightWidth)
			[
				SNew(SBox)
					.IsEnabled_Lambda([this]() { return bEnableOffsets; })
					[	SNew(SVectorInputBox)
						.bColorAxisLabels(true)
						.AllowSpin(true)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
						.X_Lambda([this]()->TOptional<float> { return MaxScaleOffset.X; })
						.Y_Lambda([this]()->TOptional<float> { return MaxScaleOffset.Y; })
						.Z_Lambda([this]()->TOptional<float> { return MaxScaleOffset.Z; })
						.OnXChanged_Lambda([this](float InValue)
						{MaxScaleOffset.X = InValue; if (IsMaxScaleLock()) { MaxScaleOffset.Y = InValue; MaxScaleOffset.Z = InValue; SetRandomMesh(); }})
						.OnYChanged_Lambda([this](float InValue)
						{MaxScaleOffset.Y = InValue; if (IsMaxScaleLock()) { MaxScaleOffset.X = InValue; MaxScaleOffset.Z = InValue; SetRandomMesh(); }})
						.OnZChanged_Lambda([this](float InValue)
						{MaxScaleOffset.Z = InValue; if (IsMaxScaleLock()) { MaxScaleOffset.X = InValue; MaxScaleOffset.Y = InValue; SetRandomMesh(); }})
					]
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			.AutoWidth()
			[
				SNew(SButton)
				.OnClicked_Lambda([this]() { MaxScaleOffset = FVector::OneVector;return FReply::Handled(); })
				.Visibility_Lambda([this]() { return MaxScaleOffset.X==1 && MaxScaleOffset.Y==1 && MaxScaleOffset.Z==1 ? EVisibility::Hidden : EVisibility::Visible; })
				.ButtonStyle( FAppStyle::Get(), "NoBorder" )
				.Content()
				[
					SNew(SImage)
					.Image(FAppStyle::GetBrush("PropertyWindow.DiffersFromDefault"))
				]
			]
		]

		+ SVerticalBox::Slot()
		.Padding(0, 5)
		.AutoHeight()
		[
			CreateSharedWidget().ToSharedRef()
		]

		// Reference mesh widgets
		+SVerticalBox::Slot()
		[
			SNew(SScrollBox)
				+ SScrollBox::Slot()
				[
					SAssignNew(ReferenceMeshesListView, SListView<TSharedPtr<FReferenceMesh>>)
						.ItemHeight(300)
						.ListItemsSource(&ReferenceMeshes)
						.OnGenerateRow(this, &FMeshSpawnToolkit::GetReferenceMeshWidget)
						.OnSelectionChanged_Lambda([this](TSharedPtr<FReferenceMesh> InMesh, ESelectInfo::Type SelectionType)
							{
								SelectedMeshIndex = ReferenceMeshes.IndexOfByKey(InMesh);
								SetRandomMesh();
							}
						)
						.ForceVolatile(true)
						.ExternalScrollbar(VerticalScrollbar)
				]
		]

		+ SVerticalBox::Slot()
		.Padding(0, 5)
		.AutoHeight()
		[
			CreateSharedWidget().ToSharedRef()
		]

		// Bake all placed meshes into instance mesh
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SButton)
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.Content()
				[
					SNew(STextBlock)
						.Text(LOCTEXT("MeshSpawnModeBakeAllMesh", "Bake All Spawned Meshes To InstanceMesh"))
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 9))
				]
				.OnClicked_Lambda([this]()
					{
						BakeToInstanceMesh(false);
						return FReply::Handled();
					}
				)
		]
	];
}

TSharedPtr<SWidget> FMeshSpawnToolkit::CreateSharedWidget()
{
	return SNew(SVerticalBox);
}

void FMeshSpawnToolkit::BindCommands()
{
	const TSharedRef<FUICommandList>& CommandList = GetToolkitCommands();
	const FMeshSpawnCommands& Commands = FMeshSpawnCommands::Get();

	// Paint command action
	CommandList->MapAction(
		Commands.PaintCommand, 
		FExecuteAction::CreateLambda([this]()
		{
			ChangeMode(ESpawnMode::Paint);
			SpawnModeIndex = 3;
			if (EdMode)
			{
				EdMode->RegisterBrush();
			}
		}),
		FCanExecuteAction(),
		FIsActionChecked::CreateLambda([this]()
		{
			return GetCurrentSpawnMode() == ESpawnMode::Paint;
		})
	);
}

float FMeshSpawnToolkit::GetMinDistance()
{
	if (GetRandomMesh())
	{
		return fMinDistance * ScaleRandom.Size() * GetRandomMesh()->GetBounds().GetSphere().W;
	}
	return fMinDistance * ScaleRandom.Size();
}

void FMeshSpawnToolkit::SetRandomMesh()
{
	if (IsUseSelected())
	{
		if (SelectedMeshIndex >= 0)
		{
			PickedMesh = ReferenceMeshes[SelectedMeshIndex].Get()->StaticMesh;
		}
	}
	else
	{
		auto Meshes = ReferenceMeshes;
		int Rand = FMath::FRandRange(0.0f, 100.0f);

		auto PickedMeshes = Meshes.FilterByPredicate([Rand](const TSharedPtr<FReferenceMesh>& InMesh)
			{
				return (InMesh.Get()->StaticMesh && InMesh.Get()->Chance >= Rand);
			}
		);
		if (PickedMeshes.Num() == 0)
		{
			return;
		}
		PickedMesh = PickedMeshes[FMath::RandRange(0, PickedMeshes.Num() - 1)].Get()->StaticMesh;
	}

	PositionRandom = FMath::RandPointInBox(FBox(MinPositionOffset, MaxPositionOffset));
	FVector RandRot = FMath::RandPointInBox(FBox(MinRotateOffset.Euler(), MaxRotateOffset.Euler()));
	RotateRandom = FRotator(RandRot.X, RandRot.Y, RandRot.Z);
	
	if (IsMinScaleLock() && IsMaxScaleLock())
	{
		ScaleRandom = FVector::OneVector * FMath::RandRange(MinScaleOffset.X, MaxScaleOffset.X);
	}
	else
	{
		ScaleRandom = FMath::RandPointInBox(FBox(MinScaleOffset, MaxScaleOffset));
	}
}

UStaticMesh* FMeshSpawnToolkit::GetRandomMesh()
{
	{
		if (ReferenceMeshes.Num() == 0)
		{
			UE_LOG(LogTemp, Error, TEXT("[MeshSpawnToolkit] ReferenceMeshes is empty."));
			PickedMesh = nullptr;
			return nullptr;
		}

		int32 Index = FMath::RandRange(0, ReferenceMeshes.Num() - 1);

		if (!ReferenceMeshes.IsValidIndex(Index))
		{
			UE_LOG(LogTemp, Error, TEXT("[MeshSpawnToolkit] Invalid index %d for ReferenceMeshes.Num() = %d"), Index, ReferenceMeshes.Num());
			PickedMesh = nullptr;
			return nullptr;
		}

		if (!ReferenceMeshes[Index].IsValid())
		{
			UE_LOG(LogTemp, Error, TEXT("[MeshSpawnToolkit] ReferenceMesh[%d] is not valid."), Index);
			PickedMesh = nullptr;
			return nullptr;
		}

		PickedMesh = ReferenceMeshes[Index]->StaticMesh;
		return PickedMesh;
	}
}

void FMeshSpawnToolkit::ChangeMode(FString InSpawnMode)
{
	LastSpawnMode = GetCurrentSpawnMode();
	CurrentSpawnMode = InSpawnMode;
	
	if (EdMode)
	{
		EdMode->OnSpawnModeChange(InSpawnMode);
	}
}

void FMeshSpawnToolkit::ChangeMode(int InDirection)
{
	SpawnModeIndex += InDirection;
	SpawnModeIndex = (SpawnModes.Num() + (SpawnModeIndex % SpawnModes.Num())) % SpawnModes.Num() ;
	
	if (SpawnModeIndex >= 0 && SpawnModeIndex < SpawnModes.Num())
	{
		LastSpawnMode = GetCurrentSpawnMode();
		ChangeMode(*SpawnModes[SpawnModeIndex].Get());
	}
}

void FMeshSpawnToolkit::BakeToInstanceMesh(bool BakeSelected)
{
	if (EdMode)
	{
		TArray<UPrimitiveComponent*> SpawnedComponents;
		if (BakeSelected)
		{
			SpawnedComponents = EdMode->GetSelectedPrimitives();
		}
		else
		{
			SpawnedComponents = EdMode->GetSpawnedComponents();
		}
		
		if (SpawnedComponents.Num() > 0)
		{
			GEditor->BeginTransaction(LOCTEXT("MeshSpawnMode_Bake", "Bake to InstaceMesh"));
			FMeshInstancingSettings Settings;
			const IMeshMergeUtilities& MeshUtilities = FModuleManager::Get().LoadModuleChecked<IMeshMergeModule>("MeshMergeUtilities").GetUtilities();
			MeshUtilities.MergeComponentsToInstances(SpawnedComponents, EdMode->GetWorld(), EdMode->GetWorld()->GetCurrentLevel(), Settings);
			GEditor->EndTransaction();
		}
		
		EdMode->DestroyActors(BakeSelected);
	}
}

TSharedRef<SWidget> FMeshSpawnToolkit::GenerateWidget(TSharedPtr<FString> InItem)
{
	FText ItemAsText = FText::FromString(*InItem.Get());
	
	return
		SNew(SBox)
		.WidthOverride(300)
		[
			SNew(STextBlock)
			.Text(ItemAsText)
		.ToolTipText(ItemAsText)
		];
}
FName FMeshSpawnToolkit::GetToolkitFName() const
{
	return FName("MeshSpawnEdMode");
}

FText FMeshSpawnToolkit::GetBaseToolkitName() const
{
	return NSLOCTEXT("MeshSpawnToolkit", "DisplayName", "MeshSpawnEdMode Tool");
}

class FEdMode* FMeshSpawnToolkit::GetEditorMode() const
{
	return GLevelEditorModeTools().GetActiveMode(FMeshSpawnMode::EM_SpawnModeId);
}

FReply FMeshSpawnToolkit::OnPaintClicked()
{
	CurrentSpawnMode = ESpawnMode::Paint;
	return FReply::Handled();
}

TOptional<float> FMeshSpawnToolkit::GetBrushSize() const
{
	return BrushSize;
}

void FMeshSpawnToolkit::OnBrushSizeChanged(float NewValue)
{
	BrushSize = NewValue;
}

void FMeshSpawnToolkit::UnbindAll()
{
	if (ReferenceMeshes.Num() == 0)
	{
		return;
	}

	for (auto& Item : ReferenceMeshes)
	{
		if (Item.IsValid())
		{
			Item->StaticMesh = nullptr;
		}
	}
}

#undef LOCTEXT_NAMESPACE
