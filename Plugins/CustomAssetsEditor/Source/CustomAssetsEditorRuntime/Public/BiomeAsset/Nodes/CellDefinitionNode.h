#pragma once

#include "CoreMinimal.h"
#include "RuntimeNode.h"
#include "UObject/Object.h"
#include "BiomeAsset/Structs/CellDefinitionData.h"
#include "CellDefinitionNode.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMASSETSEDITORRUNTIME_API UCellDefinitionNode : public URuntimeNode
{
	GENERATED_BODY()
	
public:
	FCellDefinitionData IdData;
};
