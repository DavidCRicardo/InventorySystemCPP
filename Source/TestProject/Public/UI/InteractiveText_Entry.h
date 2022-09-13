// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "InteractiveText_Entry.generated.h"

UCLASS()
class UInteractiveText_Entry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void InitializeName(const FName& Name);

	UFUNCTION()
	FName GetIDName();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UImage* SelectedImage;
	
protected:
	UFUNCTION()
	virtual void NativeConstruct() override;
	
	// IUserObjectListEntry
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	// IUserObjectListEntry
	
	virtual void NativeOnEntryReleased() override;

	virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* NameLabel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UImage* IconImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	FName ID;
	
	UPROPERTY()
	class AMyPlayerController* PC;

	UPROPERTY()
	class UMyGameInstance* GameInstance;
};