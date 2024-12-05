#pragma once

#include "CoreMinimal.h"
#include "RuntimeNode.h"
#include "UObject/Object.h"
#include "BiomeAsset/NodeSaveData/CellDefinitionData.h"
#include "RuntimeCellDefinitionNode.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CUSTOMASSETSEDITORRUNTIME_API URuntimeCellDefinitionNode : public URuntimeNode
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Blueprintable)
	UCellDefinitionData* NodeInfo = nullptr;
};
