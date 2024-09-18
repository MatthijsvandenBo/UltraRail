#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UltraRail/Public/WorldGen/WaveFunctionCollapse/Structs/BlockIdConnection.h"
#include "BiomeBlockIDs.generated.h"


/**
 * 
 */
UCLASS()
class ULTRARAIL_API UBiomeBlockIDs : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FBlockIdConnection> BlockIdConnections;
	
	UFUNCTION(BlueprintCallable)
	void NormalizeWeights();
};
