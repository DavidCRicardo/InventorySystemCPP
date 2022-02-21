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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_MainWeaponMesh)
	USkeletalMeshComponent* Weapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Chest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, NonTransactional, meta=(Category="Default", OverrideNativeName="MainFeet"))
	USkeletalMeshComponent* MainFeet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, NonTransactional, meta=(Category="Default", OverrideNativeName="MainHands"))
	USkeletalMeshComponent* MainHands;
	
	UFUNCTION()
	void SetWeaponMesh();
	//UFUNCTION()
	//void SetChestMesh();
	/*UFUNCTION()
	void SetFeetMesh();
	UFUNCTION()
	void SetHandsMesh();*/
	
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
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing=OnRep_MainWeaponMesh, meta=(DisplayName="Main Weapon Mesh", Category="Inventory|Equipment"))
	USkeletalMesh* WeaponMesh;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing="OnRep_MainChestMesh", meta=(DisplayName="Main Chest Mesh", Category="Inventory|Equipment"))
	USkeletalMesh* ChestMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing="OnRep_MainFeetMesh", meta=(DisplayName="Main Feet Mesh", Category="Inventory|Equipment", OverrideNativeName="MainFeetMesh"))
	USkeletalMesh* MainFeetMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, ReplicatedUsing="OnRep_MainHandsMesh", meta=(DisplayName="Main Hands Mesh", Category="Inventory|Equipment", OverrideNativeName="MainHandsMesh"))
	USkeletalMesh* MainHandsMesh;
	
	UFUNCTION()
	void UpdateMainWeaponMesh(USkeletalMesh* NewMesh);

	UFUNCTION()
	void UpdateChestMesh(USkeletalMesh* NewMesh);
	
	UFUNCTION()
	void OnRep_MainWeaponMesh();

	UFUNCTION()
	void OnRep_MainChestMesh();
	
	UFUNCTION(meta=(OverrideNativeName="OnRep_MainFeetMesh"))
	void OnRep_MainFeetMesh();
	
	UFUNCTION(meta=(OverrideNativeName="OnRep_MainHandsMesh"))
	void OnRep_MainHandsMesh();

	/** The player's maximum health. This is the highest that their health can be, and the value that their health starts at when spawned.*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	/** The player's current health. When reduced to 0, they are considered dead.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_CurrentHealth)
	float CurrentHealth;

	/** RepNotify for changes made to current health.*/
	UFUNCTION()
	void OnRep_CurrentHealth();

	/** Getter for Max Health.*/
	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; } 

	/** Getter for Current Health.*/
	UFUNCTION(BlueprintPure, Category="Health")
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	/** Setter for Current Health. Clamps the value between 0 and MaxHealth and calls OnHealthUpdate. Should only be called on the server.*/
	UFUNCTION(BlueprintCallable, Category="Health")
	void SetCurrentHealth(float healthValue);

protected:
	/** Response to health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnHealthUpdate();
};