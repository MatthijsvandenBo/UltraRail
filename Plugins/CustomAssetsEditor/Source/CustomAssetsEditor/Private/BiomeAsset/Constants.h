#pragma once
#include "Assets/BiomeAsset.h"

namespace Constants
{

	using UAssetSupportType = UBiomeAsset;
	
	const static FText AssetDisplayName = FText::FromString(TEXT("Biome Asset"));
	constexpr static FColor AssetDisplayColor = FColor(34, 139, 34);

	const static FName CategoryKey(TEXT("BiomeAssetEditor"));
	const static FName MainModeName(TEXT("MainBiomeAssetAppMode"));
	const static FName ApplicationIdentifier(TEXT("BiomeAssetEditor"));

	// Styles
	const static FName StyleSetName = TEXT("BiomeAssetEditorStyle");

	/// Tab Identifiers

	const static FName DetailsTabName = TEXT("Details");
	const static FName DetailsTabIdentifier(TEXT("BiomeAssetApp/DetailsTab"));
	
} // Constants