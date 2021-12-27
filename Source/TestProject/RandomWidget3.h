#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RandomWidget3.generated.h"

UCLASS()
class TESTPROJECT_API URandomWidget3 : public UUserWidget
{
	GENERATED_BODY()

protected:
	void GenerateRandom();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RandomNumberLabel;

	UPROPERTY(meta = (BindWidget))
	class UButton* GenerateButton;

	UFUNCTION()
	void OnGenerateButtonClicked();
	
	void NativeConstruct() override;
};
