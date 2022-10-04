// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MT_HealthComponent.h"

// Sets default values for this component's properties
UMT_HealthComponent::UMT_HealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100.0f;
	// ...
}


// Called when the game starts
void UMT_HealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	MyOwner = GetOwner();//Como este componente esta atachado al actor, entonces el getowner devolvera al actor

	if (IsValid(MyOwner))
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &UMT_HealthComponent::TakingDamage);

	}

}

void UMT_HealthComponent::TakingDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || bIsDead) //Si esta muerto no volveremos llamar al broadcast
	{
		return; //Retorna y no hace lo de abajo

	}

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.0f, MaxHealth);
	

	if (CurrentHealth == 0.0f)
	{
		bIsDead = true;
	//	OnDeadDelegate.Broadcast(DamageCauser);

	}

	//Para llamar al delegate se usa Broadcast, y dentro de ella viene todos los parametros de la firma. Entonces cuando Taking Damaga reciba daño, la funcion evento OnHealthChangeDelegate se llamara 
	OnHealthChangeDelegate.Broadcast(this, DamagedActor, Damage, DamageType, InstigatedBy, DamageCauser);


	if (bDebug)
	{
		UE_LOG(LogTemp, Log, TEXT("My Healt is: %f"), CurrentHealth);
	}


	
}


