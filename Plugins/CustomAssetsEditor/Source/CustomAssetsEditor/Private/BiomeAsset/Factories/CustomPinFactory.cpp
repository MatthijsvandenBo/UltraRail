#include "BiomeAsset/Factories/CustomPinFactory.h"

#include "BiomeAsset/Constants.h"
#include "BiomeAsset/Pins/CustomGraphPin.h"

TSharedPtr<SGraphPin> FCustomPinFactory::CreatePin(UEdGraphPin* Pin) const
{
	if (Constants::CustomPinSubCategory != Pin->PinType.PinSubCategory)
		return nullptr;

	return SNew(SCustomGraphPin, Pin);
};
