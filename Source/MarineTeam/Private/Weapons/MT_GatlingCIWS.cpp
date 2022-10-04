// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/MT_GatlingCIWS.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"
#include "BaseShip.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h" //Para usar el lookat
#include "Weapons/MT_GatlingProjectile.h"
#include "Weapons/MT_LaunchProjectile.h"
#include "MarineTeam/MarineTeam.h"
#include "Components/AudioComponent.h"

AMT_GatlingCIWS::AMT_GatlingCIWS() {

	TraceLength = 10000.0f;
	//MuzzleSocketName = "SCK_Muzzle";
	IsFiring = false;
	bIsTurretMoveInitial = true;
	bhit = false;
	MaxPitch = 90.0f;
	MaxYaw = 90.0f;


	AudioSoundFireCIWS = CreateDefaultSubobject<UAudioComponent>(TEXT("My SoundExplosionComponent"));
	AudioSoundFireCIWS->SetupAttachment(RootComponent);
}

void AMT_GatlingCIWS::EnemigoMasCercano()
{
	AActor* CurrentOwner = GetOwner();
	if (IsValid(CurrentOwner))
	{
		BaseShipOwner = Cast<ABaseShip>(CurrentOwner);

		if (IsValid(BaseShipOwner))
		{
			if (BaseShipOwner->TOverlapLaunchProjectile.Num() == 0)
			{
				if (bIsTurretMoveInitial)
				{
					return;
				}

				UE_LOG(LogTemp, Log, TEXT("Desactivo Fire"));
				GetWorld()->GetTimerManager().ClearTimer(TimerHandle_StartFireCIWS);
				if (IsValid(AudioSoundFireCIWS))
				{
					AudioSoundFireCIWS->SetActive(false);
				}
				MoveCIWSToInitial();

			}

			if (BaseShipOwner->TOverlapLaunchProjectile.Num() > 0)
			{

				AMT_LaunchProjectile* MisilCercanoInicial = BaseShipOwner->TOverlapLaunchProjectile[0];
				//float distanciaCercanoInicial = GetDistanceTo(MisilCercanoInicial);
				if (IsValid(MisilCercanoInicial))
				{
					float distanciaCercanoInicial = (BaseShipOwner->TurretCIWSBodyComponent->GetComponentLocation() - MisilCercanoInicial->GetActorLocation()).Size();


					for (auto& MisilNuevo : BaseShipOwner->TOverlapLaunchProjectile)
					{
						if (MisilNuevo == nullptr)
						{
							BaseShipOwner->TOverlapLaunchProjectile.Remove(MisilNuevo);
						}

						//float distanciaNuevaEnemigoConTorreta = GetDistanceTo(MisilNuevo);
						float distanciaNuevaEnemigoConTorreta = (BaseShipOwner->TurretCIWSBodyComponent->GetComponentLocation() - MisilNuevo->GetActorLocation()).Size();
						if (distanciaNuevaEnemigoConTorreta < distanciaCercanoInicial)
						{
							MisilCercanoInicial = MisilNuevo;
							distanciaCercanoInicial = distanciaNuevaEnemigoConTorreta;
						}
					}
				}
				BaseShipOwner->OverlapLaunchProjectile = MisilCercanoInicial;

				MoveCIWSToTarget();

				bIsTurretMoveInitial = false;
			}
		}
	}
}

