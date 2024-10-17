#pragma once

#include "CoreMinimal.h"
#include "CellDefinitionData.generated.h"

USTRUCT(Blueprintable)
struct CUSTOMASSETSEDITORRUNTIME_API FCellDefinitionData
{
	GENERATED_BODY()

	UPROPERTY(Blueprintable, BlueprintReadWrite)
	int32 ID;

	UPROPERTY(Blueprintable, BlueprintReadWrite)
	TSubclassOf<AActor> Block;
};
