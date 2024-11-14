#include "BiomeAsset/Nodes/CellDefinitionNode.h"
#include "BiomeAsset/NodeSaveData/CellDefinitionData.h"

void UCellDefinitionNode::ContextDeleteAction_Function()
{
	if (!CanUserDeleteNode())
		return;
	
	DestroyNode();
}

UEdGraphNode* UCellDefinitionNode::FNewNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2D Location, const bool bSelectNewNode)
{
	UCellDefinitionNode* Result = NewObject<UCellDefinitionNode>(ParentGraph);
	Result->CreateNewGuid();
	Result->NodePosX = Location.X;
	Result->NodePosY = Location.Y;
	Result->NodeType = ENodeTypes::CellDefinition;
	Result->SetNodeInfo(NewObject<UCellDefinitionData>(Result));

	Result->CreateCustomPin(EGPD_Output, TEXT("ConnectionTop"), Constants::CustomPinSubCategory);
	Result->CreateCustomPin(EGPD_Output, TEXT("ConnectionRight"), Constants::CustomPinSubCategory);
	Result->CreateCustomPin(EGPD_Output, TEXT("ConnectionBottom"), Constants::CustomPinSubCategory);
	Result->CreateCustomPin(EGPD_Output, TEXT("ConnectionLeft"), Constants::CustomPinSubCategory);

	ParentGraph->Modify();
	ParentGraph->AddNode(Result, true, bSelectNewNode);
	
	return Result;
}
