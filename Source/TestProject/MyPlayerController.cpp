// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> InventoryObj(TEXT("/Game/UI/BP_InventoryLayout"));
	WidgetClass = InventoryObj.Class;

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	//InventoryComponent->bWantsInitializeComponent = true;
	//InventoryComponent->InitializeComponent();
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
		if (W_InventoryLayout == nullptr)
		{
			W_InventoryLayout = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
			W_InventoryLayout->AddToViewport();
			FAnchors Anchors = {0.7f, 0.3f};
			W_InventoryLayout->SetAnchorsInViewport(Anchors);

			W_InventoryLayout->SetVisibility(ESlateVisibility::Hidden);
		}

		if (W_InventoryLayout->GetVisibility() == ESlateVisibility::Hidden)
		{
			InventoryComponent->InitInventory();
			W_InventoryLayout->SetVisibility(ESlateVisibility::Visible);
			
			SetInputMode(FInputModeGameAndUI());
			bShowMouseCursor = true;
		}
		else
		{
			W_InventoryLayout->SetVisibility(ESlateVisibility::Hidden);
			// W_InventoryLayout->RemoveFromParent();
			// W_InventoryLayout = nullptr;
			
			SetInputMode(FInputModeGameOnly());
			bShowMouseCursor = false;
		}
	}
}

void AMyPlayerController::Interact()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Interact!"));

	InventoryComponent->ANewTestingMethod();
	InventoryComponent->ANewTextingMethodNonStatic();
}