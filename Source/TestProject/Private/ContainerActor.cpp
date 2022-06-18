// Fill out your copyright notice in the Description page of Project Settings.



#include "ContainerActor.h"

#include "MyPlayerController.h"

// Sets default values
AContainerActor::AContainerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->NumberOfRowsInventory = 3;
	InventoryComponent->SlotsPerRowInventory = 3;
	
	C_Name = "NULL";
	C_SlotsPerRow = 3;
	C_CanStoreItems = true;
	
	// C_InventorySize = 9;
}

// Called when the game starts or when spawned
void AContainerActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		C_NumberOfRows = InventoryComponent->NumberOfRowsInventory;
		C_SlotsPerRow = InventoryComponent->SlotsPerRowInventory;
		
		C_InventorySize = C_NumberOfRows * C_SlotsPerRow;

		InitializeInventory();
	}
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
				PlayersViewing.Add(PlayerController->PlayerState);
				
				PlayerController->InventoryManagerComponent->Server_UseContainer(this);
				
				return Super::OnActorUsed_Implementation(Controller);
			}
		}
	}
	
	return false;
}

void AContainerActor::GetContainerProperties_Implementation(FName& Namee, uint8& NumberOfRows, uint8& SlotsPerRow, bool& IsStorageContainer,
	uint8& InventorySize)
{
	IInventoryInterface::GetContainerProperties_Implementation(Namee, NumberOfRows, SlotsPerRow, IsStorageContainer, InventorySize);

	Namee = C_Name;
	NumberOfRows = C_NumberOfRows;
	SlotsPerRow = C_SlotsPerRow;
	IsStorageContainer = C_CanStoreItems;
	InventorySize = C_InventorySize;
}

void AContainerActor::GetContainerInventory_Implementation(UInventoryComponent*& OutInventoryComponent)
{
	OutInventoryComponent = InventoryComponent;
	//return InventoryComponent;
}

TArray<APlayerState*> AContainerActor::GetPlayersViewing_Implementation()
{
	return PlayersViewing;
	return IInventoryInterface::GetPlayersViewing_Implementation();
}

bool AContainerActor::InitializeInventory()
{
	if (HasAuthority())
	{
		InventoryComponent->Server_InitInventory(C_InventorySize);

		//LoadInventoryItems(C_InventorySize, InventoryComponent->Inventory);
		
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
