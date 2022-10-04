// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MT_LaunchProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UMT_HealthComponent;
class UParticleSystem;
class USpringArmComponent;
class UCameraComponent;

UENUM(Blueprintable)
enum class EMT_LauncherProjectileType : uint8
{
	//CameraType_None		UMETA(DisplayName = "None"), //Siempre se debe empezar un valor por defecto, es este caso le ponemos None
	CharacterType_Player	UMETA(DisplayName = "Player Missile"),
	CharacterType_Enemy	    UMETA(DisplayName = "Enemy Missile")
};


UCLASS()
class MARINETEAM_API AMT_LaunchProjectile : public AActor
{
	GENERATED_BODY()

public:

	bool bIsDestroy;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Components")
	USphereComponent* ProjectileCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Components")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Components")
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) //Dejamos sin categoria para que sean tomado como primordiales
	EMT_LauncherProjectileType ProjectileType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Components")
	UMT_HealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Effects")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Components")
	UCameraComponent* CameraLauncher;

	

protected:

	UFUNCTION()
	void OnHealthChanged(UMT_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	UFUNCTION()
	void CheckOverlapLaunchProjectil(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:	
	// Sets default values for this actor's properties
	AMT_LaunchProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	EMT_LauncherProjectileType GetProjectileType() { return ProjectileType; };

	UPROPERTY(EditAnywhere, Category = "My Sounds")
	UAudioComponent* AudioSoundFluying;

};
