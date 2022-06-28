// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWidgetsLayoutBP.h"
#include "GameFramework/HUD.h"
#include "UI/ELayout.h"
#include "UI/HUDLayout.h"
#include "MyHUD.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION()
	bool IsAnyWidgetVisible();

	UFUNCTION()
	void ToggleWindow(const ELayout Layout);

	FWidgetsLayoutBP* GetWidgetBPClass(FName Name);
	
	UFUNCTION()
	UUserWidget* GetInteractWidget();

	UPROPERTY(EditAnywhere, Category = "Default")
	UHUDLayout* HUDReference;
	
protected:
	AMyHUD();
	
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	UDataTable* WidgetDB;
	
	UUserWidget* CreateWidgetFromDataTable(const UDataTable* WidgetTable, FWidgetsLayoutBP*& NewWidgetData, FName Name);
	
	UPROPERTY(EditAnywhere, Category = "Default")
	UUserWidget* InteractTextWidget;
};