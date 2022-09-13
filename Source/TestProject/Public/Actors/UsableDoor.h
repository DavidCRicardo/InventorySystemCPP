// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "UsableDoor.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEMCPP_API AUsableDoor : public AUsableActor
{
	GENERATED_BODY()

	AUsableDoor();

	bool OnWasUsed() override;

protected:
	virtual void BeginPlay() override;

	FText OpenText;
	FText CloseText;
};
