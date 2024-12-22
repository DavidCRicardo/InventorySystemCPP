#include "UI/InteractiveText_Panel.h"
#include "MyPlayerController.h"
#include "Components/Image.h"
#include "Components/ListView.h"
#include "UI/InteractiveText_Entry.h"

void UInteractiveText_Panel::NativeConstruct()
{
	Super::NativeConstruct();

	MyPlayerController = Cast<AMyPlayerController>(GetOwningPlayer());
	if (!IsValid(MyPlayerController))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("InteractiveText_Panel: PlayerController Not Valid")));
	}
	
	FDelegateHandle DelegateHandleOnItemClicked = InteractiveText_List->OnItemClicked().AddLambda([this](UObject* Object)
	{
		if (UInteractiveText_Entry* Entry = Cast<UInteractiveText_Entry>(Object))
		{
			const FName ItemName = Entry->GetEntryText();
			MyPlayerController->CollectFromPanel(ItemName);
		}
	});
	//InteractiveText_List->OnItemClicked().Remove(DelegateHandle);

	FDelegateHandle DelegateHandleOnItemIsHoveredChanged = InteractiveText_List->OnItemIsHoveredChanged().AddLambda([this](UObject* Object, bool IsHovered)
	{
		if (UInteractiveText_Entry* Entry = Cast<UInteractiveText_Entry>(Object))
		{
			InteractiveText_List->SetSelectedItem(Entry);
		}
	});

	ManageListVisibility();
}

void UInteractiveText_Panel::ManageListVisibility()
{
	if (InteractiveText_List->GetNumItems() > 0)
	{
		SetSelectedItemOnInteractiveList();

		SetVisibility(ESlateVisibility::Visible);
	}
	else{
		SetVisibility(ESlateVisibility::Collapsed);
	} 
}

void UInteractiveText_Panel::AddEntryToList(UInteractiveText_Entry* NewEntry)
{
	if (NewEntry)
	{
		InteractiveText_List->AddItem(NewEntry);

		ManageListVisibility();
	}
}

void UInteractiveText_Panel::RemoveEntryFromList(const FName& ID)
{
	TArray<UObject*> InListItems = InteractiveText_List->GetListItems();

	for(UObject*& Object : InListItems)
	{
		if (UInteractiveText_Entry* Entry = Cast<UInteractiveText_Entry>(Object))
		{
			if (Entry->GetEntryText() == ID)
			{
				InteractiveText_List->RemoveItem(Object);

				ManageListVisibility();
				
				return;
			}
		}
	}
}

void UInteractiveText_Panel::SetSelectedItemOnInteractiveList(){
	
	UObject* Object = InteractiveText_List->GetItemAt(0);
	InteractiveText_List->SetSelectedItem(Object);
}

uint32 UInteractiveText_Panel::GetSelectedItemOnInteractiveList()
{
	uint32 Index = 0;
	for (UObject* Object : InteractiveText_List->GetListItems())
	{
		if (Object == InteractiveText_List->GetSelectedItem<UObject>())
		{
			return Index;
		}
		Index++;
	}
	return Index;
}