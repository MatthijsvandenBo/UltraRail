#include "WorldGen/WaveFunctionCollapse/BasicCellObserver.h"
#include "WorldGen/WaveFunctionCollapse/WaveCollapseGen.h"
#include "WorldGen/WaveFunctionCollapse/Structs/CellState.h"
#include "WorldGen/WaveFunctionCollapse/DataAssets/BiomeBlockIDs.h"

DEFINE_LOG_CATEGORY(LogBasicCellObserver);

#pragma region LOCAL_FUNCTION_DEFINITIONS

static bool UpdateCell(FCellState& TargetCell, const TArray<FBlockIdWeight>& AllowedConnectionFilter);

#pragma endregion // LOCAL_FUNCTION_DEFINITIONS

// Sets default values
ABasicCellObserver::ABasicCellObserver()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABasicCellObserver::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicCellObserver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region CELL_OBSERVER_IMPLEMENTATIONS

void ABasicCellObserver::SetupCellObserver_Implementation(AWaveCollapseGen* WaveCollapseGen)
{
	WaveCollapse = WaveCollapseGen;
}

void ABasicCellObserver::ObserveCell_Implementation(UObject* Observer, const int32 X, const int32 Y)
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
	const auto* RuleSet = WaveCollapse->GetBiomeBlockIDs()->BlockIdConnections.FindByPredicate([&FilterId = CellState.BlockID](const FBlockIdConnection& IdConnection)
	{
		return IdConnection.ID == FilterId;
	});
	
	// Should never be possible, but you can never be too sure
	if (RuleSet == nullptr)
	{
		UE_LOG(LogBasicCellObserver, Error, TEXT("Rule-set on id `%d` could not be found"), CellState.BlockID);
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
	WaveCollapse->OnCellCollapsed(CellState, X, Y);
}

void ABasicCellObserver::GetLastObserved_Implementation(int32& X, int32& Y)
{
	X = LastObserved[0];
	Y = LastObserved[1];
}

#pragma endregion // CELL_OBSERVER_IMPLEMENTATIONS

#pragma region LOCAL_FUNCTION_IMPLEMENTATION

static bool UpdateCell(
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