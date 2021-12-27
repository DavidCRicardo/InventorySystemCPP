// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemDataTable.h"
#include "Item.generated.h"

UCLASS()
class TESTPROJECT_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Amount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FItemDataTable Item_Data;

	UPROPERTY()
	UDataTable* MyDataTable;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
