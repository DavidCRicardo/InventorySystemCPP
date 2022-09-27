// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDLayout.generated.h"

class UButton;
class UBorder;
class UTertiaryHUD;
class UMainLayout;
class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UHUDLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* Border_Profile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UBorder* Border_Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_Profile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMainLayout* MainLayout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTertiaryHUD* TertiaryHUD;

	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	UWidgetAnimation* Btn_Inventory_Animation;
	UPROPERTY(Transient, BlueprintReadWrite, meta = (BindWidgetAnim))
	UWidgetAnimation* Btn_Profile_Animation;
	
protected:
	UFUNCTION()
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SlipUpInventory();
	UFUNCTION()
	void SlipUpProfile();

	UFUNCTION()
	void SlipDownInventory();
	UFUNCTION()
	void SlipDownProfile();

	UFUNCTION()
	void OnBtnUIClicked();
};