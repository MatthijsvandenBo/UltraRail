#include "BiomeAsset/Actions/BiomeAssetAction.h"

#include "BiomeAsset/Apps/BiomeAssetEditorApp.h"
#include "Assets/BiomeAsset.h"
#include "BiomeAsset/Constants.h"

FBiomeAssetAction::FBiomeAssetAction(const EAssetTypeCategories::Type Category)
{
	AssetCategory = Category;
}

FText FBiomeAssetAction::GetName() const
{
	return Constants::AssetDisplayName; 
}

FColor FBiomeAssetAction::GetTypeColor() const
{
	return Constants::AssetDisplayColor;
}

UClass* FBiomeAssetAction::GetSupportedClass() const
{
	return Constants::UAssetSupportType::StaticClass();
}

void FBiomeAssetAction::OpenAssetEditor(const TArray<UObject*>& InObjects,
	const TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (const auto Object : InObjects)
	{
		Constants::UAssetSupportType* BiomeAsset = Cast<Constants::UAssetSupportType>(Object);
		if (BiomeAsset == nullptr)
			continue;

		const TSharedRef<FBiomeAssetEditorApp> EditorApp(new FBiomeAssetEditorApp());
		EditorApp->InitEditor(Mode, EditWithinLevelEditor, BiomeAsset);
	}
}

uint32 FBiomeAssetAction::GetCategories()
{
	return AssetCategory;
}

