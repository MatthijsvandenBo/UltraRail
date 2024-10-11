#include "Actions/BiomeAssetAction.h"
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
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	// TODO! Open de editor
}

uint32 FBiomeAssetAction::GetCategories()
{
	return AssetCategory;
}

