// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
//#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerInput.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
	GetCharacterMovement()->bIgnoreBaseRotation = true;

	//GetCharacterMovement()->DefaultLandMovementMode = MOVE_Flying;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

	//PlayerInputComponent->BindAxis("MyCharacter_Turn", this, &AMyCharacter::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("MyCharacter_LookUp", this, &AMyCharacter::AddControllerPitchInput);
	
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
