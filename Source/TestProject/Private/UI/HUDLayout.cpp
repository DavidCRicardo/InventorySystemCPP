// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDLayout.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "MyPlayerController.h"
//#include "MyHUD.h"
#include "FWidgetsLayoutBP.h"
//#include "UI/MainLayout.h"
#include "Engine/DataTable.h"

void UHUDLayout::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Inventory->OnReleased.AddUniqueDynamic(this, &UHUDLayout::OnBtnUIClicked);
	Btn_Profile->OnReleased.AddUniqueDynamic(this, &UHUDLayout::OnBtnUIClicked);

	Btn_Inventory->OnHovered.AddUniqueDynamic(this, &UHUDLayout::SlipUpInventory);
	Btn_Profile->OnHovered.AddUniqueDynamic(this, &UHUDLayout::SlipUpProfile);

	Btn_Inventory->OnUnhovered.AddUniqueDynamic(this, &UHUDLayout::SlipDownInventory);
	Btn_Profile->OnUnhovered.AddUniqueDynamic(this, &UHUDLayout::SlipDownProfile);

	/*UDataTable* WidgetDB = nullptr;

	UDataTable* BP_WidgetDB = LoadObject<UDataTable>(this, TEXT("/Game/Blueprints/Widgets_DB.Widgets_DB"));
	if (IsValid(BP_WidgetDB))
	{
		WidgetDB = BP_WidgetDB;

		const UDataTable* WidgetTable = WidgetDB;
		FWidgetsLayoutBP* NewWidgetData = nullptr;

		NewWidgetData = WidgetTable->FindRow<FWidgetsLayoutBP>(FName("MainLayout_WBP"), "", true);
		if (NewWidgetData)
		{
			MainLayout = CreateWidget<UMainLayout>(GetWorld(), NewWidgetData->Widget);

			if (MainLayout)
			{
				MainLayout->AddToViewport();
			}
		}
	}*/
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