// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MT_GatlingProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UMT_HealthComponent;




UCLASS()
class MARINETEAM_API AMT_GatlingProjectile : public AActor
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Projectile")
	USphereComponent* ProjectileCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Projectile")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Projectile")
	UProjectileMovementComponent* ProjectileMovementComponent;


protected:


	UFUNCTION()
	void CheckProjectileDamage(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	
public:	
	// Sets default values for this actor's properties
	AMT_GatlingProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
