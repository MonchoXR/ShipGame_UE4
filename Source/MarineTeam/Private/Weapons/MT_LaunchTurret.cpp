// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MT_LaunchTurret.h"
#include "Weapons/MT_LaunchProjectile.h"
#include "BaseShip.h"
#include "Kismet/GameplayStatics.h" //uso spawn de sonidos 


void AMT_LaunchTurret::BeginPlay()
{
	Super::BeginPlay();

}

AMT_LaunchTurret::AMT_LaunchTurret()
{

	/*MuzzleSocketName = "SCK_Muzzle";*/
	bCanSpawn = true;
	
}



void AMT_LaunchTurret::Tick(float DeltaTime)
{
	
}

void AMT_LaunchTurret::StartAction()
{
	Super::StartAction();

	AActor* CurrentOwner = GetOwner();
	if (CurrentOwner)
	{
		ABaseShip* BaseShipOwner = Cast<ABaseShip>(CurrentOwner);
		if (IsValid(BaseShipOwner))
		{

			//	USkeletalMeshComponent* CharacterMeshComponent = CurrentOwnerCharacter->GetMesh();
			//	if (IsValid(CharacterMeshComponent))
			//	{
			//		FVector MuzzleSocketLocation = CharacterMeshComponent->GetSocketLocation(MuzzleSocketName);
			//		FRotator MuzzleSocketRotation = CharacterMeshComponent->GetSocketRotation(MuzzleSocketName);

			if (IsValid(ProjectileClass))
			{

				CurrentProjectile = GetWorld()->SpawnActor<AMT_LaunchProjectile>(ProjectileClass, BaseShipOwner->SceneComponentBulletSpawn->GetComponentLocation(), BaseShipOwner->SceneComponentBulletSpawn->GetComponentRotation());
				if (IsValid(CurrentProjectile))
				{
					
					CurrentProjectile->SetOwner(this);

				}
			}
			if (IsValid(SoundCannonShot))
			{
				UGameplayStatics::SpawnSoundAtLocation(this, SoundCannonShot, GetActorLocation());

			}
			

		
		}
	}

}

void AMT_LaunchTurret::StartActionWithCam()
{
	Super::StartActionWithCam();

	AActor* CurrentOwner = GetOwner();
	if (CurrentOwner)
	{
		ABaseShip* BaseShipOwner = Cast<ABaseShip>(CurrentOwner);
		if (IsValid(BaseShipOwner))
		{

			if (bCanSpawn)
			{
				//UE_LOG(LogTemp, Warning, TEXT("entre if booleando bCanSpawn es %s"), bCanSpawn ? TEXT("true") : TEXT("false"));

				CurrentProjectileCam = GetWorld()->SpawnActor<AMT_LaunchProjectile>(ProjectileClass, BaseShipOwner->SceneComponentBulletSpawn->GetComponentLocation(), BaseShipOwner->SceneComponentBulletSpawn->GetComponentRotation());
			
			//	CurrentProjectileCam->AudioSoundFluying->SetActive(true);
				
				if (IsValid(CurrentProjectileCam))
				{
					CurrentProjectileCam->SetOwner(this);
					if (IsValid(SoundCannonShot))
					{
						UGameplayStatics::SpawnSoundAtLocation(this, SoundCannonShot, GetActorLocation());
					}
					//UE_LOG(LogTemp, Warning, TEXT("entre if booleando bIsDestroyOnOcean es %s"), CurrentProjectileCam->bIsDestroyOnOcean ? TEXT("true") : TEXT("false"));
					if (CurrentProjectileCam->bIsDestroy)
					{
						MoveCameraToTarget(CurrentProjectileCam);
						CurrentProjectileCam->bIsDestroy = false;
						bCanSpawn = false;
					}

				}
			
			
			}
			else 
			{
				if (CurrentProjectileCam->bIsDestroy)
				{
					bCanSpawn = true;
					StartActionWithCam();
				}
				else
				{
					StartAction();
				}
			}

		//	UE_LOG(LogTemp, Warning, TEXT("entro tick booleando bIsDestroyOnOcean es %s"), CurrentProjectileCam->bIsDestroyOnOcean ? TEXT("true") : TEXT("false"));
			
		/*	if (IsValid(CurrentProjectileCam))
			{*/

	
	/*		if (CurrentProjectileCam->bIsDestroyOnOcean)
			{
				bCanSpawn = true;
				StartActionWithCam();
			}

			else {
				CurrentProjectile = GetWorld()->SpawnActor<AMT_LaunchProjectile>(ProjectileClass, BaseShipOwner->SceneComponentBulletSpawn->GetComponentLocation(), BaseShipOwner->SceneComponentBulletSpawn->GetComponentRotation());
				CurrentProjectile->SetOwner(this);

				if (IsValid(SoundCannonShot))
				{
					UGameplayStatics::SpawnSoundAtLocation(this, SoundCannonShot, GetActorLocation());

				}
			}*/

		}
	}

}


void AMT_LaunchTurret::MoveCameraToTarget(AMT_LaunchProjectile* CurrentProjectileWithCam)
{
	AActor* CurrentOwner = GetOwner();
	ABaseShip* BaseShipOwner = Cast<ABaseShip>(CurrentOwner);

	AController* BaseShipController = BaseShipOwner->GetController();

	if (IsValid(BaseShipController))
	{

		APlayerController* PlayerBaseShipController = Cast<APlayerController>(BaseShipController);

		if (PlayerBaseShipController)
		{
			PlayerBaseShipController->SetViewTargetWithBlend(CurrentProjectileWithCam, 0.5f);//Esta funcion mueve la camara del controlador a un punto halladamos detectado como target
		//	UE_LOG(LogTemp, Warning, TEXT("entra view target"));
			
		}

	}


}

void AMT_LaunchTurret::StopAction()
{


}
