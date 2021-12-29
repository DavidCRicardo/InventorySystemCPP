// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
	// Get ItemDB 
    static ConstructorHelpers::FObjectFinder<UDataTable> ItemObject(TEXT("/Game/Blueprints/Item_DB.Item_DB"));
	if (ItemObject.Succeeded())
	{
		ItemData = ItemObject.Object;
	}
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
	static const FString ContextString(TEXT("TestItem"));
    FItemDataTable* Item = ItemData->FindRow<FItemDataTable>(FName(TEXT("Apple")), ContextString, true);

	UE_LOG (LogTemp, Warning, TEXT ("Terminated InventoryItem Constructor!!"));
	
	/* Clear Inventory */
	Inventory.Empty(numberSlots);
	//Inventory.Reset(numberSlots);        
	
	for(size_t i = 0; i < numberSlots - 1; i++)
	{
		AItem* EmptyItem;
		//EmptyItem->Item_Data.Icon = Item->Icon;
		
		//FCString::Atoi(*);
		//EmptyItem->ID = Item->ID;
		//EmptyItem->Amount = Item->Amount;
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
