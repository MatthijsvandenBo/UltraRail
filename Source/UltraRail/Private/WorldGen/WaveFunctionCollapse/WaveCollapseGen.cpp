#include "WorldGen/WaveFunctionCollapse/WaveCollapseGen.h"
#include "WorldGen/WaveFunctionCollapse/BlockStateObserver.h"
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
		!BlockStateObserver->Implements<UBlockStateObserver>() ||
		!FieldObserver->Implements<UFieldObserver>()
	)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
			TEXT("Either one of the observers is invalid"));
		return;
	}

	// setup of the block states array
	auto Width = IFieldObserver::Execute_GetFieldWidth(FieldObserver);
	auto Depth = IFieldObserver::Execute_GetFieldDepth(FieldObserver);
	BlockStates.Init({}, Width * Depth);
	// Normalize the biome block weights
	BiomeBlockIDs->NormalizeWeights();
}

// Called every frame
void AWaveCollapseGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	auto LastObservedX = 0;
	auto LastObservedY = 0;
	GetLastObserved(LastObservedX, LastObservedY);

	TArray<FBlockState> FieldState;
	GetFieldState(FieldState);
	const auto& FieldSize = FieldState.Num();
	int64 BestIndex = -1;
	const int64 BiomeBlockCount = BiomeBlockIDs->BlockIdConnections.Num();
	int64 LowestCollapsePossibilities = BiomeBlockCount;


	for (int64 i = 0; i < FieldSize; i++)
	{
		const auto& IndexedState = FieldState[i];

		if (IndexedState.BlockID != -1)
			continue;

		int64 IndexedCollapsePossibilities = 0;
		int32 X = 0;
		int32 Y = 0;
		TranslateIndexToCart(i, X, Y);
		TArray<FBlockState> Neighbours;
		GetNeighbours(X, Y, Neighbours);

		// Filter out the block states allowed by all neighbours
	}
	

	return false;
}

void AWaveCollapseGen::GetFieldState_Implementation(TArray<FBlockState>& FieldState)
{
	FieldState = BlockStates;
}

bool AWaveCollapseGen::GetItem_Implementation(
	int32 X, int32 Y, FBlockState& BlockState)
{
	TArray<FBlockState> FieldState;
	GetFieldState(FieldState);
	BlockState = FieldState[TranslateIndexFromCart(X, Y)];
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

void AWaveCollapseGen::TranslateIndexToCart_Implementation(int64 Index, int32& X, int32& Y)
{
	X = Index % GetFieldWidth();
	Y = Index / GetFieldDepth();
}

int64 AWaveCollapseGen::TranslateIndexFromCart_Implementation(int32 X, int32 Y)
{
	return Y * GetFieldWidth() + X;
}

bool AWaveCollapseGen::AreNeighbouring_Implementation(
	TScriptInterface<IFieldObserver>& Observer, int32 X1, int32 Y1, int32 X2, int32 Y2)
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

void AWaveCollapseGen::GetNeighbours_Implementation(int32 X, int32 Y, TArray<FBlockState>& Neighbours)
{
	Neighbours = {};
	IFieldObserver::GetNeighbours_Implementation(X, Y, Neighbours);
}

/// Block State Observer Implementations

void AWaveCollapseGen::ObserveBlock_Implementation(TScriptInterface<IFieldObserver>& Observer, int32 X, int32 Y)
{
	IBlockStateObserver::ObserveBlock_Implementation(Observer, X, Y);
}

void AWaveCollapseGen::GetLastObserved_Implementation(int32& X, int32& Y)
{
	X = LastObserved[0];
	Y = LastObserved[1];
}
