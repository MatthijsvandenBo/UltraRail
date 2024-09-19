#include "WorldGen/WaveFunctionCollapse/WaveCollapseGen.h"
#include "..\..\..\Public\WorldGen\WaveFunctionCollapse\CellStateObserver.h"
#include "WorldGen/WaveFunctionCollapse/FieldObserver.h"
#include "WorldGen/WaveFunctionCollapse/DataAssets/BiomeBlockIDs.h"


// Sets default values
AWaveCollapseGen::AWaveCollapseGen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWaveCollapseGen::BeginPlay()
{
	Super::BeginPlay();
	
	if (BlockStateObserver == nullptr || FieldObserver == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
			TEXT("Either one of the observers is a null pointer"));
		return;
	}
	if (
		!BlockStateObserver->Implements<UCellStateObserver>() ||
		!FieldObserver->Implements<UFieldObserver>()
	)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
			TEXT("Either one of the observers is invalid"));
		return;
	}

	// Normalize the id connection weights
	BiomeBlockIDs->NormalizeWeights();
	
	// setup of the block states array
	IFieldObserver::Execute_SetupField(FieldObserver, BiomeBlockIDs);
	for (const auto& [BlockID, Weight] : FieldState[0].Entropy)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
			FString::Printf(TEXT("Id: %d; Weight: %f"), BlockID, Weight));
}

// Called every frame
void AWaveCollapseGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaveCollapseGen::SetupField_Implementation(UBiomeBlockIDs* Data)
{
	const auto RegisteredIds = Data->RegisteredIds();
	TArray<FBlockIdWeight> Weights = {};
	for (const auto& RegisteredID : RegisteredIds)
		Weights.Add({RegisteredID, 1.f / RegisteredIds.Num()});
	
	FieldState.Init({FCellState::Empty_State, Weights}, GridWidth * GridDepth);
}

/// Field Observer Implementations

bool AWaveCollapseGen::GetCurrentOptimalLocation_Implementation(int32& X, int32& Y)
{
	// return the first item 
	if (bIsEmpty)
	{
		X = 0;
		Y = 0;
		bIsEmpty = false;
		return true;
	}

	const auto& FieldSize = FieldState.Num();
	int64 BestIndex = -1;
	const int64 BiomeBlockCount = BiomeBlockIDs->BlockIdConnections.Num();
	int64 LowestEntropy = BiomeBlockCount;

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
	}

	if (BestIndex == -1)
	{
		X = -1;
		Y = -1;
		return false;
	}

	IFieldObserver::Execute_TranslateIndexToCart(FieldObserver, BestIndex, X, Y);
	return true;
}

void AWaveCollapseGen::GetFieldState_Implementation(TArray<FCellState>& Field)
{
	Field = FieldState;
}

bool AWaveCollapseGen::GetCell_Implementation(
	const int32 X, const int32 Y, FCellState& CellState)
{
	CellState = FieldState[IFieldObserver::Execute_TranslateIndexFromCart(FieldObserver, X, Y)];
	return true;
}

bool AWaveCollapseGen::SetCell_Implementation(const int32 X, const int32 Y, const FCellState NewCellState)
{
	const auto Index = IFieldObserver::Execute_TranslateIndexFromCart(FieldObserver, X, Y);
	FieldState[Index] = NewCellState;
	return true;
}

int32 AWaveCollapseGen::GetFieldWidth_Implementation()
{
	return GridWidth;
}

int32 AWaveCollapseGen::GetFieldDepth_Implementation()
{
	return GridDepth;
}

void AWaveCollapseGen::TranslateIndexToCart_Implementation(const int64 Index, int32& X, int32& Y)
{
	X = Index % IFieldObserver::Execute_GetFieldWidth(FieldObserver);
	Y = Index / IFieldObserver::Execute_GetFieldDepth(FieldObserver);
}

int64 AWaveCollapseGen::TranslateIndexFromCart_Implementation(const int32 X, const int32 Y)
{
	return Y * IFieldObserver::Execute_GetFieldWidth(FieldObserver) + X;
}

bool AWaveCollapseGen::AreNeighbouring_Implementation(
	TScriptInterface<IFieldObserver>& Observer, const int32 X1, const int32 Y1, const int32 X2, const int32 Y2)
{
	const auto Width = Observer->GetFieldWidth();
	const auto Depth = Observer->GetFieldDepth();
	
	if (X1 > Width || X1 < 0 ||
		X2 > Width || X2 < 0 ||
		Y1 > Depth || Y1 < 0 ||
		Y2 > Depth || Y2 < 0)
		return false;
		
	return abs(X2 - X1) ^ abs(Y2 - Y1);
}

