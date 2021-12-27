// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"


// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UE_LOG (LogTemp, Warning, TEXT ("Initialized InventoryItem Constructor!!"));

	ID = "Default";
	Amount = 1;
	//Item_Data = FItemDataTable();

	// Get ItemDB 
	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDBObject(TEXT("/Game/Blueprints/Item_DB.Item_DB"));
	if (ItemDBObject.Succeeded())
	{
		MyDataTable = ItemDBObject.Object;
		UE_LOG (LogTemp, Warning, TEXT ("ItemDBObject exists!!"));
	}
	
	UE_LOG (LogTemp, Warning, TEXT ("Trying to find the Row!!"));

	// Find Row from ItemDB
	static const FString ContextString(TEXT("test"));
	FItemDataTable* ItemDB_Reference = MyDataTable->FindRow<FItemDataTable>(FName(TEXT("Apple")), ContextString, true);
	if (ItemDB_Reference)
	{
		Item_Data.Amount = ItemDB_Reference->Amount;
		Item_Data.Icon = ItemDB_Reference->Icon;
		UE_LOG (LogTemp, Warning, TEXT ("Our Row exists!!"));
	}

	UE_LOG (LogTemp, Warning, TEXT ("Terminated InventoryItem Constructor!!"));

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

