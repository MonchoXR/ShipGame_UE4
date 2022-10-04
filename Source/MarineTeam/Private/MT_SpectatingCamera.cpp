// Fill out your copyright notice in the Description page of Project Settings.


#include "MT_SpectatingCamera.h"

// Sets default values
AMT_SpectatingCamera::AMT_SpectatingCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpectatingCameraComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpectatingCameraComponent"));
	RootComponent = SpectatingCameraComponent;

}

