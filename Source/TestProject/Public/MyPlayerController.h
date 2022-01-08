// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InventoryComponent.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();
	
	virtual void SetPawn(APawn* InPawn) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(Category="Default", OverrideNativeName="InventoryComponent"))
	UInventoryComponent* InventoryComponent;
	
	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleInventory();

	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleMenu();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Default")
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, Category = "Default")
	class UInventoryLayout* W_InventoryLayout;
	
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category="Character")
	void Interact();

	void InitializeInventoryLayout();
	
	void PrintInventory();
};