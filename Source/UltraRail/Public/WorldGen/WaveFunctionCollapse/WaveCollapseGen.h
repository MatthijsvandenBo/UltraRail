#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WorldGen/WaveFunctionCollapse/BlockStateObserver.h"
#include "WorldGen/WaveFunctionCollapse/FieldObserver.h"

#include "WaveCollapseGen.generated.h"

class UBiomeBlockIDs;
class ABlock;

UCLASS(Blueprintable, BlueprintType)
class ULTRARAIL_API AWaveCollapseGen : public AActor, public IBlockStateObserver, public IFieldObserver
{
	GENERATED_BODY()

	// Exposed Fields 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid",
		meta=(AllowPrivateAccess))
	int32 GridWidth = 30;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid",
		meta=(AllowPrivateAccess))
	int32 GridDepth = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Blocks",
		meta=(AllowPrivateAccess))
	TObjectPtr<UBiomeBlockIDs> BiomeBlockIDs = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generation",
		meta=(AllowPrivateAccess))
	TObjectPtr<AActor> BlockStateObserver = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generation",
		meta=(AllowPrivateAccess))
	TObjectPtr<AActor> FieldObserver = nullptr;
	

	// Non-exposed Fields
	
	UPROPERTY(Blueprintable)
	TMap<int32, TSubclassOf<ABlock>> ToBlockLookupMap;
	UPROPERTY(Blueprintable)
	TMap<TSubclassOf<ABlock>, int32> ToIdLookupMap;

	UPROPERTY()
	TArray<FBlockState> BlockStates;
	UPROPERTY()
	bool bIsEmpty = true;
	
	UPROPERTY()
	FVector2f LastObserved = {0, 0};

public:
	// Sets default values for this actor's properties
	AWaveCollapseGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/// --- Field Observer Overrides

	virtual bool GetCurrentOptimalLocation_Implementation(int32& X, int32& Y) override;
	virtual void GetFieldState_Implementation(TArray<FBlockState>& FieldState) override;
	virtual bool GetItem_Implementation(int32 X, int32 Y, FBlockState& BlockState) override;
	virtual int32 GetFieldWidth_Implementation() override;
	virtual int32 GetFieldDepth_Implementation() override;
	virtual void TranslateIndexToCart_Implementation(int64 Index, int32& X, int32& Y) override;
	virtual int64 TranslateIndexFromCart_Implementation(int32 X, int32 Y) override;
	virtual bool AreNeighbouring_Implementation(TScriptInterface<IFieldObserver>& Observer, int32 X1, int32 Y1, int32 X2, int32 Y2) override;
	virtual void GetNeighbours_Implementation(int32 X, int32 Y, TArray<FBlockState>& Neighbours) override;
	
	/// --- Block State Observer Overrides

	virtual void ObserveBlock_Implementation(TScriptInterface<IFieldObserver>& Observer, int32 X, int32 Y) override;
	virtual void GetLastObserved_Implementation(int32& X, int32& Y) override;

};

