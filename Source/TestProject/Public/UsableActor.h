// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActorInterface.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "UsableActor.generated.h"

UCLASS()
class INVENTORYSYSTEMCPP_API AUsableActor : public AActor, public IUsableActorInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUsableActor();

	/* Interface */
	virtual FText GetUseActionText_Implementation() override;
	virtual bool GetIsActorUsable_Implementation() override; 
	virtual bool BeginOutlineFocus_Implementation() override;
	virtual bool EndOutlineFocus_Implementation() override;
	virtual bool OnActorUsed_Implementation(APlayerController* Controller) override;
	/* Ends Interface */
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* Scene;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Action; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* UsedSound;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	bool IsUsable;

	UPROPERTY(Replicated)
	UStaticMesh* WorldMesh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	bool OnWasUsed();

	UFUNCTION(Server, Reliable, WithValidation)
	void OnRep_WasUsed();
	bool OnRep_WasUsed_Validate();
	void OnRep_WasUsed_Implementation();
	
	UPROPERTY()
	UUserWidget* InteractUserWidget;

	UFUNCTION()
	void SetInteractText(FText Text);
	UFUNCTION()
	void SetScreenPosition(FVector2D ScreenPosition);
	
private:
	UPROPERTY(ReplicatedUsing = "OnRep_WasUsed")
	bool WasUsed;
};