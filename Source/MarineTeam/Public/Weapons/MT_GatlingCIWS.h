// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/MT_Weapon.h"
#include "MT_GatlingCIWS.generated.h"


class UParticleSystem;
class ABaseShip;
class AMT_GatlingProjectile;
class AMT_LaunchProjectile;
class UAudioComponent;
/**
 * 
 */
UCLASS()
class MARINETEAM_API AMT_GatlingCIWS : public AMT_Weapon
{
	GENERATED_BODY()

public:

	AMT_GatlingCIWS();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Lincetrace|Debug")
	bool bDrawLineTrace;

	bool bIsTurretMoveInitial;
	bool IsFiring;
	bool bhit;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Lincetrace")
	float TraceLength;

	float CurrentTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Components", meta = (ClampMin = 0.0, UIMin = 0.0, ClampMax = 90.0, UIMax =90.0))
	float MaxPitch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Components",   meta = (ClampMin = 0.0, UIMin = 0, ClampMax = 90.0, UIMax = 90.0))
	float MaxYaw;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "My Components", meta = (ClampMin = -MaxYaw, UIMin = -MaxYaw))
	//float MinYaw;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Effects")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Effects")
	UParticleSystem* ImpacEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Effects")
	UParticleSystem* TraceEffect;

	//UPROPERTY(EditAnywhere, Category = "Mis sonidos")
	//USoundBase* SoundFireCIWS;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Effects")
	FName TraceParamName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My LauncherTurret")
	TSubclassOf<AMT_GatlingProjectile> GatlingProjectileClass;

	AMT_GatlingProjectile* CurrentProjectile;

	ABaseShip* BaseShipOwner;

public:
//	FTimerHandle TimerHandle_SoundStartFireCIWS;
	FTimerHandle TimerHandle_StartFireCIWS;

	UPROPERTY(EditAnywhere, Category = "My Sounds")
	UAudioComponent* AudioSoundFireCIWS;

protected:
	
	virtual void StartAction() override;

	virtual void StopAction() override;

public:

	float GetDamage() const{ return Damage; };//Enviamos daño al Projectil GatilingCIWS

	void EnemigoMasCercano();
	void MoveCIWSToTarget();
	void MoveCIWSToInitial();
};


