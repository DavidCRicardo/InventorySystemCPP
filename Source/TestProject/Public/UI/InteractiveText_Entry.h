// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "InteractiveText_Entry.generated.h"

class UTextBlock;
class UImage;
class UBorder;
UCLASS()
class UInteractiveText_Entry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetEntryText(const FName& Name);

	UFUNCTION()
	FName GetEntryText();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* NameLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UImage* SelectedImage;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	//UBorder* MainBorder;

protected:
	UFUNCTION()
	virtual void NativeConstruct() override;
	
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry
	
	virtual void NativeOnEntryReleased() override;

	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UImage* IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	FName ID;

	UPROPERTY()
	class AMyPlayerController* PC;

	UPROPERTY()
	class UMyGameInstance* GameInstance;
};