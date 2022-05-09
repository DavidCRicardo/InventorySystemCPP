// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Attribute_Entry.h"


void UAttribute_Entry::NativeConstruct()
{
	Super::NativeConstruct();
	
	AttributeTextBlock->SetText(FText::FromName("Default Text"));
}

void UAttribute_Entry::SetAttributeText(FText& Text)
{
	//AttributeTextBlock->SetText(Text);
}

void UAttribute_Entry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	
}