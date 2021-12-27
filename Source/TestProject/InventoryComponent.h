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
	
	UFUNCTION()
	void InitInventory(int32 numberSlots);
	void RefreshInventorySlots();
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<AItem*> Inventory;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 NumberOfSlots;

	UFUNCTION()
	static void ANewTestingMethod();
	UFUNCTION()
	void ANewTextingMethodNonStatic();
};
