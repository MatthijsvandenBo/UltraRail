#pragma once

#include "CoreMinimal.h"
#include "RuntimeNode.h"
#include "BiomeAsset/NodeSaveData/CellConnectionData.h"
#include "RuntimeCellConnectionNode.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class CUSTOMASSETSEDITORRUNTIME_API URuntimeCellConnectionNode : public URuntimeNode 
{
	GENERATED_BODY()

public:
	UPROPERTY(Blueprintable)
	UCellConnectionData* NodeInfo = nullptr;
};
