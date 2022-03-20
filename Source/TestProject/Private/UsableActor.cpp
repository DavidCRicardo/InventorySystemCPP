// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableActor.h"

#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "UI/InteractText.h"

// Sets default values
AUsableActor::AUsableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	
	SetRootComponent(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);
	
	Name = FName(TEXT("NULL"));
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
	DOREPLIFETIME(AUsableActor, WorldMesh);
}

FText AUsableActor::GetUseActionText_Implementation()
{
	FText FormatInput = FText::FromName(Name);
	FFormatNamedArguments Args;
	Args.Add("Action", Action);
	Args.Add("Name", FormatInput);

	FText FormattedText = FText::Format(FormatInput, Args);

	return FormattedText;
	// return IUsableActorInterface::GetUseActionText_Implementation();
}

bool AUsableActor::GetIsActorUsable_Implementation()
{
	// return IUsableActorInterface::GetIsActorUsable_Implementation();

	return IsUsable;
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

bool AUsableActor::OnRep_WasUsed_Validate()
{
	return true;
}

void AUsableActor::OnRep_WasUsed_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("OnRep_WasUsed_Implementation being called"));
	OnWasUsed();
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

bool AUsableActor::OnWasUsed()	 {
	
	if (IsValid(UsedSound)) {
		UGameplayStatics::PlaySoundAtLocation(this, UsedSound, GetActorLocation());
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("WasUsed being called"));

	return true;
}
