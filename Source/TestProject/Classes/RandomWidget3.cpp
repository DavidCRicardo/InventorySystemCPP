#include "RandomWidget3.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void URandomWidget3::NativeConstruct()
{
	Super::NativeConstruct();

	GenerateRandom();

	GenerateButton->OnClicked.AddUniqueDynamic(this, &URandomWidget3::OnGenerateButtonClicked);
}

void URandomWidget3::GenerateRandom() 
{
	int32 RandomNumber = FMath::RandRange(100, 200);
	RandomNumberLabel->SetText(FText::AsNumber(RandomNumber));
}

void URandomWidget3::OnGenerateButtonClicked()
{
	GenerateRandom();
}

