#include "UltraRail/Public/WorldGen/WaveFunctionCollapse/Blocks/Block.h"


// Sets default values
ABlock::ABlock()
{
	PrimaryActorTick.bCanEverTick = true;

	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>("BlockMesh");
	
	// blocks are stationary for the ground, no physics or gravity
	BlockMesh->SetSimulatePhysics(false);
	BlockMesh->SetEnableGravity(false);
}

// Called when the game starts or when spawned
void ABlock::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

