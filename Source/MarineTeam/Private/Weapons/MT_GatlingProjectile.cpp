// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MT_GatlingProjectile.h"
#include "Weapons/MT_LaunchProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/MT_HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/MT_GatlingCIWS.h"


// Sets default values
AMT_GatlingProjectile::AMT_GatlingProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ProjectileCollision"));
	RootComponent = ProjectileCollision;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(ProjectileCollision);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;


}

// Called when the game starts or when spawned
void AMT_GatlingProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this, &AMT_GatlingProjectile::CheckProjectileDamage);

}

// Called every frame
void AMT_GatlingProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMT_GatlingProjectile::CheckProjectileDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AMT_LaunchProjectile* LaunchProjectile = Cast<AMT_LaunchProjectile>(OtherActor);

	AActor* CurrentOwner = GetOwner();

	if (IsValid(CurrentOwner))
	{
		AMT_GatlingCIWS* CurrentGatlingCIWS = Cast<AMT_GatlingCIWS>(CurrentOwner);

		if(IsValid(CurrentGatlingCIWS))
		{

			UGameplayStatics::ApplyPointDamage(LaunchProjectile, CurrentGatlingCIWS->GetDamage(), SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);

			//if (IsValid(LaunchProjectile))
			//{
			//bool bPlayerAttackingEnemy = LaunchProjectile->GetProjectileType() == EMT_LauncherProjectileType::CharacterType_Enemy;
			////bool bEnemyAttackingPlayer = LaunchProjectile->GetProjectileType() == EMT_LauncherProjectileType::CharacterType_Enemy && LaunchProjectile->GetProjectileType() == EMT_LauncherProjectileType::CharacterType_Player;
			////if (bPlayerAttackingEnemy || bEnemyAttackingPlayer)
			//	if (bPlayerAttackingEnemy)
			//	{
			//		UGameplayStatics::ApplyPointDamage(LaunchProjectile, CurrentGatlingCIWS->GetDamage(), SweepResult.Location, SweepResult, GetInstigatorController(), this, nullptr);
			//	}

			//}
		}
	}
}


