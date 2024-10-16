#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "BiomeAsset/Constants.h"
#include "CellGraphNode.generated.h"


/**
 * 
 */
UCLASS()
class UCellGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

private:
	FUIAction ContextDeleteAction;
	void ContextDeleteAction_Function();
	
public:
    struct FNewNodeAction final : public FEdGraphSchemaAction
    {
    	FNewNodeAction() {}
		FNewNodeAction(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
			: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}

    	virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, bool bSelectNewNode) override;
    };

	UCellGraphNode();

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override { return FText::FromString(TEXT("Cell")); }
	virtual FLinearColor GetNodeTitleColor() const override { return Constants::AssetDisplayColor; }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	UEdGraphPin* CreateCustomPin(EEdGraphPinDirection Direction, FName Name);
};


