// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

#include "Blueprint/UserWidget.h"

AMyHUD::AMyHUD()
{
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("HUD Begin Play!"));

	/*FStringClassReference MyWidgetClassRef(TEXT("/Game/UI/BP_HUDLayout.BP_HUDLayout_C"));
	if ( UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		HUDReference = CreateWidget<UUserWidget>(GetWorld(), MyWidgetClass);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("HUD Create Widget!"));

		if (HUDReference)
		{
			HUDReference->AddToViewport();

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("HUD Added to Viewport!"));
		}
	}else
	{
		UE_LOG (LogTemp, Warning, TEXT ("MyHUD Cannot Load Class!!"));
	}*/
}

void AMyHUD::Test()
{
	if (GEngine) 
    {
    	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Test!"));
    }
}