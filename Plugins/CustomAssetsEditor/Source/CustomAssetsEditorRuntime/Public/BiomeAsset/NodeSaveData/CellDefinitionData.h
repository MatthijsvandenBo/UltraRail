#pragma once

#include "CoreMinimal.h"
#include "CellDefinitionData.generated.h"

UCLASS(BlueprintType)
class CUSTOMASSETSEDITORRUNTIME_API UCellDefinitionData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Blueprintable)
	int32 ID;

	UPROPERTY(EditAnywhere, Blueprintable)
	TSubclassOf<AActor> Block;
};
