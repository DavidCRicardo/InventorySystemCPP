// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BoneSword.h"

ABoneSword::ABoneSword() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> LocalStaticMesh(TEXT("/Game/Items/Equipment/Weapons/Meshes/SM_Weapon_Bone_Sword.SM_Weapon_Bone_Sword"));
	StaticMesh->SetStaticMesh(LocalStaticMesh.Object);

	Name = LOCTABLE(COMMON_WORDS2, "ID_BoneSword");
	Action = LOCTABLE(COMMON_WORDS2, "Use");

	ID = FName("ID_BoneSword");
}
