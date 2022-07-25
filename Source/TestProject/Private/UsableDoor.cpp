// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableDoor.h"

AUsableDoor::AUsableDoor() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> LocalStaticMesh(TEXT("/Game/Environment/Meshes/SM_Door.SM_Door"));
	StaticMesh->SetStaticMesh(LocalStaticMesh.Object);

	static ConstructorHelpers::FObjectFinder<USoundCue> LocalSoundCue(TEXT("/Game/Environment/Sounds/Environment/SFX_Door_Cue.SFX_Door_Cue"));
	UsedSound = LocalSoundCue.Object;

	Name = FText::FromString("Door");
	Action = FText::FromString("Open");


	StaticMesh->SetMobility(EComponentMobility::Movable);
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	StaticMesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;	
	StaticMesh->SetCanEverAffectNavigation(true);
}

bool AUsableDoor::OnWasUsed()
{
	float LocalRotation = 0.f;

	if (WasUsed)
	{
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		LocalRotation = -90;
	}
	else {
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		LocalRotation = 90;
	}

	FRotator Rotator = StaticMesh->GetComponentRotation();	
	
	FRotator NewRotator = FRotator(Rotator.Roll, Rotator.Yaw + LocalRotation, Rotator.Pitch);

	StaticMesh->SetWorldRotation(NewRotator);
	

	return Super::OnWasUsed();
}