// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "FWidgetsLayoutBP.h"
#include "Blueprint/UserWidget.h"
#include "UI/ContainerLayout.h"
#include "UI/HUDLayout.h"
#include "UI/InteractiveText_Panel.h"
#include "UI/ELayout.h"
#include "UI/InventoryLayout.h"
#include "UI/MainLayout.h"
#include "UI/ProfileLayout.h"
#include "UI/TertiaryHUD.h"
#include "Components/InventoryManagerComponent.h"

AMyHUD::AMyHUD()
{
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	UDataTable* BP_WidgetDB = LoadObject<UDataTable>(this, TEXT("/Game/Blueprints/Widgets_DB.Widgets_DB"));
	if (IsValid(BP_WidgetDB))
	{
		WidgetDB = BP_WidgetDB;
	}

	const UDataTable* WidgetTable = WidgetDB;
	FWidgetsLayoutBP* NewWidgetData = nullptr;

	NewWidgetData = WidgetTable->FindRow<FWidgetsLayoutBP>(FName("HUDLayout_WBP"), "", true);
	if (NewWidgetData)
	{
		HUDReference = CreateWidget<UHUDLayout>(GetWorld(), NewWidgetData->Widget);

		if (HUDReference)
		{	
			HUDReference->AddToViewport();
		}
	}
	
	// UE_BUILD_SHIPPING on PlayerController line 112
	#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT
	if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetOwner()))
	{
		PlayerController->SetupHUDReferences();
	}
	#endif
}

UUserWidget* AMyHUD::GenerateInteractWidget(FText Text)
{
	const UDataTable* WidgetTable = WidgetDB;
	FWidgetsLayoutBP* NewWidgetData = nullptr;

	UUserWidget* EntryUserWidget = CreateWidgetFromDataTable(WidgetTable, NewWidgetData, FName("InteractiveText_Entry_WBP"));
	UInteractiveText_Entry* Entry = Cast<UInteractiveText_Entry>(EntryUserWidget);
	FName LocalName = *Text.ToString();
	Entry->SetEntryText(LocalName);

	UUserWidget* PanelUserWidget = CreateWidgetFromDataTable(WidgetTable, NewWidgetData, FName("InteractiveText_Panel_WBP"));
	UInteractiveText_Panel* Panel = Cast<UInteractiveText_Panel>(PanelUserWidget);
	Panel->AddEntryToList(Entry);
	Panel->AddToViewport();

	return Panel;
}

bool AMyHUD::IsAnyWidgetVisible()
{
	if (
		HUDReference->MainLayout->Inventory->IsVisible()
		||
		HUDReference->MainLayout->Profile->IsVisible()
		||
		HUDReference->MainLayout->Container->IsVisible()
		||
		HUDReference->TertiaryHUD->InteractiveMenu->IsVisible()
		)
	{
		return true;
	}

	return false;
}

void AMyHUD::ToggleWindow(const ELayout Layout)
{
	if (Layout == ELayout::Inventory)
	{
		if (HUDReference->MainLayout->Inventory)
		{
			HUDReference->MainLayout->Inventory->ToggleWindow();
		}
	}
	else if (Layout == ELayout::Equipment)
	{
		if(HUDReference->MainLayout->Profile)
		{
			HUDReference->MainLayout->Profile->ToggleWindow();
		}
	}
	else if (Layout == ELayout::Container)
	{
		if(HUDReference->MainLayout->Container)
		{
			HUDReference->MainLayout->Container->ToggleWindow();
		}
	}
}

UUserWidget* AMyHUD::CreateWidgetFromDataTable(const UDataTable* WidgetTable, FWidgetsLayoutBP*& NewWidgetData, FName Name)
{
	NewWidgetData = WidgetTable->FindRow<FWidgetsLayoutBP>(Name, "", true);
	if (NewWidgetData)
	{
		return CreateWidget<UUserWidget>(GetWorld(), NewWidgetData->Widget);
	}
	return nullptr;
}

FWidgetsLayoutBP* AMyHUD::GetWidgetBPClass(const FName Name)
{
	const UDataTable* WidgetTable = WidgetDB;
	
	if (!IsValid(WidgetTable))
	{
		UE_LOG(LogTemp, Warning, TEXT("WidgetTable Not Valid"))
	}

	return WidgetTable->FindRow<FWidgetsLayoutBP>(Name, "", true);
}