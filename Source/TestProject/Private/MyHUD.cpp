// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

#include "FWidgetsLayoutBP.h"
#include "Blueprint/UserWidget.h"
#include "UI/HUDLayout.h"
#include "UI/InventoryLayout.h"
#include "UI/ProfileLayout.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> BP_WidgetDB(TEXT("/Game/Blueprints/Widgets_DB.Widgets_DB"));
	if (BP_WidgetDB.Succeeded())
	{
		WidgetDB = BP_WidgetDB.Object;
	}
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	
	const UDataTable* WidgetTable = WidgetDB;
	
	FWidgetsLayoutBP* NewWidgetData = WidgetTable->FindRow<FWidgetsLayoutBP>(FName("HUDLayout_WBP"), "", true);
	if (NewWidgetData)
	{
		HUDReference = CreateWidget<UHUDLayout>(GetWorld(), NewWidgetData->Widget);
		
		if (HUDReference)
		{
			HUDReference->AddToViewport();
		}
	}else
	{
		UE_LOG (LogTemp, Warning, TEXT ("Cannot Load Class HUDLayout_WBP."));
	}

	NewWidgetData = WidgetTable->FindRow<FWidgetsLayoutBP>(FName("ProfileLayout_WBP"), "", true);	
	if (NewWidgetData)
	{
		ProfileLayout = CreateWidget<UProfileLayout>(GetWorld(), NewWidgetData->Widget);
		
		if (ProfileLayout)
		{
			ProfileLayout->AddToViewport();
			ProfileLayout->SetAnchorsInViewport(FAnchors{0.2f, 0.2f});
			ProfileLayout->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	NewWidgetData = WidgetTable->FindRow<FWidgetsLayoutBP>(FName("InventoryLayout_WBP"), "", true);	
	if (NewWidgetData)
	{
		InventoryLayout = CreateWidget<UInventoryLayout>(GetWorld(), NewWidgetData->Widget);
		
		if (InventoryLayout)
		{
			InventoryLayout->AddToViewport();
			InventoryLayout->SetAnchorsInViewport(FAnchors{0.7f, 0.2f});
			InventoryLayout->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

bool AMyHUD::IsAnyWidgetVisible()
{
	if (InventoryLayout->IsVisible() || ProfileLayout->IsVisible())
	{
		return true;
	}
	return false;
}

void AMyHUD::ToggleWindow(const ELayout Layout)
{
	if (Layout == ELayout::Inventory)
	{
		InventoryLayout->ToggleWindow();
	}
	else if (Layout == ELayout::Equipment)
	{
		ProfileLayout->ToggleWindow();
	}
}

void AMyHUD::RefreshWidgetUILayout(const ELayout Layout)
{
	if (Layout == ELayout::Inventory)
	{
		InventoryLayout->RefreshWindow();
	}
	else if (Layout == ELayout::Equipment)
	{
		ProfileLayout->RefreshWindow();
	}
}

FWidgetsLayoutBP* AMyHUD::GetWidgetBPClass(const FName Name)
{
	const UDataTable* WidgetTable = WidgetDB;
	return WidgetTable->FindRow<FWidgetsLayoutBP>(Name, "", true);
}