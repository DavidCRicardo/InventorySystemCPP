// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TertiaryHUD.h"

#include "FWidgetsLayoutBP.h"
#include "MyHUD.h"
#include "UI/InteractiveText_Entry.h"

void UTertiaryHUD::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
}

void UTertiaryHUD::CreateInteractiveTextEntry(FName IDName)
{
	FWidgetsLayoutBP* WidgetLayout = Cast<AMyHUD>(PlayerController->MyHUD)->GetWidgetBPClass("InteractiveText_Entry_WBP");
	if (WidgetLayout)
	{
		UClass* WidgetClass = WidgetLayout->Widget;
		
		UInteractiveText_Entry* Entry = CreateWidget<UInteractiveText_Entry>(this, WidgetClass);
		
		Entry->InitializeName(IDName);
		
		InteractiveMenu->AddEntryToList(Entry);
	}

	/*UInteractiveText_Entry* Entry2 = CreateWidget<UInteractiveText_Entry>(GetWorld(), InteractiveTextWidgetClass);
	InteractiveMenu->AddEntryToList(Entry2);*/
}

void UTertiaryHUD::RemoveInteractiveTextEntry(const FName& ID)
{
	InteractiveMenu->RemoveEntryFromList(ID);
}
