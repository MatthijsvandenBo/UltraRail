#pragma once

#include "EdGraphUtilities.h"
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
		return FSlateColor(FLinearColor(.2f, 1.f, .2f));
	}
};

struct FCustomPinFactory : public FGraphPanelPinFactory
{
public:
	virtual ~FCustomPinFactory() override {}
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override
	{
		if (FName(TEXT("CustomPin")) != Pin->PinType.PinSubCategory)
			return nullptr;

		return SNew(SCustomGraphPin, Pin);
	};
};