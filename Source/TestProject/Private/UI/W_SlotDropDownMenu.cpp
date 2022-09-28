// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_SlotDropDownMenu.h"
#include "Components/Button.h"

/*UW_SlotDropDownMenu::UW_SlotDropDownMenu()
{
	Btn_UseMultiple->SetIsEnabled(false);
}*/

void UW_SlotDropDownMenu::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_UseMultiple->SetIsEnabled(false);

	Btn_Use->OnClicked.AddUniqueDynamic(this, &UW_SlotDropDownMenu::BtnUseClicked);
	Btn_Move->OnClicked.AddUniqueDynamic(this, &UW_SlotDropDownMenu::BtnMoveClicked);
}

void UW_SlotDropDownMenu::BtnUseClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Use Button"))
	GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Blue, TEXT("Use Button"));
}

void UW_SlotDropDownMenu::BtnMoveClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Move Button"))
	GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Blue, TEXT("Move Button"));
}