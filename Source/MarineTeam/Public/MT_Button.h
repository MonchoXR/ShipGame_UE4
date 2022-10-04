// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "MT_Cannon.h"
#include "MT_Button.generated.h"


class UStaticMeshComponent;
class UBoxComponent;
//class UMaterialInterface;
UCLASS()
class MARINETEAM_API AMT_Button : public AActor
{
	GENERATED_BODY()

protected:

	/*Para todo tipo de componente se empieza la letra U*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes") //Permitira visualizar y editar las variables dentro del motor
	USceneComponent* CustomRootComponent;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes")
	//UStaticMeshComponent* DoorFrameComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes")
	UStaticMeshComponent* ButtonComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes")
	UBoxComponent* KeyZoneColliderComponent;



protected:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My Button")
	bool bIsTouch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Button")
	FName KeyTag;

public:	
	// Sets default values for this actor's properties
	AMT_Button();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//creamos una funcion que va bindear a un delegate
	UFUNCTION()
	void CheckKeyFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void CheckKeyEndFromPlayer(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "My Button")
	void BP_ActivaButton();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "My Button")
	void BP_DesactivaButton();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActivaButton();
	void DesactivaButton();


public: //si pongo private porque sale error?

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Mis Materiales")
	UMaterial* StoredMaterialAct;

	UPROPERTY(EditAnywhere, BlueprintReadwrite, Category = "Mis Materiales")
	UMaterial* StoredMaterialDes;


	UMaterialInstanceDynamic* DynamicMaterialAct;
	UMaterialInstanceDynamic* DynamicMaterialInstDes;
	
};
