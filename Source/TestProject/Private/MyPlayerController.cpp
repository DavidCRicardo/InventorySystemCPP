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
#include "Actors/UsableDoor.h"

AMyPlayerController::AMyPlayerController()
{
	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>(TEXT("InventoryComponent"));
	InventoryManagerComponent->SetIsReplicated(true);
	
	PlayerInventoryComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));

	InventoryManagerComponent->NumberOfRowsInventory = PlayerInventoryComponent->NumberOfRowsInventory;
	InventoryManagerComponent->SlotsPerRowInventory = PlayerInventoryComponent->SlotsPerRowInventory;
	
	if (PlayerInventoryComponent->NumberOfSlotsOnHotbar > GetMaximumHotbarSlots())
	{
		PlayerInventoryComponent->NumberOfSlotsOnHotbar = GetMaximumHotbarSlots();
	}
	InventoryManagerComponent->NumberOfSlotsOnHotbar = PlayerInventoryComponent->NumberOfSlotsOnHotbar;

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

	InputComponent->BindAction("UseHotbar1", IE_Pressed, this, &AMyPlayerController::UseHotbarSlot1);
	InputComponent->BindAction("UseHotbar2", IE_Pressed, this, &AMyPlayerController::UseHotbarSlot2);
	InputComponent->BindAction("UseHotbar3", IE_Pressed, this, &AMyPlayerController::UseHotbarSlot3);
	InputComponent->BindAction("UseHotbar4", IE_Pressed, this, &AMyPlayerController::UseHotbarSlot4);
	InputComponent->BindAction("UseHotbar5", IE_Pressed, this, &AMyPlayerController::UseHotbarSlot5);
}

void AMyPlayerController::UseHotbarSlot1() {
	InventoryManagerComponent->Client_UseHotbarSlot(0);
}
void AMyPlayerController::UseHotbarSlot2() {
	InventoryManagerComponent->Client_UseHotbarSlot(1);
}
void AMyPlayerController::UseHotbarSlot3() {
	InventoryManagerComponent->Client_UseHotbarSlot(2);
}
void AMyPlayerController::UseHotbarSlot4() {
	InventoryManagerComponent->Client_UseHotbarSlot(3);
}
void AMyPlayerController::UseHotbarSlot5() {
	InventoryManagerComponent->Client_UseHotbarSlot(4);
}

void AMyPlayerController::QuitGame()
{
	FGenericPlatformMisc::RequestExit(false);
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	DisableInput(this);

	/*if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Server")));
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Client")));
	}*/

	// Delay
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		UE_LOG(LogTemp, Warning, TEXT("This text will appear in the console 0.5 second(s) after execution"))

		CharacterReference = Cast<AMyCharacter>(GetPawn());

		PlayerInventoryComponent->EquipmentCharacterReference = CharacterReference;
		InventoryManagerComponent->InitializeInventoryManager(PlayerInventoryComponent);
		
		// Server: Initialize Inventory
		InventoryManagerComponent->Server_InitInventory();

		InventoryManagerComponent->InitializePlayerAttributes();

		// UE_BUILD_DEBUG/UE_BUILD_DEVELOPMENT on PlayerController line 51-56
		#if UE_BUILD_SHIPPING
			SetupHUDReferences();
		#endif

		EnableInput(this);
	}, 0.5, false);
}

int AMyPlayerController::GetCurrentViewMode(const APlayerController* PlayerController)
{

	if (IsValid(PlayerController))
	{
		UGameViewportClient* GameViewportClient = PlayerController->GetWorld()->GetGameViewport();
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

		bool ignore = GameViewportClient->IgnoreInput();
		EMouseCaptureMode capt = GameViewportClient->GetMouseCaptureMode(); // ->CaptureMouseOnClick();

		if (ignore == false && capt == EMouseCaptureMode::CaptureDuringMouseDown)
		{
			return 0;  // Game And UI
		}
		else if (ignore == true && capt == EMouseCaptureMode::NoCapture)
		{
			return 1;  // UI Only
		}
		else
		{
			return 2;  // Game Only
		}
	}

	return -1;
}

