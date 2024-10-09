#pragma once

#include "CoreMinimal.h"
#include "CollapseSettings.h"
#include "CellState.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct ULTRARAIL_API FCellState
{
	GENERATED_BODY()

public:

	static inline constexpr int32 Empty_State = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BlockID = Empty_State;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FBlockIdWeight> Entropy;
};
