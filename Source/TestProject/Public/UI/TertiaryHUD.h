// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractText.h"
#include "Blueprint/UserWidget.h"
#include "TertiaryHUD.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UTertiaryHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UInteractText* InteractText;

protected:
	UFUNCTION()
	virtual void NativeConstruct() override;
};
