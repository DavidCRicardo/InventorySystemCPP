// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SimpleAxe.h"
#include "MyGameInstance.h"

ASimpleAxe::ASimpleAxe() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> LocalStaticMesh(TEXT("/Game/Items/Equipment/Weapons/Meshes/SM_Weapon_Simple_Axe.SM_Weapon_Simple_Axe"));
	StaticMesh->SetStaticMesh(LocalStaticMesh.Object);
}

void ASimpleAxe::BeginPlay()
{
	FName InTableID = Cast<UMyGameInstance>(GetGameInstance())->COMMON_WORDS;
	Name = FText::FromStringTable(InTableID, TEXT("ID_SimpleAxe"));

	ID = FName("ID_SimpleAxe");
}
