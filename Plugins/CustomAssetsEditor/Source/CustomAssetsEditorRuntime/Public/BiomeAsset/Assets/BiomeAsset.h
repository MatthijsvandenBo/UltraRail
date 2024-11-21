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
	UPROPERTY()
	UBiomeAssetRuntimeGraph* Graph = nullptr;

	UFUNCTION(BlueprintCallable, Category="Utility|Search")
	TSubclassOf<AActor> FindTypeByID(int32 SearchID);

	UFUNCTION(BlueprintCallable, Category="Utility|Search")
	int32 FindIdByType(TSubclassOf<AActor> SearchType);

	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetTopWeightMapByID(const int32 SearchID) { return GetWeightMapByID(SearchID, 0); }
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetRightWeightMapByID(const int32 SearchID) { return GetWeightMapByID(SearchID, 1); }
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetBottomWeightMapByID(const int32 SearchID) { return GetWeightMapByID(SearchID, 2); }
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetLeftWeightMapByID(const int32 SearchID) { return GetWeightMapByID(SearchID, 3); }
	
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetTopWeightMapByType(const TSubclassOf<AActor> SearchType) { return GetWeightMapByType(SearchType, 0); }
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetRightWeightMapByType(const TSubclassOf<AActor> SearchType) { return GetWeightMapByType(SearchType, 1); }
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetBottomWeightMapByType(const TSubclassOf<AActor> SearchType) { return GetWeightMapByType(SearchType, 2); }
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetLeftWeightMapByType(const TSubclassOf<AActor> SearchType) { return GetWeightMapByType(SearchType, 3); }

private:
	template <class T>
	TArray<T*> FindNodeTypes();

	TMap<int32, float> GetWeightMapByID(int32 SearchID, int32 Index);
	TMap<int32, float> GetWeightMapByType(const TSubclassOf<AActor>& SearchType, int32 Index);
};

template <class T>
TArray<T*> UBiomeAsset::FindNodeTypes()
{
	TArray<T*> Array;

	for (const auto Node : Graph->Nodes)
	{
		if (auto Casted = Cast<T>(Node); Casted != nullptr)
			Array.Add(Casted);
	}

	return Array;
}
