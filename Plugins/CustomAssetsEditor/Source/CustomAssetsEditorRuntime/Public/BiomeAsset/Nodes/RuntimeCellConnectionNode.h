#pragma once

#include "CoreMinimal.h"
#include "RuntimeNode.h"
#include "BiomeAsset/Structs/CellConnectionData.h"
#include "RuntimeCellConnectionNode.generated.h"

/**
 * 
 */
UCLASS()
class CUSTOMASSETSEDITORRUNTIME_API URuntimeCellConnectionNode : public URuntimeNode 
{
	GENERATED_BODY()

	UPROPERTY(Blueprintable)
	FCellConnectionData ConnectionData;
};
