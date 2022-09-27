// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/UsableDoor.h"
#include "Internationalization/StringTableRegistry.h"
#include "UI/InteractText.h"
#include "MyGameInstance.h"
#include "UI/InteractiveText_Entry.h"
#include "UI/InteractiveText_Panel.h"

AUsableDoor::AUsableDoor() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> LocalStaticMesh(TEXT("/Game/Environment/Meshes/SM_Door.SM_Door"));
	StaticMesh->SetStaticMesh(LocalStaticMesh.Object);

	static ConstructorHelpers::FObjectFinder<USoundCue> LocalSoundCue(TEXT("/Game/Environment/Sounds/Environment/SFX_Door_Cue.SFX_Door_Cue"));
	UsedSound = LocalSoundCue.Object;

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
		Action = CloseText;
		
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		LocalRotation = -90;
	}
	else {
		Action = OpenText;
		
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		LocalRotation = 90;
	}


	//SetInteractText(IUsableActorInterface::Execute_GetUseActionText(this));
	UInteractiveText_Panel* Panel = Cast<UInteractiveText_Panel>(InteractUserWidget);
	UListView* ListView = Panel->InteractiveText_List;
	UInteractiveText_Entry* Entry = Cast<UInteractiveText_Entry>(ListView->GetItemAt(0));
	Entry->SetNameLabelText(IUsableActorInterface::Execute_GetUseActionText(this));
	ListView->RegenerateAllEntries();

	FRotator Rotator = StaticMesh->GetComponentRotation();	
	
	FRotator NewRotator = FRotator(Rotator.Roll, Rotator.Yaw + LocalRotation, Rotator.Pitch);

	StaticMesh->SetWorldRotation(NewRotator);
	

	return Super::OnWasUsed();
}

void AUsableDoor::BeginPlay()
{
	FName InTableID = Cast<UMyGameInstance>(GetGameInstance())->COMMON_WORDS;
	Name = FText::FromStringTable(InTableID, "Door");

	OpenText = FText::FromStringTable(InTableID, "Open");
	CloseText = FText::FromStringTable(InTableID, "Close");
	Action = OpenText;
}