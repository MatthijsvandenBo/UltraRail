#pragma once
#include "Assets/BiomeAsset.h"

namespace Constants
{

	using UAssetSupportType = UBiomeAsset;
	
	const static FText AssetDisplayName = FText::FromString(TEXT("Biome Asset"));
	constexpr static FColor AssetDisplayColor = FColor(34, 139, 34);

	const static FName CategoryKey(TEXT("BiomeAssetEditor"));
	const static FName ApplicationIdentifier(TEXT("BiomeAssetEditor"));

	// Mode Names
	
	const static FName MainModeName(TEXT("MainBiomeAssetAppMode"));

	// Styles
	
	const static FName StyleSetName = TEXT("BiomeAssetEditorStyle");

	// Layout versions

	const static FName MainLayoutName = TEXT("MainBiomeAssetAppMode_Layout_v0.0.3");

	/// Tab Identifiers

	const static FName PropertiesTabName = TEXT("Properties");
	const static FName PropertiesTabIdentifier(TEXT("BiomeAssetApp/PropertiesTab"));

	const static FName GraphTabName = TEXT("Graph");
	const static FName GraphTabIdentifier = TEXT("BiomeAssetApp/GraphTab");
	
} // Constants