void AWaveCollapseGen::GetNeighbours_Implementation(const int32 X, const int32 Y, TArray<FIntVector>& Neighbours)
{
	Neighbours = {};
	const auto Width = IFieldObserver::Execute_GetFieldWidth(FieldObserver);
	const auto Depth = IFieldObserver::Execute_GetFieldDepth(FieldObserver);

	// Top
	if (Y < Depth)
		Neighbours.Add({X, Y + 1, 1});

	// Right
	if (X < Width)
		Neighbours.Add({X + 1, Y, 1});

	// Bottom
	if (Y > 0)
		Neighbours.Add({X, Y - 1, 1});

	// Left
	if (X > 0)
		Neighbours.Add({X - 1, Y, 1});
}

bool AWaveCollapseGen::GetTopNeighbour_Implementation(const int32 X, const int32 Y, FCellState& TopNeighbour)
{
	const auto Depth = IFieldObserver::Execute_GetFieldDepth(FieldObserver);
	if (Y >= Depth)
		return false;

	return IFieldObserver::Execute_GetCell(FieldObserver, X, Y + 1, TopNeighbour);
}

bool AWaveCollapseGen::GetRightNeighbour_Implementation(int32 X, int32 Y, FCellState& RightNeighbour)
{
	const auto Width = IFieldObserver::Execute_GetFieldWidth(FieldObserver);
	if (X >= Width)
		return false;

	return IFieldObserver::Execute_GetCell(FieldObserver, X + 1, Y, RightNeighbour);
}

bool AWaveCollapseGen::GetBottomNeighbour_Implementation(int32 X, int32 Y, FCellState& BottomNeighbour)
{
	if (Y <= 0)
		return false;

	return IFieldObserver::Execute_GetCell(FieldObserver, X, Y - 1, BottomNeighbour);
}

bool AWaveCollapseGen::GetLeftNeighbour_Implementation(int32 X, int32 Y, FCellState& LeftNeighbour)
{
	if (X <= 0)
		return false;

	return IFieldObserver::Execute_GetCell(FieldObserver, X - 1, Y, LeftNeighbour);
}

/// Block State Observer Implementations

void AWaveCollapseGen::ObserveCell_Implementation(TScriptInterface<IFieldObserver>& Observer, const int32 X, const int32 Y)
{
	ICellStateObserver::ObserveCell_Implementation(Observer, X, Y);
	FCellState CellState;
	if (!Observer->GetCell(X, Y, CellState))
		return;

	// Update the state
	const auto ChosenCollapseValue = FMath::RandRange(0, CellState.Entropy.Num() - 1);
	CellState.BlockID = CellState.Entropy[ChosenCollapseValue].BlockID;
	CellState.Entropy.Empty();

	// Insert the state
	Observer->SetCell(X, Y, CellState);

	// Top Cell
	FCellState TopNeighbourState;
	if (Observer->GetTopNeighbour(X, Y, TopNeighbourState) && TopNeighbourState.BlockID != FCellState::Empty_State)
	{
		// update the Top neighbour
		// TODO: Update Top Neighbour
	}

	// Right Cell
	FCellState RightNeighbourState;
	if (Observer->GetTopNeighbour(X, Y, RightNeighbourState) && RightNeighbourState.BlockID != FCellState::Empty_State)
	{
		// update the Right neighbour
		// TODO: Update Right Neighbour
	}

	// Bottom Cell
	FCellState BottomNeighbourState;
	if (Observer->GetTopNeighbour(X, Y, BottomNeighbourState) && BottomNeighbourState.BlockID != FCellState::Empty_State)
	{
		// update the Bottom neighbour
		// TODO: Update Bottom Neighbour
	}

	// Left Cell
	FCellState LeftNeighbourState;
	if (Observer->GetTopNeighbour(X, Y, LeftNeighbourState) && LeftNeighbourState.BlockID != FCellState::Empty_State)
	{
		// update the Left neighbour
		// TODO: Update Left Neighbour
	}
	LastObserved[0] = X;
	LastObserved[1] = Y;
	
}

void AWaveCollapseGen::GetLastObserved_Implementation(int32& X, int32& Y)
{
	X = LastObserved[0];
	Y = LastObserved[1];
}
