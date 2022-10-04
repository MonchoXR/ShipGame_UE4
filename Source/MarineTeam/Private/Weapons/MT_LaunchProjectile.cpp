// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MT_LaunchProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/MT_HealthComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "BaseShip.h"
//#include "C:/Program Files/Epic Games/UE_4.26/Engine/Plugins/Experimental/Water/Source/Runtime/Public/WaterBodyActor.h"
#include "Public/WaterBodyOceanActor.h"
#include "Weapons/MT_LaunchTurret.h"
#include "Components/AudioComponent.h"

// Sets default values
AMT_LaunchProjectile::AMT_LaunchProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	ProjectileCollision->SetSphereRadius(50.0f);
	RootComponent = ProjectileCollision;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(ProjectileCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComponent->InitialSpeed = 4000.0f;
	ProjectileMovementComponent->MaxSpeed = 4000.0f;

	HealthComponent = CreateDefaultSubobject<UMT_HealthComponent>(TEXT("My HealthComponent")); //Como es un componente abstracto no necesita atachar a un root 

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("My SpringArmLauncher"));
	SpringArmComponent->SetupAttachment(ProjectileCollision);
	SpringArmComponent->TargetArmLength = 1000
		;
	SpringArmComponent->TargetOffset.Z=200;

	CameraLauncher = CreateDefaultSubobject<UCameraComponent>(TEXT("My CameraLauncher"));
	CameraLauncher->SetupAttachment(SpringArmComponent);

	AudioSoundFluying = CreateDefaultSubobject<UAudioComponent>(TEXT("My SoundExplosionComponent"));
	AudioSoundFluying->SetupAttachment(ProjectileCollision);

	bIsDestroy = true;
}

// Called when the game starts or when spawned
void AMT_LaunchProjectile::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &AMT_LaunchProjectile::OnHealthChanged);
	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &AMT_LaunchProjectile::CheckOverlapLaunchProjectil);
	if (IsValid(AudioSoundFluying))
	{
		//UE_LOG(LogTemp, Warning, TEXT("suena"));
		AudioSoundFluying->SetActive(true);

	}

}

// Called every frame
void AMT_LaunchProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMT_LaunchProjectile::OnHealthChanged(UMT_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

	//if (HealthComponent->IsDead() && GetProjectileType() == EMT_LauncherProjectileType::CharacterType_Enemy)
	if (HealthComponent->IsDead() )
	{
		bIsDestroy = true;
		Destroy();
		

		if (IsValid(ExplosionEffect))
		{

			//  ABaseShip* BaseShipOwner = Cast<ABaseShip>(CurrentOwner);
				UGameplayStatics::SpawnEmitterAttached(ExplosionEffect, ProjectileMesh);
	

		}
		
	}
}

void AMT_LaunchProjectile::CheckOverlapLaunchProjectil(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	AWaterBodyOcean* MyAWaterBodyOcean = Cast<AWaterBodyOcean>(OtherActor);

	if (IsValid(MyAWaterBodyOcean))
	{

		bIsDestroy = true;
	//	Destroy();

		if (IsValid(ExplosionEffect))
		{
			
			UGameplayStatics::SpawnEmitterAttached(ExplosionEffect, ProjectileMesh);
		}
		ProjectileMesh->SetHiddenInGame(true);
		ProjectileMovementComponent->SetVelocityInLocalSpace(FVector(0.0f,0.0f, 0.0f));
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
		SetLifeSpan(2.0f);
		AudioSoundFluying->SetActive(false);
	}



	ABaseShip* OtherBaseShip = Cast<ABaseShip>(OtherActor);
	

	if (IsValid(OtherBaseShip))
	{

		TArray<UPrimitiveComponent*> ArrayUprimitive;

		OtherBaseShip->GetOverlappingComponents(ArrayUprimitive);

		bool bArrayUprimitive = ArrayUprimitive.Contains(OtherComp);
			//UE_LOG(LogTemp, Warning, TEXT("mi bArrayUprimitive es %s"), bArrayUprimitive ? TEXT("true") : TEXT("false"));
			

		if (ArrayUprimitive.Contains(OtherComp))
		{

			bIsDestroy = true;
			UGameplayStatics::ApplyPointDamage(OtherActor, 50.0f, SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
		//	UE_LOG(LogTemp, Warning, TEXT("Mi Collider que choco es: %s"), *OtherComp->GetName());
			Destroy();
			//return;


			if (IsValid(ExplosionEffect))
			{

				UGameplayStatics::SpawnEmitterAttached(ExplosionEffect, ProjectileMesh);
			}

		}
	}
		AActor* CurrentOwner = GetOwner();


	//if (IsValid(CurrentOwner) && GetProjectileType() == EMT_LauncherProjectileType::CharacterType_Enemy)
	//{
	//	AMT_LaunchTurret* CurrentLaunchTurret = Cast<AMT_LaunchTurret>(CurrentOwner);
	//	if (IsValid(CurrentLaunchTurret))
	//	{

	//		//UGameplayStatics::ApplyPointDamage(OtherActor, CurrentLaunchTurret->GetDamage(), SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
	//	}

	//}
	







}