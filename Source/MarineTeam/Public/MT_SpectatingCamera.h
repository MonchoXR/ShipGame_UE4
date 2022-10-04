// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MT_SpectatingCamera.generated.h"

class UStaticMeshComponent;

//ASi se crea un enum
UENUM(Blueprintable)
enum class EMT_SpectatingCameraType : uint8
{
	CameraType_None		UMETA(DisplayName = "None"), //Siempre se debe empezar un valor por defecto, es este caso le ponemos None
	CameraType_Victory	UMETA(DisplayName = "Victory"),
	CameraType_GameOver UMETA(DisplayName = "GameOver")
};

UCLASS()
class MARINETEAM_API AMT_SpectatingCamera : public AActor
{
	GENERATED_BODY()


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Components")
	UStaticMeshComponent* SpectatingCameraComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "My SpectatingCamera")
	EMT_SpectatingCameraType CameraType;


public:	
	// Sets default values for this actor's properties
	AMT_SpectatingCamera();

public:	


	EMT_SpectatingCameraType GetCameraType() { return CameraType; };
};
