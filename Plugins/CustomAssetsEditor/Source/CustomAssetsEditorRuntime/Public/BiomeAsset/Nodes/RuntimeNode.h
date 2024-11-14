#pragma once

#include "CoreMinimal.h"
#include "NodeTypes.h"
#include "UObject/Object.h"
#include "RuntimeNode.generated.h"

class URuntimePin;
/**
 * 
 */
UCLASS(BlueprintType)
class CUSTOMASSETSEDITORRUNTIME_API URuntimeNode : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	ENodeTypes NodeType = ENodeTypes::CellDefinition;
	
	UPROPERTY()
	URuntimePin* InputPin;

	UPROPERTY()
	TArray<URuntimePin*> OutputPins;

	UPROPERTY()
	FVector2D Position;
};
