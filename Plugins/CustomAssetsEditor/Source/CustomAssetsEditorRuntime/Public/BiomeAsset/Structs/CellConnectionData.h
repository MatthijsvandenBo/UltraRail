#pragma once

#include "CoreMinimal.h"
#include "CellConnectionData.generated.h"

USTRUCT(Blueprintable)
struct CUSTOMASSETSEDITORRUNTIME_API FCellConnectionData
{
	GENERATED_BODY()
	
	UPROPERTY(Blueprintable, BlueprintReadWrite)
	TMap<int32, float> WeightMap;
};
