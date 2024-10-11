﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/FieldObserver.h"
#include "BasicFieldObserver.generated.h"

class UBiomeBlockIDs;

UCLASS(Blueprintable)
class ULTRARAIL_API ABasicFieldObserver : public AActor, public IFieldObserver
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FCellState> FieldState;
	UPROPERTY()
	int64 BiomeBlockCount = 0;
	UPROPERTY()
	int32 FieldWidth = 0;
	UPROPERTY()
	int32 FieldDepth = 0;

public:
	// Sets default values for this actor's properties
	ABasicFieldObserver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region FIELD_OBSERVER_IMPLEMENTATIONS

	virtual void  TranslateIndexToCart_Implementation(const int64 Index, int32& X, int32& Y) override;
	virtual int64 TranslateIndexFromCart_Implementation(const int32 X, const int32 Y) override;

	virtual void SetupFieldObserver_Implementation(AWaveCollapseGen* WaveCollapseGen) override;
	virtual bool GetCurrentOptimalLocation_Implementation(int32& X, int32& Y) override;
	virtual void GetFieldState_Implementation(TArray<FCellState>& Field) override;
	virtual bool GetCell_Implementation(int32 X, int32 Y, FCellState& CellState) override;
	virtual bool SetCell_Implementation(int32 X, int32 Y, const FCellState& NewCellState) override;

	virtual int32 GetFieldWidth_Implementation() override;
	virtual int32 GetFieldDepth_Implementation() override;

	virtual bool GetTopNeighbour_Implementation(int32 X, int32 Y, FCellState& TopNeighbour) override;
	virtual bool GetRightNeighbour_Implementation(int32 X, int32 Y, FCellState& RightNeighbour) override;
	virtual bool GetBottomNeighbour_Implementation(int32 X, int32 Y, FCellState& BottomNeighbour) override;
	virtual bool GetLeftNeighbour_Implementation(int32 X, int32 Y, FCellState& LeftNeighbour) override;

	virtual bool GetColumn_Implementation(int32 ColumnIndex, TArray<FCellState>& Column) override;
	virtual bool SetColumn_Implementation(int32 ColumnIndex, const TArray<FCellState>& NewColumn) override;

#pragma endregion // FIELD_OBSERVER_IMPLEMENTATIONS
};