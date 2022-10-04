// Fill out your copyright notice in the Description page of Project Settings.


#include "MT_BaseShipDestroyed.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AMT_BaseShipDestroyed::AMT_BaseShipDestroyed()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseShipDestroyedComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My BaseShipDestroyedComponent"));
	RootComponent = BaseShipDestroyedComponent;

	ExplosionEffectInFront = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("My ExplosionEffectComponent"));
	ExplosionEffectInFront->SetupAttachment(BaseShipDestroyedComponent);

	ExplosionEffectInBack = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("My ExplosionEffectBackComponent"));
	ExplosionEffectInBack->SetupAttachment(BaseShipDestroyedComponent);

	SoundExplosion = CreateDefaultSubobject<UAudioComponent>(TEXT("My SoundExplosionComponent"));;
	SoundExplosion->SetupAttachment(BaseShipDestroyedComponent);
}

// Called when the game starts or when spawned
void AMT_BaseShipDestroyed::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMT_BaseShipDestroyed::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMT_BaseShipDestroyed::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

