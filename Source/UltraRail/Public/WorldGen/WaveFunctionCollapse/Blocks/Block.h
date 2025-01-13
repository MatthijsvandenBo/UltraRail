#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Block.generated.h"


UCLASS(Blueprintable, BlueprintType)
class ULTRARAIL_API ABlock : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool bAutoDestroyBlueprint = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> BlockMesh;

public:
	// Sets default values for this actor's properties
	ABlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ReplaceSelf() noexcept;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
