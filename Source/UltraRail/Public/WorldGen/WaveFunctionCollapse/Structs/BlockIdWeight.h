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
 UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(UIMin = "0.0", UIMax = "1.0"))
 float Weight = 1;
};
