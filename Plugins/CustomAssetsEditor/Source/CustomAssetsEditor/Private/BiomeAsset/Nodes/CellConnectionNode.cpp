#include "BiomeAsset/Nodes/CellConnectionNode.h"
#include "BiomeAsset/NodeSaveData/CellConnectionData.h"

void UCellConnectionNode::ContextDeleteAction_Function()
{
	if (!CanUserDeleteNode())
		return;

	DestroyNode();
}

UEdGraphNode* UCellConnectionNode::FNewNodeAction::PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin,
	const FVector2D Location, const bool bSelectNewNode)
{
	UCellConnectionNode* Result = NewObject<UCellConnectionNode>(ParentGraph);
	Result->CreateNewGuid();
	Result->NodePosX = Location.X;
	Result->NodePosY = Location.Y;
	Result->NodeType = ENodeTypes::CellConnection;
	Result->SetNodeInfo(NewObject<UCellConnectionData>(Result));

	const auto InputPin = Result->CreateCustomPin(EGPD_Input, TEXT("CellDefinition"), Constants::CustomPinSubCategory);

	if (FromPin != nullptr)
		Result->GetSchema()->TryCreateConnection(FromPin, InputPin);
	
	ParentGraph->Modify();
	ParentGraph->AddNode(Result, true, bSelectNewNode);

	return Result;
}