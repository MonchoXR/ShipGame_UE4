// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/MT_Weapon.h"
#include "MT_LaunchTurret.generated.h"

class AMT_LaunchProjectile;
/**
 * 
 */
UCLASS()
class MARINETEAM_API AMT_LaunchTurret : public AMT_Weapon
{
	GENERATED_BODY()

protected:

	bool bCanSpawn;
	bool bDestroySpawn;

protected:


	UPROPERTY(EditAnywhere, Category = "My Sounds")
	USoundBase* SoundCannonShot;

protected:

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Effects")
//	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My LauncherTurret")
	TSubclassOf<AMT_LaunchProjectile> ProjectileClass;


	AMT_LaunchProjectile* CurrentProjectile;
	AMT_LaunchProjectile* CurrentProjectileCam;

	

public:

	AMT_LaunchTurret();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;
	void MoveCameraToTarget(AMT_LaunchProjectile* CurrentProjectileWithCam);

protected:

	virtual void StartAction() override;
	virtual void StartActionWithCam() override;

	virtual void StopAction() override;

public: 
	float GetDamage() const { return Damage; };//Enviamos daño al Projectil de Torreta
};
