#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UltraRail/Public/WorldGen/Structs/CollapseSettings.h"
#include "Block.generated.h"


UCLASS(Blueprintable, BlueprintType)
class ULTRARAIL_API ABlock : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Generating",
		meta=(AllowPrivateAccess))
	FCollapseSettings CollapseSettings;

public:
	// Sets default values for this actor's properties
	ABlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
