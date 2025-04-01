// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameMode.h"

#include "Character/Base/BaseCharacter.h"
#include "Character/Base/CharacterController.h"

ATestGameMode::ATestGameMode()
{
	DefaultPawnClass=ABaseCharacter::StaticClass();
	PlayerControllerClass=ACharacterController::StaticClass();
}
