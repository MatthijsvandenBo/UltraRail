#pragma once

#include "CoreMinimal.h"
#include "NodeInfoBase.h"
#include "CellConnectionData.generated.h"

UCLASS(BlueprintType)
class CUSTOMASSETSEDITORRUNTIME_API UCellConnectionData : public UNodeInfoBase 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Blueprintable, BlueprintReadWrite)
	TMap<int32, double> WeightMap;

	UFUNCTION(BlueprintCallable, Category = "Utility")
	void Normalize();
};

