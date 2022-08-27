﻿#pragma once

#include "CoreMinimal.h"
#include "InteractiveText_Entry.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "InteractiveText_Panel.generated.h"

UCLASS(Abstract)
class UInteractiveText_Panel : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void AddEntryToList(UInteractiveText_Entry* NewEntry);
	UFUNCTION()	
	void RemoveEntryFromList(const FName& ID);
	UFUNCTION()
	void SetSelectedItemOnInteractiveList();
	UFUNCTION()
	uint32 GetSelectedItemOnInteractiveList();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = ( BindWidget ) )
	UListView* InteractiveText_List;
	
protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	AMyPlayerController* MyPlayerController;
	
	UFUNCTION()
	void ManageListVisibility();
};