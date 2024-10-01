#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Interfaces/CellStateObserver.h"
#include "Interfaces/FieldObserver.h"

#include "WaveCollapseGen.generated.h"

class UBiomeBlockIDs;
class ABlock;

DECLARE_LOG_CATEGORY_EXTERN(LogWaveFunctionCollapse, Log, All);

UCLASS(Blueprintable, BlueprintType)
class ULTRARAIL_API AWaveCollapseGen : public AActor
{
	GENERATED_BODY()

	// Exposed Fields 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid",
		meta=(AllowPrivateAccess))
	int32 FieldWidth = 60;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid",
		meta=(AllowPrivateAccess))
	int32 FieldDepth = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid",
		meta=(AllowPrivateAccess))
	float GridSize = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Blocks",
		meta=(AllowPrivateAccess))
	TObjectPtr<UBiomeBlockIDs> BiomeBlockIDs = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generation",
		meta=(AllowPrivateAccess))
	TObjectPtr<AActor> CellStateObserver = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generation",
		meta=(AllowPrivateAccess))
	TObjectPtr<AActor> FieldObserver = nullptr;
	

	// Non-exposed Fields
	
	UPROPERTY(Blueprintable)
	TMap<int32, TSubclassOf<ABlock>> ToBlockLookupMap;
	UPROPERTY(Blueprintable)
	TMap<TSubclassOf<ABlock>, int32> ToIdLookupMap;

public:
	// Sets default values for this actor's properties
	AWaveCollapseGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CollapseField();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void OnCellCollapsed(const FCellState& CellState, int32 X, int32 Y);

	UFUNCTION(BlueprintCallable)
	void CollapseFieldAsync();

	UFUNCTION(BlueprintCallable)
	const int32& GetFieldWidth() const noexcept { return FieldWidth; }
	UFUNCTION(BlueprintCallable)
	const int32& GetFieldDepth() const noexcept { return FieldDepth; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UBiomeBlockIDs* GetBiomeBlockIDs() const noexcept { return BiomeBlockIDs.Get(); };
};

