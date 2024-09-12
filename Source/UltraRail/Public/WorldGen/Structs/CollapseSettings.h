#pragma once

#include "CoreMinimal.h"
#include "BlockIdWeight.h"
#include "CollapseSettings.generated.h"

/**
 * This structure is used by the observers for the WaveCollapseGen to determine which blocks are to be allowed next
 */
USTRUCT(Blueprintable)
struct ULTRARAIL_API FCollapseSettings
{
 GENERATED_BODY()

 UPROPERTY(EditAnywhere, BlueprintReadOnly)
 TArray<FBlockIdWeight> AllowedTopIDs;
 UPROPERTY(EditAnywhere, BlueprintReadOnly)
 TArray<FBlockIdWeight> AllowedRightIDs;
 UPROPERTY(EditAnywhere, BlueprintReadOnly)
 TArray<FBlockIdWeight> AllowedBottomIDs;
 UPROPERTY(EditAnywhere, BlueprintReadOnly)
 TArray<FBlockIdWeight> AllowedLeftIDs;
};
