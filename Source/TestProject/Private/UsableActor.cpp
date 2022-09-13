// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableActor.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InteractText.h"
//#include "MyGameInstance.h"

// Sets default values
AUsableActor::AUsableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	
	SetRootComponent(Scene);
	StaticMesh->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);

	//GameInstance = Cast<UMyGameInstance>(GetGameInstance());

	Name = FText::FromString("NULL");
	Action = FText::FromString("Use");
	IsUsable = true;
	UsedSound = nullptr;
	WasUsed = false;

	InteractUserWidget = nullptr;
}

void AUsableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AUsableActor, Name);
	DOREPLIFETIME(AUsableActor, IsUsable);
	DOREPLIFETIME(AUsableActor, WasUsed);
}

FText AUsableActor::GetUseActionText_Implementation()
{
	FFormatNamedArguments Args;
	Args.Add("Action", Action);
	Args.Add("Name", Name);
	
	FText FormattedText = FText::Format(
		NSLOCTEXT("MyNamespace", "MyKey", "[F] {Action} {Name}"),
		Args
	);
	
	return FormattedText;
	// return IUsableActorInterface::GetUseActionText_Implementation();
}

bool AUsableActor::GetIsActorUsable_Implementation()
{
	return IsUsable;
	// return IUsableActorInterface::GetIsActorUsable_Implementation();
}

bool AUsableActor::BeginOutlineFocus_Implementation()
{
	StaticMesh->SetRenderCustomDepth(true);
	StaticMesh->SetCustomDepthStencilValue(2);
	
	return true;
	//return IUsableActorInterface::BeginOutlineFocus_Implementation();
}

bool AUsableActor::EndOutlineFocus_Implementation()
{
	StaticMesh->SetRenderCustomDepth(false);
	StaticMesh->SetCustomDepthStencilValue(0);

	return true;
	//	return IUsableActorInterface::EndOutlineFocus_Implementation();
}

bool AUsableActor::OnActorUsed_Implementation(APlayerController* Controller)
{
	if (HasAuthority()) {
		if (WasUsed)
		{
			WasUsed = false;
		}
		else {
			WasUsed = true;
		}
		OnRep_WasUsed();

		return true;
	}
	
	return false;
	// return IUsableActorInterface::OnActorUsed_Implementation(Controller);
}

// Called when the game starts or when spawned
void AUsableActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AUsableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUsableActor::SetInteractText(FText Text)
{
	if (UInteractText* Widget = Cast<UInteractText>(InteractUserWidget))
	{
		Widget->InteractText->SetText(Text);
	}
}

void AUsableActor::SetScreenPosition(FVector2D ScreenPosition)
{
	InteractUserWidget->SetPositionInViewport(ScreenPosition);
}

void AUsableActor::OnRep_WasUsed()
{
	OnWasUsed();
}

bool AUsableActor::OnWasUsed()
{
	if (IsValid(UsedSound)) {
		UGameplayStatics::PlaySoundAtLocation(this, UsedSound, GetActorLocation());
	}
	
	return true;
}