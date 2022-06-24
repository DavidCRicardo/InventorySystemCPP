// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Attribute_Entry.h"


void UAttribute_Entry::SetAttributeText(const FText& Text)
{
	AttributeText = Text;
}

void UAttribute_Entry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
	if (UAttribute_Entry* Entry = Cast<UAttribute_Entry>(ListItemObject))
	{
		if (IsValid(Entry))
		{
			AttributeTextBlock->SetText(Entry->AttributeText);
		}
	}
}

void UAttribute_Entry::SetID(uint8& Index)
{
	ID = Index;
}