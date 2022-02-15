// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyHUD.h"
#include "Blueprint/UserWidget.h"

AMyPlayerController::AMyPlayerController()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	// EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", IE_Pressed, this, &AMyPlayerController::Interact);
	InputComponent->BindAction("ToggleProfile", IE_Pressed, this, &AMyPlayerController::ToggleProfile);
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AMyPlayerController::ToggleInventory);
	InputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AMyPlayerController::ToggleMenu);
}

void AMyPlayerController::UI_UseInventoryItem_Implementation(const uint8& InventorySlot)
{
	InventoryComponent->UseInventoryItem(InventorySlot);
	HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
}

void AMyPlayerController::UI_MoveInventoryItem_Implementation(const uint8& FromInventorySlot,
	const uint8& ToInventorySlot)
{
	if (InventoryComponent->MoveInventoryItem(FromInventorySlot, ToInventorySlot))
	{
		HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
	}
}

void AMyPlayerController::UI_EquipInventoryItem_Implementation(const uint8& FromInventorySlot,
	const uint8& ToInventorySlot)
{
	//IInventoryInterface::UI_EquipInventoryItem_Implementation(FromInventorySlot, ToInventorySlot);

	InventoryComponent->EquipItem(FromInventorySlot, ToInventorySlot);
	RefreshWidgets();
}

void AMyPlayerController::UI_UnEquipInventoryItem_Implementation(const uint8& FromInventorySlot,
	const uint8& ToInventorySlot)
{
	IInventoryInterface::UI_UnEquipInventoryItem_Implementation(FromInventorySlot, ToInventorySlot);

	InventoryComponent->UnEquipItem(FromInventorySlot, ToInventorySlot);
	RefreshWidgets();
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUD_Reference = Cast<AMyHUD>(GetHUD());
}

void AMyPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	//APawn* Pawn = GetPawn();
	//Character_Reference = Cast<AMyCharacter>(GetPawn());
	//Character_Reference = (InPawn ? Cast<AMyCharacter>(InPawn) : NULL);
}

void AMyPlayerController::ToggleProfile()
{
	HUD_Reference->ToggleWindow(ELayout::Equipment);
	
	if (HUD_Reference->IsAnyWidgetVisible())
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}else
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void AMyPlayerController::ToggleInventory()
{
	HUD_Reference->ToggleWindow(ELayout::Inventory);

	if (HUD_Reference->IsAnyWidgetVisible())
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}else
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void AMyPlayerController::ToggleMenu()
{
	if(InventoryComponent->AddItem(TEXT("G_Apple"), 1))
	{
		HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
		//PrintInventory();
	}
}

void AMyPlayerController::Interact()
{
	if (InventoryComponent->AddItem(TEXT("Simple_Axe"), 1))
	{
		HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
		//PrintEquipment();
	}
	
	// Warning: Item doesn't exists
	// Warning: Not enough space on Inventory
	
	//PrintInventory();
}

void AMyPlayerController::RefreshWidgets()
{
	HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
	HUD_Reference->RefreshWidgetUILayout(ELayout::Equipment);
}

//void AMyPlayerController::MoveInventoryItem(const uint8 FromInventorySlot, const uint8 ToInventorySlot)
//{
	/*if (ToInventorySlot > InventoryComponent->Inventory.Num() - 1)
	{
		if (FromInventorySlot != ToInventorySlot)
		{
			FSlotStructure LocalSlot = GetItemFrom(InventoryComponent->Inventory, FromInventorySlot);
			FSlotStructure SwapSlot = GetItemFrom(EquipmentComponent->Inventory, ToInventorySlot);

			AddItemToInventoryAndToIndex(EquipmentComponent->Inventory, LocalSlot, ToInventorySlot);
			AddItemToInventoryAndToIndex(InventoryComponent->Inventory, SwapSlot, FromInventorySlot);
			
			HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
			HUD_Reference->RefreshWidgetUILayout(ELayout::Equipment);
		}
		if (EquipmentComponent->MoveInventoryItem(FromInventorySlot, ToInventorySlot))
		{
			HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
			HUD_Reference->RefreshWidgetUILayout(ELayout::Equipment);
			//HUD_Reference->RefreshWidgetUI(1);
			//HUD_Reference->RefreshWidgetUI(2);
		}
	}*/
	
	//if (InventoryComponent->MoveInventoryItem(FromInventorySlot, ToInventorySlot))
	//{
	//	HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
	//}
	
	
	/*if (InventoryComponent->MoveInventoryItem(FromInventorySlot, ToInventorySlot))
	{
		HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
		// PrintInventory();
	}*/
//}

void AMyPlayerController::AddItemToInventoryAndToIndex(TArray<FSlotStructure> Inventory, FSlotStructure& ContentToAdd, const uint8& InventorySlot)
{
	Inventory[InventorySlot] = ContentToAdd;
}

FSlotStructure AMyPlayerController::GetItemFrom(TArray<FSlotStructure> Inventory, const int8& SlotIndex)
{
	return Inventory[SlotIndex];
}

void AMyPlayerController::PrintInventory()
{
	for (int i = 0; i < InventoryComponent->NumberOfSlots; i++)
	{
		FText a = InventoryComponent->Inventory[i].ItemStructure.Name;
		uint8 b = InventoryComponent->Inventory[i].Amount;
		//uint8 c = W_InventoryLayout->InventorySlotsArray[i]->InventorySlotIndex;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Item: %s, Amount %i"),*a.ToString(), b));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Item: %s , Amount %i, Index: %i"), *a.ToString(), b, c));
	}
}

void AMyPlayerController::PrintEquipment()
{
	for (int i = 0; i < EquipmentComponent->NumberOfEquipmentSlots; i++)
	{
		FText a = EquipmentComponent->Inventory[i].ItemStructure.Name;
		uint8 b = EquipmentComponent->Inventory[i].Amount;
		
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Item: %s, Amount %i"),*a.ToString(), b));
	}
}

/*void AMyPlayerController::EquipItemFromInventory(uint8 FromInventorySlot, uint8 ToInventorySlot)
{
	InventoryComponent->EquipFromInventory(FromInventorySlot, ToInventorySlot);
}*/