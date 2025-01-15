#pragma once

#include "CoreMinimal.h"
#include "BlockIdWeight.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct ULTRARAIL_API FBlockIdWeight
{
 GENERATED_BODY()
public:
 UPROPERTY(EditAnywhere, BlueprintReadOnly)
 int32 BlockID = 0;
 UPROPERTY(EditAnywhere, BlueprintReadOnly)
 double Weight = 1;
};
