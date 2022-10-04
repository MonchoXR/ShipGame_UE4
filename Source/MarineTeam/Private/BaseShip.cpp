// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseShip.h"
#include "Weapons/MT_Weapon.h"
#include "MT_Character.h"
#include "Weapons/MT_LaunchProjectile.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h" //uso spawn de sonidos 
#include "MarineTeam/MarineTeam.h"
#include "Components/MT_HealthComponent.h"
#include "Weapons/MT_GatlingCIWS.h"
#include "Core/MT_GameMode.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h" //Para usar el lookat
#include "DrawDebugHelpers.h"
#include "MT_SpectatingCamera.h"



// Sets default values
ABaseShip::ABaseShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	PlayerCollisonComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("My PlayerColliderComponent"));
//	PlayerCollisonComponent->SetupAttachment(StaticShipComponent);
	PlayerCollisonComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);//No fisicas para esta colision	 										
	PlayerCollisonComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	PlayerCollisonComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	RootComponent = PlayerCollisonComponent;


	StaticShipComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My StaticShipComponent"));
	StaticShipComponent->SetupAttachment(PlayerCollisonComponent);
	//RootComponent = StaticShipComponent;

	//FPSCameraComponent1 = CreateDefaultSubobject<UCameraComponent>(TEXT("My FPS CameraComponent"));
	//FPSCameraComponent1->SetupAttachment(StaticShipComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("My SpringArmComponent"));
	SpringArmComponent->SetupAttachment(PlayerCollisonComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->TargetArmLength = 3100.0f;
	SpringArmComponent->SocketOffset.Z=1950.0f;
	SpringArmComponent->SetRelativeLocation(FVector(5580.0f, 0.0f,0.0f ));

	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("My TPS CameraComponent"));
	TPSCameraComponent->SetupAttachment(SpringArmComponent);


	CIWSRadarCollisonComponent = CreateDefaultSubobject<USphereComponent>(TEXT("My CIWSColliderComponent"));
	CIWSRadarCollisonComponent->SetupAttachment(PlayerCollisonComponent);
	CIWSRadarCollisonComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);//No fisicas para esta colision	 										
	CIWSRadarCollisonComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CIWSRadarCollisonComponent->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECR_Overlap);


	TurretBaseComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My TurretBaseComponent"));
	TurretBaseComponent->SetupAttachment(PlayerCollisonComponent);

	TurretBarrelComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My TurretBarrelComponent"));
	TurretBarrelComponent->SetupAttachment(TurretBaseComponent);


	TurretCIWSBaseComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My TurretCIWSBaseComponent"));
	TurretCIWSBaseComponent->SetupAttachment(CIWSRadarCollisonComponent);

	TurretCIWSBodyComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My TurretCIWSBodyComponent"));
	TurretCIWSBodyComponent->SetupAttachment(TurretCIWSBaseComponent);

	TurretCIWSBarrelComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My TurretCIWSBarrelComponent"));
	TurretCIWSBarrelComponent->SetupAttachment(TurretCIWSBodyComponent);

	SceneComponentBulletSpawn = CreateDefaultSubobject<USceneComponent>(TEXT("My SceneComponentBulletSpawn"));
	SceneComponentBulletSpawn->SetupAttachment(TurretBarrelComponent);

	SceneComponentPivotTurret = CreateDefaultSubobject<USceneComponent>(TEXT("My SceneComponentPivotTurret"));
	SceneComponentPivotTurret->SetupAttachment(PlayerCollisonComponent);


	SceneComponentCIWS = CreateDefaultSubobject<USceneComponent>(TEXT("My SceneComponentCIWS"));
	SceneComponentCIWS->SetupAttachment(TurretCIWSBarrelComponent);
	
	HealthComponent = CreateDefaultSubobject<UMT_HealthComponent>(TEXT("My HealthComponent")); //Como es un componente abstracto no necesita atachar a un root 

	/**MyOwnPawnMovement**/
	//OurPawnMovement = CreateDefaultSubobject<UMyCustomPawnMovementComponent>(TEXT("My MovemementComponent"));
	//OurPawnMovement->UpdatedComponent = RootComponent;

	MyFloatPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("My FloatMovemementComponent"));
	
	ScenePivotCIWS = CreateDefaultSubobject<USceneComponent>(TEXT("My SceneComponentPivot"));
	ScenePivotCIWS->SetupAttachment(PlayerCollisonComponent);

	AudioMoveCIWSFinal = CreateDefaultSubobject<UAudioComponent>(TEXT("My SoundExplosionComponent"));
	AudioMoveCIWSFinal->SetupAttachment(PlayerCollisonComponent);


	//StaticShipComponent->SetSimulatePhysics(true)//Activa la fisicas, siempre al mesh que corresponde para conociendo
	//MoveForce = 10000.0f;
	//MoveSpeed = 1000.0f;
	//RotationSpeed = 20.0f;

	BaseTurnRate = 10.0f;
	BaseLookUpRate = 10.0f;
	
	MaxPitch = 90.0f;
	MaxYaw = 90.0f;

	ContaWheel = 0;

}


