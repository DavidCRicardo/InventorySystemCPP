// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/FItemStructure.h"
#include "MyWeaponItem.generated.h"

UCLASS()
class TESTPROJECT_API AMyWeaponItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyWeaponItem();

	UPROPERTY(BlueprintReadWrite, EditAnywhere);
	FItemStructure Item;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere);
	uint8 CurrentLevel;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere);
	uint8 MaxLevel;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere);
	float BaseDamage;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere);
	uint8 Value;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
