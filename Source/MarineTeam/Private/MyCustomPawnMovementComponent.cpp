// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomPawnMovementComponent.h"
UMyCustomPawnMovementComponent::UMyCustomPawnMovementComponent(){
	

	MoveSpeedMovement = 1200.0f;

}
void UMyCustomPawnMovementComponent::TickComponent(float Deltatime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) 
{
	
	Super::TickComponent(Deltatime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(Deltatime))
	{
		return;
	}

	FVector DesireMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f)* MoveSpeedMovement;

	if(!DesireMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesireMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		//If we bump into something, slide along the side of it
		if (Hit.IsValidBlockingHit()) {
			SlideAlongSurface(DesireMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		/*	UE_LOG(LogTemp, Warning, TEXT("Valid Blocking Hit"))*/
		}

	}

}