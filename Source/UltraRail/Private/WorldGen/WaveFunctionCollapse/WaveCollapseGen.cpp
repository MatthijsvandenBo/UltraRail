#include "WorldGen/WaveFunctionCollapse/WaveCollapseGen.h"
#include "WorldGen/WaveFunctionCollapse/Interfaces/CellStateObserver.h"
#include "WorldGen/WaveFunctionCollapse/Interfaces/FieldObserver.h"
#include "WorldGen/WaveFunctionCollapse/DataAssets/BiomeBlockIDs.h"

#pragma region LOCAL_FUNCTION_DEFINITIONS

bool UpdateCell(FCellState& TargetCell, const TArray<FBlockIdWeight>& AllowedConnectionFilter);

#pragma endregion // LOCAL_FUNCTION_DEFINITIONS

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

	
	// test run
	int32 OptX = 0;
	int32 OptY = 0;
	IFieldObserver::Execute_GetCurrentOptimalLocation(FieldObserver, OptX, OptY);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		FString::Printf(TEXT("[%d, %d]"), OptX, OptY));
	ICellStateObserver::Execute_ObserveCell(BlockStateObserver, FieldObserver, OptX, OptY);
	
	FCellState TargetCell;
	IFieldObserver::Execute_GetCell(FieldObserver, OptX, OptY, TargetCell);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		FString::Printf(TEXT("Target-cell id: %d"), TargetCell.BlockID));
	
	FCellState RightCell;
	IFieldObserver::Execute_GetRightNeighbour(FieldObserver, OptX, OptY, RightCell);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		FString::Printf(TEXT("Entropy: %d; Weight: %f; first-entr: %d"), RightCell.Entropy.Num(), RightCell.Entropy[0].Weight, RightCell.Entropy[0].BlockID));
	
    IFieldObserver::Execute_GetCurrentOptimalLocation(FieldObserver, OptX, OptY);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
        FString::Printf(TEXT("[%d, %d]"), OptX, OptY));
	
    ICellStateObserver::Execute_ObserveCell(BlockStateObserver, FieldObserver, OptX, OptY);
    IFieldObserver::Execute_GetCell(FieldObserver, OptX, OptY, TargetCell);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
        FString::Printf(TEXT("Target-cell id: %d"), TargetCell.BlockID));
    IFieldObserver::Execute_GetRightNeighbour(FieldObserver, OptX, OptY, RightCell);
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
        FString::Printf(TEXT("Entropy: %d; Weight: %f"), RightCell.Entropy.Num(), RightCell.Entropy[0].Weight));
}

// Called every frame
void AWaveCollapseGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/// Field Observer Implementations
#pragma region FIELD_OBSERVER_IMPLEMENTATIONS

void AWaveCollapseGen::SetupField_Implementation(UBiomeBlockIDs* Data)
{
	const auto RegisteredIds = Data->RegisteredIds();
	TArray<FBlockIdWeight> Weights = {};
	for (const auto& RegisteredID : RegisteredIds)
		Weights.Add({RegisteredID, 1.f / RegisteredIds.Num()});
	
	FieldState.Init({FCellState::Empty_State, Weights}, GridWidth * GridDepth);
}

bool AWaveCollapseGen::GetCurrentOptimalLocation_Implementation(int32& X, int32& Y)
{
	const auto& FieldSize = FieldState.Num();
	int64 BestIndex = -1;
	int64 LowestEntropy = BiomeBlockIDs->BlockIdConnections.Num() + 1;

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
	const auto Width = IFieldObserver::Execute_GetFieldWidth(FieldObserver);
	const auto Depth = IFieldObserver::Execute_GetFieldDepth(FieldObserver);

	if (X < 0 || X >= Width || Y < 0 || Y >= Depth)
		return false;
	
	CellState = FieldState[IFieldObserver::Execute_TranslateIndexFromCart(FieldObserver, X, Y)];
	return true;
}

