// Fill out your copyright notice in the Description page of Project Settings.


#include "MT_Cannon.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AMT_Cannon::AMT_Cannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	CannonComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mi Cannon"));
	CannonComponent->SetupAttachment(CustomRootComponent);


	OpenAngle = -90.0f;
}

// Called when the game starts or when spawned
void AMT_Cannon::BeginPlay()
{
	Super::BeginPlay();
	
}



// Called every frame
void AMT_Cannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMT_Cannon::CannonRotator()
{
	BP_CannonRotator();
}

void AMT_Cannon::CannonRotatorReverse()
{
	BP_CannonRotatorReverse();
}


