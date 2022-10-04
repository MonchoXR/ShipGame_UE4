// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MT_GameMode.generated.h"

class AMT_BaseShipDestroyed;
class AMT_SpectatingCamera;
/**
 * 
 */
UCLASS()
class MARINETEAM_API AMT_GameMode : public AGameModeBase
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My SpectatingCamera")
	float SpectatingBlendTime;

	UPROPERTY(BlueprintReadOnly, Category = "My SpectatingCamera")
	AMT_SpectatingCamera* VictoryCamera;

	UPROPERTY(BlueprintReadOnly, Category = "My SpectatingCamera")
	AMT_SpectatingCamera* GameOverCamera;


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "My BaseShipDestroyed")
	TSubclassOf<AMT_BaseShipDestroyed> BaseShipDestroyedClass;

protected:

	virtual void BeginPlay() override;
	void SetupSpectatingCameras();

	void MoveCameraToSpectatingPoint(ABaseShip* BaseShip, AMT_SpectatingCamera* SpectatingCamera);
	void MoveCameraToSpectatingPointForGameOver(AMT_BaseShipDestroyed* BaseShipDestroyed, AMT_SpectatingCamera* SpectatingCamera);


public:

	UFUNCTION()
	void Victory();

	UFUNCTION()
	void GameOver(ABaseShip* BaseShip);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_Victory();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_GameOver(ABaseShip* BaseShip);
};
