// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MT_BaseShipEnemy.h"
#include "Components/MT_HealthComponent.h"
#include "Weapons/MT_LaunchProjectile.h"

AMT_BaseShipEnemy::AMT_BaseShipEnemy()
{

}

void AMT_BaseShipEnemy::CheckRadarFromCIWS(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::CheckRadarFromCIWS(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);


	if (IsValid(OtherActor))
	{

		OverlapLaunchProjectile = Cast<AMT_LaunchProjectile>(OtherActor);

		if (IsValid(OverlapLaunchProjectile) && OverlapLaunchProjectile->GetProjectileType() == EMT_LauncherProjectileType::CharacterType_Player)
		{
			TOverlapLaunchProjectile.Add(OverlapLaunchProjectile);

			//if (TOverlapLaunchProjectile.Num() == 1) //Para evitar que suene de nuevo cuando haga otro everlap
			//{

		
			//	if (IsValid(SoundMoveCIWS))
			//	{
			//		SoundMoveCIWS->SetActive(true);

			//	}

			//}

		}
	}
	

}

void AMT_BaseShipEnemy::CheckRadarENDFromCIWS(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::CheckRadarENDFromCIWS(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (IsValid(OtherActor))
	{
		OverlapLaunchProjectile = Cast<AMT_LaunchProjectile>(OtherActor);
		if (IsValid(OverlapLaunchProjectile) && OverlapLaunchProjectile->GetProjectileType() == EMT_LauncherProjectileType::CharacterType_Player)
		{

			TOverlapLaunchProjectile.Remove(OverlapLaunchProjectile);
		/*	if (TOverlapLaunchProjectile.Num() == 0)
			{

				if (IsValid(AudioMoveCIWSFinal))
				{
					AudioMoveCIWSFinal->SetActive(true);
				}

			}*/

		}

	}
}
