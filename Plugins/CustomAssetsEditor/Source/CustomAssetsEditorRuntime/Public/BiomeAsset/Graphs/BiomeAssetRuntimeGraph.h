#pragma once

#include "CoreMinimal.h"
#include "BiomeAsset/Nodes/RuntimeNode.h"
#include "UObject/NameTypes.h"
#include "BiomeASsetRuntimeGraph.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMASSETSEDITORRUNTIME_API UBiomeAssetRuntimeGraph : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<URuntimeNode*> Nodes;
};
