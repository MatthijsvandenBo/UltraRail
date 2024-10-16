// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Styling/SlateStyle.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCustomAsset, All, Log);

struct FCustomPinFactory;

class FCustomAssetsEditorModule : public IModuleInterface
{
private:
	TSharedPtr<FSlateStyleSet> StyleSet = nullptr;
	TSharedPtr<FCustomPinFactory> PinFactory = nullptr;
	
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
