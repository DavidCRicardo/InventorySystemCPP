// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActorInterface.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundCue.h"
#include "Internationalization/StringTableRegistry.h"
#include "UsableActor.generated.h"

static const FName LCOMMON_WORDS2 = "/Game/UI/COMMON_WORDS.COMMON_WORDS";

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

	UPROPERTY(Replicated)
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Action; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundCue* UsedSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool IsUsable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = "OnRep_WasUsed")
	bool WasUsed;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, meta = (Category, OverrideNativeName = "OnWasUsed"))
	virtual bool OnWasUsed();

	UFUNCTION(BlueprintCallable, meta = (Category, OverrideNativeName = "OnRep_WasUsed"))
	virtual void OnRep_WasUsed();

	UPROPERTY()
	UUserWidget* InteractUserWidget;

	UFUNCTION()
	void SetInteractText(FText Text);
	UFUNCTION()
	void SetScreenPosition(FVector2D ScreenPosition);
};