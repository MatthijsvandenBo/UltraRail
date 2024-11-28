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
	TSubclassOf<AActor> FindTypeByID(int32 SearchID) const noexcept;

	UFUNCTION(BlueprintCallable, Category="Utility|Search")
	int32 FindIdByType(TSubclassOf<AActor> SearchType) const noexcept;

	UFUNCTION(BlueprintCallable, Category="Utility|Search")
	int32 GetDefinitionCount() const noexcept;
	UFUNCTION(BlueprintCallable, Category="Utility|Search")
	int32 GetConnectionCount() const noexcept;
	UFUNCTION(BlueprintCallable, Category="Utility|Search")
	TArray<int32> GetRegisteredIDs() const noexcept;

	TArray<TMap<int32, float>*> GetWeightMapsByID(int32 SearchID) const;
	TMap<int32, float>* GetWeightMapByID(int32 SearchID, int32 Index) const;
	TMap<int32, float>* GetWeightMapByType(const TSubclassOf<AActor>& SearchType, int32 Index) const;

	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetTopWeightMapByID(const int32 SearchID) const noexcept {
		const auto Map = GetWeightMapByID(SearchID, 0);
		if (Map == nullptr)
			return {};
		return *Map;
	}
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetRightWeightMapByID(const int32 SearchID) const noexcept
	{
		const auto Map = GetWeightMapByID(SearchID, 1);
		if (Map == nullptr)
			return {};
		return *Map;
	}
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetBottomWeightMapByID(const int32 SearchID) const noexcept
	{
		const auto Map = GetWeightMapByID(SearchID, 2);
		if (Map == nullptr)
			return {};
		return *Map;
	}
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetLeftWeightMapByID(const int32 SearchID) const noexcept
	{
		const auto Map = GetWeightMapByID(SearchID, 3);
		if (Map == nullptr)
			return {};
		return *Map;
	}
	
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetTopWeightMapByType(const TSubclassOf<AActor> SearchType) const noexcept
	{
		const auto Map = GetWeightMapByType(SearchType, 0);
		if (Map == nullptr)
			return {};
		return *Map;
	}
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetRightWeightMapByType(const TSubclassOf<AActor> SearchType) const noexcept
	{
		const auto Map = GetWeightMapByType(SearchType, 1);
		if (Map == nullptr)
			return {};
		return *Map;
	}
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetBottomWeightMapByType(const TSubclassOf<AActor> SearchType) const noexcept
	{
		const auto Map = GetWeightMapByType(SearchType, 2);
		if (Map == nullptr)
			return {};
		return *Map;
	}
	UFUNCTION(BlueprintCallable, Category="Utility|Weight")
	TMap<int32, float> GetLeftWeightMapByType(const TSubclassOf<AActor> SearchType) const noexcept {
		const auto Map = GetWeightMapByType(SearchType, 3);
		if (Map == nullptr)
			return {};
		return *Map;
	}

private:
	template <class T>
	TArray<T*> FindNodeTypes() const;

};

template <class T>
TArray<T*> UBiomeAsset::FindNodeTypes() const
{
	TArray<T*> Array;

	for (const auto Node : Graph->Nodes)
	{
		if (auto Casted = Cast<T>(Node); Casted != nullptr)
			Array.Add(Casted);
	}

	return Array;
}
