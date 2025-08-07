// Copyright Bakhtiiar Akhmednov. All Rights Reserved. 2025

#pragma once

#include "CoreMinimal.h"
#include "Framework/MultiBox/MultiBox.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Misc/Attribute.h"
#include "Textures/SlateIcon.h"
#include "MeshSpawnCommands.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SVectorInputBox.h"
#include "Widgets/Input/SRotatorInputBox.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Text/STextBlock.h"
/**
 * The Slate Utility functions, which will grow during the project
 */
class MESHSPAWN_API SlateUtil
{
public:

	/** Creates sized text block in a SBox  */
	static TSharedPtr<SWidget> SSizedTextBlock(FText InText, float InWidth, float InHeight)
	{
		return SNew(SBox)
			.HeightOverride(InHeight)
			.VAlign(EVerticalAlignment::VAlign_Center)
			[
				SNew(STextBlock)
				.Text(InText)
			];
			
	}
};
