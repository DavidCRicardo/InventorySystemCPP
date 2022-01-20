// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_ItemTooltip.h"

void UW_ItemTooltip::InitializeTooltip(FItemStructure& Item)
{
	Name->SetText(Item.Name);
	Icon = Item.Icon;

	/*FText TempText;
	switch (Item.ItemType)
	{
	case EItemType::Consumable:
		TempText = TEXT('');
		break;
	case EItemType::Undefined:
	default:
		TempText = TEXT('');
		break;
	}
	
	Type->SetText(TempText);*/
	
	Description->SetText(Item.Description);
}
