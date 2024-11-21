#include "BiomeASset/Assets/BiomeAsset.h"

#include "BiomeAsset/Nodes/RuntimeCellConnectionNode.h"

TSubclassOf<AActor> UBiomeAsset::FindTypeByID(int32 SearchID)
{
	const auto SearchingNodes = FindNodeTypes<URuntimeCellDefinitionNode>();
	for (const auto Node : SearchingNodes)
	{
		if (Node->NodeInfo->ID == SearchID)
			return Node->NodeInfo->Block;
	}

	return nullptr;
}

int32 UBiomeAsset::FindIdByType(TSubclassOf<AActor> SearchType)
{
	const auto SearchingNodes = FindNodeTypes<URuntimeCellDefinitionNode>();
	for (const auto Node : SearchingNodes)
	{
		if (Node->NodeInfo->Block == SearchType)
			return Node->NodeInfo->ID;
	}

	return -1;
}

TMap<int32, float> UBiomeAsset::GetWeightMapByID(const int32 SearchID, const int32 Index)
{
	const auto SearchingNodes = FindNodeTypes<URuntimeCellDefinitionNode>();
	URuntimeCellDefinitionNode* TargetNode = nullptr;
	for (const auto Node : SearchingNodes)
	{
		if (Node->NodeInfo->ID == SearchID)
		{
			TargetNode = Node;
			break;
		}
	}

	if (TargetNode == nullptr)
		return {};

	return reinterpret_cast<URuntimeCellConnectionNode*>(TargetNode->OutputPins[Index])->NodeInfo->WeightMap;
}

TMap<int32, float> UBiomeAsset::GetWeightMapByType(const TSubclassOf<AActor>& SearchType, int32 Index)
{
	const auto SearchID = FindIdByType(SearchType);
	return GetWeightMapByID(SearchID, Index);
}
