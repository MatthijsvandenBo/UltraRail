#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RuntimeNode.generated.h"

class URuntimePin;
/**
 * 
 */
UCLASS()
class CUSTOMASSETSEDITORRUNTIME_API URuntimeNode : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName NodeClassName;
	
	UPROPERTY()
	URuntimePin* InputPin;

	UPROPERTY()
	TArray<URuntimePin*> OutputPins;

	UPROPERTY()
	FVector2D Position;
};