// Called when the game starts or when spawned
void ABaseShip::BeginPlay()
{
	Super::BeginPlay();

	InitializeReferences();
	CreateInitialWeapon();

	PlayerCollisonComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseShip::CheckPosseFromPlayer);
	PlayerCollisonComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseShip::CheckPosseEndFromPlayer);

	CIWSRadarCollisonComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseShip::CheckRadarFromCIWS);
	CIWSRadarCollisonComponent->OnComponentEndOverlap.AddDynamic(this, &ABaseShip::CheckRadarENDFromCIWS);

	HealthComponent->OnHealthChangeDelegate.AddDynamic(this, &ABaseShip::OnHealthChanged);

//	GetWorld()->GetTimerManager().SetTimer(TimerHandle_Debug, this, &ABaseShip::estadoOverlap, 1.0f, true);
}

void ABaseShip::InitializeReferences()
{
	GameModeReference = Cast<AMT_GameMode>(GetWorld()->GetAuthGameMode()); //Traemos el GameModeActual que exite en escena
}


void ABaseShip::MoveHorizontal(float value)
{
	//FVector Force(MoveForce*value,0,0); //Vector 3 en unreal
	//StaticShipComponent->AddForce(Force);


	/********Una forma de mover***************************/
	//float MovemententEnX = value * MoveSpeed * GetWorld()->GetDeltaSeconds(); //GetDeltaSeconds es como deltatime
	//AddActorLocalOffset(FVector(MovemententEnX, 0, 0),true);
	/********Una forma de mover***************************/

	/*AddMovementInput(GetActorForwardVector() * value);  */

	/**MyOwnPawnMovement**/
	//if (OurPawnMovement)
	//{
	//	OurPawnMovement->AddInputVector(GetActorForwardVector() * value);
	//}

	if (MyFloatPawnMovement)
	{
		MyFloatPawnMovement->AddInputVector(GetActorForwardVector() * value);
	}


}

void ABaseShip::MoveVertical(float value)
{
	//FVector Force(0, MoveForce * value, 0); //Vector 3 en unreal
	//StaticShipComponent->AddForce(Force);

	/*AddMovementInput(GetActorRightVector() * value);*/

	/**MyOwnPawnMovement**/
	//if (OurPawnMovement)
	//{
	//	OurPawnMovement->AddInputVector(GetActorRightVector() * value);
	//}

	if (MyFloatPawnMovement)
	{
		MyFloatPawnMovement->AddInputVector(GetActorRightVector() * value);
	}


}

