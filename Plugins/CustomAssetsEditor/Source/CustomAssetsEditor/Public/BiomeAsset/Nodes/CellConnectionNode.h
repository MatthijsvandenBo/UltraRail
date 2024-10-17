#pragma once

#include "CoreMinimal.h"
#include "CustomGraphNode.h"
#include "CellConnectionNode.generated.h"

/**
 * 
 */
UCLASS()
class UCellConnectionNode : public UCustomGraphNode
{
	GENERATED_BODY()

protected:
	virtual void ContextDeleteAction_Function() override;

public:
	struct FNewNodeAction final : UCustomGraphNode::FNewNodeAction
	{
		FNewNodeAction(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTop, const int32 InCategory)
			: UCustomGraphNode::FNewNodeAction(InNodeCategory, InMenuDesc, InToolTop, InCategory) {}
		
		virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, const bool bSelectNewNode) override;
	};

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString(TEXT("Cell Connections")); }
};
