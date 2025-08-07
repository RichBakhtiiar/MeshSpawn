// Copyright Bakhtiiar Akhmednov. All Rights Reserved. 2025

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateStyle.h"

/**  */
class FMeshSpawnStyle
{
public:
	/** Initialize toolkit style */
	static void Initialize();

	/** Destroys toolkit style */
	static void Shutdown();

	/** Reloads textures used by slate renderer */
	static void ReloadTextures();

	/** Returns The Slate style set */
	static const ISlateStyle& Get();

	/** Returns The Slate style set name */
	static FName GetStyleSetName();

private:
	/** Creates all styles settings */
	static TSharedRef< class FSlateStyleSet > Create();

private:
	/** Slate Style pointer */
	static TSharedPtr< class FSlateStyleSet > StyleInstance;
};