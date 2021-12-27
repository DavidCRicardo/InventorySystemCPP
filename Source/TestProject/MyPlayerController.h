// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
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
	
	UFUNCTION(BlueprintCallable, Category="Character")
	void ToggleInventory();
	
protected:
	UPROPERTY(EditAnywhere, Category = "Default")
	class TSubclassOf<class UUserWidget> WidgetClass;

	UPROPERTY(EditAnywhere, Category = "Default")
	class UUserWidget* W_InventoryLayout;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(Category="Default", OverrideNativeName="InventoryComponent"))
	class UInventoryComponent* InventoryComponent;
	
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable, Category="Character")
	void Interact();
	
	
};
