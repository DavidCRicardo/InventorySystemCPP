// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void IsAnyWidgetVisible();
	
protected:
	AMyHUD();
	
	virtual void DrawHUD() override;
	
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, Category = "Default")
	UUserWidget* HUDReference;
};
