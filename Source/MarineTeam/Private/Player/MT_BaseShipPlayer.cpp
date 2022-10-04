// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MT_BaseShipPlayer.h"
#include "MT_SpectatingCamera.h"

AMT_BaseShipPlayer::AMT_BaseShipPlayer()
{
	SceneSpectatingGameOverCamera = CreateDefaultSubobject<USceneComponent>(TEXT("My SceneSpectatingGameOverCamera"));
	SceneSpectatingGameOverCamera->SetupAttachment(RootComponent);
}

void AMT_BaseShipPlayer::BeginPlay()
{
	Super::BeginPlay();
	CreateInitialSpectatingCamera();
}
void AMT_BaseShipPlayer::CreateInitialSpectatingCamera()
{

	if (IsValid(GameOverCameraClass))
	{
		GameOverCamera = GetWorld()->SpawnActor<AMT_SpectatingCamera>(GameOverCameraClass, GetActorLocation(), GetActorRotation());//Spawneamos el actor al mundo

		if (IsValid(GameOverCamera))
		{
			GameOverCamera->SetOwner(this);//Dueno del arma es este personaje
			GameOverCamera->AttachToComponent(SceneSpectatingGameOverCamera, FAttachmentTransformRules::SnapToTargetNotIncludingScale);//Atachamos el actor(el arma) al mesh
		}
	}

}