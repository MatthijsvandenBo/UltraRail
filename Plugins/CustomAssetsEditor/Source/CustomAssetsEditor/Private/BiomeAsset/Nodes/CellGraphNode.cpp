#include "BiomeAsset/Nodes/CellGraphNode.h"

void UCellGraphNode::ContextDeleteAction_Function()
{
	if (!CanUserDeleteNode())
		return;
	
	DestroyNode();
}

UEdGraphNode* UCellGraphNode::FNewNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
                                                            const FVector2D Location, const bool bSelectNewNode)
{
	UCellGraphNode* Result = NewObject<UCellGraphNode>(ParentGraph);
	Result->CreateNewGuid();
	Result->NodePosX = Location.X;
	Result->NodePosY = Location.Y;

	Result->CreateCustomPin(EGPD_Output, TEXT("ConnectionTop"), Constants::CustomPinSubCategory);
	Result->CreateCustomPin(EGPD_Output, TEXT("ConnectionRight"), Constants::CustomPinSubCategory);
	Result->CreateCustomPin(EGPD_Output, TEXT("ConnectionBottom"), Constants::CustomPinSubCategory);
	Result->CreateCustomPin(EGPD_Output, TEXT("ConnectionLeft"), Constants::CustomPinSubCategory);

	ParentGraph->Modify();
	ParentGraph->AddNode(Result, true, bSelectNewNode);
	
	return Result;
}
