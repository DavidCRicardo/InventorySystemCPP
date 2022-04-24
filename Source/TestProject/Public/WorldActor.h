// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Inventory/FSlotStructure.h"
#include "WorldActor.generated.h"

UCLASS()
class INVENTORYSYSTEMCPP_API AWorldActor : public AUsableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWorldActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool StartWithPhysicsEnabled;

	
	UFUNCTION()
	bool LoadItemFromList();
	UFUNCTION()
	void UpdateItemAmount();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_InitializeItemData();
	bool Server_InitializeItemData_Validate();
	void Server_InitializeItemData_Implementation();
	
	virtual bool OnActorUsed_Implementation(APlayerController* Controller) override;

	void OnRep_WorldMesh();

private:
	UPROPERTY()
	FSlotStructure InventoryItem;

	UPROPERTY()
	bool IsSinglePlayer;
};