void AMT_GatlingCIWS::StartAction()
{
	Super::StartAction();

	AActor* CurrentOwner = GetOwner();
	
	if (IsValid(CurrentOwner))
	{

		BaseShipOwner = Cast<ABaseShip>(CurrentOwner);



		if (IsValid(BaseShipOwner))
		{

			if (IsValid(GatlingProjectileClass))
			{
				CurrentProjectile = GetWorld()->SpawnActor<AMT_GatlingProjectile>(GatlingProjectileClass, BaseShipOwner->SceneComponentCIWS->GetComponentLocation(), BaseShipOwner->SceneComponentCIWS->GetComponentRotation());

				if (IsValid(CurrentProjectile))
				{
					CurrentProjectile->SetOwner(this);
				}
			}


			if (IsValid(MuzzleEffect))
			{

				//  ABaseShip* BaseShipOwner = Cast<ABaseShip>(CurrentOwner);
				if (IsValid(BaseShipOwner))
				{
					UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, BaseShipOwner->SceneComponentCIWS);
				}

			}

			if (IsValid(BaseShipOwner->TurretCIWSBarrelComponent))
			{

				//Rotacion solo del barrel de su mismo lugar
				FRotator NewRotation = BaseShipOwner->TurretCIWSBarrelComponent->GetRelativeRotation();
				NewRotation.Roll = NewRotation.Roll + 18.0;
				BaseShipOwner->TurretCIWSBarrelComponent->SetRelativeRotation(NewRotation);
			}
		}

		

		GetWorld()->GetTimerManager().SetTimer(TimerHandle_StartFireCIWS, this, &AMT_GatlingCIWS::StartAction, 0.1f, true);
	}
}



void AMT_GatlingCIWS::MoveCIWSToTarget()
{
	AActor* CurrentOwner = GetOwner();
	BaseShipOwner = Cast<ABaseShip>(CurrentOwner);

	AMT_LaunchProjectile* MyOverlapLaunchProjectile = BaseShipOwner->OverlapLaunchProjectile;
	UStaticMeshComponent* MyTurretCIWSBodyComponent = BaseShipOwner->TurretCIWSBodyComponent;
	UStaticMeshComponent* MyTurretCIWSBaseComponent = BaseShipOwner->TurretCIWSBaseComponent;
	USceneComponent* MyScenePivotCIWS = BaseShipOwner->ScenePivotCIWS;


	if (IsValid(MyOverlapLaunchProjectile) && IsValid(MyTurretCIWSBodyComponent) && IsValid(MyTurretCIWSBaseComponent)) 
	{

		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(MyTurretCIWSBodyComponent->GetComponentLocation(), MyOverlapLaunchProjectile->GetActorLocation());
		
		FRotator NewTargetLocalRotator = UKismetMathLibrary::InverseTransformRotation(MyScenePivotCIWS->GetComponentTransform(), TargetRotation);
		//UE_LOG(LogTemp, Warning, TEXT("Mi Rotacion Inverse   %s "), *NewTargetLocalRotator.ToString());
		
		float TargetRotationPitchClamped = FMath::ClampAngle(NewTargetLocalRotator.Pitch, 0.0f, MaxPitch);
		float TargetRotationYawClamped = FMath::ClampAngle(NewTargetLocalRotator.Yaw, -MaxYaw, MaxYaw);
		//UE_LOG(LogTemp, Warning, TEXT("Mi rotarion de LineTracees %f, %f"), TargetRotationPitchClamped, TargetRotationYawClamped);

		if (TargetRotationPitchClamped == 0.0f || TargetRotationPitchClamped == MaxPitch || TargetRotationYawClamped == -MaxYaw || TargetRotationYawClamped == MaxYaw)

		{
			//MyOverlapLaunchProjectile->ProjectileCollision->SetCollisionResponseToChannel(COLLISION_RADAR, ECR_Ignore);
			BaseShipOwner->TOverlapLaunchProjectile.Remove(MyOverlapLaunchProjectile);
			
			MoveCIWSToInitial();
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle_StartFireCIWS);
			if (IsValid(AudioSoundFireCIWS))
			{
				AudioSoundFireCIWS->SetActive(false);
			}
		}
		else
		{
			
		    MyTurretCIWSBaseComponent->SetRelativeRotation(FMath::RInterpConstantTo(MyTurretCIWSBaseComponent->GetRelativeRotation(), FRotator(0.0f, NewTargetLocalRotator.Yaw, 0.f), GetWorld()->GetDeltaSeconds(), 120.0f));
			MyTurretCIWSBodyComponent->SetRelativeRotation(FMath::RInterpConstantTo(MyTurretCIWSBodyComponent->GetRelativeRotation(), FRotator(NewTargetLocalRotator.Pitch, 0.0f, 0.0f), GetWorld()->GetDeltaSeconds(), 120.0f));

			//UE_LOG(LogTemp, Warning, TEXT("Mi Rotacion Body      %s "), *MyTurretCIWSBaseComponent->GetRelativeRotation().ToString());

			FVector ShotDirection = MyTurretCIWSBodyComponent->GetComponentRotation().Vector();
			FVector TraceEnd = MyTurretCIWSBodyComponent->GetComponentLocation() + (ShotDirection * 6000.0f);

			if (bDrawLineTrace)
			{
				DrawDebugLine(GetWorld(), MyTurretCIWSBodyComponent->GetComponentLocation(), TraceEnd, FColor::Red, false, 1.0f, 0.0f, 1.0f);
			}
				

			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			QueryParams.AddIgnoredActor(BaseShipOwner);//para que no colision en si mismo				
			QueryParams.bTraceComplex = true;

			bhit = GetWorld()->LineTraceSingleByChannel(HitResult, MyTurretCIWSBodyComponent->GetComponentLocation(), TraceEnd, ECC_Visibility, QueryParams);
			if (bhit)
			{
				if (IsFiring)
				{
					return;
				}

				
				StartAction();
				//SoundStartFireCIWS();
				if (IsValid(AudioSoundFireCIWS))
				{
					AudioSoundFireCIWS->SetActive(true);
				}

				IsFiring = true;
			}
			else
				{
					GetWorld()->GetTimerManager().ClearTimer(TimerHandle_StartFireCIWS);
					if (IsValid(AudioSoundFireCIWS))
					{
						AudioSoundFireCIWS->SetActive(false);
					}
					IsFiring = false;
				}
		}
	
	}
}


