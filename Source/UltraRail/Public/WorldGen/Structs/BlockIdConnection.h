// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockIdConnection.generated.h"

class ABlock;

/**
 * This struct is used by the BiomeBlockIDs data asset to determine the bound id to a given block
 */
USTRUCT(Blueprintable)
struct ULTRARAIL_API FBlockIdConnection
{
 GENERATED_BODY()

 UPROPERTY(EditAnywhere, BlueprintReadOnly)
 TSubclassOf<ABlock> Block;

 UPROPERTY(EditAnywhere, BlueprintReadOnly)
 int32 ID;
};
