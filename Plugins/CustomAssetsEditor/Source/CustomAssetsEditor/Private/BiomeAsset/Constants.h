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

	/// Tab Identifiers
	
	const static FName PrimaryTabIdentifier(TEXT("BiomeAssetAppPrimaryTab"));
	
} // Constants