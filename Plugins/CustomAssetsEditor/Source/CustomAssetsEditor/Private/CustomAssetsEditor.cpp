// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomAssetsEditor.h"
#include "BiomeAsset/Actions/BiomeAssetAction.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "BiomeAsset/Constants.h"

#define LOCTEXT_NAMESPACE "FCustomAssetsEditorModule"

void FCustomAssetsEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& AssetToolsModule = IAssetTools::Get();
	const EAssetTypeCategories::Type CustomAssetType = AssetToolsModule.RegisterAdvancedAssetCategory(
		Constants::CategoryKey,
		FText::FromString("Custom Assets"));

	const TSharedPtr<FBiomeAssetAction> CustomAssetAction = MakeShareable(new FBiomeAssetAction(CustomAssetType));
	AssetToolsModule.RegisterAssetTypeActions(CustomAssetAction.ToSharedRef());
}

void FCustomAssetsEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomAssetsEditorModule, CustomAssetsEditor)