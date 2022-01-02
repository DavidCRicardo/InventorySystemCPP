// Fill out your copyright notice in the Description page of Project Settings.

#include "Item.h"


// Sets default values
AItem::AItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ID = "Default";
	Amount = 1;
}

AItem::AItem(FName id, uint8 amount, FItemDataTable Data)
{
	ID = id;
	Amount = amount;
	ItemStructure = Data;
}

void AItem::InitItem(FName id, uint8 amount, FItemDataTable Data)
{
	ID = id;
	Amount = amount;
	ItemStructure = Data;
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

