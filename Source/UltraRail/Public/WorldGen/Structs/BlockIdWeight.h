// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlockIdWeight.generated.h"

/**
 * 
 */
USTRUCT(Blueprintable)
struct ULTRARAIL_API FBlockIdWeight
{
 GENERATED_BODY()
public:
 UPROPERTY(EditAnywhere, BlueprintReadOnly)
 int32 BlockID = 0;
 UPROPERTY(EditAnywhere, BlueprintReadOnly)
 float Weight = 1;
};
