// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActor.h"

#include "Net/UnrealNetwork.h"


// Sets default values
AWorldActor::AWorldActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ID = FName(TEXT("None"));
	Amount = 1;
	StartWithPhysicsEnabled = true;
}

// Called when the game starts or when spawned
void AWorldActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AWorldActor::LoadItemFromList()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("LoadItemFromList being called"));

	return true;
}

void AWorldActor::UpdateItemAmount()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("UpdateItemAmount being called"));

}

bool AWorldActor::Server_InitializeItemData_Validate()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Server_InitializeItemData_Validate being called"));

	return true;
}

void AWorldActor::Server_InitializeItemData_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Server_InitializeItemData_Implementation being called"));
}

bool AWorldActor::OnActorUsed_Implementation(APlayerController* Controller)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("OnActorUsed_Implementation being called"));

	return Super::OnActorUsed_Implementation(Controller);
}

void AWorldActor::OnRep_WorldMesh()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("OnRep_WorldMesh being called"));

	StaticMesh->SetStaticMesh(WorldMesh);
}




