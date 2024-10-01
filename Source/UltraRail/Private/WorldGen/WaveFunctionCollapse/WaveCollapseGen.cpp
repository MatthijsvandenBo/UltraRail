#include "WorldGen/WaveFunctionCollapse/WaveCollapseGen.h"

#include "WorldGen/WaveFunctionCollapse/Interfaces/CellStateObserver.h"
#include "WorldGen/WaveFunctionCollapse/Interfaces/FieldObserver.h"
#include "WorldGen/WaveFunctionCollapse/DataAssets/BiomeBlockIDs.h"
#include "WorldGen/WaveFunctionCollapse/Blocks/Block.h"

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

	StartFieldWidth = FieldWidth;
	
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
		UE_LOG(LogWaveFunctionCollapse, Error, TEXT("Either one of the observers is invalid"))
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

	GenerateStartChunk();

	// Just a test with extra chunk generation
	FTimerHandle UnusedHandle;
	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AWaveCollapseGen::GenerateNextChunk, 10.f, false);
}

void AWaveCollapseGen::CollapseField()
{
	int32 OptX = 0;
	int32 OptY = 0;

	while (IFieldObserver::Execute_GetCurrentOptimalLocation(FieldObserver, OptX, OptY))
		ICellStateObserver::Execute_ObserveCell(CellStateObserver, FieldObserver, OptX, OptY);

	UE_LOG(LogWaveFunctionCollapse, Log, TEXT("Field is collapsed"))
}

void AWaveCollapseGen::SetupInterfaces()
{
	IFieldObserver::Execute_SetupFieldObserver(FieldObserver, this);
	ICellStateObserver::Execute_SetupCellObserver(CellStateObserver, this);
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

		const FVector TwoDCoordinate = {Y * GridSize, (X + GenerateOffset) * GridSize, 0};
		
		GetWorld()->SpawnActor(
    		SpawnedClass->Get(),
    		&TwoDCoordinate
    	);
	});
}

void AWaveCollapseGen::CollapseFieldAsync()
{
	AsyncTask(ENamedThreads::Type::BackgroundThreadPriority, [this]
	{
		CollapseField();	
	});
}

void AWaveCollapseGen::GenerateStartChunk()
{
	FieldWidth = StartFieldWidth;
	SetupInterfaces();
	CollapseFieldAsync();
}

void AWaveCollapseGen::GenerateNextChunk()
{
	// Retrieve the last column of the previous generated chunk
	TArray<FCellState> OldLastColumn;
	IFieldObserver::Execute_GetColumn(FieldObserver, IFieldObserver::Execute_GetFieldWidth(FieldObserver) - 1, OldLastColumn);

	// Setup the offsets and the correction in the field-width as
	// the first column is used as a reference and not to be generated
	GenerateOffset = GetGenerationFieldWidth() - 1;
	FieldWidth = GetExtraChunkGenerationFieldWidth();

	// Setup the interfaces
	SetupInterfaces();

	// Revert the changes in the width
	FieldWidth = GetExtraChunkGenerationFieldWidth() - 1;

	// Update the first column in the observer
	IFieldObserver::Execute_SetColumn(FieldObserver, 0, OldLastColumn);

	// Collapse the field async
	CollapseFieldAsync();
}
