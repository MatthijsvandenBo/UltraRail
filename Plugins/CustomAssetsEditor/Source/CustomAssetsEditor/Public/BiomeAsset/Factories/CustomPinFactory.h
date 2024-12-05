#pragma once

#include "EdGraphUtilities.h"
#include "CoreMinimal.h"

struct FCustomPinFactory : public FGraphPanelPinFactory
{
	virtual ~FCustomPinFactory() override {}
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* Pin) const override;
};