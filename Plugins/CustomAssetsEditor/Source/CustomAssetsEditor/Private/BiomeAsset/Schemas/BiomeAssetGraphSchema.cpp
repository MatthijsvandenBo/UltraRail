#include "BiomeAsset/Schemas/BiomeAssetGraphSchema.h"

#include "BiomeAsset/Nodes/CellGraphNode.h"

void UBiomeAssetGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	const TSharedPtr<UCellGraphNode::FNewNodeAction> NewCellGraphNodeAction = MakeShareable(
		new UCellGraphNode::FNewNodeAction(
			FText::FromString(TEXT("Creation")),
			FText::FromString(TEXT("CellDefinition")),
			FText::FromString(TEXT("Creates a new node defining a cell ruling")),
			0
		)
	);

	ContextMenuBuilder.AddAction(NewCellGraphNodeAction);
}

const FPinConnectionResponse UBiomeAssetGraphSchema::CanCreateConnection(const UEdGraphPin* A,
	const UEdGraphPin* B) const
{
	if (A == nullptr || B == nullptr)
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT(""));

	if (A->Direction == B->Direction)
		return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Input can only connect to outputs"));

	return FPinConnectionResponse(CONNECT_RESPONSE_BREAK_OTHERS_AB, TEXT(""));
}
