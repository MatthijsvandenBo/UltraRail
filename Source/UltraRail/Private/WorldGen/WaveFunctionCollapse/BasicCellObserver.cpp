#include "WorldGen/WaveFunctionCollapse/BasicCellObserver.h"

#include "BiomeAsset/Assets/BiomeAsset.h"
#include "WorldGen/WaveFunctionCollapse/WaveCollapseGen.h"
#include "WorldGen/WaveFunctionCollapse/Interfaces/FieldObserver.h"
#include "WorldGen/WaveFunctionCollapse/Structs/CellState.h"

DEFINE_LOG_CATEGORY(LogBasicCellObserver);

#pragma region LOCAL_FUNCTION_DEFINITIONS

static bool UpdateCell(FCellState& TargetCell, const TMap<int32, double>& AllowedConnectionFilter);

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
	
	// Uses the Rand() function,
	// so init seed with 'FMath::RandInit(seed)'
	auto RandomValue = FMath::RandRange(0., 100.);
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

	// Should never be possible, but you can never be too sure
	if (!WaveCollapse->GetBiomeAsset()->GetRegisteredIDs().Contains(CellState.BlockID))
	{
		UE_LOG(LogBasicCellObserver, Error, TEXT("Rule-set on id `%d` could not be found"), CellState.BlockID);
		return;
	}

	UpdateCellSurrounding_Implementation(Observer, X, Y);

	LastObserved[0] = X;
	LastObserved[1] = Y;
}

void ABasicCellObserver::UpdateCellSurrounding_Implementation(UObject* FieldObserver, const int32 X, const int32 Y)
{
	FCellState CellState;
	if (!IFieldObserver::Execute_GetCell(FieldObserver, X, Y, CellState) && CellState.BlockID == FCellState::Empty_State)
		return;
	
	#define UPDATE_SURROUNDING_CELL(Observer, Asset, NeighbourDir, X, Y, XOffset, YOffset, State) \
		if (IFieldObserver::Execute_Get##NeighbourDir##Neighbour(Observer, X, Y, State) && State.BlockID == FCellState::Empty_State) { \
			UpdateCell(State, (Asset)->Get##NeighbourDir##WeightMapByID(CellState.BlockID)); \
			IFieldObserver::Execute_SetCell(Observer, X + XOffset, Y + YOffset, State); \
		}

	FCellState NeighbourState;
	// Updates the top neighbour
	UPDATE_SURROUNDING_CELL(FieldObserver, WaveCollapse->GetBiomeAsset(),   Top, X, Y, 0, 1, NeighbourState)
	
	// Updates the right neighbour
	UPDATE_SURROUNDING_CELL(FieldObserver, WaveCollapse->GetBiomeAsset(),  Right, X, Y, 1, 0, NeighbourState)
	
	// Updates the bottom neighbour
	UPDATE_SURROUNDING_CELL(FieldObserver, WaveCollapse->GetBiomeAsset(), Bottom, X, Y, 0, -1, NeighbourState)
	
	// Updates the left neighbour
	UPDATE_SURROUNDING_CELL(FieldObserver, WaveCollapse->GetBiomeAsset(),   Left, X, Y, -1, 0, NeighbourState)
}

void ABasicCellObserver::GetLastObserved_Implementation(int32& X, int32& Y)
{
	X = LastObserved[0];
	Y = LastObserved[1];
}

#pragma endregion // CELL_OBSERVER_IMPLEMENTATIONS

#pragma region LOCAL_FUNCTION_IMPLEMENTATION

bool UpdateCell(
	FCellState& TargetCell,
	const TMap<int32, double>& AllowedConnectionFilter
)
{
	if (TargetCell.BlockID != FCellState::Empty_State ||
		AllowedConnectionFilter.IsEmpty())
		return false;

	// Split the TargetCell into references to both the block id and the entropy
	auto& [TargetID, TargetEntropy] = TargetCell;

	// Transform the AllowedConnectionFilter array from [ID, WEIGHT] -> ID
	TArray<int32> AllowedConnectionIDs = {};
	AllowedConnectionIDs.Reserve(AllowedConnectionFilter.Num());
	for (const auto& [ID, _] : AllowedConnectionFilter)
		AllowedConnectionIDs.Add(ID);

	// Filter out the not-allowed connected id's
	auto NewEntropy = TargetEntropy.FilterByPredicate([&AllowedConnectionIDs](const FBlockIdWeight& EntropyEntry)
	{
		return AllowedConnectionIDs.Contains(EntropyEntry.BlockID);
	});

	// Restructure the weight values
	
	for (auto& [ID, Weight] : NewEntropy)
	{
		const auto AddedWeight = AllowedConnectionFilter[ID];
		Weight += AddedWeight;
	}

	// Normalize the weights 
	double TotalWeightValue = 0.;
	for (auto& [_, Weight] : NewEntropy)
		TotalWeightValue += Weight;
	for (auto& [_, Weight] : NewEntropy)
		Weight /= (TotalWeightValue / 100.);

	// Set the new entropy to the cell
	TargetEntropy = NewEntropy;
	return true;
}

#pragma endregion // LOCAL_FUNCTION_IMPLEMENTATIONS
