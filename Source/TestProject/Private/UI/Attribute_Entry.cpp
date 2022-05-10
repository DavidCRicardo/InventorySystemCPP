// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Attribute_Entry.h"


void UAttribute_Entry::SetAttributeText(const FText& Text)
{
	DebugText = Text;
}

void UAttribute_Entry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("NativeOnListItemObjectSet")));

	AttributeTextBlock->SetText(FText::FromString("Default Text"));
	
	if (UAttribute_Entry* Entry = Cast<UAttribute_Entry>(ListItemObject))
	{
		if (IsValid(Entry))
		{
			// Working
			AttributeTextBlock->SetText(Entry->DebugText);

			// Working but...
			// All entries will be Strength: 0
			/* FFormatNamedArguments Args;
			uint8 Value = 0;
			Args.Add("Value", Value);
			FText FormattedText = FText::Format(
				NSLOCTEXT("MyNamespace", "StrengthKey", "Strength: {Value}"),
				Args
			);
			AttributeTextBlock->SetText(FormattedText);
			*/
		}
	}
}

void UAttribute_Entry::SetID(uint8& Index)
{
	ID = Index;
	//DebugText = FText::FromString(FString::FromInt(Index));
}