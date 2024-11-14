#pragma once

#include "CoreMinimal.h"
#include "CellConnectionData.generated.h"

UCLASS(BlueprintType)
class CUSTOMASSETSEDITORRUNTIME_API UCellConnectionData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite)
	TMap<int32, float> WeightMap;
};
