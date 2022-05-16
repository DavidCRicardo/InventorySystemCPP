// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "ContainerActor.generated.h"

UCLASS()
class INVENTORYSYSTEMCPP_API AContainerActor : public AUsableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AContainerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};