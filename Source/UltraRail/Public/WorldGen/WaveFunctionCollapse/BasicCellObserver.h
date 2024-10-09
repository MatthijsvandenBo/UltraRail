#pragma once

#include "CoreMinimal.h"
#include "Interfaces/CellStateObserver.h"
#include "BasicCellObserver.generated.h"

class AWaveCollapseGen;

DECLARE_LOG_CATEGORY_EXTERN(LogBasicCellObserver, Log, All);

UCLASS(Blueprintable)
class ULTRARAIL_API ABasicCellObserver : public AActor, public ICellStateObserver
{
	GENERATED_BODY()

	UPROPERTY()
	FIntVector2 LastObserved = {0, 0};

	UPROPERTY()
	AWaveCollapseGen* WaveCollapse = nullptr;

public:
	// Sets default values for this actor's properties
	ABasicCellObserver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region CELL_OBSERVER_IMPLEMENTATION

	virtual void SetupCellObserver_Implementation(AWaveCollapseGen* WaveCollapseGen) override;
	virtual void ObserveCell_Implementation(UObject* Observer, int32 X, int32 Y) override;
	virtual void GetLastObserved_Implementation(int32& X, int32& Y) override;

#pragma endregion // CELL_OBSERVER_IMPLEMENTATION
};
