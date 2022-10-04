// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MT_Weapon.h"

// Sets default values
AMT_Weapon::AMT_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Damage = 20.0f;
}

// Called when the game starts or when spawned
void AMT_Weapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMT_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMT_Weapon::StartAction()
{
	BP_StartAction();
}

void AMT_Weapon::StartActionWithCam()
{
}

void AMT_Weapon::StopAction()
{
	BP_StopAction();
}

