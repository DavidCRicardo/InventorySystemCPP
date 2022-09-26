// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TertiaryHUD.h"
#include "FWidgetsLayoutBP.h"
#include "MyHUD.h"
#include "MyPlayerController.h"
#include "UI/InteractiveText_Entry.h"
#include "UI/InteractiveText_Panel.h"

void UTertiaryHUD::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
}

UInteractiveText_Entry* UTertiaryHUD::CreateInteractiveTextEntry(FName IDName)
{
	UInteractiveText_Entry* Entry = nullptr;

	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PlayerController->MyHUD)->GetWidgetBPClass("InteractiveText_Entry_WBP");
	if (WidgetLayout)
	{
		UClass* WidgetClass = WidgetLayout->Widget;
		
		Entry = CreateWidget<UInteractiveText_Entry>(this, WidgetClass);
		
		Entry->SetEntryText(IDName);
	}

	return Entry;
}

void UTertiaryHUD::RemoveInteractiveTextEntry(const FName& ID)
{
	InteractiveMenu->RemoveEntryFromList(ID);
}

void UTertiaryHUD::AddInteractiveTextEntry(UInteractiveText_Entry* Entry) {
	InteractiveMenu->AddEntryToList(Entry);
}