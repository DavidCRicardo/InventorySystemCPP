// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_GenerateRandomNumber.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>

void UWidget_GenerateRandomNumber::NativeConstruct()
{
	Super::NativeConstruct();

	GenerateRandom();

	GenerateButton->OnClicked.AddUniqueDynamic(this, &UWidget_GenerateRandomNumber::OnGenerateButtonClicked);
}

void UWidget_GenerateRandomNumber::GenerateRandom() 
{
	int32 RandomNumber = FMath::RandRange(0, 100);
	RandomNumberLabel->SetText(FText::AsNumber(RandomNumber));
}

void UWidget_GenerateRandomNumber::OnGenerateButtonClicked()
{
	GenerateRandom();
}