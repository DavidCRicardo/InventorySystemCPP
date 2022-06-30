// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDLayout.generated.h"

class UTertiaryHUD;
class UMainLayout;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API UHUDLayout : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMainLayout* MainLayout;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTertiaryHUD* TertiaryHUD;
	
protected:
	UFUNCTION()
	virtual void NativeConstruct() override;
};
