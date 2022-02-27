// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UsableActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UUsableActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TESTPROJECT_API IUsableActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	bool OnActorUsed(APlayerController* Controller);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	bool BeginOutlineFocus();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	bool EndOutlineFocus();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	bool GetIsActorUsable();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Default")
	FText GetUseActionText();
};
