// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TertiaryHUD.generated.h"

class AMyPlayerController;
class UInteractiveText_Panel;
class UInteractText;
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
	
protected:
	UFUNCTION()
	virtual void NativeConstruct() override;
};