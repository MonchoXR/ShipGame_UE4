// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "MT_Button.h"
#include "MT_Cannon.generated.h"

class UStaticMeshComponent;

UCLASS()
class MARINETEAM_API AMT_Cannon : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes") //Permitira visualizar y editar las variables dentro del motor
	USceneComponent* CustomRootComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes")
	UStaticMeshComponent* CannonComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Cannon") //Permita editar en el blueprint y en la scena principal o en lo niveles
	float OpenAngle;
public:	
	// Sets default values for this actor's properties
	AMT_Cannon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "My Cannon")
	void BP_CannonRotator();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "My Cannon")
	void BP_CannonRotatorReverse();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CannonRotator();
	void CannonRotatorReverse();


};