void ABaseShip::PitchCamera(float value)
{
	//CameraInput.Y = value;

	
	AddControllerPitchInput(value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABaseShip::YawCamera(float value)
{

		//FRotator NewRotation = TPSCameraComponent->GetComponentRotation();
		//TurretBarrelComponent->SetWorldRotation(NewRotation);
     	//TurretBaseComponent->SetWorldRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));	
		
	if (IsValid(TPSCameraComponent))
	{

		FRotator NewRotation = TPSCameraComponent->GetComponentRotation();
		FRotator NewTargetLocalRotator = UKismetMathLibrary::InverseTransformRotation(SceneComponentPivotTurret->GetComponentTransform(), NewRotation);

	//	FVector TraceEnd = SceneComponentPivotTurret->GetComponentLocation() + (TPSCameraComponent->GetComponentRotation().Vector() * 15000.0f);
		//	DrawDebugLine(GetWorld(), SceneComponentPivotTurret->GetComponentLocation(), TraceEnd, FColor::Red, false, 3.0f, 0.0f, 1.0f);

		float TargetRotationPitchClamped = FMath::ClampAngle(NewTargetLocalRotator.Pitch, -10.0f, MaxPitch);//Con el clamp no puedes igualar entre esos ambos valores
		float TargetRotationYawClamped = FMath::ClampAngle(NewTargetLocalRotator.Yaw, -MaxYaw, MaxYaw);

		TurretBarrelComponent->SetRelativeRotation(FMath::RInterpConstantTo(TurretBarrelComponent->GetRelativeRotation(), FRotator(TargetRotationPitchClamped, 0.0f, 0.f), GetWorld()->GetDeltaSeconds(), 5.0f));
		//UE_LOG(LogTemp, Warning, TEXT("Mi TargetRotationPitchClamped es %f "), TargetRotationPitchClamped);

		/*
		if (TargetRotationPitchClamped >=1)
		{
			TurretBarrelComponent->SetRelativeRotation(FMath::RInterpConstantTo(TurretBarrelComponent->GetRelativeRotation(), FRotator(MaxPitch, 0.0f, 0.f), GetWorld()->GetDeltaSeconds(), 5.0f));
		//	UE_LOG(LogTemp, Warning, TEXT("Mi Rotacion mayor que 1    %f "), TurretBarrelComponent->GetRelativeRotation().Pitch);
		}


		if (TargetRotationPitchClamped < 1.0f && TargetRotationPitchClamped > -1)
		{
			TurretBarrelComponent->SetRelativeRotation(FMath::RInterpConstantTo(TurretBarrelComponent->GetRelativeRotation(), FRotator(0.0f, 0.0f, 0.f), GetWorld()->GetDeltaSeconds(), 5.0f));
		}

		if (TargetRotationPitchClamped <= -1)
		{

			TurretBarrelComponent->SetRelativeRotation(FMath::RInterpConstantTo(TurretBarrelComponent->GetRelativeRotation(), FRotator(-MaxPitch, 0.0f, 0.f), GetWorld()->GetDeltaSeconds(), 5.0f));

		}

	*/
	//UE_LOG(LogTemp, Warning, TEXT("Mi Rotacion mayor que 1    %f "), TargetRotationPitchClamped);
		TurretBaseComponent->SetRelativeRotation(FMath::RInterpConstantTo(TurretBaseComponent->GetRelativeRotation(), FRotator(0.0f, TargetRotationYawClamped, 0.f), GetWorld()->GetDeltaSeconds(), 20.0f));

		AddControllerYawInput(value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}


void ABaseShip::TurnAtRate(float value)
{
	

	/*hacemos rotacion al buque*/
	FRotator NewRotationActor = GetActorRotation();
	NewRotationActor.Yaw = NewRotationActor.Yaw + value*0.3;
	SetActorRotation(NewRotationActor);
	
//	AddControllerYawInput(value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABaseShip::LookUpRate(float value)
{
	AddControllerPitchInput(value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABaseShip::StartWheelUp()
{ 
	ContaWheel += 1;

	if (ContaWheel >= 2)
	{
		ContaWheel = 1;
		return;
	}
	
	bIsWheelPress = true;

}

void ABaseShip::StartWheelDown()
{
	ContaWheel -= 1;
	if (ContaWheel < -4)
	{
		ContaWheel = -4;
		return;
	}
	
	bIsWheelPress = true;

}

void ABaseShip::WheelInterpolation(float LocationOnX, float NewTargetArmLength, float NewSocketOffset)
{

	if (IsValid(SpringArmComponent))
	{
		float Newx = (FMath::FInterpConstantTo(SpringArmComponent->GetRelativeLocation().X, LocationOnX, GetWorld()->GetDeltaSeconds(), 2000.0f));
		SpringArmComponent->SetRelativeLocation(FVector(Newx, 0.0f, 0.0f));

		SpringArmComponent->TargetArmLength = FMath::FInterpConstantTo(SpringArmComponent->TargetArmLength, NewTargetArmLength, GetWorld()->GetDeltaSeconds(), 2000.0f);
		SpringArmComponent->SocketOffset.Z = FMath::FInterpConstantTo(SpringArmComponent->SocketOffset.Z, NewSocketOffset, GetWorld()->GetDeltaSeconds(), 2000.0f);
		
	}

	if (SpringArmComponent->GetRelativeLocation().X == LocationOnX && SpringArmComponent->TargetArmLength == NewTargetArmLength && SpringArmComponent->SocketOffset.Z == NewSocketOffset)
	{
		bIsWheelPress = false;
	}

}


/*
void ABaseShip::Jump()
{
	FVector Force(0, 0, JumpForce); //Vector 3 en unreal
	StaticShipComponent->AddImpulse(Force); //AddImpulse fuerza instantanea

}
*/


// Called every frame
void ABaseShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (!MovementDirection.IsZero())
	//{
	//	const FVector NewLocation = GetActorLocation() + (MovementDirection * DeltaTime * MoveSpeed);
	//	SetActorLocation(NewLocation);

	//}

	if (IsValid(CurrentGatlingCIWS))
	{
		CurrentGatlingCIWS->EnemigoMasCercano();
	}

	/*********************Camera Wheel*********************************/
	
	if (bIsWheelPress) 
	{
		
		if (ContaWheel == 1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tick en interpolation 1"));
			WheelInterpolation(1400.0f, 80.0f, 550.0f);
		}

		if (ContaWheel == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tick en interpolation 0"));
			WheelInterpolation(1400.0f, 580.0f, 580.0f);
		}

		if (ContaWheel == -1)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tick en interpolation -1"));
			WheelInterpolation(0.0f, 3000.0f, 800.0f);
		}


		if (ContaWheel == -2)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tick en interpolation -2"));
			WheelInterpolation(0.0f, 3500.0f , 1300.0f);
		}


		if (ContaWheel == -3)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tick en interpolation -3"));
			WheelInterpolation(0.0f, 4000.0f, 1800.0f);
		}

		if (ContaWheel == -4)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tick en interpolation -4"));
			WheelInterpolation(0.0f, 4000.0f, 2300.0f);
		}
	}
	/*********************Camera Wheel*********************************/
	
}

// Called to bind functionality to input
void ABaseShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/********************Inicio Movement****************************************************/
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseShip::MoveHorizontal); //BindAxis recibe(nomnre,objeto, funcion qye vamos a crear567(que queremos enlazar)))
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseShip::MoveVertical);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseShip::Jump);

	// *** El &APawn::AddControllerPitchInput puede funcionar con el PawnMovemenetComponent o FloatingMovementComponent **
	//PlayerInputComponent->BindAxis("CameraPitch", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("CameraYaw", this, &APawn::AddControllerYawInput);

	
	PlayerInputComponent->BindAxis("CameraPitch", this, &ABaseShip::PitchCamera);
	PlayerInputComponent->BindAxis("CameraYaw", this, &ABaseShip::YawCamera);

	 PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseShip::LookUpRate);
	 PlayerInputComponent->BindAxis("TurnRate", this, &ABaseShip::TurnAtRate);

	 PlayerInputComponent->BindAction("WeaponAction", IE_Pressed, this, &ABaseShip::StartWeaponAction);
	 PlayerInputComponent->BindAction("WeaponAction", IE_Released, this, &ABaseShip::StopWeaponAction);

	 PlayerInputComponent->BindAction("WeaponActionWithCam", IE_Pressed, this, &ABaseShip::StartWeaponActionWithCam);
	// PlayerInputComponent->BindAction("WeaponAction", IE_Released, this, &ABaseShip::StartWeaponActionWithCam);
	 
	 PlayerInputComponent->BindAction("WheelUp", IE_Pressed, this, &ABaseShip::StartWheelUp);
	 PlayerInputComponent->BindAction("WheelDown", IE_Pressed, this, &ABaseShip::StartWheelDown);

}

