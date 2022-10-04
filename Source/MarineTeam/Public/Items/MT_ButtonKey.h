// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/MT_Item.h"
#include "MT_ButtonKey.generated.h"

class UStaticMeshComponent;

/**
 * 
 */
UCLASS()
class MARINETEAM_API AMT_ButtonKey : public AMT_Item
{
	GENERATED_BODY()


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mis Componentes")
	UStaticMeshComponent* KeyMeshComponent;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mi Key")
	FName KeyTag;//Es como un string mejorado para poner nombres o tags

public:
	// Sets default values for this actor's properties
	AMT_ButtonKey();

public:
	UFUNCTION(BlueprintCallable, Category = "Mi Getter")
    FName GetKeyTag() const { return KeyTag; }; //por tema de optimizacion ponemos const para que siempre me devuelta el mismo valor y no me genere duplicado en memoria.
													//Para funciones que retornar algun valor puede defenirse aqui

protected:

	virtual void Pickup(ABaseShip* PickupActor) override;
	
};
