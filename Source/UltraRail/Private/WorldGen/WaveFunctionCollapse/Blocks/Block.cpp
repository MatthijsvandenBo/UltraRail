#include "UltraRail/Public/WorldGen/WaveFunctionCollapse/Blocks/Block.h"

#include "Engine/StaticMeshActor.h"


// Sets default values
ABlock::ABlock()
{
	PrimaryActorTick.bCanEverTick = true;

	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>("BlockMesh");
	RootComponent = BlockMesh;
	
	// blocks are stationary for the ground, no physics or gravity
	BlockMesh->SetSimulatePhysics(false);
	BlockMesh->SetEnableGravity(false);
	BlockMesh->SetMobility(EComponentMobility::Stationary);
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
	if (!bAutoDestroyBlueprint)
		return;
	
	ReplaceSelf();
}

void ABlock::ReplaceSelf() noexcept
{
	auto World = BlockMesh->GetWorld();
	const auto& Transform = BlockMesh->GetComponentTransform();
	const auto MeshActor = Cast<AStaticMeshActor>(
		World->SpawnActor(AStaticMeshActor::StaticClass(), &Transform));
	MeshActor->SetMobility(EComponentMobility::Stationary);
	MeshActor->GetStaticMeshComponent()->SetStaticMesh(BlockMesh->GetStaticMesh());
	Destroy();
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

