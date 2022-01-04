// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "UI/InventoryLayout.h"
#include "Blueprint/UserWidget.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryObj(TEXT("/Game/UI/WBP_InventoryLayout"));
	WidgetClass = InventoryObj.Class;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", IE_Pressed, this, &AMyPlayerController::Interact);
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AMyPlayerController::ToggleInventory);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InventoryComponent->InitInventory();

	InitializeInventoryLayout();

	UE_LOG (LogTemp, Warning, TEXT ("MyPlayerController Initialized!!"))
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
	if (WidgetClass != nullptr)
	{
		if (W_InventoryLayout->GetVisibility() == ESlateVisibility::Hidden)
		{
			W_InventoryLayout->SetVisibility(ESlateVisibility::Visible);
			
			SetInputMode(FInputModeGameAndUI());
			bShowMouseCursor = true;
		}
		else
		{
			W_InventoryLayout->SetVisibility(ESlateVisibility::Hidden);
			
			SetInputMode(FInputModeGameOnly());
			bShowMouseCursor = false;
		}
	}
}

void AMyPlayerController::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Interact!"));
	
	if(
		InventoryComponent->AddItem(TEXT("Apple"), 2))
	{
		W_InventoryLayout->RefreshInventorySlots(); //Needs Debug
	}

	bool HasEmptySlots = false;
	if (HasEmptySlots)
	{
		
	}
	
	FItemStructure* ItemExists{};
	if (ItemExists)
	{
		
	}else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item doesn't exists on Inventory"));
		UE_LOG(LogTemp, Warning, TEXT("Add or Create Stack"));
	}
	// Error: Item doesn't exists
	// Error: Not enough space on Inventory
	// Warning: Update InventoryLayout (InventorySlots) if occurs any changes

	// Warning: If Item reaches limit stack if needs to accumulate on another slot
	// Error: Not enough space on Inventory
}

void AMyPlayerController::InitializeInventoryLayout()
{
	if (WidgetClass != nullptr)
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
	}
}