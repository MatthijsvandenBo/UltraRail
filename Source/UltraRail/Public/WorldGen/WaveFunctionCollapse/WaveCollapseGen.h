#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Structs/CellState.h"
#include "WaveCollapseGen.generated.h"

class ABlock;
class UBiomeAsset;

DECLARE_LOG_CATEGORY_EXTERN(LogWaveFunctionCollapse, Log, All);

UCLASS(Blueprintable, BlueprintType)
class ULTRARAIL_API AWaveCollapseGen : public AActor
{
	GENERATED_BODY()

	// Events
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFieldCollapsedDelegate,
		bool, Success,
		bool, WasStartingChunk);

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
	TObjectPtr<UBiomeAsset> BiomeAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generation",
		meta=(AllowPrivateAccess))
	TObjectPtr<AActor> CellStateObserver = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generation",
		meta=(AllowPrivateAccess))
	TObjectPtr<AActor> FieldObserver = nullptr;

	// Non-exposed Fields
	
	UPROPERTY(Blueprintable)
	TMap<int32, TSubclassOf<AActor>> ToBlockLookupMap;
	UPROPERTY(Blueprintable)
	TMap<TSubclassOf<AActor>, int32> ToIdLookupMap;
	
	UPROPERTY()
	TArray<FCellState> LastGeneratedColumn;

	UPROPERTY()
	bool bIsBusy = false;

public:
	// Sets default values for this actor's properties
	AWaveCollapseGen();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CollapseField();

	UFUNCTION(BlueprintCallable)
	void SetupInterfaces(int Width);

	UFUNCTION(BlueprintCallable)
	void CollapseFieldAsync(bool StartingChunk);

	void SetupLookupMaps() noexcept;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override {};

	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateStartChunk();
	
	UFUNCTION(BlueprintCallable, CallInEditor)
	void GenerateNextChunk();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const int32& GetGenerationFieldWidth() const noexcept { return FieldWidth; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const int32& GetGenerationFieldDepth() const noexcept { return FieldDepth; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const int32& GetGenerationOffset() const noexcept { return GenerateOffset; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	const float& GetGenerationGridSize() const noexcept { return GridSize; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UBiomeAsset* GetBiomeAsset() const noexcept { return BiomeAsset.Get(); }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool SetBiomeAsset(UBiomeAsset* NewBiomeAsset)
	{
		if (bIsBusy)
			return false;

		BiomeAsset = NewBiomeAsset;
		SetupLookupMaps();
		return true;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool SetFieldWidth(int32 NewFieldWidth)
	{
		if (bIsBusy)
			return false;

		FieldWidth = NewFieldWidth;
		return true;
	}

	UPROPERTY(BlueprintAssignable)
	FOnFieldCollapsedDelegate OnFieldCollapsed;

private:
	UFUNCTION()
	void ResolveField(const TArray<FCellState>& FieldState, bool FirstIsDummy = false) const noexcept;
};

