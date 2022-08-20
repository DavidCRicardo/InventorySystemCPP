// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryInterface.h"
#include "UsableActor.h"
#include "Components/InventoryComponent.h"
#include "ContainerActor.generated.h"

UCLASS()
class INVENTORYSYSTEMCPP_API AContainerActor : public AUsableActor, public IInventoryInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AContainerActor();
	
	/* */
	virtual bool OnActorUsed_Implementation(APlayerController* Controller) override;
	
	/* Interface */
	virtual void GetContainerProperties_Implementation(FName& Namee, uint8& NumberOfRows, uint8& SlotsPerRow, bool& IsStorageContainer, uint8& InventorySize) override;
	virtual void GetContainerInventory_Implementation(UInventoryComponent*& OutInventoryComponent) override;
	virtual TArray<APlayerState*> GetPlayersViewing_Implementation() override;
	virtual bool ContainerLooted_Implementation() override;
	/* Ends Interface*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<APlayerState*> PlayersViewing;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	virtual bool InitializeInventory();
	
	UFUNCTION()
	bool LoadInventoryItems(uint8 Size, TArray<FSlotStructure> InventoryItems);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Name", Category="Properties"))
	FName C_Name;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Slots Per Row", Category="Properties"))
	uint8 C_NumberOfRows;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Slots Per Row", Category="Properties"))
	uint8 C_SlotsPerRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Can Store Items", Category="Properties"))
	bool C_CanStoreItems;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Inventory Size", Category="Properties"))
	uint8 C_InventorySize;
};