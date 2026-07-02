// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"   
#include "Rod3ntPlayer.generated.h"

UCLASS()
class PROJECTROD3NTTEST_API ARod3ntPlayer : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARod3ntPlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	class UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	class UBaseAttributes* BaseAttributes;

	// --- NEW AUDIO SECTION ---
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundBase* SpawnGlideSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* SpawnGlideAudioComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	class USoundBase* RegularGlideSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* RegularGlideAudioComp;

	//Climbing Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
	float ClimbingTraceDistance = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Climbing")
	bool bIsFacingWall = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Climbing")
	FHitResult WallHitResult;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Climbing")
	bool bIsClimbingTraceActive = false;

	//Throw Variables

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Throwing")
	float ThrowSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwing")
	float ThrowFactor = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Throwing")
	float ThrowChargeLimit = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Throwing")
	bool bChargingThrow = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Throwing")
	bool bCanThrow = false;

	//Gliding Variables

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gliding")
	bool bAmISquirrel = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gliding")
	bool bIsGliding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gliding")
	float GlideTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gliding")
	float GlideTimeLimit = 5.0f;

	//Glide Time Physics Variables

	float GlideGravityScale = 0.07f;
	float GlideAirControl = 0.7f;
	float NormalGravityScale = 1.0f; //default gravity
	float NormalAirControl = 0.2f; //default air control
	float NormalBrakingDeceleration = 1500.0f; //default falling braking decel


	//Spawning Gliding Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Glide", meta = (ExposeOnSpawn = "true"))
	bool bStartwithSpawnGlide = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Glide")
	bool bIsSpawnGliding = false;

	//Glide speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Glide")
	float SpawnGlideSpeed = 2500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Glide")
	float SpawnTurnInterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Glide")
	float SpawnMinDescentSpeed = 150.0f;

	//Wind Support
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn Glide|Wind")
	FVector CurrentWindDirection;

	UFUNCTION(BlueprintCallable, Category = "Spawn Glide|Wind")
	void SetWindVelocity(FVector NewWind);

	//Distance Checking variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Distance Check")
	float DistanceFromGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance Check")
	float MaxGroundDistance = 100000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance Check")
	float InitalDistancefromGround;

	//This is for the UI Slider
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance Check")
	float SmoothenDistanceFromGround;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Distance Check")
	float SmoothingSpeed = 5.0f;

protected:
	// Called when the game starts or when spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	EGameplayEffectReplicationMode ASCReplicationMode = EGameplayEffectReplicationMode::Mixed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//Physics Function for the Spawn Gliding
	void HandleSpawnGlidePhysics(float DeltaTime);

	//Distance Check Function
	void UpdateGroundDistance();

	FTimerHandle GroundDistanceTimerHandle;


	//Optamizing Climb Trace with a Timer instead of ticking every frame
	void PerformClimbingTrace();
	FTimerHandle ClimbingTraceTimerHandle;

	//Cached Character Movement Component for easy access
	UPROPERTY()
	class UCharacterMovementComponent* CachedMoveComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "GAS")
	TArray<FGameplayAbilitySpecHandle> GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant);

	UFUNCTION(BlueprintCallable, Category = "GAS")
	void RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilityHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category = "GAS")
	void SendAbilitiesChangedEvent();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "GAS")
	void ServerSendGameplayEventToSelf(FGameplayEventData EventData);

	UFUNCTION(BlueprintCallable, Category = "Climbing")
	bool ClimbingTrace(FHitResult& OutHit);

	UFUNCTION(BlueprintCallable, Category = "Climbing")
	void StartClimbingTrace();

	UFUNCTION(BlueprintCallable, Category = "Climbing")
	void StopClimbingTrace();

	UFUNCTION(BlueprintImplementableEvent, Category = "Climbing")
	void StopClimbing();

	UFUNCTION(BlueprintImplementableEvent, Category = "Climbing")
	void AutoClimbStart();

	UFUNCTION(BlueprintCallable, Category = "Throwing")
	void ChargeThrow(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Throwing")
	void StartThrowCharge();

	UFUNCTION(BlueprintCallable, Category = "Throwing")
	void ReleaseThrow();

	UFUNCTION(BlueprintCallable, Category = "Gliding")
	void GlideLogic(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "Gliding")
	void StartGliding();

	UFUNCTION(BlueprintCallable, Category = "Gliding")
	void StopGliding();

	UFUNCTION(BlueprintCallable, Category = "Distance Check")
	void StartGroundDistanceUpdates(float Rate = 0.05f);

	UFUNCTION(BlueprintCallable, Category = "Distance Check")
	void StopGroundDistanceUpdates();

	UFUNCTION(BlueprintCallable, Category = "Spawn Glide")
	void HandleLanding();

};