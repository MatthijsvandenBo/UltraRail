#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/CellState.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Grid",
		meta=(AllowPrivateAccess))
	int32 GenerateOffset = 0;

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
	UPROPERTY()
	int32 StartFieldWidth = 0;

public:
	// Sets default values for this actor's properties
	AWaveCollapseGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CollapseField();

	UFUNCTION(BlueprintCallable)
	void SetupInterfaces();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override {};

	UFUNCTION(BlueprintCallable)
	void CollapseFieldAsync();

	UFUNCTION(BlueprintCallable)
	void GenerateStartChunk();
	
	UFUNCTION(BlueprintCallable)
	void GenerateNextChunk();

	UFUNCTION(BlueprintCallable)
	const int32& GetGenerationFieldWidth() const noexcept { return FieldWidth; }
	UFUNCTION(BlueprintCallable)
	const int32& GetGenerationFieldDepth() const noexcept { return FieldDepth; }
	UFUNCTION(BlueprintCallable)
	int32 GetExtraChunkGenerationFieldWidth() const noexcept { return StartFieldWidth + 1; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UBiomeBlockIDs* GetBiomeBlockIDs() const noexcept { return BiomeBlockIDs.Get(); };

private:
	UFUNCTION()
	void ResolveField(const TArray<FCellState>& FieldState) const noexcept;
};

