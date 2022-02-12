// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FWidgetsLayoutBP.h"
#include "GameFramework/HUD.h"
#include "UI/ELayout.h"
#include "MyHUD.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class TESTPROJECT_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION()
	bool IsAnyWidgetVisible();

	UFUNCTION()
	void ToggleWindow(const ELayout Layout);
	
	UFUNCTION()
	void RefreshWidgetUILayout(ELayout Layout);
	
	FWidgetsLayoutBP* GetWidgetBPClass(FName Name);

protected:
	AMyHUD();
	
	virtual void DrawHUD() override;
	
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Default")
	class UHUDLayout* HUDReference;

	UPROPERTY(EditAnywhere, Category = "Default")
	class UInventoryLayout* InventoryLayout;
	
	UPROPERTY(EditAnywhere, Category = "Default")
	class UProfileLayout* ProfileLayout;
	
	UPROPERTY()
    UDataTable* WidgetDB;
};
