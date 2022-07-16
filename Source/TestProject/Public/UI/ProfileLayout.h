﻿#pragma once

#include "CoreMinimal.h"
#include "WindowLayout.h"
#include "Components/ListView.h"
#include "ProfileLayout.generated.h"

class USlotLayout;

UCLASS()
class INVENTORYSYSTEMCPP_API UProfileLayout : public UWindowLayout
{
	GENERATED_BODY()
	
public:
	UProfileLayout();
	
	virtual void ToggleWindow() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UUniformGridPanel* EquipmentGridPanel;
	
	UPROPERTY()
	TArray<USlotLayout*>EquipmentSlotsArray;

	UPROPERTY(meta = (BindWidget))
	UListView* Attributes_ListView;
	
	UFUNCTION()
	void UpdatePlayerStatsUI(const TArray<uint8>& InAttributesArray);
private:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CreateAttributesEntry();
};
