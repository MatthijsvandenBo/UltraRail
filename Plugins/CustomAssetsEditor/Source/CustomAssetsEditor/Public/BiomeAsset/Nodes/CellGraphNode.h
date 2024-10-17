#pragma once

#include "CoreMinimal.h"
#include "CustomGraphNode.h"
#include "CellGraphNode.generated.h"


/**
 * 
 */
UCLASS()
class UCellGraphNode : public UCustomGraphNode 
{
	GENERATED_BODY()
	
protected:
	virtual void ContextDeleteAction_Function() override;
	
public:
    struct FNewNodeAction final : public UCustomGraphNode::FNewNodeAction
    {
    	FNewNodeAction() {}
		FNewNodeAction(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
			: UCustomGraphNode::FNewNodeAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}

    	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode) override;
    };

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString(TEXT("Cell")); }
};


