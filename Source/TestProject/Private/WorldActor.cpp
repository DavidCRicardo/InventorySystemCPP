// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActor.h"
#include "MyPlayerController.h"
#include "Components/EquipmentComponent.h"
#include "Components/InventoryManagerComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AWorldActor::AWorldActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ID = FName(TEXT("None"));
	Amount = 1;
	StartWithPhysicsEnabled = true;

	// Get ItemDB 
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_ItemDB(TEXT("/Game/Blueprints/Item_DB.Item_DB"));
	if (BP_ItemDB.Succeeded())
	{
		ItemDB = BP_ItemDB.Object;
	}else{
		UE_LOG(LogTemp, Warning, TEXT ("ItemDB DataTable not found!!"));
	}
}

// Called when the game starts or when spawned
void AWorldActor::BeginPlay()
{
	Super::BeginPlay();
	
	Server_InitializeItemData();
}

// Called every frame
void AWorldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorldActor::Server_InitializeItemData_Implementation()
{
	LoadItemFromList();
}

bool AWorldActor::LoadItemFromList()
{
	FItemStructure* NewItemData = ItemDB->FindRow<FItemStructure>(FName(ID), "", true);

	if (!NewItemData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading Item... returned NULL."));
		return false;
	}

	InventoryItem.InitSlot(*NewItemData, Amount);
	
	InventoryItem.Amount = Amount;
	InventoryItem.ItemStructure = *NewItemData;

	return true;
}

bool AWorldActor::OnActorUsed_Implementation(APlayerController* Controller)
{
	if (HasAuthority())
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(Controller))
		{
			bool OutSuccess = false;
			UInventoryComponent* InventoryComp = PlayerController->PlayerInventoryComponent;
			
			PlayerController->InventoryManagerComponent->TryToAddItemToInventory(InventoryComp, InventoryItem, OutSuccess);

			if (OutSuccess)
			{
				Destroy();
			}
			
			return Super::OnActorUsed_Implementation(Controller);
		}
	}
	
	return false;
}

void AWorldActor::OnRep_WorldMesh()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("OnRep_WorldMesh being called"));

	StaticMesh->SetStaticMesh(WorldMesh);
}

void AWorldActor::UpdateItemAmount()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("UpdateItemAmount being called"));
}