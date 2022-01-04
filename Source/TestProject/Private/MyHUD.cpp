// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

#include "Blueprint/UserWidget.h"

AMyHUD::AMyHUD()
{
	UE_LOG (LogTemp, Warning, TEXT ("MyHUD Initialized!!"));

	//static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetClass(TEXT("/Game/UI/BP_HUDLayout"));
	//WidgetClass = HUDWidgetClass.Class;
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
		
	/*if (WidgetClass != nullptr)
	{
		HUDReference = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("HUD Create Widget!"));

		if (HUDReference)
		{
			HUDReference->AddToViewport();

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("HUD Added to Viewport!"));
		}
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("MyHUD Cannot Load Class!!"));
		UE_LOG (LogTemp, Warning, TEXT ("MyHUD Cannot Load Class!!"));
	}*/

	/*FStringClassReference MyWidgetClassRef(TEXT("/Game/UI/BP_HUDLayout.BP_HUDLayout_C"));
	if ( UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>() )
	{
		UUserWidget* MyWidget = CreateWidget<UUserWidget>(GetWorld(), MyWidgetClass);
		
		if (MyWidget)
		{
			MyWidget->AddToViewport();

			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("HUD Added to Viewport!"));
		}
	}*/
}

void AMyHUD::Test()
{
	if (GEngine) 
    {
    	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Test!"));
    }
}