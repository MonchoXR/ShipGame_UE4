// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MT_Weapon.generated.h"

class UDamageType;
class AMT_LaunchProjectile;

UCLASS()
class MARINETEAM_API AMT_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMT_Weapon();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Ojo: aqui no es necesario hacer override y escribir en el hijo porque se definiran en blueprint
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "My Weapon")
	void BP_StartAction();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "My Weapon")
	void BP_StopAction();



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//UFUNCTION(BlueprintCallable)
	UFUNCTION()
	virtual void StartAction(); //Ponermos virtual para que estas funciones puedan los hijos suscribir o reemplazar

	virtual void StartActionWithCam();

	//UFUNCTION(BlueprintCallable)
	virtual void StopAction();


};
