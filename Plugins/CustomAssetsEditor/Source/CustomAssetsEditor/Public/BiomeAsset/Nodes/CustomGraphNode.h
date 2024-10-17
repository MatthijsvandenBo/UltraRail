#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "BiomeAsset/Constants.h"
#include "CustomGraphNode.generated.h"

/*
 *
 */
UCLASS()
class UCustomGraphNode : public UEdGraphNode
{
	GENERATED_BODY()

	FUIAction ContextDeleteAction;
	
protected:
	 virtual void ContextDeleteAction_Function() {};

public:
	UCustomGraphNode();
	virtual ~UCustomGraphNode() override {};

	struct FNewNodeAction : public FEdGraphSchemaAction
	{
		FNewNodeAction() {}
		FNewNodeAction(const FText& InNodeCategory, const FText& InMenuDesc, const FText& InToolTip, const int32 InGrouping)
			: FEdGraphSchemaAction(InNodeCategory, InMenuDesc, InToolTip, InGrouping) {}

		virtual UEdGraphNode* PerformAction(UEdGraph* ParentGraph, UEdGraphPin* FromPin, const FVector2D Location, const bool bSelectNewNode) override
		{
			return nullptr;
		}
	};

	virtual FLinearColor GetNodeTitleColor() const override { return Constants::AssetDisplayColor; }
	virtual bool CanUserDeleteNode() const override { return true; }
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;

	virtual UEdGraphPin* CreateCustomPin(EEdGraphPinDirection Direction, FName PinName, FName PinSubCategory);
};
