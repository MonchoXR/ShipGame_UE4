// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
//#include "MyCustomPawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/TextRenderComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

#include "BaseShip.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UStaticMeshComponent;
class AMT_Weapon;
class UBoxComponent;
class AMT_Character;
class UMT_UMG_EnterBaseShip;
class USphereComponent;
class AMT_LaunchProjectile;
class UMT_HealthComponent;
class AMT_GatlingCIWS;
class AMT_GameMode;
class UAudioComponent;
class AMT_SpectatingCamera;
class UFloatingPawnMovement;


UCLASS()
class MARINETEAM_API ABaseShip : public APawn
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mis Movimientos")
	float JumpForce; //usaremos esta variable en bluerint y para buscarlo click derecho en el editor bluprint y lo encontremos

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mis Movimientos")
	float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mis Movimientos")
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Components", meta = (ClampMin = 0.0, UIMin = 0.0, ClampMax = 90.0, UIMax = 90.0))
	float MaxPitch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Components", meta = (ClampMin = 0.0, UIMin = 0, ClampMax = 90.0, UIMax = 90.0))
	float MaxYaw;

	bool bIsWheelPress;
	//bool bIsOnRadar;
	int ContaWheel;

	FTimerHandle TimerHandle_Debug;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mi Key")
	TArray<FName> Doorkeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AMT_Weapon> InitalWeaponClass; //Referencia Cuando existe en el contenedor Browser

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	AMT_Weapon* CurrentWeapon; //Referencia Cuando ya existe en escena
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AMT_Weapon> InitalWeaponClass2;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
	AMT_Weapon* CurrentWeapon2; 

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mis Componentes")
	AMT_Character* OverlappedCharacter;

	UPROPERTY(EditAnywhere, Category = "My Sounds")
	UAudioComponent* SoundMoveCIWS;

	UPROPERTY(EditAnywhere, Category = "My Sounds")
	UAudioComponent* AudioMoveCIWSFinal;
	

	
	AMT_GatlingCIWS* CurrentGatlingCIWS;

	AMT_GameMode* GameModeReference;



	/*---Components start --- */

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	UStaticMeshComponent* TurretCIWSBaseComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	UStaticMeshComponent* TurretCIWSBodyComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	UStaticMeshComponent* TurretCIWSBarrelComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes")
	USceneComponent* SceneComponentBulletSpawn;

	UPROPERTY(VisibleAnywhere, Category = "Mis Componentes")
	USceneComponent* SceneComponentCIWS;

	UPROPERTY(VisibleAnywhere, Category = "Mis Componentes")
	USceneComponent* ScenePivotCIWS;


	UPROPERTY(BlueprintReadWrite, Category = "Mis Componentes")
	TArray<AMT_LaunchProjectile*> TOverlapLaunchProjectile;

	UPROPERTY(BlueprintReadWrite, Category = "Mis Componentes")
	AMT_LaunchProjectile* OverlapLaunchProjectile;

protected:  

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes") 
	UStaticMeshComponent* StaticShipComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	UCameraComponent* TPSCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	UCameraComponent* FPSCameraComponent1;

	//**MyOwnPawnMovement**/
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	//UMyCustomPawnMovementComponent* OurPawnMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	UFloatingPawnMovement* MyFloatPawnMovement;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	UStaticMeshComponent* TurretBaseComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes")
	UStaticMeshComponent* TurretBarrelComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	UBoxComponent* PlayerCollisonComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	USphereComponent* CIWSRadarCollisonComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Componentes")
	UMT_HealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes")
	USceneComponent* SceneComponentPivotTurret;



/*---Components End --- */


public:
	// Sets default values for this pawn's properties
	ABaseShip();

	//virtual FVector GetPawnViewLocation() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitializeReferences();
	void MoveHorizontal(float value); // como es una funcion usada en BindAxis entonces necesitamos poner paramatro de un valor

	void MoveVertical(float value);

	void PitchCamera(float value);
	void YawCamera(float value);

	void TurnAtRate(float value);
	void LookUpRate(float value);
	void StartWheelUp();
	void StartWheelDown();
	void WheelInterpolation( float LocationOnX, float NewTargetArmLength, float NewSocketOffset);

	void CreateInitialWeapon();
	void StartWeaponAction();
	void StopWeaponAction();
	void StartWeaponActionWithCam();
	UFUNCTION(BlueprintImplementableEvent, Category = "Mis Movimientos") //Hace q se pueda implementar en un blueprint de una clase q debe ver
	void Jump();


	UFUNCTION()
	void estadoOverlap();

	UFUNCTION()
	void CheckPosseFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CheckPosseEndFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void CheckRadarFromCIWS(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void CheckRadarENDFromCIWS(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void OnHealthChanged(UMT_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**MyOwnPawnMovement**/
	//virtual UPawnMovementComponent* GetMovementComponent() const override;



	void AddKey(FName NewKey);

	bool hasKey(FName KeyTag);


};
