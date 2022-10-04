// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/MT_GameMode.h"
#include "BaseShip.h"
#include "MT_BaseShipDestroyed.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h" //Para obtener todos los actores en una sola clase
#include "MT_SpectatingCamera.h"
#include "Player/MT_BaseShipPlayer.h"

void AMT_GameMode::BeginPlay()
{
	Super::BeginPlay();
	SetupSpectatingCameras();

}

void AMT_GameMode::SetupSpectatingCameras()
{

	TArray<AActor*> SpectatingArrayCameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMT_SpectatingCamera::StaticClass(), SpectatingArrayCameraActors);//obtenemos todos los actores de la clase SpectatingCamera

	if (SpectatingArrayCameraActors.Num() > 0)
	{

		for (AActor* SpectatingActor : SpectatingArrayCameraActors)
		{
			AMT_SpectatingCamera* SpectatingCamera = Cast<AMT_SpectatingCamera>(SpectatingActor);
			if (IsValid(SpectatingCamera))
			{
				switch (SpectatingCamera->GetCameraType())
				{
				case EMT_SpectatingCameraType::CameraType_Victory:
					VictoryCamera = SpectatingCamera;
					break;

				case EMT_SpectatingCameraType::CameraType_GameOver:
					GameOverCamera = SpectatingCamera;
					break;
				default:
					break;
				}
			}

		}

	}




}

void AMT_GameMode::MoveCameraToSpectatingPoint(ABaseShip* BaseShip, AMT_SpectatingCamera* SpectatingCamera)
{
	if (!IsValid(BaseShip) || !IsValid(SpectatingCamera))
	{
		return;
	}
	AController* BaseShipController = BaseShip->GetController();
	if (IsValid(BaseShipController))
	{
		//Vamos a castear porque los controladores pueden ser de AI o del Jugador, queremis del jugador
		APlayerController* PlayerController = Cast<APlayerController>(BaseShipController);
		if (IsValid(PlayerController))
		{
			PlayerController->SetViewTargetWithBlend(SpectatingCamera, SpectatingBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);//Esta funcion mueve la camara del controlador a un punto halladamos detectado como target

		}
	}

}

void AMT_GameMode::MoveCameraToSpectatingPointForGameOver(AMT_BaseShipDestroyed* BaseShipDestroyed, AMT_SpectatingCamera* SpectatingCamera)
{
	if (!IsValid(BaseShipDestroyed) || !IsValid(SpectatingCamera))
	{
		return;
	}
	AController* BaseShipController = BaseShipDestroyed->GetController();
	if (IsValid(BaseShipController))
	{
		//Vamos a castear porque los controladores pueden ser de AI o del Jugador, queremis del jugador
		APlayerController* PlayerController = Cast<APlayerController>(BaseShipController);
		if (IsValid(PlayerController))
		{
			//PlayerController->SetViewTargetWithBlend(SpectatingCamera, SpectatingBlendTime, EViewTargetBlendFunction::VTBlend_Cubic);//Esta funcion mueve la camara del controlador a un punto halladamos detectado como target
			PlayerController->SetViewTargetWithBlend(SpectatingCamera, SpectatingBlendTime);//Esta funcion mueve la camara del controlador a un punto halladamos detectado como target

		}
	}

}




void AMT_GameMode::Victory()
{
}



void AMT_GameMode::GameOver(ABaseShip* BaseShip)
{
	//BaseShip->GetMovementComponent()->StopMovementImmediately();
	//BaseShip->DetachFromControllerPendingDestroy();

	//BaseShip->Destroy();
	//	BaseShip->SetLifeSpan(5.0f);


	if (IsValid(BaseShipDestroyedClass))
	{
		//AMT_BaseShipPlayer* BaseShipPlayer = Cast<AMT_BaseShipPlayer>(BaseShip);
		
	
			AController* BaseShipController = BaseShip->GetController();

			if (IsValid(BaseShipController))
			{
				APlayerController* BaseShipPlayerController = Cast<APlayerController>(BaseShipController);

				if (IsValid(BaseShipPlayerController))
				{
					AMT_BaseShipDestroyed* BaseDestroyedCurrent = GetWorld()->SpawnActor<AMT_BaseShipDestroyed>(BaseShipDestroyedClass, BaseShip->GetActorLocation(), BaseShip->GetActorRotation());

					if (IsValid(BaseDestroyedCurrent))
					{

						BaseShipPlayerController->Possess(BaseDestroyedCurrent);
						
						AMT_BaseShipPlayer* BaseShipPlayer = Cast<AMT_BaseShipPlayer>(BaseShip);
						if (IsValid(BaseShipPlayer))
						{
							MoveCameraToSpectatingPointForGameOver(BaseDestroyedCurrent, BaseShipPlayer->GameOverCamera);
						}
						
						BaseShip->Destroy();
						BaseDestroyedCurrent->SoundExplosion->SetActive(true);
					//	MoveCameraToSpectatingPointForGameOver(BaseDestroyedCurrent, GameOverCamera);
					}

				}

				else
				{
					AMT_BaseShipDestroyed* BaseDestroyedCurrent = GetWorld()->SpawnActor<AMT_BaseShipDestroyed>(BaseShipDestroyedClass, BaseShip->GetActorLocation(), BaseShip->GetActorRotation());
					BaseShip->Destroy();
					BaseDestroyedCurrent->SoundExplosion->SetActive(true);
				}
			}
		


	}

	
	

	BP_GameOver(BaseShip);
}
