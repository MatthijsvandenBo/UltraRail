#include "BiomeAsset/Nodes/CustomGraphNode.h"

UCustomGraphNode::UCustomGraphNode()
{
	ContextDeleteAction = FUIAction(
		FExecuteAction::CreateLambda(
			[this] { ContextDeleteAction_Function(); }
		)
	);
}

void UCustomGraphNode::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	FToolMenuSection& Section = Menu->AddSection(TEXT("SectionName"), FText::FromString(TEXT("Custom Node Actions")));

	Section.AddMenuEntry(
		TEXT("DEL"),
		FText::FromString(TEXT("Delete")),
		FText::FromString(TEXT("Deletes this node")),
		FSlateIcon(),
		ContextDeleteAction
	);
}

UEdGraphPin* UCustomGraphNode::CreateCustomPin(const EEdGraphPinDirection Direction,
                                               const FName PinName, const FName PinSubCategory)
{
	const FName Category = Direction == EGPD_Input ? TEXT("Inputs") : TEXT("Outputs");

	UEdGraphPin* Pin = CreatePin(
		Direction,
		Category,
		PinName
	);

	Pin->PinType.PinSubCategory = PinSubCategory;
	return Pin;
}
