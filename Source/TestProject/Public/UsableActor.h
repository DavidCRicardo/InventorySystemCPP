// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActorInterface.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "UsableActor.generated.h"

UCLASS()
class TESTPROJECT_API AUsableActor : public AActor, public IUsableActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUsableActor();

	/* Interface */
	virtual bool OnActorUsed_Implementation(APlayerController* Controller) override;
	virtual bool BeginOutlineFocus_Implementation() override;
	virtual bool EndOutlineFocus_Implementation() override;
	virtual bool GetIsActorUsable_Implementation() override;
	virtual FText GetUseActionText_Implementation() override;
	/* Ends Interface */
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent* Scene;
	
	UPROPERTY()
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(Replicated)
	FName Name; // = "NULL"

	UPROPERTY()
	FText Action; // = "Use";

	UPROPERTY()
	USoundCue* UsedSound;
	
	UPROPERTY(Replicated)
	bool IsUsable; // = true;

	UPROPERTY(ReplicatedUsing="OnRep_WasUsed")
	bool WasUsed;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Server, Reliable, WithValidation)
	void OnRep_WasUsed();
	bool OnRep_WasUsed_Validate();
	void OnRep_WasUsed_Implementation();
};
