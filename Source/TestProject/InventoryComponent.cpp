// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG (LogTemp, Warning, TEXT ("Inventory (1/2): Component Initialized!!"));

	NumberOfSlots = 32;
	//InitInventory(NumberOfSlots);
	
	RefreshInventorySlots();
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

/* Initializes the Inventory Array to a Specified Size */    
void UInventoryComponent::InitInventory(const int32 numberSlots)
{
	/* Clear Inventory */
	Inventory.Empty(numberSlots);
	//Inventory.Reset(numberSlots);        
	
	for(size_t i = 0; i < numberSlots - 1; i++)
	{
		AItem* EmptyItem;

		// Get ItemDB 
		static ConstructorHelpers::FObjectFinder<UDataTable> ItemDBObject(TEXT("/Game/Blueprints/Item_DB.Item_DB"));
		// Find Row from ItemDB
		static const FString ContextString(TEXT("test"));
		FItemDataTable* ItemDB_Reference = ItemDBObject.Object->FindRow<FItemDataTable>(FName(TEXT("Apple")), ContextString, true);
		if (ItemDB_Reference)
		{
			//EmptyItem->Item_Data.Amount = ItemDB_Reference->Amount;
			//EmptyItem->Item_Data.Icon = ItemDB_Reference->Icon;
			UE_LOG (LogTemp, Warning, TEXT ("Our Row exists!!"));
		}

		UE_LOG (LogTemp, Warning, TEXT ("Terminated InventoryItem Constructor!!"));
		
		Inventory.Add(EmptyItem);
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Inventory Component : InitInventory!"));

	UE_LOG (LogTemp, Warning, TEXT ("Inventory (2/2): Initialized!!"));
}

void UInventoryComponent::RefreshInventorySlots()
{
	
}

void UInventoryComponent::ANewTestingMethod()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Testing a Static Method!"));
}

void UInventoryComponent::ANewTextingMethodNonStatic()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Testing a Non-Static Method!"));

	InitInventory(32);
}
