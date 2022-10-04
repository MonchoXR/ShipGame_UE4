// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "MT_Character.generated.h"

class ABaseShip;
class UCameraComponent;
class UMT_UMG_EnterBaseShip;
class USpringArmComponent;
class UCapsuleComponent;

UCLASS()
class MARINETEAM_API AMT_Character : public ACharacter
{
	GENERATED_BODY()

public:

	bool Possessed;


	
protected:

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Components")
	//UCapsuleComponent* CharacterCollisonComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes")
	UCameraComponent* FPSCameraComponent;

	//////////Para UMG//////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My UI")
	TSubclassOf<UMT_UMG_EnterBaseShip> TextUIClass;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My UI")
	UMT_UMG_EnterBaseShip* TextWidget;
	////////Para UMG//////////////

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Components")
	USpringArmComponent* SpringArmCharacter;


public:
	// Sets default values for this character's properties
	AMT_Character();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value); 
	void MoveRight(float value);
	void PossessedInputPlayer();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DisplayText();
	void HiddeText();
};
