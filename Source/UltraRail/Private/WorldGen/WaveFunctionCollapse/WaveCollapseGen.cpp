#include "WorldGen/WaveFunctionCollapse/WaveCollapseGen.h"

#include "WorldGen/WaveFunctionCollapse/Interfaces/CellStateObserver.h"
#include "WorldGen/WaveFunctionCollapse/Interfaces/FieldObserver.h"
#include "WorldGen/WaveFunctionCollapse/DataAssets/BiomeBlockIDs.h"
#include "WorldGen/WaveFunctionCollapse/Blocks/Block.h"

#pragma region LOCAL_FUNCTION_DEFINITIONS

bool UpdateCell(FCellState& TargetCell, const TArray<FBlockIdWeight>& AllowedConnectionFilter);

#pragma endregion // LOCAL_FUNCTION_DEFINITIONS

DEFINE_LOG_CATEGORY(LogWaveFunctionCollapse);

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
	
	if (CellStateObserver == nullptr || FieldObserver == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
			TEXT("Either one of the observers is a null pointer"));
		return;
	}
	if (
		!CellStateObserver->Implements<UCellStateObserver>() ||
		!FieldObserver->Implements<UFieldObserver>()
	)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red,
			TEXT("Either one of the observers is invalid"));
		return;
	}

	// Normalize the id connection weights
	BiomeBlockIDs->NormalizeWeights();

	// setup the lookup tables
	for (const auto& [BlockClass, BlockID, _] : BiomeBlockIDs->BlockIdConnections)
	{
		ToBlockLookupMap.Add(BlockID, BlockClass);
		ToIdLookupMap.Add(BlockClass, BlockID);
	}
	
	// setup of the block states array
	IFieldObserver::Execute_SetupField(FieldObserver, BiomeBlockIDs, GridWidth, GridDepth);
	ICellStateObserver::Execute_SetupCellObserver(CellStateObserver, this);
	AsyncTask(ENamedThreads::Type::AnyBackgroundThreadNormalTask, [this]
	{
		CollapseField();
	});
}

// Called every frame
void AWaveCollapseGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaveCollapseGen::OnCellCollapsed(
	const FCellState& CellState, const int32 X, const int32 Y)
{
	AsyncTask(ENamedThreads::Type::GameThread, [this, CellState, X, Y]
	{

		const auto SpawnedClass = ToBlockLookupMap.Find(CellState.BlockID);
		if (SpawnedClass == nullptr)
		{
			// Log out that the id was invalid (should not be possible)
			UE_LOG(LogWaveFunctionCollapse, Error, TEXT("Lookup for block id `%d` failed"), CellState.BlockID);
			return;
		}

		const FVector TwoDCoordinate = {Y * GridSize, X * GridSize, 0};
		
		GetWorld()->SpawnActor(
    		SpawnedClass->Get(),
    		&TwoDCoordinate
    	);	
	});
}

void AWaveCollapseGen::CollapseField()
{
	int32 OptX = 0;
	int32 OptY = 0;

	while (IFieldObserver::Execute_GetCurrentOptimalLocation(FieldObserver, OptX, OptY))
		ICellStateObserver::Execute_ObserveCell(CellStateObserver, FieldObserver, OptX, OptY);	
}

/// Field Observer Implementations
#pragma region FIELD_OBSERVER_IMPLEMENTATIONS

void AWaveCollapseGen::SetupField_Implementation(
	UBiomeBlockIDs* Data, const int32 Width, const int32 Depth)
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

	TranslateIndexToCart(BestIndex, X, Y);
	return true;
}

void AWaveCollapseGen::GetFieldState_Implementation(TArray<FCellState>& Field)
{
	Field = FieldState;
}

bool AWaveCollapseGen::GetCell_Implementation(
	const int32 X, const int32 Y, FCellState& CellState)
{
	if (X < 0 || X >= GridWidth || Y < 0 || Y >= GridDepth)
		return false;
	
	CellState = FieldState[TranslateIndexFromCart(X, Y)];
	return true;
}

bool AWaveCollapseGen::SetCell_Implementation(const int32 X, const int32 Y, const FCellState NewCellState)
{
	if (X < 0 || X >= GridWidth || Y < 0 || Y >= GridDepth)
		return false;
	
	const auto Index = TranslateIndexFromCart(X, Y);
	FieldState[Index] = NewCellState;
	return true;
}

void AWaveCollapseGen::TranslateIndexToCart(const int64 Index, int32& X, int32& Y) const
{
	X = Index % GridWidth;
	Y = Index / GridWidth;
}

int64 AWaveCollapseGen::TranslateIndexFromCart(const int32 X, const int32 Y) const
{
	return Y * GridWidth + X;
}

bool AWaveCollapseGen::GetTopNeighbour_Implementation(const int32 X, const int32 Y, FCellState& TopNeighbour)
{
	return IFieldObserver::Execute_GetCell(FieldObserver, X, Y + 1, TopNeighbour);
}

bool AWaveCollapseGen::GetRightNeighbour_Implementation(int32 X, int32 Y, FCellState& RightNeighbour)
{
	return IFieldObserver::Execute_GetCell(FieldObserver, X + 1, Y, RightNeighbour);
}

bool AWaveCollapseGen::GetBottomNeighbour_Implementation(int32 X, int32 Y, FCellState& BottomNeighbour)
{
	return IFieldObserver::Execute_GetCell(FieldObserver, X, Y - 1, BottomNeighbour);
}

bool AWaveCollapseGen::GetLeftNeighbour_Implementation(int32 X, int32 Y, FCellState& LeftNeighbour)
{
	return IFieldObserver::Execute_GetCell(FieldObserver, X - 1, Y, LeftNeighbour);
}

#pragma endregion // FIELD_OBSERVER_IMPLEMENTATIONS

/// Block State Observer Implementations
#pragma region CELL_OBSERVER_IMPLEMENTATIONS

void AWaveCollapseGen::SetupCellObserver_Implementation(AWaveCollapseGen* WaveCollapseGen)
{
	ICellStateObserver::SetupCellObserver_Implementation(WaveCollapseGen);
}

void AWaveCollapseGen::ObserveCell_Implementation(UObject* Observer, const int32 X, const int32 Y)
{
	FCellState CellState;
	if (!IFieldObserver::Execute_GetCell(Observer, X, Y, CellState) && CellState.BlockID != FCellState::Empty_State)
		return;

	// Update the state
	const auto& CellEntropy = CellState.Entropy.Num();
	auto RandomValue = FMath::RandRange(0.f, 1.f);
	auto ChosenCollapseValue = 0;
	for (int32 i = 0; i < CellEntropy; i++)
	{
		if (RandomValue < CellState.Entropy[i].Weight)
		{
			ChosenCollapseValue = i;
			break;
		}
		RandomValue -= CellState.Entropy[i].Weight;
	}
	
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
		UE_LOG(LogWaveFunctionCollapse, Error, TEXT("Rule-set on id `%d` could not be found"), CellState.BlockID);
		return;
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

	// Notify the wave function collapser that a cell has been collapsed
	OnCellCollapsed(CellState, X, Y);
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