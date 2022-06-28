// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ContainerLayout.h"

#include "MyHUD.h"
#include "Internationalization/StringTableRegistry.h"

UContainerLayout::UContainerLayout()
{
}

void UContainerLayout::NativeConstruct()
{
	Super::NativeConstruct();

	FText Text = LOCTABLE(COMMON_WORDS, "CONTAINERKey");
	Super::SetTitleToWindow(Text);
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