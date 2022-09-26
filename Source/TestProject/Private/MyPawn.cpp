// Fill out your copyright notice in the Description page of Project Settings.
//LogCharacterMovement: Error: CharacterMovement owned by MyPawn_0 must update a component owned by a Character

#include "MyPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/SpringArmComponent.h"

/*
 * MyPawn is not currently used to anything 
 */

// Sets default values
AMyPawn::AMyPawn(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create Root
	RootComponent = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");

	// Create Capsule
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	CapsuleComponent->SetupAttachment(RootComponent);

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("ArrowComponent");
	ArrowComponent->SetupAttachment(CapsuleComponent);
	
	// Create StaticMesh
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> Asset(TEXT("/Game/Items/Consumables/Meshes/SM_Food_Apple.SM_Food_Apple"));
	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> Asset(TEXT("/Game/Items/Consumables/Meshes/SM_Food_Apple.SM_Food_Apple"));
    //if (Asset.Succeeded())
    //{
    	//SkeletalMeshComponent->SetSkeletalMesh(Asset.Object);
    	SkeletalMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    	SkeletalMeshComponent->SetWorldScale3D(FVector(1.f));
    //}
	
	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(CapsuleComponent);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	CharacterMovement = CreateDefaultSubobject<UCharacterMovementComponent>(TEXT("CharacterMovement"));

	bAddDefaultMovementBindings = true;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	//check(GEngine != nullptr)
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("MyPawn is initialized!"));
	UE_LOG (LogTemp, Warning, TEXT ("MyPawn Initialized!!"))

}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	//check(PlayerInputComponent);
	
	InitializeDefaultPawnInputBindings();

	if (bAddDefaultMovementBindings)
	{
		PlayerInputComponent->BindAxis("MyPawn_MoveForward", this, &AMyPawn::MoveForward);
		PlayerInputComponent->BindAxis("MyPawn_MoveRight", this, &AMyPawn::MoveRight);
		PlayerInputComponent->BindAxis("MyPawn_MoveUp", this, &AMyPawn::MoveUp_World);
	}
}

void AMyPawn::InitializeDefaultPawnInputBindings()
{
	static bool bBindingsAdded = false;
	if (!bBindingsAdded)
	{
		bBindingsAdded = true;

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyPawn_MoveForward", EKeys::W, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyPawn_MoveForward", EKeys::S, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyPawn_MoveRight", EKeys::A, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyPawn_MoveRight", EKeys::D, 1.f));
		
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyPawn_MoveUp", EKeys::LeftControl, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyPawn_MoveUp", EKeys::SpaceBar, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyPawn_MoveUp", EKeys::C, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyPawn_MoveUp", EKeys::E, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MyPawn_MoveUp", EKeys::Q, -1.f));
	}
}

void AMyPawn::MoveForward(float Val)
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

void AMyPawn::MoveRight(float Val)
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

void AMyPawn::MoveUp_World(float Val)
{
	if (Val != 0.f)
	{
		UE_LOG (LogTemp, Warning, TEXT ("MoveUp_World!!"))

		AddMovementInput(FVector::UpVector, Val);
	}
}