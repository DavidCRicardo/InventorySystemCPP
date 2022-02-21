// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerInput.h"
#include "Net/UnrealNetwork.h"

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
	
	/**/
	InteractionField = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionField"));
	InteractionField->SetupAttachment(GetMesh());

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh());
	Weapon->SetIsReplicated(true);
	
	Chest = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Chest"));
	Chest->SetupAttachment(GetMesh());
	
	WeaponMesh = nullptr;
	ChestMesh = nullptr;

	//Initialize the player's Health
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	
	/*MainHandMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("MainHand"));
	if (MainHandMesh)
	{
		MainHandMesh->SetupAttachment(GetMesh());
	}*/

	
 	/*// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Instantiating your class Components
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	//Set the location and rotation of the Character Mesh Transform
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FQuat(FRotator(0.0f, 0.0f, 0.0f)));

	// Attaching your class Components to the default character's Skeletal Mesh Component.
	SpringArmComp->SetupAttachment(GetMesh());
	CameraComp->SetupAttachment(SpringArmComp,USpringArmComponent::SocketName);

	//Setting class variables of the spring arm
	SpringArmComp->bUsePawnControlRotation = true;

	//Setting class variables of the Character movement component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;*/

	//GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
	
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health.
	DOREPLIFETIME(AMyCharacter, CurrentHealth);

	DOREPLIFETIME(AMyCharacter, Weapon);
	DOREPLIFETIME(AMyCharacter, WeaponMesh);
	DOREPLIFETIME(AMyCharacter, ChestMesh);
	DOREPLIFETIME(AMyCharacter, MainFeetMesh);
	DOREPLIFETIME(AMyCharacter, MainHandsMesh);
}

void AMyCharacter::OnHealthUpdate()
{
	//Client-specific functionality
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	//Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}

	//Functions that occur on all machines. 
	/*  
		Any special functionality that should occur as a result of damage or death should be placed here. 
	*/
}

void AMyCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate();
	}
}

void AMyCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMyCharacter::SetWeaponMesh()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("SetWeaponMesh")));

	Weapon->SetSkeletalMesh(WeaponMesh);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "MainWeapon");
}

void AMyCharacter::UpdateMainWeaponMesh(USkeletalMesh* NewMesh)
{
	WeaponMesh = NewMesh;
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("UpdateMainWeaponMesh")));

	SetWeaponMesh();
	
}
void AMyCharacter::UpdateChestMesh(USkeletalMesh* NewMesh)
{
	ChestMesh = NewMesh;
	
	Chest->SetSkeletalMesh(ChestMesh);
	Chest->SetMasterPoseComponent(GetMesh());
}

void AMyCharacter::OnRep_MainWeaponMesh()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("OnRep_MainWeaponMesh")));

	Weapon->SetSkeletalMesh(WeaponMesh);
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "MainWeapon");
}

void AMyCharacter::OnRep_MainChestMesh()
{
	Chest->SetSkeletalMesh(ChestMesh);
	Chest->SetMasterPoseComponent(GetMesh());
}

void AMyCharacter::OnRep_MainFeetMesh()
{
	
}

void AMyCharacter::OnRep_MainHandsMesh()
{
	
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InitializeDefaultPawnInputBindings();
	
	PlayerInputComponent->BindAxis("MyCharacter_MoveForward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MyCharacter_MoveRight", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MyCharacter_MoveUp", this, &AMyCharacter::MoveUp_World);

	PlayerInputComponent->BindAxis("MyCharacter_Turn", this, &AMyCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("MyCharacter_LookUp", this, &AMyCharacter::AddControllerPitchInput);
	
	PlayerInputComponent->BindAxis("MyCharacter_LookUpRate", this, &AMyCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("MyCharacter_TurnRate", this, &AMyCharacter::TurnAtRate);
}

void AMyCharacter::InitializeDefaultPawnInputBindings()
{
	static bool bBindingsAdded = false;
	if (!bBindingsAdded)
	{
		bBindingsAdded = true;

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyCharacter_MoveForward", EKeys::W, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyCharacter_MoveForward", EKeys::S, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyCharacter_MoveRight", EKeys::A, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyCharacter_MoveRight", EKeys::D, 1.f));
		
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyCharacter_MoveUp", EKeys::E, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyCharacter_MoveUp", EKeys::Q, -1.f));
		
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyCharacter_Turn", EKeys::MouseX, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyCharacter_LookUp", EKeys::MouseY, -1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyCharacter_TurnRate", EKeys::Left, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyCharacter_TurnRate", EKeys::Right, 1.f));

		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ToggleProfile", EKeys::P));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ToggleInventory", EKeys::I));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("ToggleMenu", EKeys::M));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Interact", EKeys::F));

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
			AddMovementInput( FRotationMatrix(ControlSpaceRot).GetScaledAxis( EAxis::Y ), Val );
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
			AddMovementInput( FRotationMatrix(ControlSpaceRot).GetScaledAxis( EAxis::X ), Val );
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
}

void AMyCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}