void AMyPlayerController::Tick(float DeltaTime)
{
	if (GetCurrentViewMode(this) == 0)
	{
		GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Red, "Game And UI");
	}
	if (GetCurrentViewMode(this) == 1)
	{
		GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Red, "UI Only");
	}
	if (GetCurrentViewMode(this) == 2)
	{
		GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Red, "Game Only");
	}
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
	
		InventoryManagerComponent->Client_LoadInventoryUI();
		InventoryManagerComponent->Client_LoadProfileUI();

		InventoryManagerComponent->Client_LoadHotbarUI();
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

void AMyPlayerController::OnActorUsed(AActor* Actor1)
{
	if (HasAuthority())
	{
		/*AActor* Actor{};

		// Get Usable Actor
		if (CharacterReference->UsableActorsInsideRange.Num() > 0)
		{
			// Get Selected Item
			uint32 Index = 0;
			GetSelectedItemIndex(Index);

			Actor = CharacterReference->UsableActorsInsideRange[Index];
		}*/

		if (IsValid(Actor1))
		{
			IUsableActorInterface::Execute_OnActorUsed(Actor1, this);
		}
	}
}

void AMyPlayerController::ToggleInventory()
{
	if (IsValid(HUD_Reference))
	{
		HUD_Reference->ToggleWindow(ELayout::Inventory);		
		SetInputDependingFromVisibleWidgets();
	}
}

void AMyPlayerController::ToggleProfile()
{
	if (IsValid(HUD_Reference))
	{
		HUD_Reference->ToggleWindow(ELayout::Equipment);
		SetInputDependingFromVisibleWidgets();
	}
}

void AMyPlayerController::ToggleContainer()
{
	if (IsValid(HUD_Reference))
	{
		HUD_Reference->ToggleWindow(ELayout::Container);
		SetInputDependingFromVisibleWidgets();
	}
}

void AMyPlayerController::SetInputDependingFromVisibleWidgets()
{
	if (HUD_Reference->IsAnyWidgetVisible())
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;

		HUDLayoutReference->MainLayout->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;

		HUDLayoutReference->MainLayout->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void AMyPlayerController::EnableUIMode()
{
	if (!IsValid(HUD_Reference))
	{
		return;
	}

	if (!bShowMouseCursor)
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;

		SetMouseToCenterPosition();
	}
}

void AMyPlayerController::DisableUIMode()
{
	if (!IsValid(HUD_Reference))
	{
		return;
	}

	if (bShowMouseCursor)
	{
		SetInputDependingFromVisibleWidgets();
	//	SetInputMode(FInputModeGameOnly());
	//	bShowMouseCursor = false;
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
	//Server_OnActorUsed();
	//OnActorUsed();	
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
	else {
		UE_LOG(LogTemp, Warning, TEXT("Character Reference is null"))
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
		else {
			// It's an Usable Actor
			Server_OnActorUsed(Actor);

			return;
		}
	}
}

UUserWidget* AMyPlayerController::GenerateInteractWidget(FText Text)
{
	return HUD_Reference->GenerateInteractWidget(Text);
}

UUserWidget* AMyPlayerController::CreateInteractWidget(FName Name) {
	return HUD_Reference->CustomCreateWidget(Name);
}

void AMyPlayerController::SetMouseToCenterPosition()
{
	int32 SizeX;
	int32 SizeY;
	GetViewportSize(SizeX, SizeY);

	SetMouseLocation(SizeX / 2, SizeY / 2);
}

void AMyPlayerController::AddUsableActorToDropMenu(FName IDName)
{
	if (IsValid(HUD_Reference))
	{
		UInteractiveText_Entry* Entry = HUD_Reference->HUDReference->TertiaryHUD->CreateInteractiveTextEntry(IDName);
		HUD_Reference->HUDReference->TertiaryHUD->AddInteractiveTextEntry(Entry);

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

void AMyPlayerController::UI_MoveHotbarItem_Implementation(const uint8& FromSlot, const uint8& ToSlot, const bool IsDraggedFromInventory, const bool IsDraggedFromHotbar)
{
	InventoryManagerComponent->Client_MoveHotbarSlotItem(FromSlot, ToSlot, IsDraggedFromInventory, IsDraggedFromHotbar);
}

uint8 AMyPlayerController::UIGetPlayerGold()
{
	return InventoryManagerComponent->Gold;
}