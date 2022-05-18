// Fill out your copyright notice in the Description page of Project Settings.



#include "ContainerActor.h"

// Sets default values
AContainerActor::AContainerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventorySize = 4;
}

bool AContainerActor::OnActorUsed_Implementation(APlayerController* Controller)
{
	return Super::OnActorUsed_Implementation(Controller);
}

// Called when the game starts or when spawned
void AContainerActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		InitializeInventory();
	}
}

bool AContainerActor::InitializeInventory()
{
	if (HasAuthority())
	{
		InventoryComponent->Server_InitInventory(InventorySize);
		
		return true;
	}
	
	return false;
}

bool AContainerActor::LoadInventoryItems(uint8 Size, TArray<FSlotStructure> InventoryItems)
{
	if (HasAuthority())
	{
		InventorySize = Size;
		InventoryComponent->LoadInventoryItems(InventorySize, InventoryItems);

		return true;
	}
	
	return false;
}
