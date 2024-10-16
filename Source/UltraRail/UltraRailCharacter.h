// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UltraRailCharacter.generated.h"

UCLASS(Blueprintable)
class AUltraRailCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUltraRailCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

private:

};

