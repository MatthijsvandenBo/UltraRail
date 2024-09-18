#pragma once

#include "CoreMinimal.h"
#include "CollapseSettings.h"
#include "BlockState.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct ULTRARAIL_API FBlockState
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BlockID = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCollapseSettings CollapseSettings;
};
