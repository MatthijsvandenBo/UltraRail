#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RuntimePin.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMASSETSEDITORRUNTIME_API URuntimePin : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FName PinName;
	
	UPROPERTY()
	FGuid PinId;

	UPROPERTY()
	URuntimePin* Connection = nullptr;
};
