// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FItemStructure.h"
#include "Blueprint/UserWidget.h"
#include "SlotLayout.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API USlotLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	USlotLayout(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemID;

	UFUNCTION()
	void UpdateSlot(FItemStructure ItemStructure, uint8 Amount);

	UFUNCTION()
	void InitializeSlot(UTexture2D* BackgroundRef);
	
protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;
	
	UPROPERTY(meta = (BindWidget))
	class UBorder* SlotBorder;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* AmountTextBlock;
	
	UFUNCTION(BlueprintSetter, BlueprintCallable)
	void SetAmountText(const FText& _newAmount);

	UFUNCTION(BlueprintGetter, BlueprintCallable)
	FText GetAmountText();

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<UTexture2D> WidgetClass;

	//UPROPERTY(EditAnywhere, Category = "Default")
	//UTexture2D* Background;

private:
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
	UImage* Background;
};