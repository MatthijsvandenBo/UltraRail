#pragma once

#include "CoreMinimal.h"
#include "RuntimeNode.h"
#include "UObject/Object.h"
#include "BiomeAsset/Structs/CellDefinitionData.h"
#include "RuntimeCellDefinitionNode.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMASSETSEDITORRUNTIME_API URuntimeCellDefinitionNode : public URuntimeNode
{
	GENERATED_BODY()
	
public:
	FCellDefinitionData IdData;
};
