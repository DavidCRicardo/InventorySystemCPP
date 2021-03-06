// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "EquipmentComponent.generated.h"

class AMyCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEMCPP_API UEquipmentComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void ClearInventoryItem(uint8 InventorySlot) override;
	virtual void SetInventoryItem(uint8 InventorySlot, FSlotStructure& Item) override;

	UFUNCTION()
	virtual void UpdateEquippedMeshes(uint8 InventorySlot);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(DisplayName="Equipment Character Reference", Category="Default", OverrideNativeName="EquipmentCharacterReference"))
	AMyCharacter* EquipmentCharacterReference;
};