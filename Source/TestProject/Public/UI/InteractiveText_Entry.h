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
	virtual void DoubleClick();

	UFUNCTION()
	virtual void OnClickByLambda();

	UFUNCTION()
	virtual void ResetBorder();
	
	UFUNCTION()
	void IsHoveredChanged(bool bIsHovered);

	UFUNCTION()
	void InitializeName(const FName& Name);
	FName GetIDName();

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	UBorder* SelectedBorder;*/
	
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
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	//UButton* Button_InteractiveText;

	UPROPERTY()
	class AMyPlayerController* PC;
	
	UFUNCTION()
	void OnClick();

	UFUNCTION()
	void OnRelease();

	UFUNCTION()
	void OnPressed();

	///UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(BindWidget))
	//class UBorder* Border_ToChangeColor;
};