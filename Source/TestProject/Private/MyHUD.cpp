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

	const FStringClassReference MyWidgetClassRef(TEXT("/Game/UI/BP_HUDLayout.BP_HUDLayout_C"));
	if ( UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>())
	{
		HUDReference = CreateWidget<UUserWidget>(GetWorld(), MyWidgetClass);
		
		if (HUDReference)
		{
			HUDReference->AddToViewport();
		}
	}else
	{
		UE_LOG (LogTemp, Warning, TEXT ("MyHUD.cpp.BeginPlay():Cannot Load Class!!"));
	}
}

void AMyHUD::IsAnyWidgetVisible()
{
	
}