/**MyOwnPawnMovement**/
//UPawnMovementComponent* ABaseShip::GetMovementComponent() const
//{
//	return OurPawnMovement;
//}
/********************Fin Movement****************************************************/


void ABaseShip::AddKey(FName NewKey)
{
	Doorkeys.Add(NewKey);

}


bool ABaseShip::hasKey(FName KeyTag)
{
	return Doorkeys.Contains(KeyTag);
}


void ABaseShip::CreateInitialWeapon()
{
	if (IsValid(InitalWeaponClass))
	{
		CurrentWeapon = GetWorld()->SpawnActor<AMT_Weapon>(InitalWeaponClass, GetActorLocation(), GetActorRotation());//Spawneamos el actor al mundo

		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->SetOwner(this);//Dueno del arma es este personaje
			CurrentWeapon->AttachToComponent(StaticShipComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);//Atachamos el actor(el arma) al mesh
		}
	}

	if (IsValid(InitalWeaponClass2))
	{
		CurrentWeapon2 = GetWorld()->SpawnActor<AMT_Weapon>(InitalWeaponClass2, GetActorLocation(), GetActorRotation());//Spawneamos el actor al mundo

		if (IsValid(CurrentWeapon2))
		{
			CurrentWeapon2->SetOwner(this);//Dueno del arma es este personaje
			CurrentWeapon2->AttachToComponent(StaticShipComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);//Atachamos el actor(el arma) al mesh
			CurrentGatlingCIWS = Cast<AMT_GatlingCIWS>(CurrentWeapon2);
			
		}
	}
}


