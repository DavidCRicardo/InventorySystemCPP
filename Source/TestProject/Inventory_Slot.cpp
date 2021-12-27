// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory_Slot.h"
#include "Components/Image.h"

UInventory_Slot::UInventory_Slot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> ObjectFind(TEXT("/Game/Textures/T_UI_Slot"));
	Background_Slot = ObjectFind.Object;
}

void UInventory_Slot::NativeConstruct()
{
	Super::NativeConstruct();
	
	UE_LOG (LogTemp, Warning, TEXT ("InventorySlot Initialized!! (1/2)"));
	
	InitializeVariables();
}

void UInventory_Slot::InitializeVariables()
{
	if (ItemInfo == nullptr)
	{
		Amount->SetText(FText::FromString(""));
		Background->SetBrushFromTexture(Background_Slot);

		UE_LOG (LogTemp, Warning, TEXT ("InventorySlot Defining Default Values"));
	}
	else
	{
		UE_LOG (LogTemp, Warning, TEXT ("InventorySlot Defining Item Values"));
	}
	
	
	UE_LOG (LogTemp, Warning, TEXT ("InventorySlot Initialized!! (2/2)"));
}

void UInventory_Slot::SetAmountText(const FText& _newAmount)
{
	Amount->SetText(_newAmount);
}

FText UInventory_Slot::GetAmountText()
{
	return Amount->GetText();
}