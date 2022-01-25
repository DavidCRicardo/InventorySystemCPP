// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/EquipmentComponent.h"
#include "UI/ProfileLayout.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> ProfileObj(TEXT("/Game/UI/WBP_ProfileLayout"));
    WidgetClass = ProfileObj.Class;
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	NumberOfEquipmentSlots = 4;

	InitInventory(NumberOfEquipmentSlots);

	InitializeLayout();
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentComponent::InitInventory(int32 NumberSlots)
{
	Super::InitInventory(NumberSlots);
	
}

void UEquipmentComponent::InitializeLayout()
{
	if (WidgetClass != nullptr)
	{
		if (WindowWidget == nullptr)
		{
			WindowWidget = CreateWidget<UProfileLayout>(GetWorld(), WidgetClass);
			WindowWidget->AddToViewport();

			WindowWidget->SetAnchorsInViewport(FAnchors{0.2f, 0.2f});

			WindowWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UEquipmentComponent::RefreshWidgetUI()
{
	if (UProfileLayout* Widget = Cast<UProfileLayout>(WindowWidget))
	{
		Widget->RefreshWindow();
	}
}


void UEquipmentComponent::ToggleWindow()
{
	Super::ToggleWindow();
}
