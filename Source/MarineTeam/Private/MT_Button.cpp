
// Fill out your copyright notice in the Description page of Project Settings.


#include "MT_Button.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "BaseShip.h"


// Sets default values
AMT_Button::AMT_Button()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	CustomRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CustomRoot"));
	RootComponent = CustomRootComponent;

	//DoorFrameComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrame"));
	//DoorFrameComponent->SetupAttachment(CustomRootComponent);

	ButtonComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Button"));
	ButtonComponent->SetupAttachment(CustomRootComponent);

	KeyZoneColliderComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("KeyZoneCollider"));
	KeyZoneColliderComponent->SetupAttachment(CustomRootComponent);
	KeyZoneColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	KeyZoneColliderComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	KeyZoneColliderComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);



	KeyTag = "KeyA";
	
}

// Called when the game starts or when spawned
void AMT_Button::BeginPlay()
{
	Super::BeginPlay();

	KeyZoneColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &AMT_Button::CheckKeyFromPlayer);
	KeyZoneColliderComponent->OnComponentEndOverlap.AddDynamic(this, &AMT_Button::CheckKeyEndFromPlayer);



	//DynamicMaterialAct = ButtonComponent->CreateDynamicMaterialInstance(0, StoredMaterialAct);
	//DynamicMaterialInstDes = ButtonComponent->CreateDynamicMaterialInstance(0, StoredMaterialDes);
	DynamicMaterialAct = UMaterialInstanceDynamic::Create(StoredMaterialAct, ButtonComponent);
	DynamicMaterialInstDes = UMaterialInstanceDynamic::Create(StoredMaterialDes, ButtonComponent);
	
	
}

void AMT_Button::CheckKeyFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if (bIsTouch)
	//{
	//	return; //Si esta abiera la puerta retornar y ya no hace lo de abajo
	//}

		if (IsValid(OtherActor)) // Validar que el apuntador del actor no sea nulo
		{
			ABaseShip* OverlappedActor = Cast<ABaseShip>(OtherActor);
	
			if (IsValid(OverlappedActor))
			{

				if (OverlappedActor->hasKey(KeyTag))
				{
					if (IsValid(DynamicMaterialAct)) {
						/*ButtonComponent->SetMaterial(0, DynamicMaterialAct);*/

						ButtonComponent->CreateDynamicMaterialInstance(0,StoredMaterialAct);
						if (IsValid(ButtonComponent)) 
						{
							ActivaButton();
						}
					}
				}


			}

		}

}

void AMT_Button::CheckKeyEndFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{


	if (IsValid(OtherActor))
	{
		ABaseShip* OverlappedActor = Cast<ABaseShip>(OtherActor);
		if (IsValid(OverlappedActor))
		{

			if (OverlappedActor->hasKey(KeyTag))
			{
				if (IsValid(DynamicMaterialInstDes)) {
					/*ButtonComponent->SetMaterial(0, DynamicMaterialInstDes);*/
					ButtonComponent->CreateDynamicMaterialInstance(0, StoredMaterialDes);
					DesactivaButton();
				}
			}


		}

	}




}


// Called every frame
void AMT_Button::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMT_Button::ActivaButton()
{
	

	
	BP_ActivaButton();
	/*bIsTouch = false;*/
}

void AMT_Button::DesactivaButton()
{


	BP_DesactivaButton();
	//bIsTouch = true;
}