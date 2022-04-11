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
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("OnItemClicked")));

		if (UInteractiveText_Entry* Entry = Cast<UInteractiveText_Entry>(Object))
		{
			//Entry->OnClickByLambda();

			const FName ItemName = Entry->GetIDName();
			//MyPlayerController->InventoryManagerComponent->AddItem(ItemName, 1);
			MyPlayerController->CollectFromPanel(ItemName);
			
			InteractiveText_List->RemoveItem(Entry);
			
			//ManageListVisibility();
		}
	});
	//InteractiveText_List->OnItemClicked().Remove(DelegateHandle);

	FDelegateHandle DelegateHandleOnItemIsHoveredChanged = InteractiveText_List->OnItemIsHoveredChanged().AddLambda([this](UObject* Object, bool IsHovered)
	{
		if (UInteractiveText_Entry* Entry = Cast<UInteractiveText_Entry>(Object))
		{
			Entry->IsHoveredChanged(IsHovered);

			InteractiveText_List->SetSelectedItem(Entry);
		}
	});

	FDelegateHandle DelegateHandleOnItemDoubleClicked = InteractiveText_List->OnItemDoubleClicked().AddLambda([](UObject* Object)
	{
		if (UInteractiveText_Entry* Entry = Cast<UInteractiveText_Entry>(Object))
		{
			Entry->DoubleClick();
		}
	});

	FDelegateHandle DelegateHandleOnItemSelectionChanged = InteractiveText_List->OnItemSelectionChanged().AddLambda([](UObject* Object)
	{
		if (UInteractiveText_Entry* Entry = Cast<UInteractiveText_Entry>(Object))
		{
			
		}
	});
}

void UInteractiveText_Panel::ManageListVisibility()
{
	if (InteractiveText_List->GetNumItems() == 0)
	{
		SetVisibility(ESlateVisibility::Hidden);
	} else
	{
		SetVisibility(ESlateVisibility::Visible);
	}
}

void UInteractiveText_Panel::AddEntryToList(UInteractiveText_Entry* NewEntry)
{
	if (NewEntry)
	{
		InteractiveText_List->AddItem(NewEntry);
		
		//ManageListVisibility();
	}
}

void UInteractiveText_Panel::RemoveEntryFromList(const FName& ID)
{
	TArray<UObject*> InListItems = InteractiveText_List->GetListItems();

	for(UObject*& Object : InListItems)
	{
		if (UInteractiveText_Entry* Entry = Cast<UInteractiveText_Entry>(Object))
		{
			if (Entry->GetIDName() == ID)
			{
				InteractiveText_List->RemoveItem(Object);
				return;
			}
		}
	}
}
