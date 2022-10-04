// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MT_ButtonKey.h"
#include "Components/StaticMeshComponent.h"
#include "BaseShip.h" //Si bien el padre tiene la referencua del character pero hacemos include porque no sabe que usaremos sus funciones

AMT_ButtonKey::AMT_ButtonKey()
{

	KeyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMeshComponenet"));
	KeyMeshComponent->SetupAttachment(RootComponent);
	KeyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);//No queremos colisionar porque lo hace el MainCollider

	KeyTag = "keyA";


}

void AMT_ButtonKey::Pickup(ABaseShip* PickupActor)
{
	Super::Pickup(PickupActor); //Llama  y va hacer lo que hace el padre de esta funcion 



	PickupActor->AddKey(KeyTag);
	Destroy();

}
