// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MT_HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangeSignature, UMT_HealthComponent*, HealthComponent, AActor*, DamagedActor, float, Damage, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(MarineTeam), meta=(BlueprintSpawnableComponent) )
class MARINETEAM_API UMT_HealthComponent : public UActorComponent
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My Debug")
	bool bDebug;

	UPROPERTY(BlueprintReadOnly, Category = "My Health Component")
	bool bIsDead; 

	UPROPERTY(BlueprintReadWrite, Category = "My Health Component")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Health Component", meta = (ClampMin = 0.0, UIMin = 0.0))//meta para clampear los valores en la edicion
	float MaxHealth;

	UPROPERTY(BlueprintReadWrite, Category = "My Health Component")
	AActor* MyOwner;

public:  //Sera llamado por le character para el GameOverz
	UPROPERTY(BlueprintAssignable)//Con esto podemis ver los EVENTS en el editor. Este caso se motrará OnHealthChangeDelegate
	FOnHealthChangeSignature OnHealthChangeDelegate;

public:	
	// Sets default values for this component's properties
	UMT_HealthComponent();

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return bIsDead; }; 

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void TakingDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

};
