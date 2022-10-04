// Fill out your copyright notice in the Description page of Project Settings.


#include "MT_Character.h"
#include "BaseShip.h"
#include "Widgets/MT_UMG_EnterBaseShip.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CapsuleComponent.h"
// Sets default values
AMT_Character::AMT_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//CharacterCollisonComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("My CharacterColliderComponent"));
	//CharacterCollisonComponent->SetupAttachment(CharacterCollisonComponent);
	//CharacterCollisonComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);//No fisicas para esta colision	 										
	//CharacterCollisonComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	//CharacterCollisonComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SpringArmCharacter = CreateDefaultSubobject<USpringArmComponent>(TEXT("My SpringArmCharacter"));
	SpringArmCharacter->SetupAttachment(RootComponent);
	SpringArmCharacter->bUsePawnControlRotation = true;

	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("My FPS CameraComponent"));
	FPSCameraComponent->bUsePawnControlRotation = true;
	FPSCameraComponent->SetupAttachment(SpringArmCharacter);

	Possessed = false;
	
}

void AMT_Character::MoveForward(float value)
{
	//AddMovementInput(GetActorForwardVector() * value); //GetActorForwardVector solamente para Actor. AddMovementInput es una funcion propia para character


	if ((Controller != nullptr) && (value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
	}
}

void AMT_Character::MoveRight(float value)
{

	//AddMovementInput(GetActorRightVector() * value);

	if ((Controller != nullptr) && (value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, value);
	}
}



// Called when the game starts or when spawned
void AMT_Character::BeginPlay()
{
	Super::BeginPlay();
	//DisplayText();
}

// Called every frame
void AMT_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AMT_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForwardChar", this, &AMT_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRightChar", this, &AMT_Character::MoveRight);

	PlayerInputComponent->BindAxis("CameraUpChar", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("CameraRightChar", this, &APawn::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Possessed", IE_Pressed, this, &AMT_Character::PossessedInputPlayer);
	//PlayerInputComponent->BindKey(FKey("E"), IE_Pressed, this, &AMT_Character::ControlPlayer);
}


void AMT_Character::PossessedInputPlayer()
{
	if (Possessed)
	{
		AController* PlayerController = GetController();
		if (PlayerController)
		{
			AActor* CurrentOwner = GetOwner();
			ABaseShip* BaseShipOwner = Cast<ABaseShip>(CurrentOwner);
			if (BaseShipOwner)
			{
				//PlayerController->UnPossess();
				PlayerController->Possess(BaseShipOwner);
				HiddeText();
				this->Destroy();
				//UE_LOG(LogTemp, Log, TEXT("Posesion!"));
			}
		}
	}
}

void AMT_Character::DisplayText()
{
	
		if (IsValid(TextUIClass))
		{
			 TextWidget = CreateWidget<UMT_UMG_EnterBaseShip>(GetWorld(), TextUIClass);
			 
			if (IsValid(TextWidget))
			{
				TextWidget->AddToViewport();
			}
		}
		
}

void AMT_Character::HiddeText()
{
	//TextWidget->SetVisibility(ESlateVisibility::Hidden);
	//TextWidget->RemoveFromViewport();
	if (IsValid(TextWidget)) {
		TextWidget->RemoveFromParent();
	}
}
