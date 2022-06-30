// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

#include "MyCharacter.h"
#include "MyHUD.h"
#include "WorldActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/EquipmentComponent.h"
#include "Components/InventoryManagerComponent.h"
#include "UI/ContainerLayout.h"
#include "UI/HUDLayout.h"
#include "UI/InteractiveText_Panel.h"
#include "UI/MainLayout.h"
#include "UI/TertiaryHUD.h"

AMyPlayerController::AMyPlayerController()
{
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryComponent"));
	InventoryManagerComponent->SetIsReplicated(true);
	
	PlayerInventoryComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));

	CharacterReference = nullptr;
	
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

	DisableInput(this);

	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Server")));
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Client")));
	}

	// Delay: 1 second
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		UE_LOG(LogTemp, Warning, TEXT("This text will appear in the console 3 seconds after execution"))

		CharacterReference = Cast<AMyCharacter>(GetPawn());

		PlayerInventoryComponent->EquipmentCharacterReference = CharacterReference;
		InventoryManagerComponent->InitializeInventoryManager(PlayerInventoryComponent);
		
		// Server: Initialize Inventory
		InventoryManagerComponent->Server_InitInventory();

		InventoryManagerComponent->InitializePlayerAttributes();

		EnableInput(this);
	}, 1, false);
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

void AMyPlayerController::UI_MoveInventoryItem_Implementation(const uint8& FromInventorySlot,
                                                              const uint8& ToInventorySlot)
{
	InventoryManagerComponent->Server_MoveInventoryItem(FromInventorySlot, ToInventorySlot);
}

void AMyPlayerController::UI_MoveContainerItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
	InventoryManagerComponent->Server_MoveContainerItem(FromInventorySlot, ToInventorySlot);
}

void AMyPlayerController::UI_DepositContainerItem_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
	InventoryManagerComponent->Server_DepositContainerItem(FromInventorySlot, ToInventorySlot);
}

void AMyPlayerController::UI_TakeContainerItem_Implementation(const uint8& FromContainerSlot, const uint8& ToInventorySlot)
{
	InventoryManagerComponent->Server_TakeContainerItem(FromContainerSlot, ToInventorySlot);
}

void AMyPlayerController::UI_EquipInventoryItem_Implementation(const uint8& FromInventorySlot,
                                                               const uint8& ToInventorySlot)
{
	InventoryManagerComponent->Server_EquipFromInventory(FromInventorySlot, ToInventorySlot);
}

void AMyPlayerController::UI_UnEquipInventoryItem_Implementation(const uint8& FromInventorySlot,
	const uint8& ToInventorySlot)
{
	InventoryManagerComponent->Server_UnEquipToInventory(FromInventorySlot, ToInventorySlot);
}

void AMyPlayerController::UI_DropInventoryItem_Implementation(const uint8& InventorySlot)
{
	InventoryManagerComponent->Server_DropItemFromInventory(InventorySlot);
}

void AMyPlayerController::UI_UseInventoryItem_Implementation(const uint8& InventorySlot)
{
	InventoryManagerComponent->Server_UseInventoryItem(InventorySlot);
}

void AMyPlayerController::UI_UseContainerItem_Implementation(const uint8& InventorySlot)
{
	InventoryManagerComponent->Server_UseContainerItem(InventorySlot);
}

void AMyPlayerController::UI_EquipFromContainer_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
	InventoryManagerComponent->Server_EquipFromContainer(FromInventorySlot, ToInventorySlot);
}

void AMyPlayerController::UI_UnEquipToContainer_Implementation(const uint8& FromInventorySlot, const uint8& ToInventorySlot)
{
	InventoryManagerComponent->Server_UnEquipToContainer(FromInventorySlot, ToInventorySlot);
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

UDataTable* AMyPlayerController::GetItemDB()
{
	return InventoryManagerComponent->GetItemDB();
}

