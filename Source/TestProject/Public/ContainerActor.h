// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Components/InventoryComponent.h"
#include "ContainerActor.generated.h"

UCLASS()
class INVENTORYSYSTEMCPP_API AContainerActor : public AUsableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AContainerActor();

	virtual bool OnActorUsed_Implementation(APlayerController* Controller) override;

	UPROPERTY()
	UInventoryComponent* InventoryComponent;
	
	UPROPERTY()
	uint8 InventorySize;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	bool InitializeInventory();
	
	UFUNCTION()
	bool LoadInventoryItems(uint8 Size, TArray<FSlotStructure> InventoryItems);
};