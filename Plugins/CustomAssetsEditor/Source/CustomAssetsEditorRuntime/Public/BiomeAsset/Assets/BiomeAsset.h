#pragma once

#include "CoreMinimal.h"
#include "BiomeAsset/Graphs/BiomeAssetRuntimeGraph.h"
#include "BiomeAsset.generated.h"

/**
 * This is a custom asset written to be used for wave-function-collapse world generation
 */
UCLASS(BlueprintType)
class CUSTOMASSETSEDITORRUNTIME_API UBiomeAsset : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TSubclassOf<AActor>, int32> RegisteredIds;

	UPROPERTY()
	UBiomeAssetRuntimeGraph* Graph = nullptr;
};
