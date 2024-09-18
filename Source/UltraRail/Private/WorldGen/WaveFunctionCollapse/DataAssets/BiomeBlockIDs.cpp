﻿#include "UltraRail/Public/WorldGen/WaveFunctionCollapse/DataAssets/BiomeBlockIDs.h"

// LOCAL DECLARATIONS

void NormalizeCollapseWeights(TArray<FBlockIdWeight>& Weights);

// END LOCAL DECLARATIONS

void UBiomeBlockIDs::NormalizeWeights()
{
	for (auto& [Block, ID, CollapseSettings] : BlockIdConnections)
	{
		NormalizeCollapseWeights(CollapseSettings.AllowedTopIDs);
		NormalizeCollapseWeights(CollapseSettings.AllowedRightIDs);
		NormalizeCollapseWeights(CollapseSettings.AllowedBottomIDs);
		NormalizeCollapseWeights(CollapseSettings.AllowedLeftIDs);
	}
}


// LOCAL IMPLEMENTATIONS

void NormalizeCollapseWeights(TArray<FBlockIdWeight>& Weights)
{
	float TotalValue = 0;

	for (const auto& [_, Weight] : Weights)
		TotalValue += Weight;

	for (auto& [_, Weight] : Weights)
		Weight /= TotalValue;
}

// END LOCAL IMPLEMENTATIONS