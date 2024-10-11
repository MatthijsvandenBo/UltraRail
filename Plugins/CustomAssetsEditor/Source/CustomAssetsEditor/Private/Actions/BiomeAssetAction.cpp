#include "Actions/BiomeAssetAction.h"

#include "Apps/BiomeAssetEditorApp.h"
#include "Assets/BiomeAsset.h"

FBiomeAssetAction::FBiomeAssetAction(const EAssetTypeCategories::Type Category)
{
	AssetCategory = Category;
}

FText FBiomeAssetAction::GetName() const
{
	return FText::FromString(TEXT("Biome Asset"));
}

FColor FBiomeAssetAction::GetTypeColor() const
{
	return FColor(34, 139, 34);
}

UClass* FBiomeAssetAction::GetSupportedClass() const
{
	return UBiomeAsset::StaticClass();
}

void FBiomeAssetAction::OpenAssetEditor(const TArray<UObject*>& InObjects,
	const TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	const EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone;
	for (const auto Object : InObjects)
	{
		UBiomeAsset* BiomeAsset = Cast<UBiomeAsset>(Object);
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

