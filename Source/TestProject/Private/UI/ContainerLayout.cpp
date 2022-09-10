// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ContainerLayout.h"
#include "LootActor.h"
#include "Components/InventoryManagerComponent.h"
#include "Internationalization/StringTableRegistry.h"

UContainerLayout::UContainerLayout() {}

void UContainerLayout::SetTextToContainer(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
	{
		FText Text{};
		if (IsStorageContainer)
		{
			Text = LOCTABLE(COMMON_WORDS, "Container");
			Super::SetTitleToWindow(Text);
		}
		else {
			Text = LOCTABLE(COMMON_WORDS, "Loot");
			Super::SetTitleToWindow(Text);
		}
	}
}

void UContainerLayout::NativeConstruct()
{
	Super::NativeConstruct();

	OnNativeVisibilityChanged.AddUObject(this, &UContainerLayout::SetTextToContainer);
}

void UContainerLayout::ToggleWindow()
{
	Super::ToggleWindow();
}

void UContainerLayout::OnButtonQuitClicked()
{
	if (IsValid(PlayerController))
	{
		PlayerController->InventoryManagerComponent->Server_CloseContainer();
	}
}