// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class TESTPROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;	
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character")
	float BaseTurnRate;

	/** Base lookup rate, in deg/sec. Other scaling may affect final lookup rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character")
	float BaseLookUpRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")
	class USphereComponent* InteractionField;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category="Character")
	virtual void MoveForward(float Val);

	UFUNCTION(BlueprintCallable, Category="Character")
	virtual void MoveRight(float Val);

	UFUNCTION(BlueprintCallable, Category="Character")
	virtual void MoveUp_World(float Val);

	UFUNCTION(BlueprintCallable, Category="Character")
	virtual void TurnAtRate(float Rate);

	UFUNCTION(BlueprintCallable, Category="Character")
	virtual void LookUpAtRate(float Rate);
	
	void InitializeDefaultPawnInputBindings();
	

	
	UFUNCTION()
	void SetHandMesh();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, NonTransactional, meta=(Category="Default", OverrideNativeName="MainHand"))
	USkeletalMeshComponent* MainHand;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing="OnRep_MainHandMesh", meta=(DisplayName="Main Hand Mesh", Category="Inventory|Equipment", OverrideNativeName="MainHandMesh"))
	USkeletalMesh* MainHandMesh;

	UFUNCTION(meta=(Category, OverrideNativeName="OnRep_MainHandMesh"))
	virtual void OnRep_MainHandMesh();
	
};