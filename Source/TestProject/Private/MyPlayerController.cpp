// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyHUD.h"
#include "WorldActor.h"
#include "Blueprint/UserWidget.h"

AMyPlayerController::AMyPlayerController()
{
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryComponent"));
	InventoryManagerComponent->SetIsReplicated(true);
	
	//PlayerInventoryComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
}

void AMyPlayerController::TestMethod()
{
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Interact", IE_Pressed, this, &AMyPlayerController::Interact);
	InputComponent->BindAction("ToggleProfile", IE_Pressed, this, &AMyPlayerController::ToggleProfile);
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AMyPlayerController::ToggleInventory);
	InputComponent->BindAction("ToggleMenu", IE_Pressed, this, &AMyPlayerController::ToggleMenu);

	InputComponent->BindAction("ToggleUIMode", IE_Pressed, this, &AMyPlayerController::EnableUIMode);
	InputComponent->BindAction("ToggleUIMode", IE_Released, this, &AMyPlayerController::DisableUIMode);

	InputComponent->BindAction("TestKey", IE_Pressed, this, &AMyPlayerController::TestMethod);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CharacterReference = Cast<AMyCharacter>(GetPawn());
	InventoryManagerComponent->CharacterReference = CharacterReference;
	
	if (AMyHUD* HUDReferenceResult = Cast<AMyHUD>(GetHUD()))
	{
		HUD_Reference = HUDReferenceResult;
		if (UHUDLayout* HUDResult = Cast<UHUDLayout>(HUD_Reference->HUDLayoutReference))
		{
			HUDLayoutReference = HUDResult;

			MainHUD = HUDLayoutReference->MainLayout;
		}
	}
}

void AMyPlayerController::UI_UseInventoryItem_Implementation(const uint8& InventorySlot)
{
	InventoryManagerComponent->UseInventoryItem(InventorySlot);
	
	HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
	HUD_Reference->RefreshWidgetUILayout(ELayout::Equipment);
}

void AMyPlayerController::UI_MoveInventoryItem_Implementation(const uint8& FromInventorySlot,
	const uint8& ToInventorySlot)
{
	if (InventoryManagerComponent->MoveInventoryItem(FromInventorySlot, ToInventorySlot))
	{
		HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
	}
}

void AMyPlayerController::UI_DropInventoryItem_Implementation(const uint8& InventorySlot)
{
	IInventoryInterface::UI_DropInventoryItem_Implementation(InventorySlot);

	InventoryManagerComponent->Server_DropItemFromInventory_Implementation(InventorySlot);
	RefreshWidgets();
}

void AMyPlayerController::UI_EquipInventoryItem_Implementation(const uint8& FromInventorySlot,
                                                               const uint8& ToInventorySlot)
{
	IInventoryInterface::UI_EquipInventoryItem_Implementation(FromInventorySlot, ToInventorySlot);

	InventoryManagerComponent->Server_EquipFromInventory_Implementation(FromInventorySlot, ToInventorySlot);
	RefreshWidgets();
}

void AMyPlayerController::UI_UnEquipInventoryItem_Implementation(const uint8& FromInventorySlot,
	const uint8& ToInventorySlot)
{
	IInventoryInterface::UI_UnEquipInventoryItem_Implementation(FromInventorySlot, ToInventorySlot);

	InventoryManagerComponent->Server_UnEquipFromInventory_Implementation(FromInventorySlot, ToInventorySlot);
	RefreshWidgets();
}

void AMyPlayerController::Server_OnActorUsed_Implementation(AActor* Actor)
{
	OnActorUsed(Actor);
}
void AMyPlayerController::OnActorUsed(AActor* Actor)
{
	if (HasAuthority())
	{
		if (IsValid(Actor))
		{
			if(AWorldActor* WorldActor = Cast<AWorldActor>(Actor))
			{
				WorldActor->OnActorUsed_Implementation(this);
				
				//InventoryManagerComponent->Server_RefreshInventorySlots();
				//InventoryManagerComponent->AddItem(WorldActor->ID, WorldActor->Amount);
			}
		}
	}
}

void AMyPlayerController::Server_OnActorDropped_Implementation(FSlotStructure LocalSlot)
{
	OnActorDropped(LocalSlot);
}

