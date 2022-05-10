﻿#pragma once

#include "CoreMinimal.h"
#include "SlotLayout.h"
#include "WindowLayout.h"
#include "Components/ListView.h"
#include "ProfileLayout.generated.h"

UCLASS()
class INVENTORYSYSTEMCPP_API UProfileLayout : public UWindowLayout
{
	GENERATED_BODY()
	
public:
	UProfileLayout();
	
	virtual void ToggleWindow() override;
	virtual void RefreshWindow() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UUniformGridPanel* EquipmentGridPanel;
	
	UPROPERTY()
	TArray<USlotLayout*>EquipmentSlotsArray;

	UPROPERTY()
	AMyPlayerController* PlayerController;

	UPROPERTY(meta = (BindWidget))
	UListView* Attributes_ListView;

private:
	virtual void NativeConstruct() override;
	virtual void CreateChildWidgets() override;
	virtual void InitializeSlots() override;
	virtual void OnButtonQuitClicked() override;

	UFUNCTION()
	void CreateAttributesEntry();
	
	UFUNCTION()
	void UpdatePlayerStatsUI();

	UPROPERTY()
	uint8 NumberOfColumns;
	UPROPERTY()
	uint8 NumberOfRows;
};
