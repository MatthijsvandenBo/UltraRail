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
		reinterpret_cast<URuntimeCellDefinitionNode*>(NewRuntimeNode)->NodeInfo = reinterpret_cast<UCellDefinitionData*>(
			reinterpret_cast<const UCellDefinitionNode*>(UiNode)->GetNodeInfo());
	}

	if (UiNode->NodeType == ENodeTypes::CellConnection)
	{
		NewRuntimeNode = NewObject<URuntimeCellConnectionNode>(Outer);
		reinterpret_cast<URuntimeCellConnectionNode*>(NewRuntimeNode)->NodeInfo = reinterpret_cast<UCellConnectionData*>(
			reinterpret_cast<const UCellConnectionNode*>(UiNode)->GetNodeInfo());
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
		const auto* NodeInfo = reinterpret_cast<const URuntimeCellDefinitionNode*>(RuntimeNode)->NodeInfo;
		if (NodeInfo == nullptr)
		{
			reinterpret_cast<UCellDefinitionNode*>(NewUiNode)->SetNodeInfo(
				NewObject<UCellDefinitionData>(RuntimeNode)
			);
		}
		else
		{
			reinterpret_cast<UCellDefinitionNode*>(NewUiNode)->SetNodeInfo(
				DuplicateObject(NodeInfo, RuntimeNode)
			);
		}
	}

	if (RuntimeNode->NodeType == ENodeTypes::CellConnection)
	{
		NewUiNode = NewObject<UCellConnectionNode>(Outer);
		const auto* NodeInfo = reinterpret_cast<const URuntimeCellConnectionNode*>(RuntimeNode)->NodeInfo;
		if (NodeInfo == nullptr)
		{
			reinterpret_cast<UCellConnectionNode*>(NewUiNode)->SetNodeInfo(
				NewObject<UCellConnectionData>(RuntimeNode)
			);
		}
		else
		{
			reinterpret_cast<UCellConnectionNode*>(NewUiNode)->SetNodeInfo(
				DuplicateObject(NodeInfo, RuntimeNode)
			);
		}
	}

	if (NewUiNode != nullptr)
		NewUiNode->NodeType = RuntimeNode->NodeType;

	return NewUiNode;
}
