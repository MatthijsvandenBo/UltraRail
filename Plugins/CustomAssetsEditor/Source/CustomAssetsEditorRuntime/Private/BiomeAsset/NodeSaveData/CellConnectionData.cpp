#include "BiomeAsset/NodeSaveData/CellConnectionData.h"

void UCellConnectionData::Normalize()
{
	float TotalWeight = 0;
	for (const auto& [_, Weight] : WeightMap)
		TotalWeight += Weight;

	if (TotalWeight == 0)
		return;

	for (auto& [_, Weight] : WeightMap)
		Weight /= TotalWeight;
}
