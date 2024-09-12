// Copyright Epic Games, Inc. All Rights Reserved.

#include "UltraRailPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "UltraRailCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AUltraRailPlayerController::AUltraRailPlayerController()
{
	
}

void AUltraRailPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}