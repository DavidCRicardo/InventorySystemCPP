// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractiveText_Panel.h"
#include "InteractText.h"
#include "Blueprint/UserWidget.h"
#include "TertiaryHUD.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UTertiaryHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInteractText* InteractText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInteractiveText_Panel* InteractiveMenu;

	UPROPERTY()
	AMyPlayerController* PlayerController;

	UFUNCTION()
	void CreateInteractiveTextEntry(FName IDName);
	UFUNCTION()
	void RemoveInteractiveTextEntry(const FName& ID);

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	//TSubclassOf<UInteractiveText_Entry> InteractiveTextWidgetClass = nullptr;
	
protected:
	UFUNCTION()
	virtual void NativeConstruct() override;
	
};
