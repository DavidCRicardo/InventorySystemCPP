// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/FSlotStructure.h"
#include "InventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INVENTORYSYSTEMCPP_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UDataTable* GetItemDB();

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	TArray<FSlotStructure> Inventory;
	
	UFUNCTION(Server, Reliable)
	void Server_InitInventory(const uint8& Size);
	UFUNCTION()
	void InitInventory(const uint8& Size);

	UFUNCTION()
	bool LoadInventoryItems(uint8 Size, TArray<FSlotStructure> Array);
	
	UFUNCTION()
	void GetInventoryItems(TArray<FSlotStructure>& InventoryItems);

	UFUNCTION()
	void SetInventoryItem(uint8& Index, FSlotStructure& Item);
	
};