// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/InteractiveText_Entry.h"

#include "MyPlayerController.h"
#include "WorldActor.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Item/FItemStructure.h"

void UInteractiveText_Entry::NativeConstruct()
{
	Super::NativeConstruct();

	//	const UDataTable* ItemTable = ItemDB;
	//	FItemStructure* NewItemData = ItemTable->FindRow<FItemStructure>(FName(ID), "", true);

	if (IsValid(NameLabel))
	{
		NameLabel->SetText(FText::FromName("Default Text"));
		
		SelectedImage->SetOpacity(0.f);
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Not Valid")));
	}

	PC = Cast<AMyPlayerController>(GetOwningPlayer());
	if (!IsValid(PC))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller Not Valid")));
	}
	//SelectedBorder->SetBrushColor({1,1,1,0});

	//Button_InteractiveText->OnClicked.AddUniqueDynamic(this, &UInteractiveText_Entry::OnClick);
	//Button_InteractiveText->OnPressed.AddUniqueDynamic(this, &UInteractiveText_Entry::OnPressed);
	//Button_InteractiveText->OnReleased.AddUniqueDynamic(this, &UInteractiveText_Entry::OnRelease);
}

void UInteractiveText_Entry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	SelectedImage->SetOpacity(0.f);
	NameLabel->SetText(FText::FromName("Default"));
	
	if (UInteractiveText_Entry* Entry = Cast<UInteractiveText_Entry>(ListItemObject))
	{
		if (IsValid(Entry) && IsValid(PC))
		{
			const UDataTable* DataTable = PC->GetItemDB();
			if (IsValid(DataTable))
			{
				FItemStructure* NewItemData = DataTable->FindRow<FItemStructure>(FName(Entry->ID), "", true);
				if (NewItemData)
				{
					IconImage->SetBrushFromTexture(NewItemData->Icon);
					NameLabel->SetText(NewItemData->Name);
				}
			}		
		}
	}
}

void UInteractiveText_Entry::NativeOnEntryReleased()
{
	IUserObjectListEntry::NativeOnEntryReleased();

	const FLinearColor LinearColor = {1,1,1,1};

	//SelectedBorder->SetBrushColor(LinearColor);

	//SelectedImage->SetOpacity(1.f);

}

void UInteractiveText_Entry::NativeOnItemSelectionChanged(bool bIsSelected)
{
	IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("NativeOnItemSelectionChanged")));
	
	FLinearColor LinearColor;
	
	if (bIsSelected)
	{
		LinearColor = {0.75,0.25,0,1};

		//SelectedBorder->SetBrushColor(LinearColor);
		SelectedImage->SetOpacity(1.f);

		
	}else{
		LinearColor = {1,1,1,0};
		
		//SelectedBorder->SetBrushColor(LinearColor);
		SelectedImage->SetOpacity(0.f);

	}
}

void UInteractiveText_Entry::IsHoveredChanged(bool bIsHovered)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("bIsHovered")));

	/*if (bIsHovered)
	{
		NameLabel->SetText(FText::FromName("IsHovered"));
		
		//SelectedBorder->SetBrushColor({1,1,1,1});
		SelectedImage->SetOpacity(1.f);

	}else
	{
		NameLabel->SetText(FText::FromName("IsNotHovered"));

		//SelectedBorder->SetBrushColor({1,1,1,0});
		SelectedImage->SetOpacity(0.f);
	}*/
}

void UInteractiveText_Entry::InitializeName(const FName& Name)
{
	ID = Name;
}

FName UInteractiveText_Entry::GetIDName()
{
	return ID;
}

void UInteractiveText_Entry::DoubleClick()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("NativeOnMouseButtonDoubleClick")));
	
	//NameLabel->SetText(FText::FromName("Double Click"));
}

void UInteractiveText_Entry::OnClick()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("OnClick")));

	//NameLabel->SetText(FText::FromName("Click"));
}
void UInteractiveText_Entry::OnRelease()
{
	//NameLabel->SetText(FText::FromName("Release"));
}
void UInteractiveText_Entry::OnPressed()
{
	//NameLabel->SetText(FText::FromName("Pressed"));
}
void UInteractiveText_Entry::OnClickByLambda()
{
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("OnClickByLambda")));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("Used!")));

	//NameLabel->SetText(FText::FromName("OnClickByLambda"));
}

void UInteractiveText_Entry::ResetBorder()
{
	//SelectedBorder->SetBrushColor({1,1,1,0});
	
}
