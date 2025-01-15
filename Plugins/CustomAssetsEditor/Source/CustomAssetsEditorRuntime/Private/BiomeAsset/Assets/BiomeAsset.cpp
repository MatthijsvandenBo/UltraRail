#include "BiomeASset/Assets/BiomeAsset.h"

#include "BiomeAsset/Nodes/RuntimeCellConnectionNode.h"
#include "BiomeAsset/Pins/RuntimePin.h"

TSubclassOf<AActor> UBiomeAsset::FindTypeByID(const int32 SearchID) const noexcept
{
	const auto SearchingNodes = FindNodeTypes<URuntimeCellDefinitionNode>();
	for (const auto Node : SearchingNodes)
	{
		if (Node->NodeInfo->ID == SearchID)
			return Node->NodeInfo->Block;
	}

	return nullptr;
}

int32 UBiomeAsset::FindIdByType(const TSubclassOf<AActor> SearchType) const noexcept
{
	const auto SearchingNodes = FindNodeTypes<URuntimeCellDefinitionNode>();
	for (const auto Node : SearchingNodes)
	{
		if (Node->NodeInfo->Block == SearchType)
			return Node->NodeInfo->ID;
	}

	return -1;
}

int32 UBiomeAsset::GetDefinitionCount() const noexcept
{
	return FindNodeTypes<URuntimeCellDefinitionNode>().Num();
}

int32 UBiomeAsset::GetConnectionCount() const noexcept
{
	return FindNodeTypes<URuntimeCellConnectionNode>().Num();
}

TArray<int32> UBiomeAsset::GetRegisteredIDs() const noexcept
{
	const auto SearchingNodes = FindNodeTypes<URuntimeCellDefinitionNode>();
	TArray<int32> RegisteredIDs;
	RegisteredIDs.Reserve(SearchingNodes.Num());
	for (const auto Node : SearchingNodes)
		RegisteredIDs.Add(Node->NodeInfo->ID);

	return RegisteredIDs;
		
}

TArray<TMap<int32, double>*> UBiomeAsset::GetWeightMapsByID(const int32 SearchID) const
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

	TArray<TMap<int32, double>*> Weights;
	Weights.Reserve(TargetNode->OutputPins.Num());
	for (const auto* OutputPin : TargetNode->OutputPins)
	{
		// Add in an empty when the output pin is not connected to anything
		// This has the purpose of keeping the cardinal directions aligned
		if (OutputPin->Connection == nullptr)
		{
			Weights.Add({});
			continue;
		}
		
		Weights.Add(&Cast<URuntimeCellConnectionNode>(OutputPin->Connection->ParentNode)->NodeInfo->WeightMap);
	}

	return Weights;
}

TMap<int32, double>* UBiomeAsset::GetWeightMapByID(const int32 SearchID, const int32 Index) const
{
	const auto Maps = GetWeightMapsByID(SearchID);
	if (Index >= Maps.Num() || Index < 0)
		return nullptr;
	
	return Maps[Index];
}

TMap<int32, double>* UBiomeAsset::GetWeightMapByType(const TSubclassOf<AActor>& SearchType, const int32 Index) const
{
	const auto SearchID = FindIdByType(SearchType);
	return GetWeightMapByID(SearchID, Index);
}
