// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableActor.h"


// Sets default values
AUsableActor::AUsableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool AUsableActor::OnActorUsed_Implementation(APlayerController* Controller)
{
	return IUsableActorInterface::OnActorUsed_Implementation(Controller);
}

bool AUsableActor::BeginOutlineFocus_Implementation()
{
	return IUsableActorInterface::BeginOutlineFocus_Implementation();
}

bool AUsableActor::EndOutlineFocus_Implementation()
{
	return IUsableActorInterface::EndOutlineFocus_Implementation();
}

bool AUsableActor::GetIsActorUsable_Implementation()
{
	return IUsableActorInterface::GetIsActorUsable_Implementation();
}

FText AUsableActor::GetUseActionText_Implementation()
{
	return IUsableActorInterface::GetUseActionText_Implementation();
}

// Called when the game starts or when spawned
void AUsableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUsableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AUsableActor::OnRep_WasUsed_Validate()
{
	return true;
}

void AUsableActor::OnRep_WasUsed_Implementation()
{
}

