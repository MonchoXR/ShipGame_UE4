// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MyCustomPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()


class MARINETEAM_API UMyCustomPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	virtual void TickComponent(float Deltatime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Mis Movimientos") //podemos poner BlueprintReadwrite porque no la variable no es puntero
	float MoveSpeedMovement;


public:
	
	UMyCustomPawnMovementComponent();
};
