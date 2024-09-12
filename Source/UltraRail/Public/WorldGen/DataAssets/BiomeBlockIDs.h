// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UltraRail/Public/WorldGen/Structs/BlockIdConnection.h"
#include "BiomeBlockIDs.generated.h"


/**
 * 
 */
UCLASS()
class ULTRARAIL_API UBiomeBlockIDs : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FBlockIdConnection> BlockIdConnections;
};
