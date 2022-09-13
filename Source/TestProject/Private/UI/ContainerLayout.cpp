// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ContainerLayout.h"
#include "Internationalization/StringTableRegistry.h"
#include "Components/InventoryManagerComponent.h"
#include "LootActor.h"

UContainerLayout::UContainerLayout() {}

void UContainerLayout::SetTextToContainer(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
	{
		FName InTableID = GameInstance->COMMON_WORDS;
		FText Text{};
		if (IsStorageContainer)
		{
			Text = FText::FromStringTable(InTableID, "Container");
		}
		else {
			Text = FText::FromStringTable(InTableID, "Loot");
		}
		Super::SetTitleToWindow(Text);
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