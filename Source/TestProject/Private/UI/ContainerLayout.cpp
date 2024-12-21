// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ContainerLayout.h"
#include "Internationalization/StringTableRegistry.h"
#include "Components/InventoryManagerComponent.h"
#include "LootActor.h"

void UContainerLayout::NativeConstruct()
{
	Super::NativeConstruct();

	OnNativeVisibilityChanged.AddUObject(this, &UContainerLayout::SetTextToContainer);

	WindowLayout = ELayout::Container;
}

void UContainerLayout::OnButtonQuitClicked()
{
	if (IsValid(PlayerController))
	{
		PlayerController->InventoryManagerComponent->Server_CloseContainer();
	
		PlayerController->SetInputDependingFromVisibleWidgets();
	}
}

void UContainerLayout::SetTextToContainer(ESlateVisibility InVisibility)
{
	if (InVisibility == ESlateVisibility::Visible)
	{
		FName InTableID = GameInstance->COMMON_WORDS;
		FText Text{};
		if (IsStorageContainer)
		{
			Text = FText::FromStringTable(InTableID, TEXT("Container"));
		}
		else {
			Text = FText::FromStringTable(InTableID, TEXT("Loot"));
		}
		Super::SetTitleToWindow(Text);
	}
}