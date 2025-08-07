// Copyright Bakhtiiar Akhmednov. All Rights Reserved. 2025

#include "MeshSpawnStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Framework/Application/SlateApplication.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"

TSharedPtr< FSlateStyleSet > FMeshSpawnStyle::StyleInstance = NULL;

void FMeshSpawnStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FMeshSpawnStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FMeshSpawnStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("MeshSpawnStyle"));
	return StyleSetName;
}

#define PLUGIN_IMAGE_BRUSH( Path, ... ) FSlateImageBrush( IPluginManager::Get().FindPlugin("MeshSpawn")->GetBaseDir()  + "/" + Path + ".png", __VA_ARGS__ )
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( Style->RootToContentDir( RelativePath, TEXT(".png") ), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".ttf") ), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( Style->RootToContentDir( RelativePath, TEXT(".otf") ), __VA_ARGS__ )

const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);
const FVector2D Icon40x40(40.0f, 40.0f);

TSharedRef< FSlateStyleSet > FMeshSpawnStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("MeshSpawnStyle"));

	Style->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	Style->Set("MeshSpawn.PaintCommand", new PLUGIN_IMAGE_BRUSH(TEXT("/Resources/paint"), Icon20x20));
	
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("MeshSpawn")->GetBaseDir() / TEXT("Resources"));
	Style->Set("ClassIcon.MeshSpawnPreset",  new PLUGIN_IMAGE_BRUSH(TEXT("/Resources/paint"), Icon40x40));
	Style->Set("ClassThumbnail.MeshSpawnPreset",  new PLUGIN_IMAGE_BRUSH(TEXT("/Resources/paint"), Icon40x40));
	Style->Set("ClassIcon.MeshSpawnMode", new PLUGIN_IMAGE_BRUSH(TEXT("/Resources/paint_02"), Icon16x16));
	Style->Set("ClassThumbnail.MeshSpawnMode", new PLUGIN_IMAGE_BRUSH(TEXT("/Resources/paint_02"), Icon40x40));
	
	return Style;
}

void FMeshSpawnStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FMeshSpawnStyle::Get()
{
	return *StyleInstance;
}

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT
