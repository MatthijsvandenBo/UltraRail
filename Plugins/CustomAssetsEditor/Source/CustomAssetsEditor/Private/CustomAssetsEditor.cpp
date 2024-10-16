// Copyright Epic Games, Inc. All Rights Reserved.

#include "CustomAssetsEditor.h"
#include "BiomeAsset/Actions/BiomeAssetAction.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "BiomeAsset/Constants.h"
#include "Styling/SlateStyleRegistry.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FCustomAssetsEditorModule"

DEFINE_LOG_CATEGORY(LogCustomAsset)

void FCustomAssetsEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	IAssetTools& AssetToolsModule = IAssetTools::Get();
	const EAssetTypeCategories::Type CustomAssetType = AssetToolsModule.RegisterAdvancedAssetCategory(
		Constants::CategoryKey,
		FText::FromString("Custom Assets"));

	const TSharedPtr<FBiomeAssetAction> CustomAssetAction = MakeShareable(new FBiomeAssetAction(CustomAssetType));
	AssetToolsModule.RegisterAssetTypeActions(CustomAssetAction.ToSharedRef());
	
	const TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("CustomAssetsEditor");
	const FString ContentDir = Plugin->GetContentDir();

	StyleSet = MakeShareable(new FSlateStyleSet(Constants::StyleSetName));
	StyleSet->SetContentRoot(ContentDir);

	FSlateImageBrush* ThumbnailBrush =
		new FSlateImageBrush(
			StyleSet->RootToContentDir(
				TEXT("BiomeAsset/Thumbnail"),
				TEXT(".png")
			),
			FVector2D(128.f, 128.f)
		);

	FSlateImageBrush* IconBrush =
		new FSlateImageBrush(
			StyleSet->RootToContentDir(
				TEXT("BiomeAsset/Icon"),
				TEXT(".png")
			),
			FVector2D(128.f, 128.f)
		);

	StyleSet->Set(TEXT("ClassThumbnail.BiomeAsset"), ThumbnailBrush);
	StyleSet->Set(TEXT("ClassIcon.BiomeAsset"), IconBrush);
	
	// Only when we really need a style-set
	// FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
}

void FCustomAssetsEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomAssetsEditorModule, CustomAssetsEditor)