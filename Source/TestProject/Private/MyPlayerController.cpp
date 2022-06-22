// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "ContainerActor.h"
#include "MyHUD.h"
#include "WorldActor.h"
#include "Blueprint/UserWidget.h"
#include "UI/ContainerLayout.h"

AMyPlayerController::AMyPlayerController()
{
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryComponent"));
	InventoryManagerComponent->SetIsReplicated(true);
	
	PlayerInventoryComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
	
	bReplicates = true;
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

	InputComponent->BindAction("QuitGame", IE_Pressed, this, &AMyPlayerController::QuitGame);

}
void AMyPlayerController::QuitGame()
{
	FGenericPlatformMisc::RequestExit(false);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	CharacterReference = Cast<AMyCharacter>(GetPawn());

	// Server: Initialize Inventory
	PlayerInventoryComponent->EquipmentCharacterReference = CharacterReference;

	InventoryManagerComponent->InitializeInventoryManager(PlayerInventoryComponent);

	// careful here
	InventoryManagerComponent->Server_InitInventory();
	//InventoryManagerComponent->Server_InitInventory_Implementation();

	InventoryManagerComponent->CharacterReference = CharacterReference;

	InventoryManagerComponent->InitializePlayerAttributes();
}

void AMyPlayerController::SetupHUDReferences()
{
	// Client: Setup HUD Reference
	if (AMyHUD* HUDReferenceResult = Cast<AMyHUD>(GetHUD()))
	{
		HUD_Reference = HUDReferenceResult;

		HUDLayoutReference = HUDReferenceResult->HUDReference;
		
		//Client: Init InventoryManagerUI
		InventoryManagerComponent->InitializeInventoryManagerUI(HUDLayoutReference->MainLayout);
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
	InventoryManagerComponent->Server_MoveInventoryItem(FromInventorySlot, ToInventorySlot);
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

void AMyPlayerController::UI_MoveContainerItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{

	RefreshWidgets();
}

void AMyPlayerController::UI_DepositContainerItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
	InventoryManagerComponent->Server_DepositContainerItem(FromInventorySlot, ToInventorySlot);
	RefreshWidgets();
}

void AMyPlayerController::UI_TakeContainerItem_Implementation(const uint8& FromContainerSlot, const uint8& ToInventorySlot)
{
	InventoryManagerComponent->Server_TakeContainerItem(FromContainerSlot, ToInventorySlot);
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
			IUsableActorInterface::Execute_OnActorUsed(Actor, this);
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

void AMyPlayerController::ToggleContainer()
{
	if (IsValid(HUD_Reference))
	{
		HUD_Reference->ToggleWindow(ELayout::Container);
	
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
}

void AMyPlayerController::GetSelectedItemIndex(uint32& Index)
{
	Index = HUD_Reference->HUDReference->TertiaryHUD->InteractiveMenu->GetSelectedItemOnInteractiveList();
}

void AMyPlayerController::Interact()
{
	if (CharacterReference->UsableActorsInsideRange.Num() > 0)
	{
		// Get Selected Item
		uint32 Index = 0;
		GetSelectedItemIndex(Index);
		
		AActor* Actor = CharacterReference->UsableActorsInsideRange[Index];
		//if (CharacterReference->WorldActorsInsideRange.Num() > 0)
		//{
		//	Actor = CharacterReference->WorldActorsInsideRange[Index];
			if (AWorldActor* WorldActor = Cast<AWorldActor>(Actor))
			{
				CollectFromPanel(WorldActor->ID);

				return;
			}
		//}
		
		//if (CharacterReference->UsableActorsInsideRange.Num() > 0)
		//{
		//	Actor = CharacterReference->UsableActorsInsideRange[Index];
			if (AUsableActor* UsableActor = Cast<AUsableActor>(Actor))
			{
				Server_OnActorUsed(UsableActor);

				return;
			}
		//}
	}
}

bool AMyPlayerController::IsContainerOpen()
{
	return HUD_Reference->HUDReference->MainLayout->Container->IsVisible();
}

void AMyPlayerController::CollectFromPanel(const FName& Name)
{
	for (AActor*& Actor : CharacterReference->UsableActorsInsideRange)
	{
		if (AWorldActor* WorldActor = Cast<AWorldActor>(Actor))
		{
			if (WorldActor->ID == Name)
			{
				Server_OnActorUsed(WorldActor);
				
				//HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
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
	if (!IsValid(HUD_Reference))
	{
		return;
	}
	
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

void AMyPlayerController::AddUsableActorToDropMenu(FName IDName)
{
	if (IsValid(HUD_Reference))
	{
		HUD_Reference->HUDReference->TertiaryHUD->CreateInteractiveTextEntry(IDName);

		EnableUIMode();
	}
}

void AMyPlayerController::RemoveUsableActorToDropMenu(const FName& ID)
{
	if (IsValid(HUD_Reference))
	{
		HUD_Reference->HUDReference->TertiaryHUD->RemoveInteractiveTextEntry(ID);
	}
}

void AMyPlayerController::RefreshWidgets()
{
	/*HUD_Reference->RefreshWidgetUILayout(ELayout::Inventory);
	HUD_Reference->RefreshWidgetUILayout(ELayout::Equipment);
	HUD_Reference->RefreshWidgetUILayout(ELayout::Container);*/
}

TArray<uint8> AMyPlayerController::GetPlayerAttributes()
{
	return InventoryManagerComponent->AttributesArray;
}

void AMyPlayerController::RefreshContainerUI(uint8 SlotsPerRow, uint8 NumberOfRows)
{
	HUD_Reference->RefreshContainerSlotsUI(SlotsPerRow, NumberOfRows);
}

/*void AMyPlayerController::PrintInventory()
{
	for (int i = 0; i < InventoryManagerComponent->NumberOfSlots; i++)
	{
		FText a = InventoryManagerComponent->Inventory[i].ItemStructure.Name;
		uint8 b = InventoryManagerComponent->Inventory[i].Amount;
		//uint8 c = W_InventoryLayout->InventorySlotsArray[i]->InventorySlotIndex;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Item: %s, Amount %i"),*a.ToString(), b));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Item: %s , Amount %i, Index: %i"), *a.ToString(), b, c));
	}
}*/

UDataTable* AMyPlayerController::GetItemDB()
{
	return InventoryManagerComponent->GetItemDB();
}