void ABaseShip::StartWeaponActionWithCam()
{


	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StartActionWithCam();
	}

}

void ABaseShip::StopWeaponAction()
{

	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StopAction();
	}
}


void ABaseShip::StartWeaponAction()
{


	if (IsValid(CurrentWeapon))
	{
		CurrentWeapon->StartAction();
	}

}



void ABaseShip::CheckPosseFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (IsValid(OtherActor))
	{
		OverlappedCharacter = Cast<AMT_Character>(OtherActor);

		if (IsValid(OverlappedCharacter))
		{
			OverlappedCharacter->SetOwner(this);
			OverlappedCharacter->Possessed = true;
			OverlappedCharacter->DisplayText();
		}
	}
	

}

void ABaseShip::CheckPosseEndFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (IsValid(OtherActor))
	{
		OverlappedCharacter = Cast<AMT_Character>(OtherActor);

		if (IsValid(OverlappedCharacter))
		{
			OverlappedCharacter->Possessed = false;
			OverlappedCharacter->HiddeText();
		}
	}

}




void ABaseShip::CheckRadarFromCIWS(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	
	if (IsValid(OtherActor))
	{		

		OverlapLaunchProjectile = Cast<AMT_LaunchProjectile>(OtherActor);

		if (IsValid(OverlapLaunchProjectile) && OverlapLaunchProjectile->GetProjectileType() == EMT_LauncherProjectileType::CharacterType_Enemy)
		{
			TOverlapLaunchProjectile.Add(OverlapLaunchProjectile);
			
			if (TOverlapLaunchProjectile.Num() == 1) //Para evitar que suene de nuevo cuando haga otro everlap
			{	
				
				//bIsOnRadar = true;
				if (IsValid(SoundMoveCIWS))
				{
					SoundMoveCIWS->SetActive(true);

				}	

			}				
		
		}	
	}
}


void ABaseShip::CheckRadarENDFromCIWS(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{	
		OverlapLaunchProjectile = Cast<AMT_LaunchProjectile>(OtherActor);
		if (IsValid(OverlapLaunchProjectile) && OverlapLaunchProjectile->GetProjectileType() == EMT_LauncherProjectileType::CharacterType_Enemy)
		{

			TOverlapLaunchProjectile.Remove(OverlapLaunchProjectile);
			if (TOverlapLaunchProjectile.Num() == 0)
			{

				if (IsValid(AudioMoveCIWSFinal))
				{
					AudioMoveCIWSFinal->SetActive(true);
				}

			}
			
		}

	}

}

/*---- Delegate desde HealtComponent--*/
void ABaseShip::OnHealthChanged(UMT_HealthComponent* CurrentHealthComponent, AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (HealthComponent->IsDead())
	{
		
		if (IsValid(GameModeReference))
		{
			if (IsValid(CurrentGatlingCIWS))
			{
				CurrentGatlingCIWS->AudioSoundFireCIWS->SetActive(false);
			}
			GameModeReference->GameOver(this);
		}

	}

}




void ABaseShip::estadoOverlap()
{	
	//UE_LOG(LogTemp, Warning, TEXT("Mi CurrentRotationPitch es %f"), CurrentRotationPitch);
	UE_LOG(LogTemp, Warning, TEXT("Total de Projectile es %d"), TOverlapLaunchProjectile.Num());


	//for (int i = 0; i < TOverlapLaunchProjectile.Num(); i++)
	//{

	//	UE_LOG(LogTemp, Warning, TEXT("Los Misiles son: %s"), *TOverlapLaunchProjectile[i]->GetName());
	//}

	//if (IsValid(OverlapLaunchProjectile)) {
	//	UE_LOG(LogTemp, Warning, TEXT("Mi Proyectile mas cercano es: %s"), *OverlapLaunchProjectile->GetName());
	//}
	//UE_LOG(LogTemp, Warning, TEXT("mi booleando es %s"), bIsTurretMoveInitial ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, WarningWarning, TEXT("mi booleando es %s"), bIsInRange ? TEXT("true") : TEXT("false"));
	//UE_LOG(LogTemp, Warning, TEXT("---------"));

}


