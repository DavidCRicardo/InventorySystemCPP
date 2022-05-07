#pragma once

#include "CoreMinimal.h"
#include "SlotLayout.h"
#include "WindowLayout.h"
#include "Components/VerticalBox.h"
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
	UVerticalBox* AttributesBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* StrengthValue;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* EnduranceValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* DexterityValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* IntelligenceValue;
	
private:
	virtual void NativeConstruct() override;
	virtual void CreateChildWidgets() override;
	virtual void InitializeSlots() override;
	virtual void OnButtonQuitClicked() override;

	UFUNCTION()
	void InitializePlayerStatsUI();
	
	UFUNCTION()
	void UpdatePlayerStatsUI();

	UPROPERTY()
	uint8 NumberOfColumns;
	UPROPERTY()
	uint8 NumberOfRows;
};
