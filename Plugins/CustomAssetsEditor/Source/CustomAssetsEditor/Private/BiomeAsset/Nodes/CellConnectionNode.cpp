#include "BiomeAsset/Nodes/CellConnectionNode.h"

void UCellConnectionNode::ContextDeleteAction_Function()
{
	if (!CanUserDeleteNode())
		return;

	DestroyNode();
}

UEdGraphNode* UCellConnectionNode::FNewNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2D Location, const bool bSelectNewNode)
{
	UCellConnectionNode* NewNode = NewObject<UCellConnectionNode>(ParentGraph);
	NewNode->CreateNewGuid();
	NewNode->NodePosX = Location.X;
	NewNode->NodePosY = Location.Y;

	const auto InputPin = NewNode->CreateCustomPin(EGPD_Input, TEXT("CellDefinition"), Constants::CustomPinSubCategory);

	if (FromPin != nullptr)
		NewNode->GetSchema()->TryCreateConnection(FromPin, InputPin);
	
	ParentGraph->Modify();
	ParentGraph->AddNode(NewNode, true, bSelectNewNode);

	return NewNode;
}