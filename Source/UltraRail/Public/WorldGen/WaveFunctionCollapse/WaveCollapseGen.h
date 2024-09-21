#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interfaces/CellStateObserver.h"
#include "Interfaces/FieldObserver.h"

#include "WaveCollapseGen.generated.h"

class UBiomeBlockIDs;
class ABlock;

UCLASS(Blueprintable, BlueprintType)
class ULTRARAIL_API AWaveCollapseGen : public AActor, public ICellStateObserver, public IFieldObserver
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
	TArray<FCellState> FieldState;
	
	UPROPERTY()
	FIntVector2 LastObserved = {0, 0};

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
#pragma region FIELD_OBSERVER_IMPLEMENTATIONS
	
	virtual void SetupField_Implementation(UBiomeBlockIDs* Data) override;
	virtual bool GetCurrentOptimalLocation_Implementation(int32& X, int32& Y) override;
	virtual void GetFieldState_Implementation(TArray<FCellState>& Field) override;
	virtual bool GetCell_Implementation(int32 X, int32 Y, FCellState& CellState) override;
	virtual bool SetCell_Implementation(int32 X, int32 Y, FCellState NewCellState) override;
	virtual int32 GetFieldWidth_Implementation() override;
	virtual int32 GetFieldDepth_Implementation() override;
	virtual void TranslateIndexToCart_Implementation(int64 Index, int32& X, int32& Y) override;
	virtual int64 TranslateIndexFromCart_Implementation(int32 X, int32 Y) override;
	virtual bool AreNeighbouring_Implementation(TScriptInterface<IFieldObserver>& Observer, int32 X1, int32 Y1, int32 X2, int32 Y2) override;
	virtual void GetNeighbours_Implementation(int32 X, int32 Y, TArray<FIntVector>& Neighbours) override;

	virtual bool GetTopNeighbour_Implementation(int32 X, int32 Y, FCellState& TopNeighbour) override;
	virtual bool GetRightNeighbour_Implementation(int32 X, int32 Y, FCellState& RightNeighbour) override;
	virtual bool GetBottomNeighbour_Implementation(int32 X, int32 Y, FCellState& BottomNeighbour) override;
	virtual bool GetLeftNeighbour_Implementation(int32 X, int32 Y, FCellState& LeftNeighbour) override;

#pragma endregion // FIELD_OBSERVER_IMPLEMENTATION
	
	/// --- Block State Observer Overrides
#pragma region CELL_OBSERVER_IMPLEMENTATION
	
	virtual void ObserveCell_Implementation(UObject* Observer, int32 X, int32 Y) override;
	virtual void GetLastObserved_Implementation(int32& X, int32& Y) override;

#pragma endregion // CELL_OBSERVER_IMPLEMENTATION
};

