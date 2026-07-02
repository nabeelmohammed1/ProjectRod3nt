// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "RodentAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTROD3NTTEST_API ARodentAIController : public AAIController
{
	GENERATED_BODY()
	

public:
	ARodentAIController();

	virtual void Tick(float DeltaTime) override;	

	UFUNCTION(BlueprintCallable, Category = "AI Optamize")
	void SetAiActive(bool bNewState);

	//AI Sight Perception Logic

	UFUNCTION(BlueprintCallable, Category = "AI|Custom Sight")
	void ReduceVisionTemporaily(float NewPeripheralAngle, float NewSightRadius);

	UFUNCTION(BlueprintCallable, Category = "AI|Custom Sight")
	void RestoreVisionToNormal();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Custom Sight")
	bool bDrawDebugSight = false;


protected:
	UPROPERTY(BlueprintReadOnly, Category = "AI Logic")
	bool bStartNow = false;

	//Sight Config Stuff

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	void InitSightDefaults();

	void DrawDebugSight();

	//Perception Component and Sight Config

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComp;

	UPROPERTY(Transient)
	TObjectPtr<UAISenseConfig_Sight> SightConfig;

	UPROPERTY(Transient)
	float DefaultPeripheralAngleDeg = 90.f;

	UPROPERTY(Transient)
	float DefaultSightRadius = 1000.f;

	UPROPERTY(Transient)
	float DefaultLoseSightRadius = 1200.f;

	UPROPERTY(Transient)
	bool bDefaultsInitialized = false;	

	//Blueprint implemetable events	

	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent, Category = "AI Logic")
	void LastTimeSawPlayer();

};
