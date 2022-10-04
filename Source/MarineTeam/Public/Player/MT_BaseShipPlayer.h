// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseShip.h"
#include "MT_BaseShipPlayer.generated.h"

class AMT_SpectatingCamera;
/**
 * 
 */
UCLASS()
class MARINETEAM_API AMT_BaseShipPlayer : public ABaseShip
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMT_BaseShipPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My SpectatingCamera")
	TSubclassOf<AMT_SpectatingCamera> GameOverCameraClass; //Referencia Cuando existe en el contenedor Browser


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes")
	USceneComponent* SceneSpectatingGameOverCamera;

protected:
	void CreateInitialSpectatingCamera();

public:
	
	UPROPERTY(BlueprintReadWrite, Category = "My SpectatingCamera")
	AMT_SpectatingCamera* GameOverCamera; //Referencia Cuando ya existe en escena

};
