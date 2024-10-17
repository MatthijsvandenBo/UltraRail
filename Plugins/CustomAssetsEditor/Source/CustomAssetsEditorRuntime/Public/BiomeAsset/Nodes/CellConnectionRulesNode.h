#pragma once

#include "CoreMinimal.h"
#include "BiomeAsset/Structs/CellConnectionData.h"
#include "UObject/Object.h"
#include "CellConnectionRulesNode.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMASSETSEDITORRUNTIME_API UCellConnectionRulesNode : public UObject
{
	GENERATED_BODY()

	UPROPERTY(Blueprintable)
	FCellConnectionData ConnectionData;
};
