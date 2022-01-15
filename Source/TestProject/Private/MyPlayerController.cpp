// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"

AMyPlayerController::AMyPlayerController()
{
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void AMyPlayerController::UI_UseInventoryItem_Implementation(const uint8& InventorySlot)
{
	InventoryComponent->UseInventoryItem(InventorySlot);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", IE_Pressed, this, &AMyPlayerController::Interact);
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AMyPlayerController::ToggleInventory);
	InputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AMyPlayerController::ToggleMenu);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InventoryComponent->InitInventory();

	InventoryComponent->InitializeInventoryLayout();

	UE_LOG(LogTemp, Warning, TEXT ("MyPlayerController Initialized!!"))
}

void AMyPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	//APawn* Pawn = GetPawn();
	//HUD_Reference = Cast<AMyHUD>(GetHUD());
	//Character_Reference = Cast<AMyCharacter>(GetPawn());
	//Character_Reference = (InPawn ? Cast<AMyCharacter>(InPawn) : NULL);
}


void AMyPlayerController::ToggleInventory()
{
	InventoryComponent->ToggleInventory();

	if (bShowMouseCursor)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}else
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}
}

void AMyPlayerController::ToggleMenu()
{
	InventoryComponent->AddItem(TEXT("G_Apple"), 1);
	InventoryComponent->RefreshInventoryUI();
	//W_InventoryLayout->RefreshInventorySlots();

	//PrintInventory();
}

void AMyPlayerController::Interact()
{
	if (InventoryComponent->AddItem(TEXT("Apple"), 3))
	{
		//W_InventoryLayout->RefreshInventorySlots();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Added to Inventory")));
		InventoryComponent->RefreshInventoryUI();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Full"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Inventory Full")));
	}

	// Error: Item doesn't exists
	// Error: Not enough space on Inventory
	// Warning: Update InventoryLayout (InventorySlots) if occurs any changes

	// Warning: If Item reaches limit stack if needs to accumulate on another slot
	// Error: Not enough space on Inventory

	//PrintInventory();
}

void AMyPlayerController::InitializeInventoryLayout()
{
	/*if (WidgetClass != nullptr)
	{
		if (W_InventoryLayout == nullptr)
		{
			W_InventoryLayout = CreateWidget<UInventoryLayout>(GetWorld(), WidgetClass);
			W_InventoryLayout->AddToViewport();

			//W_InventoryLayout->SetAlignmentInViewport(FVector2D{0.5,0.5});
			W_InventoryLayout->SetAnchorsInViewport(FAnchors{0.7f, 0.3f});

			//FVector2D Position = FVector2D{0,0};
			//W_InventoryLayout->SetPositionInViewport(Position);

			W_InventoryLayout->SetVisibility(ESlateVisibility::Hidden);
		}
	}*/
}

void AMyPlayerController::PrintInventory()
{
	for (int i = 0; i < InventoryComponent->NumberOfSlots; i++)
	{
		FText a = InventoryComponent->Inventory[i].ItemStructure.Name;
		uint8 b = InventoryComponent->Inventory[i].Amount;
		//uint8 c = W_InventoryLayout->InventorySlotsArray[i]->InventorySlotIndex;

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Item: %s , Amount %i, Index: %i"),
		//	                                 *a.ToString(), b, c));
	}
}

void AMyPlayerController::MoveInventoryItem(const uint8 FromInventorySlot, const uint8 ToInventorySlot)
{

	if (InventoryComponent->MoveInventoryItem(FromInventorySlot, ToInventorySlot))
	{
		InventoryComponent->RefreshInventoryUI();
		//W_InventoryLayout->RefreshInventorySlots();
		//PrintInventory();
	}
}
