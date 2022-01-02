// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	//UFUNCTION()
	//void InitInventory(int32 numberSlots);
	//void RefreshInventorySlots();
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AItem*> Inventory;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 NumberOfSlots;
	
	FItemDataTable* ItemDB_Reference;
	
	void InitInventory(int32 NumberSlots = 32);
	
	UFUNCTION()
	bool AddItem(FName ID, uint8 Amount);
	
	FItemDataTable* GetItemDB() const;

protected:
	
private:
	UPROPERTY()
	UDataTable* ItemDB;

	UFUNCTION()
	bool HasEmptySlots();
	UFUNCTION()
	bool CreateStack();
	UFUNCTION()
	bool AddToStack();
	UFUNCTION()
	int8 HasPartialStack(AItem* SlotStructure);
	
};
