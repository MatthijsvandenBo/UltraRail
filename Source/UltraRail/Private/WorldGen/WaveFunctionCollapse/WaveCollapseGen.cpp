#include "WorldGen/WaveFunctionCollapse/WaveCollapseGen.h"

#include "BiomeAsset/Assets/BiomeAsset.h"
#include "WorldGen/WaveFunctionCollapse/Interfaces/CellStateObserver.h"
#include "WorldGen/WaveFunctionCollapse/Interfaces/FieldObserver.h"

DEFINE_LOG_CATEGORY(LogWaveFunctionCollapse);

// Sets default values
AWaveCollapseGen::AWaveCollapseGen()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AWaveCollapseGen::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(BiomeAsset))
	{
		UE_LOG(LogWaveFunctionCollapse, Error, TEXT("Biome-asset may not be null"))
		return;
	}

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

	// setup the lookup tables
	SetupLookupMaps();
}

void AWaveCollapseGen::CollapseField()
{
	int32 OptX = 0;
	int32 OptY = 0;

	while (IFieldObserver::Execute_GetCurrentOptimalLocation(FieldObserver, OptX, OptY))
		ICellStateObserver::Execute_ObserveCell(CellStateObserver, FieldObserver, OptX, OptY);

	UE_LOG(LogWaveFunctionCollapse, Log, TEXT("Field is collapsed"))
}

void AWaveCollapseGen::SetupInterfaces(const int Width)
{
	if (bIsBusy)
		return;
	
	IFieldObserver::Execute_SetupFieldObserver(FieldObserver, this, Width, FieldDepth);
	ICellStateObserver::Execute_SetupCellObserver(CellStateObserver, this);
}

void AWaveCollapseGen::CollapseFieldAsync(bool StartingChunk)
{
	if (bIsBusy)
	{
		OnFieldCollapsed.Broadcast(false, false);
		return;
	}

	bIsBusy = true;
		
	AsyncTask(ENamedThreads::Type::BackgroundThreadPriority, [this, StartingChunk]
	{
		CollapseField();
		IFieldObserver::Execute_GetColumn(FieldObserver, IFieldObserver::Execute_GetFieldWidth(FieldObserver) - 1, LastGeneratedColumn);

		AsyncTask(ENamedThreads::Type::GameThread, [this, StartingChunk]
		{
			TArray<FCellState> FieldState;
			IFieldObserver::Execute_GetFieldState(FieldObserver, FieldState);
			
			ResolveField(FieldState, !StartingChunk);
			bIsBusy = false;
			GenerateOffset += FieldWidth;
			OnFieldCollapsed.Broadcast(true, StartingChunk);
		});
	});
}

void AWaveCollapseGen::SetupLookupMaps() noexcept
{
	auto Ids = BiomeAsset->GetRegisteredIDs();
	for (const auto BlockID : Ids)
	{
		ToBlockLookupMap.Add(BlockID, BiomeAsset->FindTypeByID(BlockID));
		ToIdLookupMap.Add(BiomeAsset->FindTypeByID(BlockID), BlockID);
	}
}

void AWaveCollapseGen::GenerateStartChunk()
{
	if (bIsBusy || !IsValid(BiomeAsset))
		return;
	
	SetupInterfaces(FieldWidth);
	CollapseFieldAsync(true);
}

void AWaveCollapseGen::GenerateNextChunk()
{
	if (bIsBusy || !IsValid(BiomeAsset))
		return;
	
	// Setup the interfaces
	SetupInterfaces(FieldWidth + 1);

	// Update the first column in the observer
	IFieldObserver::Execute_SetColumn(FieldObserver, 0, LastGeneratedColumn, CellStateObserver);

	// Collapse the field async
	CollapseFieldAsync(false);
}

void AWaveCollapseGen::ResolveField(const TArray<FCellState>& FieldState, const bool FirstIsDummy) const noexcept
{
	const auto FieldSize = FieldState.Num();
	const auto World = GetWorld();
	for (int64 i = 0; i < FieldSize; ++i)
	{
		int32 X = 0;
		int32 Y = 0;
		IFieldObserver::Execute_TranslateIndexToCart(FieldObserver, i, X, Y);

		if (FirstIsDummy && X == 0)
			continue;
		
		// split the cell-state entry into its id and weights (where weights are unused)
		const auto& [BlockID, _] = FieldState[i];
		
		const auto SpawnedClass = ToBlockLookupMap.Find(BlockID);
		if (SpawnedClass == nullptr)
		{
			// Log out that the id was invalid (should not be possible)
			UE_LOG(LogWaveFunctionCollapse, Error, TEXT("Lookup for block id `%d` failed"), BlockID);
			return;
		}

		const FVector TwoDCoordinate = {Y * GridSize, (X + GenerateOffset - FirstIsDummy) * GridSize, ZGenerateOffset};
        		
		World->SpawnActor(
			SpawnedClass->Get(),
			&TwoDCoordinate
		);
	}
}
