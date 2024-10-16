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
	UEdGraphNode* Result = NewObject<UCellGraphNode>(ParentGraph);
	Result->CreateNewGuid();
	Result->NodePosX = Location.X;
	Result->NodePosY = Location.Y;

	Result->CreatePin(
		EGPD_Output,
		TEXT("Outputs"),
		TEXT("ConnectionTop")
	);
	
	Result->CreatePin(
		EGPD_Output,
		TEXT("Outputs"),
		TEXT("ConnectionRight")
	);

	Result->CreatePin(
		EGPD_Output,
		TEXT("Outputs"),
		TEXT("ConnectionBottom")
	);
	Result->CreatePin(
		EGPD_Output,
		TEXT("Outputs"),
		TEXT("ConnectionLeft")
	);

	ParentGraph->Modify();
	ParentGraph->AddNode(Result, true, bSelectNewNode);
	
	return Result;
}

UCellGraphNode::UCellGraphNode()
{
	ContextDeleteAction = FUIAction(FExecuteAction::CreateLambda([this] { ContextDeleteAction_Function(); }));
}

void UCellGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("SectionName"), FText::FromString(TEXT("Custom Node Actions")));

	Section.AddMenuEntry(
		TEXT("DEL"),
		FText::FromString(TEXT("DEL")),
		FText::FromString(TEXT("Deletes this node")),
		FSlateIcon(),
		ContextDeleteAction
	);
}

UEdGraphPin* UCellGraphNode::CreateCustomPin(EEdGraphPinDirection Direction, FName Name)
{
	FName Category = (Direction == EGPD_Input) ? TEXT("Inputs") : TEXT("Outputs");
	FName SubCategory = TEXT("CustomPin");

	UEdGraphPin* Pin = CreatePin(
		Direction,
		Category,
		Name
	);

	Pin->PinType.SubCategory = SubCategory;
	return Pin;
}