void AMyPlayerController::OnActorDropped(FSlotStructure LocalSlot)
{
	UClass* LocalClass = LocalSlot.ItemStructure.Class;

	// Drop at character feet
	FVector LocalLocation {0.0f, 0.0f, -98.0f};
			
	FVector PawnLocation = GetPawn()->GetActorLocation();

	//Drop Distance Range From Character
	//const uint8 DropDistanceRangeX = FMath::RandRange(64, 96);
	const uint8 DropDistanceRangeX = FMath::RandRange(75, 100);
	FVector DistanceFromPawn {(float)DropDistanceRangeX,1.0f,1.0f};

	// Drop Items 360 Degrees Around Player
	const float DropItemsRotation = FMath::FRandRange(-180, 180);
	//FRotator Rotation {1.0f, DropItemsRotation, DropItemsRotation}; // Drop Around Player
	FRotator Rotation {1.0f, 1.0f, DropItemsRotation}; // Drop In One Point

	FVector VectorRotated = Rotation.RotateVector(DistanceFromPawn);

	FVector FinalLocation = PawnLocation + LocalLocation + VectorRotated; 

	// Give The Dropped Object A Random Rotation
	// const int8 RandomRotation = FMath::RandRange(-10, 10);
	// FRotator FinalRotator {0.0f, 0.0f, (float)RandomRotation * 10};
	FRotator FinalRotator {1.0f, 1.0f, 1.0f};

	FVector FinalScale {1.0f,1.0f,1.0f};
		
	FTransform OutTransform {};
	OutTransform = FTransform(FinalRotator, FinalLocation, FinalScale);
	
	// Spawn World Actor
	AWorldActor* WActor = GetWorld()->SpawnActor<AWorldActor>(LocalClass, OutTransform);
	if (WActor)
	{
		WActor->Amount = LocalSlot.Amount;
	}
}

void AMyPlayerController::ToggleProfile()
{
	if (IsValid(HUD_Reference))
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
}

void AMyPlayerController::ToggleInventory()
{
	if (IsValid(HUD_Reference))
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
}

void AMyPlayerController::ToggleMenu()
{
	if(InventoryManagerComponent->AddItem(TEXT("Cardboard_Chest"), 1))
	{
		InventoryManagerComponent->AddItem(TEXT("Cardboard_Boots"), 1);
		InventoryManagerComponent->AddItem(TEXT("Cardboard_Gloves"), 1);
		
		HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
		PrintInventory();
	}
}

void AMyPlayerController::GetSelectedItemIndex(uint32& Index)
{
	Index = HUD_Reference->HUDLayoutReference->TertiaryHUD->InteractiveMenu->GetSelectedItemOnInteractiveList();
}

void AMyPlayerController::Interact()
{
	if (CharacterReference->UsableActorsInsideRange.Num() > 0)
	{
		// Get Selected Item
		uint32 Index = 0;
		GetSelectedItemIndex(Index);
		
		AActor* Actor = CharacterReference->UsableActorsInsideRange[Index];
		
		if (AWorldActor* WorldActor = Cast<AWorldActor>(Actor))
		{
			CollectFromPanel(WorldActor->ID);

			HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
		}
	}
}

void AMyPlayerController::UseWorldActor(AWorldActor* WorldActor)
{
	Server_OnActorUsed(WorldActor);

	InventoryManagerComponent->AddItem(WorldActor->ID, WorldActor->Amount);
}

void AMyPlayerController::CollectFromPanel(const FName& Name)
{
	for (AActor*& Actor : CharacterReference->UsableActorsInsideRange)
	{
		if (AWorldActor* WorldActor = Cast<AWorldActor>(Actor))
		{
			if (WorldActor->ID == Name)
			{
				UseWorldActor(WorldActor);
				return;
			}
		}
	}
}

UUserWidget* AMyPlayerController::GetInteractWidget()
{
	return HUD_Reference->GetInteractWidget();
}

void AMyPlayerController::SetMouseToCenterPosition()
{
	int32 SizeX;
	int32 SizeY;
	GetViewportSize(SizeX, SizeY);

	SetMouseLocation(SizeX / 2, SizeY / 2);
}

void AMyPlayerController::EnableUIMode()
{
	SetInputMode(FInputModeGameAndUI());
	bShowMouseCursor = true;

	SetMouseToCenterPosition();
}

void AMyPlayerController::DisableUIMode()
{
	if (HUD_Reference->IsAnyWidgetVisible())
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;
	}else
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
	//SetInputMode(FInputModeGameOnly());
	//bShowMouseCursor = false;
}

void AMyPlayerController::AddUsableActorToDropMenu(FName IDName)
{
	if (IsValid(HUD_Reference))
	{
		HUD_Reference->HUDLayoutReference->TertiaryHUD->CreateInteractiveTextEntry(IDName);

		EnableUIMode();
	}
}

void AMyPlayerController::RemoveUsableActorToDropMenu(const FName& ID)
{
	if (IsValid(HUD_Reference))
	{
		HUD_Reference->HUDLayoutReference->TertiaryHUD->RemoveInteractiveTextEntry(ID);
	}
}

void AMyPlayerController::RefreshWidgets()
{
	HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
	HUD_Reference->RefreshWidgetUILayout(ELayout::Equipment);
}

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
	for (int i = 0; i < InventoryManagerComponent->NumberOfSlots; i++)
	{
		FText a = InventoryManagerComponent->Inventory[i].ItemStructure.Name;
		uint8 b = InventoryManagerComponent->Inventory[i].Amount;
		//uint8 c = W_InventoryLayout->InventorySlotsArray[i]->InventorySlotIndex;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Item: %s, Amount %i"),*a.ToString(), b));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Item: %s , Amount %i, Index: %i"), *a.ToString(), b, c));
	}
}

UDataTable* AMyPlayerController::GetItemDB()
{
	return InventoryManagerComponent->GetItemDB();
}