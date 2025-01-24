#include "BiomeAsset/Factories/CustomNodeFactory.h"

#include "BiomeAsset/Nodes/CellConnectionNode.h"
#include "BiomeAsset/Nodes/RuntimeCellConnectionNode.h"
#include "BiomeAsset/Nodes/RuntimeCellDefinitionNode.h"
#include "BiomeAsset/Nodes/CellDefinitionNode.h"

FCustomNodeFactory::FCustomNodeFactory()
{
}

URuntimeNode* FCustomNodeFactory::CreateRuntimeNode(const UCustomGraphNode* UiNode, UObject* Outer)
{
	URuntimeNode* NewRuntimeNode = nullptr;

	if (UiNode->NodeType == ENodeTypes::CellDefinition)
	{
		NewRuntimeNode = NewObject<URuntimeCellDefinitionNode>(Outer);
		Cast<URuntimeCellDefinitionNode>(NewRuntimeNode)->NodeInfo = Cast<UCellDefinitionData>(
			Cast<UCellDefinitionNode>(UiNode)->GetNodeInfo());
		check(IsValid(Cast<URuntimeCellDefinitionNode>(NewRuntimeNode)->NodeInfo))
	}

	if (UiNode->NodeType == ENodeTypes::CellConnection)
	{
		NewRuntimeNode = NewObject<URuntimeCellConnectionNode>(Outer);
		auto CastedNode = Cast<UCellConnectionNode>(UiNode);
		auto CastedInfo = Cast<UCellConnectionData>(CastedNode->GetNodeInfo());
		// Normalize the weights when saving the value
		CastedInfo->Normalize();
		Cast<URuntimeCellConnectionNode>(NewRuntimeNode)->NodeInfo = CastedInfo;
		check(IsValid(Cast<URuntimeCellConnectionNode>(NewRuntimeNode)->NodeInfo))
	}

	if (NewRuntimeNode != nullptr)
		NewRuntimeNode->NodeType = UiNode->NodeType;
	
	return NewRuntimeNode;
}

UCustomGraphNode* FCustomNodeFactory::CreateEditorNode(URuntimeNode* RuntimeNode, UObject* Outer)
{
	UCustomGraphNode* NewUiNode = nullptr;

	if (RuntimeNode->NodeType == ENodeTypes::CellDefinition)
	{
		NewUiNode = NewObject<UCellDefinitionNode>(Outer);
		const auto* NodeInfo = Cast<URuntimeCellDefinitionNode>(RuntimeNode)->NodeInfo;
		if (NodeInfo == nullptr)
		{
			Cast<UCellDefinitionNode>(NewUiNode)->SetNodeInfo(
				NewObject<UCellDefinitionData>(RuntimeNode)
			);
		}
		else
		{
			Cast<UCellDefinitionNode>(NewUiNode)->SetNodeInfo(
				DuplicateObject(NodeInfo, RuntimeNode)
			);
		}
	}

	if (RuntimeNode->NodeType == ENodeTypes::CellConnection)
	{
		NewUiNode = NewObject<UCellConnectionNode>(Outer);
		const auto* NodeInfo = Cast<URuntimeCellConnectionNode>(RuntimeNode)->NodeInfo;
		if (NodeInfo == nullptr)
		{
			Cast<UCellConnectionNode>(NewUiNode)->SetNodeInfo(
				NewObject<UCellConnectionData>(RuntimeNode)
			);
		}
		else
		{
			Cast<UCellConnectionNode>(NewUiNode)->SetNodeInfo(
				DuplicateObject(NodeInfo, RuntimeNode)
			);
		}
	}

	if (NewUiNode != nullptr)
		NewUiNode->NodeType = RuntimeNode->NodeType;

	return NewUiNode;
}
