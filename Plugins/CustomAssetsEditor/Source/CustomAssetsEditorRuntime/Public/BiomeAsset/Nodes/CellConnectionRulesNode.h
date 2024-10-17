#pragma once

#include "CoreMinimal.h"
#include "RuntimeNode.h"
#include "BiomeAsset/Structs/CellConnectionData.h"
#include "CellConnectionRulesNode.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMASSETSEDITORRUNTIME_API UCellConnectionRulesNode : public URuntimeNode 
{
	GENERATED_BODY()

	UPROPERTY(Blueprintable)
	FCellConnectionData ConnectionData;
};