void AMT_GatlingCIWS::MoveCIWSToInitial()
{
	AActor* CurrentOwner = GetOwner();
	BaseShipOwner = Cast<ABaseShip>(CurrentOwner);


	UStaticMeshComponent* MyTurretCIWSBodyComponent = BaseShipOwner->TurretCIWSBodyComponent;
	UStaticMeshComponent* MyTurretCIWSBaseComponent = BaseShipOwner->TurretCIWSBaseComponent;


	if (IsValid(MyTurretCIWSBodyComponent) && IsValid(MyTurretCIWSBaseComponent)) 
	{
		CurrentTime += GetWorld()->GetDeltaSeconds();
		MyTurretCIWSBodyComponent->SetRelativeRotation(FMath::RInterpConstantTo(MyTurretCIWSBodyComponent->GetRelativeRotation(), FRotator(0.0f, 0.0f, 0.0f), 1.3f, 1.0f));
		MyTurretCIWSBaseComponent->SetRelativeRotation(FMath::RInterpConstantTo(MyTurretCIWSBaseComponent->GetRelativeRotation(), FRotator(0.0f, 0.0f, 0.0f), 1.3f, 1.0f));
		//UE_LOG(LogTemp, Warning, TEXT("Mi rotarion es %s "), *TurretCIWSBodyComponent->GetRelativeRotation().ToString());

		if (CurrentTime >= 3.0f)
		{
			CurrentTime = 0.0f;
			bIsTurretMoveInitial = true;
		}
	}

}


void AMT_GatlingCIWS::StopAction()
{

	BP_StopAction();
	//UE_LOG(LogTemp, Log, TEXT("Player stop Action !"));
}



