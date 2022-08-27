// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class INVENTORYSYSTEMCPP_API AMyCharacter : public ACharacter
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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	class USphereComponent* InteractionField;

	UPROPERTY()
	class AMyPlayerController* MyPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* MainWeapon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Chest;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Feet;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Hands;

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

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TArray<AActor*> UsableActorsInsideRange;
	UPROPERTY(/*Replicated,*/ EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TArray<AActor*> WorldActorsInsideRange;
	
	UPROPERTY(BlueprintReadWrite, ReplicatedUsing="OnRep_MainWeaponMesh", meta=(DisplayName="Weapon Mesh", Category="Inventory|Equipment"))
	USkeletalMesh* MainWeaponMesh;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing="OnRep_MainChestMesh", meta=(DisplayName="Main Chest Mesh", Category="Inventory|Equipment"))
	USkeletalMesh* ChestMesh;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing="OnRep_MainFeetMesh", meta=(DisplayName="Main Feet Mesh", Category="Inventory|Equipment"))
	USkeletalMesh* FeetMesh;

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing="OnRep_MainHandsMesh", meta=(DisplayName="Main Hands Mesh", Category="Inventory|Equipment"))
	USkeletalMesh* HandsMesh;
	
	UFUNCTION(meta=(OverrideNativeName="OnRep_MainWeaponMesh"))
	void OnRep_MainWeaponMesh();

	UFUNCTION(meta=(OverrideNativeName="OnRep_MainChestMesh"))
	void OnRep_MainChestMesh();
	
	UFUNCTION(meta=(OverrideNativeName="OnRep_MainFeetMesh"))
	void OnRep_MainFeetMesh();

	UFUNCTION(meta=(OverrideNativeName="OnRep_MainHandsMesh"))
	void OnRep_MainHandsMesh();

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};