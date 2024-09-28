#include "WorldGen/WaveFunctionCollapse/BasicFieldObserver.h"
#include "WorldGen/WaveFunctionCollapse/WaveCollapseGen.h"
#include "WorldGen/WaveFunctionCollapse/DataAssets/BiomeBlockIDs.h"

// Sets default values
ABasicFieldObserver::ABasicFieldObserver()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABasicFieldObserver::BeginPlay()
{
	Super::BeginPlay();
}

void ABasicFieldObserver::TranslateIndexToCart(const int64 Index, int32& X, int32& Y) const noexcept
{
	X = Index % FieldWidth;
	Y = Index / FieldWidth;
}

int64 ABasicFieldObserver::TranslateIndexFromCart(const int32 X, const int32 Y) const noexcept
{
	return Y * FieldWidth + X;
}

// Called every frame
void ABasicFieldObserver::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region FIELD_OBSERVER_IMPLEMENTATIONS

void ABasicFieldObserver::SetupFieldObserver_Implementation(AWaveCollapseGen* WaveCollapseGen)
{
	const auto BiomeBlocks = WaveCollapseGen->GetBiomeBlockIDs();
	BiomeBlockCount = BiomeBlocks->BlockIdConnections.Num();
	FieldWidth = WaveCollapseGen->GetFieldWidth();
	FieldDepth = WaveCollapseGen->GetFieldDepth();
	
	const auto RegisteredIds = BiomeBlocks->RegisteredIds();
	TArray<FBlockIdWeight> Weights = {};
	for (const auto& RegisteredID : RegisteredIds)
		Weights.Add({RegisteredID, 1.f / RegisteredIds.Num()});
	
	FieldState.Init({FCellState::Empty_State, Weights}, FieldWidth * FieldDepth);
}

bool ABasicFieldObserver::GetCurrentOptimalLocation_Implementation(int32& X, int32& Y)
{
	const auto& FieldSize = FieldState.Num();
	int64 BestIndex = -1;
	int64 LowestEntropy = BiomeBlockCount + 1;

	for (int64 i = 0; i < FieldSize; i++)
	{
		const auto& [BlockID, Entropy] = FieldState[i];
		if (BlockID != FCellState::Empty_State)
			continue;

		const auto& IndexedEntropy = Entropy.Num();
		if (IndexedEntropy < LowestEntropy)
		{
			LowestEntropy = IndexedEntropy;
			BestIndex = i;
		}

		// When the entropy is 1. We can immediately break out the loop as there can not be a lower entropy
		if (LowestEntropy == 1)
			break;
	}

	if (BestIndex == -1)
	{
		X = -1;
		Y = -1;
		return false;
	}

	TranslateIndexToCart(BestIndex, X, Y);
	return true;
}

void ABasicFieldObserver::GetFieldState_Implementation(TArray<FCellState>& Field)
{
	Field = FieldState;
}

bool ABasicFieldObserver::GetCell_Implementation(
	const int32 X, const int32 Y, FCellState& CellState)
{
	if (X < 0 || X >= FieldWidth || Y < 0 || Y >= FieldDepth)
		return false;
	
	CellState = FieldState[TranslateIndexFromCart(X, Y)];
	return true;
}

bool ABasicFieldObserver::SetCell_Implementation(const int32 X, const int32 Y, const FCellState NewCellState)
{
	if (X < 0 || X >= FieldWidth || Y < 0 || Y >= FieldDepth)
		return false;
	
	const auto Index = TranslateIndexFromCart(X, Y);
	FieldState[Index] = NewCellState;
	return true;
}

bool ABasicFieldObserver::GetTopNeighbour_Implementation(const int32 X, const int32 Y, FCellState& TopNeighbour)
{
	return GetCell_Implementation(X, Y + 1, TopNeighbour);
}

bool ABasicFieldObserver::GetRightNeighbour_Implementation(int32 X, int32 Y, FCellState& RightNeighbour)
{
	return GetCell_Implementation(X + 1, Y, RightNeighbour);
}

bool ABasicFieldObserver::GetBottomNeighbour_Implementation(int32 X, int32 Y, FCellState& BottomNeighbour)
{
	return GetCell_Implementation(X, Y - 1, BottomNeighbour);
}

bool ABasicFieldObserver::GetLeftNeighbour_Implementation(int32 X, int32 Y, FCellState& LeftNeighbour)
{
	return GetCell_Implementation(X - 1, Y , LeftNeighbour);
}

#pragma endregion // FIELD_OBSERVER_IMPLEMENTATIONS