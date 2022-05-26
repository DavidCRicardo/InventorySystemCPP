// Fill out your copyright notice in the Description page of Project Settings.



#include "ContainerActor.h"

#include "MyPlayerController.h"

// Sets default values
AContainerActor::AContainerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	
	C_Name = "NULL";
	C_SlotsPerRow = 4;
	C_CanStoreItems = false;
	C_InventorySize = 4;
}

bool AContainerActor::OnActorUsed_Implementation(APlayerController* Controller)
{
	if (HasAuthority())
	{
		if (IsUsable)
		{
			if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(Controller))
			{
				//Server: Use Container And Add Player To Viewers List
				// Add Player State to Players Viewing
				// PlayerController->PlayerState;
				
				PlayerController->InventoryManagerComponent->Server_UseContainer(this);
				
				return Super::OnActorUsed_Implementation(Controller);
			}
		}
	}
	
	return false;
}

void AContainerActor::GetContainerProperties_Implementation(FName& Namee, uint8& SlotsPerRow, bool& IsStorageContainer,
	uint8& InventorySize)
{
	IInventoryInterface::GetContainerProperties_Implementation(Namee, SlotsPerRow, IsStorageContainer, InventorySize);

	Namee = C_Name;
	SlotsPerRow = C_SlotsPerRow;
	IsStorageContainer = C_CanStoreItems;
	InventorySize = C_InventorySize;
}

UInventoryComponent* AContainerActor::GetContainerInventory_Implementation()
{
	return InventoryComponent;
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
		InventoryComponent->Server_InitInventory(C_InventorySize);
		
		return true;
	}
	
	return false;
}

bool AContainerActor::LoadInventoryItems(uint8 Size, TArray<FSlotStructure> InventoryItems)
{
	if (HasAuthority())
	{
		C_InventorySize = Size;
		InventoryComponent->LoadInventoryItems(C_InventorySize, InventoryItems);

		return true;
	}
	
	return false;
}
