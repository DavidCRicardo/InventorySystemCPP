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

	MainHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MainHand"));
	MainHand->SetupAttachment(GetMesh());

	MainHandMesh = nullptr;
	
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

void AMyCharacter::OnRep_MainHandMesh()
{
	SetHandMesh();
	
	/*FEquipmentSockets__pf2757191718 bpfv__LocalEquipmentSockets__pf{};
	bool bpfv__CallFunc_K2_AttachToComponent_ReturnValue__pf{};
	const UScriptStruct* __Local__47 = FEquipmentSockets__pf2757191718::StaticStruct();
	uint8* __Local__48 = (uint8*)FMemory_Alloca(__Local__47->GetStructureSize());
	__Local__47->InitializeStruct(__Local__48);
	FEquipmentSockets__pf2757191718& __Local__46 = *reinterpret_cast<FEquipmentSockets__pf2757191718*>(__Local__48);
	bpfv__LocalEquipmentSockets__pf = __Local__46;
	if(::IsValid(MainHand))
	{
		MainHand->SetSkeletalMesh(MainHandMesh, true);
		bpfv__CallFunc_K2_AttachToComponent_ReturnValue__pf = MainHand->USceneComponent::K2_AttachToComponent((*(AccessPrivateProperty<USkeletalMeshComponent* >((this), ACharacter::__PPO__Mesh() ))), bpfv__LocalEquipmentSockets__pf.bpv__MainxHand_14_8A558B7C43EA00D3972964BA4E935F53__pfG, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	}*/
}

void AMyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AMyCharacter, MainHandMesh);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyCharacter::SetHandMesh()
{
	MainHand->SetSkeletalMesh(MainHandMesh);
	MainHand->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "MainHand");
}

void AMyCharacter::UpdateMainHandMesh(USkeletalMesh* NewHandMesh)
{
	MainHandMesh = NewHandMesh;
	SetHandMesh();
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
