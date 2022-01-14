// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_GenerateRandomNumber.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UWidget_GenerateRandomNumber::NativeConstruct()
{
	Super::NativeConstruct();

	GenerateRandom();

	GenerateButton->OnClicked.AddUniqueDynamic(this, &UWidget_GenerateRandomNumber::OnGenerateButtonClicked);
	GenerateButton->OnHovered.AddUniqueDynamic(this, &UWidget_GenerateRandomNumber::OnGenerateButtonOnHovered);
	GenerateButton->OnPressed.AddUniqueDynamic(this, &UWidget_GenerateRandomNumber::OnGenerateButtonOnPressed);
	GenerateButton->OnReleased.AddUniqueDynamic(this, &UWidget_GenerateRandomNumber::OnGenerateButtonOnReleased);
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

void UWidget_GenerateRandomNumber::OnGenerateButtonOnReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("OnGenerateButtonReleased"))
}

void UWidget_GenerateRandomNumber::OnGenerateButtonOnHovered()
{
	UE_LOG(LogTemp, Warning, TEXT("OnGenerateButtonHovered"))
}

void UWidget_GenerateRandomNumber::OnGenerateButtonOnPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("OnGenerateButtonPressed"))
}