// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseShip.h"
#include "MT_BaseShipEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MARINETEAM_API AMT_BaseShipEnemy : public ABaseShip
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMT_BaseShipEnemy();

protected:

//	virtual void OnHealthChanged(UMT_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) override;


virtual	void CheckRadarFromCIWS(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;


virtual	void CheckRadarENDFromCIWS(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;


};
