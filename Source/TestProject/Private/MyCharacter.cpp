// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyPlayerController.h"
#include "UsableActor.h"
#include "UsableActorInterface.h"
#include "WorldActor.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryManagerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerInput.h"
#include "Net/UnrealNetwork.h"
#include "UI/InteractiveText_Panel.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	InteractionField = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionField"));
	InteractionField->SetupAttachment(GetMesh());

	InteractionField->InitSphereRadius(150);
	//InteractionField->InitCapsuleSize(150.f, 100.f);
	InteractionField->SetCollisionProfileName(TEXT("CollisionTrigger"));

	// Initialize the player's equipment
	MainWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	MainWeapon->SetupAttachment(GetMesh());

	Chest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Chest"));
	Chest->SetupAttachment(GetMesh());

	Hands = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands"));
	Hands->SetupAttachment(GetMesh());

	Feet = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Feet"));
	Feet->SetupAttachment(GetMesh());

	Legs = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Legs"));
	Legs->SetupAttachment(GetMesh());

	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(GetMesh());

	MainWeaponMesh = nullptr;
	ChestMesh = nullptr;
	FeetMesh = nullptr;
	HandsMesh = nullptr;
	LegsMesh = nullptr;
	HeadMesh = nullptr;
	//GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyCharacter, UsableActorsInsideRange);

	DOREPLIFETIME(AMyCharacter, MainWeaponMesh);
	DOREPLIFETIME(AMyCharacter, ChestMesh);
	DOREPLIFETIME(AMyCharacter, FeetMesh);
	DOREPLIFETIME(AMyCharacter, HandsMesh);
	DOREPLIFETIME(AMyCharacter, LegsMesh);
	DOREPLIFETIME(AMyCharacter, HeadMesh);
}

void AMyCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsLocallyControlled() && OverlappedComp == InteractionField)
	{
		if (OtherActor && (OtherActor != this) && OtherComp)
		{
			// GetUsableActor
			bool bDoesImplementInterface = OtherActor->Implements<UUsableActorInterface>();
			if (bDoesImplementInterface)
			{
				if (IsValid(OtherActor))
				{
					if (AWorldActor* WorldActor = Cast<AWorldActor>(OtherActor))
					{
						WorldActorsInsideRange.Add(WorldActor);
						UsableActorsInsideRange.Add(WorldActor);

						MyPlayerController->AddUsableActorToDropMenu(WorldActor->ID);
						IUsableActorInterface::Execute_BeginOutlineFocus(WorldActor);
						
						SetActorTickEnabled(true);

						return;
					}

					if (AUsableActor* UsableActor = Cast<AUsableActor>(OtherActor))
					{
						UsableActorsInsideRange.Add(UsableActor);

						if (IUsableActorInterface::Execute_GetIsActorUsable(UsableActor))
						{
							IUsableActorInterface::Execute_BeginOutlineFocus(UsableActor);

							/* Widget Related */
							if (!UsableActor->InteractUserWidget)
							{
								// Set Interact Text
								FText MessageText = IUsableActorInterface::Execute_GetUseActionText(UsableActor);
								
								/* Debug */
								FString a = MessageText.ToString();
								// L "Use Pot"
								// 
								UE_LOG(LogTemp, Warning, TEXT("%s"), *a);
								/**/

								UUserWidget* Entry = MyPlayerController->CreateInteractWidget("InteractiveText_Entry_WBP");
								UUserWidget* Panel = MyPlayerController->CreateInteractWidget("InteractiveText_Panel_WBP");

								if (UInteractiveText_Entry* a1 = Cast<UInteractiveText_Entry>(Entry))
								{
									a1->SetNameLabelText(MessageText);
									FString string1 = MessageText.ToString();
									FName name1 = *string1;

									//a1->SetEntryText(name1);

									if (UInteractiveText_Panel* a2 = Cast<UInteractiveText_Panel>(Panel))
									{
										a2->AddEntryToList(a1);
										a2->AddToViewport();

										UsableActor->InteractUserWidget = a2;
									}
								}


								//UsableActor->InteractUserWidget = MyPlayerController->GenerateInteractWidget(MessageText);

								//UInteractiveText_Panel* test = Cast<UInteractiveText_Panel>(UsableActor->InteractUserWidget);
								//if (test)
								//{
								//	UsableActor->SetInteractText(MessageText);
								//	//UInteractiveText_Entry* testEntry = test->InteractiveText_List[0];
								//	//test->InteractiveText_List->RegenerateAllEntries();
								//}
								//else {
								//	UE_LOG(LogTemp, Warning, TEXT("This test is Not Valid"))
								//}
							}

							UsableActor->InteractUserWidget->SetVisibility(ESlateVisibility::Visible);

							SetActorTickEnabled(true);
						}

						return;
					}
				}
			}
		}
	}
}

void AMyCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsLocallyControlled() && OverlappedComp == InteractionField)
	{
		if (OtherActor && (OtherActor != this) && OtherComp)
		{
			bool bDoesImplementInterface = OtherActor->Implements<UUsableActorInterface>();
			if (bDoesImplementInterface)
			{
				if (IsValid(OtherActor))
				{
					if (AWorldActor* WorldActor = Cast<AWorldActor>(OtherActor))
					{
						MyPlayerController->RemoveUsableActorToDropMenu(WorldActor->ID);
						IUsableActorInterface::Execute_EndOutlineFocus(WorldActor);

						WorldActorsInsideRange.Remove(WorldActor);
						UsableActorsInsideRange.Remove(WorldActor);

						return;
					}

					if (AUsableActor* UsableActor = Cast<AUsableActor>(OtherActor))
					{
						IUsableActorInterface::Execute_EndOutlineFocus(UsableActor);
						UsableActor->InteractUserWidget->SetVisibility(ESlateVisibility::Hidden);

						UsableActorsInsideRange.Remove(UsableActor);

						// At the moment, Containers are the only case that run the code until here
						if (MyPlayerController->IsContainerOpen())
						{
							MyPlayerController->InventoryManagerComponent->Server_CloseContainer();
						}

						return;
					}
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	MyPlayerController = Cast<AMyPlayerController>(GetController());

	/* Overlap Events */
	InteractionField->OnComponentBeginOverlap.AddDynamic(this, &AMyCharacter::OnBeginOverlap);
	InteractionField->OnComponentEndOverlap.AddDynamic(this, &AMyCharacter::OnEndOverlap);
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UsableActorsInsideRange.Num() == 0)
	{
		if (IsValid(MyPlayerController))
		{
			MyPlayerController->DisableUIMode();
			MyPlayerController->Tick(DeltaTime);
		}

		// For Debug Purposes
		SetActorTickEnabled(false);
		return;
	}

	for (AActor*& UsableActor : UsableActorsInsideRange)
	{
		if (Cast<AWorldActor>(UsableActor))
		{
			return;
		}

		if (AUsableActor* TempUsableActor = Cast<AUsableActor>(UsableActor))
		{
			if (IUsableActorInterface::Execute_GetIsActorUsable(TempUsableActor))
			{
				FVector2D ScreenPosition = {};
				MyPlayerController->ProjectWorldLocationToScreen(UsableActor->GetActorLocation(), ScreenPosition);
				TempUsableActor->SetScreenPosition(ScreenPosition);
				if (MyPlayerController->ProjectWorldLocationToScreen(UsableActor->GetActorLocation(), ScreenPosition))
				{
					if (TempUsableActor->InteractUserWidget->GetVisibility() == ESlateVisibility::Hidden)
					{
						TempUsableActor->InteractUserWidget->SetVisibility(ESlateVisibility::Visible);
						TempUsableActor->InteractUserWidget->SetVisibility(ESlateVisibility::Hidden);
					}

					TempUsableActor->SetScreenPosition(ScreenPosition);
				}
				else
				{
					TempUsableActor->InteractUserWidget->SetVisibility(ESlateVisibility::Hidden);
				}
			}
			else {
				IUsableActorInterface::Execute_EndOutlineFocus(TempUsableActor);
				TempUsableActor->InteractUserWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

/* OnRep Functions */
void AMyCharacter::OnRep_MainWeaponMesh()
{
	MainWeapon->SetSkeletalMesh(MainWeaponMesh);
	MainWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "MainWeapon");
}

void AMyCharacter::OnRep_MainChestMesh()
{
	Chest->SetSkeletalMesh(ChestMesh);
	Chest->SetLeaderPoseComponent(GetMesh());
}

void AMyCharacter::OnRep_MainFeetMesh()
{
	Feet->SetSkeletalMesh(FeetMesh);
	Feet->SetLeaderPoseComponent(GetMesh());
}

void AMyCharacter::OnRep_MainHandsMesh()
{
	Hands->SetSkeletalMesh(HandsMesh);
	Hands->SetLeaderPoseComponent(GetMesh());
}

void AMyCharacter::OnRep_MainLegsMesh()
{
	Legs->SetSkeletalMesh(LegsMesh);
	Legs->SetLeaderPoseComponent(GetMesh());
}

void AMyCharacter::OnRep_MainHeadMesh() {
	Head->SetSkeletalMesh(HeadMesh);
	Head->SetLeaderPoseComponent(GetMesh());
}

/* End OnRep Functions */

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InitializeDefaultPawnInputBindings();

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick

	/* Keyboard */

	PlayerInputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyCharacter::TurnAtRate);

	/* Flying Mode */

	PlayerInputComponent->BindAxis("MoveUp", this, &AMyCharacter::MoveUp_World);

	/* Mobile */

	//PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("TurnRate", this, &AMyCharacter::TurnAtRate);
	//PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("LookUpRate", this, &AMyCharacter::LookUpAtRate);

	// handle touch devices
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &AMyCharacter::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &AMyCharacter::TouchStopped);

} 

void AMyCharacter::InitializeDefaultPawnInputBindings()
{
	static bool bBindingsAdded = false;
	if (!bBindingsAdded)
	{
		bBindingsAdded = true;

		/* Keyboard */

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::S, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::A, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::D, 1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Turn", EKeys::MouseX, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUp", EKeys::MouseY, -1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Left, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Right, 1.f));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ToggleProfile", EKeys::P));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ToggleInventory", EKeys::I));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ToggleMenu", EKeys::M));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Interact", EKeys::F));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ToggleUIMode", EKeys::LeftAlt));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("QuitGame", EKeys::Escape));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("UseHotbar1", EKeys::One));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("UseHotbar2", EKeys::Two));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("UseHotbar3", EKeys::Three));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("UseHotbar4", EKeys::Four));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("UseHotbar5", EKeys::Five));

		/* Flying Mode */
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveUp", EKeys::E, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveUp", EKeys::Q, -1.f));

		/* Mobile */
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::Gamepad_LeftY, 1.f)); // Gamepad Left Thumbstick Y-Axis
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::Gamepad_LeftX, 1.f)); // Gamepad Left Thumbstick X-Axis
		
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUpRate", EKeys::Gamepad_RightY, 1.f)); // Gamepad Right Thumbstick Y-Axis
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Gamepad_RightX, 1.f)); // Gamepad Right Thumbstick X-Axis

		//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Turn", EKeys::Gamepad_RightX, 1.f));
		//UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUp", EKeys::Gamepad_RightY, -1.f));
	}
}

void AMyCharacter::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val);
		}
	}
}

void AMyCharacter::MoveForward(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), Val);
		}
	}
}

void AMyCharacter::MoveUp_World(float Val)
{
	if (Val != 0.f)
	{
		AddMovementInput(FVector::UpVector, Val);
	}
}

void AMyCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);

	/* Mobile */
	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);

	/* Mobile */
	// calculate delta for this frame from the rate information
	//AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	//Jump();
}

void AMyCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	//StopJumping();
}