bool AWaveCollapseGen::SetCell_Implementation(const int32 X, const int32 Y, const FCellState NewCellState)
{
	const auto Width = IFieldObserver::Execute_GetFieldWidth(FieldObserver);
	const auto Depth = IFieldObserver::Execute_GetFieldDepth(FieldObserver);

	if (X < 0 || X >= Width || Y < 0 || Y >= Depth)
		return false;
	
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

#pragma endregion // FIELD_OBSERVER_IMPLEMENTATIONS

/// Block State Observer Implementations
#pragma region CELL_OBSERVER_IMPLEMENTATIONS

void AWaveCollapseGen::ObserveCell_Implementation(UObject* Observer, const int32 X, const int32 Y)
{
	FCellState CellState;
	if (!IFieldObserver::Execute_GetCell(Observer, X, Y, CellState))
		return;

	// Update the state
	const auto ChosenCollapseValue = FMath::RandRange(0, CellState.Entropy.Num() - 1);
	CellState.BlockID = CellState.Entropy[ChosenCollapseValue].BlockID;
	CellState.Entropy.Empty();

	// Insert the state
	IFieldObserver::Execute_SetCell(Observer, X, Y, CellState);

	// Retrieve the rule-set of the collapsed id
	const auto* RuleSet = BiomeBlockIDs->BlockIdConnections.FindByPredicate([&FilterId = CellState.BlockID](const FBlockIdConnection& IdConnection)
	{
		return IdConnection.ID == FilterId;
	});
	
	// Should never be possible, but you can never be too sure
	if (RuleSet == nullptr)
	{
		// TODO: Log out a warning!
	}

	FCellState NeighbourState;
	
	// Top Cell
	if (IFieldObserver::Execute_GetTopNeighbour(Observer, X, Y, NeighbourState) &&
		NeighbourState.BlockID == FCellState::Empty_State)
	{
		// update the Top neighbour
		UpdateCell(NeighbourState, RuleSet->CollapseSettings.AllowedTopIDs);
		IFieldObserver::Execute_SetCell(Observer, X, Y + 1, NeighbourState);
	}

	// Right Cell
	if (IFieldObserver::Execute_GetRightNeighbour(Observer, X, Y, NeighbourState) &&
		NeighbourState.BlockID == FCellState::Empty_State)
	{
		// update the Right neighbour
		UpdateCell(NeighbourState, RuleSet->CollapseSettings.AllowedRightIDs);
		IFieldObserver::Execute_SetCell(Observer, X + 1, Y, NeighbourState);
	}

	// Bottom Cell
	if (IFieldObserver::Execute_GetBottomNeighbour(Observer, X, Y, NeighbourState) &&
		NeighbourState.BlockID == FCellState::Empty_State)
	{
		// update the Bottom neighbour
		UpdateCell(NeighbourState, RuleSet->CollapseSettings.AllowedBottomIDs);
		IFieldObserver::Execute_SetCell(Observer, X, Y - 1, NeighbourState);
	}

	// Left Cell
	if (IFieldObserver::Execute_GetLeftNeighbour(Observer, X, Y, NeighbourState) &&
		NeighbourState.BlockID == FCellState::Empty_State)
	{
		// update the Left neighbour
		UpdateCell(NeighbourState, RuleSet->CollapseSettings.AllowedLeftIDs);
		IFieldObserver::Execute_SetCell(Observer, X - 1, Y, NeighbourState);
	}
	
	LastObserved[0] = X;
	LastObserved[1] = Y;
}

void AWaveCollapseGen::GetLastObserved_Implementation(int32& X, int32& Y)
{
	X = LastObserved[0];
	Y = LastObserved[1];
}

#pragma endregion // CELL_OBSERVER_IMPLEMENTATIONS

#pragma region LOCAL_FUNCTION_IMPLEMENTATION

bool UpdateCell(
	FCellState& TargetCell,
	const TArray<FBlockIdWeight>& AllowedConnectionFilter)
{
	if (TargetCell.BlockID != FCellState::Empty_State ||
		AllowedConnectionFilter.IsEmpty())
		return false;

	// Split the TargetCell into references to both the block id and the entropy
	auto& [TargetID, TargetEntropy] = TargetCell;

	// Transform the AllowedConnectionFilter array from [ID, WEIGHT] -> ID
	TArray<int32> AllowedConnectionIDs = {};
	AllowedConnectionIDs.Init(0, AllowedConnectionFilter.Num());
	for (int64 i = 0; i < AllowedConnectionFilter.Num(); i++)
		AllowedConnectionIDs[i] = AllowedConnectionFilter[i].BlockID;

	// Filter out the not-allowed connected id's
	auto NewEntropy = TargetEntropy.FilterByPredicate([&AllowedConnectionIDs](const FBlockIdWeight& EntropyEntry)
	{
		return AllowedConnectionIDs.Contains(EntropyEntry.BlockID);
	});

	// Restructure the weight values
	
	for (auto& [ID, Weight] : NewEntropy)
	{
		const auto AddedWeight = AllowedConnectionFilter.FindByPredicate([&ID](const FBlockIdWeight& EntropyEntry)
		{
			return EntropyEntry.BlockID == ID;
		})->Weight;

		Weight += AddedWeight;
	}

	// Normalize the weights 
	float TotalWeightValue = 0.f;
	for (auto& [_, Weight] : NewEntropy)
		TotalWeightValue += Weight;
	for (auto& [_, Weight] : NewEntropy)
		Weight /= TotalWeightValue;

	// Set the new entropy to the cell
	TargetEntropy = NewEntropy;
	return true;
}

#pragma endregion // LOCAL_FUNCTION_IMPLEMENTATIONS