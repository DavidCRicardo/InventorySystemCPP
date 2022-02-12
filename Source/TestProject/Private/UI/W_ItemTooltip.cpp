// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_ItemTooltip.h"
#include "Item/FItemType.h"

void UW_ItemTooltip::InitializeTooltip(const FItemStructure& Item)
{
	Name->SetText(Item.Name);
	Icon->SetBrushFromTexture(Item.Icon);

	FText InText;
	switch (Item.ItemType)
	{
	case EItemType::Consumable:
		InText = FItemType::Consumable;
		break;
	case EItemType::Equipment:
		InText = FItemType::Equipment;
		break;
	case EItemType::Miscellaneous:
		InText = FItemType::Miscellanious;
		break;
	case EItemType::Undefined:
	default:
		InText = FItemType::Undefined;
		break;
	}
	
	Type->SetText(InText);

	Description->SetText(Item.Description);
}