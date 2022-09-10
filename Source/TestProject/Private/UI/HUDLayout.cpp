// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDLayout.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "MyPlayerController.h"

void UHUDLayout::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Inventory->OnReleased.AddUniqueDynamic(this, &UHUDLayout::OnBtnUIClicked);
	Btn_Profile->OnReleased.AddUniqueDynamic(this, &UHUDLayout::OnBtnUIClicked);

	Btn_Inventory->OnHovered.AddUniqueDynamic(this, &UHUDLayout::SlipUpInventory);
	Btn_Profile->OnHovered.AddUniqueDynamic(this, &UHUDLayout::SlipUpProfile);

	Btn_Inventory->OnUnhovered.AddUniqueDynamic(this, &UHUDLayout::SlipDownInventory);
	Btn_Profile->OnUnhovered.AddUniqueDynamic(this, &UHUDLayout::SlipDownProfile);

}

void UHUDLayout::OnBtnUIClicked() {
	if (AMyPlayerController* Controller = Cast<AMyPlayerController>(GetOwningPlayer()))
	{
		if (Btn_Inventory->IsHovered())
		{
			Controller->ToggleInventory();
		}
		else if (Btn_Profile->IsHovered())
		{
			Controller->ToggleProfile();
		}
	};
}

void UHUDLayout::SlipUpInventory()
{
	if (Btn_Inventory_Animation)
	{
		PlayAnimationForward(Btn_Inventory_Animation);
	}
}

void UHUDLayout::SlipUpProfile()
{
	if (Btn_Profile_Animation)
	{
		PlayAnimationForward(Btn_Profile_Animation);
	}
}


void UHUDLayout::SlipDownInventory()
{
	if (Btn_Inventory_Animation)
	{
		PlayAnimationReverse(Btn_Inventory_Animation);
	}
}

void UHUDLayout::SlipDownProfile()
{
	if (Btn_Profile_Animation)
	{
		PlayAnimationReverse(Btn_Profile_Animation);		
	}
}