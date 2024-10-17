#pragma once

#include "BiomeAsset/Constants.h"
#include "KismetPins/SGraphPinColor.h"
#include "EdGraph/EdGraphPin.h"
#include "CoreMinimal.h"

/**
 * 
 */
class SCustomGraphPin : public SGraphPin
{
public:
	SLATE_BEGIN_ARGS(SCustomGraphPin) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObject)
	{
		SGraphPin::Construct(SGraphPin::FArguments(), InGraphPinObject);
	}

protected:
	virtual FSlateColor GetPinColor() const override
	{
		return Constants::CustomPinColor;
	}
};