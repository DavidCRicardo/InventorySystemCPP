// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/W_ItemTooltip.h"
#include "Item/FSItemType.h"

void UW_ItemTooltip::InitializeTooltip(const FItemStructure& Item)
{
	Name->SetText(Item.Name);
	Icon->SetBrushFromTexture(Item.Icon);

	FText InText;
	switch (Item.ItemType)
	{
	case EItemType::Consumable:
		InText = FSItemType::Consumable;
		break;
	case EItemType::Equipment:
		InText = FSItemType::Equipment;
		break;
	case EItemType::Miscellaneous:
		InText = FSItemType::Miscellanious;
		break;
	case EItemType::Undefined:
	default:
		InText = FSItemType::Undefined;
		break;
	}
	
	Type->SetText(InText);

	Description->SetText(Item.Description);
}