//void AMT_GatlingCIWS::StartAction()
//{
//	Super::StartAction();
//
//	AActor* CurrentOwner = GetOwner();
//	BaseShipOwner = Cast<ABaseShip>(CurrentOwner);
//
//
//
//	if (IsValid(BaseShipOwner))
//	{
//		FVector EyeLocation;
//		FRotator EyeRotation;
//
//		BaseShipOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
//
//
//		//FVector ShotDirection = EyeRotation.Vector();
//		//FVector TraceEnd = EyeLocation + (ShotDirection * TraceLength);
//
//
//		//FCollisionQueryParams QueryParams;
//		//QueryParams.AddIgnoredActor(this);
//		//QueryParams.AddIgnoredActor(BaseShipOwner);//para que no colision en si mismo
//		//QueryParams.bTraceComplex = true;
//
//		//FVector TraceEndPoint = TraceEnd;
//
//	//	FHitResult HitResult;
//
//		//if (IsValid(BaseShipOwner->OverlapLaunchProjectile))
//		//{
//			//bool bhit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, TraceEnd, ECC_Visibility, QueryParams);
//			//bool bhit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, BaseShipOwner->GetActorLocation(), ECC_Visibility, QueryParams);//ECC_Visibility tiene q estar bloqueada
//			//bool bhit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, BaseShipOwner->OverlapLaunchProjectile->GetActorLocation(), ECC_Visibility, QueryParams);//ECC_Visibility tiene q estar bloqueada
//
//
//			/**
//			bool bhit = GetWorld()->LineTraceSingleByChannel(HitResult, EyeLocation, BaseShipOwner->OverlapLaunchProjectile->GetActorLocation(), COLLISION_RADAR, QueryParams);//ECC_Visibility tiene q estar bloqueada
//			if (bhit)
//			{
//
//				//Hace daño
//				AActor* HitActor = HitResult.GetActor();
//				if (IsValid(HitActor))
//				{
//					UGameplayStatics::ApplyPointDamage(HitActor, Damage, FVector(0, 0, 0), HitResult, CurrentOwner->GetInstigatorController(), this, DamageType);
//					//UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, HitResult, CurrentOwner->GetInstigatorController(), this, DamageType);
//
//
//				}
//
//				//TraceEndPoint = HitResult.ImpactPoint;
//
//				if (IsValid(ImpacEffect))
//				{
//					//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpacEffect, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
//
//					//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpacEffect, TraceEndPoint, HitResult.ImpactNormal.Rotation());
//				}
//			}
//
//
//			if (bDrawLineTrace)
//			{
//
//				//UE_LOG(LogTemp, Warning, TEXT("Entra Debug"));
//				//DrawDebugLine(GetWorld(), EyeLocation, BaseShipOwner->GetActorLocation(), FColor::Red, false, 1.0f, 0.0f, 1.0f);
//				DrawDebugLine(GetWorld(), EyeLocation, BaseShipOwner->OverlapLaunchProjectile->GetActorLocation(), FColor::Red, false, 1.0f, 0.0f, 1.0f);
//
//			}
//
//			*
//		if (IsValid(GatlingProjectileClass))
//		{
//
//			CurrentProjectile = GetWorld()->SpawnActor<AMT_GatlingProjectile>(GatlingProjectileClass, BaseShipOwner->SceneComponentCIWS->GetComponentLocation(), BaseShipOwner->SceneComponentCIWS->GetComponentRotation());
//
//			if (IsValid(CurrentProjectile))
//			{
//				CurrentProjectile->SetOwner(this);
//			}
//
//
//		}
//
//
//		if (IsValid(MuzzleEffect))
//		{
//
//			//  ABaseShip* BaseShipOwner = Cast<ABaseShip>(CurrentOwner);
//			if (IsValid(BaseShipOwner))
//			{
//				UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, BaseShipOwner->SceneComponentCIWS);
//			}
//
//		}
//
//
//		/*
//		if (IsValid(TraceEffect))
//		{
//			//USkeletalMeshComponent* CharacterMeshComponent = CurrentOwnerCharacter->GetMesh();
//			if (IsValid(BaseShipOwner))
//			{
//				FVector SceneLocation = BaseShipOwner->SceneComponentCIWS->GetComponentLocation();
//				//	FVector MuzzleSocketLocation = CharacterMeshComponent->GetSocketLocation(MuzzleSocketName);
//				UParticleSystemComponent* TraceComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, SceneLocation);
//
//				if (IsValid(TraceComponent))
//				{
//					TraceComponent->SetVectorParameter(TraceParamName, HitResult.ImpactPoint);
//
//				}
//			}
//		}
//		*/
//		//}
//	}
//
//	//UE_LOG(LogTemp, Log, TEXT("Player stop Action !"));
//}
