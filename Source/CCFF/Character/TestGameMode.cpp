// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameMode.h"

#include "BaseCharacter.h"
#include "CharacterController.h"

ATestGameMode::ATestGameMode()
{
	DefaultPawnClass=ABaseCharacter::StaticClass();
	PlayerControllerClass=ACharacterController::StaticClass();
}
